/* $Id: tess_winding.c,v 1.11 1999/11/11 03:21:43 kendallb Exp $ */

/*
 * Mesa 3-D graphics library
 * Version:  3.1
 *
 * Copyright (C) 1999  Brian Paul   All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*****************************************************************************
 *
 * GLU 1.3 Polygon Tessellation - Implementation of line segment intersection
 *  handling (both simple and TODO: Balaban's algorithm) and edge labelling
 *  for winding rule operations on contours
 *
 * Gareth Hughes <garethh@bell-labs.com>, August 1999
 *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glu.h>

#include "gluP.h"

#include "tess.h"
#include "tess_macros.h"
#include "tess_hash.h"
#include "tess_heap.h"
#include "tess_winding.h"


/*****************************************************************************
 * Internal type definitions:
 *****************************************************************************/

typedef struct
{
    tess_vertex_t	*start;
    tess_vertex_t	*end;
    tess_contour_t	*contour;
} tess_edge_t;

typedef struct
{
    tess_vertex_t	*edge;
    heap_t		*vertices;
} edge_vertex_list_t;

typedef struct
{
    GLdouble		angle;
    GLuint		flags;
    tess_vertex_t	*vertex;
} cv_desc_t;


#define CVD_PREVIOUS		0x01
#define CVD_NEXT		0x02
#define CVD_OUTER_REGION	0x10

#define EVL_INSERT(t, k, p)	hashtable_insert( (t), ((int) k), ((void *) p) )
#define EVL_ELEMENT(t, k)	(edge_vertex_list_t *) hashtable_element( (t), ((int) k) )

#define CVL_INSERT(t, c)	hashtable_insert( (t), ((int) c), ((void *) c) )
#define CVL_DELETE(t, c)	hashtable_delete( (t), ((int) c) )
#define CVL_ELEMENT(t, c)	(tess_vertex_t *) hashtable_element( (t), ((int) c) )

#define CVCL_INSERT(t, v, c)	hashtable_insert( (t), (v), ((void *) c) )
#define CVCL_ELEMENT(t, v)	(heap_t *) hashtable_element( (t), (v) )

#define HEAP_CVD(d)		((cv_desc_t *) d)


/*****************************************************************************
 * Labels for edge/contour intersections:
 *****************************************************************************/
#define INSIDE		1
#define OUTSIDE		2
#define SHARED1		3
#define SHARED2		4
#define ISECTED		5

#define FORWARD		1
#define BACKWARD	2


/*****************************************************************************
 * Internal function prototypes:
 *****************************************************************************/

static void extract_intersected_contours( GLUtesselator *tobj );
static void label_contours( GLUtesselator *tobj );
static void collect_contours( GLUtesselator *tobj );

static GLenum calc_intersections_simple( GLUtesselator *tobj );
static void intersect_segments( GLUtesselator *tobj,
				hashtable_t *edge_vertices,
				tess_edge_t *e1, tess_edge_t *e2 );

static void project_vertex( tess_contour_t *contour, tess_vertex_t *vertex );
static void add_new_vertices( GLUtesselator *tobj, hashtable_t *table );

static void tess_combine_callback( GLUtesselator *tobj, GLdouble coords[3],
				   void *vertex_data[4], GLfloat weight[4],
				   void **out_data );

extern GLdouble point_line_test( GLdouble u[2], GLdouble v[2], GLdouble p[2] );
GLboolean interior_point_test( tess_vertex_t *cross, tess_vertex_t *point );
GLboolean point_contour_test( tess_contour_t *contour, tess_vertex_t *point );

extern void delete_contour( tess_contour_t **contour );
static void cleanup( GLUtesselator *tobj );



/*****************************************************************************
 *
 *			LINE SEGMENT INTERSECTION FUNCTIONS
 *
 *****************************************************************************/


/*****************************************************************************
 * calc_intersections
 *
 * I haven't implemented Balaban's algorithm, which is significantly faster
 *  than the simple approach, but also significantly more complex...
 * Calculate the intersection points between all the contours and add
 *  new vertices as appropriate.
 *****************************************************************************/
GLenum tess_calc_intersections( GLUtesselator *tobj )
{
    return calc_intersections_simple( tobj );
}


/*****************************************************************************
 * calc_intersections_simple
 *
 * Use a simple O(n^2) approach to calculate the edge intersections, ie.
 *  test each edge against all others.
 *****************************************************************************/
static GLenum calc_intersections_simple( GLUtesselator *tobj )
{
    tess_contour_t	*contour = tobj->contours;
    tess_vertex_t	*vertex;
    tess_edge_t		*edges;
    hashtable_t		*edge_vertices;
    GLuint		i = 0, j, k;

    MSG( 1, "    -> calc_intersections_simple( tobj:%p )\n", tobj );

    edges  = (tess_edge_t *)
	malloc( tobj->vertex_count * sizeof(tess_edge_t) );
    if ( edges == NULL ) {
	tess_error_callback( tobj, GLU_OUT_OF_MEMORY );
	return GLU_ERROR;
    }

    edge_vertices = hashtable_init( HT_DEFAULT_SIZE );

    /* Build up a list of all the edges. */
    for ( j = 0; j < tobj->contour_count; j++ )
    {
	vertex = contour->vertices;

	for ( k = 0; k < contour->vertex_count; k++ )
	{
	    edges[ i ].start = vertex;
	    edges[ i ].end = vertex->next;
	    edges[ i ].contour = contour;

	    vertex = vertex->next; i++;
	}

	contour = contour->next;
    }

    /* Do a simple test between all the edges... */
    for ( i = 0; i < tobj->vertex_count; i++ )
    {
	for ( j = i + 1; j < tobj->vertex_count; j++ )
	{
	    intersect_segments( tobj, edge_vertices,
				&edges[ i ], &edges[ j ] );
	}
    }

    /* Only add new elements and recount vertices if we found any
       intersections. */
    if ( edge_vertices->nelements > 0 )
    {
	add_new_vertices( tobj, edge_vertices );

	/* Re-calculate the contour vertex counts. */
	MSG( 1, "         global count was: %d\n", tobj->vertex_count );
	tobj->vertex_count = 0;

	for ( contour = tobj->contours, i = 0; i < tobj->contour_count; i++ )
	{
	    vertex = contour->vertices;

	    MSG( 1, "           count was: %d\n", contour->vertex_count );
	    contour->vertex_count = 0;

	    while ( vertex != contour->last_vertex )
	    {
		contour->vertex_count++;
		tobj->vertex_count++;

		vertex = vertex->next;
	    }

	    /* Handle any intersections between last -> first vertices. */
	    while ( vertex->next != contour->vertices )
	    {
		contour->vertex_count++;
		tobj->vertex_count++;

		vertex = vertex->next;
	    }

	    /* Save the new last vertex. */
	    contour->last_vertex = vertex;

	    /* Count the final vertex in the contour. */
	    contour->vertex_count++;
	    tobj->vertex_count++;

	    MSG( 1, "           count is: %d\n", contour->vertex_count );
	    contour = contour->next;
	}
	MSG( 1, "         global count is: %d\n", tobj->vertex_count );
    }

    if ( edges ) {
	free( edges );
    }

    if ( edge_vertices )
    {
	if ( edge_vertices->elements ) {
	    free( edge_vertices->elements );
	}
	free( edge_vertices );
    }

    MSG( 1, "    <- calc_intersections_simple( tobj:%p )\n", tobj );
    return GLU_NO_ERROR;
}


