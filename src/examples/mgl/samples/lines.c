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
* Description:  Simple demo showing how to draw a bunch of lines on
*				the screen with the MGL.
*
****************************************************************************/

#include <math.h>
#include "mglsamp.h"
#include "pattern.h"

/*----------------------------- Global Variables --------------------------*/

#define MAX_LINES 35

typedef struct {
	point_t p1,p2;
	point_t	d1,d2;
	int 	color;
	} vector;

char demoName[] = "lines";
int maxx,maxy;

/*------------------------------ Implementation ---------------------------*/

/****************************************************************************
PARAMETERS:
dc	- Display DC

REMARKS:
Draw solid lines and change their color.
****************************************************************************/
ibool lineTest(
	MGLDC *dc)
{
    int i,lines;
	vector *v;

	lines = ((float)MAX_LINES/maxy)*maxy;
	
	v = (vector *)MGL_malloc(sizeof(vector)*lines);

	for(i=0;i<lines;i++) {
        v[i].p1.x = i*(maxx/MAX_LINES);
        v[i].p1.y = i*(maxy/MAX_LINES);
        v[i].p2.x = maxx - i*(maxx/MAX_LINES);
        v[i].p2.y = i*(maxy/MAX_LINES);
		v[i].d1.x = 1;
		v[i].d1.y = -1;
		v[i].d2.x = -1;
		v[i].d2.y = 1;
        v[i].color = randomColor();
	}

	/* Display title message at the top of the window. */
    mainWindow(dc,"Line Demonstration");
    statusLine("Press any key to continue, ESC to Abort");

    MGL_setPenStyle(MGL_BITMAP_SOLID);
    while (!checkEvent()) {
		for(i=0;i<lines;i++) {
			/* change the position of both vertices */
	        v[i].p1.x += v[i].d1.x;
	        v[i].p1.y += v[i].d1.y;
	        v[i].p2.x += v[i].d2.x;
	        v[i].p2.y += v[i].d2.y;

			/* change the direction of the first vertice */
			if(v[i].p1.x>maxx) v[i].d1.x = -1;
			if(v[i].p1.x<=0) v[i].d1.x = 1;
			if(v[i].p1.y>maxy) v[i].d1.y = -1;
			if(v[i].p1.y<=0) v[i].d1.y = 1;

			/* change the direction of the second vertice */
			if(v[i].p2.x>maxx) v[i].d2.x = -1;
			if(v[i].p2.x<=0) v[i].d2.x = 1;
			if(v[i].p2.y>maxy) v[i].d2.y = -1;
			if(v[i].p2.y<=0) v[i].d2.y = 1;

	        MGL_setColor(v[i].color);
	        MGL_line(v[i].p1,v[i].p2);
			}
        }

	MGL_free(v);
    defaultAttributes(dc);
	return pause();
}

/****************************************************************************
PARAMETERS:
dc	- Display DC

REMARKS:
This demo randomly places lines onto the screen. The size, color and pen
style are randomly changing as well. The pen style MGL_BITMAP_TRANSPARENT
uses the foreground color to fill in all pixels in the bitmap. Where the
pixels in the bitmap pattern are a 0, the original background color is
retained. In the MGL_BITMAP_OPAQUE mode, the background color is used to
fill in the pixels in the bitmap that are set to a 0.
****************************************************************************/
ibool randomLineTest(
	MGLDC *dc)
{
	int val;
    point_t p1,p2;

	/* Display new title message at the top of the window. */
    mainWindow(dc,"Random line Demonstration");
    statusLine("Press any key to continue, ESC to Abort");

	while (!checkEvent()) {
        p1.x = MGL_random(maxx);
        p1.y = MGL_random(maxy);
        p2.x = MGL_random(maxx);
        p2.y = MGL_random(maxy);
		MGL_setColor(randomColor());
		MGL_setBackColor(randomColor());
		MGL_setPenSize(MGL_random(5)+1,MGL_random(5)+1);
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
		MGL_line(p1,p2);
        }
	defaultAttributes(dc);
	return pause();
}

void demo(MGLDC *dc)
{
	/* Get the screen size and store them. */
    maxx = MGL_maxx();
    maxy = MGL_maxy();

	if(!lineTest(dc)) return;
	if(!randomLineTest(dc)) return;
}
