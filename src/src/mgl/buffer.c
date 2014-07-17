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
* Description:	Offscreen buffer management code.
*
****************************************************************************/

#include "mgl.h"

/*------------------------- Implementation --------------------------------*/

/****************************************************************************
DESCRIPTION:
Creates a new MGL memory buffer for storing bitmaps

HEADER:
mgraph.h

PARAMETERS:
dc		- MGL display or windowed device context to allocate buffer from
width	- Width of the buffer in pixels
height	- Height of the buffer in scanlines
cacheIt	- True for MGL to cache bitmap surface

RETURNS:
Pointer to the allocated buffer, NULL on failure.

REMARKS:
This function allocates a new lightweight bitmap buffer in offscreen display
memory. Lighweight bitmap buffers are used to store bitmap and sprite
information in the offscreen display memory in the hardware, but are not
full device contexts. Hence buffers have much less memory overhead than a
full offscreen device context. Buffers can only be used for storing bitmaps
and blitting the around on the screen. You can copy the contents to a MGL
device context using the MGL_putBuffer, MGL_stretchBuffer and
MGL_putBufferSrcTrans functions. You can also copy the contents of an
MGL device context to a buffer using the MGL_copyToBuffer function.

If you need to draw on a buffer in offscreen memory, create a full
offscreen device context instead. Then you can call any of the standard
MGL drawing functions and BitBlt operations for the offscreen memory buffer.
The primary disadvantage of doing this is that a full offscreen device
context has a lot more memory overhead involved in maintaining the
device context state information than a simple offscreen buffer.

If you set the cacheIt parameter to true, the MGL will always keep a cached
copy of the surface memory for the bufferin system memory. If your offscreen
buffers get lost (ie: on a fullscreen mode switch), they will automatically
be restored when the application regains the active focus. This also allows
the MGL to compact the offscreen memory heap when necessary. If you don't
set the cacheIt parameter to true, then it is your applications
responsibility to reload the bitmaps when the focus is lost and regained.

Note:	The MGL automatically manages offscreen display memory, and if you
		run out of offscreen display memory it will place the buffer
		surfaces in system memory (if allowSysMem is true). Hence you should
		allocate your important buffers first, to ensure they end up in
		offscreen memory.

Note:	To stop the MGL from allocating buffers in system memory, set the
		cacheIt flag to -1 and the buffers will only be stored in offscreen
		memory. A value of -2 will disable system memory and also disable
		caching.

SEE ALSO:
MGL_lockBuffer, MGL_unlockBuffer, MGL_destroyBuffer, MGL_createOffscreenDC,
MGL_putBuffer, MGL_stretchBuffer, MGL_putBufferSrcTrans,
MGL_putBufferDstTrans, MGL_copyToBuffer, MGL_copyBitmapToBuffer
****************************************************************************/
MGLBUF * MGLAPI MGL_createBuffer(
	MGLDC *dc,
	int width,
	int height,
	ibool cacheIt)
{
	MGLBUF	*buf;
	ibool	(MGLAPIP create)(MGLBUF *buf);

	/* Check that the DC is valid, and find buffer create function */
	CHECK(dc != NULL);
	switch (dc->deviceType) {
		case MGL_DISPLAY_DEVICE:
			create = dc->v->d.createOffscreenBuffer;
			break;
		case MGL_WINDOWED_DEVICE:
			create = dc->v->w.createOffscreenBuffer;
			break;
		case MGL_MEMORY_DEVICE:
			create = NULL;
			break;
		default:
			FATALERROR(grInvalidDC);
			return NULL;
		}

	/* Allocate memory for the new buffer */
	if ((buf = _LST_newNode(sizeof(MGLBUF))) == NULL) {
		FATALERROR(grNoMem);
		return NULL;
		}

	/* Create the offscreen buffer list if it does not exist */
	if (DEV.offBufList == NULL) {
		if ((DEV.offBufList = _LST_create()) == NULL) {
			FATALERROR(grNoMem);
			goto Error;
			}
		}

	/* Now fill in the information for the buffer and create it */
	buf->dc = dc;
	buf->width = width;
	buf->height = height;
	buf->cacheBytesPerLine = BYTESPERLINE(width,dc->mi.bitsPerPixel);
	if (!create || !create(buf)) {
		/* Return failure if not in hardware when creating offscreen DC's */
		if (cacheIt < 0) {
			__MGL_result = grNoOffscreenMem;
			goto Error;
			}

		/* Buffer creation failed in hardware, so allocate it in system
		 * memory.
		 */
		if ((buf->surface = MGL_calloc(1,height * buf->cacheBytesPerLine)) == NULL) {
			FATALERROR(grNoMem);
			goto Error;
			}
		buf->bytesPerLine = buf->cacheBytesPerLine;
		buf->surfaceCache = buf->surface;
		}

	/* If the buffer is not in system memory, create the surface cache for
	 * the buffer in system memory.
	 */
	if (cacheIt && cacheIt > -2 && !buf->surfaceCache) {
		if ((buf->surfaceCache = MGL_calloc(1,height * buf->cacheBytesPerLine)) == NULL) {
			FATALERROR(grNoMem);
			goto Error;
			}
		}

	/* Add the new buffer to the end of the buffer chain */
	_LST_addToTail(DEV.offBufList,buf);
	return buf;

Error:
	if (DEV.offBufList && DEV.offBufList->count == 0) {
		_LST_destroy(DEV.offBufList,_LST_freeNode);
		DEV.offBufList = NULL;
		}
	_LST_freeNode(buf);
	return NULL;
}

/****************************************************************************
DESCRIPTION:
Lock a buffer to begin direct surface access

HEADER:
mgraph.h

PARAMETERS:
buf	- MGL buffer to lock

REMARKS:
This function locks a buffer so that an application can begin drawing
directly on the surface memory. You /must/ call this function before you
draw directly on the bitmap surface!

SEE ALSO:
MGL_unlockBuffer
****************************************************************************/
void MGLAPI MGL_lockBuffer(
	MGLBUF *buf)
{
	CHECK(buf != NULL);
	if (buf->surface != buf->surfaceCache)
		buf->dc->r.lockBuffer(buf);
}

