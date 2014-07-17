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
* Description:	MGL_putBitmap and related routines.
*
****************************************************************************/

#include "mgl.h"

/*------------------------- Implementation --------------------------------*/

/****************************************************************************
DESCRIPTION:
Draw a monochrome bitmap at the specified location.

HEADER:
mgraph.h

PARAMETERS:
dc			- Device context to draw bitmap on
x			- x coordinate to draw bitmap at
y			- y coordinate to draw bitmap at
width		- Width of the bitmap to draw in pixels
byteWidth	- Width of the bitmap image in bytes
height		- Height of the bitmap in scanlines
image		- Pointer to the buffer holding the bitmap

REMARKS:
This function draws a monochrome bitmap in the current foreground color on the
current device context. Where a bit is a 1 in the bitmap definition, a pixel is plotted
in the foreground color, where a bit is a 0 the original pixels are left alone. This
function can be used to implement fast hardware pixel masking for drawing fast
transparent bitmaps on devices that do not have a native hardware transparent BitBlt
function.
****************************************************************************/
void MGLAPI MGL_putMonoImage(
	MGLDC *dc,
	int x,
	int y,
	int width,
	int byteWidth,
	int height,
	void *image)
{
	rect_t	d,r,clip;

	/* Clip to destination device context */
	if (dc == _MGL_dcPtr)
		dc = &DC;
	d.left = x;				d.top = y;
	d.right = x + width;	d.bottom = y + height;
	if (!MGL_sectRect(dc->a.clipRect,d,&r))
		return;

	/* Now draw the bitmap */
	MAKE_HARDWARE_CURRENT(dc,false);
	if (dc->a.clipRegion) {
		/* Draw it clipped to a complex clip region */
		x += dc->a.viewPort.left;
		y += dc->a.viewPort.top;
		d.left = x;				d.top = y;
		d.right = x + width;	d.bottom = y + height;
		BEGIN_CLIP_REGION(clip,dc->intClipRegion);
			if (MGL_sectRect(clip,d,&r)) {
				if (!MGL_equalRect(d,r)) {
					/* Draw it clipped to the clip rectangle */
					dc->r.ClipMonoImage(x,y,width,height,byteWidth,image,dc->a.backMode,
                        r.left,r.top,
                        r.right,r.bottom);
					}
				else {
					/* Draw it unclipped for maximum speed */
					dc->r.PutMonoImage(x,y,width,height,byteWidth,image,dc->a.backMode);
					}
				}
		END_CLIP_REGION;
		}
	else if (!MGL_equalRect(d,r)) {
		/* Draw it clipped to the clip rectangle */
		dc->r.ClipMonoImage(x+dc->a.viewPort.left,y+dc->a.viewPort.top,
			width,height,byteWidth,image,dc->a.backMode,
			dc->intClipRect.left,dc->intClipRect.top,
			dc->intClipRect.right,dc->intClipRect.bottom);
		}
	else {
		/* Draw it unclipped for maximum speed */
		dc->r.PutMonoImage(x+dc->a.viewPort.left,y+dc->a.viewPort.top,
			width,height,byteWidth,image,dc->a.backMode);
		}
	RESTORE_HARDWARE(dc,false);
}

