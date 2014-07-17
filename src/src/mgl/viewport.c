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
* Description:	Viewport control routines
*
****************************************************************************/

#include "mgl.h"

/*------------------------- Implementation --------------------------------*/

/****************************************************************************
PARAMETERS:
left,top	- Left top coordinate
right,botom	- Right bottom coordinate

REMARKS:
Generic routine to set the internal clipping rectangle values. This routine
simply sets both the viewport space and screen space clipping rectangles. We
also ensure that the clip rectangle does not go outside of the full screen
clip space.

We also set the internal clipping rectangle to be translated into screen
space, along with fixed point versions of the same thing.
****************************************************************************/
static void __MGL_setClipRect(
	MGLDC *dc,
	int left,
	int top,
	int right,
	int bottom)
{
	rect_t	r;
	int		adjustX = dc->a.viewPort.left - dc->a.viewPortOrg.x;
	int		adjustY = dc->a.viewPort.top - dc->a.viewPortOrg.y;

	/* Clip to screen clipping boundary */
	r.left = left;	r.right = right;
	r.top = top;    r.bottom = bottom;
	MGL_offsetRect(r,adjustX,adjustY);
	MGL_sectRectFast(r,dc->size,&r);
	MGL_offsetRect(r,-adjustX,-adjustY);

	/* Now set internal clipping rectangles */
	dc->a.clipRect.left = r.left;
	dc->a.clipRect.top = r.top;
	dc->a.clipRect.right = r.right;
	dc->a.clipRect.bottom = r.bottom;
	dc->clipRectFX.left = MGL_TOFIX(r.left);
	dc->clipRectFX.top = MGL_TOFIX(r.top);
	dc->clipRectFX.right = MGL_TOFIX(r.right);
	dc->clipRectFX.bottom = MGL_TOFIX(r.bottom);
	dc->intClipRect.left = r.left + adjustX;
	dc->intClipRect.top = r.top + adjustY;
	dc->intClipRect.right = r.right + adjustX;
	dc->intClipRect.bottom = r.bottom + adjustY;
	dc->intClipRectFX.left = MGL_TOFIX(dc->intClipRect.left);
	dc->intClipRectFX.top = MGL_TOFIX(dc->intClipRect.top);
	dc->intClipRectFX.right = MGL_TOFIX(dc->intClipRect.right);
	dc->intClipRectFX.bottom = MGL_TOFIX(dc->intClipRect.bottom);

	/* Now let the hardware driver set it's own internal clip rectangle if
	 * that is required.
	 */
	if (dc->r.setClipRect)
		dc->r.setClipRect(dc,left,top,right,bottom);
}

/****************************************************************************
DESCRIPTION:
Sets the currently active viewport.

HEADER:
mgraph.h

PARAMETERS:
view	- New global viewport bounding rectangle

REMARKS:
Sets the dimensions of the currently active viewport. These dimensions are global to
the entire display area used by the currently active video device driver. Note that
when the viewport is changing, the viewport origin is always reset back to (0,0).

All output in MGL is relative to the current viewport, so by changing the viewport
to a new value you can make all output appear in a different rectangular portion of
the video display.

SEE ALSO:
MGL_getViewport, MGL_setRelViewport, MGL_clearViewport,
MGL_setClipRect, MGL_setViewportOrg
****************************************************************************/
void MGLAPI MGL_setViewport(
	rect_t view)
{
	DC.a.viewPort.left 		= (view.left += DC.size.left);
	DC.a.viewPort.top 		= (view.top += DC.size.top);
	DC.a.viewPort.right 	= (view.right += DC.size.left);
	DC.a.viewPort.bottom	= (view.bottom += DC.size.top);
	DC.viewPortFX.left 		= MGL_TOFIX(view.left);
	DC.viewPortFX.top 		= MGL_TOFIX(view.top);
	DC.viewPortFX.right 	= MGL_TOFIX(view.right);
	DC.viewPortFX.bottom 	= MGL_TOFIX(view.bottom);
	__MGL_setClipRect(&DC,0,0,view.right - view.left,view.bottom - view.top);
	DC.a.CP.x = DC.a.CP.y = 0;
	DC.a.viewPortOrg.x = DC.a.viewPortOrg.y = 0;
}

