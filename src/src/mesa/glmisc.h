/* $Id: glmisc.h,v 1.2 1999/11/11 01:22:26 brianp Exp $ */

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


#ifndef GLMISC_H
#define GLMISC_H


#include "types.h"


extern void
_mesa_ClearIndex( GLfloat c );

extern void
_mesa_ClearColor( GLclampf red, GLclampf green,
                  GLclampf blue, GLclampf alpha );

extern void
_mesa_Clear( GLbitfield mask );

extern void
_mesa_Finish( void );

extern void
_mesa_Flush( void );

extern GLboolean
_mesa_try_Hint( GLcontext *ctx, GLenum target, GLenum mode );

extern void
_mesa_Hint( GLenum target, GLenum mode );

extern void
_mesa_DrawBuffer( GLenum mode );

extern void
_mesa_ReadBuffer( GLenum mode );


#endif
