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
* Description:	Common header file to all hardware OpenGL display drivers.
*
****************************************************************************/

#ifndef	__DRIVERS_COMMON_GOPENGL_H
#define	__DRIVERS_COMMON_GOPENGL_H

#ifndef	__DRIVERS_COMMON_GDDRAW_H
#include "drivers/common/gddraw.h"
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
	} FSOPENGL_state;

#pragma pack()

/*-------------------------- Global Variables -----------------------------*/

extern FGL_hwInfo	_MGL_glHWInfo;

/*------------------------- Function Prototypes ---------------------------*/

ibool	MGLAPI FSOPENGL_detect(int *driver,char *driverId,char *realname);
void 	MGLAPI FSOPENGL_useMode(modetab modes,int mode,int id,gmode_t *modeInfo);
ibool 	MGLAPI FSOPENGL_setMode(MGLDC *dc,int virtualX,int virtualY,int numBuffers,ibool stereo,int refreshRate);
void 	MGLAPI FSOPENGL_setAccelVectors(MGLDC *dc);
void 	MGLAPI FSOPENGL_restoreTextMode(MGLDC *dc);
void 	MGLAPI FSOPENGL_setActivePage(MGLDC *dc,int page);
void 	MGLAPI FSOPENGL_realizePalette(MGLDC *dc,palette_t *pal,int num,int index,int waitVRT);
void 	MGLAPI FSOPENGL_vSync(MGLDC *dc);
void * 	MGLAPI FSOPENGL_getDefaultPalette(MGLDC *dc);
ulong 	MGLAPI FSOPENGL_getWinDC(MGLDC *dc);

/* Hardware device driver support routines */

void	MGLAPI FSOPENGL_beginDirectAccess(void);
void	MGLAPI FSOPENGL_endDirectAccess(void);
void	MGLAPI FSOPENGL_beginPixel(MGLDC *dc);
void	MGLAPI FSOPENGL_endPixel(MGLDC *dc);

/* Hardware rendering routines */

// TODO: Implement this using fgl calls
//void	MGLAPI FSOPENGL_fillRect(int x1,int y1,int x2,int y2);
//void	MGLAPI FSOPENGL_ropFillRect(int x1,int y1,int x2,int y2);
// bitBlt
// stretchBlt
// transBlt

/* Stubs to do lock/unlock calls while drawing to linear framebuffer */

void 	MGLAPI FSOPENGL_lock(void);
void 	MGLAPI FSOPENGL_unlock(void);

color_t	MGLAPI OGLSTUB_getPixel(int x,int y);
void    MGLAPI OGLSTUB_putPixel(int x,int y);
void    MGLAPI OGLSTUB_putMonoImage(MGLDC *dc,int x,int y,int byteWidth,int height,uchar *image);
void    MGLAPI OGLSTUB_putMouseImage(MGLDC *dc,int x,int y,int byteWidth,int height,uchar *andMask,uchar *xorMask,color_t color);
void	MGLAPI OGLSTUB_getImage(MGLDC *dc,int left,int top,int right,int bottom,int dstLeft,int dstTop,int op,void *surface,int bytesPerLine);
void 	MGLAPI OGLSTUB_putImage(MGLDC *dc,int left,int top,int right,int bottom,int dstLeft,int dstTop,int op,void *surface,int bytesPerLine,MGLDC *src);
void 	MGLAPI OGLSTUB_getDivot(MGLDC *dc,int left,int top,int right,int bottom,void *divot);
void 	MGLAPI OGLSTUB_putDivot(MGLDC *dc,void *divot);
void 	MGLAPI OGLSTUB_stretchBlt1x2(MGLDC *dst,MGLDC *src,int left,int top,int right,int bottom,int dstLeft,int dstTop,palette_t *pal,ibool translate);
void 	MGLAPI OGLSTUB_stretchBlt2x2(MGLDC *dst,MGLDC *src,int left,int top,int right,int bottom,int dstLeft,int dstTop,palette_t *pal,ibool translate);
void 	MGLAPI OGLSTUB_stretchBlt(MGLDC *dst,MGLDC *src,int left,int top,int right,int bottom,int dstLeft,int dstTop,fix32_t zoomx,fix32_t zoomy,palette_t *pal,ibool translate);

void    MGLAPI OGLSTUB_solidLine(fix32_t x1,fix32_t y1,fix32_t x2,fix32_t y2);
void    MGLAPI OGLSTUB_ropSolidLine(fix32_t x1,fix32_t y1,fix32_t x2,fix32_t y2);
void    MGLAPI OGLSTUB_pattLine(fix32_t x1,fix32_t y1,fix32_t x2,fix32_t y2);
void    MGLAPI OGLSTUB_colorPattLine(fix32_t x1,fix32_t y1,fix32_t x2,fix32_t y2);
void    MGLAPI OGLSTUB_fatSolidLine(fix32_t x1,fix32_t y1,fix32_t x2,fix32_t y2);
void    MGLAPI OGLSTUB_fatRopSolidLine(fix32_t x1,fix32_t y1,fix32_t x2,fix32_t y2);
void    MGLAPI OGLSTUB_fatPattLine(fix32_t x1,fix32_t y1,fix32_t x2,fix32_t y2);
void    MGLAPI OGLSTUB_fatColorPattLine(fix32_t x1,fix32_t y1,fix32_t x2,fix32_t y2);
void    MGLAPI OGLSTUB_ditherLine(fix32_t x1,fix32_t y1,fix32_t x2,fix32_t y2);