/****************************************************************************
DESCRIPTION:
Draw a lightweight bitmap at the specified location.

HEADER:
mgraph.h

PARAMETERS:
dc		- Device context to display bitmap on
x		- x coordinate to draw bitmap at
y		- y coordinate to draw bitmap at
bitmap	- Bitmap to display
op		- Write mode to use when drawing bitmap

REMARKS:
Draws a lightweight bitmap at the specified location. The bitmap can be
in any color format, and will be translated as necessary to the color format
required by the current device context.

Note that for maximum performance when displaying 8 bit bitmaps, you should
ensure that the color palette for the bitmap is identical to the device
context, otherwise the pixels in the bitmap will be translated during the
draw operation.

Supported write modes are enumerated in MGL_writeModeType.

SEE ALSO:
MGL_loadBitmap
****************************************************************************/
void MGLAPI MGL_putBitmap(
	MGLDC *dc,
	int x,
	int y,
	const bitmap_t *bitmap,
	int op)
{
	rect_t	d,r,clip;
	int 	left,top,right,bottom,srcLeft,srcTop,srcRight,srcBottom;

	/* Clip to destination device context */
	if (dc == _MGL_dcPtr)
		dc = &DC;
	d.left = x;						d.top = y;
	d.right = x + bitmap->width;	d.bottom = y + bitmap->height;
	if (!MGL_sectRect(dc->a.clipRect,d,&d))
		return;

	/* Now blit the bitmap */
	if (bitmap->bitsPerPixel == 1) {
		/* Bitmap is a monochrome bitmap, so simply draw this using the
		 * normal mono bitmap blitting code, but set the requested write
		 * mode and colors first. Clipping is handled by the device driver
		 * rendering.
		 */
		int 		oldop = dc->a.writeMode;
		color_t 	oldColor = dc->a.color;
		color_t 	oldBackColor = dc->a.backColor;
		palette_t	*pal = bitmap->pal;

		MAKE_HARDWARE_CURRENT(dc,false);
		dc->r.SetMix(op);
		if (pal) {
			dc->r.SetForeColor(MGL_rgbColor(dc,pal[1].red,pal[1].green,pal[1].blue));
			dc->r.SetBackColor(MGL_rgbColor(dc,pal[0].red,pal[0].green,pal[0].blue));
			}
		x += dc->a.viewPort.left;
		y += dc->a.viewPort.top;
		d.left = x;						d.top = y;
		d.right = x + bitmap->width;	d.bottom = y + bitmap->height;
		if (dc->a.clipRegion) {
			/* Draw it clipped to a complex clip region */
			BEGIN_CLIP_REGION(clip,dc->intClipRegion);
				if (MGL_sectRect(clip,d,&r)) {
					if (!MGL_equalRect(d,r)) {
						/* Draw it clipped to the clip rectangle */
						dc->r.ClipMonoImage(x,y,bitmap->width,bitmap->height,
							bitmap->bytesPerLine,bitmap->surface,dc->a.backMode,
                            r.left,r.top,
                            r.right,r.bottom);
						}
					else {
						/* Draw it unclipped for maximum speed */
						dc->r.PutMonoImage(x,y,bitmap->width,bitmap->height,
							bitmap->bytesPerLine,bitmap->surface,dc->a.backMode);
						}
					}
			END_CLIP_REGION;
			}
		else if (MGL_sectRect(dc->intClipRect,d,&r)) {
			if (!MGL_equalRect(d,r)) {
				/* Draw it clipped to the clip rectangle */
				dc->r.ClipMonoImage(x,y,
					bitmap->width,bitmap->height,bitmap->bytesPerLine,
					bitmap->surface,dc->a.backMode,
					dc->intClipRect.left,dc->intClipRect.top,
					dc->intClipRect.right,dc->intClipRect.bottom);
				}
			else {
				/* Draw it unclipped for maximum speed */
				dc->r.PutMonoImage(x,y,bitmap->width,bitmap->height,bitmap->bytesPerLine,bitmap->surface,dc->a.backMode);
				}
			}
		dc->r.SetMix(oldop);
		dc->r.SetForeColor(oldColor);
		dc->r.SetBackColor(oldBackColor);
		RESTORE_HARDWARE(dc,false);
		}
	else {
		/* Adjust to bitmap coordinates */
		MGL_offsetRect(d,-x,-y);
		x += d.left;
		y += d.top;
		MAKE_HARDWARE_CURRENT(dc,true);
		if (NEED_TRANSLATE_BM(bitmap,dc)) {
			/* Translate the pixel information when doing the Blt */
			if (dc->a.clipRegion) {
				left = d.left;
				top = d.top;
				right = d.right;
				bottom = d.bottom;
				d.left = x + dc->a.viewPort.left;
				d.top = y + dc->a.viewPort.top;
				d.right = d.left + (right-left);
				d.bottom = d.top + (bottom-top);
				BEGIN_CLIP_REGION(clip,dc->intClipRegion);
					if (MGL_sectRect(clip,d,&r)) {
						srcLeft = left + (r.left - d.left);
						srcRight = srcLeft + (r.right - r.left);
						srcTop = top + (r.top - d.top);
						srcBottom = srcTop + (r.bottom - r.top);
						dc->r.ConvertBltSys(bitmap->surface,bitmap->bytesPerLine,
							srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
							r.left,r.top,bitmap->bitsPerPixel,GAPF(bitmap->pf),
							GAPAL(dc->colorTab),GAPAL(bitmap->pal),dc->a.ditherMode,op,false);
						}
				END_CLIP_REGION;
				}
			else {
				dc->r.ConvertBltSys(bitmap->surface,bitmap->bytesPerLine,
					d.left,d.top,d.right-d.left,d.bottom-d.top,
					x + dc->a.viewPort.left,y + dc->a.viewPort.top,
					bitmap->bitsPerPixel,GAPF(bitmap->pf),GAPAL(dc->colorTab),
					GAPAL(bitmap->pal),dc->a.ditherMode,op,false);
				}
			}
		else {
			if (dc->a.clipRegion) {
				left = d.left;
				top = d.top;
				right = d.right;
				bottom = d.bottom;
				d.left = x + dc->a.viewPort.left;
				d.top = y + dc->a.viewPort.top;
				d.right = d.left + (right-left);
				d.bottom = d.top + (bottom-top);
				BEGIN_CLIP_REGION(clip,dc->intClipRegion);
					if (MGL_sectRect(clip,d,&r)) {
						srcLeft = left + (r.left - d.left);
						srcRight = srcLeft + (r.right - r.left);
						srcTop = top + (r.top - d.top);
						srcBottom = srcTop + (r.bottom - r.top);
						dc->r.BitBltSys(bitmap->surface,bitmap->bytesPerLine,
							srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
							r.left,r.top,op,false);
						}
				END_CLIP_REGION;
				}
			else {
				dc->r.BitBltSys(bitmap->surface,bitmap->bytesPerLine,
					d.left,d.top,d.right-d.left,d.bottom-d.top,
					x + dc->a.viewPort.left,y + dc->a.viewPort.top,op,false);
				}
			}
		RESTORE_HARDWARE(dc,true);
		}
}