/****************************************************************************
DESCRIPTION:
Unlock a buffer after direct surface access

HEADER:
mgraph.h

PARAMETERS:
buf	- MGL buffer to unlock

REMARKS:
This function unlocks a buffer after the application has completed direct
surface access on the buffer.

SEE ALSO:
MGL_lockBuffer
****************************************************************************/
void MGLAPI MGL_unlockBuffer(
	MGLBUF *buf)
{
	CHECK(buf != NULL);
	if (buf->surface != buf->surfaceCache)
		buf->dc->r.unlockBuffer(buf);
}

/****************************************************************************
DESCRIPTION:
Destroys an existing MGL buffer

HEADER:
mgraph.h

PARAMETERS:
buf			- MGL buffer to destroy

REMARKS:
This function destroys an MGL buffer, and frees all resources associated
with the buffer.

SEE ALSO:
MGL_createBuffer
****************************************************************************/
void MGLAPI MGL_destroyBuffer(
	MGLBUF *buf)
{
	MGLBUF	*prev;

	/* Remove the buffer from the list of allocated buffers */
	if (!buf)
		return;
	if ((prev = _LST_findNode(DEV.offBufList,buf)) == NULL) {
		SETERROR(grInvalidBuffer);
		return;
		}
	_LST_removeNext(DEV.offBufList,prev);

	/* Destroy the buffer in offscreen memory if it was allocated */
	if (buf->surface != buf->surfaceCache) {
		switch (buf->dc->deviceType) {
			case MGL_DISPLAY_DEVICE:
				buf->dc->v->d.destroyOffscreenBuffer(buf);
				break;
			case MGL_WINDOWED_DEVICE:
				buf->dc->v->w.destroyOffscreenBuffer(buf);
				break;
			default:
				LOGFATAL("Invalid DC for offscreen buffer!");
				break;
			}
		}
	else {
		/* Buffer was in system memory */
		MGL_free(buf->surface);
		}

	/* Free up all memory occupied by the buffer */
	if (buf->surfaceCache)
		MGL_free(buf->surfaceCache);
	_LST_freeNode(buf);

	/* If this is the last buffer, destroy the list */
	if (DEV.offBufList->count == 0) {
		_LST_destroy(DEV.offBufList,_LST_freeNode);
		DEV.offBufList = NULL;
		}
}

/****************************************************************************
PARAMETERS:
width				- Width of the source bitmap
height				- Height of the source bitmap
bytesPerLine		- Pitch of the source bitmap
bitsPerPixel		- Color depth of the source bitmap
surface				- Surface of the source bitmap
left				- Left coordinate in device context to blit from
top					- Top coordinate in device context to blit from
right   			- Right coordinate in device context to blit from
bottom  			- Bottom coordinate in device context to blit from
dstLeft 			- Left coordinate in buffer to blit to
dstTop  			- Top coordinate in buffer to blit to
buf					- MGL buffer to blit to
cacheBytesPerLine	- Pitch of the surface cache
surfaceCache		- Surface cache bitmap to copy to (NULL to ignore)

REMARKS:
Internal function to do the work of coping data to a buffer.
****************************************************************************/
static void __MGL_copyToBuffer(
	int width,
	int height,
	int bytesPerLine,
	int bitsPerPixel,
	void *surface,
	int left,
	int top,
	int right,
	int bottom,
	int dstLeft,
	int dstTop,
	MGLBUF *buf,
	int cacheBytesPerLine,
	void *surfaceCache)
{
	rect_t	d;
	int 	widthBytes;
	uchar 	*src,*dst,*cache;

	/* Clip the source rectangle to the bitmap dimensions */
	if (!MGL_sectRectCoord(0,0,width,height,left,top,right,bottom,&d))
		return;
	dstLeft += (d.left - left);	dstTop += (d.top - top);
	left = d.left;				right = d.right;
	top = d.top;				bottom = d.bottom;

	/* Clip to buffer dimensions */
	d.left = dstLeft;					d.top = dstTop;
	d.right = dstLeft + (right-left);	d.bottom = dstTop + (bottom-top);
	if (!MGL_sectRectCoord(0,0,buf->width,buf->height,d.left,d.top,d.right,d.bottom,&d))
		return;
	left += (d.left - dstLeft);
	top += (d.top - dstTop);

	/* Now copy the bitmap information */
	src = PIXEL_ADDR(left,top,surface,bytesPerLine,bitsPerPixel);
	dst = PIXEL_ADDR(dstLeft,dstTop,buf->surface,buf->bytesPerLine,bitsPerPixel);
	cache = PIXEL_ADDR(left,top,surfaceCache,cacheBytesPerLine,bitsPerPixel);
	widthBytes = BYTESPERLINE((d.right-d.left),bitsPerPixel);
	if (buf->surface != buf->surfaceCache)
		buf->dc->r.lockBuffer(buf);
	while (height--) {
		MGL_memcpy(dst,src,widthBytes);
		if (surfaceCache)
			MGL_memcpy(cache,src,widthBytes);
		src += bytesPerLine;
		dst += buf->bytesPerLine;
		cache += cacheBytesPerLine;
		}
	if (buf->surface != buf->surfaceCache)
		buf->dc->r.unlockBuffer(buf);
}

