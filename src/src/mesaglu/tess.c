/* $Id: tess.c,v 1.21 1999/11/11 03:21:43 kendallb Exp $ */

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
 * GLU 1.3 Polygon Tessellation by Gareth Hughes <garethh@bell-labs.com>
 *
 *****************************************************************************/

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glu.h>

#include "tess.h"
#include "tess_macros.h"
#include "tess_fist.h"
#if 0
#include "tess_grid.h"
#endif

/*****************************************************************************
 * Internal function prototypes:
 *****************************************************************************/

static void init_callbacks( tess_callbacks_t *callbacks );

static void tess_cleanup( GLUtesselator *tobj );
static void inspect_current_contour( GLUtesselator *tobj );

void delete_contour( tess_contour_t **contour );
static void delete_all_contours( GLUtesselator *tobj );

#define TESS_CHECK_ERRORS(t)	if ( (t)->error != GLU_NO_ERROR ) goto cleanup

#ifdef DEBUG
GLint		tess_dbg_level;
#endif


/*****************************************************************************
 *
 *			GLU TESSELLATION FUNCTIONS
 *
 *****************************************************************************/


/*****************************************************************************
 * gluNewTess
 *****************************************************************************/
GLUtesselator* GLAPIENTRY gluNewTess( void )
{
    GLUtesselator *tobj;

#ifdef DEBUG
    if ( getenv( "GLU_TESS_DBG_LEVEL" ) ) {
	tess_dbg_level = atoi( getenv( "GLU_TESS_DBG_LEVEL" ) );
    } else {
	tess_dbg_level = DBG_LEVEL_BASE;
    }
#endif

    MSG( 15, "-> gluNewTess()\n" );

    tobj = malloc( sizeof(GLUtesselator) );
    if ( tobj == NULL ) {
	return NULL;
    }

    init_callbacks( &tobj->callbacks );

    tobj->boundary_only = GL_FALSE;
    tobj->winding_rule = GLU_TESS_WINDING_ODD;
    tobj->tolerance = 0.0;

    tobj->plane.normal[X] = 0.0;
    tobj->plane.normal[Y] = 0.0;
    tobj->plane.normal[Z] = 0.0;
    tobj->plane.dist = 0.0;

    tobj->contour_count = 0;
    tobj->contours = tobj->last_contour = NULL;
    tobj->current_contour = NULL;

    CLEAR_BBOX_2DV( tobj->mins, tobj->maxs );

    tobj->vertex_count = 0;
    tobj->sorted_vertices = NULL;
#if 0
    tobj->grid = NULL;
#endif
    tobj->cvc_lists = NULL;
    tobj->data = NULL;
    tobj->edge_flag = GL_FALSE;
    tobj->label = 0;

    tobj->error = GLU_NO_ERROR;

    MSG( 15, "<- gluNewTess() tobj:%p\n", tobj );
    return tobj;
}


/*****************************************************************************
 * gluDeleteTess
 *****************************************************************************/
void GLAPIENTRY gluDeleteTess( GLUtesselator *tobj )
{
    MSG( 15, "-> gluDeleteTess( tobj:%p )\n", tobj );

    if ( ( tobj->error == GLU_NO_ERROR ) && ( tobj->contour_count > 0 ) )
    {
	/* gluEndPolygon was not called. */
	tess_error_callback( tobj, GLU_TESS_ERROR3 );
    }

    /* Delete all internal structures. */
    tess_cleanup( tobj );
    free( tobj );

    MSG( 15, "<- gluDeleteTess()\n" );
}


/*****************************************************************************
 * gluTessBeginPolygon
 *****************************************************************************/
void GLAPIENTRY gluTessBeginPolygon( GLUtesselator *tobj, void *polygon_data )
{
    MSG( 15, "-> gluTessBeginPolygon( tobj:%p data:%p )\n", tobj, polygon_data );

    tobj->error = GLU_NO_ERROR;

    if ( tobj->current_contour != NULL )
    {
	/* gluEndPolygon was not called. */
	tess_error_callback( tobj, GLU_TESS_ERROR3 );
	tess_cleanup( tobj );
    }

    tobj->vertex_count = 0;
    tobj->data = polygon_data;
    tobj->edge_flag = GL_FALSE;
    tobj->label = 0;

    MSG( 15, "<- gluTessBeginPolygon( tobj:%p data:%p )\n", tobj, polygon_data );
}


