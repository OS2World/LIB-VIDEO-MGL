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
* Description:	Mouse support routines.
*
****************************************************************************/

#include "mgl.h"

/*------------------------- Global Variables ------------------------------*/

static cursor_t cursor;
static int		mx,my,screenmx,screenmy;
static int		visible = -1,oldVisible;
static MGLDC	*DispDC = NULL;

/*------------------------- Implementation --------------------------------*/

/****************************************************************************
REMARKS:
Initialises the mouse module.
{secret}
****************************************************************************/
void _MS_init(void)
{
	visible = -1;
}

/****************************************************************************
PARAMETERS:
dc	- Display device context to display the mouse on

REMARKS:
Saves the current display device context, and allocates the memory we
require for saving the image behind the mouse cursor. We also save the
internal color representation to draw the cursor in white.
{secret}
****************************************************************************/
void _MS_setDisplayDC(
	MGLDC *dc)
{
	if ((DispDC = dc) != NULL) {
		visible = -1;
		EVT_setMouseRange(dc->mi.xRes,dc->mi.yRes);
		MS_setCursorColor(MGL_realColor(dc,MGL_WHITE));
		}
}

/****************************************************************************
PARAMETERS:
x,y	- New location for mouse cursor

REMARKS:
Routine called to move the mouse cursor to the new location. This is called
repeatedly by the event library to move the mouse cursor. This routine is
written to be able to be called from within an interrupt handler. The old
MGL used to do it this way, but the new MGL handler only moves the mouse
when the user application is processing events.
{secret}
****************************************************************************/
void EVTAPI _MS_moveCursor(
	int x,
	int y)
{
	int	screenx,screeny;

	if (DispDC) {
		screenx = (mx = x) + DispDC->startX;
		screeny = (my = y) + DispDC->startY;
		if (screenx != screenmx || screeny != screenmy) {
			screenmx = screenx;
			screenmy = screeny;
			if (visible == 0)
				DispDC->v->d.setCursorPos(DispDC,mx,my);
			}
		}
}

/****************************************************************************
REMARKS:
Save the mouse cursor state internally to be restored later.
{secret}
****************************************************************************/
void _MS_saveState(void)
{
	visible--;
	oldVisible = visible;
}

/****************************************************************************
REMARKS:
Restore the mouse cursor state that was saved previously.
{secret}
****************************************************************************/
void _MS_restoreState(void)
{
	visible = oldVisible;
	MS_show();
}

/*------------------------ Public interface routines ----------------------*/

/****************************************************************************
DESCRIPTION:
Displays the mouse cursor.

HEADER:
mgraph.h

REMARKS:
Increments the internal mouse cursor display counter, and displays the cursor
when the counter gets to zero. Calls to MS_hide decrement the counter, and
this call effectively cancels a single MS_hide call, allowing the MS_show and
MS_hide calls to be nested.

If the mouse was obscured with the MS_obscure function, this reverses the
effect and will redisplay the mouse cursor again. On systems with a hardware
mouse cursor, the MS_obscure function effectively does nothing, while on
systems using a software mouse cursor, the MS_obscure function simply calls
MS_hide.

Note that the mouse cursor display counter is reset to -1 by default when an
MGL fullscreen mode is started, so a single MS_show will display the mouse
cursor after the mode has been started.

SEE ALSO:
MS_hide, MS_obscure
****************************************************************************/
void MGLAPI MS_show(void)
{
	if (visible < 0) {
		visible++;
		if (DispDC && visible == 0) {
			// Show the mouse cursor
			DispDC->v->d.setCursorPos(DispDC,mx,my);
			DispDC->v->d.showCursor(DispDC,true);
			}
		}
}

/****************************************************************************
DESCRIPTION:
Hides the mouse cursor.

HEADER:
mgraph.h

REMARKS:
Decrements the internal mouse cursor display counter, and hides the cursor
if the counter was previously set to zero. Calls to MS_show increment the
counter, allowing the MS_show and MS_hide calls to be nested.

SEE ALSO:
MS_show, MS_obscure
***************************************************************************/
void MGLAPI MS_hide(void)
{
	visible--;
	if (DispDC && visible == -1)
		DispDC->v->d.showCursor(DispDC,false);
}

