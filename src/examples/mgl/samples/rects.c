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
* Description:  Display a number of random rectangles with random
*               attributes.
*
****************************************************************************/

#include "mglsamp.h"
#include "pattern.h"

/*----------------------------- Global Variables --------------------------*/
char demoName[] = "rects";

/*------------------------------ Implementation ---------------------------*/

void demo(MGLDC *dc)
{
    int     maxx,maxy,val;
    rect_t  r;
	
	/* Display title message at the top of the window. */
    mainWindow(dc,"Rectangle Demonstration");
    statusLine("Press any key to continue, ESC to Abort");

    maxx = MGL_maxx();
    maxy = MGL_maxy();

    while (!checkEvent()) {
        r.left = MGL_random(maxx);
        r.right = MGL_random(maxx);
        r.top = MGL_random(maxy);
        r.bottom = MGL_random(maxy);

        /* Fix the rectangle so it is not empty */

        if (r.right < r.left)
            SWAP(r.left,r.right);
        if (r.bottom < r.top)
            SWAP(r.top,r.bottom);

        MGL_setColor(randomColor());
        MGL_setBackColor(randomColor());

        if ((val = MGL_random(3)) == 0) {
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

        MGL_fillRect(r);
        }

    defaultAttributes(dc);

	pause();
}








