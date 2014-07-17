/* $Id: tess_macros.h,v 1.7 1999/11/11 03:21:43 kendallb Exp $ */

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

#ifndef __GLU_TESS_MACROS_H__
#define __GLU_TESS_MACROS_H__

#ifdef	SCITECH
#include "../mesa/macros.h"
#include "../mesa/mmath.h"
#else
#include "../src/macros.h"
#include "../src/mmath.h"
#endif

#define X	0
#define Y	1
#define Z	2

/* Limits: */
#ifndef DBL_MAX
#define DBL_MAX		1E+37
#endif

#define MAX_GLDOUBLE	DBL_MAX
#define EQUAL_EPSILON	1E-09


/* Radians to degrees conversion: */
#define DEG_TO_RAD( A )		( (A) * ( M_PI / 180.0 ) )
#define RAD_TO_DEG( A )		( (A) * ( 180.0 / M_PI ) )


/* Comparison: */

#define COMPARE_3DV( A, B )				\
( ( ABSD( (A)[X] - (B)[X] ) < EQUAL_EPSILON ) &&	\
  ( ABSD( (A)[Y] - (B)[Y] ) < EQUAL_EPSILON ) &&	\
  ( ABSD( (A)[Z] - (B)[Z] ) < EQUAL_EPSILON ) )


/* Length: */

#define LEN_3DV( V )		LEN_3FV( V )

#define LEN_SCALAR( V1, V2 )	( GL_SQRT( (V1)*(V1) + (V2)*(V2) ) )

#define NORMALIZE_3DV( V )	NORMALIZE_3FV( V )

#define NORMALIZE_2DV( V )			\
do {						\
   GLdouble len = LEN_SQUARED_2DV( V );		\
   if ( len > 1e-50 ) {				\
      len = 1.0 / GL_SQRT( len );		\
      (V)[0] *= len;				\
      (V)[1] *= len;				\
   }						\
} while(0)

#define LEN_2DV( V )		( GL_SQRT( (V)[0] * (V)[0] + (V)[1] * (V)[1] ) )

#define LEN_SQUARED_2DV( V )	( (V)[0] * (V)[0] + (V)[1] * (V)[1] )


/* Bounding boxes: */

#define CLEAR_BBOX_2DV( MINS, MAXS )		\
do {						\
    (MINS)[X] = (MINS)[Y] = MAX_GLDOUBLE;	\
    (MAXS)[X] = (MAXS)[Y] = -MAX_GLDOUBLE;	\
} while ( 0 )

#define ACC_BBOX_2V( V, MINS, MAXS )		\
do {						\
    if ( (V)[X] < (MINS)[X] ) {			\
	(MINS)[X] = (V)[X];			\
    }						\
    if ( (V)[X] > (MAXS)[X] ) {			\
	(MAXS)[X] = (V)[X];			\
    }						\
    if ( (V)[Y] < (MINS)[Y] ) {			\
	(MINS)[Y] = (V)[Y];			\
    }						\
    if ( (V)[Y] > (MAXS)[Y] ) {			\
	(MAXS)[Y] = (V)[Y];			\
    }						\
} while ( 0 )

/* Test if bbox B is inside bbox A: */
#define CONTAINS_BBOX_2V( AMINS, AMAXS, BMINS, BMAXS )			\
( ( (AMINS)[X] < (BMINS)[X] ) && ( (AMINS)[Y] < (BMINS)[Y] ) &&		\
  ( (AMAXS)[X] > (BMAXS)[X] ) && ( (AMAXS)[Y] > (BMAXS)[Y] ) )

#define INSIDE_BBOX_2V( V, MINS, MAXS )			\
( ( (MINS)[X] < (V)[X] ) && ( (MINS)[Y] < (V)[Y] ) &&	\
  ( (MAXS)[X] > (V)[X] ) && ( (MAXS)[Y] > (V)[Y] ) )

#endif /* __GLU_TESS_MACROS_H__ */