/****************************************************************************
DESCRIPTION:
Copies a section of a device context to an offscreen buffer

HEADER:
mgraph.h

PARAMETERS:
dc		- Device context to blit from
left	- Left coordinate in device context to blit from
top		- Top coordinate in device context to blit from
right   - Right coordinate in device context to blit from
bottom  - Bottom coordinate in device context to blit from
dstLeft - Left coordinate in buffer to blit to
dstTop  - Top coordinate in buffer to blit to
buf		- MGL buffer to blit to

REMARKS:
This function is used to copy bitmap data from an MGL device context into
an offscreen buffer. The information copied is clipped to the full dimensions
of the buffer, and the source clipping rectangle in the source device context
is ignored.

SEE ALSO:
MGL_copyBitmapToBuffer, MGL_putBuffer
****************************************************************************/
void MGLAPI MGL_copyToBuffer(
	MGLDC *dc,
	int left,
	int top,
	int right,
	int bottom,
	int dstLeft,
	int dstTop,
	MGLBUF *buf)
{
	void	*surfaceCache = NULL;

	CHECK(dc);
	CHECK(buf);
	CHECK(dc->mi.bitsPerPixel == buf->dc->mi.bitsPerPixel);
	if (buf->surface != buf->surfaceCache)
		surfaceCache = buf->surfaceCache;
	__MGL_copyToBuffer(dc->mi.xRes+1,dc->mi.yRes+1,dc->mi.bytesPerLine,
		dc->mi.bitsPerPixel,dc->surface,left,top,right,bottom,
		dstLeft,dstTop,buf,buf->cacheBytesPerLine,surfaceCache);
}

/****************************************************************************
DESCRIPTION:
Copies a section of a bitmap to an offscreen buffer

HEADER:
mgraph.h

PARAMETERS:
bitmap	- Bitmap to blit from
left	- Left coordinate in device context to blit from
top		- Top coordinate in device context to blit from
right   - Right coordinate in device context to blit from
bottom  - Bottom coordinate in device context to blit from
dstLeft - Left coordinate in buffer to blit to
dstTop  - Top coordinate in buffer to blit to
buf		- MGL buffer to blit to

REMARKS:
This function is used to copy bitmap data from a lightweight system memory
bitmap into an offscreen buffer. The information copied is clipped to the
full dimensions of the buffer.

SEE ALSO:
MGL_copyToBuffer, MGL_putBuffer
****************************************************************************/
void MGLAPI MGL_copyBitmapToBuffer(
	bitmap_t *bitmap,
	int left,
	int top,
	int right,
	int bottom,
	int dstLeft,
	int dstTop,
	MGLBUF *buf)
{
	void 	*surfaceCache = NULL;

	CHECK(bitmap);
	CHECK(buf);
	CHECK(bitmap->bitsPerPixel == buf->dc->mi.bitsPerPixel);
	if (buf->surface != buf->surfaceCache)
		surfaceCache = buf->surfaceCache;
	__MGL_copyToBuffer(bitmap->width,bitmap->height,bitmap->bytesPerLine,
		bitmap->bitsPerPixel,bitmap->surface,left,top,right,bottom,
		dstLeft,dstTop,buf,buf->cacheBytesPerLine,surfaceCache);
}

/****************************************************************************
DESCRIPTION:
Copy an offscreen buffer to the specified location.

HEADER:
mgraph.h

PARAMETERS:
dc	- Device context to copy buffer to
x	- x coordinate to copy buffer to
y	- y coordinate to copy buffer to
buf	- Buffer to copy
op	- Write mode to use when drawing buffer

REMARKS:
Copies an offscreen buffer to the specified location on the device context.
Supported write modes are enumerated in MGL_writeModeType.

Note:	This function will fail if you attempt to copy a buffer to a
		device context that is not the device context that the original
		buffer was allocated for.

SEE ALSO:
MGL_putBufferSrcTrans, MGL_putBufferDstTrans, MGL_stretchBuffer,
MGL_createBuffer, MGL_copyToBuffer, MGL_copyBitmapToBuffer,
****************************************************************************/
void MGLAPI MGL_putBuffer(
	MGLDC *dc,
	int x,
	int y,
	MGLBUF *buf,
	int op)
{
	rect_t	d,r,clip;
	int 	left,top,right,bottom,srcLeft,srcTop,srcRight,srcBottom;

	/* Check that the device context pointer is valid */
	CHECK(dc == buf->dc);
	if (dc == _MGL_dcPtr)
		dc = &DC;

	/* Clip to destination device context */
	d.left = x;					d.top = y;
	d.right = x + buf->width;	d.bottom = y + buf->height;
	if (!MGL_sectRect(dc->a.clipRect,d,&d))
		return;

	/* Adjust to buffer coordinates */
	MGL_offsetRect(d,-x,-y);
	x += d.left;
	y += d.top;
	MAKE_HARDWARE_CURRENT(dc,true);
	if (buf->surfaceCache != buf->surface) {
		/* Buffer is in offscreen memory */
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
					if (buf->linearStart == -1) {
						dc->r.BltBuffer(buf,dc,
							srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
							r.left,r.top,op);
						}
					else {
						dc->r.BitBltLin(buf->linearStart,buf->bytesPerLine,
							srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
							r.left,r.top,op);
						}
					}
			END_CLIP_REGION;
			}
		else {
			if (buf->linearStart == -1) {
				dc->r.BltBuffer(buf,dc,
					d.left,d.top,d.right-d.left,d.bottom-d.top,
					x + dc->a.viewPort.left,y + dc->a.viewPort.top,op);
				}
			else {
				dc->r.BitBltLin(buf->linearStart,buf->bytesPerLine,
					d.left,d.top,d.right-d.left,d.bottom-d.top,
					x + dc->a.viewPort.left,y + dc->a.viewPort.top,op);
				}
			}
		}
	else {
		/* Buffer is in system memory */
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
					dc->r.BitBltSys(buf->surface,buf->bytesPerLine,
						srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
						r.left,r.top,op,false);
					}
			END_CLIP_REGION;
			}
		else {
			dc->r.BitBltSys(buf->surface,buf->bytesPerLine,
				d.left,d.top,d.right-d.left,d.bottom-d.top,
				x + dc->a.viewPort.left,y + dc->a.viewPort.top,op,false);
			}
		}
	RESTORE_HARDWARE(dc,true);
}

