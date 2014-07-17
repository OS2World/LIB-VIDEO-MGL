/* $Id: tess_fist.c,v 1.22 1999/11/11 03:21:43 kendallb Exp $ */

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
 * GLU 1.3 Polygon Tessellation - Implementation of FIST algorithm
 *
 * For more info on FIST, see:
 *  http://www.cosy.sbg.ac.at/~held/projects/triang/triang.html
 *
 * Gareth Hughes <garethh@bell-labs.com>, April 1999
 *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glu.h>

#include "gluP.h"

#include "tess.h"
#include "tess_macros.h"
#include "tess_heap.h"
#include "tess_fist.h"
#include "tess_winding.h"
#if 0
#include "tess_grid.h"
#endif


/*****************************************************************************
 * Internal function prototypes:
 *****************************************************************************/

static GLenum remove_coincident_vertices( GLUtesselator *tobj );

static GLenum compute_orientations( GLUtesselator *tobj );
static GLenum sort_vertices( GLUtesselator *tobj );
static GLenum transform_build_bridges( GLUtesselator *tobj );
static GLenum classify_angles( GLUtesselator *tobj );
static void classify_vertex( tess_contour_t *contour, tess_vertex_t *vertex );
static GLenum tessellate_contours( GLUtesselator *tobj );
static GLenum output_contours( GLUtesselator *tobj );

GLdouble point_line_test( GLdouble u[2], GLdouble v[2], GLdouble p[2] );
GLboolean point_triangle_test( tess_vertex_t *triangle, tess_vertex_t *point );
GLdouble angle_3dv( GLdouble va[2], GLdouble vb[2], GLdouble vc[2] );

static void tess_begin_callback( GLUtesselator *tobj, GLenum mode );
static void tess_vertex_callback( GLUtesselator *tobj, void *vertex_data );
static void tess_end_callback( GLUtesselator *tobj );
static void tess_edgeflag_callback( GLUtesselator *tobj, GLboolean flag );
static void tess_output_triangle( GLUtesselator *tobj, tess_vertex_t *vertex );

static void cleanup( GLUtesselator *tobj );

void contour_dump( tess_contour_t *contour );


/*****************************************************************************
 * Internal type definitions:
 *****************************************************************************/

typedef struct
{
    tess_vertex_t	*vertex;
    GLdouble		dist;
} fist_vecdist_t;

#define RV_INSERT(t, r)			\
	hashtable_insert( (t), ((int) r), ((void *) r) )
#define RV_DELETE(t, r)			\
	hashtable_delete( (t), ((int) r) )



/*****************************************************************************
 *
 *		    FAST INDUSTRIAL-STRENGTH TRIANGULATION (FIST)
 *
 *****************************************************************************/


/*****************************************************************************
 * fist_tessellation
 *
 * The main algorithm.  This is the core triangulation code that implements
 * the Fast Industrial-Strength Triangulation algorithm.
 *****************************************************************************/
GLenum fist_tessellation( GLUtesselator *tobj )
{
    MSG( 5, "  -> fist_tessellation( tobj:%p )\n", tobj );

    /* Remove zero-length edges. */
    remove_coincident_vertices( tobj );

    contour_dump( tobj->contours );

    /* Calculate and add any intersection points. */
    tess_calc_intersections( tobj );

    /* Sort and renumber the vertices of all the polygons. */
    sort_vertices( tobj );

    /* Compute and adjust the orientations of all the polygons. */
    compute_orientations( tobj );

    contour_dump( tobj->contours );

    /* Do those fancy calculations for the GLU winding rules. */
    tess_preprocess_contours( tobj );

    /*
     * Transform multiple polygons into a single polygon by building all
     *  bridges.
     */
    if ( tobj->contour_count > 1 )
    {
#if 1
	tess_contour_t	*contour = tobj->contours;
	GLuint		i;

	for ( i = 0 ; i < tobj->contour_count ; i++ )
	{
	    contour_dump( contour );
	    contour = contour->next;
	}
#endif
	transform_build_bridges( tobj );
    }

    /* Classify all of the angles of the polygon. */
    classify_angles( tobj );

#if 1
    /* Tessellate all the contours that make up the polygon. */
    tessellate_contours( tobj );

    /* Cleanup after tessellation is complete. */
    cleanup( tobj );
#else
    output_contours( tobj );
    contour_dump( tobj->contours );
#endif

    MSG( 5, "  <- fist_tessellation( tobj:%p )\n", tobj );
    return tobj->error;
}


/*****************************************************************************
 * fist_recovery_process
 *
 * The main recovery algorithm.  This is the multi-phase recovery process the
 * FIST algorithm uses when things start going wrong.  It ensures a
 * topologically-valid triangulation is produced, no matter what.
 *****************************************************************************/
