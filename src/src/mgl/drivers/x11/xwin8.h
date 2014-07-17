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
* Environment:	Unix/X11
*
* Description:	Header file for the 8-bit X11 driver.
*
****************************************************************************/

#ifndef	__DRIVERS_UNIX_XWIN8_H
#define	__DRIVERS_UNIX_XWIN8_H

#include "mglunix.h"
#include "mgldd.h"

#ifndef	__DRIVERS_PACKED_PACKED8_H
#include "drivers/packed/packed8.h"
#endif

#include "drivers/common/gunix.h"

typedef struct {
	// What the hell do I have to put here ?
	Display *dpy;
	Window  w;
	//	XImage  *img;
} XWIN8_data;

/*------------------------- Function Prototypes ---------------------------*/

/* 8 bit C based routines */

ibool 	MGLAPI XWIN8_detect(void *data,int id,int force,int *driver,int *mode,modetab availableModes);
ibool 	MGLAPI XWIN8_initDriver(void *data,MGLDC *dc,int driverId,int modeId,ulong hwnd,int virtualX,int virtualY,int numBuffers,ibool stereo,int refreshRate,ibool useLinearBlits);
void   *MGLAPI XWIN8_createInstance(void);
void    MGLAPI XWIN8_destroyInstance(void *data);

void MGLAPI XWIN8_bitBlt(MGLDC *dc,int left,int top,int right,int bottom, int dstLeft,int dstTop,int op,int incx,int incy);
color_t MGLAPI XWIN8_getPixel(int x, int y);
void MGLAPI XWIN8_putPixel(int x, int y);
ulong   MGLAPI XWIN8_getWinDC(MGLDC *dc);
void *  MGLAPI XWIN8_getDefaultPalette(MGLDC *dc);
void    MGLAPI XWIN8_realizePalette(MGLDC *dc,palette_t *pal,int num,int index,int waitVRT);
void MGLAPI  XWIN8_putImage(MGLDC *dc,int left,int top,int right,
	int bottom,int dstLeft,int dstTop,int op,void *surface,
	int bytesPerLine,MGLDC *src);

#endif /* __DRIVERS_UNIX_XWIN8_H */