/*****************************************************************************
 * intersect_segments
 *
 * Calculate the intersection points of the given line segments.  Taken from
 *  the comp.graphics.algorithms FAQ (1.03).
 *****************************************************************************/
static void intersect_segments( GLUtesselator *tobj,
				hashtable_t *edge_vertices,
				tess_edge_t *e1, tess_edge_t *e2 )
{
    GLdouble	*a = e1->start->v, *b = e1->end->v;
    GLdouble	*c = e2->start->v, *d = e2->end->v;

    /*
     * Calculate the values required for the intersection test.  Taken
     *  directly from the comp.graphics.algorithms FAQ.
     */
    GLdouble	denom =
	( ( b[X] - a[X] ) * ( d[Y] - c[Y] ) ) -
	( ( b[Y] - a[Y] ) * ( d[X] - c[X] ) );

    GLdouble	r =
	( ( a[Y] - c[Y] ) * ( d[X] - c[X] ) ) -
	( ( a[X] - c[X] ) * ( d[Y] - c[Y] ) );

    GLdouble	s =
	( ( a[Y] - c[Y] ) * ( b[X] - a[X] ) ) -
	( ( a[X] - c[X] ) * ( b[Y] - a[Y] ) );

    /*
     * A denominator of zero means that the two line segments are parallel.
     *  Again, we use our epsilon value to prevent rounding errors.
     */
    if ( ABSD( denom ) > EQUAL_EPSILON )
    {
	r = r / denom;
	s = s / denom;

	/*
	 * FIXME: We want to exclude the intersections at the endpoints of
	 *  the line segments, so these comparisons become strict
	 *  inequalities.  Verify that this is the best way to do this...
	 * FIXME: What I should be saying is this - can we use our epsilon
	 *  value in here?
	 */
	if ( ( r > 0.0 ) && ( r < 1.0 ) &&
	     ( s > 0.0 ) && ( s < 1.0 ) )
	{
	    edge_vertex_list_t	*list1, *list2;
	    heap_elt_t		*elt1, *elt2;
	    tess_vertex_t	*v1, *v2;

	    /*
	     * Parameters to tessellation combine callback.
	     */
	    GLdouble	coords[3];
	    void	*vertex_data[4];
	    GLfloat	weight[4];
	    void	*data = NULL;

	    ASSIGN_4V( vertex_data,
		       e1->start->data, e1->end->data,
		       e2->start->data, e2->end->data );
	    ASSIGN_4V( weight,
		       (GLfloat) ( 1.0 - r ) * 0.5, (GLfloat) r * 0.5,
		       (GLfloat) ( 1.0 - s ) * 0.5, (GLfloat) s * 0.5 );

	    /*
	     * Calculate the actual point of intersection.  Again, taken
	     *  from the comp.graphics.alrogithms FAQ.
	     */
	    coords[X] = e1->start->coords[X] +
		r * ( e1->end->coords[X] - e1->start->coords[X] );
	    coords[Y] = e1->start->coords[Y] +
		r * ( e1->end->coords[Y] - e1->start->coords[Y] );
	    coords[Z] = e1->start->coords[Z] +
		r * ( e1->end->coords[Z] - e1->start->coords[Z] );

	    MSG( 1, "         r: %.2f s: %.2f new: (%.2f, %.2f, %.2f)\n",
		 r, s, coords[X], coords[Y], coords[Z] );

	    /* Combine the intersection into a new vertex. */
	    tess_combine_callback( tobj, coords, vertex_data, weight, &data );

	    /* Allocate the new vertices. */
	    v1 = (tess_vertex_t *) malloc( sizeof(tess_vertex_t) );
	    v2 = (tess_vertex_t *) malloc( sizeof(tess_vertex_t) );

	    v1->index = v2->index = -1;
	    v1->data = v2->data = data;

	    v1->coords[X] = v2->coords[X] = coords[X];
	    v1->coords[Y] = v2->coords[Y] = coords[Y];
	    v1->coords[Z] = v2->coords[Z] = coords[Z];

	    v1->side = v2->side = 0.0;
	    v1->label = v2->label = 0;
	    v1->mark = v2->mark = 0;

	    /* Project the new 3D vertices into our 2D tessellation space. */
	    project_vertex( tobj->contours, v1 );
	    project_vertex( tobj->contours, v2 );

	    v1->next = v1->previous = v2->next = v2->previous = NULL;

	    /* Grab the first edge's list of new vertices. */
	    if ( ( list1 = EVL_ELEMENT( edge_vertices, e1->start ) ) == NULL )
	    {
		MSG( 1, "           alloc'ing new list for 1: %p\n", e1->start );
		list1 = (edge_vertex_list_t *)
		    malloc( sizeof(edge_vertex_list_t) );

		list1->vertices = heap_init();
		list1->edge = e1->start;

		EVL_INSERT( edge_vertices, e1->start, list1 );
	    }

	    /* Grab the second edge's list of new vertices. */
	    if ( ( list2 = EVL_ELEMENT( edge_vertices, e2->start ) ) == NULL )
	    {
		MSG( 1, "           alloc'ing new list for 2: %p\n", e2->start );
		list2 = (edge_vertex_list_t *)
		    malloc( sizeof(edge_vertex_list_t) );

		list2->vertices = heap_init();
		list2->edge = e2->start;

		EVL_INSERT( edge_vertices, e2->start, list2 );
	    }

	    /*
	     * Allocate entries for the new vertices for each edge's list.
	     *  These lists are used to post-process the intersection
	     *  calculations by actually adding the vertices to the
	     *  appropriate edges.  We need to do this after all
	     *  intersections have been calculated, otherwise it will screw
	     *  with the later intersection tests...
	     */
	    elt1 = (heap_elt_t *) malloc( sizeof(heap_elt_t) );
	    elt2 = (heap_elt_t *) malloc( sizeof(heap_elt_t) );

	    elt1->index = elt2->index = 0;
	    elt1->next = elt1->previous = elt2->next = elt2->previous = NULL;

	    /*
	     * We want the new vertices to be sorted by closeness to the
	     *  start vertex of the edge.  r and s give a value between 0
	     *  and 1, where 0 is the start vertex and 1 is the end vertex,
	     *  so we give preference in our maximum-weight priority queue
	     *  by reversing the values.
	     */
	    elt1->value = 1.0 - r;
	    elt2->value = 1.0 - s;

	    elt1->ptr = (void *) v1;
	    elt2->ptr = (void *) v2;

	    /* Insert the entries into the new vertex lists. */
	    heap_insert( list1->vertices, elt1 );
	    heap_insert( list2->vertices, elt2 );

	    MSG( 1, "           heap sizes: %d and %d\n",
		 list1->vertices->size, list2->vertices->size );

	    /*
	     * Add the new vertices to their corresponding contour's cross
	     *  vertex list.
	     */
	    CVL_INSERT( e1->contour->cross_vertices, v1 );
	    CVL_INSERT( e2->contour->cross_vertices, v2 );
	}
    }
    else
    {
	MSG( 1, "         segments parallel!  denom: %g r: %g s: %g\n", denom, r, s );

	if ( ABSD( r ) < EQUAL_EPSILON )
	{
	    MSG( 1 , "          and coinc: (%.1f, %.1f) -> (%.1f, %.1f), (%.1f, %.1f) -> (%.1f, %.1f)\n",
		 a[X], a[Y], b[X], b[Y], c[X], c[Y], d[X], d[Y] );
	}
    }
}