/****************************************************************************
DESCRIPTION:
Draw a section of a lightweight bitmap at the specified location.

HEADER:
mgraph.h

PARAMETERS:
dc		- Device context to display bitmap on
left	- Left coordinate of section to draw
top		- Top coordinate of section to draw
right	- Right coordinate of section to draw
bottom	- Bottom coordinate of section to draw
dstLeft	- Left coordinate of destination of bitmap section
dstTop	- Right coordinate for destination of bitmap section
bitmap	- Bitmap to display
op		- Write mode to use when drawing bitmap

REMARKS:
Draws a section of a lightweight bitmap at the specified location. The bitmap
can be in any color format, and will be translated as necessary to the color
format required by the current device context.

Note that for maximum performance when displaying 8 bit bitmaps, you should
ensure that the color palette for the bitmap is identical to the device
context, otherwise the pixels in the bitmap will be translated during the
draw operation.

SEE ALSO:
MGL_loadBitmap
****************************************************************************/
void MGLAPI MGL_putBitmapSection(
	MGLDC *dc,
	int left,
	int top,
	int right,
	int bottom,
	int dstLeft,
	int dstTop,
	const bitmap_t *bitmap,
	int op)
{
	rect_t	d,r,clip;
	int 	srcLeft,srcTop,srcRight,srcBottom;

	/* Clip the source rectangle to the bitmap dimensions */
	if (dc == _MGL_dcPtr)
		dc = &DC;
	if (!MGL_sectRectCoord(0,0,bitmap->width,bitmap->height,left,top,right,bottom,&d))
		return;
	dstLeft += (d.left - left);	dstTop += (d.top - top);
	left = d.left;				right = d.right;
	top = d.top;				bottom = d.bottom;

	/* Clip to destination device context */
	d.left = dstLeft;					d.top = dstTop;
	d.right = dstLeft + (right-left);	d.bottom = dstTop + (bottom-top);
	if (!MGL_sectRect(dc->a.clipRect,d,&d))
		return;
	left += (d.left - dstLeft);	right = left + (d.right - d.left);
	top += (d.top - dstTop);	bottom = top + (d.bottom - d.top);

	/* Blt the pixels to the device */
	if (bitmap->bitsPerPixel == 1) {
		int 		oldop = dc->a.writeMode;
		color_t 	oldColor = dc->a.color;
		color_t 	oldBackColor = dc->a.backColor;
		palette_t	*pal = bitmap->pal;

		MAKE_HARDWARE_CURRENT(dc,false);
		dc->r.SetMix(op);
		if (pal) {
			dc->r.SetForeColor(MGL_rgbColor(dc,pal[1].red,pal[1].green,pal[1].blue));
			dc->r.SetBackColor(MGL_rgbColor(dc,pal[0].red,pal[0].green,pal[0].blue));
			}
		if (dc->a.clipRegion) {
			/* Draw it clipped to a complex clip region */
			dstLeft += dc->a.viewPort.left - left;
			dstTop += dc->a.viewPort.top - top;
			d.left = dstLeft + dc->a.viewPort.left;
			d.top = dstTop + dc->a.viewPort.top;
			d.right = d.left + (right-left);
			d.bottom = d.top + (bottom-top);
			BEGIN_CLIP_REGION(clip,dc->intClipRegion);
				if (MGL_sectRect(clip,d,&r)) {
					/* Draw it clipped to the clip rectangle */
					dc->r.ClipMonoImage(
						dstLeft,dstTop,bitmap->width,bitmap->height,
						bitmap->bytesPerLine,bitmap->surface,dc->a.backMode,
						d.left,d.top,d.right,d.bottom);
					}
			END_CLIP_REGION;
			}
		else {
			/* Draw it clipped to the clip rectangle */
			dc->r.ClipMonoImage(
				dstLeft+dc->a.viewPort.left-left,dstTop+dc->a.viewPort.top-top,
				bitmap->width,bitmap->height,bitmap->bytesPerLine,
				bitmap->surface,dc->a.backMode,
				d.left+dc->a.viewPort.left,d.top+dc->a.viewPort.top,
				d.right+dc->a.viewPort.left,d.bottom+dc->a.viewPort.top);
			}
		dc->r.SetMix(oldop);
		dc->r.SetForeColor(oldColor);
		dc->r.SetBackColor(oldBackColor);
		RESTORE_HARDWARE(dc,false);
		}
	else {
		dstLeft = d.left;			dstTop = d.top;
		MAKE_HARDWARE_CURRENT(dc,true);
		if (NEED_TRANSLATE_BM(bitmap,dc)) {
			/* Translate the pixel information when doing the Blt */
			if (dc->a.clipRegion) {
				d.left = dstLeft + dc->a.viewPort.left;
				d.top = dstTop + dc->a.viewPort.top;
				d.right = d.left + (right-left);
				d.bottom = d.top + (bottom-top);
				BEGIN_CLIP_REGION(clip,dc->intClipRegion);
					if (MGL_sectRect(clip,d,&r)) {
						srcLeft = left + (r.left - d.left);
						srcRight = srcLeft + (r.right - r.left);
						srcTop = top + (r.top - d.top);
						srcBottom = srcTop + (r.bottom - r.top);
						dc->r.ConvertBltSys(bitmap->surface,bitmap->bytesPerLine,
							srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
							r.left,r.top,bitmap->bitsPerPixel,GAPF(bitmap->pf),
							GAPAL(dc->colorTab),GAPAL(bitmap->pal),dc->a.ditherMode,op,false);
						}
				END_CLIP_REGION;
				}
			else {
				dc->r.ConvertBltSys(bitmap->surface,bitmap->bytesPerLine,
					left,top,right-left,bottom-top,
					dstLeft + dc->a.viewPort.left,dstTop + dc->a.viewPort.top,
					bitmap->bitsPerPixel,GAPF(bitmap->pf),GAPAL(dc->colorTab),
					GAPAL(bitmap->pal),dc->a.ditherMode,op,false);
				}
			}
		else {
			if (dc->a.clipRegion) {
				d.left = dstLeft + dc->a.viewPort.left;
				d.top = dstTop + dc->a.viewPort.top;
				d.right = d.left + (right-left);
				d.bottom = d.top + (bottom-top);
				BEGIN_CLIP_REGION(clip,dc->intClipRegion);
					if (MGL_sectRect(clip,d,&r)) {
						srcLeft = left + (r.left - d.left);
						srcRight = srcLeft + (r.right - r.left);
						srcTop = top + (r.top - d.top);
						srcBottom = srcTop + (r.bottom - r.top);
						dc->r.BitBltSys(bitmap->surface,bitmap->bytesPerLine,
							srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
							r.left,r.top,op,false);
						}
				END_CLIP_REGION;
				}
			else {
				dc->r.BitBltSys(bitmap->surface,bitmap->bytesPerLine,
					left,top,right-left,bottom-top,
					dstLeft + dc->a.viewPort.left,dstTop + dc->a.viewPort.top,op,false);
				}
			}
		RESTORE_HARDWARE(dc,true);
		}
}