/*****************************************************************************
 * gluTessBeginContour
 *****************************************************************************/
void GLAPIENTRY gluTessBeginContour( GLUtesselator *tobj )
{
    MSG( 15, "  -> gluTessBeginContour( tobj:%p )\n", tobj );
    TESS_CHECK_ERRORS( tobj );

    if ( tobj->current_contour != NULL )
    {
	/* gluTessEndContour was not called. */
	tess_error_callback( tobj, GLU_TESS_ERROR4 );
	return;
    }

    tobj->current_contour = malloc( sizeof(tess_contour_t) );
    if ( tobj->current_contour == NULL ) {
	tess_error_callback( tobj, GLU_OUT_OF_MEMORY );
	return;
    }

    COPY_3V( tobj->current_contour->plane.normal, tobj->plane.normal );
    tobj->current_contour->plane.dist = tobj->plane.dist;

    tobj->current_contour->area = 0.0;
    tobj->current_contour->orientation = GLU_UNKNOWN;

    tobj->current_contour->label = 0;
    tobj->current_contour->winding = 0;

    tobj->current_contour->rotx = tobj->current_contour->roty = 0.0;

    CLEAR_BBOX_2DV( tobj->current_contour->mins,
		    tobj->current_contour->maxs );

    tobj->current_contour->vertex_count = 0;
    tobj->current_contour->vertices =
	tobj->current_contour->last_vertex = NULL;

    tobj->current_contour->reflex_vertices = NULL;
    tobj->current_contour->cross_vertices = hashtable_init( HT_DEFAULT_SIZE );

 cleanup:
    MSG( 15, "  <- gluTessBeginContour( tobj:%p )\n", tobj );
    return;
}


/*****************************************************************************
 * gluTessVertex
 *****************************************************************************/
void GLAPIENTRY gluTessVertex( GLUtesselator *tobj, GLdouble coords[3],
			       void *vertex_data )
{
    tess_contour_t		*current = tobj->current_contour;
    tess_vertex_t		*last_vertex;

    MSG( 15, "    -> gluTessVertex( tobj:%p coords:(%.2f,%.2f,%.2f) )\n", tobj, coords[0], coords[1], coords[2] );
    TESS_CHECK_ERRORS( tobj );

    if ( current == NULL )
    {
	/* gluTessBeginContour was not called. */
	tess_error_callback( tobj, GLU_TESS_ERROR2 );
	return;
    }

    tobj->vertex_count++;

    last_vertex = current->last_vertex;

    if ( last_vertex == NULL )
    {
	last_vertex = malloc( sizeof(tess_vertex_t) );
	if ( last_vertex == NULL ) {
	    tess_error_callback( tobj, GLU_OUT_OF_MEMORY );
	    return;
	}

	current->vertices = last_vertex;
	current->last_vertex = last_vertex;

	last_vertex->index = -1;
	last_vertex->data = vertex_data;

	last_vertex->coords[X] = coords[X];
	last_vertex->coords[Y] = coords[Y];
	last_vertex->coords[Z] = coords[Z];

	last_vertex->side = 0.0;
	last_vertex->label = 0;
	last_vertex->mark = 0;

	last_vertex->next = NULL;
	last_vertex->previous = NULL;

	current->vertex_count++;
    }
    else
    {
	tess_vertex_t	*vertex;

	vertex = malloc( sizeof(tess_vertex_t) );
	if ( vertex == NULL ) {
	    tess_error_callback( tobj, GLU_OUT_OF_MEMORY );
	    return;
	}

	vertex->index = -1;
	vertex->data = vertex_data;

	vertex->coords[X] = coords[X];
	vertex->coords[Y] = coords[Y];
	vertex->coords[Z] = coords[Z];

	vertex->side = 0.0;
	vertex->label = 0;
	vertex->mark = 0;

	vertex->next = NULL;
	vertex->previous = last_vertex;

	current->vertex_count++;

	last_vertex->next = vertex;
	current->last_vertex = vertex;
    }

 cleanup:
    MSG( 15, "    <- gluTessVertex( tobj:%p )\n", tobj );
    return;
}