/****************************************************************************
DESCRIPTION:
Copy a section of an offscreen buffer to the specified location.

HEADER:
mgraph.h

PARAMETERS:
dc		- Device context to copy buffer to
left	- Left coordinate of section to copy
top		- Top coordinate of section to copy
right	- Right coordinate of section to copy
bottom	- Bottom coordinate of section to copy
dstLeft	- Left coordinate of destination to copy buffer to
dstTop	- Right coordinate of destination to copy buffer to
buf		- Buffer to copy
op		- Write mode to use when drawing buffer

REMARKS:
Copies a section of an offscreen buffer to the specified location on the
device context.

Note:	This function will fail if you attempt to copy a buffer to a
		device context that is not the device context that the original
		buffer was allocated for.

SEE ALSO:
MGL_putBuffer, MGL_putBufferSrcTrans, MGL_putBufferDstTrans,
MGL_stretchBuffer, MGL_createBuffer, MGL_copyToBuffer,
MGL_copyBitmapToBuffer,
****************************************************************************/
void MGLAPI MGL_putBufferSection(
	MGLDC *dc,
	int left,
	int top,
	int right,
	int bottom,
	int dstLeft,
	int dstTop,
	MGLBUF *buf,
	int op)
{
	rect_t	d,r,clip;
	int 	srcLeft,srcTop,srcRight,srcBottom;

	/* Check that the device context pointer is valid */
	CHECK(dc == buf->dc);
	if (dc == _MGL_dcPtr)
		dc = &DC;

	/* Clip the source rectangle to the buffer dimensions */
	if (!MGL_sectRectCoord(0,0,buf->width,buf->height,left,top,right,bottom,&d))
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
	if (buf->surfaceCache != buf->surface) {
		/* Buffer is in offscreen memory */
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
					if (buf->linearStart == -1) {
						dc->r.BltBuffer(buf,dc,
							srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
							r.left,r.top,op);
						}
					else {
						dc->r.BitBltLin(buf->linearStart,buf->bytesPerLine,
							srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
							r.left,r.top,op);
						}
					}
			END_CLIP_REGION;
			}
		else {
			if (buf->linearStart == -1) {
				dc->r.BltBuffer(buf,dc,
					left,top,right-left,bottom-top,
					dstLeft + dc->a.viewPort.left,dstTop + dc->a.viewPort.top,op);
				}
			else {
				dc->r.BitBltLin(buf->linearStart,buf->bytesPerLine,
					left,top,right-left,bottom-top,
					dstLeft + dc->a.viewPort.left,dstTop + dc->a.viewPort.top,op);
				}
			}
		}
	else {
		/* Buffer is in system memory */
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
					dc->r.BitBltSys(buf->surface,buf->bytesPerLine,
						srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
						r.left,r.top,op,false);
					}
			END_CLIP_REGION;
			}
		else {
			dc->r.BitBltSys(buf->surface,buf->bytesPerLine,
				left,top,right-left,bottom-top,
				dstLeft + dc->a.viewPort.left,dstTop + dc->a.viewPort.top,op,false);
			}
		}
	RESTORE_HARDWARE(dc,true);
}

/****************************************************************************
DESCRIPTION:
Copy an offscreen buffer to the specified location with source transparency.

HEADER:
mgraph.h

PARAMETERS:
dc			- Device context to copy buffer to
x			- x coordinate to copy buffer to
y			- y coordinate to copy buffer to
buf			- Buffer to copy
transparent	- Transparent color for the bitmap
op			- Write mode to use when drawing buffer

REMARKS:
Copies an offscreen buffer to the specified location on the device context.
Supported write modes are enumerated in MGL_writeModeType.

REMARKS:
Copies a offscreen buffer to the specified location on the device context
with either source or destination transparency.

When transferring the data with source transparency, for pixels in the source
image that are equal to the specified transparent color, the related pixel
in the destination buffer will remain untouched. This allows you to quickly
transfer sprites between device contexts with a single color being allocated
as a transparent color.

Note:	This function will fail if you attempt to copy a buffer to a
		device context that is not the device context that the original
		buffer was allocated for.

SEE ALSO:
MGL_putBuffer, MGL_putBufferSrcTransSection, MGL_putBufferDstTransSection,
MGL_stretchBuffer, MGL_createBuffer, MGL_copyToBuffer,
MGL_copyBitmapToBuffer
****************************************************************************/
void MGLAPI MGL_putBufferSrcTrans(
	MGLDC *dc,
	int x,
	int y,
	MGLBUF *buf,
	color_t transparent,
	int op)
{
	rect_t	d,r,clip;
	int 	left,top,right,bottom;
	int		srcLeft,srcTop,srcRight,srcBottom,bytesPerLine;
	void	*surface;

	/* Check that the device context pointer is valid */
	CHECK(dc == buf->dc);
	if (dc == _MGL_dcPtr)
		dc = &DC;

	/* Clip to destination device context */
	d.left = x;					d.top = y;
	d.right = x + buf->width;	d.bottom = y + buf->height;
	if (!MGL_sectRect(dc->a.clipRect,d,&d))
		return;

	/* Adjust to buffer coordinates */
	MGL_offsetRect(d,-x,-y);
	x += d.left;
	y += d.top;
	MAKE_HARDWARE_CURRENT(dc,true);
	if (buf->surfaceCache == buf->surface || (!(buf->dc->flags & MGL_HW_SRCTRANS_BLT) && buf->surfaceCache)) {
		/* Buffer is in system memory */
		if (!(buf->dc->flags & MGL_HW_SRCTRANS_BLT)) {
			surface = buf->surfaceCache;
			bytesPerLine = buf->cacheBytesPerLine;
			}
		else {
			surface = buf->surface;
			bytesPerLine = buf->bytesPerLine;
			}
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
					dc->r.SrcTransBltSys(surface,bytesPerLine,
						srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
						r.left,r.top,op,transparent,false);
					}
			END_CLIP_REGION;
			}
		else {
			dc->r.SrcTransBltSys(surface,bytesPerLine,
				d.left,d.top,d.right-d.left,d.bottom-d.top,
				x + dc->a.viewPort.left,y + dc->a.viewPort.top,
				op,transparent,false);
			}
		}
	else {
		/* Buffer is in offscreen memory */
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
					if (buf->linearStart == -1) {
						dc->r.SrcTransBltBuffer(buf,dc,
							srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
							r.left,r.top,op,transparent);
						}
					else {
						dc->r.SrcTransBltLin(buf->linearStart,buf->bytesPerLine,
							srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
							r.left,r.top,op,transparent);
						}
					}
			END_CLIP_REGION;
			}
		else {
			if (buf->linearStart == -1) {
				dc->r.SrcTransBltBuffer(buf,dc,
					d.left,d.top,d.right-d.left,d.bottom-d.top,
					x + dc->a.viewPort.left,y + dc->a.viewPort.top,
					op,transparent);
				}
			else {
				dc->r.SrcTransBltLin(buf->linearStart,buf->bytesPerLine,
					d.left,d.top,d.right-d.left,d.bottom-d.top,
					x + dc->a.viewPort.left,y + dc->a.viewPort.top,
					op,transparent);
				}
			}
		}
	RESTORE_HARDWARE(dc,true);
}