GLenum fist_recovery_process( GLUtesselator *tobj, tess_contour_t *contour )
{
    tess_vertex_t	*vertex = contour->vertices;
    GLuint		i;

    MSG( 5, "  -> fist_recovery_process( tobj:%p c:%p )\n", tobj, contour );

    /*
     * We start the multi-phase recovery process by clipping zero area
     *  ears, and ears where a vertex lies on another edge.  This may
     *  get us out of trouble, but if not the hard-core recovery must
     *  take place.
     */
    for ( i = 0 ; i < contour->vertex_count ; i++ )
    {
	if ( vertex->previous->index == vertex->next->index )
	{
	    MSG( 5, "       zero area ear: (%d, %d, %d)\n", vertex->previous->index, vertex->index, vertex->next->index );

	    /* Output the triangle. */
	    tess_output_triangle( tobj, vertex );

	    /* Remove the vertex from the contour as required. */
	    vertex->previous->next = vertex->next->next;
	    vertex->next->next->previous = vertex->previous;

	    /* Set the new edge flag. */
	    vertex->previous->edge_flag = GL_FALSE;

	    if ( contour->vertices == vertex ) {
		contour->vertices = vertex->previous;
	    }
	    contour->vertex_count -= 2;

	    /*
	     * Update the reflex vertex set.  We first need to remove
	     *  any entries for the three vertices we have touched,
	     *  and then re-classify the two vertices that remain in
	     *  the contour.
	     */
	    RV_DELETE( contour->reflex_vertices, vertex->previous );
	    RV_DELETE( contour->reflex_vertices, vertex );
	    RV_DELETE( contour->reflex_vertices, vertex->next );
	    RV_DELETE( contour->reflex_vertices, vertex->next->next );

	    classify_vertex( contour, vertex->previous );
	    classify_vertex( contour, vertex->next->next );

	    free( vertex->next );
	    free( vertex );

	    MSG( 5, "  <- fist_recovery_process( tobj:%p ) okay\n", tobj );
	    return GLU_NO_ERROR;
	}

	vertex = vertex->next;
    }

    contour_dump( contour );

    /* Report a failure in the recovery process. */
    tess_error_callback( tobj, GLU_TESS_ERROR8 );

    MSG( 5, "  <- fist_recovery_process( tobj:%p ) failed!\n", tobj );
    return GLU_ERROR;
}



/*****************************************************************************
 *
 *				INTERNAL FUNCTIONS
 *
 *****************************************************************************/


/*****************************************************************************
 * remove_coincident_vertices
 *****************************************************************************/
static GLenum remove_coincident_vertices( GLUtesselator *tobj )
{
    tess_contour_t	*contour = tobj->contours;
    GLuint		i;

    MSG( 5, "    -> remove_coincident_vertices( tobj:%p )\n", tobj );

    for ( i = 0 ; i < tobj->contour_count ; i++ )
    {
	tess_vertex_t	*vertex = contour->vertices;
	GLuint		j;

	for ( j = 0 ; j < contour->vertex_count ; j++ )
	{
	    if ( ( ABSD( vertex->coords[X]
			 - vertex->next->coords[X] ) <= tobj->tolerance ) &&
		 ( ABSD( vertex->coords[Y]
			 - vertex->next->coords[Y] ) <= tobj->tolerance ) &&
		 ( ABSD( vertex->coords[Z]
			 - vertex->next->coords[Z] ) <= tobj->tolerance ) )
	    {
		tess_vertex_t	*coinc = vertex->next;

		/* Coincident vertices, so remove one of them. */
		MSG( 5, "         coincident (%.2f,%.2f,%.2f) and (%.2f,%.2f,%.2f) count: %d\n", vertex->coords[X], vertex->coords[Y], vertex->coords[Z], vertex->next->coords[X], vertex->next->coords[Y], vertex->next->coords[Z], contour->vertex_count );

		vertex->next = coinc->next;
		coinc->next->previous = vertex;

		if ( contour->vertices == coinc ) {
		    contour->vertices = vertex;
		}
		if ( contour->last_vertex == coinc ) {
		    contour->last_vertex = vertex;
		}

		contour->vertex_count--;
		tobj->vertex_count--;

		free( coinc );
	    }

	    vertex = vertex->next;
	}

	contour = contour->next;
    }

    MSG( 5, "    <- remove_coincident_vertices( tobj:%p )\n", tobj );
    return GLU_NO_ERROR;
}


/*****************************************************************************
 * compute_orientations
 *
 * The orientations of each contour loop previously calculated are inspected
 * and adjusted so that the final triangulation will be CCW with respect to
 * the normal of the triangulation plane.
 *****************************************************************************/
static int compare_contour_areas( const void *c1, const void *c2 );

static GLenum compute_orientations( GLUtesselator *tobj )
{
    tess_contour_t	**sorted_contours;
    tess_contour_t	*current;
    GLuint		i;

    MSG( 15, "    -> compute_orientations( tobj:%p )\n", tobj );

    if ( tobj->contour_count > 1 )
    {
	/*
	 * If we have multiple contours, sort them by their size to determine
	 *  which one is the exterior.  Make sure this exterior contour is
	 *  oriented CCW with respect to the tessellation normal, and if it
	 *  is not oriented this way reverse all the contours.
	 *
	 * FIXME: Is this the best place to do this?  I think the winding
	 *  rule stuff will work out the exteriour contour better than this.
	 */

	sorted_contours = (tess_contour_t **)
	    malloc( tobj->contour_count * sizeof(tess_contour_t *) );
	if ( sorted_contours == NULL ) {
	    tess_error_callback( tobj, GLU_OUT_OF_MEMORY );
	    return GLU_ERROR;
	}

	current = tobj->contours;
	i = 0;

	while ( i < tobj->contour_count )
	{
	    sorted_contours[ i++ ] = current;
	    current = current->next;
	}

	/* Sort the contours by their area. */
	qsort( sorted_contours, tobj->contour_count,
	       sizeof(tess_contour_t *), compare_contour_areas );

	tobj->contours     = sorted_contours[ 0 ];
	tobj->last_contour = sorted_contours[ tobj->contour_count - 1 ];

	current = tobj->contours;

	for ( i = 1; i < tobj->contour_count; i++ )
	{
	    current->next = sorted_contours[ i ];
	    sorted_contours[ i ]->previous = current;

	    current = current->next;
	}

	/* Wrap the contour list. */
	tobj->last_contour->next = tobj->contours;
	tobj->contours->previous = tobj->last_contour;

	if ( sorted_contours ) {
	    free( sorted_contours );
	}
    }

    MSG( 15, "    <- compute_orientations( tobj:%p )\n", tobj );
    return GLU_NO_ERROR;
}