/*****************************************************************************
 * project_vertex
 *
 * Given a contour, project the 3D vertex coords into the 2D plane used for
 *  the tessellation algorithm using the x and y rotation values previously
 *  calculated.
 *****************************************************************************/
static void project_vertex( tess_contour_t *contour, tess_vertex_t *vertex )
{
    tess_plane_t	*plane = &contour->plane;
    GLdouble		proj[3], yrot[3], xrot[3];
    GLdouble		rotx = contour->rotx;
    GLdouble		roty = contour->roty;

    /* FIXME: This needs a cleanup, 'cos I'm sure it's inefficient. */

    proj[X] = vertex->coords[X] - plane->dist * plane->normal[X];
    proj[Y] = vertex->coords[Y] - plane->dist * plane->normal[Y];
    proj[Z] = vertex->coords[Z] - plane->dist * plane->normal[Z];

    yrot[X] = cos( roty ) * proj[X] - sin( roty ) * proj[Z];
    yrot[Y] = proj[Y];
    yrot[Z] = sin( roty ) * proj[X] + cos( roty ) * proj[Z];

    xrot[X] = yrot[X];
    xrot[Y] = cos( rotx ) * yrot[Y] - sin( rotx ) * yrot[Z];
    xrot[Z] = sin( rotx ) * yrot[Y] + cos( rotx ) * yrot[Z];

    vertex->v[X] = xrot[X];
    vertex->v[Y] = xrot[Y];
}


/*****************************************************************************
 * add_new_vertices
 *****************************************************************************/
static void add_new_vertices( GLUtesselator *tobj, hashtable_t *edge_vertices )
{
    GLuint	i;

    /* Make sure the table has some elements before we iterate over
       them all. */
    if ( edge_vertices->nelements == 0 ) return;

    MSG( 1, "      -> add_new_vertices( tobj:%p ) n: %d\n", tobj, edge_vertices->nelements );

    for ( i = 0; i < edge_vertices->size; i++ )
    {
	hashtable_elt_t		*table_elt = edge_vertices->elements[ i ];
	hashtable_elt_t		*next;

	MSG( 1, "           i: %d\n", i );

	while ( table_elt )
	{
	    edge_vertex_list_t	*list = (edge_vertex_list_t *) table_elt->ptr;
	    heap_elt_t		*heap_elt;
	    tess_vertex_t	*start = list->edge;
	    tess_vertex_t	*end = start->next;
	    tess_vertex_t	*previous = start;

	    MSG( 1, "             list: %p\n", list );

	    while ( ( heap_elt = heap_extract_max( list->vertices ) ) != NULL )
	    {
		tess_vertex_t	*vertex = HEAP_VERTEX( heap_elt );

		MSG( 1, "               v: %d (%.2f,%.2f)\n",
		     vertex->index, vertex->v[X], vertex->v[Y] );

		previous->next = vertex;
		vertex->previous = previous;

		previous = vertex;

		free( heap_elt );
	    }

	    /* Clean up the edge vertex list. */
	    heap_cleanup( &list->vertices );
	    free( list );

	    previous->next = end;
	    end->previous = previous;

	    /* Clean up the table while we're here. */
	    next = table_elt->next;
	    free( table_elt );
	    table_elt = next;
	}
    }

    MSG( 1, "      <- add_new_vertices( tobj:%p )\n", tobj );
}


/*****************************************************************************
 * tess_combine_callback
 *****************************************************************************/
static void tess_combine_callback( GLUtesselator *tobj, GLdouble coords[3],
				   void *vertex_data[4], GLfloat weight[4],
				   void **out_data )
{
    if ( tobj->callbacks.combineData != NULL )
    {
	( tobj->callbacks.combineData )( coords, vertex_data, weight,
					 out_data, tobj->data );
    }
    else if ( tobj->callbacks.combine != NULL )
    {
	( tobj->callbacks.combine )( coords, vertex_data, weight, out_data );
    }
}



/*****************************************************************************
 *
 *			CONTOUR PREPROCESSING FUNCTIONS
 *
 *****************************************************************************/


/*****************************************************************************
 * tess_preprocess_contours
 *****************************************************************************/
GLenum tess_preprocess_contours( GLUtesselator *tobj )
{
    extract_intersected_contours( tobj );

    label_contours( tobj );

    collect_contours( tobj );

    cleanup( tobj );

    return GLU_NO_ERROR;
}


/*****************************************************************************
 * extract_intersected_contours
 *
 * Extract the correct output set of contours from any intersected contours.
 *****************************************************************************/
static void add_descriptors( GLUtesselator *tobj );
static void label_edges( GLUtesselator *tobj, tess_contour_t *contour );
static GLboolean edge_rule( GLUtesselator *tobj,
			    tess_vertex_t *vertex, GLuint *dir );
static tess_contour_t *collect( GLUtesselator *tobj,
				tess_vertex_t *vertex, GLuint dir );

static void extract_intersected_contours( GLUtesselator *tobj )
{
    tess_contour_t	*contour = tobj->contours;
    tess_contour_t	*contours = NULL, *last_contour = NULL;
    GLuint		i, contour_count = 0;

    MSG( 1, "    -> extract_intersected_contours( tobj:%p )\n", tobj );

    add_descriptors( tobj );

    for ( i = 0 ; i < tobj->contour_count ; i++ )
    {
	if ( contour->label == ISECTED )
	{
	    /* Label the individual edges of any intersected contours. */
	    label_edges( tobj, contour );
	}

	contour = contour->next;
    }

    contour = tobj->contours;

    for ( i = 0 ; i < tobj->contour_count ; i++ )
    {
	tess_vertex_t	*vertex = contour->vertices;
	GLuint		j, dir = 0;

	for ( j = 0; j < contour->vertex_count; j++ )
	{
	    if ( edge_rule( tobj, vertex, &dir ) && ! vertex->mark )
	    {
		tess_contour_t	*new_contour;

		MSG( 1, "             including (%d -> %d)\n",
		     vertex->index, vertex->next->index );

		if ( dir == FORWARD )
		{
		    new_contour = collect( tobj, vertex, dir );
		}
		else
		{
		    new_contour = collect( tobj, vertex->next, dir );
		}

		MSG( 1, "               new: %p count: %d\n",
		     new_contour, new_contour->vertex_count );

		/* Add the new contour to the new list of contours. */
		if ( contour_count == 0 )
		{
		    new_contour->next = new_contour->previous = NULL;

		    contours = last_contour = new_contour;
		}
		else
		{
		    last_contour->next = new_contour;
		    new_contour->previous = last_contour;
		    new_contour->next = NULL;

		    last_contour = new_contour;
		}

		contour_count++;
	    }

	    vertex = vertex->next;
	}

	contour = contour->next;
    }

    /*
     * FIXME: Do this a little better...
     */
    if ( contour_count > 0 )
    {
	tobj->contours = contours;
	tobj->last_contour = last_contour;
	tobj->contour_count = contour_count;
    }

    MSG( 1, "    <- extract_intersected_contours( tobj:%p ) nc: %d\n", tobj, contour_count );
}


/*****************************************************************************
 * add_descriptors
 *****************************************************************************/