/****************************************************************************
DESCRIPTION:
Sets the currently active viewport for a specific DC.

HEADER:
mgraph.h

PARAMETERS:
dc		- Device context to change viewport for
view	- New global viewport bounding rectangle

REMARKS:
This function is the same as MGL_setViewport, however the device context
does not have to be the current device context.

SEE ALSO:
MGL_setViewport, MGL_getViewport, MGL_setRelViewport, MGL_clearViewport,
MGL_setClipRect, MGL_setViewportOrg
****************************************************************************/
void MGLAPI MGL_setViewportDC(
	MGLDC *dc,
	rect_t view)
{
	if (dc == _MGL_dcPtr) {
		MGL_setViewport(view);
		dc->a.viewPort = DC.a.viewPort;
		dc->a.clipRect = DC.a.clipRect;
		}
	else {
		dc->a.viewPort.left 	= (view.left += dc->size.left);
		dc->a.viewPort.top 		= (view.top += dc->size.top);
		dc->a.viewPort.right 	= (view.right += dc->size.left);
		dc->a.viewPort.bottom	= (view.bottom += dc->size.top);
		dc->viewPortFX.left 	= MGL_TOFIX(view.left);
		dc->viewPortFX.top 		= MGL_TOFIX(view.top);
		dc->viewPortFX.right 	= MGL_TOFIX(view.right);
		dc->viewPortFX.bottom 	= MGL_TOFIX(view.bottom);
		__MGL_setClipRect(dc,0,0,view.right - view.left,view.bottom - view.top);
		dc->a.CP.x = dc->a.CP.y = 0;
		dc->a.viewPortOrg.x = dc->a.viewPortOrg.y =0;
		}
}

/****************************************************************************
DESCRIPTION:
Sets a viewport relative to the current viewport.

HEADER:
mgraph.h

PARAMETERS:
view	- Bounding rectangle for the new viewport

REMARKS:
Sets the current viewport to the viewport specified by view, relative to the currently
active viewport. The new viewport is restricted to fall within the bounds of the
currently active viewport. Note that when the viewport is changing, the viewport
origin is always reset back to (0,0).

All output in MGL is relative to the current viewport, so by changing the viewport
to a new value you can make all output appear in a different rectangular portion of
the video display.

SEE ALSO:
MGL_getViewport, MGL_setViewport, MGL_clearViewport, MGL_setClipRect,
MGL_setViewportOrg
****************************************************************************/
void MGLAPI MGL_setRelViewport(
	rect_t view)
{
	MGL_offsetRect(view,DC.a.viewPort.left,DC.a.viewPort.top);
	MGL_setViewport(view);
}

/****************************************************************************
DESCRIPTION:
Sets a viewport relative to the current viewport for a specific DC.

HEADER:
mgraph.h

PARAMETERS:
dc		- Device context to change viewport for
view	- Bounding rectangle for the new viewport

REMARKS:
This function is the same as MGL_setRelViewport, however the device context
does not have to be the current device context.

SEE ALSO:
MGL_setRelViewport, MGL_getViewport, MGL_setViewport, MGL_clearViewport,
MGL_setClipRect, MGL_setViewportOrg
****************************************************************************/
void MGLAPI MGL_setRelViewportDC(
	MGLDC *dc,
	rect_t view)
{
	MGL_offsetRect(view,dc->a.viewPort.left,dc->a.viewPort.top);
	MGL_setViewportDC(dc,view);
}

/****************************************************************************
DESCRIPTION:
Sets the logical viewport origin.

HEADER:
mgraph.h

PARAMETERS:
org	- New logical viewport origin.

REMARKS:
This function sets the currently active viewport origin. When a new viewport is set
with the MGL_setViewport function, the viewport origin is reset to (0,0), which
means that any primitives drawn at pixel location (0,0) will appear at the top left
hand corner of the viewport.

You can change the logical coordinate of the viewport origin to any value you
please, which will effectively offset all drawing within the currently active
viewport. Hence if you set the viewport origin to (10,10), drawing a pixel at (10,10)
would make it appear at the top left hand corner of the viewport.

SEE ALSO:
MGL_setViewportOrgDC, MGL_getViewportOrg, MGL_setViewport
****************************************************************************/
void  MGLAPI MGL_setViewportOrg(
	point_t org)
{
	DC.a.viewPortOrg = org;
	MGL_offsetRect(DC.a.viewPort,-org.x,-org.y);
}