/*****************************************************************************
 * gluTessEndContour
 *****************************************************************************/
void GLAPIENTRY gluTessEndContour( GLUtesselator *tobj )
{
    MSG( 15, "  -> gluTessEndContour( tobj:%p )\n", tobj );
    TESS_CHECK_ERRORS( tobj );

    if ( tobj->current_contour == NULL )
    {
	/* gluTessBeginContour was not called. */
	tess_error_callback( tobj, GLU_TESS_ERROR2 );
	return;
    }

    if ( tobj->current_contour->vertex_count > 0 ) {
	inspect_current_contour( tobj );
    } else {
	delete_contour( &tobj->current_contour );
    }

 cleanup:
    MSG( 15, "  <- gluTessEndContour( tobj:%p )\n", tobj );
    return;
}


/*****************************************************************************
 * gluTessEndPolygon
 *****************************************************************************/
void GLAPIENTRY gluTessEndPolygon( GLUtesselator *tobj )
{
    MSG( 15, "-> gluTessEndPolygon( tobj:%p )\n", tobj );
    TESS_CHECK_ERRORS( tobj );

    if ( tobj->current_contour != NULL )
    {
	/* gluTessBeginPolygon was not called. */
	tess_error_callback( tobj, GLU_TESS_ERROR1 );
	return;
    }
    TESS_CHECK_ERRORS( tobj );

    /*
     * Ensure we have at least one contour to tessellate.  If we have none,
     *  clean up and exit gracefully.
     */
    if ( tobj->contour_count == 0 ) {
	tess_cleanup( tobj );
	return;
    }

    /* Wrap the contour list. */

    tobj->last_contour->next = tobj->contours;
    tobj->contours->previous = tobj->last_contour;

    TESS_CHECK_ERRORS( tobj );

    /*
     * Before we tessellate the contours, ensure we have the appropriate
     *  callbacks registered.  We at least need the begin, vertex and end
     *  callbacks to do any meaningful work.
     */
    if ( ( ( tobj->callbacks.begin != NULL ) ||
	   ( tobj->callbacks.beginData != NULL ) ) &&
	 ( ( tobj->callbacks.vertex != NULL ) ||
	   ( tobj->callbacks.vertexData != NULL ) ) &&
	 ( ( tobj->callbacks.end != NULL ) ||
	   ( tobj->callbacks.endData != NULL ) ) )
    {
	fist_tessellation( tobj );
    }

 cleanup:
    delete_all_contours( tobj );
    MSG( 15, "<- gluTessEndPolygon( tobj:%p )\n", tobj );
}


/*****************************************************************************
 * gluTessCallback
 *****************************************************************************/
void GLAPIENTRY gluTessCallback( GLUtesselator *tobj, GLenum which,
				 void (GLCALLBACKP fn)() )
{
    switch ( which )
    {
	/* Register the begin callbacks. */
    case GLU_TESS_BEGIN:
	tobj->callbacks.begin = (void (GLCALLBACKP)(GLenum)) fn;
	break;
    case GLU_TESS_BEGIN_DATA:
	tobj->callbacks.beginData = (void (GLCALLBACKP)(GLenum, void *)) fn;
	break;

	/* Register the edge flag callbacks. */
    case GLU_TESS_EDGE_FLAG:
	tobj->callbacks.edgeFlag = (void (GLCALLBACKP)(GLboolean)) fn;
	break;
    case GLU_TESS_EDGE_FLAG_DATA:
	tobj->callbacks.edgeFlagData =
		(void (GLCALLBACKP)(GLboolean, void *)) fn;
	break;

	/* Register the vertex callbacks. */
    case GLU_TESS_VERTEX:
	tobj->callbacks.vertex = (void (GLCALLBACKP)(void *)) fn;
	break;
    case GLU_TESS_VERTEX_DATA:
	tobj->callbacks.vertexData = (void (GLCALLBACKP)(void *, void *)) fn;
	break;

	/* Register the end callbacks. */
    case GLU_TESS_END:
	tobj->callbacks.end = (void (GLCALLBACKP)(void)) fn;
	break;
    case GLU_TESS_END_DATA:
	tobj->callbacks.endData = (void (GLCALLBACKP)(void *)) fn;
	break;

	/* Register the error callbacks. */
    case GLU_TESS_ERROR:
	tobj->callbacks.error = (void (GLCALLBACKP)(GLenum)) fn;
	break;
    case GLU_TESS_ERROR_DATA:
	tobj->callbacks.errorData = (void (GLCALLBACKP)(GLenum, void *)) fn;
	break;

	/* Register the combine callbacks. */
    case GLU_TESS_COMBINE:
	tobj->callbacks.combine =
		(void (GLCALLBACKP)(GLdouble[3], void *[4],
				GLfloat [4], void **)) fn;
	break;
    case GLU_TESS_COMBINE_DATA:
	tobj->callbacks.combineData =
		(void (GLCALLBACKP)(GLdouble[3], void *[4], GLfloat [4],
				void **, void *)) fn;
	break;

    default:
	MSG( 1, "  gluTessCallback( tobj:%p which:%d ) invalid enum\n", tobj, which );
	tobj->error = GLU_INVALID_ENUM;
	break;
    }
}


