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
* Description:  Code to implement the Apple.
*
****************************************************************************/

#include "fbpch.hpp"
#pragma hdrstop

/*------------------------------ Implementation ---------------------------*/

void CreateApple()
/****************************************************************************
*
* Function:     CreateApple
*
* Description:  Creates the apple sprite with all it's associated bitmaps.
*
****************************************************************************/
{
	apple = new FX_Sprite(1, C_APPLE_STARTX, C_APPLE_STARTY,
        C_APPLE_SIZEX, C_APPLE_SIZEY, C_FORE_W * C_TILE_W,
        C_FORE_H * C_TILE_H, 0, false);
	apple->AddBitmap(bitmapList[61],FB_NONE,FB_RIGHT,32,32,0,0);
	apple->SetAction(FB_NONE, FB_RIGHT);
}

void UpdateApple()
/****************************************************************************
*
* Function:     UpdateApple
*
* Description:  Update the apple sprite for the next frame
*
****************************************************************************/
{
    if ((apple->VelocityX() != 0) && (apple->X() >= 420 * C_UNIT)) {
		apple->SetX(0, FB_ABSOLUTE);
		apple->SetY(0, FB_ABSOLUTE);
		apple->SetAccelerationX(0, FB_ABSOLUTE);
		apple->SetAccelerationY(0, FB_ABSOLUTE);
		apple->SetVelocityX(0, FB_ABSOLUTE);
		apple->SetVelocityY(0, FB_ABSOLUTE);
        apple->SetActive(FALSE);
        }
}

