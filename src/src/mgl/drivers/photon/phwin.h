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
* Description:	Main PHWIN device driver header file. This file contains all
*		of the C language declarations and definitions for the
*		PHWIN device driver.
*
****************************************************************************/

#ifndef	__DRIVERS_QNX_PHWIN_H
#define	__DRIVERS_QNX_PHWIN_H

/*------------------------- Function Prototypes ---------------------------*/

/* PHWIN C based routines */

ibool 	MGLAPI PHWIN_initDriver(void *data,MGLDC *dc,int driverId,int modeId,ulong hwnd,int virtualX,int virtualY,int numBuffers,ibool stereo,int refreshRate,ibool useLinearBlits);
void *	MGLAPI PHWIN_createInstance(void);
void	MGLAPI PHWIN_destroyInstance(void *data);

int 	PH_getBitsPerPixel(MGLDC *dc);
void 	PH_findPixelFormat(MGLDC *dc);
ulong 	MGLAPI PH_getWinDC(MGLDC *dc);
ibool 	MGLAPI PH_noZbuf(MGLDC *dc);
void * 	MGLAPI PH_getDefaultPalette(MGLDC *dc);
void 	MGLAPI PH_realizePalette(MGLDC *dc,palette_t *pal,int num,int index,int waitVRT);
void	MGLAPI PH_putImage(MGLDC *dc,int left,int top,int right,int bottom,int dstLeft,int dstTop,int op,void *surface,int bytesPerLine, MGLDC *src);
void	MGLAPI PH_translateImage(MGLDC *dc,int left,int top,int right,int bottom,int dstLeft,int dstTop,int op,void *surface,int bytesPerLine,int bitsPerPixel,palette_t *pal,pixel_format_t *pf,MGLDC *src);
void 	MGLAPI PH_stretchBlt1x2(MGLDC *dst,MGLDC *src,int left,int top,int right,int bottom,int dstLeft,int dstTop,palette_t *pal,ibool idPal);
void 	MGLAPI PH_stretchBlt2x2(MGLDC *dst,MGLDC *src,int left,int top,int right,int bottom,int dstLeft,int dstTop,palette_t *pal,ibool idPal);
void 	MGLAPI PH_stretchBlt(MGLDC *dst,MGLDC *src,int left,int top,int right,int bottom,int dstLeft,int dstTop,fix32_t zoomx,fix32_t zoomy,palette_t *pal,ibool idPal);

#endif	/* __DRIVERS_QNX_PHWIN_H */