/****************************************************************************
DESCRIPTION:
Copy an offscreen buffer to the specified location with destination
transparency.

HEADER:
mgraph.h

PARAMETERS:
dc			- Device context to copy buffer to
x			- x coordinate to copy buffer to
y			- y coordinate to copy buffer to
buf			- Buffer to copy
transparent	- Transparent color for the bitmap
op			- Write mode to use when drawing buffer

REMARKS:
Copies an offscreen buffer to the specified location on the device context.
Supported write modes are enumerated in MGL_writeModeType.

REMARKS:
Copies a offscreen buffer to the specified location on the device context
with either source or destination transparency.

When transferring the data with destination transparency, pixels in the
destination image that are equal to the specified transparent color will be
updated, and those pixels that are not the same will be skipped. This is
effectively the operation performed for 'blueScreen'ing or color keying and
can also be used for drawing transparent sprites. Note however that
destination transparency is very slow in software compared to source
transparency!

Note:	This function will fail if you attempt to copy a buffer to a
		device context that is not the device context that the original
		buffer was allocated for.

SEE ALSO:
MGL_putBuffer, MGL_putBufferSrcTransSection, MGL_putBufferDstTransSection,
MGL_stretchBuffer, MGL_createBuffer, MGL_copyToBuffer,
MGL_copyBitmapToBuffer
****************************************************************************/
void MGLAPI MGL_putBufferDstTrans(
	MGLDC *dc,
	int x,
	int y,
	MGLBUF *buf,
	color_t transparent,
	int op)
{
	rect_t	d,r,clip;
	int 	left,top,right,bottom;
	int		srcLeft,srcTop,srcRight,srcBottom,bytesPerLine;
	void	*surface;

	/* Check that the device context pointer is valid */
	CHECK(dc == buf->dc);
	if (dc == _MGL_dcPtr)
		dc = &DC;

	/* Clip to destination device context */
	d.left = x;					d.top = y;
	d.right = x + buf->width;	d.bottom = y + buf->height;
	if (!MGL_sectRect(dc->a.clipRect,d,&d))
		return;

	/* Adjust to buffer coordinates */
	MGL_offsetRect(d,-x,-y);
	x += d.left;
	y += d.top;
	MAKE_HARDWARE_CURRENT(dc,true);
	if (buf->surfaceCache == buf->surface || (!(buf->dc->flags & MGL_HW_SRCTRANS_BLT) && buf->surfaceCache)) {
		/* Buffer is in system memory */
		if (!(buf->dc->flags & MGL_HW_SRCTRANS_BLT)) {
			surface = buf->surfaceCache;
			bytesPerLine = buf->cacheBytesPerLine;
			}
		else {
			surface = buf->surface;
			bytesPerLine = buf->bytesPerLine;
			}
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
					dc->r.DstTransBltSys(surface,bytesPerLine,
						srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
						r.left,r.top,op,transparent,false);
					}
			END_CLIP_REGION;
			}
		else {
			dc->r.DstTransBltSys(surface,bytesPerLine,
				d.left,d.top,d.right-d.left,d.bottom-d.top,
				x + dc->a.viewPort.left,y + dc->a.viewPort.top,
				op,transparent,false);
			}
		}
	else {
		/* Buffer is in offscreen memory */
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
					if (buf->linearStart == -1) {
						dc->r.DstTransBltBuffer(buf,dc,
							srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
							r.left,r.top,op,transparent);
						}
					else {
						dc->r.DstTransBltLin(buf->linearStart,buf->bytesPerLine,
							srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
							r.left,r.top,op,transparent);
						}
					}
			END_CLIP_REGION;
			}
		else {
			if (buf->linearStart == -1) {
				dc->r.DstTransBltBuffer(buf,dc,
					d.left,d.top,d.right-d.left,d.bottom-d.top,
					x + dc->a.viewPort.left,y + dc->a.viewPort.top,
					op,transparent);
				}
			else {
				dc->r.DstTransBltLin(buf->linearStart,buf->bytesPerLine,
					d.left,d.top,d.right-d.left,d.bottom-d.top,
					x + dc->a.viewPort.left,y + dc->a.viewPort.top,
					op,transparent);
				}
			}
		}
	RESTORE_HARDWARE(dc,true);
}