static heap_t *get_cvc_list( hashtable_t *cvc_lists, tess_vertex_t *vertex );
static heap_elt_t *add_cv_descriptor( heap_t *heap, cv_desc_t *desc );
static int compare_edge_angles( const void *e1, const void *e2 );

static void add_descriptors( GLUtesselator *tobj )
{
    tess_contour_t	*contour = tobj->contours;
    GLuint		i;

    MSG( 1, "      -> add_descriptors( tobj:%p )\n", tobj );

    tobj->cvc_lists = hashtable_init( HT_DEFAULT_SIZE );

    for ( i = 0; i < tobj->contour_count; i++ )
    {
	if ( contour->cross_vertices->nelements > 0 )
	{
	    hashtable_t	*table = contour->cross_vertices;
	    GLuint	j;

	    /* Mark the contour as intersected. */
	    contour->label = ISECTED;

	    for ( j = 0; j < table->size; j++ )
	    {
		hashtable_elt_t	*elt = table->elements[ j ];

		while ( elt )
		{
		    tess_vertex_t	*vertex = HASH_VERTEX( elt );

		    heap_t	*cvc_list =
			get_cvc_list( tobj->cvc_lists, vertex );

		    cv_desc_t	*desc1 = (cv_desc_t *)
			malloc( sizeof(cv_desc_t) );
		    cv_desc_t	*desc2 = (cv_desc_t *)
			malloc( sizeof(cv_desc_t) );

		    GLdouble	e1[2], e2[2];

		    /* Add the previous descriptor. */
		    SUB_2V( e1, vertex->previous->v, vertex->v );
		    NORMALIZE_2DV( e1 );

		    desc1->angle = ( e1[Y] >= 0 ) ?
			RAD_TO_DEG( acos( e1[X] ) ) :
			RAD_TO_DEG( acos( e1[X] ) ) + 180.0;
		    desc1->flags = CVD_PREVIOUS;
		    desc1->vertex = vertex;

		    /* Add the next descriptor. */
		    SUB_2V( e2, vertex->next->v, vertex->v );
		    NORMALIZE_2DV( e2 );

		    desc2->angle = ( e2[Y] >= 0 ) ?
			RAD_TO_DEG( acos( e2[X] ) ) :
			RAD_TO_DEG( acos( e2[X] ) ) + 180.0;
		    desc2->flags = CVD_NEXT;
		    desc2->vertex = vertex;

		    /*
		     * Flag the connectivity list and descriptors if the
		     *  list contains vertices from the outer region.
		     */
		    if ( i == 0 )
		    {
			cvc_list->flags = 1;
			desc1->flags |= CVD_OUTER_REGION;
			desc2->flags |= CVD_OUTER_REGION;
		    }

		    add_cv_descriptor( cvc_list, desc1 );
		    add_cv_descriptor( cvc_list, desc2 );

		    MSG( 1, "           desc: %d angle: %-6.2f flags: %x n: %d\n", vertex->index, desc1->angle, desc1->flags, cvc_list->size );
		    MSG( 1, "           desc: %d angle: %-6.2f flags: %x n: %d\n", vertex->index, desc2->angle, desc2->flags, cvc_list->size );

		    elt = elt->next;
		}
	    }
	}

	contour = contour->next;
    }

    /*
     * Our cross vertex connectivity lists aren't really heaps, they should
     *  be sorted lists.  So, let's sort each list by edge angle now.
     */
    if ( tobj->cvc_lists )
    {
	hashtable_t	*table = tobj->cvc_lists;
	GLuint		i;

	for ( i = 0; i < table->size; i++ )
	{
	    hashtable_elt_t	*elt = table->elements[ i ];

	    while ( elt )
	    {
		heap_t	*heap = (heap_t *) elt->ptr;

		/* Sort the contours by their area. */
		qsort( heap->elements, heap->size,
		       sizeof(cv_desc_t *), compare_edge_angles );

		elt = elt->next;
	    }
	}
    }

    MSG( 1, "      <- add_descriptors( tobj:%p )\n", tobj );
}

/*****************************************************************************
 * get_cvc_list
 *
 * Get the cross vertex connectivity list, from the global set, for the
 *  given vertex.  If we need to, allocate and initialize a new one.
 *****************************************************************************/
static heap_t *get_cvc_list( hashtable_t *cvc_lists, tess_vertex_t *vertex )
{
    heap_t	*heap;

    if ( ( heap = CVCL_ELEMENT( cvc_lists, vertex->index ) ) == NULL )
    {
	heap = heap_init();

	CVCL_INSERT( cvc_lists, vertex->index, heap );
    }

    return heap;
}

/*****************************************************************************
 * add_cv_descriptor
 *****************************************************************************/
static heap_elt_t *add_cv_descriptor( heap_t *heap, cv_desc_t *desc )
{
    heap_elt_t	*elt;

    elt = (heap_elt_t *) malloc( sizeof(heap_elt_t) );
    if ( elt == NULL ) {
	return NULL;
    }

    /* We want our descriptor list to be increasing in angle size. */
    elt->value = - desc->angle;
    elt->ptr = (void *) desc;
    elt->index = 0;

    elt->next = NULL;
    elt->previous = NULL;

    heap_insert( heap, elt );

    return elt;
}

/*****************************************************************************
 * compare_edge_angles
 *****************************************************************************/
static int compare_edge_angles( const void *e1, const void *e2 )
{
    cv_desc_t	*desc1, *desc2;

    desc1 = HEAP_CVD( (*((heap_elt_t **) e1))->ptr );
    desc2 = HEAP_CVD( (*((heap_elt_t **) e2))->ptr );

    if ( desc1->angle < desc2->angle ) {
	return -1;
    }
    if ( desc1->angle > desc2->angle ) {
	return 1;
    }
    return 0;
}


/*****************************************************************************
 * label_edges
 *****************************************************************************/
static GLboolean edge_on_contour( tess_contour_t *contour,
				  tess_vertex_t *start, tess_vertex_t *end );
static void label_no_intersections( GLUtesselator *tobj,
				    tess_contour_t *contour,
				    tess_vertex_t *vertex );
static GLboolean label_using_cvc_list( GLUtesselator *tobj, heap_t *cvc_list,
				       tess_vertex_t *vertex,
				       tess_vertex_t *point );