/*****************************************************************************
 * gluTessProperty
 *
 * Set the current value of the given property.
 *****************************************************************************/
void GLAPIENTRY gluTessProperty( GLUtesselator *tobj, GLenum which,
				 GLdouble value )
{
    switch ( which )
    {
    case GLU_TESS_BOUNDARY_ONLY:
	tobj->boundary_only = (GLboolean) value;
	break;

    case GLU_TESS_TOLERANCE:
	MSG( 15, "   gluTessProperty( tobj:%p ) tolerance: %0.9f\n", tobj, value );
	tobj->tolerance = value;
	break;

    case GLU_TESS_WINDING_RULE:
	tobj->winding_rule = (GLenum) value;
	break;

    default:
	MSG( 1, "   gluTessProperty( tobj:%p which:%d ) invalid enum\n", tobj, which );
	tobj->error = GLU_INVALID_ENUM;
	break;
    }
}


/*****************************************************************************
 * gluGetTessProperty
 *
 * Return the current value of the given property.
 *****************************************************************************/
void GLAPIENTRY gluGetTessProperty( GLUtesselator *tobj, GLenum which,
				    GLdouble *value )
{
    switch ( which )
    {
    case GLU_TESS_BOUNDARY_ONLY:
	*value = tobj->boundary_only;
	break;

    case GLU_TESS_TOLERANCE:
	*value = tobj->tolerance;
	break;

    case GLU_TESS_WINDING_RULE:
	*value = tobj->winding_rule;
	break;

    default:
	MSG( 1, "   gluGetTessProperty( tobj:%p which:%d ) invalid enum\n", tobj, which );
	tobj->error = GLU_INVALID_ENUM;
	break;
    }
}


/*****************************************************************************
 * gluTessNormal
 *
 * Set the current tessellation normal.
 *****************************************************************************/
void GLAPIENTRY gluTessNormal( GLUtesselator *tobj, GLdouble x,
			       GLdouble y, GLdouble z )
{
    MSG( 15, "   gluTessNormal( tobj:%p n:(%.2f,%.2f,%.2f) )\n", tobj, x, y, z );

    tobj->plane.normal[X] = x;
    tobj->plane.normal[Y] = y;
    tobj->plane.normal[Z] = z;
}



/*****************************************************************************
 *
 *			OBSOLETE TESSELLATION FUNCTIONS
 *
 *****************************************************************************/

void GLAPIENTRY gluBeginPolygon( GLUtesselator *tobj )
{
    gluTessBeginPolygon( tobj, NULL );
    gluTessBeginContour( tobj );
}

void GLAPIENTRY gluNextContour( GLUtesselator *tobj, GLenum type )
{
    gluTessEndContour( tobj );
    gluTessBeginContour( tobj );
}

void GLAPIENTRY gluEndPolygon( GLUtesselator *tobj )
{
    gluTessEndContour( tobj );
    gluTessEndPolygon( tobj );
}