/*****************************************************************************
 * compare_contour_areas
 *****************************************************************************/
static int compare_contour_areas( const void *c1, const void *c2 )
{
    GLdouble	area1, area2;

    area1 = (*((tess_contour_t **) c1))->area;
    area2 = (*((tess_contour_t **) c2))->area;

    if ( area1 < area2 ) {
	return 1;
    }
    if ( area1 > area2 ) {
	return -1;
    }
    return 0;
}


/*****************************************************************************
 * sort_vertices
 *
 * Sort all the vertices from all contours lexicographically, first by
 * x-coordinate, and then by y-coordinate.  Duplicates are removed from this
 * array, so that coincident vertices have the same index in the array.
 *****************************************************************************/
static int compare_vertices( const void *v1, const void *v2 );

static GLenum sort_vertices( GLUtesselator *tobj )
{
    tess_contour_t	*current;
    tess_vertex_t	*vertex;
    GLuint		i, j, n = 0, vertex_count, removed;

    MSG( 15, "    -> sort_vertices( tobj:%p )\n", tobj );

    /* Allocate the sorted vertices array. */

    tobj->sorted_vertices = (tess_vertex_t **)
	malloc( tobj->vertex_count * sizeof(tess_vertex_t *) );
    if ( tobj->sorted_vertices == NULL ) {
	tess_error_callback( tobj, GLU_OUT_OF_MEMORY );
	return GLU_ERROR;
    }

    /* Add each vertex from each contour to the array for sorting. */

    for ( current = tobj->contours, i = 0;
	  i < tobj->contour_count; current = current->next, i++ )
    {
	for ( vertex = current->vertices, j = 0;
	      j < current->vertex_count; vertex = vertex->next, j++ )
	{
	    tobj->sorted_vertices[ n++ ] = vertex;
	}
    }

    /* Sort the array of vertices. */
    qsort( tobj->sorted_vertices, tobj->vertex_count,
	   sizeof(tess_vertex_t *), compare_vertices );

    tobj->sorted_vertices[0]->index = 0;

    vertex_count = tobj->vertex_count;
    removed = 0;
    i = 1;

    /* Scan through the array, removing all duplicate entries, and assign
     *  each vertex its index in the array. */

    while ( i < vertex_count )
    {
	tobj->sorted_vertices[ i ] = tobj->sorted_vertices[ i + removed ];

	if ( COMPARE_3DV( tobj->sorted_vertices[ i ]->coords,
			  tobj->sorted_vertices[ i - 1 ]->coords ) )
	{
	    tobj->sorted_vertices[ i ]->index = i - 1;

	    removed++;
	    vertex_count--;

	    MSG( 25, "         v: (%.2f, %.2f) index: %d\n",
		 tobj->sorted_vertices[i]->v[X],
		 tobj->sorted_vertices[i]->v[Y],
		 tobj->sorted_vertices[i]->index );
	}
	else
	{
	    tobj->sorted_vertices[ i ]->index = i;

	    MSG( 25, "         v: (%.2f, %.2f) index: %d\n",
		 tobj->sorted_vertices[i]->v[X],
		 tobj->sorted_vertices[i]->v[Y],
		 tobj->sorted_vertices[i]->index );

	    i++;
	}
    }

    /*
     * Shuffle each contour around so the head of the vertex list is the
     *  leftmost vertex.
     */
    current = tobj->contours;

    for ( i = 0; i < tobj->contour_count; i++ )
    {
	tess_vertex_t	*left = current->vertices;
	tess_vertex_t	*vertex = current->vertices->next;
	GLuint		j;

	for ( j = 1; j < current->vertex_count; j++ )
	{
	    if ( vertex->index < left->index )
	    {
		left = vertex;
	    }
	    vertex = vertex->next;
	}
	current->vertices = left;
	current->last_vertex = left->previous;

	current = current->next;
    }

    MSG( 15, "    <- sort_vertices( tobj:%p )\n", tobj );
    return GLU_NO_ERROR;
}

/*****************************************************************************
 * compare_vertices
 *
 * FIXME: Make this faster...  At least it's right now, though.
 *****************************************************************************/
static int compare_vertices( const void *v1, const void *v2 )
{
    tess_vertex_t	*vertex1 = *((tess_vertex_t **) v1);
    tess_vertex_t	*vertex2 = *((tess_vertex_t **) v2);

    if ( ABSD( vertex1->v[X] - vertex2->v[X] ) >= EQUAL_EPSILON )
    {
	return ( vertex1->v[X] > vertex2->v[X] ) ? 1 : -1;
    }
    else
    {
	return ( vertex1->v[Y] > vertex2->v[Y] ) ? 1 : -1;
    }
}


