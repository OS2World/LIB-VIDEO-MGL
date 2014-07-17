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
* Description:	Common header file to all DirectDraw display drivers.
*
*               This module interfaces with DirectDraw and handles all of
*				the common DirectDraw specific code for the DirectDraw
*				drivers.
*
****************************************************************************/

#ifndef	__DRIVERS_COMMON_GDDRAW_H
#define	__DRIVERS_COMMON_GDDRAW_H

#ifndef	__DRIVERS_COMMON_COMMON_H
#include "drivers/common/common.h"
#endif

/*---------------------- Macro and type definitions -----------------------*/

#pragma pack(1)				/* Pack structures to byte granularity		*/

/* The following defines the internal DDRAW device driver state buffer. A
 * copy of this is allocated for us in the device context created for
 * the device driver. This state buffer is used to maintain device specific
 * state information that is device context specific.
 */

typedef struct {
	display_vec	v;				/* Display device vectors and info		*/
	} DDRAW_state;

#pragma pack()

/*------------------------- Function Prototypes ---------------------------*/

ibool	MGLAPI DDRAW_detect(int *driver,char *driverId);
void 	MGLAPI DDRAW_useMode(modetab modes,int mode,int id,gmode_t *modeInfo);
ibool 	MGLAPI DDRAW_setMode(MGLDC *dc,int virtualX,int virtualY,int numBuffers,ibool stereo,int refreshRate);
void	MGLAPI DDRAW_setAccelVectors(MGLDC *dc);
void 	MGLAPI DDRAW_restoreTextMode(MGLDC *dc);
void 	MGLAPI DDRAW_makeCurrent(MGLDC *dc,ibool partial);
void 	MGLAPI DDRAW_setActivePage(MGLDC *dc,int page);
void 	MGLAPI DDRAW_setVisualPage(MGLDC *dc,int page,int waitVRT);
void 	MGLAPI DDRAW_realizePalette(MGLDC *dc,palette_t *pal,int num,int index,int waitVRT);
void 	MGLAPI DDRAW_vSync(MGLDC *dc);
ibool	MGLAPI DDRAW_makeOffscreenDC(MGLDC *dc);
ibool	MGLAPI DDRAW_makeLinearOffscreenDC(MGLDC *dc);
void 	* MGLAPI DDRAW_getDefaultPalette(MGLDC *dc);

/* Fullscreen hardware OpenGL interfacing */

ibool 	MGLAPI DDOPENGL_detect(void);
void 	MGLAPI DDOPENGL_useMode(modetab modes,int mode,int id,gmode_t *modeInfo);

/* Hardware device driver support routines */

void	MGLAPI DDRAW_beginDrawing(void);
void	MGLAPI DDRAW_endDrawing(void);
void	MGLAPI DDRAW_beginDirectAccess(void);
void	MGLAPI DDRAW_endDirectAccess(void);

/* Hardware rendering routines */

void	MGLAPI DDRAW_fillRect(int x1,int y1,int x2,int y2);
void	MGLAPI DDRAW_ropFillRect(int x1,int y1,int x2,int y2);
void 	MGLAPI DDRAW_bitBlt(MGLDC *dc,int left,int top,int right,int bottom,int dstLeft,int dstTop,int op,int incx,int incy);
void 	MGLAPI DDRAW_bitBltOff(MGLDC *dst,MGLDC *src,int left,int top,int right,int bottom,int dstLeft,int dstTop,int op);
void 	MGLAPI DDRAW_srcTransBltOff(MGLDC *dst,MGLDC *src,int left,int top,int right,int bottom,int dstLeft,int dstTop,int op,color_t transparent);
void 	MGLAPI DDRAW_dstTransBltOff(MGLDC *dst,MGLDC *src,int left,int top,int right,int bottom,int dstLeft,int dstTop,int op,color_t transparent);

/* Offscreen memory hardware rendering routines */

void	MGLAPI DDRAW_OFF_fillRect(int x1,int y1,int x2,int y2);
void	MGLAPI DDRAW_OFF_ropFillRect(int x1,int y1,int x2,int y2);
void 	MGLAPI DDRAW_OFF_bitBlt(MGLDC *dc,int left,int top,int right,int bottom,int dstLeft,int dstTop,int op,int incx,int incy);

