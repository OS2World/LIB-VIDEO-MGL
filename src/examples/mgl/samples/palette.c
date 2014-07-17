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
* Description:  Simple demo showing how to use the palette functions and
*				to do palette animation and fades using the mgl.
*
****************************************************************************/

#include "mglsamp.h"

#define IMAGE_NAME "frog.bmp"

/*----------------------------- Global Variables --------------------------*/

char demoName[] = "palette";
rect_t 		rect = {0};
int			posx,posy;
MGLDC 		*memdc;

/*------------------------------ Implementation ---------------------------*/

ibool colorDemo(MGLDC *dc)
/****************************************************************************
*
* Function:     colorDemo
* Parameters:   dc  - Device context
*
* Description:  Display the current color palette on the screen.
*
****************************************************************************/
{
    int         color,width,height,x,y,i,j,top,bottom,start;
    palette_t   pal[256];

    mainWindow(dc,"Color Demonstration");

    if (maxcolor <= 15) {
        /* Simple color demonstration for 16 color displays */

        width = 2 * ((MGL_maxx()+1) / 16);
        height = 2 * ((MGL_maxy()-10)  / 10);

        x = width / 2;
        y = height / 2;
        color = 1;
        for (j = 0; j < 3; j++) {
            for (i = 0; i < 5; i++) {
                MGL_setColor(color++);
                MGL_fillRectCoord(x,y,x+width,y+height);
                x += (width/2) * 3;
                }
            y += (height / 2) * 3;
            x = width / 2;
            }
        }
    else if (maxcolor == 255) {
        /* Color demonstration for large numbers of colors */

        width = 2 * ((MGL_maxx()+1) / 46);
        height = 2 * ((MGL_maxy()-10)  / 47);

        x = width / 2;
        y = height / 2;
        color = 1;
        for (j = 0; j < 16; j++) {
            for (i = 0; i < 16; i++) {
                MGL_setColor(color++);
                MGL_fillRectCoord(x,y,x+width,y+height);
                x += (width/2) * 3;
                }
            y += (height / 2) * 3;
            x = width / 2;
            }

        //MGL_setColor(MGL_defaultColor());
        statusLine("Press a key to rotate palette");
        waitEvent();
        statusLine("Press a key to rotate other direction");

        // Palette rotations

        while (!checkEvent()) {
			/* Copy image from image in memory to the screen. */
            MGL_rotatePalette(dc,palsize-1,1,MGL_ROTATE_UP);
            MGL_realizePalette(dc,palsize-1,1,true);
            }
        waitEvent();
        statusLine("Press a key to fade palette");

        while (!checkEvent()) {
            MGL_rotatePalette(dc,palsize-1,1,MGL_ROTATE_DOWN);
            MGL_realizePalette(dc,palsize-1,1,true);
            }
        waitEvent();

        // Palette fade out
        MGL_getPalette(dc,pal,palsize,0);
        for (i = 63; i >= 0; i--) {
            MGL_fadePalette(dc,pal+1,palsize-1,1,i*4);
            MGL_realizePalette(dc,palsize-1,1,true);
            }

        // Palette fade in
        for (i = 0; i <= 63; i++) {
            MGL_fadePalette(dc,pal+1,palsize-1,1,i*4);
            MGL_realizePalette(dc,palsize-1,1,true);
            }
        }
    else {
        /* Color demonstration for Dithered/HiColor/TrueColor modes */

        start = MGL_maxx()/8;
        width = MGL_maxx() - start*2;
        top = MGL_maxy()/8;
        bottom = MGL_maxy()/8 + MGL_maxy()/5;
        for (x = 0; x < width; x++) {
            MGL_setColor(MGL_packColor(&dc->pf,(x * 255L) / width,0,0));
            MGL_lineCoord(start+x,top,start+x,bottom);
            }

        top += MGL_maxy()/5;
        bottom += MGL_maxy()/5;
        for (x = 0; x < width; x++) {
            MGL_setColor(MGL_packColor(&dc->pf,0,(x * 255L) / width,0));
            MGL_lineCoord(start+x,top,start+x,bottom);
            }

        top += MGL_maxy()/5;
        bottom += MGL_maxy()/5;
        for (x = 0; x < width; x++) {
            MGL_setColor(MGL_packColor(&dc->pf,0,0,(x * 255L) / width));
            MGL_lineCoord(start+x,top,start+x,bottom);
            }

        top += MGL_maxy()/5;
        bottom += MGL_maxy()/5;
        for (x = 0; x < width/2; x++) {
            MGL_setColor(MGL_packColor(&dc->pf,(((width/2)-x) * 255L) / (width/2),
                (x * 255L) / (width/2),0));
            MGL_lineCoord(start+x,top,start+x,bottom);
            }
        for (; x < width; x++) {
            MGL_setColor(MGL_packColor(&dc->pf,0,
                ( ((width/2) - (x - (width/2))) * 255L) / (width/2),
                ((x-width/2) * 255L) / (width/2) ));
            MGL_lineCoord(start+x,top,start+x,bottom);
            }
        }

    MGL_setColor(MGL_defaultColor());
    if (!pause())
        return false;
    MGL_setPalette(dc,defPal,palsize,0);
    MGL_realizePalette(dc,MGL_getPaletteSize(dc),0,true);
    return true;
}

void demo(MGLDC *dc)
{
    mainWindow(dc,"Palette Demonstration");
    statusLine("Press any key to continue, ESC to Abort");

	if(!colorDemo(dc)) return;
}
