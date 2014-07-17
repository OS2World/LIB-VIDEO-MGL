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
* Description:  Sample program showing how to use the SciTech MGL to
*               display fullscreen stereo bitmaps for viewing with
*               LC shutter glasses.
*
*               This version uses MGLSAMP.C front-end shell for
*               selecting display mode resolution for stereo. (DaveM)
*              
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mgraph.h"
#include "mglsamp.h"
#ifdef	__WINDOWS__
#define STRICT
#define	WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commdlg.h>
#endif


/*----------------------------- Global Variables --------------------------*/

char demoName[] = "sterdemo";
int maxx,maxy;
int refreshRate = MGL_DEFAULT_REFRESH;
char bitmapName[] = "stereo.bmp";

/*------------------------------ Implementation ---------------------------*/

void demo(MGLDC *dc)
{
	int             mode;
	int             x,y,width,height;
	bitmap_t        *bitmap;
	event_t     	evt;

	/* Get display mode selected. */
	maxx = MGL_maxx();
	maxy = MGL_maxy();
	mode = MGL_getMode(dc);

	/* Now load the bitmap file from disk. Note that we store the bitmap
	 * files with the left and right images together side by side in a
	 * single image, using the defacto standard "cross-eyed free-view"
	 * format (right-eye image on left, left-eye image on right). (DaveM)
	 */
	bitmap = MGL_loadBitmap(bitmapName,true);
	if (!bitmap)
		MGL_fatalError("Unable to locate stereo bitmap!");

	/* Create the stereo display mode */
	MGL_changeDisplayMode(mode);
	if ((dc = MGL_createStereoDisplayDC(1,refreshRate)) == NULL)
		MGL_fatalError(MGL_errorMsg(MGL_result()));
	MGL_makeCurrentDC(dc);

	/* Display the bitmap on the screen for the left and right images */
	width = bitmap->width/2;
	height = bitmap->height;
	x = (MGL_maxx()-width)/2;
	y = (MGL_maxy()-height)/2;
	MGL_setActivePage(dc,0 | MGL_LEFT_BUFFER);
	MGL_clearDevice();
	MGL_putBitmapSection(dc,width,0,width*2,height,x,y,bitmap,MGL_REPLACE_MODE);
	MGL_setActivePage(dc,0 | MGL_RIGHT_BUFFER);
	MGL_clearDevice();
	MGL_putBitmapSection(dc,0,0,width,height,x,y,bitmap,MGL_REPLACE_MODE);
	MGL_startStereo(dc);
	EVT_halt(&evt,EVT_KEYDOWN);

	/* Disable free running stereo mode */
	MGL_stopStereo(dc);
}