static void label_edges( GLUtesselator *tobj, tess_contour_t *contour )
{
    tess_vertex_t	*vertex = contour->vertices;
    GLuint		i;

    MSG( 1, "      -> label_edges( tobj:%p c:%p )\n", tobj, contour );

    tobj->label = 0;

    for ( i = 0; i < contour->vertex_count; i++ )
    {
	if ( ! CVL_ELEMENT( contour->cross_vertices, vertex ) &&
	     ! CVL_ELEMENT( contour->cross_vertices, vertex->next ) )
	{
	    MSG( 1, "           no cross vertices (%d -> %d)\n",
		 vertex->index, vertex->next->index );

	    label_no_intersections( tobj, contour, vertex );
	}
	else
	{
	    MSG( 1, "           cross vertices (%d -> %d)\n",
		 vertex->index, vertex->next->index );

	    if ( ( contour != tobj->contours ) &&
		 edge_on_contour( tobj->contours, vertex, vertex->next ) )
	    {
		/*
		 * Edge is shared with the outer region, and is thus
		 *  marked as SHARED1.
		 */
		MSG( 1, "             vertex SHARED1\n" );
		vertex->label = tobj->label = SHARED1;
	    }
	    else if ( ( contour != tobj->contours ) &&
		      edge_on_contour( tobj->contours, vertex->next, vertex ) )
	    {
		/*
		 * Edge is shared with the outer region, but in reverse,
		 *  and is thus marked as SHARED2.
		 */
		MSG( 1, "             vertex SHARED2\n" );
		vertex->label = tobj->label = SHARED2;
	    }
	    else
	    {
		heap_t	*cvc_list1 =
		    get_cvc_list( tobj->cvc_lists, vertex );
		heap_t	*cvc_list2 =
		    get_cvc_list( tobj->cvc_lists, vertex->next );

		/*
		 * We need to test the cross vertex connectivity lists
		 *  to see if they contain vertices from the outer region.
		 */
		if ( ( cvc_list1 && cvc_list1->flags ) ||
		     ( cvc_list2 && cvc_list2->flags ) )
		{
		    MSG( 1, "             testing vertex...\n" );

		    vertex->label = tobj->label = OUTSIDE;

		    if ( label_using_cvc_list( tobj, cvc_list1,
					       vertex, vertex->next ) ||
			 label_using_cvc_list( tobj, cvc_list2,
					       vertex, vertex->previous ) )
		    {
			MSG( 1, "             vertex INSIDE\n" );
		    }
		    else
		    {
			MSG( 1, "             vertex OUTSIDE\n" );
		    }
		}
		else
		{
		    /*
		     * The edge is labelled the same as if it had no cross
		     *  vertices as end points.
		     */
		    label_no_intersections( tobj, contour, vertex );
		}
	    }

	    MSG( 1, "\n" );
	}

	vertex = vertex->next;
    }

    MSG( 1, "      <- label_edges( tobj:%p c:%p )\n", tobj, contour );
}

/*****************************************************************************
 * edge_on_contour
 *
 * Test if the given edge lies on the given contour.  We use start and end
 *  vertices to allow testing if the edge lies on the contour in reverse.
 *****************************************************************************/
static GLboolean edge_on_contour( tess_contour_t *contour,
				  tess_vertex_t *start, tess_vertex_t *end )
{
    tess_vertex_t	*vertex = contour->vertices;
    GLuint		i;

    for ( i = 0; i < contour->vertex_count; i++ )
    {
	if ( ( vertex == start ) && ( vertex->next == end ) ) {
	    return GL_TRUE;
	}
	vertex = vertex->next;
    }
    return GL_FALSE;
}

/*****************************************************************************
 * label_no_intersections
 *
 * Label the given edge if the contour contains no cross vertices.
 *****************************************************************************/
static void label_no_intersections( GLUtesselator *tobj,
				    tess_contour_t *contour,
				    tess_vertex_t *vertex )
{
    if ( contour == tobj->contours )
    {
	if ( INSIDE_BBOX_2V( vertex->v, tobj->contours->mins,
			     tobj->contours->maxs ) )
	{
	    MSG( 1, "             vertex INSIDE\n" );
	    vertex->label = tobj->label = INSIDE;
	}
	else
	{
	    MSG( 1, "             vertex OUTSIDE\n" );
	    vertex->label = tobj->label = OUTSIDE;
	}
    }
    else
    {
	MSG( 1, "             vertex as previous: %d\n", tobj->label );
	vertex->label = tobj->label;
    }
}

/*****************************************************************************
 * label_using_cvc_list
 *
 * As the cross vertex connectivity lists for one or more of the endpoints of
 *  the current edge contain vertices from the outer region, we must determine
 *  if the edge lies "inside" one of the angles at the outer region vertex.
 *****************************************************************************/
static GLboolean label_using_cvc_list( GLUtesselator *tobj, heap_t *cvc_list,
				       tess_vertex_t *vertex,
				       tess_vertex_t *point )
{
    GLuint	i;

    /* Ensure we're given a valid cross vertex connectivity list. */
    if ( ! cvc_list ) return GL_FALSE;

    for ( i = 0; i < cvc_list->size; i++ )
    {
	heap_elt_t	*cvc_elt = cvc_list->elements[ i ];
	cv_desc_t	*desc = HEAP_CVD( cvc_elt->ptr );

	MSG( 1, "               cmp (%d, %d, %d) flags: 0x%x labels: 0%x 0%x\n",
	     desc->vertex->previous->index, desc->vertex->index,
	     desc->vertex->next->index, desc->flags, desc->vertex->previous->label, desc->vertex->next->label );

	/* Make sure we don't compare the current edge with itself. */
	if ( ( vertex == desc->vertex->previous ) ||
	     ( vertex == desc->vertex ) )
	{
	    MSG( 1, "                 ignoring, same edge\n" );
	    continue;
	}

	/*
	 * We only need to test one of the descriptors (there are two, one
	 *  for the previous edge and one for the next edge).  So, we test
	 *  the first descriptor only if it lies on the outer region.  If
	 *  we find that the test point lies inside the angle, we can label
	 *  the edge and return.
	 */
	if ( ( desc->flags & CVD_PREVIOUS ) &&
	     ( desc->flags & CVD_OUTER_REGION ) &&
	     ( interior_point_test( desc->vertex, point ) ) )
	{
	    vertex->label = tobj->label = INSIDE;

	    return GL_TRUE;
	}
    }

    return GL_FALSE;
}


/*****************************************************************************
 * edge_rule
 *
 * Determine if the given edge should be included in a resulting contour,
 *  and if so return the direction it should be included in.
 *****************************************************************************/
static GLboolean edge_rule( GLUtesselator *tobj,
			    tess_vertex_t *vertex, GLuint *dir )
{
    switch ( tobj->winding_rule )
    {
    case GLU_TESS_WINDING_ODD:
	if ( vertex->label == OUTSIDE )
	{
	    *dir = FORWARD;
	    return GL_TRUE;
	}
	else if ( vertex->label == INSIDE )
	{
	    *dir = BACKWARD;
	    return GL_TRUE;
	}
	break;

    case GLU_TESS_WINDING_NONZERO:
	/*
	 * FIXME: Fill this in...
	 */
	break;

    case GLU_TESS_WINDING_POSITIVE:
	if ( ( vertex->label == OUTSIDE ) || ( vertex->label == SHARED1 ) )
	{
	    *dir = FORWARD;
	    return GL_TRUE;
	}
	break;

    case GLU_TESS_WINDING_NEGATIVE:
	/*
	 * FIXME: Fill this in...
	 */
	break;

    case GLU_TESS_WINDING_ABS_GEQ_TWO:
	/*
	 * FIXME: Fill this in...
	 */
	break;

    default:
	break;
    }

    return GL_FALSE;
}

/*****************************************************************************
 * collect
 *
 * Actually perform the collection of vertices into the new contours.
 *****************************************************************************/
static void jump( GLUtesselator *tobj, tess_vertex_t **vertex, GLuint *dir );