/*****************************************************************************
 * tess_error_callback
 *
 * Internal error handler.  Call the user-registered error callback.
 *
 * 2nd arg changed from 'errno' to 'errnum' since MSVC defines errnum as
 *  a macro (of all things) and thus breaks the build -tjump
 *****************************************************************************/

void tess_error_callback( GLUtesselator *tobj, GLenum errnum )
{
    if ( tobj->error == GLU_NO_ERROR )
    {
	tobj->error = errnum;
    }

    if ( tobj->callbacks.errorData != NULL )
    {
	( tobj->callbacks.errorData )( errnum, tobj->data );
    }
    else if ( tobj->callbacks.error != NULL )
    {
	( tobj->callbacks.error )( errnum );
    }
}



/*****************************************************************************
 *
 *				INTERNAL FUNCTIONS
 *
 *****************************************************************************/


/*****************************************************************************
 * init_callbacks
 *****************************************************************************/
static void init_callbacks( tess_callbacks_t *callbacks )
{
	callbacks->begin        = ( void (GLCALLBACKP)(GLenum) ) NULL;
	callbacks->beginData    = ( void (GLCALLBACKP)(GLenum, void *) ) NULL;
	callbacks->edgeFlag     = ( void (GLCALLBACKP)(GLboolean) ) NULL;
	callbacks->edgeFlagData = ( void (GLCALLBACKP)(GLboolean, void *) ) NULL;
	callbacks->vertex       = ( void (GLCALLBACKP)(void *) ) NULL;
	callbacks->vertexData   = ( void (GLCALLBACKP)(void *, void *) ) NULL;
	callbacks->end          = ( void (GLCALLBACKP)(void) ) NULL;
	callbacks->endData      = ( void (GLCALLBACKP)(void *) ) NULL;
	callbacks->error        = ( void (GLCALLBACKP)(GLenum) ) NULL;
	callbacks->errorData    = ( void (GLCALLBACKP)(GLenum, void *) ) NULL;
	callbacks->combine      = ( void (GLCALLBACKP)(GLdouble [3], void *[4],
						   GLfloat [4], void **) ) NULL;
	callbacks->combineData  = ( void (GLCALLBACKP)(GLdouble [3], void *[4],
						   GLfloat [4], void **,
						   void *) ) NULL;
}


/*****************************************************************************
 * tess_cleanup
 *****************************************************************************/
static void tess_cleanup( GLUtesselator *tobj )
{
    MSG( 15, "  -> tess_cleanup( tobj:%p )\n", tobj );

    if ( tobj->current_contour != NULL ) {
	delete_contour( &tobj->current_contour );
    }
    if ( tobj->contours != NULL ) {
	delete_all_contours( tobj );
    }

    MSG( 15, "  <- tess_cleanup( tobj:%p )\n", tobj );
}


/*****************************************************************************
 * inspect_current_contour
 *****************************************************************************/
static GLenum	find_normal( GLUtesselator *tobj );
static void	project_current_contour( GLUtesselator *tobj );
static GLenum	save_current_contour( GLUtesselator *tobj );

static void inspect_current_contour( GLUtesselator *tobj )
{
    tess_contour_t *current = tobj->current_contour;
    GLdouble origin[3] = { 0.0, 0.0, 0.0 };

    MSG( 15, "    -> inspect_current_contour( tobj:%p )\n", tobj );

    if ( current->vertex_count < 3 )
    {
	MSG( 15, "         count %d < 3, deleting\n", current->vertex_count );
	delete_contour( &tobj->current_contour );
	return;
    }

    current->last_vertex->next = current->vertices;
    current->vertices->previous = current->last_vertex;

    if ( ( tobj->contours == NULL ) &&
	 ( COMPARE_3DV( current->plane.normal, origin ) ) )
    {
	/* We haven't been given a normal, so let's take a guess. */
	if ( find_normal( tobj ) == GLU_ERROR ) {
	    return;
	}
	COPY_3V( tobj->plane.normal, current->plane.normal );
	tobj->plane.dist = current->plane.dist;
    }
    else
    {
	MSG( 15, "         normal: (%.2f,%.2f,%.2f)\n", tobj->plane.normal[X], tobj->plane.normal[Y], tobj->plane.normal[Z] );
    }

    project_current_contour( tobj );

    if ( save_current_contour( tobj ) == GLU_ERROR ) {
	return;
    }

    MSG( 15, "    <- inspect_current_contour( tobj:%p )\n", tobj );
}