/*****************************************************************************
 * transform_build_bridges
 *
 * Multiply-connected polygonal areas are transformed into a single degenerate
 * contour by iteratively linking the internal island loops with the outer
 * boundary by means of contour "bridges".  Thus, the triangulation need only
 * deal with a single contour when this process has been completed.
 *****************************************************************************/
static int compare_contour_left_vertices( const void *c1, const void *c2 );
static int compare_vertex_distances( const void *v1, const void *v2 );

static GLenum transform_build_bridges( GLUtesselator *tobj )
{
    tess_contour_t	**sorted;
    tess_contour_t	*contour = tobj->contours;
    GLuint		i, num_sorted;

    MSG( 5, "    -> transform_build_bridges( tobj:%p )\n", tobj );

    sorted = (tess_contour_t **)
	malloc( tobj->contour_count * sizeof(tess_contour_t *) );
    if ( sorted == NULL ) {
	tess_error_callback( tobj, GLU_OUT_OF_MEMORY );
	return GLU_ERROR;
    }

    for ( contour = tobj->contours, num_sorted = 0, i = 0;
	  i < tobj->contour_count; contour = contour->next, i++ )
    {
	if ( contour->orientation == GLU_CW )
	{
	    sorted[ num_sorted++ ] = contour;
	}
    }

    /* Sort the contours by their leftmost vertices. */
    qsort( sorted, num_sorted, sizeof(tess_contour_t *),
	   compare_contour_left_vertices );

    /* Reset contour pointer.  This is the contour we want to add
       the islands to. */
    contour = tobj->contours;

    for ( i = 0; i < num_sorted; i++ )
    {
	fist_vecdist_t	*closest;
	tess_vertex_t	*vertex, *left_vertex = sorted[ i ]->vertices;
	tess_vertex_t	*new_edge[2];
	GLuint		j, num_closest = left_vertex->index;

	closest = (fist_vecdist_t *)
	    malloc( num_closest * sizeof(fist_vecdist_t) );
	if ( closest == NULL ) {
	    tess_error_callback( tobj, GLU_OUT_OF_MEMORY );
	    return GLU_ERROR;
	}

	for ( vertex = contour->vertices, j = 0;
	      j < contour->vertex_count; vertex = vertex->next, j++ )
	{
	    if ( vertex->index < left_vertex->index )
	    {
		closest[ vertex->index ].vertex = vertex;
		closest[ vertex->index ].dist =
		    LEN_SCALAR( vertex->v[X] - left_vertex->v[X],
				vertex->v[Y] - left_vertex->v[Y] );
	    }
	}

	qsort( closest, num_closest, sizeof(fist_vecdist_t),
	       compare_vertex_distances );

	/*
	 * FIXME:  Check validity of added edge...
	 */

	new_edge[0] = (tess_vertex_t *) malloc( sizeof(tess_vertex_t) );
	new_edge[1] = (tess_vertex_t *) malloc( sizeof(tess_vertex_t) );

	if ( ( new_edge[0] == NULL ) || ( new_edge[1] == NULL ) ) {
	    tess_error_callback( tobj, GLU_OUT_OF_MEMORY );
	    return GLU_ERROR;
	}

	/* FIXME: Describe what the hell is going on in here. */

	MEMCPY( new_edge[0], left_vertex, sizeof(tess_vertex_t) );
	MEMCPY( new_edge[1], closest[0].vertex, sizeof(tess_vertex_t) );

	new_edge[0]->next = new_edge[1];
	new_edge[1]->previous = new_edge[0];

	left_vertex->previous->next = new_edge[0];
	closest[0].vertex->next->previous = new_edge[1];

	closest[0].vertex->next = left_vertex;
	left_vertex->previous = closest[0].vertex;

	contour->vertex_count += 2 + sorted[ i ]->vertex_count;

	sorted[ i ]->previous->next = sorted[ i ]->next;
	sorted[ i ]->next->previous = sorted[ i ]->previous;

	if ( sorted[ i ] == tobj->last_contour ) {
	    tobj->last_contour = sorted[ i ]->previous;
	}

	tobj->contour_count--;

	if ( closest ) {
	    free( closest );
	    closest = NULL;
	}
	if ( sorted[ i ] ) {
	    free( sorted[ i ] );
	    sorted[ i ] = NULL;
	}

	MSG( 5, "         added edges (%d, %d, %d, %d) and (%d, %d, %d, %d)\n", new_edge[0]->previous->index, new_edge[0]->index, new_edge[0]->next->index, new_edge[0]->next->next->index, left_vertex->previous->previous->index, left_vertex->previous->index, left_vertex->index, left_vertex->next->index );
    }

    if ( sorted ) {
	free( sorted );
    }

    MSG( 5, "    <- transform_build_bridges( tobj:%p ) count: %d\n", tobj, tobj->contour_count );
    return GLU_NO_ERROR;
}

/*****************************************************************************
 * compare_contour_left_vertices
 *****************************************************************************/
static int compare_contour_left_vertices( const void *c1, const void *c2 )
{
    tess_vertex_t	*vertex1 = (*((tess_contour_t **) c1))->vertices;
    tess_vertex_t	*vertex2 = (*((tess_contour_t **) c2))->vertices;

    return vertex1->index - vertex2->index;
}

/*****************************************************************************
 * compare_vertex_distances
 *****************************************************************************/
static int compare_vertex_distances( const void *v1, const void *v2 )
{
    fist_vecdist_t	*vecdist1 = (fist_vecdist_t *) v1;
    fist_vecdist_t	*vecdist2 = (fist_vecdist_t *) v2;

    if ( ( vecdist2->dist - vecdist1->dist ) < EQUAL_EPSILON ) {
	return 1;
    } else {
	return -1;
    }
}


