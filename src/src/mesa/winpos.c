/* $Id: winpos.c,v 1.3 1999/11/11 01:22:28 brianp Exp $ */

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


/*
 * GL_MESA_window_pos extension
 *
 * This extension offers a set of functions named glWindowPos*MESA() which
 * directly set the current raster position to the given window coordinate.
 * glWindowPos*MESA() are similar to glRasterPos*() but bypass the
 * modelview, projection and viewport transformations.
 *
 * These functions should be very handy in conjunction with glDrawPixels()
 * and glCopyPixels().
 *
 * If your application uses glWindowPos*MESA() and may be compiled with
 * a real OpenGL instead of Mesa you can simply copy the winpos.[ch] files
 * into your source tree and compile them with the rest of your code
 * since glWindowPos*MESA() can, and is, implemented in terms of standard
 * OpenGL commands when not using Mesa.  In your source files which use
 * glWindowPos*MESA() just #include "winpos.h".
 */


#ifdef PC_HEADER
#include "all.h"
#else
#include "glheader.h"
#include "context.h"
#include "feedback.h"
#include "mmath.h"
#include "rastpos.h"
#include "winpos.h"
#endif


/*
 * This is a MESA extension function.  Pretty much just like glRasterPos
 * except we don't apply the modelview or projection matrices; specify a
 * window coordinate directly.
 * Caller:  context->API.WindowPos4fMESA pointer.
 */
void
_mesa_WindowPos4fMESA( GLfloat x, GLfloat y, GLfloat z, GLfloat w )
{
   /* KW: Assume that like rasterpos, this must be outside begin/end.
    */
   GET_CURRENT_CONTEXT(ctx);
   ASSERT_OUTSIDE_BEGIN_END_AND_FLUSH( ctx, "glWindowPosMESA" );

   /* set raster position */
   ctx->Current.RasterPos[0] = x;
   ctx->Current.RasterPos[1] = y;
   ctx->Current.RasterPos[2] = CLAMP( z, 0.0F, 1.0F );
   ctx->Current.RasterPos[3] = w;

   ctx->Current.RasterPosValid = GL_TRUE;

   /* raster color */
   if (0 && ctx->Light.Enabled) {

      /* KW: I don't see how this can work - would have to take the
       *     inverse of the projection matrix or the combined
       *     modelProjection matrix, transform point and normal, and
       *     do the lighting.  Those inverses are not used for
       *     anything else.  This is not an object-space lighting
       *     issue - what this is trying to do is something like
       *     clip-space or window-space lighting...
       *
       *     Anyway, since the implementation was never correct, I'm
       *     not fixing it now - just use the unlit color. 
       */

      /* KW:  As a reprise, we now *do* keep the inverse of the projection
       *      matrix, so it is not infeasible to try to swim up stream
       *      in this manner.  I still don't want to implement it,
       *      however.
       */
   }
   else {
      /* use current color or index */
      if (ctx->Visual->RGBAflag) {
	 UBYTE_RGBA_TO_FLOAT_RGBA(ctx->Current.RasterColor, 
				  ctx->Current.ByteColor);
      }
      else {
	 ctx->Current.RasterIndex = ctx->Current.Index;
      }
   }

   ctx->Current.RasterDistance = 0.0;

   {
      GLuint texSet;
      for (texSet=0; texSet<MAX_TEXTURE_UNITS; texSet++) {
         COPY_4FV( ctx->Current.RasterMultiTexCoord[texSet],
                  ctx->Current.Texcoord[texSet] );
      }
   }

   if (ctx->RenderMode==GL_SELECT) {
      gl_update_hitflag( ctx, ctx->Current.RasterPos[2] );
   }
}


#ifndef GL_MESA_window_pos

/*
 * OpenGL implementation of glWindowPos*MESA()
 */
void glWindowPos4fMESA( GLfloat x, GLfloat y, GLfloat z, GLfloat w )
{
   GLfloat fx, fy;

   /* Push current matrix mode and viewport attributes */
   glPushAttrib( GL_TRANSFORM_BIT | GL_VIEWPORT_BIT );

   /* Setup projection parameters */
   glMatrixMode( GL_PROJECTION );
   glPushMatrix();
   glLoadIdentity();
   glMatrixMode( GL_MODELVIEW );
   glPushMatrix();
   glLoadIdentity();

   glDepthRange( z, z );
   glViewport( (int) x - 1, (int) y - 1, 2, 2 );

   /* set the raster (window) position */
   fx = x - (int) x;
   fy = y - (int) y;
   glRasterPos4f( fx, fy, 0.0, w );

   /* restore matrices, viewport and matrix mode */
   glPopMatrix();
   glMatrixMode( GL_PROJECTION );
   glPopMatrix();

   glPopAttrib();
}

#endif
