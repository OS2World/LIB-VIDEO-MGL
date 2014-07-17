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
* Description:  Simple demo showing how to draw a bunch of simple ellipses
*				on the screen with the MGL.
*
****************************************************************************/

#include "mglsamp.h"
#include "pattern.h"

#define MIN_RADIUS	5

/*----------------------------- Global Variables --------------------------*/

char demoName[] = "ellipse";
int maxx,maxy;

/*------------------------------ Implementation ---------------------------*/

/****************************************************************************
PARAMETERS:
dc	- Display dc

REMARKS:
This demo fills the entire screen with an ellipses. The MGL_fillEllipse
function uses a bounding rectangle defining the ellipses. The size, color
and pen style are randomly changing as well.
****************************************************************************/
ibool ellipsesDemo(
	MGLDC *dc)
{
	int val;
    rect_t  r;

	/* Display title message at the top of the window. */
    mainWindow(dc,"Filled ellipses Demonstration");
    statusLine("Press any key to start random test, ESC to Abort");

	/* The pen style MGL_BITMAP_TRANSPARENT uses the foreground
	 * color to fill in all pixels in the bitmap. Where the pixels in the
	 * bitmap pattern are a 0, the original background color is retained. 
	 * In the MGL_BITMAP_OPAQUE mode, the background color is used to 
	 * fill in the pixels in the bitmap that are set to a 0.
	 */
	while (!checkEvent()) {
		MGL_setColor(randomColor());
		MGL_setBackColor(randomColor());
		if (val == 0) {
            MGL_setPenStyle(MGL_BITMAP_TRANSPARENT);
			MGL_setPenBitmapPattern(0,&bitpat[MGL_random(NUMPATS)+1]);
			MGL_usePenBitmapPattern(0);
			}
        else if (val == 1) {
            MGL_setPenStyle(MGL_BITMAP_OPAQUE);
			MGL_setPenBitmapPattern(0,&bitpat[MGL_random(NUMPATS)+1]);
			MGL_usePenBitmapPattern(0);
			}
        else {
            MGL_setPenStyle(MGL_BITMAP_SOLID);
			val = 0;
            }
		val++;
		r.top = maxy/2;
		r.left = maxx/2;
		r.right = maxx/2;
        r.bottom = maxy/2;
		while((r.top || (r.right<maxx)) && (!checkEvent())) {
			if(r.top) r.top--;
			if(r.left) r.left--;
			if(r.right<maxx) r.right++;
			if(r.bottom<maxy) r.bottom++;
			MGL_fillEllipse(r);
			}
        }
	defaultAttributes(dc);
	return pause();
}

/****************************************************************************
PARAMETERS:
dc	- Display dc

REMARKS:
Demo randomly places ellipses on the screen.
****************************************************************************/
ibool randomEllipsesDemo(
	MGLDC *dc)
{
	int val;
    rect_t  r;

    mainWindow(dc,"Random filled ellipses Demonstration");
    statusLine("Press any key to continue, ESC to Abort");

    while (!checkEvent()) {
        r.left = MGL_random(maxx-100);
        r.top = MGL_random(maxy-100);
        r.right = r.left + MGL_random(100);
        r.bottom = r.top + MGL_random(100);
		MGL_setColor(randomColor());
		MGL_setBackColor(randomColor());
		if ((val = MGL_random(5)) == 0) {
            MGL_setPenStyle(MGL_BITMAP_TRANSPARENT);
			MGL_setPenBitmapPattern(0,&bitpat[MGL_random(NUMPATS)+1]);
			MGL_usePenBitmapPattern(0);
			}
        else if (val == 1) {
            MGL_setPenStyle(MGL_BITMAP_OPAQUE);
			MGL_setPenBitmapPattern(0,&bitpat[MGL_random(NUMPATS)+1]);
			MGL_usePenBitmapPattern(0);
			}
        else {
            MGL_setPenStyle(MGL_BITMAP_SOLID);
            }
		MGL_fillEllipse(r);
        }

    defaultAttributes(dc);
    return pause();
}

void demo(MGLDC *dc)
{
	/* Get the screen size and store them. */
    maxx = MGL_maxx();
    maxy = MGL_maxy();

	if(!ellipsesDemo(dc)) return;
	if(!randomEllipsesDemo(dc)) return;
}
