/****************************************************************************
*
*				 	   Mesa bindings for SciTech MGL
*
*               Copyright (C) 1996-1998 SciTech Software, Inc.
*                            All rights reserved.
*
* Language:     ANSI C
* Environment:	Any
*
* Description:	Header file for the Mesa/OpenGL interface bindings for the
*				SciTech MGL graphics library. Uses the MGL internal
*				device context structures to get direct access to the
*				high performance MGL rasterization functions for maximum
*				performance. Utilizes the VESA VBE/AF Accelerator Functions
*				via the MGL's accelerated device driver functions, as well
*				as basic DirectDraw accelerated functions provided by the
*				MGL.
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Library General Public
* License as published by the Free Software Foundation; either
* version 2 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Library General Public
* License along with this library; if not, write to the Free
* Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
****************************************************************************/

#ifndef	__MGLMESA_H
#define	__MGLMESA_H

#include "GL/gl.h"
#include "GL/glu.h"
#include "mgraph.h"
#include "pmapi.h"

/*---------------------- Macros and type definitions ----------------------*/

/* Callback structure for external code in DLL's such as Mesa */

#pragma pack(1)
typedef struct {
	long	dwSize;
	MGLDC * (MGLAPIP MGL_makeCurrentDC)(MGLDC *dc);
	void    (MGLAPIP MGL_setActivePage)(MGLDC *dc,int page);
	void    (MGLAPIP MGL_setVisualPage)(MGLDC *dc,int page,int waitVRT);
	int     (MGLAPIP MGL_surfaceAccessType)(MGLDC *dc);
	ibool   (MGLAPIP MGL_isDisplayDC)(MGLDC *dc);
	ibool   (MGLAPIP MGL_isWindowedDC)(MGLDC *dc);
	ibool   (MGLAPIP MGL_isMemoryDC)(MGLDC *dc);
	MGLDC * (MGLAPIP MGL_createMemoryDC)(int xSize,int ySize,int bitsPerPixel,pixel_format_t *pf);
	ibool   (MGLAPIP MGL_destroyDC)(MGLDC *dc);
	void    (MGLAPIP MGL_bitBltCoord)(MGLDC *dst,MGLDC *src,int left,int top,int right,int bottom,int dstLeft,int dstTop,int op);
	void    (MGLAPIP MGL_setPaletteEntry)(MGLDC *dc,int entry,uchar red,uchar green,uchar blue);
	void    (MGLAPIP MGL_setPalette)(MGLDC *dc,palette_t *pal,int numColors,int startIndex);
	void    (MGLAPIP MGL_getPalette)(MGLDC *dc,palette_t *pal,int numColors,int startIndex);
	void    (MGLAPIP MGL_realizePalette)(MGLDC *dc,int numColors,int startIndex,int waitVRT);
	} MESA_imports;

/* Exports for all MGLMesa specific functions */

typedef struct {
	long	dwSize;
	void	(MGLAPIP MGLMesaChooseVisual)(MGLDC *dc,MGLVisual *visual);
	ibool	(MGLAPIP MGLMesaSetVisual)(MGLDC *dc,MGLVisual *visual);
	ibool	(MGLAPIP MGLMesaCreateContext)(MGLDC *dc,ibool forceMemDC);
	void 	(MGLAPIP MGLMesaDestroyContext)(MGLDC *dc);
	void 	(MGLAPIP MGLMesaMakeCurrent)(MGLDC *dc);
	void	(MGLAPIP MGLMesaSwapBuffers)(MGLDC *dc,ibool waitVRT);
	void 	(MGLAPIP MGLMesaSetPaletteEntry)(MGLDC *dc,int entry,uchar red,uchar green,uchar blue);
	void 	(MGLAPIP MGLMesaSetPalette)(MGLDC *dc,palette_t *pal,int numColors,int startIndex);
	void 	(MGLAPIP MGLMesaRealizePalette)(MGLDC *dc,int numColors,int startIndex,int waitVRT);
	void 	(MGLAPIP MGLMesaResizeBuffers)(void);
	void *	(MGLAPIP MGLMesaGetProcAddress)(const char *procName);
	} MESA_exports;
#pragma pack()

