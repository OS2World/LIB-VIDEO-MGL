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
* Description:  Simple sample program showing how to use the bltBlt
*				functions to blt an image from a memory dc to the screen,
*				and between areas on the same screen.
*
****************************************************************************/

#include "mglsamp.h"

#define IMAGE_NAME "frog.bmp"

/*----------------------------- Global Variables --------------------------*/

char demoName[] = "bitblit";

/*------------------------------ Implementation ---------------------------*/

void demo(MGLDC *dc)
{
	int width,height,depth;
	palette_t pal[256];
	rect_t rect = {0};
	pixel_format_t pf;
	MGLDC *memdc;
	
	/* Display title message at the top of the window. */
	mainWindow(dc,"Bit Blit Demo");

	/* Get image information */
	MGL_getBitmapSize(IMAGE_NAME,&width,&height,&depth,&pf);

	/* Create a memory device context. */ 
	if ((memdc = MGL_createMemoryDC(width,height,depth,&pf)) == NULL)
		MGL_fatalError(MGL_errorMsg(MGL_result()));

	/* Load a bitmap into the memory device context. The palette gets
	 * loaded into the memory device context as well.
	 */ 
	if(MGL_loadBitmapIntoDC(memdc,IMAGE_NAME,0,0,TRUE) == NULL) 
		MGL_fatalError(MGL_errorMsg(MGL_result()));

	/* Set the display device context with the palette from the
	 * memory device context.
	 */
	if (depth <= 8) {
		MGL_getPalette(memdc,pal,MGL_getPaletteSize(memdc),0);
		MGL_setPalette(dc,pal,MGL_getPaletteSize(memdc),0);
		MGL_realizePalette(dc,MGL_getPaletteSize(memdc),0,true);
		}
	
	rect.right = width;
	rect.bottom = height;

	/* Copy image from image in memory to the screen. */
	MGL_bitBlt(dc,memdc,rect,0,0,MGL_REPLACE_MODE);

	/* Copy image from screen to screen with an overlapping area. */
	MGL_bitBlt(dc,dc,rect,width/3,height/3,MGL_REPLACE_MODE);

	/* Remove all device contexts from memory. */
    MGL_destroyDC(memdc);

	/* Pause until the user hits a key or presses a mouse button. */
	pause();
}








