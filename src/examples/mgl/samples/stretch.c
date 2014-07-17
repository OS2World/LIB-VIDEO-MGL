/****************************************************************************
*
*                      			SciTech MGL
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
*
* Language:     ANSI C
* Environment:  MSDOS/Win32 full screen
*
* Description:  Demo showing the use of the stretchBlt functions to stretch
*				a bitmap from memory dc to the screen and stretchBitmap to
*				stretch a custom bitmap image from memory to the screen.
*
*
****************************************************************************/

#include "mglsamp.h"

#define IMAGE_NAME "frog.bmp"

/*----------------------------- Global Variables --------------------------*/

char demoName[] = "stretch";
int maxx,maxy;

/*------------------------------ Implementation ---------------------------*/

ibool bitmapStretch(MGLDC *dc)
/****************************************************************************
*
* Function:     bitmapStretch
* Parameters:   dc  - Device context
*
* Description:  Demo showing how to use stretchBitmap to stretch a custom 
*				bitmap image from memory to the screen. 
*
****************************************************************************/
{
	int x=maxx/2,y=maxy/2;
	bitmap_t *bitmap = NULL;

	mainWindow(dc,"Bitmap Stretch Demo");
    statusLine("Press any key to continue, ESC to Abort");

	/* Load a bitmap and the palette from the image file. */
	if((bitmap = MGL_loadBitmap(IMAGE_NAME,TRUE)) == NULL) 
		MGL_fatalError(MGL_errorMsg(MGL_result()));

	/* Set the display device context with the palette from the bitmap */
	MGL_setPalette(dc,bitmap->pal,MGL_getPaletteSize(dc),0);
	MGL_realizePalette(dc,MGL_getPaletteSize(dc),0,true);

	while ((x<maxx) && (y<maxy) && (!checkEvent())) {
		MGL_stretchBitmap(dc,maxx-x,maxy-y,x,y,bitmap,MGL_REPLACE_MODE);
		x += 1;
		y += 1;
		}

	/* Remove all bitmaps from memory. */
	MGL_unloadBitmap(bitmap);

	return pause();
}

ibool memoryDCStretch(MGLDC *dc)
/****************************************************************************
*
* Function:     memoryDCStretch
* Parameters:   dc  - Device context
*
* Description:  Demo showing the use of the stretchBlt functions to stretch
*				a bitmap from memory dc to the screen. 
****************************************************************************/
{
	MGLDC *memdc;
	int width,height,depth;
	pixel_format_t pf;
	palette_t pal[256];
	int x=maxx/2,y=maxy/2;

	mainWindow(dc,"Memory DC Stretch Demo");
    statusLine("Press any key to continue, ESC to Abort");

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
	MGL_getPalette(memdc,pal,MGL_getPaletteSize(memdc),0);
	MGL_setPalette(dc,pal,MGL_getPaletteSize(memdc),0);
	MGL_realizePalette(dc,MGL_getPaletteSize(memdc),0,true);

	/* Put the image onto the display device context. */
	while ((x<maxx) && (y<maxy) && (!checkEvent())) {
		MGL_stretchBltCoord(dc,memdc,0,0,width,height,maxx-x,maxy-y,x,y,MGL_REPLACE_MODE);
		x += 1;
		y += 1;
		}

	/* Remove all device contexts from memory. */
    MGL_destroyDC(memdc);

	return pause();
}

void demo(MGLDC *dc)
{
    maxx = MGL_maxx();
    maxy = MGL_maxy();

	if(!bitmapStretch(dc)) return;
	if(!memoryDCStretch(dc)) return;
}
