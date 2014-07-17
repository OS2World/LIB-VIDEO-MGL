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
* Description:  Simple demo showing how to load a bitmap file from disk and
*				display it on the screen.
*
****************************************************************************/

#include "mglsamp.h"

/*----------------------------- Global Variables --------------------------*/

char demoName[] = "loadbmp";

/*------------------------------ Implementation ---------------------------*/

void demo(MGLDC *dc)
{
	bitmap_t 	*bitmap;
	int 		posx,posy;
	palette_t	pal[256];

	/*------------------------------------------------------------------
	 * Load a bitmap from the image file as a 24-bit image. The bitmap
	 * will always be a 24-bit, RGB format image.
	 */
	mainWindow(dc,"Bitmap Loading Demo (24-bit)");

	if ((bitmap = MGL_loadBitmap("cow-24.bmp",true)) == NULL)
		MGL_fatalError(MGL_errorMsg(MGL_result()));

	/* If this is an 8bpp display device context, we need to set the
	 * palette to the RGB halftone palette as the MGL_putBitmap call
	 * is going to do a dither of the input bitmap since this BMP file is
	 * a 24-bit RGB bitmap when loaded from disk.
	 */
	if (dc->mi.bitsPerPixel == 8) {
		MGL_getHalfTonePalette(pal);
		MGL_setPalette(dc,pal,MGL_getPaletteSize(dc),0);
		MGL_realizePalette(dc,MGL_getPaletteSize(dc),0,true);
		}

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
	 * Load a bitmap from the image file as a 8-bit image. The bitmap
	 * will always be a 8-bit, color index format.
	 */
	mainWindow(dc,"Bitmap Loading Demo (8-bit)");

	if ((bitmap = MGL_loadBitmap("frog-8.bmp",true)) == NULL)
		MGL_fatalError(MGL_errorMsg(MGL_result()));

	/* If this is an 8bpp display device context, we need to set the
	 * palette to the palette contained in the bitmap.
	 */
	if (dc->mi.bitsPerPixel == 8) {
		MGL_setPalette(dc,bitmap->pal,256,0);
		MGL_realizePalette(dc,256,0,true);
		}

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
	 * Load a bitmap from the image file as a 4-bit image. The bitmap
	 * will always be a 4-bit, color index format.
	 */
	mainWindow(dc,"Bitmap Loading Demo (4-bit)");

	if ((bitmap = MGL_loadBitmap("frog-4.bmp",true)) == NULL)
		MGL_fatalError(MGL_errorMsg(MGL_result()));

	/* If this is an 8bpp display device context, we need to set the
	 * palette to the palette contained in the bitmap.
	 */
	if (dc->mi.bitsPerPixel == 8) {
		MGL_setPalette(dc,bitmap->pal,16,0);
		MGL_realizePalette(dc,16,0,true);
		}

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
	 * Load a bitmap from the image file as a 1-bit image. The bitmap
	 * will always be a 1-bit, color index format.
	 */
	mainWindow(dc,"Bitmap Loading Demo (1-bit)");

	if ((bitmap = MGL_loadBitmap("frog-1.bmp",true)) == NULL)
		MGL_fatalError(MGL_errorMsg(MGL_result()));

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
	 * Load a bitmap from the image file as a 24-bit image. The bitmap
	 * will always be a 24-bit, RGB format image.
	 */
	mainWindow(dc,"Bitmap Loading Demo (24-bit into DC)");

	/* If this is an 8bpp display device context, we need to set the
	 * palette to the RGB halftone palette as the MGL_putBitmap call
	 * is going to do a dither of the input bitmap since Bitmap's are always
	 * 24-bit RGB bitmaps when loaded from disk.
	 */
	if (dc->mi.bitsPerPixel == 8) {
		MGL_getHalfTonePalette(pal);
		MGL_setPalette(dc,pal,MGL_getPaletteSize(dc),0);
		MGL_realizePalette(dc,MGL_getPaletteSize(dc),0,true);
		}

	if (!MGL_loadBitmapIntoDC(dc,"cow-24.bmp",posx,posy,true))
		MGL_fatalError(MGL_errorMsg(MGL_result()));

	/* Pause until the user hits a key or presses a mouse button. */
	if (!pause())
		return;

	/*------------------------------------------------------------------
	 * Load a bitmap from the image file as a 8-bit color image.
	 */
	mainWindow(dc,"Bitmap Loading Demo (8-bit color into DC)");

	if (!MGL_loadBitmapIntoDC(dc,"frog-8.bmp",posx,posy,true))
		MGL_fatalError(MGL_errorMsg(MGL_result()));

	/* Pause until the user hits a key or presses a mouse button. */
	if (!pause())
		return;

	/*------------------------------------------------------------------
	 * Load a bitmap from the image file as a 4-bit color image.
	 */
	mainWindow(dc,"Bitmap Loading Demo (4-bit color into DC)");

	if (!MGL_loadBitmapIntoDC(dc,"frog-4.bmp",posx,posy,true))
		MGL_fatalError(MGL_errorMsg(MGL_result()));

	/* Pause until the user hits a key or presses a mouse button. */
	if (!pause())
		return;

	/*------------------------------------------------------------------
	 * Load a bitmap from the image file as a 1-bit color image.
	 */
	mainWindow(dc,"Bitmap Loading Demo (1-bit color into DC)");

	if (!MGL_loadBitmapIntoDC(dc,"frog-1.bmp",posx,posy,true))
		MGL_fatalError(MGL_errorMsg(MGL_result()));

	/* Pause until the user hits a key or presses a mouse button. */
	if (!pause())
		return;
}
