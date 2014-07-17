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
* Description:  Simple hello program showing how to get up and running
*               using the MGL.
*
****************************************************************************/

#include "mgraph.h"
#ifdef	ISV_LICENSE
#include "nucleus/graphics.h"
#include "isv.c"
#endif

/*------------------------------ Implementation ---------------------------*/

int main(void)
{
	MGLDC   *dc;
	int     xres,yres,i;
	event_t evt;

	/* Register the ISV license file if desired */
#ifdef	ISV_LICENSE
	GA_registerLicense(OemLicense,false);
#endif

	/* Start the MGL in the 640x480x256 display mode */
	dc = MGL_quickInit(640,480,8,1);

	/* Draw a simple pattern of lines on the display */
	xres = MGL_maxx()+1;
	yres = MGL_maxy()+1;
	for (i = 0; i < xres; i += 5) {
		MGL_setColorCI(i % 255);
		MGL_lineCoord(xres/2,yres/2,i,0);
		MGL_setColorCI((i+1) % 255);
		MGL_lineCoord(xres/2,yres/2,i,yres-1);
		}
	for (i = 0; i < yres; i += 5) {
		MGL_setColorCI((i+2) % 255);
		MGL_lineCoord(xres/2,yres/2,0,i);
		MGL_setColorCI((i+3) % 255);
		MGL_lineCoord(xres/2,yres/2,xres-1,i);
		}
	MGL_setColorCI(MGL_WHITE);
	MGL_lineCoord(0,0,xres-1,0);
    MGL_lineCoord(0,0,0,yres-1);
    MGL_lineCoord(xres-1,0,xres-1,yres-1);
	MGL_lineCoord(0,yres-1,xres-1,yres-1);

	/* Wait for a keypress or mouse click */
	EVT_halt(&evt,EVT_KEYDOWN);

	/* Close down the MGL and exit */
	MGL_exit();
	return 0;
}
