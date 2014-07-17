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
* Description:  SPR_BitmapManager class, which manages all bitmaps in
*               host memory and offscreen display memory.
*
****************************************************************************/

#include "gm/sprmgr.hpp"

/*------------------------- Implementation --------------------------------*/

/****************************************************************************
PARAMETERS:
dc          	- Device context for drawing all bitmaps to

REMARKS:
Constructor for the sprite bitmap manager class.
****************************************************************************/
SPR_BitmapManager::SPR_BitmapManager(
	MGLDC *dc)
	: dc(dc)
{
	empty();                    // Empty bitmap manager and offscreen mem
}

/****************************************************************************
REMARKS:
Empties the bitmap manager and destroys all bitmaps, allowing a new set of
bitmaps to be loaded.
****************************************************************************/
void SPR_BitmapManager::empty()
{
	bitmaps.empty();
}

/****************************************************************************
REMARKS:
Destroys a bitmap in the sprite manager.
****************************************************************************/
void SPR_BitmapManager::destroyBitmap(
	SPR_Bitmap *bmp)
{
	bitmaps.remove(bmp);
	delete bmp;
}

/****************************************************************************
PARAMETERS:
bmp	- Bitmap to add to bitmap manager

RETURNS:
Pointer to loaded bitmap, or NULL on error.

REMARKS:
Adds the opaque bitmap to the bitmap manager for later use. If the bitmap
is cached in offscreen memory it is downloaded to the hardware.
****************************************************************************/
SPR_Bitmap *SPR_BitmapManager::addOpaqueBitmap(
	bitmap_t *bmp)
{
	SPR_Bitmap	*bitmap = new SPR_Bitmap(dc,bmp);

	if (bitmap) {
		if (bitmap->isValid())
			bitmaps.addToTail(bitmap);
		else {
			delete bitmap;
			bitmap = NULL;
			}
		}
	return bitmap;
}

/****************************************************************************
PARAMETERS:
bmp	- Bitmap to add to bitmap manager

RETURNS:
Pointer to loaded bitmap, or NULL on error.

REMARKS:
Adds the opaque bitmap to the bitmap manager for later use. If the bitmap
is cached in offscreen memory it is downloaded to the hardware.
****************************************************************************/
SPR_Bitmap *SPR_BitmapManager::addOpaqueBitmap(
	MGLDC *srcDC,
	int left,
	int top,
	int right,
	int bottom)
{
	SPR_Bitmap	*bitmap = new SPR_Bitmap(dc,srcDC,left,top,right,bottom);

	if (bitmap) {
		if (bitmap->isValid())
			bitmaps.addToTail(bitmap);
		else {
			delete bitmap;
			bitmap = NULL;
			}
		}
	return bitmap;
}

/****************************************************************************
PARAMETERS:
bmp			- Bitmap to add to bitmap manager
transparent - Transparent color for the bitmap

RETURNS:
Pointer to loaded bitmap, or NULL on error.

REMARKS:
Attempts to load the specified bitmap, and will create the appropriate
bitmap given the current hardware characteristics. This function creates
a bitmap that will be drawn as transparent.
****************************************************************************/
SPR_Bitmap *SPR_BitmapManager::addTransparentBitmap(
	bitmap_t *bmp,
	color_t transparent)
{
	SPR_Bitmap	*bitmap = new SPR_TransparentBitmap(dc,bmp,transparent);

	if (bitmap) {
		if (bitmap->isValid())
			bitmaps.addToTail(bitmap);
		else {
			delete bitmap;
			bitmap = NULL;
			}
		}
	return bitmap;
}

/****************************************************************************
PARAMETERS:
bmp	- Bitmap to add to bitmap manager

RETURNS:
Pointer to loaded bitmap, or NULL on error.

REMARKS:
Adds the opaque bitmap to the bitmap manager for later use. If the bitmap
is cached in offscreen memory it is downloaded to the hardware.
****************************************************************************/
SPR_Bitmap *SPR_BitmapManager::addTransparentBitmap(
	MGLDC *srcDC,
	int left,
	int top,
	int right,
	int bottom,
	color_t transparent)
{
	SPR_Bitmap	*bitmap = new SPR_TransparentBitmap(dc,srcDC,left,top,right,bottom,transparent);

	if (bitmap) {
		if (bitmap->isValid())
			bitmaps.addToTail(bitmap);
		else {
			delete bitmap;
			bitmap = NULL;
			}
		}
	return bitmap;
}
