/* $Id: colortab.h,v 1.4 1999/11/12 08:32:37 joukj Exp $ */

/*
 * Mesa 3-D graphics library
 * Version:  3.3
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


#ifndef COLORTAB_H
#define COLORTAB_H


#include "types.h"


extern void
_mesa_ColorTableEXT( GLenum target, GLenum internalformat,
                     GLsizei width, GLenum format, GLenum type,
                     const GLvoid *table );

extern void
_mesa_ColorSubTableEXT( GLenum target, GLsizei start,
                        GLsizei count, GLenum format, GLenum type,
                        const GLvoid *table );

extern void
_mesa_GetColorTableEXT( GLenum target, GLenum format,
                        GLenum type, GLvoid *table );
#ifdef VMS
/* VMS does not support externals longer than 32 characters. */
#define _mesa_GetColorTableParameterfvEXT VMS_GCTParameterfvEXT
#define _mesa_GetColorTableParameterivEXT VMS_GCTParameterivEXT
#endif

extern void
_mesa_GetColorTableParameterfvEXT( GLenum target, GLenum pname, GLfloat *params );

extern void
_mesa_GetColorTableParameterivEXT( GLenum target, GLenum pname, GLint *params );


#endif