/****************************************************************************
DESCRIPTION:
Draw a lightweight bitmap mask in the specified color.

HEADER:
mgraph.h

PARAMETERS:
dc		- Device context to display bitmap on
x		- x coordinate to draw bitmap at
y		- y coordinate to draw bitmap at
mask	- Monochrome bitmap mask to display
color	- Color to draw in
op		- Write mode to use when drawing bitmap

REMARKS:
Draws a lightweight monochrome bitmap at the specified location. This is
just a simply utility function that draws the monochrome bitmap in a specified
color and write mode as fast as possible.

SEE ALSO:
MGL_loadBitmap
****************************************************************************/
void MGLAPI MGL_putBitmapMask(
	MGLDC *dc,
	int x,
	int y,
	const bitmap_t *mask,
	color_t color,
	int op)
{
	rect_t	d,r,clip;
	int		oldop = dc->a.writeMode;
	color_t	oldColor = dc->a.color;
	color_t	oldBackColor = dc->a.backColor;

	/* Clip to destination device context */
	if (mask->bitsPerPixel != 1)
		return;
	if (dc == _MGL_dcPtr)
		dc = &DC;
	d.left = x;						d.top = y;
	d.right = x + mask->width;	d.bottom = y + mask->height;
	if (!MGL_sectRect(dc->a.clipRect,d,&d))
		return;

	/* Adjust to bitmap coordinates */
	MGL_offsetRect(d,-x,-y);
	x += d.left;
	y += d.top;
	MAKE_HARDWARE_CURRENT(dc,false);
	dc->r.SetMix(op);
	dc->r.SetForeColor(color);
	dc->r.SetBackColor(0);
	if (dc->a.clipRegion) {
		/* Draw it clipped to a complex clip region */
		x += dc->a.viewPort.left;
		y += dc->a.viewPort.top;
		d.left = x;					d.top = y;
		d.right = x + mask->width;	d.bottom = y + mask->height;
		BEGIN_CLIP_REGION(clip,dc->intClipRegion);
			if (MGL_sectRect(clip,d,&r)) {
				if (!MGL_equalRect(d,r)) {
					/* Draw it clipped to the clip rectangle */
					dc->r.ClipMonoImage(x,y,mask->width,mask->height,
						mask->bytesPerLine,mask->surface,dc->a.backMode,
                        r.left,r.top,
                        r.right,r.bottom);
					}
				else {
					/* Draw it unclipped for maximum speed */
					dc->r.PutMonoImage(x,y,mask->width,mask->height,
						mask->bytesPerLine,mask->surface,dc->a.backMode);
					}
				}
		END_CLIP_REGION;
		}
	else if (!MGL_equalRect(d,r)) {
		/* Draw it clipped to the clip rectangle */
		dc->r.ClipMonoImage(x+dc->a.viewPort.left,y+dc->a.viewPort.top,
			mask->width,mask->height,mask->bytesPerLine,
			mask->surface,dc->a.backMode,
			dc->intClipRect.left,dc->intClipRect.top,
			dc->intClipRect.right,dc->intClipRect.bottom);
		}
	else {
		/* Draw it unclipped for maximum speed */
		dc->r.PutMonoImage(x+dc->a.viewPort.left,y+dc->a.viewPort.top,
			mask->width,mask->height,mask->bytesPerLine,mask->surface,dc->a.backMode);
		}
	dc->r.SetMix(oldop);
	dc->r.SetForeColor(oldColor);
	dc->r.SetForeColor(oldBackColor);
	RESTORE_HARDWARE(dc,false);
}

/****************************************************************************
DESCRIPTION:
Draw a transparent lightweight bitmap at the specified location with source
transparency.

HEADER:
mgraph.h

PARAMETERS:
dc			- Device context to display bitmap on
x			- x coordinate to draw bitmap at
y			- y coordinate to draw bitmap at
bitmap		- Bitmap to display
transparent	- Transparent color for the bitmap
op			- Write mode to use when drawing bitmap

REMARKS:
Draws a transparent lightweight bitmap at the specified location with either
source or destination transparency. When transferring the data with source
transparency, for pixels in the source image that are equal to the specified
transparent color, the related pixel in the destination buffer will remain
untouched. This allows you to quickly transfer sprites between device
contexts with a single color being allocated as a transparent color.

Note: 	The pixel depth and pixel format for the source bitmap and the
		device contexts must be the same or this routine will simply do
		nothing. This routine also only works with pixel depths that are
		at least 8 bits deep.

SEE ALSO:
MGL_loadBitmap, MGL_putBitmapSrcTransSection, MGL_putBitmapDstTransSection
****************************************************************************/
void MGLAPI MGL_putBitmapSrcTrans(
	MGLDC *dc,
	int x,
	int y,
	const bitmap_t *bitmap,
	color_t transparent,
	int op)
{
	rect_t	d,r,clip;
	int 	left,top,right,bottom;
	int 	srcLeft,srcTop,srcRight,srcBottom;

	/* Clip to destination device context */
	if (dc == _MGL_dcPtr)
		dc = &DC;
	if (bitmap->bitsPerPixel < 8 || (bitmap->bitsPerPixel != dc->mi.bitsPerPixel)) {
		SETERROR(grInvalidDevice);
		return;
		}
	d.left = x;						d.top = y;
	d.right = x + bitmap->width;	d.bottom = y + bitmap->height;
	if (!MGL_sectRect(dc->a.clipRect,d,&d))
		return;

	/* Adjust to bitmap coordinates */
	MGL_offsetRect(d,-x,-y);
	x += d.left;
	y += d.top;
	MAKE_HARDWARE_CURRENT(dc,true);
	if (dc->a.clipRegion) {
		left = d.left;
		top = d.top;
		right = d.right;
		bottom = d.bottom;
		d.left = x + dc->a.viewPort.left;
		d.top = y + dc->a.viewPort.top;
		d.right = d.left + (right-left);
		d.bottom = d.top + (bottom-top);
		BEGIN_CLIP_REGION(clip,dc->intClipRegion);
			if (MGL_sectRect(clip,d,&r)) {
				srcLeft = left + (r.left - d.left);
				srcRight = srcLeft + (r.right - r.left);
				srcTop = top + (r.top - d.top);
				srcBottom = srcTop + (r.bottom - r.top);
				dc->r.SrcTransBltSys(bitmap->surface,bitmap->bytesPerLine,
					srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
					r.left,r.top,op,transparent,false);
				}
		END_CLIP_REGION;
		}
	else {
		dc->r.SrcTransBltSys(bitmap->surface,bitmap->bytesPerLine,
			d.left,d.top,d.right-d.left,d.bottom-d.top,
			x + dc->a.viewPort.left,y + dc->a.viewPort.top,
			op,transparent,false);
		}
	RESTORE_HARDWARE(dc,true);
}

