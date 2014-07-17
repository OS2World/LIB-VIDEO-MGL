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
* Description:  Demo showing how to use offscreen dc's to store sprites and
*				to use harware acceleration to blt them from the offscreen
*				dc to the display dc. Also shows falling back to a memory
*				dc if an offscreen dc is not available.
*
****************************************************************************/

#include "mglsamp.h"

#define IMAGE_NAME "doggie2.bmp"

/*----------------------------- Global Variables --------------------------*/

char 			demoName[] = "sprites";
int 			bmpWidth,bmpHeight,bmpDepth;
int 			maxx,maxy;
pixel_format_t 	bmpPF;

/*------------------------------ Implementation ---------------------------*/

/****************************************************************************
PARAMETERS:
dc     		- Display dc
trans  		- Turn on/off transparent blitting

REMARKS:
Demo showing how to use memory dc's to store sprites and to use blit them
from the memory dc to the display dc.
****************************************************************************/
ibool systemBlit(
	MGLDC *dc,
	ibool trans)
{
	color_t 		transparent;
	palette_t 		pal[256];
	pixel_format_t 	pf;
	MGLDC 			*memdc;

	/* Display title message at the top of the window. */
	if (trans)
		mainWindow(dc,"System Memory Transparent Blit Demo");
	else 
		mainWindow(dc,"System Memory Blit Demo");
    statusLine("Press any key to continue, ESC to Abort");

	/* Create a memory device context */
	MGL_getPixelFormat(dc,&pf);
	if ((memdc = MGL_createMemoryDC(bmpWidth,bmpHeight,MGL_getBitsPerPixel(dc),&pf)) == NULL)
		MGL_fatalError(MGL_errorMsg(MGL_result()));

	/* Load a bitmap into the memory device context. The palette gets
	 * loaded into the memory device context as well.
	 */
	if (!MGL_loadBitmapIntoDC(memdc,IMAGE_NAME,0,0,true))
		MGL_fatalError(MGL_errorMsg(MGL_result()));

	/* Set the display device context with the palette from the
	 * memory device context.
	 */
	if (MGL_getBitsPerPixel(dc) <= 8) {
		MGL_getPalette(memdc,pal,MGL_getPaletteSize(memdc),0);
		MGL_setPalette(dc,pal,MGL_getPaletteSize(memdc),0);
		MGL_realizePalette(dc,MGL_getPaletteSize(memdc),0,true);
		}

	/* Get transparent color from pixel (0,0) */
    MGL_makeCurrentDC(memdc);
	transparent = MGL_getPixelCoord(0,0);
    MGL_makeCurrentDC(dc);

	/* Copy image from image in memory to the screen. */
    while (!checkEvent()) {
		if (trans)
			MGL_srcTransBltCoord(dc,memdc,0,0,bmpWidth,bmpHeight,MGL_random(maxx-bmpWidth),MGL_random(maxy-bmpHeight),transparent,MGL_REPLACE_MODE);
		else
			MGL_bitBltCoord(dc,memdc,0,0,bmpWidth,bmpHeight,MGL_random(maxx-bmpWidth),MGL_random(maxy-bmpHeight),MGL_REPLACE_MODE);
		}

	/* Remove all device contexts from memory. */
    MGL_destroyDC(memdc);

	return pause();
}

