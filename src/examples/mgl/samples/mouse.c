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
* Description:  Simple demo showing how to use the MGL mouse functions.
*
****************************************************************************/

#include "mglsamp.h"

/*----------------------------- Global Variables --------------------------*/

char demoName[] = "mouse";

/*------------------------------ Implementation ---------------------------*/

ibool mouseCursorDemo(MGLDC *dc)
/****************************************************************************
*
* Function:     mouseCursorDemo
* Parameters:   dc  - Device context
*
* Description:  Display the mouse cursor on the screen, and change it to
*               a number of different styles.
*
****************************************************************************/
{
	cursor_t    *cursor;

	/* turn on the mouse */
	MS_show();

	statusLine("Press any key for Hour Glass cursor");
	waitEvent();

	if((cursor = MGL_loadCursor("wait.cur")) == NULL)
		MGL_fatalError("Unable to load WAIT.CUR cursor file");
	MS_setCursor(cursor);
	statusLine("Press any key for IBEAM cursor");
	waitEvent();

	MS_setCursor(MGL_DEF_CURSOR);
	MGL_unloadCursor(cursor);
	if((cursor = MGL_loadCursor("ibeam.cur")) == NULL)
		MGL_fatalError("Unable to load IBEAM.CUR cursor file");
	MS_setCursor(cursor);
	waitEvent();

	MS_setCursor(MGL_DEF_CURSOR);
	MGL_unloadCursor(cursor);

	if (!pause()) 
		return false;

	MS_hide();

	return true;
}

void demo(MGLDC *dc)
{
	/* Display title message at the top of the window. */
	mainWindow(dc,"Mouse Cursor Demonstration");

	/* set the background color to grey */
    if (MGL_getBitsPerPixel(dc) == 8) {
		MGL_setPaletteEntry(dc,1,100,100,100);
		MGL_realizePalette(dc,1,1,true);
		MGL_setBackColor(1);
		}
	else {
		MGL_setColorRGB(100,100,100);
		}

	MGL_clearViewport();

	if(!mouseCursorDemo(dc)) return;
}
