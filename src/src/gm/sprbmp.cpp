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
* Language:		ANSI C++
* Environment:	Any
*
* Description:  Implementation for the SPR_Bitmap class hierarchy.
*
****************************************************************************/

#include "gm/sprmgr.hpp"

/*------------------------- Implementation --------------------------------*/

/****************************************************************************
PARAMETERS:
dc	- MGL device context to use
bmp	- Bitmap to load into the buffer

REMARKS:
This constructor creates a bitmap classes object from a passed in bitmap_t
object. The contents of the bitmap_t object are copied to the MGL buffer,
so you can destroy the bitmap after you have loaded it into the sprite
manager.
***************************************************************************/
SPR_Bitmap::SPR_Bitmap(
	MGLDC *dc,
	bitmap_t *bmp)
{
	if ((buf = MGL_createBuffer(dc,bmp->width,bmp->height,true)) != NULL) {
		MGL_copyBitmapToBuffer(bmp,0,0,bmp->width,bmp->height,0,0,buf);
		}
}

/****************************************************************************
PARAMETERS:
dc		- MGL device context to use
srcDC	- Source device context to copy bitmap data from
left	- Left coordinate of area to copy to bitmap
top     - Top coordinate of area to copy to bitmap
right   - Right coordinate of area to copy to bitmap
bottom  - Bottom coordinate of area to copy to bitmap

REMARKS:
This constructor creates a bitmap classes object. This version copies the
bitmap data for the bitmap from an existing MGL device context. You can
use this function to load bitmaps into a memory device context (and color
convert them), and then add them to the sprite manager.
***************************************************************************/
SPR_Bitmap::SPR_Bitmap(
	MGLDC *dc,
	MGLDC *srcDC,
	int left,
	int top,
	int right,
	int bottom)
{
	int width = right - left;
	int height = bottom - top;

	if ((buf = MGL_createBuffer(dc,width,height,true)) != NULL) {
		MGL_copyToBuffer(srcDC,left,top,right,bottom,0,0,buf);
		}
}

/****************************************************************************
REMARKS:
Destructor for the bitmap class.
***************************************************************************/
SPR_Bitmap::~SPR_Bitmap()
{
	MGL_destroyBuffer(buf);
}

/****************************************************************************
PARAMETERS:
x	- X coordinate to draw bitmap at
y	- Y coordinate to draw bitmap at
op	- Write mode to draw with

REMARKS:
This member function draws the bitmap at the specified location, with the
specified raster operation.
***************************************************************************/
void SPR_Bitmap::draw(
	int x,
	int y,
	int op)
{
	MGL_putBuffer(buf->dc,x,y,buf,op);
}

/****************************************************************************
PARAMETERS:
left	- Left coordinate of area to copy from bitmap
top     - Top coordinate of area to copy from bitmap
right   - Right coordinate of area to copy from bitmap
bottom  - Bottom coordinate of area to copy from bitmap
dstLeft	- X coordinate to draw bitmap at
dstTop	- Y coordinate to draw bitmap at
op		- Write mode to draw with

REMARKS:
This member function draws a section of the bitmap to the specified location,
with the specified raster operation.
***************************************************************************/
void SPR_Bitmap::draw(
	int left,
	int top,
	int right,
	int bottom,
	int dstLeft,
	int dstTop,
	int op)
{
	MGL_putBufferSection(buf->dc,left,top,right,bottom,dstLeft,dstTop,buf,op);
}

/****************************************************************************
RETURNS:
True if the bitmap object if valid.
***************************************************************************/
ibool SPR_Bitmap::isValid()
{
	return buf != NULL;
}

/****************************************************************************
PARAMETERS:
dc			- MGL device context to use
bmp			- Bitmap to load into the buffer
transparent	- Transparent color for the bitmap

REMARKS:
This constructor creates a bitmap classes object from a passed in bitmap_t
object. The contents of the bitmap_t object are copied to the MGL buffer,
so you can destroy the bitmap after you have loaded it into the sprite
manager.
***************************************************************************/
SPR_TransparentBitmap::SPR_TransparentBitmap(
	MGLDC *dc,
	bitmap_t *bmp,
	color_t transparent)
	: SPR_Bitmap(dc,bmp), transparent(transparent)
{
}

/****************************************************************************
PARAMETERS:
dc			- MGL device context to use
srcDC		- Source device context to copy bitmap data from
left		- Left coordinate of area to copy to bitmap
top     	- Top coordinate of area to copy to bitmap
right   	- Right coordinate of area to copy to bitmap
bottom  	- Bottom coordinate of area to copy to bitmap
transparent	- Transparent color for the bitmap

REMARKS:
This constructor creates a bitmap classes object. This version copies the
bitmap data for the bitmap from an existing MGL device context. You can
use this function to load bitmaps into a memory device context (and color
convert them), and then add them to the sprite manager.
***************************************************************************/
SPR_TransparentBitmap::SPR_TransparentBitmap(
	MGLDC *dc,
	MGLDC *srcDC,
	int left,
	int top,
	int right,
	int bottom,
	color_t transparent)
	: SPR_Bitmap(dc,srcDC,left,top,right,bottom), transparent(transparent)
{
}

/****************************************************************************
PARAMETERS:
x	- X coordinate to draw bitmap at
y	- Y coordinate to draw bitmap at
op	- Write mode to draw with

REMARKS:
This member function draws the bitmap at the specified location, with the
specified raster operation.
***************************************************************************/
void SPR_TransparentBitmap::draw(
	int x,
	int y,
	int op)
{
	MGL_putBufferSrcTrans(buf->dc,x,y,buf,transparent,op);
}

/****************************************************************************
PARAMETERS:
left	- Left coordinate of area to copy from bitmap
top     - Top coordinate of area to copy from bitmap
right   - Right coordinate of area to copy from bitmap
bottom  - Bottom coordinate of area to copy from bitmap
dstLeft	- X coordinate to draw bitmap at
dstTop	- Y coordinate to draw bitmap at
op		- Write mode to draw with

REMARKS:
This member function draws a section of the bitmap to the specified location,
with the specified raster operation.
***************************************************************************/
void SPR_TransparentBitmap::draw(
	int left,
	int top,
	int right,
	int bottom,
	int dstLeft,
	int dstTop,
	int op)
{
	MGL_putBufferSrcTransSection(buf->dc,left,top,right,bottom,dstLeft,dstTop,buf,transparent,op);
}
