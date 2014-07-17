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
* Description:  Game Framework Sprite Manager for SciTech MGL. This library
* 				provides a simple yet powerful bitmap sprite manager for
*				automatically managing both software and hardware sprites
*				within the MGL and Game Framework. The sprite manager
*				is completely stand-alone and can be used independanly of
*				the main Game Framework functions.
*
****************************************************************************/

#ifndef	__SPRITE_H
#define	__SPRITE_H

#include "mgraph.h"

/*------------------------- Function Prototypes ---------------------------*/

#ifdef	__cplusplus
#include "gm/sprmgr.hpp"
extern "C" {			/* Use "C" linkage when in C++ mode	*/
/* {secret} */
typedef SPR_Bitmap					SPR_bitmap;
/* {secret} */
typedef SPR_BitmapManager			SPR_bitmapManager;
#else
/* {secret} */
typedef struct SPR_Bitmap			SPR_bitmap;
/* {secret} */
typedef struct SPR_BitmapManager	SPR_bitmapManager;
#endif

/*---------------------------------------------------------------------------
 * Initialize the sprite manager.
 *-------------------------------------------------------------------------*/

SPR_bitmapManager * MGLAPI SPR_mgrInit(MGLDC *dc);

/*---------------------------------------------------------------------------
 * Destroys the sprite manager. You should call this on cleanup.
 *-------------------------------------------------------------------------*/

void	MGLAPI SPR_mgrExit(SPR_bitmapManager *mgr);

/*---------------------------------------------------------------------------
 * Empty the sprite manager of all bitmaps. Useful to destroy all bitmaps
 * so you can reload a new set such as when changing levels.
 *-------------------------------------------------------------------------*/

void	MGLAPI SPR_mgrEmpty(SPR_bitmapManager *mgr);

/*---------------------------------------------------------------------------
 * Add a new opaque bitmap to the sprite manager and download it to the
 * hardware. This function returns an SPR_bitmap pointer than you can use
 * to draw the bitmap at a later date.
 *-------------------------------------------------------------------------*/

SPR_bitmap * MGLAPI SPR_mgrAddOpaqueBitmap(SPR_bitmapManager *mgr,bitmap_t *bmp);
SPR_bitmap * MGLAPI SPR_mgrAddOpaqueBitmapFromDC(SPR_bitmapManager *mgr,MGLDC *srcDC,int left,int top,int right,int bottom);

/*---------------------------------------------------------------------------
 * Add a new transparent bitmap to the sprite manager and download it to the
 * hardware. This function returns an SPR_bitmap pointer than you can use
 * to draw the bitmap at a later date. This function automatically
 * determines the most efficient way to store and draw the bitmap depending
 * on the underlying hardware configuration.
 *-------------------------------------------------------------------------*/

SPR_bitmap * MGLAPI SPR_mgrAddTransparentBitmap(SPR_bitmapManager *mgr,bitmap_t *bmp,color_t transparent);
SPR_bitmap * MGLAPI SPR_mgrAddTransparentBitmapFromDC(SPR_bitmapManager *mgr,MGLDC *srcDC,int left,int top,int right,int bottom,color_t transparent);

/*---------------------------------------------------------------------------
 * Function to destroy a bitmap and remove it from the sprite manager.
 *-------------------------------------------------------------------------*/

void 	MGLAPI SPR_destroyBitmap(SPR_bitmapManager *mgr,SPR_bitmap *bmp);

/*---------------------------------------------------------------------------
 * Draw the bitmap at the specified location. Since the SPR_bitmap is
 * actually a C++ class, C++ programs can simply call the C++ member
 * functions for the bitmap class directly to draw it.   
 *-------------------------------------------------------------------------*/

void	MGLAPI SPR_draw(SPR_bitmap *bmp,int x,int y);
void	MGLAPI SPR_drawSection(SPR_bitmap *bmp,int left,int top,int right,int bottom,int dstLeft,int dstTop);
void	MGLAPI SPR_drawExt(SPR_bitmap *bmp,int x,int y,int op);
void	MGLAPI SPR_drawSectionExt(SPR_bitmap *bmp,int left,int top,int right,int bottom,int dstLeft,int dstTop,int op);

#ifdef	__cplusplus
}						/* End of "C" linkage for C++	*/
#endif	/* __cplusplus */

#endif	/* __SPRITE_H */
