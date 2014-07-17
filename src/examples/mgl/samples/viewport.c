/****************************************************************************
*
*					SciTech Multi-platform Graphics Library
*
*  ========================================================================
*
*    The contents of this file are subject to the SciTech MGL Public
*    License Version 1.0 (the "License"); you may not use this file
*    except in compliance with the License. You may obtain a copy of
*    the License at http://www.scitechsoft.com/mgl-license.txt
*
*    Software distributed under the License is distributed on an
*    "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
*    implied. See the License for the specific language governing
*    rights and limitations under the License.
*
*    The Original Code is Copyright (C) 1991-1998 SciTech Software, Inc.
*
*    The Initial Developer of the Original Code is SciTech Software, Inc.
*    All Rights Reserved.
*
*  ========================================================================
*
* Language:		ANSI C
* Environment:	Any
*
* Description:  Simple demo showing how to use the viewport functions to
*				move the output of a simple drawing to a different
*				location on the screen.
*
****************************************************************************/

#include "mglsamp.h"

#define IMAGE_NAME "frog.bmp"

/*----------------------------- Global Variables --------------------------*/

char demoName[] = "viewport";

/*------------------------------ Implementation ---------------------------*/

void demo(MGLDC *dc)
{
	bitmap_t *bitmap = NULL;
	rect_t rect = {0};

	/* Display title message at the top of the window. */
	mainWindow(dc,"Viewport Demo");

	/* Load a bitmap and the palette from the image file. */
	if((bitmap = MGL_loadBitmap(IMAGE_NAME,TRUE)) == NULL) 
		MGL_fatalError(MGL_errorMsg(MGL_result()));

	/* Set the display device context with the palette from the
	 * memory device context.
	 */
	MGL_setPalette(dc,bitmap->pal,MGL_getPaletteSize(dc),0);
	MGL_realizePalette(dc,MGL_getPaletteSize(dc),0,true);

	/* Put the image onto the display device context. */
	MGL_putBitmapSrcTrans(dc,0,0,bitmap,0,MGL_REPLACE_MODE);

 	/* change the dimensions of the current active viewport */
	rect.left = bitmap->width/3;
	rect.top = bitmap->height/3;
	rect.right = rect.left + bitmap->width;
	rect.bottom = rect.top + bitmap->height;
	MGL_setViewportDC(dc,rect);

	/* Put the image onto the display device context. */
	MGL_putBitmapSrcTrans(dc,0,0,bitmap,0,MGL_REPLACE_MODE);

	/* Pause until the user hits a key or presses a mouse button. */
	pause();
}