/*****************************************************************************
 * classify_angles
 *
 * All the internal angles of the contours are classified as convex or reflex.
 * This enables the geometric hashing that will significantly speed up the
 * triangulation process.
 *****************************************************************************/
static GLenum classify_angles( GLUtesselator *tobj )
{
    tess_contour_t	*contour;
    tess_vertex_t	*vertex;
    GLuint		i, j;

    MSG( 15, "    -> classify_angles( tobj:%p )\n", tobj );

    for ( contour = tobj->contours, i = 0;
	  i < tobj->contour_count; contour = contour->next, i++ )
    {
	if ( ! contour->reflex_vertices ) {
	    contour->reflex_vertices = hashtable_init( HT_DEFAULT_SIZE );
	}

	for ( vertex = contour->vertices, j = 0;
	      j < contour->vertex_count; vertex = vertex->next, j++ )
	{
	    /*
	     * We need to break the classification out of the loops so
	     *  we can re-classify the vertices that change when an ear
	     *  is clipped.
	     */
	    classify_vertex( contour, vertex );
	}
    }

    MSG( 15, "    <- classify_angles( tobj:%p )\n", tobj );
    return GLU_NO_ERROR;
}

/*****************************************************************************
 * classify_vertex
 *****************************************************************************/
static void classify_vertex( tess_contour_t *contour, tess_vertex_t *vertex )
{
    vertex->side = point_line_test( vertex->previous->v,
				    vertex->v, vertex->next->v );

    MSG( 5, "         classifying v: %d side: %0.2f\n", vertex->index, vertex->side );

    /*
     * We have three cases here:
     *
     *  1) vertex->side > 0  means angle < PI
     *  2) vertex->side == 0 means angle == PI
     *  3) vertex->side < 0  means angle > PI, or reflex
     *
     * So, what we want to grab here are all the vertices with
     *  a 'side' that is less than zero.  We use our epsilon
     *  value to allow for some rounding errors in the calculation
     * FIXME: Is this epsilon value small enough?
     */
    if ( vertex->side < -EQUAL_EPSILON ) {
 	RV_INSERT( contour->reflex_vertices, vertex );
    }
}


/*****************************************************************************
 * tessellate_contours
 *
 * Actually perform the ear clipping on the contours.
 *****************************************************************************/
static GLenum determine_ears( GLUtesselator *tobj, tess_contour_t *contour );
static GLboolean earity_test( tess_contour_t *contour, tess_vertex_t *vertex );
static GLdouble shape_classifier( tess_vertex_t *vertex );
static heap_elt_t *add_ear_to_heap( heap_t *heap, tess_vertex_t *vertex );

