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
* Description:  Simple demo showing how to load a JPEG file from disk and
*				display it on the screen.
*
****************************************************************************/

#include "mglsamp.h"

#define IMAGE_NAME "cow-24.jpg"

/*----------------------------- Global Variables --------------------------*/

char demoName[] = "loadjpeg";

/*------------------------------ Implementation ---------------------------*/

void demo(MGLDC *dc)
{
	bitmap_t 	*bitmap = NULL;
	int 		posx,posy;
	palette_t	pal[256];

	/*------------------------------------------------------------------
	 * Load a bitmap from the image file as a 24-bit image. The bitmap
	 * will always be a 24-bit, RGB format image.
	 */
	mainWindow(dc,"JPEG Loading Demo (24-bit)");

	if ((bitmap = MGL_loadJPEG(IMAGE_NAME,0)) == NULL)
		MGL_fatalError(MGL_errorMsg(MGL_result()));

	/* If this is an 8bpp display device context, we need to set the
	 * palette to the RGB halftone palette as the MGL_putBitmap call
	 * is going to do a dither of the input bitmap since JPEG's are always
	 * 24-bit RGB bitmaps when loaded from disk.
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
	 * Load a bitmap from the image file as a 8-bit grayscale image. The
	 * image will be an 8-bit bitmap with a grayscale palette.
	 */
	mainWindow(dc,"JPEG Loading Demo (8-bit grayscale)");

	if ((bitmap = MGL_loadJPEG(IMAGE_NAME,-1)) == NULL)
		MGL_fatalError(MGL_errorMsg(MGL_result()));

	/* If this is an 8bpp display device context, we need to set the
	 * palette to the grayscale palette stored in the bitmap.
	 */
	if (dc->mi.bitsPerPixel == 8) {
		MGL_setPalette(dc,bitmap->pal,MGL_getPaletteSize(dc),0);
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
	 * Load a bitmap from the image file as a 8-bit color image, which
	 * we get the library to quantise for us.
	 */
	mainWindow(dc,"JPEG Loading Demo (8-bit color)");

	if ((bitmap = MGL_loadJPEG(IMAGE_NAME,256)) == NULL)
		MGL_fatalError(MGL_errorMsg(MGL_result()));

	/* If this is an 8bpp display device context, we need to set the
	 * palette to the color palette stored in the bitmap.
	 */
	if (dc->mi.bitsPerPixel == 8) {
		MGL_setPalette(dc,bitmap->pal,MGL_getPaletteSize(dc),0);
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
	 * Load a bitmap from the image file as a 24-bit image. The bitmap
	 * will always be a 24-bit, RGB format image.
	 */
	mainWindow(dc,"JPEG Loading Demo (24-bit into DC)");

	/* If this is an 8bpp display device context, we need to set the
	 * palette to the RGB halftone palette as the MGL_putBitmap call
	 * is going to do a dither of the input bitmap since JPEG's are always
	 * 24-bit RGB bitmaps when loaded from disk.
	 */
	if (dc->mi.bitsPerPixel == 8) {
		MGL_getHalfTonePalette(pal);
		MGL_setPalette(dc,pal,MGL_getPaletteSize(dc),0);
		MGL_realizePalette(dc,MGL_getPaletteSize(dc),0,true);
		}

	if (!MGL_loadJPEGIntoDC(dc,IMAGE_NAME,posx,posy,0))
		MGL_fatalError(MGL_errorMsg(MGL_result()));

	/* Pause until the user hits a key or presses a mouse button. */
	if (!pause())
		return;

	/*------------------------------------------------------------------
	 * Load a bitmap from the image file as a 8-bit grayscale.
	 */
	mainWindow(dc,"JPEG Loading Demo (8-bit grayscale into DC)");

	if (!MGL_loadJPEGIntoDC(dc,IMAGE_NAME,posx,posy,-1))
		MGL_fatalError(MGL_errorMsg(MGL_result()));

	/* Pause until the user hits a key or presses a mouse button. */
	if (!pause())
		return;

	/*------------------------------------------------------------------
	 * Load a bitmap from the image file as a 8-bit color image, which
	 * we get the library to quantise for us to the palette in our DC.
	 */
	if (dc->mi.bitsPerPixel == 8) {
		mainWindow(dc,"JPEG Loading Demo (8-bit color into DC)");

		/* If this is an 8bpp display device context, we need to reset
		 * the color map to the default so it will be used for the source
		 * of the dither.
		 */
		MGL_getDefaultPalette(dc,pal);
		MGL_setPalette(dc,pal,MGL_getPaletteSize(dc),0);
		MGL_realizePalette(dc,MGL_getPaletteSize(dc),0,true);

		if (!MGL_loadJPEGIntoDC(dc,IMAGE_NAME,posx,posy,256))
			MGL_fatalError(MGL_errorMsg(MGL_result()));

		/* Pause until the user hits a key or presses a mouse button. */
		if (!pause())
			return;
		}
}