void    MGLAPI OGLSTUB_solidScanLine(int y,int x1,int x2);
void    MGLAPI OGLSTUB_ropSolidScanLine(int y,int x1,int x2);
void    MGLAPI OGLSTUB_pattScanLine(int y,int x1,int x2);
void    MGLAPI OGLSTUB_colorPattScanLine(int y,int x1,int x2);
void    MGLAPI OGLSTUB_fatSolidScanLine(int y,int x1,int x2);
void    MGLAPI OGLSTUB_fatRopSolidScanLine(int y,int x1,int x2);
void    MGLAPI OGLSTUB_fatPattScanLine(int y,int x1,int x2);
void    MGLAPI OGLSTUB_fatColorPattScanLine(int y,int x1,int x2);
void    MGLAPI OGLSTUB_ditherScanLine(int y,int x1,int x2);

void	MGLAPI OGLSTUB_solidFillRect(int x1,int y1,int x2,int y2);
void	MGLAPI OGLSTUB_ropSolidFillRect(int x1,int y1,int x2,int y2);
void	MGLAPI OGLSTUB_pattFillRect(int x1,int y1,int x2,int y2);
void	MGLAPI OGLSTUB_colorPattFillRect(int x1,int y1,int x2,int y2);
void	MGLAPI OGLSTUB_fatSolidFillRect(int x1,int y1,int x2,int y2);
void	MGLAPI OGLSTUB_fatRopSolidFillRect(int x1,int y1,int x2,int y2);
void	MGLAPI OGLSTUB_fatPattFillRect(int x1,int y1,int x2,int y2);
void	MGLAPI OGLSTUB_fatColorPattFillRect(int x1,int y1,int x2,int y2);
void	MGLAPI OGLSTUB_ditherFillRect(int x1,int y1,int x2,int y2);

void    MGLAPI OGLSTUB_solidDrawScanList(int y,int length,short *scans);
void    MGLAPI OGLSTUB_ropSolidDrawScanList(int y,int length,short *scans);
void    MGLAPI OGLSTUB_pattDrawScanList(int y,int length,short *scans);
void    MGLAPI OGLSTUB_colorPattDrawScanList(int y,int length,short *scans);
void    MGLAPI OGLSTUB_fatSolidDrawScanList(int y,int length,short *scans);
void    MGLAPI OGLSTUB_fatRopSolidDrawScanList(int y,int length,short *scans);
void    MGLAPI OGLSTUB_fatPattDrawScanList(int y,int length,short *scans);
void    MGLAPI OGLSTUB_fatColorPattDrawScanList(int y,int length,short *scans);
void    MGLAPI OGLSTUB_ditherDrawScanList(int y,int length,short *scans);

void    MGLAPI OGLSTUB_solidTrap(void);
void    MGLAPI OGLSTUB_ropSolidTrap(void);
void    MGLAPI OGLSTUB_pattTrap(void);
void    MGLAPI OGLSTUB_colorPattTrap(void);
void    MGLAPI OGLSTUB_fatSolidTrap(void);
void    MGLAPI OGLSTUB_fatRopSolidTrap(void);
void    MGLAPI OGLSTUB_fatPattTrap(void);
void    MGLAPI OGLSTUB_fatColorPattTrap(void);
void    MGLAPI OGLSTUB_ditherTrap(void);

void    MGLAPI OGLSTUB_solidDrawRegion(int x,int y,const region_t *r);
void    MGLAPI OGLSTUB_ropSolidDrawRegion(int x,int y,const region_t *r);
void    MGLAPI OGLSTUB_pattDrawRegion(int x,int y,const region_t *r);
void    MGLAPI OGLSTUB_colorPattDrawRegion(int x,int y,const region_t *r);
void    MGLAPI OGLSTUB_fatSolidDrawRegion(int x,int y,const region_t *r);
void    MGLAPI OGLSTUB_fatRopSolidDrawRegion(int x,int y,const region_t *r);
void    MGLAPI OGLSTUB_fatPattDrawRegion(int x,int y,const region_t *r);
void    MGLAPI OGLSTUB_fatColorPattDrawRegion(int x,int y,const region_t *r);
void    MGLAPI OGLSTUB_ditherDrawRegion(int x,int y,const region_t *r);

