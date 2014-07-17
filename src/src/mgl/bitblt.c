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
* Description:	Bit Block Transfer routines (BitBlt's).
*
****************************************************************************/

#include "mgl.h"

/*------------------------- Implementation --------------------------------*/

#undef	MGL_bitBlt
#undef	MGL_srcTransBlt
#undef	MGL_dstTransBlt
#undef	MGL_getDivot
#undef	MGL_divotSize

/****************************************************************************
DESCRIPTION:
Saves a divot of video memory into system RAM.

HEADER:
mgraph.h

PARAMETERS:
dc		- Device context to save divot from
left	- Left coordinate of area to save
top		- Top coordinate of area to save
right	- Right coordinate of area to save
bottom	- Bottom coordinate of area to save
divot	- Pointer to area to store the video memory in

REMARKS:
This function copies a block of video memory from the active page of the
current device context into a system RAM buffer. A divot is defined as being
a rectangular area of video memory that you wish to save, however the
bounding rectangle for the divot is expanded slightly to properly aligned
boundaries for the absolute maximum performance with the current device
context. This function is generally used to store the video memory behind
pull down menus and pop up dialog boxes, and the memory can only be restored
to exactly the same position that it was saved from.

You must pre-allocate enough space to hold the entire divot in system RAM.
Use the MGL_divotSize routine to determine the size of the memory block
required to store the divot.

SEE ALSO:
MGL_getDivot, MGL_putDivot, MGL_divotSize, MGL_malloc
****************************************************************************/
void MGLAPI MGL_getDivotCoord(
	MGLDC *dc,
	int left,
	int top,
	int right,
	int bottom,
	void *divot)
{
	rect_t		d;
	uchar		*p;
	divotHeader	hdr;

	/* Clip divot to destination device context */
	if (dc == _MGL_dcPtr)
		dc = &DC;
	d.left = left + dc->a.viewPort.left;
	d.top = top + dc->a.viewPort.top;
	d.right = right + dc->a.viewPort.left;
	d.bottom = bottom + dc->a.viewPort.top;
	if (!MGL_sectRect(dc->intClipRect,d,&d))
		return;

	/* Store header information */
	hdr.left = left;	hdr.top = top;
	hdr.right = right;	hdr.bottom = bottom;
	switch (dc->mi.bitsPerPixel) {
		case 4:
			hdr.bytesPerLine = ((right - left)+1)/2;
			break;
		case 8:
			hdr.bytesPerLine = (right - left);
			break;
		case 15:
		case 16:
			hdr.bytesPerLine = (right - left)*2;
			break;
		case 24:
			hdr.bytesPerLine = (right - left)*3;
			break;
		default: /* case 32: */
			hdr.bytesPerLine = (right - left)*4;
			break;
		}
	p = divot;
	memcpy(p,&hdr,sizeof(hdr));
	p += sizeof(hdr);
	MAKE_HARDWARE_CURRENT(dc,true);
	dc->r.GetBitmapSys(p,hdr.bytesPerLine,d.left,d.top,d.right-d.left,d.bottom-d.top,0,0);
	RESTORE_HARDWARE(dc,true);
}

/****************************************************************************
DESCRIPTION:
Saves a divot of video memory into system RAM.

HEADER:
mgraph.h

PARAMETERS:
dc		- Device context to save divot from
r		- Rectangle containing coordinates of divot to save
divot	- Pointer to area to store the video memory in

REMARKS:
This function is the same as MGL_getDivotCoord however it takes entire
rectangles as arguments instead of coordinates.

SEE ALSO:
MGL_putDivot, MGL_divotSize, MGL_malloc
****************************************************************************/
void MGL_getDivot(
	MGLDC dc,
	rect_t,
	void *divot);
/* Implemented as a macro */

/****************************************************************************
DESCRIPTION:
Replaces a divot of video memory to the location from which it was copied.

HEADER:
mgraph.h

PARAMETERS:
dc		- Device context to restore the divot to
divot	- Pointer to the divot to replace

REMARKS:
This function replaces a rectangle of video memory that was saved previously
with the MGL_getDivot function. The divot is replaced at the same location
that is was taken from on the current device context.

A divot is defined as being a rectangular area of video memory that you wish
to save, however the bounding rectangle for the divot is expanded slightly
to properly aligned boundaries for the absolute maximum performance with the
current device context. This function is generally used to store the video
memory behind pull down menus and pop up dialog boxes, and the memory can
only be restored to exactly the same position that it was saved from.

SEE ALSO:
MGL_divotSize
****************************************************************************/
void MGLAPI MGL_putDivot(
	MGLDC *dc,
	void *divot)
{
	uchar		*s;
	divotHeader	hdr;

	/* Extract header information */
	if (dc == _MGL_dcPtr)
		dc = &DC;
	s = divot;
	memcpy(&hdr,s,sizeof(hdr));
	s += sizeof(hdr);
	MAKE_HARDWARE_CURRENT(dc,true);
	dc->r.BitBltSys(s,hdr.bytesPerLine,0,0,hdr.right-hdr.left,hdr.bottom-hdr.top,hdr.left,hdr.top,GA_REPLACE_MIX,false);
	RESTORE_HARDWARE(dc,true);
}

/****************************************************************************
DESCRIPTION:
Number of bytes required to store a divot of specified size.

HEADER:
mgraph.h

PARAMETERS:
dc		- Device context to measure divot size from
left	- Left coordinate of divot area
top		- Top coordinate of divot area
right	- Right coordinate of divot area
bottom	- Bottom coordinate of divot area 

RETURNS:
Size of the specified divot in bytes.

REMARKS:
Determines the number of bytes required to store a divot of the specified
size taken from the current device context. A divot is a portion of video
memory that needs to be temporarily saved and restored, such as implementing
pull down menus and pop up dialog boxes. A divot must always be saved and
restored to the same area, and will extend the dimensions of the area
covered to obtain the maximum possible performance for saving and restoring
the memory.

SEE ALSO:
MGL_divotSize, MGL_getDivot, MGL_putDivot
****************************************************************************/
long MGLAPI MGL_divotSizeCoord(
	MGLDC *dc,
	int left,
	int top,
	int right,
	int bottom)
{
	int bytesPerLine;

	if (dc == _MGL_dcPtr)
		dc = &DC;
	switch (dc->mi.bitsPerPixel) {
		case 4:
			bytesPerLine = ((right - left)+1)/2;
			break;
		case 8:
			bytesPerLine = (right - left);
			break;
		case 15:
		case 16:
			bytesPerLine = (right - left)*2;
			break;
		case 24:
			bytesPerLine = (right - left)*3;
			break;
		default: /* case 32: */
			bytesPerLine = (right - left)*4;
			break;
		}
	return sizeof(divotHeader) + bytesPerLine * (bottom-top);
}

/****************************************************************************
DESCRIPTION:
Number of bytes required to store a divot of specified size.

HEADER:
mgraph.h

PARAMETERS:
dc	- Device context to measure divot size from
r	- Bounding rectangle of the divot area

RETURNS:
Size of the specified divot in bytes.

REMARKS:
This function is the same as MGL_divotSizeCoord however it takes entire
rectangles as arguments instead of coordinates.

SEE ALSO:
MGL_divotSizeCoord, MGL_getDivot, MGL_putDivot
****************************************************************************/
long MGL_divotSize(
	MGLDC *dc,
	rect_t r);
/* Implemented as a macro */

/****************************************************************************
DESCRIPTION:
Blts a block of image data from one device context into another.

HEADER:
mgraph.h

PARAMETERS:
dst		- Destination device context
src		- Source device context
left	- Left coordinate of image to Blt from
top		- Top coordinate of image to Blt from
right	- Right coordinate of image to Blt from
bottom	- Bottom coordinate of image to Blt from
dstLeft	- Left coordinate to Blt to
dstTop	- Right coordinate to Blt to
op		- Write mode to use during Blt

REMARKS:
Copies a block of bitmap data from one device context to another. The source
and destination rectangles may overlap even if the source and destination
device contexts are the same, and MGL will correctly handle the overlapping
regions. This routine has been highly optimized for absolute maximum performance,
so it will provide the fastest method of copying bitmap data between device
contexts. To obtain absolute maximum performance, you should align the source
and destination bitmaps on DWORD boundaries (4 pixels for 8 bit, 2 pixels for
15/16 bit) and the low level device driver code will special case this for
maximum performance.

This function will correctly handle Blt’s across device contexts with
differing pixel depths, and will perform the necessary pixel format translation
to convert from the source device to the destination device. Note that although
the code to implement this is highly optimized, this can be a time consuming
operation so you should attempt to pre-convert all bitmaps to the current
display device pixel format for maximum performance if using this routine for
sprite animation.

MGL does however have special case code to specifically handle translation of
24 bit RGB format bitmaps (the standard RGB DIB format used by Video for
Windows) to all 8 bit and above pixel formats. When converting from 24 bit to
8 bit, MGL will dither bitmaps in real time from 24 bit to the 8 bit halftone
palette. This provides a solid foundation to build real time 24 bit motion video
playback in all supported video modes in MGL.

Note that when MGL_bitBlt is called for 4 and 8 bit source bitmaps MGL first
checks if the color palettes for the source and destination bitmaps are the
same. If they are not, MGL translates the pixel values from the source bitmap
to the destination color palette, looking for the closest match color if an
exact match is not found. In order to obtain maximum performance for blt’ing
bitmaps in color index modes, you should ensure that the color palette in the
source device matches the color palette in the destination device, or you can
turn off all identity palette checking in MGL with the MGL_checkIdentityPalette
function.

This routine can also be used to perform hardware accelerated Blt’s between
offscreen memory devices and the display device when running in fullscreen
modes, providing the hardware accelerator (if present) can support this
operation.

The write mode operation specifies how the source image data should be combined
with the destination image data.  Write modes supported by the SciTech MGL are
enumerated in MGL_writeModeType.

The source and destination rectangles are clipped according to the current
clipping rectangles for the source and destination device contexts respectively.

The destination rectangle is clipped according to the current clipping
rectangles for the destination device context.

SEE ALSO:
MGL_bitBlt, MGL_stretchBlt, MGL_srcTransBlt, MGL_dstTransBlt
****************************************************************************/
void MGLAPI MGL_bitBltCoord(
	MGLDC *dst,
	MGLDC *src,
	int left,
	int top,
	int right,
	int bottom,
	int dstLeft,
	int dstTop,
	int op)
{
	rect_t	d,r,clip;
	int 	srcLeft,srcTop,srcRight,srcBottom;

	/* In order to clip the results, we first clip the source rectangle to
	 * the source device context, and then clip the destination rectangle to
	 * the destination device context.
	 */
	if (src == _MGL_dcPtr)
		src = &DC;
	if (dst == _MGL_dcPtr)
		dst = &DC;

	/* Clip to source rectangle */
	d.left = left;				d.top = top;
	d.right = right;			d.bottom = bottom;
	if (!MGL_sectRect(src->a.clipRect,d,&d))
		return;
	dstLeft += (d.left - left);	dstTop += (d.top - top);
	left = d.left;				right = d.right;
	top = d.top;				bottom = d.bottom;

	/* Clip to destination rectangle */
	d.left = dstLeft;
	d.top = dstTop;
	d.right = dstLeft + (right-left);
	d.bottom = dstTop + (bottom-top);
	if (!MGL_sectRect(dst->a.clipRect,d,&d))
		return;
	left += (d.left - dstLeft);	right = left + (d.right - d.left);
	top += (d.top - dstTop);	bottom = top + (d.bottom - d.top);

	/* Now perform the blit operation */
	if (src->mi.bitsPerPixel == 1) {
		int 		oldop = dst->a.writeMode;
		color_t 	oldColor = dst->a.color;
		color_t 	oldBackColor = dst->a.backColor;
		palette_t	*pal = TO_PAL(src->colorTab);

		MAKE_HARDWARE_CURRENT(dst,false);
		dst->r.SetMix(op);
		dst->r.SetForeColor(MGL_rgbColor(dst,pal[1].red,pal[1].green,pal[1].blue));
		dst->r.SetBackColor(MGL_rgbColor(dst,pal[0].red,pal[0].green,pal[0].blue));
		if (dst->a.clipRegion) {
			/* Draw it clipped to a complex clip region */
			dstLeft += dst->a.viewPort.left;
			dstTop += dst->a.viewPort.top;
			d.left = dstLeft + dst->a.viewPort.left;
			d.top = dstTop + dst->a.viewPort.top;
			d.right = d.left + (right-left);
			d.bottom = d.top + (bottom-top);
			BEGIN_CLIP_REGION(clip,dst->intClipRegion);
				if (MGL_sectRect(clip,d,&r)) {
					/* Draw it clipped to the clip rectangle */
					dst->r.ClipMonoImage(
						dstLeft,dstTop,src->mi.xRes+1,src->mi.yRes+1,
						src->mi.bytesPerLine,src->surface,dst->a.backMode,
						d.left,d.top,d.right,d.bottom);
					}
			END_CLIP_REGION;
			}
		else {
			/* Draw it clipped to the clip rectangle */
			dst->r.ClipMonoImage(
				dstLeft+dst->a.viewPort.left,dstTop+dst->a.viewPort.top,
				src->mi.xRes+1,src->mi.yRes+1,src->mi.bytesPerLine,
				src->surface,dst->a.backMode,
				d.left+dst->a.viewPort.left,d.top+dst->a.viewPort.top,
				d.right+dst->a.viewPort.left,d.bottom+dst->a.viewPort.top);
			}
		dst->r.SetMix(oldop);
		dst->r.SetForeColor(oldColor);
		dst->r.SetBackColor(oldBackColor);
		RESTORE_HARDWARE(dst,false);
		}
	else {
		dstLeft = d.left;	dstTop = d.top;
		MAKE_HARDWARE_CURRENT(dst,true);
		if (src == dst) {
			/* BitBlt between areas of the same device context */
			if (dst->a.clipRegion) {
				left += src->a.viewPort.left;
				top += src->a.viewPort.top;
				right += src->a.viewPort.left;
				bottom += src->a.viewPort.top;
				d.left = dstLeft + dst->a.viewPort.left;
				d.top = dstTop + dst->a.viewPort.top;
				d.right = d.left + (right-left);
				d.bottom = d.top + (bottom-top);
				BEGIN_CLIP_REGION(clip,dst->intClipRegion);
					if (MGL_sectRect(clip,d,&r)) {
						srcLeft = left + (r.left - d.left);
						srcRight = srcLeft + (r.right - r.left);
						srcTop = top + (r.top - d.top);
						srcBottom = srcTop + (r.bottom - r.top);
						dstLeft = r.left;
						dstTop = r.top;
						src->r.BitBlt(
							srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
							dstLeft,dstTop,op);
						}
				END_CLIP_REGION;
				}
			else {
				src->r.BitBlt(
					left + src->a.viewPort.left,top + src->a.viewPort.top,
					right - left,bottom - top,
					dstLeft + dst->a.viewPort.left,dstTop + dst->a.viewPort.top,op);
				}
			}
		else if (src->deviceType == MGL_MEMORY_DEVICE) {
			/* Copy from one DC to any other DC when one is in system memory */
			if (NEED_TRANSLATE_DC(src,dst)) {
				if (dst->a.clipRegion) {
					left += src->a.viewPort.left;
					top += src->a.viewPort.top;
					right += src->a.viewPort.left;
					bottom += src->a.viewPort.top;
					d.left = dstLeft + dst->a.viewPort.left;
					d.top = dstTop + dst->a.viewPort.top;
					d.right = d.left + (right-left);
					d.bottom = d.top + (bottom-top);
					srcLeft = left; srcTop = top;
					BEGIN_CLIP_REGION(clip,dst->intClipRegion);
						if (MGL_sectRect(clip,d,&r)) {
							srcLeft += (r.left - dstLeft);
							srcRight = srcLeft + (r.right - r.left);
							srcTop += (r.top - dstTop);
							srcBottom = srcTop + (r.bottom - r.top);
							dstLeft = r.left;
							dstTop = r.top;
							dst->r.ConvertBltSys(src->surface,src->mi.bytesPerLine,
								srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
								dstLeft,dstTop,src->mi.bitsPerPixel,GAPF(&src->pf),
								GAPAL(dst->colorTab),GAPAL(src->colorTab),dst->a.ditherMode,op,false);
							}
					END_CLIP_REGION;
					}
				else {
					dst->r.ConvertBltSys(src->surface,src->mi.bytesPerLine,
						left + src->a.viewPort.left,top + src->a.viewPort.top,
						right - left,bottom - top,
						dstLeft + dst->a.viewPort.left,dstTop + dst->a.viewPort.top,
						src->mi.bitsPerPixel,GAPF(&src->pf),GAPAL(dst->colorTab),
						GAPAL(src->colorTab),dst->a.ditherMode,op,false);
					}
				}
			else {
				if (dst->a.clipRegion) {
					left += src->a.viewPort.left;
					top += src->a.viewPort.top;
					right += src->a.viewPort.left;
					bottom += src->a.viewPort.top;
					d.left = dstLeft + dst->a.viewPort.left;
					d.top = dstTop + dst->a.viewPort.top;
					d.right = d.left + (right-left);
					d.bottom = d.top + (bottom-top);
					BEGIN_CLIP_REGION(clip,dst->intClipRegion);
						if (MGL_sectRect(clip,d,&r)) {
							srcLeft = left + (r.left - d.left);
							srcRight = srcLeft + (r.right - r.left);
							srcTop = top + (r.top - d.top);
							srcBottom = srcTop + (r.bottom - r.top);
							dst->r.BitBltSys(src->surface,src->mi.bytesPerLine,
								srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
								r.left,r.top,op,false);
							}
					END_CLIP_REGION;
					}
				else {
					dst->r.BitBltSys(src->surface,src->mi.bytesPerLine,
						left + src->a.viewPort.left,top + src->a.viewPort.top,
						right - left,bottom - top,
						dstLeft + dst->a.viewPort.left,dstTop + dst->a.viewPort.top,
						op,false);
					}
				}
			}
		else if (dst->deviceType == MGL_MEMORY_DEVICE) {
			/* Copy from one DC to any other DC when one is in system memory */
			if (NEED_TRANSLATE_DC(src,dst)) {
				/* TODO: We may need to deal with Convert blt reads from
				 *       the framebuffer differently.
				 */
				if (dst->a.clipRegion) {
					left += src->a.viewPort.left;
					top += src->a.viewPort.top;
					right += src->a.viewPort.left;
					bottom += src->a.viewPort.top;
					d.left = dstLeft + dst->a.viewPort.left;
					d.top = dstTop + dst->a.viewPort.top;
					d.right = d.left + (right-left);
					d.bottom = d.top + (bottom-top);
					srcLeft = left; srcTop = top;
					BEGIN_CLIP_REGION(clip,dst->intClipRegion);
						if (MGL_sectRect(clip,d,&r)) {
							srcLeft += (r.left - dstLeft);
							srcRight = srcLeft + (r.right - r.left);
							srcTop += (r.top - dstTop);
							srcBottom = srcTop + (r.bottom - r.top);
							dstLeft = r.left;
							dstTop = r.top;
							dst->r.ConvertBltSys(src->surface,src->mi.bytesPerLine,
								srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
								dstLeft,dstTop,src->mi.bitsPerPixel,GAPF(&src->pf),
								GAPAL(dst->colorTab),GAPAL(src->colorTab),dst->a.ditherMode,op,false);
							}
					END_CLIP_REGION;
					}
				else {
					dst->r.ConvertBltSys(src->surface,src->mi.bytesPerLine,
						left + src->a.viewPort.left,top + src->a.viewPort.top,
						right - left,bottom - top,
						dstLeft + dst->a.viewPort.left,dstTop + dst->a.viewPort.top,
						src->mi.bitsPerPixel,GAPF(&src->pf),GAPAL(dst->colorTab),
						GAPAL(src->colorTab),dst->a.ditherMode,op,false);
					}
				}
			else {
				/* TODO: We may need to handle ROP's in here! */
				if (dst->a.clipRegion) {
					left += src->a.viewPort.left;
					top += src->a.viewPort.top;
					right += src->a.viewPort.left;
					bottom += src->a.viewPort.top;
					d.left = dstLeft + dst->a.viewPort.left;
					d.top = dstTop + dst->a.viewPort.top;
					d.right = d.left + (right-left);
					d.bottom = d.top + (bottom-top);
					BEGIN_CLIP_REGION(clip,dst->intClipRegion);
						if (MGL_sectRect(clip,d,&r)) {
							srcLeft = left + (r.left - d.left);
							srcRight = srcLeft + (r.right - r.left);
							srcTop = top + (r.top - d.top);
							srcBottom = srcTop + (r.bottom - r.top);
							src->r.GetBitmapSys(dst->surface,dst->mi.bytesPerLine,
								srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
								r.left,r.top);
							}
					END_CLIP_REGION;
					}
				else {
					src->r.GetBitmapSys(dst->surface,dst->mi.bytesPerLine,
						left + src->a.viewPort.left,top + src->a.viewPort.top,
						right - left,bottom - top,
						dstLeft + dst->a.viewPort.left,dstTop + dst->a.viewPort.top);
					}
				}
			}
		else {
			/* BitBlt between two device contexts which may both be in
			 * hardware. We allow the MGL device driver determine how to
			 * handle the case, so that it can be done in hardware if
			 * available (such as blitting from offscreen DC or back buffers).
			 */
			if (dst->a.clipRegion) {
				left += src->a.viewPort.left;
				top += src->a.viewPort.top;
				right += src->a.viewPort.left;
				bottom += src->a.viewPort.top;
				d.left = dstLeft + dst->a.viewPort.left;
				d.top = dstTop + dst->a.viewPort.top;
				d.right = d.left + (right-left);
				d.bottom = d.top + (bottom-top);
				BEGIN_CLIP_REGION(clip,dst->intClipRegion);
					if (MGL_sectRect(clip,d,&r)) {
						srcLeft = left + (r.left - d.left);
						srcRight = srcLeft + (r.right - r.left);
						srcTop = top + (r.top - d.top);
						srcBottom = srcTop + (r.bottom - r.top);
						dstLeft = r.left;
						dstTop = r.top;
						src->r.BitBltDC(src,dst,
							srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
							dstLeft,dstTop,op);
						}
				END_CLIP_REGION;
				}
			else {
				src->r.BitBltDC(src,dst,
					left + src->a.viewPort.left,top + src->a.viewPort.top,
					right - left,bottom - top,
					dstLeft + dst->a.viewPort.left,dstTop + dst->a.viewPort.top,op);
				}
			}
		RESTORE_HARDWARE(dst,true);
		}
}

/****************************************************************************
DESCRIPTION:
Blts a block of image data from one device context into another.

HEADER:
mgraph.h

PARAMETERS:
dst		- Destination device context
src		- Source device context
r		- Rectangle defining are to Blt from
dstLeft	- Left coordinate to Blt to
dstTop	- Right coordinate to Blt to
op		- Write mode to use during Blt

REMARKS:
This function is the same as MGL_bitBltCoord, however it takes entire
rectangles as parameters instead of coordinates.

SEE ALSO:
MGL_bitBltCoord, MGL_stretchBlt, MGL_srcTransBlt, MGL_dstTransBlt
****************************************************************************/
void MGL_bitBlt(
	MGLDC *dst,
	MGLDC *src,
	rect_t r,
	int dstLeft,
	int dstTop,
	int op);
/* Implemented as a macro */

/****************************************************************************
DESCRIPTION:
Copies a block of image data with source transparency.

HEADER:
mgraph.h

PARAMETERS:
dst			- Destination device context
src			- Source device context
left		- Left coordinate of source image
top			- Top coordinate of source image
right		- Right coordinate of source image
bottom		- Bottom coordinate of source image
dstLeft		- Left coordinate of destination
dstTop		- Top coordinate of destination
transparent	- Transparent color to skip in source image
op			- Write mode to use during Blt

REMARKS:
Copies a block of bitmap data form one device context to another with either
source or destination transparency. When transferring the data with source
transparency, for pixels in the source image that are equal to the specified
transparent color, the related pixel in the destination buffer will remain
untouched. This allows you to quickly transfer sprites between device
contexts with a single color being allocated as a transparent color.

This routine has been highly optimized for maximum performance in all pixel
depths, so will provide a very fast method for performing transparent sprite
animation. However you may find that if you can use alternative techniques to
pre-compile the sprites (like using run length encoding etc.) you will be
able to build faster software based sprite animation code that can directly
access the device context surface. However this routine can also be used to
perform hardware accelerated Blt's between offscreen memory device's and the
display device when running in fullscreen modes, providing the hardware
accelerator (if present) can support this operation. If you have a hardware
accelerator capable of this, this will provide the ultimate performance for
transparent sprite animation.

The source and destination rectangles are clipped according to the current
clipping rectangles for the source and destination device contexts
respectively.

Note: 	The pixel depth and pixel format for the source bitmap and the
		device contexts must be the same or this routine will simply do
		nothing. This routine also only works with pixel depths that are
		at least 8 bits deep.

SEE ALSO:
MGL_srcTransBlt, MGL_dstTransBlt, MGL_bitBlt
****************************************************************************/
void MGLAPI MGL_srcTransBltCoord(
	MGLDC *dst,
	MGLDC *src,
	int left,
	int top,
	int right,
	int bottom,
	int dstLeft,
	int dstTop,
	color_t transparent,
	int op)
{
	rect_t	d,r,clip;
	int 	srcLeft,srcTop,srcRight,srcBottom;

	/* Check for valid contexts */
	if ((src == dst) || src->mi.bitsPerPixel < 8 || (src->mi.bitsPerPixel
			!= dst->mi.bitsPerPixel)) {
		SETERROR(grInvalidDevice);
		return;
		}

	/* In order to clip the results, we first clip the source rectangle to
	 * the source device context, and then clip the destination rectangle to
	 * the destination device context.
	 */
	if (src == _MGL_dcPtr)
		src = &DC;
	if (dst == _MGL_dcPtr)
		dst = &DC;
	d.left = left;				d.top = top;
	d.right = right;			d.bottom = bottom;
	if (!MGL_sectRect(src->a.clipRect,d,&d))
		return;
	dstLeft += (d.left - left);	dstTop += (d.top - top);
	left = d.left;				right = d.right;
	top = d.top;				bottom = d.bottom;

	/* Clip to destination device context */
	d.left = dstLeft;
	d.top = dstTop;
	d.right = dstLeft + (right-left);
	d.bottom = dstTop + (bottom-top);
	if (!MGL_sectRect(dst->a.clipRect,d,&d))
		return;
	left += (d.left - dstLeft);	right = left + (d.right - d.left);
	top += (d.top - dstTop);	bottom = top + (d.bottom - d.top);
	dstLeft = d.left;			dstTop = d.top;

	/* Now perform the blit operation */
	MAKE_HARDWARE_CURRENT(dst,true);
	if (src->deviceType == MGL_MEMORY_DEVICE || dst->deviceType == MGL_MEMORY_DEVICE) {
		/* Copy from one DC to any other DC when one is in system memory */
		if (dst->a.clipRegion) {
			left += src->a.viewPort.left;
			top += src->a.viewPort.top;
			right += src->a.viewPort.left;
			bottom += src->a.viewPort.top;
			d.left = dstLeft + dst->a.viewPort.left;
			d.top = dstTop + dst->a.viewPort.top;
			d.right = d.left + (right-left);
			d.bottom = d.top + (bottom-top);
			BEGIN_CLIP_REGION(clip,dst->intClipRegion);
				if (MGL_sectRect(clip,d,&r)) {
					srcLeft = left + (r.left - d.left);
					srcRight = srcLeft + (r.right - r.left);
					srcTop = top + (r.top - d.top);
					srcBottom = srcTop + (r.bottom - r.top);
					dst->r.SrcTransBltSys(src->surface,src->mi.bytesPerLine,
						srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
						r.left,r.top,op,transparent,false);
					}
			END_CLIP_REGION;
			}
		else {
			dst->r.SrcTransBltSys(src->surface,src->mi.bytesPerLine,
				left + src->a.viewPort.left,top + src->a.viewPort.top,
				right - left,bottom - top,
				dstLeft + dst->a.viewPort.left,dstTop + dst->a.viewPort.top,
				op,transparent,false);
			}
		}
	else {
		/* BitBlt between two device contexts which may both be in
		 * hardware. We allow the MGL device driver determine how to
		 * handle the case, so that it can be done in hardware if
		 * available (such as blitting from offscreen DC or back buffers).
		 */
		if (dst->a.clipRegion) {
			left += src->a.viewPort.left;
			top += src->a.viewPort.top;
			right += src->a.viewPort.left;
			bottom += src->a.viewPort.top;
			d.left = dstLeft + dst->a.viewPort.left;
			d.top = dstTop + dst->a.viewPort.top;
			d.right = d.left + (right-left);
			d.bottom = d.top + (bottom-top);
			BEGIN_CLIP_REGION(clip,dst->intClipRegion);
				if (MGL_sectRect(clip,d,&r)) {
					srcLeft = left + (r.left - d.left);
					srcRight = srcLeft + (r.right - r.left);
					srcTop = top + (r.top - d.top);
					srcBottom = srcTop + (r.bottom - r.top);
					dstLeft = r.left;
					dstTop = r.top;
					src->r.SrcTransBltDC(src,dst,
						srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
						dstLeft,dstTop,op,transparent);
					}
			END_CLIP_REGION;
			}
		else {
			src->r.SrcTransBltDC(src,dst,
				left + src->a.viewPort.left,top + src->a.viewPort.top,
				right - left,bottom - top,
				dstLeft + dst->a.viewPort.left,dstTop + dst->a.viewPort.top,op,transparent);
			}
		}
	RESTORE_HARDWARE(dst,true);
}

/****************************************************************************
DESCRIPTION:
Copies a block of image data with destination transparency.

HEADER:
mgraph.h

PARAMETERS:
dst			- Destination device context
src			- Source device context
left		- Left coordinate of source image
top			- Top coordinate of source image
right		- Right coordinate of source image
bottom		- Bottom coordinate of source image
dstLeft		- Left coordinate of destination
dstTop		- Top coordinate of destination
transparent	- Transparent color to skip in source image
op			- Write mode to use during Blt

REMARKS:
Copies a block of bitmap data form one device context to another with either
source or destination transparency. When transferring the data with
destination transparency, pixels in the destination image that are equal to
the specified transparent color will be updated, and those pixels that are
not the same will be skipped. This is effectively the operation performed
for 'blueScreen'ing or color keying and can also be used for drawing
transparent sprites. Note however that destination transparency is very
slow in software compared to source transparency!

This routine has been highly optimized for maximum performance in all pixel
depths, so will provide a very fast method for performing transparent sprite
animation. However you may find that if you can use alternative techniques to
pre-compile the sprites (like using run length encoding etc.) you will be
able to build faster software based sprite animation code that can directly
access the device context surface. However this routine can also be used to
perform hardware accelerated Blt's between offscreen memory device's and the
display device when running in fullscreen modes, providing the hardware
accelerator (if present) can support this operation. If you have a hardware
accelerator capable of this, this will provide the ultimate performance for
transparent sprite animation.

The source and destination rectangles are clipped according to the current
clipping rectangles for the source and destination device contexts
respectively.

Note: 	The pixel depth and pixel format for the source bitmap and the
		device contexts must be the same or this routine will simply do
		nothing. This routine also only works with pixel depths that are
		at least 8 bits deep.

SEE ALSO:
MGL_srcTransBlt, MGL_dstTransBlt, MGL_bitBlt
****************************************************************************/
void MGLAPI MGL_dstTransBltCoord(
	MGLDC *dst,
	MGLDC *src,
	int left,
	int top,
	int right,
	int bottom,
	int dstLeft,
	int dstTop,
	color_t transparent,
	int op)
{
	rect_t	d,r,clip;
	int 	srcLeft,srcTop,srcRight,srcBottom;

	/* Check for valid contexts */
	if ((src == dst) || src->mi.bitsPerPixel < 8 || (src->mi.bitsPerPixel
			!= dst->mi.bitsPerPixel)) {
		SETERROR(grInvalidDevice);
		return;
		}

	/* In order to clip the results, we first clip the source rectangle to
	 * the source device context, and then clip the destination rectangle to
	 * the destination device context.
	 */
	if (src == _MGL_dcPtr)
		src = &DC;
	if (dst == _MGL_dcPtr)
		dst = &DC;
	d.left = left;				d.top = top;
	d.right = right;			d.bottom = bottom;
	if (!MGL_sectRect(src->a.clipRect,d,&d))
		return;
	dstLeft += (d.left - left);	dstTop += (d.top - top);
	left = d.left;				right = d.right;
	top = d.top;				bottom = d.bottom;

	/* Clip to destination device context */
	d.left = dstLeft;
	d.top = dstTop;
	d.right = dstLeft + (right-left);
	d.bottom = dstTop + (bottom-top);
	if (!MGL_sectRect(dst->a.clipRect,d,&d))
		return;
	left += (d.left - dstLeft);	right = left + (d.right - d.left);
	top += (d.top - dstTop);	bottom = top + (d.bottom - d.top);
	dstLeft = d.left;			dstTop = d.top;

	/* Now perform the blit operation */
	MAKE_HARDWARE_CURRENT(dst,true);
	if (src->deviceType == MGL_MEMORY_DEVICE || dst->deviceType == MGL_MEMORY_DEVICE) {
		/* Copy from one DC to any other DC when one is in system memory */
		if (dst->a.clipRegion) {
			left += src->a.viewPort.left;
			top += src->a.viewPort.top;
			right += src->a.viewPort.left;
			bottom += src->a.viewPort.top;
			d.left = dstLeft + dst->a.viewPort.left;
			d.top = dstTop + dst->a.viewPort.top;
			d.right = d.left + (right-left);
			d.bottom = d.top + (bottom-top);
			BEGIN_CLIP_REGION(clip,dst->intClipRegion);
				if (MGL_sectRect(clip,d,&r)) {
					srcLeft = left + (r.left - d.left);
					srcRight = srcLeft + (r.right - r.left);
					srcTop = top + (r.top - d.top);
					srcBottom = srcTop + (r.bottom - r.top);
					dst->r.DstTransBltSys(src->surface,src->mi.bytesPerLine,
						srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
						r.left,r.top,op,transparent,false);
					}
			END_CLIP_REGION;
			}
		else {
			dst->r.DstTransBltSys(src->surface,src->mi.bytesPerLine,
				left + src->a.viewPort.left,top + src->a.viewPort.top,
				right - left,bottom - top,
				dstLeft + dst->a.viewPort.left,dstTop + dst->a.viewPort.top,
				op,transparent,false);
			}
		}
	else {
		/* BitBlt between two device contexts which may both be in
		 * hardware. We allow the MGL device driver determine how to
		 * handle the case, so that it can be done in hardware if
		 * available (such as blitting from offscreen DC or back buffers).
		 */
		if (dst->a.clipRegion) {
			left += src->a.viewPort.left;
			top += src->a.viewPort.top;
			right += src->a.viewPort.left;
			bottom += src->a.viewPort.top;
			d.left = dstLeft + dst->a.viewPort.left;
			d.top = dstTop + dst->a.viewPort.top;
			d.right = d.left + (right-left);
			d.bottom = d.top + (bottom-top);
			BEGIN_CLIP_REGION(clip,dst->intClipRegion);
				if (MGL_sectRect(clip,d,&r)) {
					srcLeft = left + (r.left - d.left);
					srcRight = srcLeft + (r.right - r.left);
					srcTop = top + (r.top - d.top);
					srcBottom = srcTop + (r.bottom - r.top);
					dstLeft = r.left;
					dstTop = r.top;
					src->r.DstTransBltDC(src,dst,
						srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
						dstLeft,dstTop,op,transparent);
					}
			END_CLIP_REGION;
			}
		else {
			src->r.DstTransBltDC(src,dst,
				left + src->a.viewPort.left,top + src->a.viewPort.top,
				right - left,bottom - top,
				dstLeft + dst->a.viewPort.left,dstTop + dst->a.viewPort.top,op,transparent);
			}
		}
	RESTORE_HARDWARE(dst,true);
}

/****************************************************************************
DESCRIPTION:
Copies a block of image data with source transparency.

HEADER:
mgraph.h

PARAMETERS:
dst			- Destination device context
src			- Source device context
srcRect		- Rectangle defining source image
dstLeft		- Left coordinate of destination
dstTop		- Top coordinate of destination
transparent	- Transparent color to skip in source image
op			- Write mode to use during Blt

REMARKS:
This function is the same as MGL_srcTransBltCoord, however it takes a
rectangle as a parameter instead of the four coordinates of a rectangle.

SEE ALSO:
MGL_srcTransBltCoord, MGL_dstTransBltCoord, MGL_bitBlt
****************************************************************************/
void MGL_srcTransBlt(
	MGLDC *dst,
	MGLDC *src,
	rect_t srcRect,
	int dstLeft,
	int dstTop,
	color_t transparent,
	int op);
/* Implemented as a macro */

/****************************************************************************
DESCRIPTION:
Copies a block of image data with destination transparency.

HEADER:
mgraph.h

PARAMETERS:
dst			- Destination device context
src			- Source device context
srcRect		- Rectangle defining source image
dstLeft		- Left coordinate of destination
dstTop		- Top coordinate of destination
transparent	- Transparent color to skip in source image
op			- Write mode to use during Blt

REMARKS:
This function is the same as MGL_dstTransBltCoord, however it takes a
rectangle as a parameter instead of the four coordinates of a rectangle.

SEE ALSO:
MGL_srcTransBltCoord, MGL_dstTransBltCoord, MGL_bitBlt
****************************************************************************/
void MGL_dstTransBlt(
	MGLDC *dst,
	MGLDC *src,
	rect_t srcRect,
	int dstLeft,
	int dstTop,
	color_t transparent,
	int op);
/* Implemented as a macro */

/****************************************************************************
DESCRIPTION:
Stretches a block of image data from one device context to another.

HEADER:
mgraph.h

PARAMETERS:
dst			- Destination device context
src			- Source device context
left		- Left coordinate of source image
top			- Top coordinate of source image
right		- Right coordinate of source image
bottom	   	- Bottom coordinate of source image
dstLeft		- Left coordinate of destination image
dstTop		- Top coordinate of destination image
dstRight	- Right coordinate of destination image 
dstBottom	- Bottom coordinate of destination image 
op			- Write mode to use during Blt

REMARKS:
Copies a block of bitmap data form one device context to another, stretching or
shrinking the image as necessary to fit the destination rectangle for the destination
device context.

The source and destination device context may not be the same. This routine
has been highly optimized for absolute maximum performance, so it will
provide the fastest method of stretching bitmap data between device contexts,
and can also be used to stretch bitmap data from a memory device context
to a windowed device context.

This function will correctly handle StretchBlt's across device contexts with
differing pixel depths, and will perform the necessary pixel format
translation to convert from the source device to the destination device.
Note that although the code to implement this is highly optimized, this can
be a time consuming operation so you should attempt to pre-convert all
bitmaps to the current display device pixel format for maximum performance
if possible.

MGL does however have special case code to specifically handle translation of
24 bit RGB format bitmaps (the standard RGB DIB format used by Video for
Windows) to all 8 bit and above pixel formats. When converting from 24 bit
to 8 bit, MGL will dither bitmaps in real time from 24 bit to the 8 bit
halftone palette. This provides a solid foundation to build real time 24 bit
motion video playback in all supported video modes in MGL.

Note that when MGL_bitBlt is called for 4 and 8 bit source bitmaps MGL first
checks if the color palettes for the source and destination bitmaps are the
same. If they are not, MGL translates the pixel values from the source bitmap
to the destination color palette, looking for the closest match color if an
exact match is not found. In order to obtain maximum performance for blt'ing
bitmaps in color index modes, you should ensure that the color palette in
the sourcedevice matches the color palette in the destination device, or
you can turn off all identity palette checking in MGL with the
MGL_checkIdentityPalette function.

The source and destination rectangles are clipped according to the current
clipping rectangles for the source and destination device contexts
respectively, however the zoom factor is determined using the unclipped
source and destination rectangles.

SEE ALSO:
MGL_stretchBlt, MGL_bitBlt, MGL_bitBltCoord
****************************************************************************/
void MGLAPI MGL_stretchBltCoord(
	MGLDC *dst,
	MGLDC *src,
	int left,
	int top,
	int right,
	int bottom,
	int dstLeft,
	int dstTop,
	int dstRight,
	int dstBottom,
	int op)
{
	rect_t	d,r,clip;
	int		deltaSrc,deltaDst;
	int 	fdTop, fdLeft;
	fix32_t	zoomx,zoomy;
	ibool   clipIt = false;

	/* Check for valid device contexts */
	if ((src == dst) || src->mi.bitsPerPixel < 8) {
		SETERROR(grInvalidDevice);
		return;
		}
	if (src == _MGL_dcPtr)
		src = &DC;
	if (dst == _MGL_dcPtr)
		dst = &DC;

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
		MGL_bitBltCoord(dst,src,left,top,right,bottom,dstLeft,dstTop,op);
		return;
		}

	/* Clip to the source device context */
	d.left = left;				d.top = top;
	d.right = right;			d.bottom = bottom;
	if (!MGL_sectRect(src->a.clipRect,d,&d))
		return;
	fdLeft = MGL_TOFIX(dstLeft) + ((d.left - left) * zoomx);
	fdTop = MGL_TOFIX(dstTop) + ((d.top - top) * zoomy);
	left = d.left;				right = d.right;
	top = d.top;				bottom = d.bottom;
	dstLeft = MGL_FIXROUND(fdLeft);
	dstTop = MGL_FIXROUND(fdTop);
	dstRight = dstLeft+MGL_FIXTOINT((right-left)*zoomx);
	dstBottom = dstTop+MGL_FIXTOINT((bottom-top)*zoomy);

	/* Clip the destination device context */
	d.left = dstLeft;			d.top = dstTop;
	d.right = dstRight;			d.bottom = dstBottom;
	if (!MGL_sectRect(dst->a.clipRect,d,&d))
		return;
	if ((dst->a.clipRegion) || (d.left != dstLeft) || (d.right  != dstRight)
			|| (d.top != dstTop) || (d.bottom != dstBottom))
		clipIt = true;

	/* Now perform the blit operation */
	MAKE_HARDWARE_CURRENT(dst,true);
	if (src->deviceType == MGL_MEMORY_DEVICE || dst->deviceType == MGL_MEMORY_DEVICE) {
		/* Copy from one DC to any other DC when one is in system memory */
		if (NEED_TRANSLATE_DC(src,dst)) {
			/* Translate the pixel information when doing the stretch Blt */
			if (dst->a.clipRegion) {
				left += src->a.viewPort.left;
				top += src->a.viewPort.top;
				right += src->a.viewPort.left;
				bottom += src->a.viewPort.top;
				d.left = dstLeft + dst->a.viewPort.left;
				d.top = dstTop + dst->a.viewPort.top;
				d.right = dstRight + dst->a.viewPort.left;
				d.bottom = dstBottom + dst->a.viewPort.top;
				BEGIN_CLIP_REGION(clip,dst->intClipRegion);
					if (MGL_sectRect(clip,d,&r)) {
						dst->r.StretchConvertBltSys(src->surface,src->mi.bytesPerLine,
							left,top,right-left,bottom-top,
							d.left,d.top,d.right-d.left,d.bottom-d.top,clipIt,
							r.left,r.top,r.right,r.bottom,
							src->mi.bitsPerPixel,GAPF(&src->pf),GAPAL(dst->colorTab),
							GAPAL(src->colorTab),dst->a.ditherMode,op,false);
						}
				END_CLIP_REGION;
				}
			else {
				dst->r.StretchConvertBltSys(src->surface,src->mi.bytesPerLine,
					left + src->a.viewPort.left,top + src->a.viewPort.top,
					right-left,bottom-top,
					dstLeft + dst->a.viewPort.left,dstTop + dst->a.viewPort.top,
					dstRight-dstLeft,dstBottom-dstTop,clipIt,
					dst->intClipRect.left,dst->intClipRect.top,
					dst->intClipRect.right,dst->intClipRect.bottom,
					src->mi.bitsPerPixel,GAPF(&src->pf),GAPAL(dst->colorTab),
					GAPAL(src->colorTab),dst->a.ditherMode,op,false);
				}
			}
		else {
			/* Perform the stretch blit with no pixel format conversion */
			if (dst->a.clipRegion) {
				left += src->a.viewPort.left;
				top += src->a.viewPort.top;
				right += src->a.viewPort.left;
				bottom += src->a.viewPort.top;
				d.left = dstLeft + dst->a.viewPort.left;
				d.top = dstTop + dst->a.viewPort.top;
				d.right = dstRight + dst->a.viewPort.left;
				d.bottom = dstBottom + dst->a.viewPort.top;
				BEGIN_CLIP_REGION(clip,dst->intClipRegion);
					if (MGL_sectRect(clip,d,&r)) {
						dst->r.StretchBltSys(src->surface,src->mi.bytesPerLine,
							left,top,right-left,bottom-top,
							d.left,d.top,d.right-d.left,d.bottom-d.top,clipIt,
							r.left,r.top,r.right,r.bottom,op,false);
						}
				END_CLIP_REGION;
				}
			else {
				dst->r.StretchBltSys(src->surface,src->mi.bytesPerLine,
					left + src->a.viewPort.left,top + src->a.viewPort.top,
					right-left,bottom-top,
					dstLeft + dst->a.viewPort.left,dstTop + dst->a.viewPort.top,
					dstRight-dstLeft,dstBottom-dstTop,clipIt,
					dst->intClipRect.left,dst->intClipRect.top,
					dst->intClipRect.right,dst->intClipRect.bottom,
					op,false);
				}
			}
		}
	else {
		/* StretchBlt between two device contexts which may both be in
		 * hardware. We allow the MGL device driver determine how to
		 * handle the case, so that it can be done in hardware if
		 * available (such as blitting from offscreen DC or back buffers).
		 */
		if (dst->a.clipRegion) {
			left += src->a.viewPort.left;
			top += src->a.viewPort.top;
			right += src->a.viewPort.left;
			bottom += src->a.viewPort.top;
			d.left = dstLeft + dst->a.viewPort.left;
			d.top = dstTop + dst->a.viewPort.top;
			d.right = dstRight + dst->a.viewPort.left;
			d.bottom = dstBottom + dst->a.viewPort.top;
			BEGIN_CLIP_REGION(clip,dst->intClipRegion);
				if (MGL_sectRect(clip,d,&r)) {
					dst->r.StretchBltDC(src,dst,
						left,top,right-left,bottom-top,
						d.left,d.top,d.right-d.left,d.bottom-d.top,clipIt,
						r.left,r.top,r.right,r.bottom,op);
					}
			END_CLIP_REGION;
			}
		else {
			dst->r.StretchBltDC(src,dst,
				left + src->a.viewPort.left,top + src->a.viewPort.top,
				right-left,bottom-top,
				dstLeft + dst->a.viewPort.left,dstTop + dst->a.viewPort.top,
				dstRight-dstLeft,dstBottom-dstTop,clipIt,
				dst->intClipRect.left,dst->intClipRect.top,
				dst->intClipRect.right,dst->intClipRect.bottom,op);
			}
		}
	RESTORE_HARDWARE(dst,true);
}

/****************************************************************************
DESCRIPTION:
Stretches a block of image data from one device context to another.

HEADER:
mgraph.h

PARAMETERS:
dst	- Destination device context
src	- Source device context
srcRect	- Rectangle defining source image
dstRect	- Rectangle defining destination image 

REMARKS:
This function is the same as MGL_stretchBltCoord, however it takes entire
rectangles as arguments instead of coordinates.

SEE ALSO:
MGL_stretchBltCoord, MGL_bitBlt, MGL_bitBltCoord
****************************************************************************/
void MGL_stretchBLT(
	MGLDC dst,
	MGLDC src,
	rect_t srcRect,
	rect_t destRect);
/* Implemented as a macro */