/****************************************************************************
DESCRIPTION:
Sets the logical viewport origin for a specific DC.

HEADER:
mgraph.h

PARAMETERS:
dc	- Device context to change viewport for
org	- New logical viewport origin.

REMARKS:
This function is the same as MGL_setViewportOrg, however the device context
does not have to be the current device context.

SEE ALSO:
MGL_setViewportOrg, MGL_getViewportOrg, MGL_setViewport
****************************************************************************/
void  MGLAPI MGL_setViewportOrgDC(
	MGLDC *dc,
	point_t org)
{
	if (dc == _MGL_dcPtr) {
		dc->a.viewPortOrg = DC.a.viewPortOrg = org;
		MGL_offsetRect(DC.a.viewPort,-org.x,-org.y);
		dc->a.viewPort = DC.a.viewPort;
		}
	else {
		dc->a.viewPortOrg = org;
		MGL_offsetRect(dc->a.viewPort,-org.x,-org.y);
		}
}

/****************************************************************************
DESCRIPTION:
Returns the current viewport origin.

HEADER:
mgraph.h

PARAMETERS:
org	- Place to store the viewport origin

REMARKS:
This function returns a copy of the currently active viewport origin. When a new
viewport is set with the MGL_setViewport function, the viewport origin is reset to
(0,0), which means that any primitives drawn at pixel location (0,0) will appear at
the top left hand corner of the viewport.

You can change the logical coordinate of the viewport origin to any value you
please, which will effectively offset all drawing within the currently active
viewport. Hence if you set the viewport origin to (10,10), drawing a pixel at (10,10)
would make it appear at the top left hand corner of the viewport.

SEE ALSO:
MGL_getViewportOrgDC, MGL_setViewport, MGL_setViewportOrg
****************************************************************************/
void MGLAPI MGL_getViewportOrg(
	point_t *org)
{
	*org = DC.a.viewPortOrg;
}

/****************************************************************************
DESCRIPTION:
Returns the current viewport origin for a specific DC.

HEADER:
mgraph.h

PARAMETERS:
dc	- Device context to change viewport for
org	- Place to store the viewport origin

REMARKS:
This function is the same as MGL_getViewportOrg, however the device context
does not have to be the current device context.

SEE ALSO:
MGL_getViewportOrg, MGL_setViewport, MGL_setViewportOrg
****************************************************************************/
void MGLAPI MGL_getViewportOrgDC(
	MGLDC *dc,
	point_t *org)
{
	if (dc == _MGL_dcPtr)
		*org = DC.a.viewPortOrg;
	else
		*org = dc->a.viewPortOrg;
}

/****************************************************************************
DESCRIPTION:
Returns the currently active viewport.

HEADER:
mgraph.h

PARAMETERS:
view	- Place to store the current viewport

REMARKS:
This function returns a copy of the currently active viewport. These dimensions are
global to the entire device context surface. When the viewport is changed with this
function, the viewport origin is reset to (0,0).

All output in MGL is relative to the current viewport, so by changing the viewport
to a new value you can make all output appear in a different rectangular portion of
the device surface.

SEE ALSO:
MGL_getViewportDC, MGL_setViewport, MGL_setRelViewport, MGL_setViewportOrg,
MGL_clearViewport, MGL_setClipRect
****************************************************************************/
void MGLAPI MGL_getViewport(
	rect_t *view)
{
	*view = DC.a.viewPort;
}

/****************************************************************************
DESCRIPTION:
Returns the currently active viewport for a specific DC.

HEADER:
mgraph.h

PARAMETERS:
dc		- Device context to change viewport for
view	- Place to store the current viewport

REMARKS:
This function is the same as MGL_getViewport, however the device context
does not have to be the current device context.

SEE ALSO:
MGL_getViewport, MGL_setViewport, MGL_setRelViewport, MGL_setViewportOrg,
MGL_clearViewport, MGL_setClipRect
****************************************************************************/
void MGLAPI MGL_getViewportDC(
	MGLDC *dc,
	rect_t *view)
{
	if (dc == _MGL_dcPtr)
		*view = DC.a.viewPort;
	else
		*view = dc->a.viewPort;
}