/****************************************************************************
PARAMETERS:
dc     		- Display dc
trans  		- Turn on/off transparent blitting

REMARKS:
Demo showing how to use offscreen buffers to store sprites and to use
hardware acceleration to blt them to the display dc.
****************************************************************************/
ibool bufferBlit(
	MGLDC *dc,
	ibool trans)
{
	MGLBUF			*buf = NULL;
	color_t 		transparent;
	palette_t 		pal[256];
	pixel_format_t 	pf;
	MGLDC 			*memdc;

	/* Display title message at the top of the window. */
	if (trans)
		mainWindow(dc,"Offscreen Buffer Accelerated Transparent Blit Demo");
	else 
		mainWindow(dc,"Offscreen Buffer Accelerated Blit Demo");
	statusLine("Press any key to continue, ESC to Abort");

	if ((buf = MGL_createBuffer(dc,bmpWidth,bmpHeight,-1)) == NULL) {
		gprintf("Hardware accelerated buffer not available.");
		}
	else {
		/* Create a memory device context */
		MGL_getPixelFormat(dc,&pf);
		if ((memdc = MGL_createMemoryDC(bmpWidth,bmpHeight,MGL_getBitsPerPixel(dc),&pf)) == NULL)
			MGL_fatalError(MGL_errorMsg(MGL_result()));

		/* Load a bitmap into the memory device context. The palette gets
		 * loaded into the memory device context as well.
		 */
		if (!MGL_loadBitmapIntoDC(memdc,IMAGE_NAME,0,0,true))
			MGL_fatalError(MGL_errorMsg(MGL_result()));
		MGL_copyToBuffer(memdc,0,0,bmpWidth,bmpHeight,0,0,buf);

		/* Set the display device context with the palette from the
		 * memory device context.
		 */
		if (MGL_getBitsPerPixel(dc) <= 8) {
			MGL_getPalette(memdc,pal,MGL_getPaletteSize(memdc),0);
			MGL_setPalette(dc,pal,MGL_getPaletteSize(memdc),0);
			MGL_realizePalette(dc,MGL_getPaletteSize(memdc),0,true);
			}

		/* Get transparent color from pixel (0,0) */
		MGL_makeCurrentDC(memdc);
		transparent = MGL_getPixelCoord(0,0);
		MGL_makeCurrentDC(dc);

		/* Copy image from offscreen video memory to the screen. */
	    while (!checkEvent()) {
			if (trans)
				MGL_putBufferSrcTrans(dc,MGL_random(maxx-bmpWidth),MGL_random(maxy-bmpHeight),buf,transparent,MGL_REPLACE_MODE);
			else
				MGL_putBuffer(dc,MGL_random(maxx-bmpWidth),MGL_random(maxy-bmpHeight),buf,MGL_REPLACE_MODE);
			}
		MGL_destroyDC(memdc);
		}

	/* Destroy the buffer */
	MGL_destroyBuffer(buf);
	return pause();
}

/****************************************************************************
PARAMETERS:
dc     		- Display dc
trans  		- Turn on/off transparent blitting

REMARKS:
Demo showing how to use offscreen dc's to store sprites and to use hardware
acceleration to blt them from the offscreen dc to the display dc.
****************************************************************************/
ibool offscreenBlit(
	MGLDC *dc,
	ibool trans)
{
	MGLDC		*offdc;
	color_t 	transparent;
	palette_t 	pal[256];

	/* Display title message at the top of the window. */
	if (trans)
		mainWindow(dc,"Offscreen Memory Accelerated Transparent Blit Demo");
	else 
		mainWindow(dc,"Offscreen Memory Accelerated Blit Demo");
	statusLine("Press any key to continue, ESC to Abort");

	if ((offdc = MGL_createOffscreenDC(dc,bmpWidth,bmpHeight)) == NULL) {
		gprintf("Hardware accelerated blit not available.");
		}
	else {
		/* Load a bitmap into the offscreen device context */
		if (!MGL_loadBitmapIntoDC(offdc,IMAGE_NAME,0,0,true))
			MGL_fatalError(MGL_errorMsg(MGL_result()));

		/* Set the display device context with the palette from the
		 * offscreen device context.
		 */
		if (MGL_getBitsPerPixel(dc) <= 8) {
			MGL_getPalette(offdc,pal,MGL_getPaletteSize(offdc),0);
			MGL_setPalette(dc,pal,MGL_getPaletteSize(offdc),0);
			MGL_realizePalette(dc,MGL_getPaletteSize(offdc),0,true);
			}

		/* Get transparent color from pixel (0,0) */
	    MGL_makeCurrentDC(offdc);
	    transparent = MGL_getPixelCoord(0,0);
	    MGL_makeCurrentDC(dc);

		/* Copy image from offscreen video memory to the screen. */
	    while (!checkEvent()) {
			if (trans)
				MGL_srcTransBltCoord(dc,offdc,0,0,bmpWidth,bmpHeight,MGL_random(maxx-bmpWidth),MGL_random(maxy-bmpHeight),transparent,MGL_REPLACE_MODE);
			else
				MGL_bitBltCoord(dc,offdc,0,0,bmpWidth,bmpHeight,MGL_random(maxx-bmpWidth),MGL_random(maxy-bmpHeight),MGL_REPLACE_MODE);
			}
		}

	/* Remove all device contexts from memory. */
    MGL_destroyDC(offdc);
	return pause();
}

void demo(MGLDC *dc)
{
	/* Get the screen size and store them. */
    maxx = MGL_maxx();
    maxy = MGL_maxy();

	/* Get image information */
	MGL_getBitmapSize(IMAGE_NAME,&bmpWidth,&bmpHeight,&bmpDepth,&bmpPF);

	if (!systemBlit(dc,true))
		return;
	if (!offscreenBlit(dc,true))
		return;
	if (!bufferBlit(dc,true))
		return;
	if (!systemBlit(dc,false))
		return;
	if (!offscreenBlit(dc,false))
		return;
	if (!bufferBlit(dc,false))
		return;
}
