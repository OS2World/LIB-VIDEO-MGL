/* $Id: tess_typedefs.h,v 1.10 1999/11/11 03:21:43 kendallb Exp $ */

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

#ifndef __GLU_TESS_TYPEDEFS_H__
#define __GLU_TESS_TYPEDEFS_H__

#include <stdarg.h>
#include <stdio.h>

#include "gluP.h"

#include "tess_hash.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * Tessellation type definitions:
 *****************************************************************************/
typedef struct
{
	void (GLCALLBACKP begin)( GLenum );
	void (GLCALLBACKP beginData)( GLenum , void * );
	void (GLCALLBACKP edgeFlag)( GLboolean );
	void (GLCALLBACKP edgeFlagData)( GLboolean , void * );
	void (GLCALLBACKP vertex)( void * );
	void (GLCALLBACKP vertexData)( void *, void * );
	void (GLCALLBACKP end)( void );
	void (GLCALLBACKP endData)( void * );
	void (GLCALLBACKP error)( GLenum );
	void (GLCALLBACKP errorData)( GLenum , void * );
	void (GLCALLBACKP combine)( GLdouble[3], void *[4],
				GLfloat[4], void ** );
	void (GLCALLBACKP combineData)( GLdouble[3], void *[4],
				    GLfloat[4], void **,
				    void * );
} tess_callbacks_t;

typedef struct
{
    GLdouble		normal[3];
    GLdouble		dist;
} tess_plane_t;

typedef struct vertex_s
{
    GLuint		index;
    void		*data;
    GLdouble		coords[3];
    GLdouble		v[2];
    GLboolean		edge_flag;
    GLdouble		side;
    GLuint		label;
    GLuint		mark;
    struct vertex_s	*shadow_vertex;
    struct vertex_s	*next, *previous;
} tess_vertex_t;

typedef struct contour_s
{
    GLenum		type;
    tess_plane_t	plane;
    GLdouble		area;
    GLenum		orientation;
    GLuint		label;
    GLint		winding;
    GLdouble		rotx, roty;
    GLdouble		mins[2], maxs[2];
    GLuint		vertex_count;
    tess_vertex_t	*vertices, *last_vertex;
    hashtable_t		*reflex_vertices;
    hashtable_t		*cross_vertices;
    struct contour_s	*next, *previous;
} tess_contour_t;

#ifdef __cplusplus
}
#endif

#endif /* __GLU_TESS_TYPEDEFS_H__ */