/****************************************************************************
DESCRIPTION:
Converts a point from global to local coordinates.

HEADER:
mgraph.h

PARAMETERS:
dc	- Device context in which the point is defined
p	- Pointer to point to be converted

REMARKS:
This function converts a coordinate from global coordinates to local coordinates.
Global coordinates are defined relative to the entire output device context surface,
while local coordinates are relative to the currently active viewport.

This routine is usually used to convert mouse coordinate values from global screen
coordinates to the local coordinate system of the currently active viewport.

SEE ALSO:
MGL_globalToLocalDC, MGL_localToGlobal
****************************************************************************/
void MGLAPI MGL_globalToLocal(
	point_t *p)
{
	p->x -= DC.a.viewPort.left;
	p->y -= DC.a.viewPort.top;
}

/****************************************************************************
DESCRIPTION:
Converts a point from global to local coordinates.

HEADER:
mgraph.h

PARAMETERS:
dc	- Device context in which the point is defined
p	- Pointer to point to be converted

REMARKS:
This function is the same as MGL_globalToLocal, however the device context
does not have to be the current device context.

SEE ALSO:
MGL_globalToLocal, MGL_localToGlobal
****************************************************************************/
void MGLAPI MGL_globalToLocalDC(
	MGLDC *dc,
	point_t *p)
{
	if (dc == _MGL_dcPtr) {
		p->x -= DC.a.viewPort.left;
		p->y -= DC.a.viewPort.top;
		}
	else {
		p->x -= dc->a.viewPort.left;
		p->y -= dc->a.viewPort.top;
		}
}

/****************************************************************************
DESCRIPTION:
Converts a point from local to global coordinates.

HEADER:
mgraph.h

PARAMETERS:
p	- Pointer to point to be converted

REMARKS:
This function converts a coordinate from local coordinates to global coordinates.
Global coordinates are defined relative to the entire output device display, while
local coordinates are relative to the currently active viewport.

SEE ALSO:
MGL_localToGlobalDC, MGL_globalToLocal
****************************************************************************/
void MGLAPI MGL_localToGlobal(
	point_t *p)
{
	p->x += DC.a.viewPort.left;
	p->y += DC.a.viewPort.top;
}

/****************************************************************************
DESCRIPTION:
Converts a point from local to global coordinates for a specific DC.

HEADER:
mgraph.h

PARAMETERS:
dc	- Device context in which the point is defined
p	- Pointer to point to be converted

REMARKS:
This function is the same as MGL_localToGlobal, however the device context
does not have to be the current device context.

SEE ALSO:
MGL_localToGlobal, MGL_globalToLocal
****************************************************************************/
void MGLAPI MGL_localToGlobalDC(
	MGLDC *dc,
	point_t *p)
{
	if (dc == _MGL_dcPtr) {
		p->x += DC.a.viewPort.left;
		p->y += DC.a.viewPort.top;
		}
	else {
		p->x += dc->a.viewPort.left;
		p->y += dc->a.viewPort.top;
		}
}

/****************************************************************************
DESCRIPTION:
Clears the currently active display page.

HEADER:
mgraph.h

REMARKS:
This function will clear the entire currently active display page in the current
background color. This is the fastest way to clear an entire display page, but if you
wish to only clear a portion of the page, use the MGL_clearViewport routine
instead.

SEE ALSO:
MGL_clearViewport
****************************************************************************/
void MGLAPI MGL_clearDevice(void)
{
	/* Save the current pen attributes */
	int 	svColor = DC.a.color;
	color_t	svMode = DC.a.writeMode;

	/* Set the current pen attributes to those required for the
	 * background color.
	 */
	DC.r.SetForeColor(DC.a.backColor);
	DC.r.SetMix(MGL_REPLACE_MODE);
	DC.r.solid.DrawRect(DC.size.left,DC.size.top,
		DC.size.right-DC.size.left,DC.size.bottom-DC.size.top);

	/* Restore the pen attributes */
	DC.r.SetForeColor(svColor);
	DC.r.SetMix(svMode);
}