void    MGLAPI OGLSTUB_solidEllipse(int left,int top,int A,int B,int clip);
void    MGLAPI OGLSTUB_ropSolidEllipse(int left,int top,int A,int B,int clip);
void	MGLAPI OGLSTUB_pattEllipse(int left,int top,int A,int B,int clip);
void	MGLAPI OGLSTUB_colorPattEllipse(int left,int top,int A,int B,int clip);
void    MGLAPI OGLSTUB_fatSolidEllipse(int left,int top,int A,int B,int clip);
void    MGLAPI OGLSTUB_fatRopSolidEllipse(int left,int top,int A,int B,int clip);
void    MGLAPI OGLSTUB_fatPattEllipse(int left,int top,int A,int B,int clip);
void    MGLAPI OGLSTUB_fatColorPattEllipse(int left,int top,int A,int B,int clip);
void    MGLAPI OGLSTUB_ditherEllipse(int left,int top,int A,int B,int clip);

void    MGLAPI OGLSTUB_solidFillEllipse(int left,int top, int A, int B,int clip);
void    MGLAPI OGLSTUB_ropSolidFillEllipse(int left,int top, int A, int B,int clip);
void    MGLAPI OGLSTUB_pattFillEllipse(int left,int top, int A, int B,int clip);
void    MGLAPI OGLSTUB_colorPattFillEllipse(int left,int top, int A, int B,int clip);
void    MGLAPI OGLSTUB_fatSolidFillEllipse(int left,int top, int A, int B,int clip);
void    MGLAPI OGLSTUB_fatRopSolidFillEllipse(int left,int top, int A, int B,int clip);
void    MGLAPI OGLSTUB_fatPattFillEllipse(int left,int top, int A, int B,int clip);
void    MGLAPI OGLSTUB_fatColorPattFillEllipse(int left,int top, int A, int B,int clip);
void    MGLAPI OGLSTUB_ditherFillEllipse(int left,int top, int A, int B,int clip);

void    MGLAPI OGLSTUB_solidEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);
void    MGLAPI OGLSTUB_ropSolidEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);
void    MGLAPI OGLSTUB_pattEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);
void    MGLAPI OGLSTUB_colorPattEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);
void    MGLAPI OGLSTUB_fatSolidEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);
void    MGLAPI OGLSTUB_fatRopSolidEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);
void    MGLAPI OGLSTUB_fatPattEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);
void    MGLAPI OGLSTUB_fatColorPattEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);
void    MGLAPI OGLSTUB_ditherEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);

void    MGLAPI OGLSTUB_solidFillEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);
void    MGLAPI OGLSTUB_ropSolidFillEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);
void    MGLAPI OGLSTUB_pattFillEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);
void    MGLAPI OGLSTUB_colorPattFillEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);
void    MGLAPI OGLSTUB_fatSolidFillEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);
void    MGLAPI OGLSTUB_fatRopSolidFillEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);
void    MGLAPI OGLSTUB_fatPattFillEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);
void    MGLAPI OGLSTUB_fatColorPattFillEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);
void    MGLAPI OGLSTUB_ditherFillEllipseArc(int left,int top,int A,int B,int startangle,int endangle,int clip);

void    MGLAPI OGLSTUB_stippleLine(fix32_t x1,fix32_t y1,fix32_t x2,fix32_t y2);
void	MGLAPI OGLSTUB_drawStrBitmap(int x,int y,const char *str);
void	MGLAPI OGLSTUB_drawCharVec(int x,int y,vector *vec,int sz_nx,int sz_ny,int sz_dx,int sz_dy);
void	MGLAPI OGLSTUB_complexPolygon(int count,fxpoint_t *vArray,int vinc,fix32_t xOffset,fix32_t yOffset);
void	MGLAPI OGLSTUB_polygon(int count,fxpoint_t *vArray,int vinc,fix32_t xOffset,fix32_t yOffset);
void	MGLAPI OGLSTUB_ditherPolygon(int count,fxpoint_t *vArray,int vinc,fix32_t xOffset,fix32_t yOffset);
void	MGLAPI OGLSTUB_translateImage(MGLDC *dc,int left,int top,int right,int bottom,int dstLeft,int dstTop,int op,void *surface,int bytesPerLine,int bitsPerPixel,palette_t *pal,pixel_format_t *pf,MGLDC *src);
void 	MGLAPI OGLSTUB_bitBlt(MGLDC *dc,int left,int top,int right,int bottom,int dstLeft,int dstTop,int op,int incx,int incy);
void 	MGLAPI OGLSTUB_srcTransBlt(MGLDC *dst,MGLDC *src,int left,int top,int right,int bottom,int dstLeft,int dstTop,int op,color_t transparent);
void 	MGLAPI OGLSTUB_dstTransBlt(MGLDC *dst,MGLDC *src,int left,int top,int right,int bottom,int dstLeft,int dstTop,int op,color_t transparent);
int  	MGLAPI OGLSTUB_scanRightForColor(int x,int y,color_t color);
int   	MGLAPI OGLSTUB_scanLeftForColor(int x,int y,color_t color);
int   	MGLAPI OGLSTUB_scanRightWhileColor(int x,int y,color_t color);
int   	MGLAPI OGLSTUB_scanLeftWhileColor(int x,int y,color_t color);

#endif	/* __DRIVERS_COMMON_GOPENGL_H */