/* Stubs to do lock/unlock calls while drawing to linear framebuffer */

void 	MGLAPI DDRAW_lock(void);
void 	MGLAPI DDRAW_unlock(void);

color_t	MGLAPI DDSTUB_getPixel(int x,int y);
void    MGLAPI DDSTUB_putPixel(int x,int y);
void    MGLAPI DDSTUB_putMonoImage(MGLDC *dc,int x,int y,int byteWidth,int height,uchar *image);
void    MGLAPI DDSTUB_putMouseImage(MGLDC *dc,int x,int y,int byteWidth,int height,uchar *andMask,uchar *xorMask,color_t color);
void	MGLAPI DDSTUB_getImage(MGLDC *dc,int left,int top,int right,int bottom,int dstLeft,int dstTop,int op,void *surface,int bytesPerLine);
void 	MGLAPI DDSTUB_putImage(MGLDC *dc,int left,int top,int right,int bottom,int dstLeft,int dstTop,int op,void *surface,int bytesPerLine,MGLDC *src);
void 	MGLAPI DDSTUB_getDivot(MGLDC *dc,int left,int top,int right,int bottom,void *divot);
void 	MGLAPI DDSTUB_putDivot(MGLDC *dc,void *divot);
void 	MGLAPI DDSTUB_stretchBlt1x2(MGLDC *dst,MGLDC *src,int left,int top,int right,int bottom,int dstLeft,int dstTop,palette_t *pal,ibool translate);
void 	MGLAPI DDSTUB_stretchBlt2x2(MGLDC *dst,MGLDC *src,int left,int top,int right,int bottom,int dstLeft,int dstTop,palette_t *pal,ibool translate);
void 	MGLAPI DDSTUB_stretchBlt(MGLDC *dst,MGLDC *src,int left,int top,int right,int bottom,int dstLeft,int dstTop,fix32_t zoomx,fix32_t zoomy,palette_t *pal,ibool translate);

void    MGLAPI DDSTUB_solidLine(fix32_t x1,fix32_t y1,fix32_t x2,fix32_t y2);
void    MGLAPI DDSTUB_ropSolidLine(fix32_t x1,fix32_t y1,fix32_t x2,fix32_t y2);
void    MGLAPI DDSTUB_pattLine(fix32_t x1,fix32_t y1,fix32_t x2,fix32_t y2);
void    MGLAPI DDSTUB_colorPattLine(fix32_t x1,fix32_t y1,fix32_t x2,fix32_t y2);
void    MGLAPI DDSTUB_fatSolidLine(fix32_t x1,fix32_t y1,fix32_t x2,fix32_t y2);
void    MGLAPI DDSTUB_fatRopSolidLine(fix32_t x1,fix32_t y1,fix32_t x2,fix32_t y2);
void    MGLAPI DDSTUB_fatPattLine(fix32_t x1,fix32_t y1,fix32_t x2,fix32_t y2);
void    MGLAPI DDSTUB_fatColorPattLine(fix32_t x1,fix32_t y1,fix32_t x2,fix32_t y2);
void    MGLAPI DDSTUB_ditherLine(fix32_t x1,fix32_t y1,fix32_t x2,fix32_t y2);

void    MGLAPI DDSTUB_solidScanLine(int y,int x1,int x2);
void    MGLAPI DDSTUB_ropSolidScanLine(int y,int x1,int x2);
void    MGLAPI DDSTUB_pattScanLine(int y,int x1,int x2);
void    MGLAPI DDSTUB_colorPattScanLine(int y,int x1,int x2);
void    MGLAPI DDSTUB_fatSolidScanLine(int y,int x1,int x2);
void    MGLAPI DDSTUB_fatRopSolidScanLine(int y,int x1,int x2);
void    MGLAPI DDSTUB_fatPattScanLine(int y,int x1,int x2);
void    MGLAPI DDSTUB_fatColorPattScanLine(int y,int x1,int x2);
void    MGLAPI DDSTUB_ditherScanLine(int y,int x1,int x2);

