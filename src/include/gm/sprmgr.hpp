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
* Language:     ANSI C++
* Environment:  Any
*
* Description:  Header for the SPR_BitmapManager class.
*
****************************************************************************/

#ifndef __GM_SPRMGR_HPP
#define __GM_SPRMGR_HPP

#include "gm/sprbmp.hpp"

/*-------------------------- Class Definitions ----------------------------*/

//---------------------------------------------------------------------------
// Class to manage a list of bitmaps used for a particular level. This class
// can create either opaque or transparent bitmap objects for the level.
// If the hardware supports accelerated bitmap rendering it will cache as
// many of those in an MGL offscreen memory DC as possible. If there is
// no hardware support (or we have run out of offscreen display memory)
// bitmaps will be created and rendered in software.
//
// This class also manages all the bitmaps in the level, and will
// automatically delete all the bitmaps when the bitmap manager is emptied.
//---------------------------------------------------------------------------

class SPR_BitmapManager {
protected:
	MGLDC           *dc;            // Main MGL display DC to draw onto
	SPR_BitmapList	bitmaps;        // List of all loaded bitmaps for level

public:
			// Constructor
			SPR_BitmapManager(MGLDC *dc);

			// Destructor
			~SPR_BitmapManager() {};

			// Add a new opaque bitmap object to the bitmap manager
			SPR_Bitmap *addOpaqueBitmap(bitmap_t *bmp);
			SPR_Bitmap *addOpaqueBitmap(MGLDC *srcDC,int left,int top,int right,int bottom);

			// Add a new source transparent bitmap object to the bitmap manager
			SPR_Bitmap *addTransparentBitmap(bitmap_t *bmp,color_t transparent);
			SPR_Bitmap *addTransparentBitmap(MGLDC *srcDC,int left,int top,int right,int bottom,color_t transparent);

			// Destroy all bitmaps and start again
            void empty();

			// Destroy a bitmap in the bitmap manager
			void destroyBitmap(SPR_Bitmap *bmp);
	};

#endif  // __GM_SPRMGR_HPP
