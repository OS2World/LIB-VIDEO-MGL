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
* Environment:	Windows
*
* Description:  Public Windows specific functions for the MGL.
*
****************************************************************************/

#include "mgl.h"
#include "mglwin\internal.h"

/*------------------------- Implementation --------------------------------*/

/****************************************************************************
DESCRIPTION:
Activates the Windows palette for a windowed DC.

HEADER:
mglwin.h

PARAMETERS:
dc			- Device context (must be a Windowed DC)
unrealize	- True if the palette should be unrealized first before realizing it.

RETURNS:
True if the palette has changed and the window will be repainted, or false if
the palette did not change.

REMARKS:
Activates the Windows palette for a windowed device context for an MGL
windowed application running under Windows. This function is usually in
response to the WM_PALETTECHANGED and WM_QUERYNEWPALETTE messages as
follows:

If you are chainging between SYSPAL_STATIC and SYSPAL_NOSTATIC modes,
you should always pass true for the unrealize flag to ensure that the
palette is correctly updated.

SEE ALSO:
MGL_setWinDC
****************************************************************************/
ibool MGLAPI MGL_activatePalette(
	MGLDC *dc,
	ibool unrealize)
{
	ibool		palChanged = false;
	HPALETTE	hPalOld;

	if (dc->wm.windc.hpal) {
		HDC hdc = GetDC(dc->wm.windc.hwnd);
		if (unrealize)
			UnrealizeObject(dc->wm.windc.hpal);
		hPalOld = SelectPalette(hdc,dc->wm.windc.hpal, false);
		palChanged = RealizePalette(hdc);
		SelectPalette(hdc,hPalOld, false);
		ReleaseDC(dc->wm.windc.hwnd, hdc);
		}
	return palChanged;
}

/****************************************************************************
DESCRIPTION:
Associate a window manager device context with an MGL device context.

HEADER:
mglwin.h

PARAMETERS:
dc	- MGL windowed device context to use
hdc	- Handle to window manager device context to associate

RETURNS:
True if the application's palette has changed, false if not.

REMARKS:
This function and its and the corresponding function MGL_endPaint() should
be called between the windows BeginPaint and EndPaint messages. This
function allows MGL to use the newest clipping regions and viewport settings.

MGL_beginPaint() and MGL_endPaint() must bracket drawing functions that draw
to a window type with a style of CS_PARENTDC or CS_CLASSDC. Such as dialog
box controls. These types of windows allocate device handles on the fly so
the HDC may change between calls to GetDC() or BeginPaint(). Therefore MGL
cannot draw to these types of windows without knowing the new HDC after
every BeginPaint() or GetDC() call.

OpenGL windows should NOT use MGL_beginPaint and MGL_endPaint.

A typical Windows WM_PAINT handler would be coded as follows:

	case WM_PAINT:
		hdc = BeginPaint(hwnd,&ps);
		MGL_beginPaint(winDC,hdc);
		// Do rasterizing code in here //
		MGL_bitBlt(winDC,memDC,r,0,0,MGL_REPLACE_MODE);
		MGL_endPaint(winDC);
		EndPaint(hwnd,&ps);
		return 0;

SEE ALSO:
MGL_EndPaint, MGL_activatePalette
****************************************************************************/
ibool MGLAPI MGL_beginPaint(
	MGLDC *dc,
	MGL_HDC hdc)
{
    ibool	palChanged = false;

	if (dc->deviceType == MGL_WINDOWED_DEVICE) {
		/* We trade our internal HDC for the one passed in from windows
		 * this will cause drawing to occur with the correct viewport and
		 * clipping settings (as passed in from the windows BeginPaint
		 * function)
		 */
		dc->wm.windc.hdc = hdc;
		
		/* If we have a palette for the DC, then select & realize it,
		 * returning true if the palette changed.
		 */
		if (dc->wm.windc.hpal) {
			SelectPalette(dc->wm.windc.hdc,dc->wm.windc.hpal,false);
			palChanged = RealizePalette(dc->wm.windc.hdc);
			}
		}
	return palChanged;
}