static tess_contour_t *collect( GLUtesselator *tobj,
				tess_vertex_t *vertex, GLuint dir )
{
    tess_contour_t	*contour;
    tess_vertex_t	*current = vertex;

    MSG( 1, "      -> collect( tobj:%p v:%d )\n", tobj, vertex->index );

    /* Allocate the new contour. */
    contour = (tess_contour_t *) malloc( sizeof(tess_contour_t) );
    if ( contour == NULL ) {
	tess_error_callback( tobj, GLU_OUT_OF_MEMORY );
	return NULL;
    }

    COPY_3V( contour->plane.normal, tobj->plane.normal );
    contour->plane.dist = tobj->plane.dist;

    contour->vertex_count = 0;
    contour->vertices = contour->last_vertex = NULL;

    contour->reflex_vertices = NULL;
    contour->cross_vertices = NULL;

    contour->orientation = GLU_CCW;
    contour->area = 0.0;

    contour->label = 0;
    CLEAR_BBOX_2DV( contour->mins, contour->maxs );

    contour->rotx = contour->roty = 0.0;

    /* Add vertices to the new contour. */
    do
    {
	heap_t		*cvc_list;
	tess_vertex_t	*new_vertex = (tess_vertex_t *)
	    malloc( sizeof(tess_vertex_t) );

	MSG( 1, "           *** including (%d -> %d) dir: %s\n",
	     current->index, ( dir == FORWARD ) ?
	     current->next->index : current->previous->index,
	     ( dir == FORWARD ) ? "FORWARD" : "BACKWARD" );
	current->mark |= dir;

	/* Add the new vertex to the new contour. */
	MEMCPY( new_vertex, current, sizeof(tess_vertex_t) );

	if ( contour->vertex_count == 0 )
	{
	    new_vertex->next = new_vertex->previous = NULL;

	    contour->vertices = contour->last_vertex = new_vertex;
	}
	else
	{
	    contour->last_vertex->next = new_vertex;
	    new_vertex->previous = contour->last_vertex;
	    new_vertex->next = NULL;

	    contour->last_vertex = new_vertex;
	}
	contour->vertex_count++;


	/* Move on to the next vertex. */
	current = ( dir == FORWARD ) ? current->next : current->previous;

	if ( ( current->label == SHARED1 ) || ( current->label == SHARED2 ) )
	{
	    heap_t	*cvc_list = get_cvc_list( tobj->cvc_lists, current );
	    GLuint	i;

	    /*
	     * Mark all edges that are shared with the current one as
	     *  included in a contour.
	     */
	    for ( i = 0; i < cvc_list->size; i++ )
	    {
		cv_desc_t	*desc =
		    HEAP_CVD( cvc_list->elements[ i ]->ptr );

		desc->vertex->mark |= dir;
	    }
	}

	/*
	 * If the current vertex is a cross vertex, we need to work out
	 *  which edge to traverse next.
	 */
	cvc_list = CVCL_ELEMENT( tobj->cvc_lists, current->index );

	if ( cvc_list && ( cvc_list->size > 0 ) )
	{
	    jump( tobj, &current, &dir );
	}

	MSG( 1, "           dir: %d current: %d mark: %d\n", dir, current->index, current->mark );

    } while ( ! ( current->mark & dir ) );

    contour->last_vertex->next = contour->vertices;
    contour->vertices->previous = contour->last_vertex;

    MSG( 1, "      <- collect( tobj:%p v:%d )\n", tobj, vertex->index );
    return contour;
}

/*****************************************************************************
 * jump
 *
 * When we reach a cross vertex, calculate which edge to take next.
 *
 * FIXME: This needs fixing for self-intersecting contours.
 *****************************************************************************/
static cv_desc_t *get_descriptor( GLUtesselator *tobj,
				  tess_vertex_t *vertex, GLuint flags );
static cv_desc_t *previous_descriptor( GLUtesselator *tobj, cv_desc_t *desc );

static void jump( GLUtesselator *tobj, tess_vertex_t **vertex, GLuint *dir )
{
    cv_desc_t	*desc = NULL;
    GLuint	i = 0;

    MSG( 1, "        -> jump( tobj:%p v:%d dir: %s)\n", tobj, (*vertex)->index, ( *dir == FORWARD ) ? "F" : "B");

    desc = ( *dir == FORWARD ) ?
	previous_descriptor( tobj, get_descriptor( tobj, *vertex,
						   CVD_PREVIOUS ) ) :
	previous_descriptor( tobj, get_descriptor( tobj, *vertex,
						   CVD_NEXT ) );

    MSG( 1, "             desc: %p angle: %-6.2f v: (%d, %d, %d)\n", desc, desc->angle, desc->vertex->previous->index, desc->vertex->index, desc->vertex->next->index );

    while ( i++ < 10 )
    {
	tess_vertex_t	*edge = ( desc->flags & CVD_PREVIOUS ) ?
	    desc->vertex->previous : desc->vertex;
	GLuint		new_dir;

	if ( ! edge->mark && edge_rule( tobj, edge, &new_dir ) )
	{
	    *vertex = desc->vertex;

	    MSG( 1, "               v: (%d, %d, %d) f: 0x%x e: %d dir: %s new: %s\n", (*vertex)->previous->index, (*vertex)->index, (*vertex)->next->index, desc->flags, edge->index, ( *dir == FORWARD ) ? "F" : "B", ( new_dir == FORWARD ) ? "F" : "B" );

	    if ( ( ( edge == (*vertex) ) &&
		   ( new_dir == FORWARD ) ) ||
		 ( ( edge == (*vertex)->previous ) &&
		   ( new_dir == BACKWARD ) ) )
	    {
		*dir = new_dir;
		break;
	    }
	}

	desc = previous_descriptor( tobj, desc );
    }

    if ( i >= 10 ) { exit( 1 ); }

    MSG( 1, "        <- jump( tobj:%p v:%d )\n", tobj, (*vertex)->index );
}

/*****************************************************************************
 * get_descriptor
 *
 * Return the descriptor (with the given flags) for the given vertex.
 *****************************************************************************/
static cv_desc_t *get_descriptor( GLUtesselator *tobj,
				  tess_vertex_t *vertex, GLuint flags )
{
    heap_t	*cvc_list = get_cvc_list( tobj->cvc_lists, vertex );
    GLuint	i;

    /* Find the descriptor for the given vertex. */
    for ( i = 0; i < cvc_list->size; i++ )
    {
	heap_elt_t	*elt = cvc_list->elements[ i ];
	cv_desc_t	*desc = HEAP_CVD( elt->ptr );

	/* Make sure we pick the descriptor with the given flags. */
	if ( ( desc->flags & flags ) && ( desc->vertex == vertex ) )
	{
	    return desc;
	}
    }

    return NULL;
}

/*****************************************************************************
 * previous_descriptor
 *
 * Return the descriptor nearest to the given descriptor in the clockwise
 *  direction.
 *****************************************************************************/
static cv_desc_t *previous_descriptor( GLUtesselator *tobj, cv_desc_t *desc )
{
    heap_t	*cvc_list = get_cvc_list( tobj->cvc_lists, desc->vertex );
    cv_desc_t	*ret;
    GLuint	i, current = 0;

    /* Find the descriptor for the given vertex. */
    for ( i = 0; i < cvc_list->size; i++ )
    {
	heap_elt_t	*elt = cvc_list->elements[ i ];
	cv_desc_t	*d = HEAP_CVD( elt->ptr );

	if ( d == desc )
	{
	    current = i;
	    break;
	}
    }

    ret = ( current > 0 ) ?
	HEAP_CVD( cvc_list->elements[ current - 1 ]->ptr ) :
	HEAP_CVD( cvc_list->elements[ cvc_list->size - 1 ]->ptr );

    return ret;
}