/****************************************************************************
DESCRIPTION:
Copy a section of an offscreen buffer to the specified location with
source transparency.

HEADER:
mgraph.h

PARAMETERS:
dc			- Device context to copy buffer to
left		- Left coordinate of section to copy
top			- Top coordinate of section to copy
right		- Right coordinate of section to copy
bottom		- Bottom coordinate of section to copy
dstLeft		- Left coordinate of destination to copy buffer to
dstTop		- Right coordinate of destination to copy buffer to
buf			- Buffer to copy
transparent	- Transparent color for the bitmap
op			- Write mode to use when drawing buffer

REMARKS:
Copies a section of an offscreen buffer to the specified location on the
device context with either source or destination transparency.

When transferring the data with source transparency, pixels in the source
image that are equal to the specified transparent color, the related pixel
in the destination buffer will remain untouched. This allows you to quickly
transfer sprites between device contexts with a single color being allocated
as a transparent color.

Note:	This function will fail if you attempt to copy a buffer to a
		device context that is not the device context that the original
		buffer was allocated for.

SEE ALSO:
MGL_putBuffer, MGL_putBufferSrcTrans, MGL_putBufferDstTrans,
MGL_stretchBuffer, MGL_createBuffer, MGL_copyToBuffer,
MGL_copyBitmapToBuffer
****************************************************************************/
void MGLAPI MGL_putBufferSrcTransSection(
	MGLDC *dc,
	int left,
	int top,
	int right,
	int bottom,
	int dstLeft,
	int dstTop,
	MGLBUF *buf,
	color_t transparent,
	int op)
{
	rect_t	d,r,clip;
	int 	srcLeft,srcTop,srcRight,srcBottom,bytesPerLine;
	void	*surface;

	/* Check that the device context pointer is valid */
	CHECK(dc == buf->dc);
	if (dc == _MGL_dcPtr)
		dc = &DC;

	/* Clip the source rectangle to the bitmap dimensions */
	if (!MGL_sectRectCoord(0,0,buf->width,buf->height,left,top,right,bottom,&d))
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
	if (buf->surfaceCache == buf->surface || (!(buf->dc->flags & MGL_HW_SRCTRANS_BLT) && buf->surfaceCache)) {
		/* Buffer is in system memory */
		if (!(buf->dc->flags & MGL_HW_SRCTRANS_BLT)) {
			surface = buf->surfaceCache;
			bytesPerLine = buf->cacheBytesPerLine;
			}
		else {
			surface = buf->surface;
			bytesPerLine = buf->bytesPerLine;
			}
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
					dc->r.SrcTransBltSys(surface,bytesPerLine,
						srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
						r.left,r.top,op,transparent,false);
					}
			END_CLIP_REGION;
			}
		else {
			dc->r.SrcTransBltSys(surface,bytesPerLine,
				left,top,right-left,bottom-top,
				dstLeft + dc->a.viewPort.left,dstTop + dc->a.viewPort.top,
				op,transparent,false);
			}
		}
	else {
		/* Buffer is in offscreen memory */
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
					if (buf->linearStart == -1) {
						dc->r.SrcTransBltBuffer(buf,dc,
							srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
							r.left,r.top,op,transparent);
						}
					else {
						dc->r.SrcTransBltLin(buf->linearStart,buf->bytesPerLine,
							srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
							r.left,r.top,op,transparent);
						}
					}
			END_CLIP_REGION;
			}
		else {
			if (buf->linearStart == -1) {
				dc->r.SrcTransBltBuffer(buf,dc,
					left,top,right-left,bottom-top,
					dstLeft + dc->a.viewPort.left,dstTop + dc->a.viewPort.top,
					op,transparent);
				}
			else {
				dc->r.SrcTransBltLin(buf->linearStart,buf->bytesPerLine,
					left,top,right-left,bottom-top,
					dstLeft + dc->a.viewPort.left,dstTop + dc->a.viewPort.top,
					op,transparent);
				}
			}
		}
	RESTORE_HARDWARE(dc,true);
}

/****************************************************************************
DESCRIPTION:
Copy a section of an offscreen buffer to the specified location with
destination transparency.

HEADER:
mgraph.h

PARAMETERS:
dc			- Device context to copy buffer to
left		- Left coordinate of section to copy
top			- Top coordinate of section to copy
right		- Right coordinate of section to copy
bottom		- Bottom coordinate of section to copy
dstLeft		- Left coordinate of destination to copy buffer to
dstTop		- Right coordinate of destination to copy buffer to
buf			- Buffer to copy
transparent	- Transparent color for the bitmap
sourceTrans	- True for source transparency, false for destination transparency
op			- Write mode to use when drawing buffer

REMARKS:
Copies a section of an offscreen buffer to the specified location on the
device context with either source or destination transparency.

When transferring the data with destination transparency, pixels in the
destination image that are equal to the specified transparent color will be
updated, and those pixels that are not the same will be skipped. This is
effectively the operation performed for 'blueScreen'ing or color keying and
can also be used for drawing transparent sprites. Note however that
destination transparency is very slow in software compared to source
transparency!

Note:	This function will fail if you attempt to copy a buffer to a
		device context that is not the device context that the original
		buffer was allocated for.

SEE ALSO:
MGL_putBuffer, MGL_putBufferSrcTrans, MGL_putBufferDstTrans,
MGL_stretchBuffer, MGL_createBuffer, MGL_copyToBuffer,
MGL_copyBitmapToBuffer
****************************************************************************/
void MGLAPI MGL_putBufferDstTransSection(
	MGLDC *dc,
	int left,
	int top,
	int right,
	int bottom,
	int dstLeft,
	int dstTop,
	MGLBUF *buf,
	color_t transparent,
	int op)
{
	rect_t	d,r,clip;
	int 	srcLeft,srcTop,srcRight,srcBottom,bytesPerLine;
	void	*surface;

	/* Check that the device context pointer is valid */
	CHECK(dc == buf->dc);
	if (dc == _MGL_dcPtr)
		dc = &DC;

	/* Clip the source rectangle to the bitmap dimensions */
	if (!MGL_sectRectCoord(0,0,buf->width,buf->height,left,top,right,bottom,&d))
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
	if (buf->surfaceCache == buf->surface || (!(buf->dc->flags & MGL_HW_SRCTRANS_BLT) && buf->surfaceCache)) {
		/* Buffer is in system memory */
		if (!(buf->dc->flags & MGL_HW_SRCTRANS_BLT)) {
			surface = buf->surfaceCache;
			bytesPerLine = buf->cacheBytesPerLine;
			}
		else {
			surface = buf->surface;
			bytesPerLine = buf->bytesPerLine;
			}
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
					dc->r.DstTransBltSys(surface,bytesPerLine,
						srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
						r.left,r.top,op,transparent,false);
					}
			END_CLIP_REGION;
			}
		else {
			dc->r.DstTransBltSys(surface,bytesPerLine,
				left,top,right-left,bottom-top,
				dstLeft + dc->a.viewPort.left,dstTop + dc->a.viewPort.top,
				op,transparent,false);
			}
		}
	else {
		/* Buffer is in offscreen memory */
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
					if (buf->linearStart == -1) {
						dc->r.DstTransBltBuffer(buf,dc,
							srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
							r.left,r.top,op,transparent);
						}
					else {
						dc->r.DstTransBltLin(buf->linearStart,buf->bytesPerLine,
							srcLeft,srcTop,srcRight-srcLeft,srcBottom-srcTop,
							r.left,r.top,op,transparent);
						}
					}
			END_CLIP_REGION;
			}
		else {
			if (buf->linearStart == -1) {
				dc->r.DstTransBltBuffer(buf,dc,
					left,top,right-left,bottom-top,
					dstLeft + dc->a.viewPort.left,dstTop + dc->a.viewPort.top,
					op,transparent);
				}
			else {
				dc->r.DstTransBltLin(buf->linearStart,buf->bytesPerLine,
					left,top,right-left,bottom-top,
					dstLeft + dc->a.viewPort.left,dstTop + dc->a.viewPort.top,
					op,transparent);
				}
			}
		}
	RESTORE_HARDWARE(dc,true);
}

