/* $Id: tess_winding.h,v 1.3 1999/10/13 19:13:09 gareth Exp $ */

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

#ifndef __GLU_TESS_WINDING_H__
#define __GLU_TESS_WINDING_H__

#include "tess_typedefs.h"

#ifdef __cplusplus
extern "C" {
#endif

GLenum tess_calc_intersections( GLUtesselator *tobj );
GLenum tess_preprocess_contours( GLUtesselator *tobj );

#ifdef __cplusplus
}
#endif

#endif /* __GLU_TESS_WINDING_H__ */