/****************************************************************************
DESCRIPTION:
Draw a transparent lightweight bitmap at the specified location with
destination transparency.

HEADER:
mgraph.h

PARAMETERS:
dc			- Device context to display bitmap on
x			- x coordinate to draw bitmap at
y			- y coordinate to draw bitmap at
bitmap		- Bitmap to display
transparent	- Transparent color for the bitmap
op			- Write mode to use when drawing bitmap

REMARKS:
Draws a transparent lightweight bitmap at the specified location with either
source or destination transparency. When transferring the data with
destination transparency, pixels in the destination image that are equal to
the specified transparent color will be updated, and those pixels that are
not the same will be skipped. This is effectively the operation performed
for 'blueScreen'ing or color keying and can also be used for drawing
transparent sprites. Note however that destination transparency is very
slow in software compared to source transparency!

Note: 	The pixel depth and pixel format for the source bitmap and the
		device contexts must be the same or this routine will simply do
		nothing. This routine also only works with pixel depths that are
		at least 8 bits deep.

SEE ALSO:
MGL_loadBitmap, MGL_putBitmapSrcTransSection, MGL_putBitmapDstTransSection
****************************************************************************/
void MGLAPI MGL_putBitmapDstTrans(
	MGLDC *dc,
	int x,
	int y,
	const bitmap_t *bitmap,
	color_t transparent,
	int op)
{
	rect_t	d,r,clip;
	int 	left,top,right,bottom;
	int 	srcLeft,srcTop,srcRight,srcBottom;

	/* Clip to destination device context */
	if (dc == _MGL_dcPtr)
		dc = &DC;
	if (bitmap->bitsPerPixel < 8 || (bitmap->bitsPerPixel != dc->mi.bitsPerPixel)) {
		SETERROR(grInvalidDevice);
		return;
		}
	d.left = x;						d.top = y;
	d.right = x + bitmap->width;	d.bottom = y + bitmap->height;
	if (!MGL_sectRect(dc->a.clipRect,d,&d))
		return;

	/* Adjust to bitmap coordinates */
	MGL_offsetRect(d,-x,-y);
	x += d.left;
	y += d.top;
	MAKE_HARDWARE_CURRENT(dc,true);
	if (dc->a.clipRegion) {
		left = d.left;
		top = d.top;
		right = d.right;
		bottom = d.bottom;
		d.left = x + dc->a.viewPort.left;
		d.top = y + dc->a.viewPort.top;
		d.right = d.left + (right-left);
		d.bottom = d.top + (bottom-top);
		BEGIN_CLIP_REGION(clip,dc->intClipRegion);
			if (MGL_sectRect(clip,d,&r)) {
				srcLeft = left + (r.left - d.left);
				srcRight = srcLeft + (r.right - r.left);
				srcTop = top + (r.top - d.top);
				srcBottom = srcTop + (r.bottom - r.top);
				dc->r.DstTransBltSys(bitmap->surface,bitmap->bytesPerLine,
					srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
					r.left,r.top,op,transparent,false);
				}
		END_CLIP_REGION;
		}
	else {
		dc->r.DstTransBltSys(bitmap->surface,bitmap->bytesPerLine,
			d.left,d.top,d.right-d.left,d.bottom-d.top,
			x + dc->a.viewPort.left,y + dc->a.viewPort.top,
			op,transparent,false);
		}
	RESTORE_HARDWARE(dc,true);
}

/****************************************************************************
DESCRIPTION:
Draw a section of a transparent lightweight bitmap at the specified location
with source transparency.

HEADER:
mgraph.h

PARAMETERS:
dc			- Device context to display bitmap on
left		- Left coordinate of section to draw
top			- Top coordinate of section to draw
right		- Right coordinate of section to draw
bottom		- Bottom coordinate of section to draw
dstLeft		- Left coordinate of destination of bitmap section
dstTop		- Right coordinate for destination of bitmap section
bitmap		- Bitmap to display
transparent	- Transparent color for the bitmap
op			- Write mode to use when drawing bitmap

REMARKS:
Draws a section of a transparent lightweight bitmap at the specified location
with either source or destination transparency. When transferring the data
with source transparency, pixels in the source image that are equal to the
specified transparent color, the related pixel in the destination buffer
will remain untouched. This allows you to quickly transfer sprites between
device contexts with a single color being allocated as a transparent color.

Note: 	The pixel depth and pixel format for the source bitmap and the
		device contexts must be the same or this routine will simply do
		nothing. This routine also only works with pixel depths that are
		at least 8 bits deep.

SEE ALSO:
MGL_loadBitmap, MGL_putBitmapSrcTrans, MGL_putBitmapDstTrans
****************************************************************************/
void MGLAPI MGL_putBitmapSrcTransSection(
	MGLDC *dc,
	int left,
	int top,
	int right,
	int bottom,
	int dstLeft,
	int dstTop,
	const bitmap_t *bitmap,
	color_t transparent,
	int op)
{
	rect_t	d,r,clip;
	int 	srcLeft,srcTop,srcRight,srcBottom;

	/* Check for compatible bitmaps */
	if (bitmap->bitsPerPixel < 8 || (bitmap->bitsPerPixel != dc->mi.bitsPerPixel)) {
		SETERROR(grInvalidDevice);
		return;
		}

	/* Clip the source rectangle to the bitmap dimensions */
	if (dc == _MGL_dcPtr)
		dc = &DC;
	if (!MGL_sectRectCoord(0,0,bitmap->width,bitmap->height,left,top,right,bottom,&d))
		return;
	dstLeft += (d.left - left);	dstTop += (d.top - top);
	left = d.left;				right = d.right;
	top = d.top;				bottom = d.bottom;

	/* Clip to destination device context */
	d.left = dstLeft;					d.top = dstTop;
	d.right = dstLeft + (right-left);	d.bottom = dstTop + (bottom-top);
	if (!MGL_sectRect(dc->a.clipRect,d,&d))
		return;
	left += (d.left - dstLeft);	right = left + (d.right - d.left);
	top += (d.top - dstTop);	bottom = top + (d.bottom - d.top);
	dstLeft = d.left;			dstTop = d.top;

	/* Perform the blit of the bitmap section */
	MAKE_HARDWARE_CURRENT(dc,true);
	if (dc->a.clipRegion) {
		d.left = dstLeft + dc->a.viewPort.left;
		d.top = dstTop + dc->a.viewPort.top;
		d.right = d.left + (right-left);
		d.bottom = d.top + (bottom-top);
		BEGIN_CLIP_REGION(clip,dc->intClipRegion);
			if (MGL_sectRect(clip,d,&r)) {
				srcLeft = left + (r.left - d.left);
				srcRight = srcLeft + (r.right - r.left);
				srcTop = top + (r.top - d.top);
				srcBottom = srcTop + (r.bottom - r.top);
				dc->r.SrcTransBltSys(bitmap->surface,bitmap->bytesPerLine,
					srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
					r.left,r.top,op,transparent,false);
				}
		END_CLIP_REGION;
		}
	else {
		dc->r.SrcTransBltSys(bitmap->surface,bitmap->bytesPerLine,
			left,top,right-left,bottom-top,
			dstLeft + dc->a.viewPort.left,dstTop + dc->a.viewPort.top,
			op,transparent,false);
		}
	RESTORE_HARDWARE(dc,true);
}