static GLenum tessellate_contours( GLUtesselator *tobj )
{
    tess_contour_t	*contour = tobj->contours;

    MSG( 1, "    -> tessellate_contours( tobj:%p )\n", tobj );

    if ( tobj->contour_count == 0 )
    {
	MSG( 1, "         no contours, returning...\n" );
	return GLU_NO_ERROR;
    }

    /*
     * Allocate the priority queue for the ears of the polgyon.  We use
     *  a fairly standard implementation of a heap for this queue, which
     *  is sorted by how close an ear is to an equilateral triangle.
     *  This will arguably give a higher-quality tessellation, at the cost
     *  of slightly reduced preformance.
     */
    tobj->ears = heap_init();

    /* Break the contour loop into a standard doubly-linked list. */
    tobj->last_contour->next = NULL;
    tobj->contours->previous = NULL;

    /*
     * Start outputting the triangles.  At the moment, we just ouput the
     *  ears when we get them.  Could we ouput them in strip order?  It
     *  would be nice, but it really makes the implementation of the
     *  algorithm less general than it is now.  Is the implementation
     *  too general now?  Quite possibly...
     */
    tess_begin_callback( tobj, GL_TRIANGLES );

    while ( contour )
    {
	GLboolean	clipped = GL_FALSE;

	determine_ears( tobj, contour );

	while ( contour->vertex_count > 3 )
	{
	    if ( tobj->ears->size > 0 )
	    {
		heap_elt_t	*element = heap_extract_max( tobj->ears );
		heap_elt_t	*next = NULL, *previous = NULL;
		tess_vertex_t	*vertex = HEAP_VERTEX( element );

		/* Remove ear from current contour... */
		MSG( 1, "           nv: %2d hs: %d ear: (%d, %d, %d)\n", contour->vertex_count, tobj->ears->size, vertex->previous->index, vertex->index, vertex->next->index );

		clipped = GL_TRUE;

		/* Output the triangle. */
		tess_output_triangle( tobj, vertex );

		/* Remove the vertex from the contour as required. */
		vertex->previous->next = vertex->next;
		vertex->next->previous = vertex->previous;

		/* Set the new edge flag. */
		vertex->previous->edge_flag = GL_FALSE;

		if ( contour->vertices == vertex ) {
		    contour->vertices = vertex->next;
		}
		contour->vertex_count--;

		/* Update the ear heap for the previous vertex. */
		if ( element->previous )
		{
		    /*
		     * An existing entry based on the previous vertex will
		     *  no longer be valid, so we need to remove it now.
		     */
		    previous = heap_delete( tobj->ears,
					    element->previous->index );

		    if ( ! previous ) continue;

		    /*
		     * We can now determine if clipping the current ear
		     *  will form a new ear based on the previous vertex.
		     *  If such a new ear is formed, add it to the ear
		     *  queue.  Otherwise, just clean up.
		     */
		    if ( earity_test( contour, HEAP_VERTEX( previous ) ) )
		    {
			previous->value =
			    shape_classifier( HEAP_VERTEX( previous ) );

			heap_insert( tobj->ears, previous );
		    }
		    else
		    {
			if ( previous->previous ) {
			    previous->previous->next = NULL;
			}
			free( previous );
			previous = NULL;
		    }
		}

		/* Update the ear heap for the next vertex. */
		if ( element->next )
		{
		    /*
		     * An existing entry based on the next vertex will
		     *  no longer be valid, so we need to remove it now.
		     */
		    next = heap_delete( tobj->ears, element->next->index );

		    if ( ! next ) continue;

		    /*
		     * We can now determine if clipping the current ear
		     *  will form a new ear based on the next vertex.  If
		     *  such a new ear is formed, add it to the ear queue.
		     *  Otherwise, just clean up.
		     */
		    if ( earity_test( contour, HEAP_VERTEX( next ) ) )
		    {
			next->value =
			    shape_classifier( HEAP_VERTEX( next ) );

			heap_insert( tobj->ears, next );
		    }
		    else
		    {
			if ( next->next ) {
			    next->next->previous = NULL;
			}
			free( next );
			next = NULL;
		    }
		}

		/* Update the previous and next ear queue entries. */
		if ( previous && next )
		{
		    previous->next = next;
		    next->previous = previous;
		}
		else if ( previous )
		{
		    previous->next = NULL;
		}
		else if ( next )
		{
		    next->previous = NULL;
		}

		/*
		 * Update the reflex vertex set.  We first need to remove
		 *  any entries for the three vertices we have touched,
		 *  and then re-classify the two vertices that remain in
		 *  the contour.
		 */
		RV_DELETE( contour->reflex_vertices, vertex->previous );
		RV_DELETE( contour->reflex_vertices, vertex );
		RV_DELETE( contour->reflex_vertices, vertex->next );

		classify_vertex( contour, vertex->previous );
		classify_vertex( contour, vertex->next );

		free( vertex );
		free( element );
	    }
	    else if ( clipped )
	    {
		clipped = GL_FALSE;
		determine_ears( tobj, contour );
	    }
	    else
	    {
		if ( fist_recovery_process( tobj, contour ) == GLU_NO_ERROR )
		{
		    clipped = GL_TRUE;
		}
		else
		{
		    /* Finish outputting the triangles. */
		    tess_end_callback( tobj );
#if 0
		    /* Dump the remaining contours as line loops. */
		    output_contours( tobj );
#endif
		    return GLU_ERROR;
		}
	    }
	}

	if ( contour->vertex_count == 3 )
	{
	    tess_vertex_t	*vertex = contour->vertices;

	    MSG( 1, "           nv: %2d hs: %d ear: (%d, %d, %d)\n", contour->vertex_count, tobj->ears->size, vertex->previous->index, vertex->index, vertex->next->index );

	    /* Output the last triangle. */
	    tess_output_triangle( tobj, vertex );

	    free( vertex->previous );
	    free( vertex->next );
	    free( vertex );
	}

	/* Move on to the next contour. */
	contour->vertices = NULL;
	contour->last_vertex = NULL;
	contour->vertex_count = 0;

	contour = contour->next;
    }

    /* Finish outputting the triangles. */
    tess_end_callback( tobj );

    MSG( 1, "    <- tessellate_contours( tobj:%p )\n", tobj );
    return GLU_NO_ERROR;
}

/*****************************************************************************
 * determine_ears
 *
 * Determine the potential ears for the given contour.
 *****************************************************************************/
static GLenum determine_ears( GLUtesselator *tobj, tess_contour_t *contour )
{
    tess_vertex_t	*vertex = contour->vertices;
    heap_elt_t		*element = NULL, *previous = NULL, *first = NULL;
    GLuint		i;

    MSG( 1, "      -> determine_ears( tobj:%p )\n", tobj );

    contour_dump( contour );

    for ( i = 0; i < contour->vertex_count; i++ )
    {
	if ( earity_test( contour, vertex ) )
	{
	    element = add_ear_to_heap( tobj->ears, vertex );

	    if ( element == NULL ) {
		tess_error_callback( tobj, GLU_OUT_OF_MEMORY );
		return GLU_ERROR;
	    }

	    element->previous = previous;

	    if ( previous ) {
		previous->next = element;
	    }

	    previous = element;

	    if ( i == 0 ) {
		first = element;
	    }
	}
	else
	{
	    previous = NULL;
	}

	vertex = vertex->next;
    }

    if ( previous ) {
	previous->next = first;
    }

    MSG( 1, "      <- determine_ears( tobj:%p )\n", tobj );
    return GLU_NO_ERROR;
}

/*****************************************************************************
 * earity_test
 *
 * Is the given vertex part of an ear?  Test all the reflex vertices of the
 *  contour against the ear, and if none are inside the ear we have a winner.
 *****************************************************************************/