#ifndef	NO_MGL_IMPORTS
#define	MGL_makeCurrentDC(dc)              									_MESA_imports.MGL_makeCurrentDC(dc)
#define	MGL_setActivePage(dc,page)              							_MESA_imports.MGL_setActivePage(dc,page)
#define	MGL_setVisualPage(dc,page,waitVRT)              					_MESA_imports.MGL_setVisualPage(dc,page,waitVRT)
#define	MGL_surfaceAccessType(dc)              								_MESA_imports.MGL_surfaceAccessType(dc)
#define	MGL_isDisplayDC(dc)              									_MESA_imports.MGL_isDisplayDC(dc)
#define	MGL_isWindowedDC(dc)              									_MESA_imports.MGL_isWindowedDC(dc)
#define	MGL_isMemoryDC(dc)              									_MESA_imports.MGL_isMemoryDC(dc)
#define	MGL_createMemoryDC(xSize,ySize,bitsPerPixel,pf)              		_MESA_imports.MGL_createMemoryDC(xSize,ySize,bitsPerPixel,pf)
#define	MGL_destroyDC(dc)              										_MESA_imports.MGL_destroyDC(dc)
#define	MGL_bitBltCoord(dst,src,left,top,right,bottom,dstLeft,dstTop,op)	_MESA_imports.MGL_bitBltCoord(dst,src,left,top,right,bottom,dstLeft,dstTop,op)
#define	MGL_setPaletteEntry(dc,entry,red,green,blue)        				_MESA_imports.MGL_setPaletteEntry(dc,entry,red,green,blue)
#define	MGL_setPalette(dc,pal,numColors,startIndex)         				_MESA_imports.MGL_setPalette(dc,pal,numColors,startIndex)
#define	MGL_getPalette(dc,pal,numColors,startIndex)        					_MESA_imports.MGL_getPalette(dc,pal,numColors,startIndex)
#define	MGL_realizePalette(dc,numColors,startIndex,waitVRT)					_MESA_imports.MGL_realizePalette(dc,numColors,startIndex,waitVRT)
#endif

typedef MGL_glFuncs * (PMAPIP MESA_initLibrary_t)(PM_imports *pmImp,MESA_imports *glImp,MESA_exports *glExp);
typedef MGL_gluFuncs * (PMAPIP MESAGLU_initLibrary_t)(PM_imports *pmImp,MGL_glFuncs *glFuncs);

/*--------------------------- Global Variables ----------------------------*/

#ifndef	NO_MGL_IMPORTS
extern MESA_imports _VARAPI	_MESA_imports;
#endif

/*------------------------- Function Prototypes ---------------------------*/

#ifdef	__cplusplus
extern "C" {			/* Use "C" linkage when in C++ mode	*/
#endif

void	MGLAPI MGLMesaChooseVisual(MGLDC *dc,MGLVisual *visual);
ibool	MGLAPI MGLMesaSetVisual(MGLDC *dc,MGLVisual *visual);
ibool	MGLAPI MGLMesaCreateContext(MGLDC *dc,ibool forceMemDC);
void 	MGLAPI MGLMesaDestroyContext(MGLDC *dc);
void 	MGLAPI MGLMesaMakeCurrent(MGLDC *dc);
void	MGLAPI MGLMesaSwapBuffers(MGLDC *dc,ibool waitVRT);
void 	MGLAPI MGLMesaResizeBuffers(void);
void *	MGLAPI MGLMesaGetProcAddress(const char *procName);

/* Palette manipulation support. The reason we provide palette manipulation
 * routines is so that when rendering in double buffered modes with a
 * software backbuffer, the palette for the backbuffer is kept consistent
 * with the hardware front buffer.
 */

void 	MGLAPI MGLMesaSetPaletteEntry(MGLDC *dc,int entry,uchar red,uchar green,uchar blue);
void 	MGLAPI MGLMesaSetPalette(MGLDC *dc,palette_t *pal,int numColors,int startIndex);
void 	MGLAPI MGLMesaRealizePalette(MGLDC *dc,int numColors,int startIndex,int waitVRT);

#ifdef	__cplusplus
}						/* End of "C" linkage for C++	*/
#endif	/* __cplusplus */

#endif	/* __MGLMESA_H */