/****************************************************************************
DESCRIPTION:
Draw a section of a transparent lightweight bitmap at the specified location
with destination transparency.

HEADER:
mgraph.h

PARAMETERS:
dc			- Device context to display bitmap on
left		- Left coordinate of section to draw
top			- Top coordinate of section to draw
right		- Right coordinate of section to draw
bottom		- Bottom coordinate of section to draw
dstLeft		- Left coordinate of destination of bitmap section
dstTop		- Right coordinate for destination of bitmap section
bitmap		- Bitmap to display
transparent	- Transparent color for the bitmap
op			- Write mode to use when drawing bitmap

REMARKS:
Draws a section of a transparent lightweight bitmap at the specified location
with either source or destination transparency. When transferring the data
with destination transparency, pixels in the destination image that are equal
to the specified transparent color will be updated, and those pixels that
are not the same will be skipped. This is effectively the operation performed
for 'blueScreen'ing or color keying and can also be used for drawing
transparent sprites. Note however that destination transparency is very slow
in software compared to source transparency!

Note: 	The pixel depth and pixel format for the source bitmap and the
		device contexts must be the same or this routine will simply do
		nothing. This routine also only works with pixel depths that are
		at least 8 bits deep.

SEE ALSO:
MGL_loadBitmap, MGL_putBitmapSrcTrans, MGL_putBitmapDstTrans
****************************************************************************/
void MGLAPI MGL_putBitmapDstTransSection(
	MGLDC *dc,
	int left,
	int top,
	int right,
	int bottom,
	int dstLeft,
	int dstTop,
	const bitmap_t *bitmap,
	color_t transparent,
	int op)
{
	rect_t	d,r,clip;
	int 	srcLeft,srcTop,srcRight,srcBottom;

	/* Check for compatible bitmaps */
	if (bitmap->bitsPerPixel < 8 || (bitmap->bitsPerPixel != dc->mi.bitsPerPixel)) {
		SETERROR(grInvalidDevice);
		return;
		}

	/* Clip the source rectangle to the bitmap dimensions */
	if (dc == _MGL_dcPtr)
		dc = &DC;
	if (!MGL_sectRectCoord(0,0,bitmap->width,bitmap->height,left,top,right,bottom,&d))
		return;
	dstLeft += (d.left - left);	dstTop += (d.top - top);
	left = d.left;				right = d.right;
	top = d.top;				bottom = d.bottom;

	/* Clip to destination device context */
	d.left = dstLeft;					d.top = dstTop;
	d.right = dstLeft + (right-left);	d.bottom = dstTop + (bottom-top);
	if (!MGL_sectRect(dc->a.clipRect,d,&d))
		return;
	left += (d.left - dstLeft);	right = left + (d.right - d.left);
	top += (d.top - dstTop);	bottom = top + (d.bottom - d.top);
	dstLeft = d.left;			dstTop = d.top;

	/* Perform the blit of the bitmap section */
	MAKE_HARDWARE_CURRENT(dc,true);
	if (dc->a.clipRegion) {
		d.left = dstLeft + dc->a.viewPort.left;
		d.top = dstTop + dc->a.viewPort.top;
		d.right = d.left + (right-left);
		d.bottom = d.top + (bottom-top);
		BEGIN_CLIP_REGION(clip,dc->intClipRegion);
			if (MGL_sectRect(clip,d,&r)) {
				srcLeft = left + (r.left - d.left);
				srcRight = srcLeft + (r.right - r.left);
				srcTop = top + (r.top - d.top);
				srcBottom = srcTop + (r.bottom - r.top);
				dc->r.DstTransBltSys(bitmap->surface,bitmap->bytesPerLine,
					srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
					r.left,r.top,op,transparent,false);
				}
		END_CLIP_REGION;
		}
	else {
		dc->r.DstTransBltSys(bitmap->surface,bitmap->bytesPerLine,
			left,top,right-left,bottom-top,
			dstLeft + dc->a.viewPort.left,dstTop + dc->a.viewPort.top,
			op,transparent,false);
		}
	RESTORE_HARDWARE(dc,true);
}

