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
*				display fullscreen stereo bitmaps for viewing with
*				LC shutter glasses.
*
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mgraph.h"
#ifdef	__WINDOWS__
#define STRICT
#define	WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commdlg.h>
#endif
#ifndef _MAX_PATH
#define _MAX_PATH       255
#endif
#ifdef	ISV_LICENSE
#include "nucleus/graphics.h"
#include "isv.c"
#endif

/*------------------------------ Implementation ---------------------------*/

void stereo(const char *bitmapName,int refreshRate)
{
	MGLDC           *dc;
	int             driver = grDETECT,mode = grDETECT;
	int				x,y,width,height;
	bitmap_t		*bitmap;
	event_t     	evt;

	/* Use default refresh rate previously selected by user
	 * with external utility or control panel. As much as we'd
	 * like to use 120 Hz refresh for LCS glasses, this may be
	 * too high for the *average* monitor. (DaveM)
	 */
	if (refreshRate == 0)
		refreshRate = MGL_DEFAULT_REFRESH;

	/* Start the MGL and create a stereo display device context */
	MGL_registerAllDispDrivers(true,true,true);
	if (!MGL_init(&driver,&mode,"..\\..\\..\\"))
		MGL_fatalError(MGL_errorMsg(MGL_result()));

	/* Now load the bitmap file from disk. Note that we store the bitmap
	 * files with the left and right images together side by side in a
	 * single image, using the defacto standard "cross-eyed free-view"
	 * format (right-eye image on left, left-eye image on right). (DaveM)
	 */
	bitmap = MGL_loadBitmap(bitmapName,true);
	if (!bitmap)
		MGL_fatalError("Unable to locate stereo bitmap!");

	/* Search for a suitable graphics mode to display the image */
	/* (Use STERDEMO.C companion example for selecting resolutions. (DaveM) */
	if (bitmap->bitsPerPixel >= 24)
		mode = grSVGA_640x480x4G;
	else if (bitmap->bitsPerPixel >= 15)
		mode = grSVGA_640x480x64k;
	else
		mode = grSVGA_640x480x256;

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
	MGL_exit();
}

#if	defined(__REALDOS__) || defined(__OS2_CONSOLE__) || defined(__UNIX__)

int main(int argc,char **argv)
{
	int		refreshRate = 0;
	char	bitmapName[_MAX_PATH] = "stereo\\";

	if (argc > 1 && argv[1][0] == '-') {
		refreshRate = atoi(&argv[1][1]);
		argc--;
		argv++;
		}
	if (argc > 1)
		strcat(bitmapName,argv[1]);
	else
		strcpy(bitmapName,"stereo.bmp");
	stereo(bitmapName,refreshRate);
	return 0;
}

#elif defined(__OS2_PM__)

// TODO: Add code to support OS/2 Presentation Manager apps

#elif   defined(__WINDOWS__)

static char    szAppFilter[] = "Bitmaps\0*.bmp\0";
static char    bitmapName[_MAX_PATH] = "";

ibool OpenBitmapFile(HWND hwnd)
/****************************************************************************
*
* Function:     OpenBitmapFile
* Parameters:   hwnd    - Handle to parent window
* Returns:      True for valid bitmap name, false otherwise
*
* Description:  Open the bitmap file and load it into the memory DC.
*
****************************************************************************/
{
	OPENFILENAME    ofn;

	/* Prompt user for file to open */
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.hInstance = NULL;
	ofn.lpstrFilter = szAppFilter;
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
    ofn.nFilterIndex = 0;
    ofn.lpstrFile = bitmapName;
	ofn.nMaxFile = sizeof(bitmapName);
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
    ofn.nFileOffset = 0;
    ofn.nFileExtension = 0;
    ofn.lpstrDefExt = NULL;
    ofn.lCustData = 0;
    ofn.lpfnHook = NULL;
    ofn.lpTemplateName = NULL;
	return GetOpenFileName(&ofn);
}

int PASCAL WinMain(HINSTANCE hInst,HINSTANCE hPrev,LPSTR szCmdLine,int sw)
{
	int	refreshRate;
    if (szCmdLine[0] == '-')
		refreshRate = atoi(szCmdLine+1);
	else
    	refreshRate = 0;
	strcpy(bitmapName,"");
	while (OpenBitmapFile(NULL)) {
		stereo(bitmapName,refreshRate);
		}
	return 0;
}
#endif