/*****************************************************************************
 * find_normal
 *****************************************************************************/
static GLenum find_normal( GLUtesselator *tobj )
{
    tess_contour_t	*contour = tobj->current_contour;
    tess_vertex_t	*va, *vb, *vc;
    GLdouble		a[3], b[3], c[3];

    MSG( 15, "      -> find_normal( tobj:%p )\n", tobj );

    if ( contour == NULL ) { return GLU_ERROR; }

    va = contour->vertices;
    vb = va->next;

    /* If va and vb are the same point, keep looking for a different vertex. */

    while ( COMPARE_3DV( va->coords, vb->coords ) && ( vb != va ) ) {
	vb = vb->next;
    }

    if ( vb == va ) {
	/* FIXME: What error is this? */
	tess_error_callback( tobj, GLU_TESS_ERROR7 );
    }

    SUB_3V( a, vb->coords, va->coords );

    for ( vc = vb->next; vc != va; vc = vc->next )
    {
	SUB_3V( b, vc->coords, va->coords );

	CROSS3( c, a, b );

	if ( ( ABSD( c[X] ) > EQUAL_EPSILON ) ||
	     ( ABSD( c[Y] ) > EQUAL_EPSILON ) ||
	     ( ABSD( c[Z] ) > EQUAL_EPSILON ) )
	{
	    COPY_3V( contour->plane.normal, c );
	    NORMALIZE_3DV( contour->plane.normal );

	    contour->plane.dist = - DOT3( contour->plane.normal, va->coords );

	    MSG( 15, "      <- find_normal( tobj:%p ) n: (%.2f,%.2f,%.2f)\n", tobj, contour->plane.normal[X], contour->plane.normal[Y], contour->plane.normal[Z] );
	    return GLU_NO_ERROR;
	}
    }
    /* FIXME: What error is this? */
    tess_error_callback( tobj, GLU_TESS_ERROR7 );

    return GLU_ERROR;
}

/*****************************************************************************
 * project_current_contour
 *****************************************************************************/
static GLdouble twice_contour_area( tess_vertex_t *vertex,
				    tess_vertex_t *last_vertex );

static void project_current_contour( GLUtesselator *tobj )
{
    tess_contour_t	*current = tobj->current_contour;
    tess_vertex_t	*vertex;
    GLdouble		area;
    GLdouble		zaxis[3] = { 0.0, 0.0, 1.0 }, znormal[3], xnormal[3];
    GLdouble		dot, rotx, roty;
    GLuint		i;

    MSG( 15, "      -> project_current_contour( tobj:%p )\n", tobj );

    if ( current == NULL ) { return; }

    /* Rotate the plane normal around the y-axis. */

    znormal[X] = current->plane.normal[X];
    znormal[Y] = 0.0;
    znormal[Z] = current->plane.normal[Z];

    dot = DOT3( znormal, zaxis );
    current->roty = roty = acos( dot );

    /* Rotate the plane normal around the x-axis. */

    xnormal[X] = cos( roty ) * znormal[X] - sin( roty ) * znormal[Z];
    xnormal[Y] = znormal[Y];
    xnormal[Z] = sin( roty ) * znormal[X] + cos( roty ) * znormal[Z];

    dot = DOT3( xnormal, zaxis );
    current->rotx = rotx = acos( dot );

    for ( vertex = current->vertices, i = 0;
	  i < current->vertex_count; vertex = vertex->next, i++ )
    {
	tess_plane_t	*plane = &current->plane;
	GLdouble	proj[3], yrot[3], xrot[3];

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

	ACC_BBOX_2V( vertex->v, tobj->mins, tobj->maxs );
	ACC_BBOX_2V( vertex->v, current->mins, current->maxs );
    }

    area = twice_contour_area( current->vertices,
			       current->last_vertex );
    if ( area >= 0.0 )
    {
	current->orientation = GLU_CCW;
	current->area = area;
    }
    else
    {
	current->orientation = GLU_CW;
	current->area = -area;
    }

    MSG( 15, "      <- project_current_contour( tobj:%p )\n", tobj );
}