/*****************************************************************************
 * label_contours
 *
 * Label a set of non-intersecting contours as INSIDE or OUTSIDE.
 *****************************************************************************/
static void reverse_contour( tess_contour_t *contour );

static void label_contours( GLUtesselator *tobj )
{
    tess_contour_t	*contour = tobj->contours;
    GLuint		i, j;

    MSG( 1, "    -> label_contours( tobj:%p c: %p )\n", tobj, contour );

    for ( i = 0 ; i < tobj->contour_count ; i++ )
    {
	/*
	 * As our contours have been sorted by size, we can use the first
	 *  contour as our outer region.  Thus, if a contour has no cross
	 *  vertices we use the first contour's bounding box to determine
	 *  if the contour should be labelled INSIDE or OUTSIDE.
	 */
	if ( ! contour->cross_vertices ||
	     ( contour->cross_vertices->nelements == 0 ) )
	{
	    tess_contour_t	*current = tobj->contours;
	    GLint		n =
		( tobj->contours->orientation == GLU_CCW ) ? 1 : -1;

	    contour->label = OUTSIDE;

	    for ( j = 0 ; j < tobj->contour_count ; j++ )
	    {
		if ( ( current != contour ) && 
		     point_contour_test( current, contour->vertices ) )
		{
		    contour->label = INSIDE;
		    n = ( contour->orientation == GLU_CCW ) ? n + 1 : n - 1;
		}

		current = current->next;
	    }

	    contour->winding = n;

	    MSG( 1, "           contour %s w: %d orient: %s\n", ( contour->label == OUTSIDE ) ? "OUTSIDE" : "INSIDE ", contour->winding, ( contour->orientation == GLU_CCW ) ? "CCW" : "CW" );
	}
	else
	{
	    /* FIXME: This should never happen... */
	    tess_error_callback( tobj, GLU_TESS_ERROR5 );
	    return;
	}

	hashtable_cleanup( &contour->cross_vertices );

	contour = contour->next;
    }

    MSG( 1, "    <- label_contours( tobj:%p )\n", tobj );
}

/*****************************************************************************
 * reverse_contour
 *****************************************************************************/
static void reverse_contour( tess_contour_t *contour )
{
    tess_vertex_t	*current = contour->vertices;
    GLuint		i;

    for ( i = 0 ; i < contour->vertex_count ; i++ )
    {
	tess_vertex_t	*next = current->next;
	tess_vertex_t	*previous = current->previous;

	current->next = previous;
	current->previous = next;

	current = next;
    }

    contour->last_vertex = contour->vertices->previous;
}


/*****************************************************************************
 * interior_point_test
 *
 * Determine if the given point is inside the angle given by the points
 *  cross->previous, cross and cross->next.  The given point will be the start
 *  or end vertex of an edge that has the cross vertex at the other end.
 *****************************************************************************/
GLboolean interior_point_test( tess_vertex_t *cross, tess_vertex_t *point )
{
    if ( ( point_line_test( cross->previous->v, cross->v,
			    point->v ) > EQUAL_EPSILON ) &&
	 ( point_line_test( cross->v, cross->next->v,
			    point->v ) > EQUAL_EPSILON ) )
    {
	return GL_TRUE;
    } else {
	return GL_FALSE;
    }
}

/*****************************************************************************
 * point_contour_test
 *
 * Determine if the given point lies inside the given contour.  Taken from
 *  O'Rourke 1998 p244.
 *****************************************************************************/
GLboolean point_contour_test( tess_contour_t *contour, tess_vertex_t *point )
{
    tess_vertex_t	*vertex = contour->vertices;
    GLuint		rcross = 0, lcross = 0;
    GLboolean		rstrad, lstrad;
    GLdouble		x;
    GLuint		i;

    MSG( 1, "           comparing c: %d p: %d ...\n", contour->vertices->index, point->index );

    for ( i = 0 ; i < contour->vertex_count ; i++ )
    {
	/*
	 * First check if given point is coicident with a vertex on
	 *  the contour.
	 */
	if ( vertex->index == point->index ) {
	    MSG( 1, "             p: %d coincident\n", point->index );
	    return GL_FALSE;
	}

	/*
	 * Check if current edge straddles the horizontal line through
	 *  the given point, with bias above/below.
	 */
	rstrad = ( ( vertex->v[Y] > point->v[Y] ) !=
		   ( vertex->next->v[Y] > point->v[Y] ) );
	lstrad = ( ( vertex->v[Y] < point->v[Y] ) !=
		   ( vertex->next->v[Y] < point->v[Y] ) );

	if ( rstrad || lstrad )
	{
	    /*
	     * Compute intersection of current edge with the horizontal
	     *  line through the given point.
	     */
	    x = ( vertex->v[X] * vertex->next->v[Y]
		  - vertex->next->v[X] * vertex->v[Y] )
		/ (GLdouble) ( vertex->next->v[Y] - vertex->v[Y] );

	    if ( rstrad && ( x > point->v[X] ) ) rcross++;
	    if ( lstrad && ( x < point->v[X] ) ) lcross++;
	}

	vertex = vertex->next;
    }

    MSG( 1, "             rc: %d lc: %d\n", rcross, lcross );

    /*
     * Given point is on an edge if left/right cross counts are not the
     *  same parity.
     */
    if ( ( rcross % 2 ) != ( lcross % 2 ) ) {
	MSG( 1, "             p: %d lies on edge\n", point->index );
	return GL_FALSE;
    }

    /*
     * Given point
     */
    if ( ( rcross % 2 ) == 1 ) {
	MSG( 1, "             p: %d strictly inside\n", point->index );
	return GL_TRUE;
    } else {
	MSG( 1, "             p: %d strictly outside\n", point->index );
	return GL_FALSE;
    }
}


/*****************************************************************************
 * collect_contours
 *****************************************************************************/
static void remove_contour( GLUtesselator *tobj, tess_contour_t *contour );
static GLboolean edge_rule( GLUtesselator *tobj,
			    tess_vertex_t *vertex, GLuint *dir );
static tess_contour_t *collect( GLUtesselator *tobj,
				tess_vertex_t *vertex, GLuint dir );