/****************************************************************************
DESCRIPTION:
Clears the currently active viewport.

HEADER:
mgraph.h

REMARKS:
This function will clear the currently active display page viewport to the current
background color. This is the fastest way to clear a rectangular viewport, but you
may also wish to use the MGL_fillRect routine to fill with
an arbitrary pattern instead, as this function always clears the viewport to the solid
background color.

SEE ALSO:
MGL_clearDevice, MGL_fillRect
****************************************************************************/
void MGLAPI MGL_clearViewport(void)
{
	/* Save the current pen attributes */
	int 	svColor = DC.a.color;
	color_t	svMode = DC.a.writeMode;

	/* Set the current pen attributes to those required for the
	 * background color.
	 */
	DC.r.SetForeColor(DC.a.backColor);
	DC.r.SetMix(MGL_REPLACE_MODE);
	DC.r.solid.DrawRect(DC.a.viewPort.left,DC.a.viewPort.top,
		DC.a.viewPort.right-DC.a.viewPort.left,
		DC.a.viewPort.bottom-DC.a.viewPort.top);

	/* Restore the pen attributes */
	DC.r.SetForeColor(svColor);
	DC.r.SetMix(svMode);
}

/****************************************************************************
DESCRIPTION:
Sets the current clipping rectangle.

HEADER:
mgraph.h

PARAMETERS:
clip	- New clipping rectangle to be used

REMARKS:
Sets the current clipping rectangle coordinates. The current clipping
rectangle is used to clip all output, and is always defined as being
relative to the currently active viewport. The clipping rectangle can be
no larger than the currently active viewport, and will be truncated if
an attempt is made to allow clipping outside of the active viewport.

Note:	Setting a new clip rectangle with this function clears any
		existing complex clip region in the device context.

SEE ALSO:
MGL_setClipRectDC, MGL_getClipRect, MGL_setViewport, MGL_getViewport,
MGL_setClipRegion
****************************************************************************/
void MGLAPI MGL_setClipRect(
	rect_t clip)
{
	rect_t	view;

	view.left = view.top = 0;
	view.right = DC.a.viewPort.right - DC.a.viewPort.left;
	view.bottom = DC.a.viewPort.bottom - DC.a.viewPort.top;
	MGL_sectRectFast(view,clip,&DC.a.clipRect);
	__MGL_setClipRect(&DC,DC.a.clipRect.left,DC.a.clipRect.top,
				   DC.a.clipRect.right,DC.a.clipRect.bottom);

	/* If a clip region exists, destroy it. */
	if (DC.a.clipRegion)
		MGL_freeRegion(DC.a.clipRegion);
	if (DC.intClipRegion)
		MGL_freeRegion(DC.intClipRegion);
	DC.a.clipRegion = NULL;
	DC.intClipRegion = NULL;

	/* Make sure the orignal DC up to date */
	_MGL_dcPtr->a.clipRegion = NULL;
	_MGL_dcPtr->intClipRegion = NULL;
}