static GLboolean earity_test( tess_contour_t *contour, tess_vertex_t *vertex )
{
    hashtable_t	*table = contour->reflex_vertices;
    GLuint	i;

    if ( ( vertex->side < EQUAL_EPSILON ) || ( ! table ) )
    {
	return GL_FALSE;
    }
    else
    {
	for ( i = 0; i < table->size; i++ )
	{
	    hashtable_elt_t	*elt = table->elements[ i ];

	    while ( elt )
	    {
		tess_vertex_t	*reflex = (tess_vertex_t *) elt->ptr;

		if ( point_triangle_test( vertex, reflex ) ) {
		    return GL_FALSE;
		}

		elt = elt->next;
	    }
	}

	return GL_TRUE;
    }
}

/*****************************************************************************
 * shape_classifier
 *
 * Return the minimum angle of the triangle made up by vertex, vertex->next
 *  and vertex->previous.
 *****************************************************************************/
static GLdouble shape_classifier( tess_vertex_t *vertex )
{
    GLdouble	min, current;

    min = angle_3dv( vertex->previous->v, vertex->v, vertex->next->v );

    current = angle_3dv( vertex->v, vertex->next->v, vertex->previous->v );
    if ( current < min ) { min = current; }

    current = angle_3dv( vertex->next->v, vertex->previous->v, vertex->v );
    if ( current < min ) { min = current; }

    return min;
}


/*****************************************************************************
 * add_ear_to_heap
 *
 * Add the ear containing the given vertex to the heap.
 *****************************************************************************/
static heap_elt_t *add_ear_to_heap( heap_t *heap, tess_vertex_t *vertex )
{
    heap_elt_t	*element;

    element = (heap_elt_t *) malloc( sizeof(heap_elt_t) );
    if ( element == NULL ) {
	return NULL;
    }

    element->value = shape_classifier( vertex );
    element->ptr = (void *) vertex;
    element->index = 0;

    element->next = NULL;
    element->previous = NULL;

    heap_insert( heap, element );

    return element;
}



#if 0
/*****************************************************************************
 * output_contours
 *
 * Output the tessellated contours.  Don't think we really need this anymore.
 *****************************************************************************/
static GLenum output_contours( GLUtesselator *tobj )
{
    tess_contour_t	*contour;
    tess_vertex_t	*vertex;
    GLuint		i, j;

    if ( tobj->error != GLU_NO_ERROR ) {
	glColor4ub( 0x7f, 0x7f, 0x7f, 0x7f );
    }

    for ( contour = tobj->contours, i = 0;
	  i < tobj->contour_count; contour = contour->next, i++ )
    {
	tess_begin_callback( tobj, GL_LINE_LOOP );

	for ( vertex = contour->vertices, j = 0;
	      j < contour->vertex_count; vertex = vertex->next, j++ )
	{
	    tess_vertex_callback( tobj, vertex->data );
	}
	tess_end_callback( tobj );
    }

    return GLU_NO_ERROR;
}
#endif


/*****************************************************************************
 * point_line_test
 *
 * Return positive if p is on the left of the line segment u -> v, negative
 * if p is on the right of u -> v, and 0 if p is on u -> v.
 *****************************************************************************/
GLdouble point_line_test( GLdouble u[2], GLdouble v[2], GLdouble p[2] )
{
    return ( ( u[Y] - v[Y] ) * p[X] +
	     ( v[X] - u[X] ) * p[Y] +
	     ( u[X] * v[Y] - u[Y] * v[X] ) );
}

/*****************************************************************************
 * point_triangle_test
 *
 * Determine if the given point is inside the triangle given by the points
 * triangle, triangle->next and triangle->previous.
 *****************************************************************************/
GLboolean point_triangle_test( tess_vertex_t *triangle, tess_vertex_t *point )
{
    if ( ( point_line_test( triangle->v, triangle->next->v,
			    point->v ) > EQUAL_EPSILON ) &&
	 ( point_line_test( triangle->next->v, triangle->previous->v,
			    point->v ) > EQUAL_EPSILON ) &&
	 ( point_line_test( triangle->previous->v, triangle->v,
			    point->v ) > EQUAL_EPSILON ) )
    {
	return GL_TRUE;
    } else {
	return GL_FALSE;
    }
}

/*****************************************************************************
 * angle_3dv
 *
 * Calculate the interior angle between the line segments va->vb and vb->vc.
 *****************************************************************************/
GLdouble angle_3dv( GLdouble va[2], GLdouble vb[2], GLdouble vc[2] )
{
    GLdouble	u[2], v[2];
    GLdouble	ret;

    u[X] = va[X] - vb[X]; u[Y] = va[Y] - vb[Y];
    v[X] = vc[X] - vb[X]; v[Y] = vc[Y] - vb[Y] ;

    ret = (GLdouble) ( u[X] * v[X] + u[Y] * v[Y] ) /
	( LEN_SCALAR( u[X], u[Y] ) * LEN_SCALAR( v[X], v[Y] ) );

    return (GLdouble) RAD_TO_DEG( acos( ret ) );
}


/*****************************************************************************
 *
 *			TESSELLATION CALLBACKS
 *
 *****************************************************************************/

static void tess_begin_callback( GLUtesselator *tobj, GLenum mode )
{
    if ( tobj->callbacks.beginData != NULL )
    {
	( tobj->callbacks.beginData )( mode, tobj->data );
    }
    else if ( tobj->callbacks.begin != NULL )
    {
	( tobj->callbacks.begin )( mode );
    }
}