void	MGLAPI DDSTUB_solidFillRect(int x1,int y1,int x2,int y2);
void	MGLAPI DDSTUB_ropSolidFillRect(int x1,int y1,int x2,int y2);
void	MGLAPI DDSTUB_pattFillRect(int x1,int y1,int x2,int y2);
void	MGLAPI DDSTUB_colorPattFillRect(int x1,int y1,int x2,int y2);
void	MGLAPI DDSTUB_fatSolidFillRect(int x1,int y1,int x2,int y2);
void	MGLAPI DDSTUB_fatRopSolidFillRect(int x1,int y1,int x2,int y2);
void	MGLAPI DDSTUB_fatPattFillRect(int x1,int y1,int x2,int y2);
void	MGLAPI DDSTUB_fatColorPattFillRect(int x1,int y1,int x2,int y2);
void	MGLAPI DDSTUB_ditherFillRect(int x1,int y1,int x2,int y2);

void    MGLAPI DDSTUB_solidDrawScanList(int y,int length,short *scans);
void    MGLAPI DDSTUB_ropSolidDrawScanList(int y,int length,short *scans);
void    MGLAPI DDSTUB_pattDrawScanList(int y,int length,short *scans);
void    MGLAPI DDSTUB_colorPattDrawScanList(int y,int length,short *scans);
void    MGLAPI DDSTUB_fatSolidDrawScanList(int y,int length,short *scans);
void    MGLAPI DDSTUB_fatRopSolidDrawScanList(int y,int length,short *scans);
void    MGLAPI DDSTUB_fatPattDrawScanList(int y,int length,short *scans);
void    MGLAPI DDSTUB_fatColorPattDrawScanList(int y,int length,short *scans);
void    MGLAPI DDSTUB_ditherDrawScanList(int y,int length,short *scans);

void    MGLAPI DDSTUB_solidTrap(void);
void    MGLAPI DDSTUB_ropSolidTrap(void);
void    MGLAPI DDSTUB_pattTrap(void);
void    MGLAPI DDSTUB_colorPattTrap(void);
void    MGLAPI DDSTUB_fatSolidTrap(void);
void    MGLAPI DDSTUB_fatRopSolidTrap(void);
void    MGLAPI DDSTUB_fatPattTrap(void);
void    MGLAPI DDSTUB_fatColorPattTrap(void);
void    MGLAPI DDSTUB_ditherTrap(void);

void    MGLAPI DDSTUB_solidDrawRegion(int x,int y,const region_t *r);
void    MGLAPI DDSTUB_ropSolidDrawRegion(int x,int y,const region_t *r);
void    MGLAPI DDSTUB_pattDrawRegion(int x,int y,const region_t *r);
void    MGLAPI DDSTUB_colorPattDrawRegion(int x,int y,const region_t *r);
void    MGLAPI DDSTUB_fatSolidDrawRegion(int x,int y,const region_t *r);
void    MGLAPI DDSTUB_fatRopSolidDrawRegion(int x,int y,const region_t *r);
void    MGLAPI DDSTUB_fatPattDrawRegion(int x,int y,const region_t *r);
void    MGLAPI DDSTUB_fatColorPattDrawRegion(int x,int y,const region_t *r);
void    MGLAPI DDSTUB_ditherDrawRegion(int x,int y,const region_t *r);

void    MGLAPI DDSTUB_solidEllipse(int left,int top,int A,int B,int clip);
void    MGLAPI DDSTUB_ropSolidEllipse(int left,int top,int A,int B,int clip);
void	MGLAPI DDSTUB_pattEllipse(int left,int top,int A,int B,int clip);
void	MGLAPI DDSTUB_colorPattEllipse(int left,int top,int A,int B,int clip);
void    MGLAPI DDSTUB_fatSolidEllipse(int left,int top,int A,int B,int clip);
void    MGLAPI DDSTUB_fatRopSolidEllipse(int left,int top,int A,int B,int clip);
void    MGLAPI DDSTUB_fatPattEllipse(int left,int top,int A,int B,int clip);
void    MGLAPI DDSTUB_fatColorPattEllipse(int left,int top,int A,int B,int clip);
void    MGLAPI DDSTUB_ditherEllipse(int left,int top,int A,int B,int clip);