static void collect_contours( GLUtesselator *tobj )
{
    tess_contour_t	*contour = tobj->contours;
    GLuint		i;

    MSG( 1, "    -> collect_contours( tobj:%p )\n", tobj );

#ifdef DEBUG
    switch( tobj->winding_rule )
    {
    case GLU_TESS_WINDING_ODD:
	MSG( 1, "         using ODD winding rule\n" );
	break;
    case GLU_TESS_WINDING_NONZERO:
	MSG( 1, "         using NONZERO winding rule\n" );
	break;
    case GLU_TESS_WINDING_POSITIVE:
	MSG( 1, "         using POSITIVE winding rule\n" );
	break;
    case GLU_TESS_WINDING_NEGATIVE:
	MSG( 1, "         using NEGATIVE winding rule\n" );
	break;
    case GLU_TESS_WINDING_ABS_GEQ_TWO:
	MSG( 1, "         using ABS_GEQ_TWO winding rule\n" );
	break;

    default:
	MSG( 1, "         using unknown winding rule\n" );
	break;
    }
#endif

    for ( i = 0; i < tobj->contour_count; i++ )
    {
	tess_contour_t	*next = contour->next;

#ifdef DEBUG
	switch( contour->label )
	{
	case INSIDE:
	    MSG( 1, "           contour: %d label: INSIDE  %s %d\n",
		 contour->vertices->index, ( contour->orientation == GLU_CCW ) ? "CCW" : "CW", contour->winding );
	    break;
	case OUTSIDE:
	    MSG( 1, "           contour: %d label: OUTSIDE %s %d\n",
		 contour->vertices->index, ( contour->orientation == GLU_CCW ) ? "CCW" : "CW", contour->winding );
	    break;
	case ISECTED:
	    MSG( 1, "           contour: %d label: ISECTED %s %d\n",
		 contour->vertices->index, ( contour->orientation == GLU_CCW ) ? "CCW" : "CW", contour->winding );
	    break;

	default:
	    MSG( 1, "           contour: %p unknown label\n", contour );
	    break;
	}
#endif
	switch ( tobj->winding_rule )
	{
	case GLU_TESS_WINDING_ODD:
	    if ( ( ABSI( contour->winding ) % 2 ) == 1 )
	    {
		if ( contour->orientation == GLU_CW )
		{
		    MSG( 1, "             rev CW -> CCW\n" );
		    reverse_contour( contour );
		    contour->orientation = GLU_CCW;
		}
	    }
	    else
	    {
		if ( contour->orientation == GLU_CCW )
		{
		    MSG( 1, "             rev CCW -> CW\n" );
		    reverse_contour( contour );
		    contour->orientation = GLU_CW;
		}
		else if ( tobj->contours == contour )
		{
		    MSG( 1, "             deleting contour...\n" );
		    remove_contour( tobj, contour );
		    delete_contour( &contour );
		    i--;
		}
	    }
	    break;

	case GLU_TESS_WINDING_NONZERO:
	    if ( ABSI( contour->winding ) == 1 )
	    {
		if ( contour->orientation == GLU_CW )
		{
		    MSG( 1, "             rev CW -> CCW\n" );
		    reverse_contour( contour );
		    contour->orientation = GLU_CCW;
		}
	    }
	    else if ( contour->winding == 0 )
	    {
		if ( contour->orientation == GLU_CCW )
		{
		    MSG( 1, "             rev CCW -> CW\n" );
		    reverse_contour( contour );
		    contour->orientation = GLU_CW;
		}
		else if ( tobj->contours == contour )
		{
		    MSG( 1, "             deleting contour...\n" );
		    remove_contour( tobj, contour );
		    delete_contour( &contour );
		    i--;
		}
	    }
	    else if ( ABSI( contour->winding ) > 1 )
	    {
		MSG( 1, "             deleting contour...\n" );
		remove_contour( tobj, contour );
		delete_contour( &contour );
		i--;
	    }
	    break;

	case GLU_TESS_WINDING_POSITIVE:
	    if ( contour->winding == 1 )
	    {
		if ( contour->orientation == GLU_CW )
		{
		    MSG( 1, "             rev CW -> CCW\n" );
		    reverse_contour( contour );
		    contour->orientation = GLU_CCW;
		}
	    }
	    else if ( contour->winding == 0 )
	    {
		if ( contour->orientation == GLU_CCW )
		{
		    MSG( 1, "             rev CCW -> CW\n" );
		    reverse_contour( contour );
		    contour->orientation = GLU_CW;
		}
		else if ( tobj->contours == contour )
		{
		    MSG( 1, "             deleting contour...\n" );
		    remove_contour( tobj, contour );
		    delete_contour( &contour );
		    i--;
		}
	    }
	    else if ( ( contour->winding < 0 ) ||
		      ( contour->winding > 1 ) )
	    {
		MSG( 1, "             deleting contour...\n" );
		remove_contour( tobj, contour );
		delete_contour( &contour );
		i--;
	    }
	    break;

	case GLU_TESS_WINDING_NEGATIVE:
	    if ( contour->winding == -1 )
	    {
		if ( contour->orientation == GLU_CW )
		{
		    MSG( 1, "             rev CW -> CCW\n" );
		    reverse_contour( contour );
		    contour->orientation = GLU_CCW;
		}
	    }
	    else if ( contour->winding == -2 )
	    {
		if ( contour->orientation == GLU_CCW )
		{
		    MSG( 1, "             rev CCW -> CW\n" );
		    reverse_contour( contour );
		    contour->orientation = GLU_CW;
		}
		else if ( tobj->contours == contour )
		{
		    MSG( 1, "             deleting contour...\n" );
		    remove_contour( tobj, contour );
		    delete_contour( &contour );
		    i--;
		}
	    }
	    else if ( ( contour->winding < -2 ) ||
		      ( contour->winding > -1 ) )
	    {
		MSG( 1, "             deleting contour...\n" );
		remove_contour( tobj, contour );
		delete_contour( &contour );
		i--;
	    }
	    break;

	case GLU_TESS_WINDING_ABS_GEQ_TWO:
	    if ( ABSI( contour->winding ) == 2 )
	    {
		if ( contour->orientation == GLU_CW )
		{
		    MSG( 1, "             rev CW -> CCW\n" );
		    reverse_contour( contour );
		    contour->orientation = GLU_CCW;
		}
	    }
	    else if ( ABSI( contour->winding ) == 1 )
	    {
		if ( ( contour->orientation == GLU_CCW ) &&
		     ( contour->label == INSIDE ) )
		{
		    MSG( 1, "             rev CCW -> CW\n" );
		    reverse_contour( contour );
		    contour->orientation = GLU_CW;
		}
		else if ( tobj->contours == contour )
		{
		    MSG( 1, "             deleting contour...\n" );
		    remove_contour( tobj, contour );
		    delete_contour( &contour );
		    i--;
		}
	    }
	    else if ( ( contour->winding == 0 ) ||
		      ( ABSI( contour->winding > 2 ) ) )
	    {
		MSG( 1, "             deleting contour...\n" );
		remove_contour( tobj, contour );
		delete_contour( &contour );
		i--;
	    }
	    break;

	default:
	    break;
	}

	contour = next;
    }

    MSG( 1, "    <- collect_contours( tobj:%p ) count: %d\n", tobj, tobj->contour_count );
}

/*****************************************************************************
 * remove_contour
 *****************************************************************************/
static void remove_contour( GLUtesselator *tobj, tess_contour_t *contour )
{
    if ( tobj->contours == contour ) {
	tobj->contours = contour->next;
    }
    if ( tobj->last_contour == contour ) {
	tobj->last_contour = contour->previous;
    }

    contour->previous->next = contour->next;
    contour->next->previous = contour->previous;

    tobj->contour_count--;
    tobj->vertex_count -= contour->vertex_count;
}

/*****************************************************************************
 * cleanup
 *
 * Deallocate any memory that we've used in the contour preprocessing.
 *****************************************************************************/
static void cleanup( GLUtesselator *tobj )
{
    if ( tobj->cvc_lists )
    {
	hashtable_t	*table = tobj->cvc_lists;
	GLuint		i;

	for ( i = 0; i < table->size; i++ )
	{
	    hashtable_elt_t	*elt = table->elements[ i ];

	    while ( elt )
	    {
		hashtable_elt_t	*next;
		heap_t		*heap = (heap_t *) elt->ptr;

		heap_cleanup( &heap );

		next = elt->next;
		free( elt );
		elt = next;
	    }
	}

	if ( table->elements ) {
	    free( table->elements );
	}
	free( table );

	tobj->cvc_lists = NULL;
    }
}