/*****************************************************************************
 * twice_contour_area
 *****************************************************************************/
static GLdouble twice_contour_area( tess_vertex_t *vertex,
				    tess_vertex_t *last_vertex )
{
    tess_vertex_t	*next;
    GLdouble		area, x, y;

    area = 0.0;

    x = vertex->v[X];
    y = vertex->v[Y];

    vertex = vertex->next;

    while ( vertex != last_vertex )
    {
	next = vertex->next;
	area +=
	    (vertex->v[X] - x) * (next->v[Y] - y) -
	    (vertex->v[Y] - y) * (next->v[X] - x);

	vertex = vertex->next;
    }
    return area;
}


/*****************************************************************************
 * save_current_contour
 *****************************************************************************/
static GLenum save_current_contour( GLUtesselator *tobj )
{
    tess_contour_t	*current = tobj->current_contour;
    tess_vertex_t	*vertex;
    GLuint			i;

    if ( current == NULL ) { return GLU_ERROR; }

    if ( tobj->contours == NULL )
    {
	tobj->contours = tobj->last_contour = current;
	current->next = current->previous = NULL;
    }
    else
    {
	current->previous = tobj->last_contour;

	tobj->last_contour->next = current;
	tobj->last_contour = current;

	current->next = NULL;
    }

    for ( vertex = current->vertices, i = 0;
	  i < current->vertex_count; vertex = vertex->next, i++ )
    {
	vertex->shadow_vertex = NULL;
	vertex->edge_flag = GL_TRUE;
    }

    current->type = GLU_UNKNOWN;

    tobj->contour_count++;
    tobj->current_contour = NULL;

    return GLU_NO_ERROR;
}

/*****************************************************************************
 * delete_contour
 *
 * Delete the given contour and set the pointer to NULL.
 *****************************************************************************/
void delete_contour( tess_contour_t **contour )
{
    tess_vertex_t	*vertex, *next;
    GLuint		i;

    if ( *contour == NULL ) { return; }

    vertex = (*contour)->vertices;

    for ( i = 0 ; i < (*contour)->vertex_count ; i++ )
    {
	next = vertex->next;
	free( vertex );
	vertex = next;
    }

    free( *contour );
    *contour = NULL;
}

/*****************************************************************************
 * delete_all_contours
 *****************************************************************************/
static void delete_all_contours( GLUtesselator *tobj )
{
    tess_contour_t	*current, *next_contour;
    GLuint		i;

    if ( tobj->current_contour != NULL ) {
	delete_contour( &tobj->current_contour );
    }

    for ( current = tobj->contours, i = 0 ; i < tobj->contour_count ; i++ )
    {
	tess_vertex_t	*vertex = current->vertices, *next_vertex;
	GLuint		j;

	for ( j = 0 ; j < current->vertex_count ; j ++ )
	{
	    next_vertex = vertex->next;
	    free( vertex );
	    vertex = next_vertex;
	}
	next_contour = current->next;

	free( current );
	current = next_contour;
    }

    tobj->contour_count = tobj->vertex_count = 0;
    tobj->contours = tobj->last_contour = NULL;

    CLEAR_BBOX_2DV( tobj->mins, tobj->maxs );

    ZERO_3V( tobj->plane.normal );
    tobj->plane.dist = 0.0;
}


/*****************************************************************************
 * tess_msg
 *****************************************************************************/
INLINE void tess_msg( int level, char *format, ... )
{
#ifdef DEBUG
    va_list ap;
    va_start( ap, format );

    if ( level <= tess_dbg_level ) {
	vfprintf( DBG_STREAM, format, ap );
	fflush( DBG_STREAM );
    }

    va_end( ap );
#endif
}

INLINE void tess_info( char *file, char *line )
{
#ifdef DEBUG
    fprintf( DBG_STREAM, "%9.9s:%d:\t ", file, line );
#endif
}