/****************************************************************************
DESCRIPTION:
Sets the current clipping rectangle for a specific DC.

HEADER:
mgraph.h

PARAMETERS:
dc		- Display device context in which the rectangle is located .
clip	- New clipping rectangle to be used

REMARKS:
This function is the same as MGL_setClipRect, however the device context
does not have to be the current device context.

Note:	Setting a new clip rectangle with this function clears any
		existing complex clip region in the device context.

SEE ALSO:
MGL_setClipRect, MGL_getClipRect, MGL_setViewport, MGL_getViewport,
MGL_setClipRegion
****************************************************************************/
void MGLAPI MGL_setClipRectDC(
	MGLDC *dc,
	rect_t clip)
{
	rect_t	view;

	view.left = view.top = 0;
	if (dc == _MGL_dcPtr) {
		view.right = DC.a.viewPort.right - DC.a.viewPort.left;
		view.bottom = DC.a.viewPort.bottom - DC.a.viewPort.top;
		MGL_sectRectFast(view,clip,&DC.a.clipRect);
		__MGL_setClipRect(&DC,DC.a.clipRect.left,DC.a.clipRect.top,
					   DC.a.clipRect.right,DC.a.clipRect.bottom);
		dc->a.clipRect = DC.a.clipRect;
		dc->clipRectFX = DC.clipRectFX;
		dc->intClipRect = DC.intClipRect;
		dc->intClipRectFX = DC.intClipRectFX;

		/* If a clip region exists, destroy it. */
		if (DC.a.clipRegion)
			MGL_freeRegion(DC.a.clipRegion);
		if (DC.intClipRegion)
			MGL_freeRegion(DC.intClipRegion);
		DC.a.clipRegion = NULL;
		DC.intClipRegion = NULL;
		}
	else {
		view.right = dc->a.viewPort.right - dc->a.viewPort.left;
		view.bottom = dc->a.viewPort.bottom - dc->a.viewPort.top;
		MGL_sectRectFast(view,clip,&dc->a.clipRect);
		__MGL_setClipRect(dc,dc->a.clipRect.left,dc->a.clipRect.top,
						dc->a.clipRect.right,dc->a.clipRect.bottom);

		/* If a clip region exists, destroy it. */
		if (dc->a.clipRegion)
			MGL_freeRegion(dc->a.clipRegion);
		if (dc->intClipRegion)
			MGL_freeRegion(dc->intClipRegion);
		}

	/* Make sure the orignal DC up to date */
	dc->a.clipRegion = NULL;
	dc->intClipRegion = NULL;
}

/****************************************************************************
DESCRIPTION:
Returns the current clipping rectangle.

HEADER:
mgraph.h

PARAMETERS:
clip	- Place to store the current clipping rectangle

REMARKS:
Returns the current clipping rectangle coordinates. The current clipping
rectangle is used to clip all output, and is always defined as being
relative to the currently active viewport. The clipping rectangle can be
no larger than the currently active viewport.

Note:	If a complex clip region is currently active, this function returns
		the bounding rectangle for the complex clip region.

SEE ALSO:
MGL_getClipRectDC, MGL_setClipRect, MGL_getClipRegion
****************************************************************************/
void MGLAPI MGL_getClipRect(
	rect_t *clip)
{
	*clip = DC.a.clipRect;
}

/****************************************************************************
DESCRIPTION:
Returns the current clipping rectangle for a specific DC.

HEADER:
mgraph.h

PARAMETERS:
dc		- Display device context in which the rectangle is defined
clip	- Place to store the current clipping rectangle

REMARKS:
This function is the same as MGL_getClipRect, however the device context
does not have to be the current device context.

Note:	If a complex clip region is currently active, this function returns
		the bounding rectangle for the complex clip region.

SEE ALSO:
MGL_getClipRect, MGL_setClipRect, MGL_getClipRegion
****************************************************************************/
void MGLAPI MGL_getClipRectDC(
	MGLDC *dc,
	rect_t *clip)
{
	if (dc == _MGL_dcPtr)
		*clip = DC.a.clipRect;
	else
		*clip = dc->a.clipRect;
}

/****************************************************************************
DESCRIPTION:
Sets the current complex clipping region.

HEADER:
mgraph.h

PARAMETERS:
clip	- New complex clipping region to be used

REMARKS:
This function sets the current complex clipping region for the current device
context. Complex clip regions are defined as unions of rectangles, and
allow all rendering functions in the MGL to be clipped to arbitrary
regions on the screen. Setting a complex clip region override the current
setting for a simple clip rectangle. The complex clip region is used to
clip all output, and is always defined as being relative to the currently
active viewport. The clipping region can be no larger than the currently
active viewport, and will be truncated if an attempt is made to allow
clipping outside of the active viewport.

SEE ALSO:
MGL_setClipRegionDC, MGL_getClipRegion, MGL_setViewport, MGL_getViewport,
MGL_setClipRect
****************************************************************************/
void MGLAPI MGL_setClipRegion(
	region_t *region)
{
	rect_t	view;

	CHECK(region != NULL);
	MGL_setClipRect(region->rect);
	if (!MGL_isSimpleRegion(region)) {
		view.left = view.top = 0;
		view.right = DC.a.viewPort.right - DC.a.viewPort.left;
		view.bottom = DC.a.viewPort.bottom - DC.a.viewPort.top;
		DC.a.clipRegion = MGL_sectRegionRect(region,&view);
		DC.intClipRegion = MGL_copyRegion(region);
		MGL_offsetRegion(DC.intClipRegion,DC.a.viewPort.left,DC.a.viewPort.top);
		_MGL_dcPtr->a.clipRegion = DC.a.clipRegion;
		_MGL_dcPtr->intClipRegion = DC.intClipRegion;
		}
}