/****************************************************************************
DESCRIPTION:
Cleans up after a previous call to MGL_beginPaint.

HEADER:
mglwin.h

PARAMETERS:
dc	- MGL windowed device context to use

REMARKS:
This function and its and the corresponding function MGL_beginPaint() should
be called between the windows BeginPaint and EndPaint messages.

MGL_beginPaint() and MGL_endPaint() must bracket drawing functions that draw
to a window type with a style of CS_PARENTDC or CS_CLASSDC. Such as dialog
box controls. These types of windows allocate device handles on the fly so
the HDC may change between calls to GetDC() or BeginPaint(). Therefore MGL
cannot draw to these types of windows without knowing the new HDC after
every BeginPaint() or GetDC() call.

A typical Windows WM_PAINT handler would be coded as follows:

	case WM_PAINT:
		hdc = BeginPaint(hwnd,&ps);
		MGL_beginPaint(winDC,hdc);
		// Do rasterizing code in here //
		MGL_bitBlt(winDC,memDC,r,0,0,MGL_REPLACE_MODE);
		MGL_endPaint(winDC);
		EndPaint(hwnd,&ps);
		return 0;

SEE ALSO:
MGL_beginPaint
****************************************************************************/
void MGLAPI MGL_endPaint(
	MGLDC *dc)
{
	if (dc->deviceType == MGL_WINDOWED_DEVICE) {
		/* Here we reset the MGLDC's HDC to the internal one.
		 * If the window has no private DC the HDC will be set to NULL
		 * and drawing operations will not work until a new
		 * MGL_beginPaint/MGL_endPaint bracket.
		 */
 		dc->wm.windc.hdc = dc->wm.windc.hdcOwned;
		}
}

/****************************************************************************
DESCRIPTION:
Resizes the windowed device context after a WM_SIZE	message has been captured.

HEADER:
mglwin.h

PARAMETERS:
dc	- MGL device context to resize

REMARKS:
This function is used to re-size a windowed DC, and is more efficient than
re-creating the windowed device context for every WM_SIZE message
(especially if hardware OpenGL has been initialized for the device context).
****************************************************************************/
void MGLAPI MGL_resizeWinDC(
	MGLDC *dc)
{
	RECT	r;
	rect_t	view;

	if (dc->deviceType == MGL_WINDOWED_DEVICE) {
		GetClientRect(dc->wm.windc.hwnd,&r);
		dc->mi.xRes = r.right - r.left - 1;
		dc->mi.yRes = r.bottom - r.top - 1;
		view.left = view.top = 0;
		view.right = dc->mi.xRes+1;
		view.bottom = dc->mi.yRes+1;
		dc->size = view;
		MGL_setViewportDC(dc,view);
		}
}

/****************************************************************************
DESCRIPTION:
Function to call when your app becomes active.

HEADER:
mglwin.h

PARAMETERS:
winDC	- Currently active windowed DC
active	- True if app is active, false if not

REMARKS:
Tracks whether the application is currently active for palette management.
Forces a repaint if the system palette usage is set to no-static.
****************************************************************************/
void MGLAPI MGL_appActivate(
	MGLDC *winDC,
	ibool active)
{
	if (winDC && MGL_isWindowedDC(winDC)) {
		HDC	hdc = GetDC(NULL);
		if (_MGL_isBackground != 2)
			_MGL_isBackground = !active;
		if (GetSystemPaletteUse(hdc) == SYSPAL_NOSTATIC && MGL_getBitsPerPixel(winDC) <= 8) {
			/* Re-realize the hardware palette and repaint on activate */
			MGL_realizePalette(winDC,MGL_getPaletteSize(winDC),0,-1);
			InvalidateRect(winDC->wm.windc.hwnd,NULL,TRUE);
			}
		ReleaseDC(NULL, hdc);
        }
}

/****************************************************************************
DESCRIPTION:
Returns a Windows HDC for drawing on a device context using GDI

HEADER:
mglwin.h

PARAMETERS:
dc	- Device context to obtain the Windows HDC for

RETURNS:
Windows compatible device context handle (HDC) for the device context.

REMARKS:
This function allows you to obtain a handle to a Windows compatible device
context (HDC) for any MGL device context. Using the returned HDC, you can
then call regular GDI drawing functions to draw on the device context,
instead of using the MGL drawing functions. This is most useful for drawing
objects that are not supported by the MGL, such as Bezier curves.

Note: It is possible that in some instances that a valid HDC cannot be
	  created, and this function will return NULL.
****************************************************************************/
HDC MGLAPI MGL_getWinDC(
	MGLDC *dc)
{
	return (HDC)dc->r.getWinDC(dc);
}