/****************************************************************************
DESCRIPTION:
Stretches an offscreen buffer to the specified rectangle.

HEADER:
mgraph.h

PARAMETERS:
dc			- Device context to copy bufer to
dstLeft		- Left coordinate to stretch buffer to
dstTop		- Top coordinate to stretch buffer to
dstRight	- Right coordinate to stretch buffer to
dstBottom	- Bottom coordinate to stretch buffer to
buf			- Buffer to display
op			- Write mode to use when drawing buffer

REMARKS:
Stretches an offscreen buffer to the destination rectangle on the specififed
device context.

Note:	This function will fail if you attempt to copy a buffer to a
		device context that is not the device context that the original
		buffer was allocated for.

SEE ALSO:
MGL_putBuffer, MGL_putBufferSrcTrans, MGL_putBufferDstTranst,
MGL_stretchBufferSection, MGL_createBuffer, MGL_copyToBuffer,
MGL_copyBitmapToBuffer
****************************************************************************/
void MGLAPI MGL_stretchBuffer(
	MGLDC *dc,
	int dstLeft,
	int dstTop,
	int dstRight,
	int dstBottom,
	MGLBUF *buf,
	int op)
{
	rect_t	d,r,clip;
	int		left = 0,top = 0,right = buf->width,bottom = buf->height;
	int		bytesPerLine;
	void	*surface;
	ibool   clipIt = false;

	/* Handle special case of 1:1 stretch */
	if (((right - left) == (dstRight - dstLeft)) &&
		((bottom - top) == (dstBottom - dstTop))) {
		MGL_putBuffer(dc,dstLeft,dstTop,buf,op);
		return;
		}

	/* Check that the device context pointer is valid */
	CHECK(dc == buf->dc);
	if (dc == _MGL_dcPtr)
		dc = &DC;

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
	if (buf->surfaceCache == buf->surface || (!(buf->dc->flags & MGL_HW_STRETCH_BLT) && buf->surfaceCache)) {
		/* Buffer is in system memory */
		if (!(buf->dc->flags & MGL_HW_STRETCH_BLT)) {
			surface = buf->surfaceCache;
			bytesPerLine = buf->cacheBytesPerLine;
			}
		else {
			surface = buf->surface;
			bytesPerLine = buf->bytesPerLine;
			}
		if (dc->a.clipRegion) {
			d.left = dstLeft + dc->a.viewPort.left;
			d.top = dstTop + dc->a.viewPort.top;
			d.right = dstRight + dc->a.viewPort.left;
			d.bottom = dstBottom + dc->a.viewPort.top;
			BEGIN_CLIP_REGION(clip,dc->intClipRegion);
				if (MGL_sectRect(clip,d,&r)) {
					dc->r.StretchBltSys(surface,bytesPerLine,
						left,top,right-left,bottom-top,
						d.left,d.top,d.right-d.left,d.bottom-d.top,clipIt,
						r.left,r.top,r.right,r.bottom,op,false);
					}
			END_CLIP_REGION;
			}
		else {
			dc->r.StretchBltSys(surface,bytesPerLine,
				left,top,right-left,bottom-top,
				dstLeft + dc->a.viewPort.left,dstTop + dc->a.viewPort.top,
				dstRight-dstLeft,dstBottom-dstTop,clipIt,
				dc->intClipRect.left,dc->intClipRect.top,
				dc->intClipRect.right,dc->intClipRect.bottom,
				op,false);
			}
		}
	else {
		/* Buffer is in offscreen memory */
		if (dc->a.clipRegion) {
			d.left = dstLeft + dc->a.viewPort.left;
			d.top = dstTop + dc->a.viewPort.top;
			d.right = dstRight + dc->a.viewPort.left;
			d.bottom = dstBottom + dc->a.viewPort.top;
			BEGIN_CLIP_REGION(clip,dc->intClipRegion);
				if (MGL_sectRect(clip,d,&r)) {
					dc->r.StretchBuffer(buf,dc,
						left,top,right-left,bottom-top,
						d.left,d.top,d.right-d.left,d.bottom-d.top,clipIt,
						r.left,r.top,r.right,r.bottom,op);
					}
			END_CLIP_REGION;
			}
		else {
			dc->r.StretchBuffer(buf,dc,
				left,top,right-left,bottom-top,
				dstLeft + dc->a.viewPort.left,dstTop + dc->a.viewPort.top,
				dstRight-dstLeft,dstBottom-dstTop,clipIt,
				dc->intClipRect.left,dc->intClipRect.top,
				dc->intClipRect.right,dc->intClipRect.bottom,op);
			}
		}
	RESTORE_HARDWARE(dc,true);
}