/****************************************************************************
DESCRIPTION:
Stretches a lightweight bitmap to the specified rectangle.

HEADER:
mgraph.h

PARAMETERS:
dc			- Device context to display bitmap on
dstLeft		- Left coordinate to stretch bitmap to
dstTop		- Top coordinate to stretch bitmap to
dstRight	- Right coordinate to stretch bitmap to
dstBottom	- Bottom coordinate to stretch bitmap to
bitmap		- Bitmap to display
op			- Write mode to use when drawing bitmap

REMARKS:
Stretches a lightweight bitmap to the specified destination rectangle on the
device context. The bitmap can be in any color format, and will be
translated as necessary to the color format required by the current device
context.

Note that for maximum performance when displaying 8 bit bitmaps, you should
ensure that the color palette for the bitmap is identical to the device
context, otherwise the pixels in the bitmap will be translated during the
draw operation.

SEE ALSO:
MGL_loadBitmap, MGL_stretchBitmapSection
****************************************************************************/
void MGLAPI MGL_stretchBitmap(
	MGLDC *dc,
	int dstLeft,
	int dstTop,
	int dstRight,
	int dstBottom,
	const bitmap_t *bitmap,
	int op)
{
	rect_t	d,r,clip;
	int		left = 0,top = 0,right = bitmap->width,bottom = bitmap->height;
	ibool   clipIt = false;

	/* Handle special case of 1:1 stretch */
	if (((right - left) == (dstRight - dstLeft)) &&
		((bottom - top) == (dstBottom - dstTop))) {
		MGL_putBitmap(dc,dstLeft,dstTop,bitmap,op);
		return;
		}
	if (dc == _MGL_dcPtr)
		dc = &DC;
	if (bitmap->bitsPerPixel < 8) {
		SETERROR(grInvalidDevice);
		return;
		}

	/* Clip to destination device context */
	d.left = dstLeft;
	d.top = dstTop;
	d.right = dstRight;
	d.bottom = dstBottom;
	if (!MGL_sectRect(dc->a.clipRect,d,&d))
		return;
	if ((dc->a.clipRegion) || (d.left != dstLeft) || (d.right  != dstRight)
			|| (d.top != dstTop) || (d.bottom != dstBottom))
		clipIt = true;

	/* Perform the blit operation */
	MAKE_HARDWARE_CURRENT(dc,true);
	if (NEED_TRANSLATE_BM(bitmap,dc)) {
		/* Translate the pixel information when doing the stretch Blt */
		if (dc->a.clipRegion) {
			d.left = dstLeft + dc->a.viewPort.left;
			d.top = dstTop + dc->a.viewPort.top;
			d.right = dstRight + dc->a.viewPort.left;
			d.bottom = dstBottom + dc->a.viewPort.top;
			BEGIN_CLIP_REGION(clip,dc->intClipRegion);
				if (MGL_sectRect(clip,d,&r)) {
					dc->r.StretchConvertBltSys(bitmap->surface,bitmap->bytesPerLine,
						left,top,right-left,bottom-top,
						d.left,d.top,d.right-d.left,d.bottom-d.top,clipIt,
						r.left,r.top,r.right,r.bottom,
						bitmap->bitsPerPixel,GAPF(bitmap->pf),GAPAL(dc->colorTab),
						GAPAL(bitmap->pal),dc->a.ditherMode,op,false);
					}
			END_CLIP_REGION;
			}
		else {
			dc->r.StretchConvertBltSys(bitmap->surface,bitmap->bytesPerLine,
				left,top,right-left,bottom-top,
				dstLeft + dc->a.viewPort.left,dstTop + dc->a.viewPort.top,
				dstRight-dstLeft,dstBottom-dstTop,clipIt,
				dc->intClipRect.left,dc->intClipRect.top,
				dc->intClipRect.right,dc->intClipRect.bottom,
				bitmap->bitsPerPixel,GAPF(bitmap->pf),GAPAL(dc->colorTab),
				GAPAL(bitmap->pal),dc->a.ditherMode,op,false);
			}
		}
	else {
		/* Perform the stretch blit with no pixel format conversion */
		if (dc->a.clipRegion) {
			d.left = dstLeft + dc->a.viewPort.left;
			d.top = dstTop + dc->a.viewPort.top;
			d.right = dstRight + dc->a.viewPort.left;
			d.bottom = dstBottom + dc->a.viewPort.top;
			BEGIN_CLIP_REGION(clip,dc->intClipRegion);
				if (MGL_sectRect(clip,d,&r)) {
					dc->r.StretchBltSys(bitmap->surface,bitmap->bytesPerLine,
						left,top,right-left,bottom-top,
						d.left,d.top,d.right-d.left,d.bottom-d.top,clipIt,
						r.left,r.top,r.right,r.bottom,op,false);
					}
			END_CLIP_REGION;
			}
		else {
			dc->r.StretchBltSys(bitmap->surface,bitmap->bytesPerLine,
				left,top,right-left,bottom-top,
				dstLeft + dc->a.viewPort.left,dstTop + dc->a.viewPort.top,
				dstRight-dstLeft,dstBottom-dstTop,clipIt,
				dc->intClipRect.left,dc->intClipRect.top,
				dc->intClipRect.right,dc->intClipRect.bottom,
				op,false);
			}
		}
	RESTORE_HARDWARE(dc,true);
}

