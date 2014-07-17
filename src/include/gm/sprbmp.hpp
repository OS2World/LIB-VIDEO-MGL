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
* Description:  Header for the internal SPR_Bitmap class hierarchy.
*
****************************************************************************/

#ifndef __SPRBMP_HPP
#define __SPRBMP_HPP

#include "mgraph.hpp"
#include "gm/dlist.hpp"

/*-------------------------- Class Definitions ----------------------------*/

//---------------------------------------------------------------------------
// This is the abstract base class for all bitmap objects. All bitmaps
// have an associated bitmap stored in host memory, which may optionally
// be destroyed when the bitmap is downloaded to the hardware.
//---------------------------------------------------------------------------

class SPR_Bitmap : public FX_DListNode {
protected:
	MGLBUF	*buf;		// MGL buffer containing the bitmap

public:
            // Constructor
			SPR_Bitmap(MGLDC *dc,bitmap_t *bmp);
			SPR_Bitmap(MGLDC *dc,MGLDC *srcDC,int left,int top,int right,int bottom);

            // Destructor
    virtual ~SPR_Bitmap();

            // Draw the bitmap at the specified location
	virtual void draw(int x,int y,int op = MGL_REPLACE_MODE);

			// Draw a section of the bitmap at the specififed location
	virtual void draw(int left,int top,int right,int bottom,int dstLeft,int dstTop,int op = MGL_REPLACE_MODE);

			// Determine if the bitmap was constructed properly
	virtual ibool isValid();
    };

typedef FX_DList<SPR_Bitmap>          SPR_BitmapList;
typedef FX_DListIterator<SPR_Bitmap>  SPR_BitmapListIter;

//---------------------------------------------------------------------------
// Source transparent bitmaps
//---------------------------------------------------------------------------

class SPR_TransparentBitmap : public SPR_Bitmap {
protected:
	color_t	transparent;    // Transparent color

public:
			// Constructor
			SPR_TransparentBitmap(MGLDC *dc,bitmap_t *bmp,color_t transparent);
			SPR_TransparentBitmap(MGLDC *dc,MGLDC *srcDC,int left,int top,int right,int bottom,color_t transparent);

			// Draw the bitmap at the specified location
	virtual void draw(int x,int y,int op = MGL_REPLACE_MODE);

			// Draw a section of the bitmap at the specififed location
	virtual void draw(int left,int top,int right,int bottom,int dstLeft,int dstTop,int op = MGL_REPLACE_MODE);
	};

#endif  // __SPRBMP_HPP
