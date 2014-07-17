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
* Description:  Double buffered mouse cursor sample program.
*
****************************************************************************/

#include "mglsamp.h"

/*----------------------------- Global Variables --------------------------*/

int     maxx,maxy,stepx,stepy,secAngle,minAngle;
rect_t  extent,dirtyRect;

char demoName[] = "mousedb";

/*------------------------------ Implementation ---------------------------*/

void initAnimation(void)
{
	maxx = MGL_maxx();
	maxy = MGL_maxy();
	extent.left = extent.top = 0;
	extent.right = maxx/5;
	extent.bottom = ((long)extent.right * 1000) / MGL_getAspectRatio();
	dirtyRect = extent;
	stepx = 1;
	stepy = 1;
	secAngle = minAngle = 90;
}

void clearDirtyRect(void)
{
	MGL_setColorCI(MGL_BLACK);
	MGL_fillRect(dirtyRect);
}

void drawClock(void)
{
	MGL_setColorCI(MGL_RED);
	MGL_fillEllipse(extent);
	MGL_setColorCI(MGL_WHITE);
	MGL_fillEllipseArc(extent,secAngle-5,secAngle);
	MGL_fillEllipseArc(extent,minAngle-5,minAngle);
}

int fixAngle(int angle)
{
    while (angle < 0)
        angle += 360;
    while (angle >= 360)
        angle -= 360;
    return angle;
}

void moveClock(void)
{
	/* Bounce the clock off the walls */
	dirtyRect = extent;
	MGL_insetRect(dirtyRect,-ABS(stepx),-ABS(stepy));
	if (extent.left + stepx < 0)
		stepx = -stepx;
	if (extent.right + stepx > maxx)
		stepx = -stepx;
	if (extent.top + stepy < 0)
		stepy = -stepy;
	if (extent.bottom + stepy > maxy)
		stepy = -stepy;
	MGL_offsetRect(extent,stepx,stepy);

	/* Update the hand movement */
	secAngle = fixAngle(secAngle - 5);
	if (secAngle == 90)
		minAngle = fixAngle(minAngle - 5);
}

void demo(MGLDC *dc)
{
	MGL_doubleBuffer(dc);
	mainWindow(dc,"Double buffered mouse cursor");
	statusLine("Method 2: Re-render partial scene per frame (RPG's)");
	MGL_swapBuffers(dc,false);
	mainWindow(dc,"Double buffered mouse cursor");
	statusLine("Method 2: Re-render partial scene per frame (RPG's)");
	MS_show();

	initAnimation();
	do {
		/* Draw the clock at the current location and move it */
		clearDirtyRect();
		drawClock();
		moveClock();

		/* Flip buffers */
		MGL_swapBuffers(dc,true);
		} while (!checkEvent());
	waitEvent();
}