/****************************************************************************
DESCRIPTION:
Stretches a section of a lightweight bitmap to the specified device context.

HEADER:
mgraph.h

PARAMETERS:
dc			- Device context to display bitmap on
left		- Left coordinate of section to stretch
top			- Top coordinate of section to stretch
right		- Right coordinate of section to stretch
bottom		- Bottom coordinate of section to stretch
dstLeft		- Left coordinate to stretch bitmap to
dstTop		- Top coordinate to stretch bitmap to
dstRight	- Right coordinate to stretch bitmap to
dstBottom	- Bottom coordinate to stretch bitmap to
bitmap		- Bitmap to display
op			- Write mode to use when drawing bitmap

REMARKS:
Stretches a section of a lightweight bitmap to the specified destination
rectangle on the device context. The bitmap can be in any color format, and
will be translated as necessary to the color format required by the current
device context.

For maximum performance when displaying 8 bit bitmaps, you should ensure
that the color palette for the bitmap is identical to the device context,
otherwise the pixels in the bitmap will be translated during the draw
operation.

SEE ALSO:
MGL_loadBitmap, MGL_stretchBitmap
****************************************************************************/
void MGLAPI MGL_stretchBitmapSection(
	MGLDC *dc,
	int left,
	int top,
	int right, 
	int bottom,
	int dstLeft,
	int dstTop,
	int dstRight,
	int dstBottom,
	const bitmap_t *bitmap,
	int op)
{
	rect_t	d,r,clip;
	int		deltaSrc,deltaDst;
	fix32_t	zoomx,zoomy;
	ibool   clipIt = false;
	int 	fdTop,fdLeft;

	/* Handle special case of 1:1 stretch */
	if (dc == _MGL_dcPtr)
		dc = &DC;
	if (bitmap->bitsPerPixel < 8) {
		SETERROR(grInvalidDevice);
		return;
		}

	/* Calculate the x zoom factor */
	deltaSrc = right - left;
	deltaDst = dstRight - dstLeft;
	if (deltaDst == deltaSrc)
		zoomx = MGL_FIX_1;
	else if (deltaDst == (deltaSrc * 2))
		zoomx = MGL_FIX_2;
	else
		zoomx = MGL_FixDiv(MGL_TOFIX(deltaDst),MGL_TOFIX(deltaSrc));

	/* Calculate the y zoom factor */
	deltaSrc = bottom - top;
	deltaDst = dstBottom - dstTop;
	if (deltaDst == deltaSrc)
		zoomy = MGL_FIX_1;
	else if (deltaDst == (deltaSrc * 2))
		zoomy = MGL_FIX_2;
	else
		zoomy = MGL_FixDiv(MGL_TOFIX(deltaDst),MGL_TOFIX(deltaSrc));

	/* Handle special case of 1:1 stretch */
	if (zoomx == MGL_FIX_1 && zoomy == MGL_FIX_1) {
		MGL_putBitmapSection(dc,left,top,right,bottom,dstLeft,dstTop,bitmap,op);
		return;
		}

	/* Clip the source rectangle to the bitmap dimensions */
	d.left = left;	d.top = top;
	d.right = MIN(right, bitmap->width);
	d.bottom = MIN(bottom, bitmap->height);
	if (MGL_emptyRect(d))
		return;
	fdLeft = MGL_TOFIX(dstLeft) + ((d.left - left) * zoomx);
	fdTop = MGL_TOFIX(dstTop) + ((d.top - top) * zoomy);
	left = d.left;				right = d.right;
	top = d.top;				bottom = d.bottom;
	dstLeft = MGL_FIXROUND(fdLeft);
	dstTop = MGL_FIXROUND(fdTop);
	dstRight = dstLeft+MGL_FIXTOINT((right-left)*zoomx);
	dstBottom = dstTop+MGL_FIXTOINT((bottom-top)*zoomy);

	/* Clip to destination device context */
	d.left = dstLeft;			d.top = dstTop;
	d.right = dstRight;			d.bottom = dstBottom;
	if (!MGL_sectRect(dc->a.clipRect,d,&d))
		return;
	if ((dc->a.clipRegion) || (d.left != dstLeft) || (d.right  != dstRight)
			|| (d.top != dstTop) || (d.bottom != dstBottom))
		clipIt = true;

	/* Perform the blit operation */
	MAKE_HARDWARE_CURRENT(dc,true);
	if (NEED_TRANSLATE_BM(bitmap,dc)) {
		/* Translate the pixel information when doing the stretch Blt */
		if (dc->a.clipRegion) {
			d.left = dstLeft + dc->a.viewPort.left;
			d.top = dstTop + dc->a.viewPort.top;
			d.right = dstRight + dc->a.viewPort.left;
			d.bottom = dstBottom + dc->a.viewPort.top;
			BEGIN_CLIP_REGION(clip,dc->intClipRegion);
				if (MGL_sectRect(clip,d,&r)) {
					dc->r.StretchConvertBltSys(bitmap->surface,bitmap->bytesPerLine,
						left,top,right-left,bottom-top,
						d.left,d.top,d.right-d.left,d.bottom-d.top,clipIt,
						r.left,r.top,r.right,r.bottom,
						bitmap->bitsPerPixel,GAPF(bitmap->pf),GAPAL(dc->colorTab),
						GAPAL(bitmap->pal),dc->a.ditherMode,op,false);
					}
			END_CLIP_REGION;
			}
		else {
			dc->r.StretchConvertBltSys(bitmap->surface,bitmap->bytesPerLine,
				left,top,right-left,bottom-top,
				dstLeft + dc->a.viewPort.left,dstTop + dc->a.viewPort.top,
				dstRight-dstLeft,dstBottom-dstTop,clipIt,
				dc->intClipRect.left,dc->intClipRect.top,
				dc->intClipRect.right,dc->intClipRect.bottom,
				bitmap->bitsPerPixel,GAPF(bitmap->pf),GAPAL(dc->colorTab),
				GAPAL(bitmap->pal),dc->a.ditherMode,op,false);
			}
		}
	else {
		/* Perform the stretch blit with no pixel format conversion */
		if (dc->a.clipRegion) {
			d.left = dstLeft + dc->a.viewPort.left;
			d.top = dstTop + dc->a.viewPort.top;
			d.right = dstRight + dc->a.viewPort.left;
			d.bottom = dstBottom + dc->a.viewPort.top;
			BEGIN_CLIP_REGION(clip,dc->intClipRegion);
				if (MGL_sectRect(clip,d,&r)) {
					dc->r.StretchBltSys(bitmap->surface,bitmap->bytesPerLine,
						left,top,right-left,bottom-top,
						d.left,d.top,d.right-d.left,d.bottom-d.top,clipIt,
						r.left,r.top,r.right,r.bottom,op,false);
					}
			END_CLIP_REGION;
			}
		else {
			dc->r.StretchBltSys(bitmap->surface,bitmap->bytesPerLine,
				left,top,right-left,bottom-top,
				dstLeft + dc->a.viewPort.left,dstTop + dc->a.viewPort.top,
				dstRight-dstLeft,dstBottom-dstTop,clipIt,
				dc->intClipRect.left,dc->intClipRect.top,
				dc->intClipRect.right,dc->intClipRect.bottom,
				op,false);
			}
		}
	RESTORE_HARDWARE(dc,true);
}

/****************************************************************************
DESCRIPTION:
Draw an icon at the specified location.

HEADER:
mgraph.h

PARAMETERS:
dc	- Device context to draw icon on
x	- x coordinate to draw icon at
y	- y coordinate to draw icon at
icon	- Icon to display

REMARKS:
Draws an icon at the specified location on the current device context. The icon may
be in any color format, and will be translated as necessary to the color format of the
display device context. The icon is drawn by punching a black hole in the
background with the icon mask, and then OR'ing in the image bitmap for the icon.

SEE ALSO:
MGL_loadIcon
****************************************************************************/
void MGLAPI MGL_putIcon(
	MGLDC *dc,
	int x,
	int y,
	const icon_t *icon)
{
	int		oldop = dc->a.writeMode;
	color_t	oldColor = dc->a.color;
	int		oldBackMode = dc->a.backMode;

	/* Punch a hole in the display with the icon's AND mask */
	MAKE_HARDWARE_CURRENT(dc,false);
	dc->r.SetMix(MGL_REPLACE_MODE);
	dc->r.SetForeColor(0);
	dc->a.backMode = MGL_TRANSPARENT_BACKGROUND;
	MGL_putMonoImage(dc,x,y,icon->xorMask.width,icon->byteWidth,icon->xorMask.height,icon->andMask);
	dc->r.SetMix(oldop);
	dc->r.SetForeColor(oldColor);
	dc->a.backMode = oldBackMode;

	/* Now XOR in the bitmap for the ICON */
	MGL_putBitmap(dc,x,y,&icon->xorMask,MGL_XOR_MODE);
	RESTORE_HARDWARE(dc,false);
}