void    MGLAPI DDSTUB_solidFillEllipse(int left,int top, int A, int B,int clip);
void    MGLAPI DDSTUB_ropSolidFillEllipse(int left,int top, int A, int B,int clip);
void    MGLAPI DDSTUB_pattFillEllipse(int left,int top, int A, int B,int clip);
void    MGLAPI DDSTUB_colorPattFillEllipse(int left,int top, int A, int B,int clip);
void    MGLAPI DDSTUB_fatSolidFillEllipse(int left,int top, int A, int B,int clip);
void    MGLAPI DDSTUB_fatRopSolidFillEllipse(int left,int top, int A, int B,int clip);
void    MGLAPI DDSTUB_fatPattFillEllipse(int left,int top, int A, int B,int clip);
void    MGLAPI DDSTUB_fatColorPattFillEllipse(int left,int top, int A, int B,int clip);
void    MGLAPI DDSTUB_ditherFillEllipse(int left,int top, int A, int B,int clip);

void    MGLAPI DDSTUB_solidEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);
void    MGLAPI DDSTUB_ropSolidEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);
void    MGLAPI DDSTUB_pattEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);
void    MGLAPI DDSTUB_colorPattEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);
void    MGLAPI DDSTUB_fatSolidEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);
void    MGLAPI DDSTUB_fatRopSolidEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);
void    MGLAPI DDSTUB_fatPattEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);
void    MGLAPI DDSTUB_fatColorPattEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);
void    MGLAPI DDSTUB_ditherEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);

void    MGLAPI DDSTUB_solidFillEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);
void    MGLAPI DDSTUB_ropSolidFillEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);
void    MGLAPI DDSTUB_pattFillEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);
void    MGLAPI DDSTUB_colorPattFillEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);
void    MGLAPI DDSTUB_fatSolidFillEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);
void    MGLAPI DDSTUB_fatRopSolidFillEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);
void    MGLAPI DDSTUB_fatPattFillEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);
void    MGLAPI DDSTUB_fatColorPattFillEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);
void    MGLAPI DDSTUB_ditherFillEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);

void    MGLAPI DDSTUB_stippleLine(fix32_t x1,fix32_t y1,fix32_t x2,fix32_t y2);
void	MGLAPI DDSTUB_drawStrBitmap(int x,int y,const char *str);
void	MGLAPI DDSTUB_drawCharVec(int x,int y,vector *vec,int sz_nx,int sz_ny,int sz_dx,int sz_dy);
void	MGLAPI DDSTUB_complexPolygon(int count,fxpoint_t *vArray,int vinc,fix32_t xOffset,fix32_t yOffset);
void	MGLAPI DDSTUB_polygon(int count,fxpoint_t *vArray,int vinc,fix32_t xOffset,fix32_t yOffset);
void	MGLAPI DDSTUB_ditherPolygon(int count,fxpoint_t *vArray,int vinc,fix32_t xOffset,fix32_t yOffset);
void	MGLAPI DDSTUB_translateImage(MGLDC *dc,int left,int top,int right,int bottom,int dstLeft,int dstTop,int op,void *surface,int bytesPerLine,int bitsPerPixel,palette_t *pal,pixel_format_t *pf,MGLDC *src);
void 	MGLAPI DDSTUB_bitBlt(MGLDC *dc,int left,int top,int right,int bottom,int dstLeft,int dstTop,int op,int incx,int incy);
void 	MGLAPI DDSTUB_srcTransBlt(MGLDC *dst,MGLDC *src,int left,int top,int right,int bottom,int dstLeft,int dstTop,int op,color_t transparent);
void 	MGLAPI DDSTUB_dstTransBlt(MGLDC *dst,MGLDC *src,int left,int top,int right,int bottom,int dstLeft,int dstTop,int op,color_t transparent);
int  	MGLAPI DDSTUB_scanRightForColor(int x,int y,color_t color);
int   	MGLAPI DDSTUB_scanLeftForColor(int x,int y,color_t color);
int   	MGLAPI DDSTUB_scanRightWhileColor(int x,int y,color_t color);
int   	MGLAPI DDSTUB_scanLeftWhileColor(int x,int y,color_t color);

#endif	/* __DRIVERS_COMMON_GDDRAW_H */
