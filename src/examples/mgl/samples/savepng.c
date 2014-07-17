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

#define IMAGE_NAME "test.png"
#define IMAGE_NAME_OUT "copy.png"
/*----------------------------- Global Variables --------------------------*/

char demoName[] = "SavePng";

/*------------------------------ Implementation ---------------------------*/

void demo(MGLDC *dc)
{
    bitmap_t *bitmap = NULL;
    int posx,posy;
    int mypalsize;
    int bits;
    pixel_format_t pf;

    palette_t halftone[256];
	/* Display title message at the top of the window. */
    mainWindow(dc,"PNG Demo");
#if(0)
	/* Load a bitmap and the palette from the image file. */
        if((bitmap = MGL_loadPNG(IMAGE_NAME,TRUE)) == NULL)
		MGL_fatalError(MGL_errorMsg(MGL_result()));

	/* Set the display device context with the palette from the
	 * memory device context.
	 */

        if (bitmap->pal) {
            switch(bitmap->bitsPerPixel) {
                case 1 : mypalsize = 2; break;
                case 4 : mypalsize = 16; break;
                case 8 : mypalsize = 256; break;
            }
            MGL_setPalette(dc, bitmap->pal,min(MGL_getPaletteSize(dc), mypalsize),0);
            MGL_realizePalette(dc,min(MGL_getPaletteSize(dc), mypalsize),0,true);
            }
        else {
            MGL_getHalfTonePalette(halftone);
            MGL_setPalette(dc,halftone,MGL_getPaletteSize(dc),0);
            MGL_realizePalette(dc,MGL_getPaletteSize(dc),0,true);
            }

        /* Justify the image that will be put on the screen */
	posx = (MGL_sizex(dc)-bitmap->width)/2;
	posy = (MGL_sizey(dc)-bitmap->height)/2;

	/* Put the image onto the display device context. */
	MGL_putBitmap(dc,posx,posy,bitmap,MGL_REPLACE_MODE);

	/* Remove all bitmaps from memory. */
        MGL_unloadBitmap(bitmap);
#endif
    if(!MGL_getPNGSize(IMAGE_NAME, &posx, &posy, &bits, &pf))
        MGL_fatalError(MGL_errorMsg(MGL_result()));
    if(!MGL_loadPNGIntoDC(dc, IMAGE_NAME, 0, 0, TRUE))
    	MGL_fatalError(MGL_errorMsg(MGL_result()));
    if(!MGL_savePNGFromDC(dc, IMAGE_NAME_OUT, 0, 0, posx, posy))
        MGL_fatalError(MGL_errorMsg(MGL_result()));
	/* Pause until the user hits a key or presses a mouse button. */
	pause();
}








