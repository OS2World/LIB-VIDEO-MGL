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
*                          Based on original code
*                   Copyright (c) 1996-95 ATI Technolgies.
*
* Language:     C++ 3.0
* Environment:	Any
*
* Description:  Header for the FX_Plane class.
*
****************************************************************************/

#ifndef __FXPLANE_HPP
#define __FXPLANE_HPP

#ifndef __FXSPRITE_HPP
#include "fxsprite.hpp"
#endif

/*-------------------------- Class Definitions ----------------------------*/

//---------------------------------------------------------------------------
// Class representing a scrolling plane, which built up of a number of
// bitmap tiles. Tiles may be marked missing from parts of the plane.
//---------------------------------------------------------------------------

class FX_Plane {
protected:
    SPR_Bitmap	**hBM;
    fix32_t     x;
    fix32_t     y;
    ushort      width;
    ushort      height;
    fix32_t     xv;
    fix32_t     xslide;
    fix32_t     xincrem;
    ushort      denom;

public:
            // Constructor
            FX_Plane(ushort width,ushort height,ushort denom,
                const char *tileListName,SPR_Bitmap *bitmapList[]);

            // Destructor
    virtual ~FX_Plane();

            // Set plane position etc
			void SetX(fix32_t x,FB_POSITION position);
			void SetY(fix32_t y,FB_POSITION position);
			void SetSlideX(fix32_t xs,FB_POSITION position);
			void SetVelocityX(fix32_t xv,FB_POSITION position);
			void SetIncremX(fix32_t xinc,FB_POSITION position);

            // Display the plane
            void Display();

            // Get info
            fix32_t X() const       { return x; };
            fix32_t Y() const       { return y; };
            ushort Width() const    { return width; };
            ushort Height() const   { return height; };
            fix32_t SlideX() const  { return xslide; };
            fix32_t IncremX() const { return xincrem; };
    };

//---------------------------------------------------------------------------
// Class representing a scrolling plane, which also has an attached surface
// list to represent areas that characters sprites like the fox can walk
// on.
//---------------------------------------------------------------------------

class FX_SurfacePlane : public FX_Plane {
protected:
    ibool        *surface;

public:
            // Constructor
            FX_SurfacePlane(ushort width,ushort height,ushort denom,
                const char *tileListName,SPR_Bitmap *bitmapList[],
                const char *surfaceListName);

            // Destructor
    virtual ~FX_SurfacePlane();

            // Keep the sprite on the plane surface
            void KeepOnSurface(FX_Sprite *sprite);

            // Return true if sprite is on plane surface
            ibool OnSurface(const FX_Sprite *sprite);
    };

#endif  // __FXPLANE_HPP
