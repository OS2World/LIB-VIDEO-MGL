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
* Description:	Header file for the 8 bit accelerated driver.
*
****************************************************************************/

#ifndef	__DRIVERS_OPENGL_OPENGL_H
#define	__DRIVERS_OPENGL_OPENGL_H

#ifndef	__DRIVERS_COMMON_GDDRAW_H
#include "drivers/common/gddraw.h"
#endif

/*------------------------- Function Prototypes ---------------------------*/

ibool 	MGLAPI OPENGL_detect(int id,int *force,int *driver,int *mode,modetab availableModes);
ibool 	MGLAPI OPENGL_initDriver(MGLDC *dc,int driverId,int modeId,ulong hwnd,int virtualX,int virtualY,int numBuffers,ibool stereo,int refreshRate,ibool useLinearBlits);

/* OpenGL surface compatible device rendering functions */
void 	MGLAPI OPENGL_setColor(color_t color);
void 	MGLAPI OPENGL_setBackColor(color_t color);
void 	MGLAPI OPENGL_drawStrBitmap(int x,int y,const char *str);
void    MGLAPI OPENGL_putMonoImage(MGLDC *dc,int x,int y,int byteWidth,int height,uchar *image);
void	MGLAPI OPENGL_getScanLine(MGLDC *dc,int y,int x1,int x2,void *buffer,int bufx,int op);
void	MGLAPI OPENGL_putScanLine(MGLDC *dc,int y,int x1,int x2,void *buffer,int bufx,int op);
void 	MGLAPI OPENGL_getImage(MGLDC *dc,int left,int top,int right,int bottom,int dstLeft,int dstTop,int op,void *surface,int bytesPerLine);
void	MGLAPI OPENGL_putImage(MGLDC *dc,int left,int top,int right,int bottom,int dstLeft,int dstTop,int op,void *surface,int bytesPerLine, MGLDC *src);
void 	MGLAPI OPENGL_stretchBlt1x2(MGLDC *dst,MGLDC *src,int left,int top,int right,int bottom,int dstLeft,int dstTop,palette_t *pal,ibool idPal);
void 	MGLAPI OPENGL_stretchBlt2x2(MGLDC *dst,MGLDC *src,int left,int top,int right,int bottom,int dstLeft,int dstTop,palette_t *pal,ibool idPal);
void 	MGLAPI OPENGL_stretchBlt(MGLDC *dst,MGLDC *src,int left,int top,int right,int bottom,int dstLeft,int dstTop,fix32_t zoomx,fix32_t zoomy,palette_t *pal,ibool idPal);

#endif	/* __DRIVERS_OPENGL_OPENGL_H */

