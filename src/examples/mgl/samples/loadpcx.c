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
* Description:  Simple demo showing how to load a pcx file from disk and
*				display it on the screen.
*
****************************************************************************/

#include "mglsamp.h"

/*----------------------------- Global Variables --------------------------*/

char demoName[] = "loadpcx";

/*------------------------------ Implementation ---------------------------*/

void demo(MGLDC *dc)
{
	int 		posx,posy;
	bitmap_t	*bitmap;

	/* Display title message at the top of the window. */
	mainWindow(dc,"PCX Loading Demo (8-bit color)");

	/* Load a pcx and the palette from the image file. */
	if ((bitmap = MGL_loadPCX("doggie2.pcx",true)) == NULL)
		MGL_fatalError(MGL_errorMsg(MGL_result()));

	/* Set the display device context with the palette from the
	 * memory device context.
	 */
	MGL_setPalette(dc,bitmap->pal,MGL_getPaletteSize(dc),0);
	MGL_realizePalette(dc,MGL_getPaletteSize(dc),0,true);

	/* Justify the image that will be put on the screen */
	posx = (MGL_sizex(dc)-bitmap->width)/2;
	posy = (MGL_sizey(dc)-bitmap->height)/2;

	/* Put the image onto the display device context. */
	MGL_putBitmap(dc,posx,posy,bitmap,MGL_REPLACE_MODE);

	/* Remove all bitmaps from memory. */
    MGL_unloadBitmap(bitmap);

	/* Pause until the user hits a key or presses a mouse button. */
	if (!pause())
		return;

	/*------------------------------------------------------------------
	 * Load a bitmap from the image file as a 8-bit color image.
	 */
	mainWindow(dc,"PCX Loading Demo (8-bit color into DC)");

	if (!MGL_loadPCXIntoDC(dc,"doggie2.pcx",posx,posy,true))
		MGL_fatalError(MGL_errorMsg(MGL_result()));

	/* Pause until the user hits a key or presses a mouse button. */
	if (!pause())
		return;
}
