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
*               test if stereo page-flipping display modes are working.
*               (Test does not require LC shutter glasses. DaveM)
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

/*------------------------------ Implementation ---------------------------*/

void stereo(int refreshRate)
{
	MGLDC   *dc;
	int     driver = grDETECT,mode = grDETECT;
	int     x,y,width,height;
	event_t	evt;

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

	/* Select a typical stereo display mode */
	mode = grSVGA_640x480x256;

	/* Create the stereo display mode */
	MGL_changeDisplayMode(mode);
	if ((dc = MGL_createStereoDisplayDC(1,refreshRate)) == NULL)
		MGL_fatalError(MGL_errorMsg(MGL_result()));
	MGL_makeCurrentDC(dc);

	/* Display red and blue rectangles in left and right draw buffers */
	width = MGL_maxx() * 3/4;
	height = MGL_maxy() * 3/4;
	x = MGL_maxx() / 4;
	y = MGL_maxy() / 4;
	MGL_setActivePage(dc, 0 | MGL_LEFT_BUFFER);
	MGL_clearDevice();
	MGL_setColor(MGL_rgbColor(dc, 255, 0, 0));
	MGL_fillRectCoord(0, 0, width, height);

	MGL_setActivePage(dc, 0 | MGL_RIGHT_BUFFER);
	MGL_clearDevice();
	MGL_setColor(MGL_rgbColor(dc, 0, 0, 255));
	MGL_fillRectCoord(x, y, x+width, y+height);

	/* Overlapping red and blue regions should appear magenta
	 * if stereo page-flipping is working correctly.
	 */
	MGL_startStereo(dc);
	EVT_halt(&evt,EVT_KEYDOWN);

	/* Disable free running stereo mode */
	MGL_stopStereo(dc);
	MGL_exit();
}

#if	defined(__REALDOS__) || defined(__OS2_CONSOLE__) || defined(__UNIX__)

int main(int argc,char **argv)
{
	int refreshRate = 0;

	if (argc > 1 && argv[1][0] == '-') {
		refreshRate = atoi(&argv[1][1]);
		argc--;
		argv++;
		}
	stereo(refreshRate);
	return 0;
}

#elif defined(__OS2_PM__)

// TODO: Add code to support OS/2 Presentation Manager apps

#elif   defined(__WINDOWS__)
int PASCAL WinMain(HINSTANCE hInst,HINSTANCE hPrev,LPSTR szCmdLine,int sw)
{
	int	refreshRate;

	if (szCmdLine[0] == '-')
		refreshRate = atoi(szCmdLine+1);
	else
		refreshRate = 0;
	stereo(refreshRate);
	return 0;
}
#endif