/****************************************************************************
DESCRIPTION:
Hides the mouse cursor from view during graphics output.

HEADER:
mgraph.h

REMARKS:
Hides the mouse cursor from view in order to perform graphics output using
MGL. If the graphics device driver supports a hardware cursor, this is
handled by the hardware, otherwise it is removed from the display. You should
call this routine rather than MS_hide in order to temporarily hide the mouse
cursor during graphics output as the MS_hide routine will always hide the
cursor, regardless of whether the system has a hardware mouse cursor or not.

SEE ALSO:
MS_show, MS_hide
****************************************************************************/
void MGLAPI MS_obscure(void)
{
	if (DispDC && (visible < 0 || !DispDC->v->d.hardwareCursor))
		MS_hide();
}

/****************************************************************************
DESCRIPTION:
Sets the mouse cursor shape.

HEADER:
mgraph.h

PARAMETERS:
curs	- Pointer to new mouse cursor shape

REMARKS:
Sets the graphics mouse cursor shape, passed in the cursor_t structure. The
cursor_t structure contains a mouse cursor AND mask and a mouse cursor XOR
mask that is used to display the cursor on the screen, along with the mouse
cursor hotspot location. Refer to the cursor_t structure definition for more
information.
****************************************************************************/
void MGLAPI MS_setCursor(
	cursor_t *curs)
{
	MS_hide();
	switch (curs->m.colors) {
		case 2:
			memcpy(&cursor,curs,sizeof(mono_cursor_t));
			break;
		case 16:
			memcpy(&cursor,curs,sizeof(color16_cursor_t));
			break;
		case 256:
			memcpy(&cursor,curs,sizeof(color256_cursor_t));
			break;
		case 16777216:
			memcpy(&cursor,curs,sizeof(colorRGBA_cursor_t));
			break;
		}
	if (DispDC)
		DispDC->v->d.setCursor(DispDC,curs);
	MS_show();
}

/****************************************************************************
DESCRIPTION:
Sets the current mouse cursor color.

HEADER:
mgraph.h

PARAMETERS:
color	- New mouse cursor color, in current display mode format.

REMARKS:
Sets the color for the mouse cursor to the specified color, which is passed
in as a packed MGL color in the proper format for the current display mode
(either a color index or a packed RGB color value). By default the mouse
cursor is set to white, which is a color index of 15 by default in MGL. If
you re-program the color palette in 4 or 8 bit modes, you will need to reset
the mouse cursor value to the value that represents white.
****************************************************************************/
void MGLAPI MS_setCursorColor(
	color_t color)
{
	if (DispDC)
		DispDC->v->d.setCursorColor(DispDC,color);
}

/****************************************************************************
DESCRIPTION:
Moves the mouse cursor to a new location.

HEADER:
mgraph.h

PARAMETERS:
x	- New mouse x coordinate (screen coordinates)
y	- New mouse y coordinate (screen coordinates)

REMARKS:
Moves the mouse cursor to the specified location in screen coordinates.

Note that it is not usually a good idea to move the mouse cursor around while
the user is interacting with the application, but this can be used to
restore the mouse cursor to a known location if it has been hidden for a
long period of time.

SEE ALSO:
MS_getPos
****************************************************************************/
void MGLAPI MS_moveTo(
	int x,
	int y)
{
	EVT_setMousePos(x,y);
}

/****************************************************************************
DESCRIPTION:
Returns the current mouse cursor location.

HEADER:
mgraph.h

PARAMETERS:
x	- Place to store value for mouse x coordinate (screen coordinates)
y	- Place to store value for mouse y coordinate (screen coordinates)

REMARKS:
Obtains the current mouse cursor position in screen coordinates. Normally
the mouse cursor location is tracked using the mouse movement events that
are posted to the event queue when the mouse moves, however this routine
provides an alternative method of polling the mouse cursor location.

SEE ALSO:
MS_moveTo
****************************************************************************/
void MGLAPI MS_getPos(
	int *x,
	int *y)
{
	EVT_getMousePos(x,y);
}