static void tess_vertex_callback( GLUtesselator *tobj, void *vertex_data )
{
    if ( tobj->callbacks.vertexData != NULL )
    {
	( tobj->callbacks.vertexData )( vertex_data, tobj->data );
    }
    else if ( tobj->callbacks.vertex != NULL )
    {
	( tobj->callbacks.vertex )( vertex_data );
    }
}

static void tess_end_callback( GLUtesselator *tobj )
{
    if ( tobj->callbacks.endData != NULL )
    {
	( tobj->callbacks.endData )( tobj->data );
    }
    else if ( tobj->callbacks.end != NULL )
    {
	( tobj->callbacks.end )();
    }
}

static void tess_edgeflag_callback( GLUtesselator *tobj, GLboolean flag )
{
    if ( flag == tobj->edge_flag ) {
	return;
    } else {
	tobj->edge_flag = flag;
    }
    MSG( 5, "             setting edge_flag: %s\n", ( flag ) ? "GL_TRUE" : "GL_FALSE" );

    if ( tobj->callbacks.edgeFlagData != NULL )
    {
	( tobj->callbacks.edgeFlagData )( tobj->edge_flag, tobj->data );
    }
    else if ( tobj->callbacks.edgeFlag != NULL )
    {
	( tobj->callbacks.edgeFlag )( tobj->edge_flag );
    }
}

static void tess_output_triangle( GLUtesselator *tobj, tess_vertex_t *vertex )
{
    tess_edgeflag_callback( tobj, vertex->edge_flag );
    tess_vertex_callback( tobj, vertex->data );

    if ( vertex->next->next == vertex->previous ) {
	tess_edgeflag_callback( tobj, vertex->edge_flag );
    } else {
	tess_edgeflag_callback( tobj, GL_FALSE );
    }
    tess_vertex_callback( tobj, vertex->next->data );

    tess_edgeflag_callback( tobj, vertex->previous->edge_flag );
    tess_vertex_callback( tobj, vertex->previous->data );
}



/*****************************************************************************
 * cleanup
 *
 * Clean up after the triangulation has taken place.  All memory allocated
 * by the triangulation should be freed before control is handed back to the
 * GLU functions.
 *****************************************************************************/
static void cleanup( GLUtesselator *tobj )
{
    tess_contour_t	*contour = tobj->contours;
    GLuint		i;

    MSG( 5, "    -> cleanup( tobj:%p )\n", tobj );

    if ( tobj->sorted_vertices != NULL ) {
	free( tobj->sorted_vertices );
    }
    tobj->sorted_vertices = NULL;

    heap_cleanup( &tobj->ears );
    tobj->ears = NULL;

    for ( i = 0; i < tobj->contour_count; i++ )
    {
	hashtable_cleanup( &contour->reflex_vertices );

	contour = contour->next;
    }

    MSG( 5, "    <- cleanup( tobj:%p )\n", tobj );
}



/*****************************************************************************
 * contour_dump
 *
 * Dump all the vertices, reflex vertices etc for the given contour.
 *****************************************************************************/
void contour_dump( tess_contour_t *contour )
{
#ifdef DEBUG
	tess_vertex_t *vertex = contour->vertices;
	GLuint i;

    if ( tess_dbg_level > 0 )
    {
	fprintf( DBG_STREAM, "\ncontour %p - nv: %d area: %.2f\n", contour, contour->vertex_count, contour->area );

	for ( i = 0; i < contour->vertex_count; i++ )
	{
	    fprintf( DBG_STREAM, "  v: %-2d (%.2f, %.2f) (%d, %d, %d) label: %d edge: %d data: %p\n", vertex->index, vertex->v[X], vertex->v[Y], vertex->previous->index, vertex->index, vertex->next->index, vertex->label, vertex->edge_flag, vertex->data );

	    if ( ( i == contour->vertex_count - 1 ) &&
		 ( vertex->next != contour->vertices ) ) {
		fprintf( DBG_STREAM, "    ***** CONTOUR DOES NOT CLOSE ! *****\n" );
	    }
	    vertex = vertex->next;
	}

	fprintf( DBG_STREAM, "\n" );

	if ( contour->reflex_vertices )
	{
	    hashtable_t	*table = contour->reflex_vertices;

	    if ( table->nelements > 0 ) {
		fprintf( DBG_STREAM, "  r:" );
	    }
	    for ( i = 0; i < table->size; i++ )
	    {
		hashtable_elt_t	*elt = table->elements[ i ];

		while ( elt )
		{
		    tess_vertex_t	*v = HASH_VERTEX( elt );
		    fprintf( DBG_STREAM, " %d", v->index );

		    elt = elt->next;
		}
	    }
	    if ( table->nelements > 0 ) {
		fprintf( DBG_STREAM, "   n: %d\n\n", table->nelements );
	    }
	}

	if ( contour->cross_vertices )
	{
	    hashtable_t	*table = contour->cross_vertices;

	    if ( table->nelements > 0 ) {
		fprintf( DBG_STREAM, "  c:" );
	    }
	    for ( i = 0; i < table->size; i++ )
	    {
		hashtable_elt_t	*elt = table->elements[ i ];

		while ( elt )
		{
		    tess_vertex_t	*v = HASH_VERTEX( elt );
		    fprintf( DBG_STREAM, " %d", v->index );

		    elt = elt->next;
		}
	    }

	    if ( table->nelements > 0 ) {
		fprintf( DBG_STREAM, "   n: %d\n\n", table->nelements );
	    }
	}

	fflush( DBG_STREAM );
    }
#endif
}