/****************************************************************************
DESCRIPTION:
Sets the current complex clipping region for a specific DC.

HEADER:
mgraph.h

PARAMETERS:
dc		- Display device context in which the region is located .
clip	- New complex clipping region to be used

REMARKS:
This function sets the current complex clipping region for the specified
device context. Complex clip regions are defined as unions of rectangles, and
allow all rendering functions in the MGL to be clipped to arbitrary
regions on the screen. Setting a complex clip region override the current
setting for a simple clip rectangle. The complex clip region is used to
clip all output, and is always defined as being relative to the currently
active viewport. The clipping region can be no larger than the currently
active viewport, and will be truncated if an attempt is made to allow
clipping outside of the active viewport.

SEE ALSO:
MGL_setClipRegion, MGL_getClipRegion, MGL_setViewport, MGL_getViewport,
MGL_setClipRect
****************************************************************************/
void MGLAPI MGL_setClipRegionDC(
	MGLDC *dc,
	region_t *region)
{
	rect_t	view;

	CHECK(region != NULL);
	MGL_setClipRectDC(dc,region->rect);
	if (dc == _MGL_dcPtr) {
		MGL_setClipRegion(region);
		}
	else {
		if (!MGL_isSimpleRegion(region)) {
			view.left = view.top = 0;
			view.right = dc->a.viewPort.right - dc->a.viewPort.left;
			view.bottom = dc->a.viewPort.bottom - dc->a.viewPort.top;
			dc->a.clipRegion = MGL_sectRegionRect(region,&view);
			dc->intClipRegion = MGL_copyRegion(region);
			MGL_offsetRegion(dc->intClipRegion,dc->a.viewPort.left,dc->a.viewPort.top);
			}
		}
}

/****************************************************************************
DESCRIPTION:
Returns the current complex clipping region.

HEADER:
mgraph.h

PARAMETERS:
clip	- Place to store the current complex clipping region

REMARKS:
Returns the current complex clipping region for the current device context.
This function works even if no complex clip region is active, and only
a simple clip rectangle is in use. In this case, the resulting clip region
is a simple region that contains only the clip rectangle for the device
context.

SEE ALSO:
MGL_getClipRegionDC, MGL_setClipRegion, MGL_setClipRect
****************************************************************************/
void MGLAPI MGL_getClipRegion(
	region_t *region)
{
	CHECK(region != NULL);
	if (DC.a.clipRegion) {
		MGL_copyIntoRegion(region,DC.a.clipRegion);
		}
	else {
		MGL_clearRegion(region);
		region->rect = DC.a.clipRegion->rect;
		}
}

/****************************************************************************
DESCRIPTION:
Returns the current complex clipping region for a specific DC.

HEADER:
mgraph.h

PARAMETERS:
clip	- Place to store the current complex clipping region

REMARKS:
Returns the current complex clipping region for the specific device context.
This function works even if no complex clip region is active, and only
a simple clip rectangle is in use. In this case, the resulting clip region
is a simple region that contains only the clip rectangle for the device
context.

SEE ALSO:
MGL_getClipRegion, MGL_setClipRegion, MGL_setClipRect
****************************************************************************/
void MGLAPI MGL_getClipRegionDC(
	MGLDC *dc,
	region_t *region)
{
	CHECK(region != NULL);
	if (dc == _MGL_dcPtr) {
		if (DC.a.clipRegion)
			MGL_copyIntoRegion(region,DC.a.clipRegion);
		else {
			MGL_clearRegion(region);
			region->rect = DC.a.clipRegion->rect;
			}
		}
	else {
		if (dc->a.clipRegion)
			MGL_copyIntoRegion(region,dc->a.clipRegion);
		else {
			MGL_clearRegion(region);
			region->rect = dc->a.clipRegion->rect;
			}
		}
}