/****************************************************************************
DESCRIPTION:
Stretches a section of an offscreen buffer to the specified device context.

HEADER:
mgraph.h

PARAMETERS:
dc			- Device context to copy buffer to
left		- Left coordinate of section to stretch
top			- Top coordinate of section to stretch
right		- Right coordinate of section to stretch
bottom		- Bottom coordinate of section to stretch
dstLeft		- Left coordinate to stretch buffer to
dstTop		- Top coordinate to stretch buffer to
dstRight	- Right coordinate to stretch buffer to
dstBottom	- Bottom coordinate to stretch buffer to
buf			- Buffer to display
op			- Write mode to use when drawing buffer

REMARKS:
Stretches a section of an offscreen buffer to the destination rectangle on
the specified device context.

Note:	This function will fail if you attempt to copy a buffer to a
		device context that is not the device context that the original
		buffer was allocated for.

SEE ALSO:
MGL_putBuffer, MGL_putBufferSrcTrans, MGL_putBufferDstTrans,
MGL_stretchBuffer, MGL_createBuffer, MGL_copyToBuffer,
MGL_copyBitmapToBuffer
****************************************************************************/
void MGLAPI MGL_stretchBufferSection(
	MGLDC *dc,
	int left,
	int top,
	int right,
	int bottom,
	int dstLeft,
	int dstTop,
	int dstRight,
	int dstBottom,
	MGLBUF *buf,
	int op)
{
	rect_t	d,r,clip;
	int		deltaSrc,deltaDst;
	fix32_t	zoomx,zoomy;
	ibool   clipIt = false;
	int 	fdTop,fdLeft,bytesPerLine;
	void	*surface;;

	/* Handle special case of 1:1 stretch */
	if (((right - left) == (dstRight - dstLeft)) &&
		((bottom - top) == (dstBottom - dstTop))) {
		MGL_putBufferSection(dc,left,top,right,bottom,dstLeft,dstTop,buf,op);
		return;
		}

	/* Check that the device context pointer is valid */
	CHECK(dc == buf->dc);
	if (dc == _MGL_dcPtr)
		dc = &DC;

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

	/* Clip the source rectangle to the bitmap dimensions */
	d.left = left;	d.top = top;
	d.right = MIN(right, buf->width);
	d.bottom = MIN(bottom, buf->height);
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
	if (buf->surfaceCache == buf->surface || (!(buf->dc->flags & MGL_HW_STRETCH_BLT) && buf->surfaceCache)) {
		/* Buffer is in system memory */
		if (!(buf->dc->flags & MGL_HW_STRETCH_BLT)) {
			surface = buf->surfaceCache;
			bytesPerLine = buf->cacheBytesPerLine;
			}
		else {
			surface = buf->surface;
			bytesPerLine = buf->bytesPerLine;
			}
		if (dc->a.clipRegion) {
			d.left = dstLeft + dc->a.viewPort.left;
			d.top = dstTop + dc->a.viewPort.top;
			d.right = dstRight + dc->a.viewPort.left;
			d.bottom = dstBottom + dc->a.viewPort.top;
			BEGIN_CLIP_REGION(clip,dc->intClipRegion);
				if (MGL_sectRect(clip,d,&r)) {
					dc->r.StretchBltSys(surface,bytesPerLine,
						left,top,right-left,bottom-top,
						d.left,d.top,d.right-d.left,d.bottom-d.top,clipIt,
						r.left,r.top,r.right,r.bottom,op,false);
					}
			END_CLIP_REGION;
			}
		else {
			dc->r.StretchBltSys(surface,bytesPerLine,
				left,top,right-left,bottom-top,
				dstLeft + dc->a.viewPort.left,dstTop + dc->a.viewPort.top,
				dstRight-dstLeft,dstBottom-dstTop,clipIt,
				dc->intClipRect.left,dc->intClipRect.top,
				dc->intClipRect.right,dc->intClipRect.bottom,
				op,false);
			}
		}
	else {
		/* Buffer is in offscreen memory */
		if (dc->a.clipRegion) {
			d.left = dstLeft + dc->a.viewPort.left;
			d.top = dstTop + dc->a.viewPort.top;
			d.right = dstRight + dc->a.viewPort.left;
			d.bottom = dstBottom + dc->a.viewPort.top;
			BEGIN_CLIP_REGION(clip,dc->intClipRegion);
				if (MGL_sectRect(clip,d,&r)) {
					dc->r.StretchBuffer(buf,dc,
						left,top,right-left,bottom-top,
						d.left,d.top,d.right-d.left,d.bottom-d.top,clipIt,
						r.left,r.top,r.right,r.bottom,op);
					}
			END_CLIP_REGION;
			}
		else {
			dc->r.StretchBuffer(buf,dc,
				left,top,right-left,bottom-top,
				dstLeft + dc->a.viewPort.left,dstTop + dc->a.viewPort.top,
				dstRight-dstLeft,dstBottom-dstTop,clipIt,
				dc->intClipRect.left,dc->intClipRect.top,
				dc->intClipRect.right,dc->intClipRect.bottom,
				op);
			}
		}
	RESTORE_HARDWARE(dc,true);
}
