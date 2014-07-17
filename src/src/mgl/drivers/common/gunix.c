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
* Description:	Set of device driver routines common to all Unix
*				display drivers.
*
****************************************************************************/

#include "mgl.h"
#include "mglunix.h"
#include "mgldd.h"
#include "drivers/common/gunix.h"

/*------------------------- Global Variables ------------------------------*/

/*-------------------------- Implementation -------------------------------*/

ibool MGLAPI XWIN_detect(void)
/****************************************************************************
*
* Function:		XWIN_detect
*
* Description:	Determines if we are running under X11.
*
****************************************************************************/
{
	return (globalDisplay != NULL);
}

void MGLAPI XWIN_useMode(modetab modes,int mode,int id,gmode_t *mi)
/****************************************************************************
*
* Function:		XWIN_useMode
* Parameters:	modes	- Video mode/device driver association table
*				mode	- Index into mode association table
*				id		- Driver id to store in table
*				mi		- Mode information block to fill in
*
* Description:	Searches through the list of modes supported by X11
*				for one that matches the desired resolution and pixel depth.
*
****************************************************************************/
{
	// TODO: Actually search

	mi->bytesPerLine = 0;
	mi->pageSize = 0;
	mi->maxPage = 0;
	DRV_useMode(modes,mode,id,1,0); // What is the `flags' field ?
}

ulong   MGLAPI XWIN_getWinDC(MGLDC *dc)
/****************************************************************************
*
* Function:		XWIN_getWinDC
* Parameters:	dc	- MGL device context to create HDC for
*
* Description:	This function returns the X11 graphic context (GC) associated
*	 			with the current window.
*
****************************************************************************/
{
	return dc->wm.xwindc.gc;
}

void         XWIN_initInternal(MGLDC *dc)
/****************************************************************************
*
* Function:		XWIN_initInternal
* Parameters:	dc	- Device context.
*
* Description:	Initialises the device driver internal tables for
*				unaccelerated driver routines.
*
****************************************************************************/
{
	dc->r.setColor 				= DRV_setColor;
	dc->r.setBackColor 			= DRV_setBackColor;
	dc->r.setWriteMode			= DRV_setWriteMode;
	dc->r.setClipRect 			= DRV_setClipRect;
	dc->r.getWinDC = XWIN_getWinDC;
#ifndef	MGL_LITE
	dc->r.setPenStyle			= DRV_setPenStyle;
	dc->r.setPenBitmapPattern	= DRV_setPenBitmapPattern;
	dc->r.setPenPixmapPattern	= DRV_setPenPixmapPattern;
	dc->r.setLineStipple		= DRV_setLineStipple;
	dc->r.setLineStippleCount	= DRV_setLineStippleCount;
#endif

	dc->r.stretchBlt1x2			= __EMU__stretchBlt1x2;
	dc->r.stretchBlt2x2			= __EMU__stretchBlt2x2;
	dc->r.stretchBlt			= __EMU__stretchBlt;

#ifndef	MGL_LITE
	dc->r.patt.line				= __EMU__fatPenLine;
	dc->r.colorPatt.line		= __EMU__fatPenLine;
	dc->r.fatSolid.line			= __EMU__fatPenLine;
	dc->r.fatRopSolid.line		= __EMU__fatPenLine;
	dc->r.fatPatt.line			= __EMU__fatPenLine;
	dc->r.fatColorPatt.line		= __EMU__fatPenLine;
	_MGL_setDrawScanListVecs(dc,  __EMU__drawScanList);
	_MGL_setDrawRegionVecs(dc,	  __EMU__drawRegion);
	_MGL_setEllipseVecs(dc,	  	  __EMU__ellipse);
	dc->r.fatSolid.ellipse		= __EMU__fatPenEllipse;
	dc->r.fatRopSolid.ellipse	= __EMU__fatPenEllipse;
	dc->r.fatPatt.ellipse		= __EMU__fatPenEllipse;
	dc->r.fatColorPatt.ellipse 	= __EMU__fatPenEllipse;
	_MGL_setFillEllipseVecs(dc,	  __EMU__fillEllipse);
	_MGL_setEllipseArcVecs(dc,	  __EMU__ellipseArc);
	dc->r.fatSolid.ellipseArc	= __EMU__fatPenEllipseArc;
	dc->r.fatRopSolid.ellipseArc= __EMU__fatPenEllipseArc;
	dc->r.fatPatt.ellipseArc	= __EMU__fatPenEllipseArc;
	dc->r.fatColorPatt.ellipseArc= __EMU__fatPenEllipseArc;
	_MGL_setFillEllipseArcVecs(dc,__EMU__fillEllipseArc);
	dc->r.getArcCoords 			= __EMU__getArcCoords;
	dc->r.drawStrBitmap			= __EMU__drawStrBitmap;
	dc->r.drawCharVec			= __EMU__drawCharVec;
	dc->r.complexPolygon 		= __EMU__complexPolygon;
	dc->r.polygon 				= __EMU__polygon;
	dc->r.ditherPolygon 		= __EMU__polygon;
	dc->r.translateImage		= __EMU__translateImage;
#endif
	_EVT_init(dc);
}

void *  MGLAPI XWIN_getDefaultPalette(MGLDC *dc)
{
	XColor cols[256];
	static palette_t default_pal[256];
	int i;
	xwindc_vars *v = &dc->wm.xwindc;
	/*
	for(i=0; i<256; i++){
		cols[i].pixel = i;
		cols[i].flags = DoGreen | DoBlue | DoRed;
	}
	XQueryColors(v->dpy, v->hpal, cols, 256);
	for(i=0; i<256; i++){
		default_pal[i].green = cols[i].green >> 8;
		default_pal[i].blue = cols[i].blue >> 8;
		default_pal[i].red = cols[i].red >> 8;
	}
	*/
	for(i=0; i<256; i++){
		default_pal[i].green = 	default_pal[i].blue = default_pal[i].red = i;
	}

	return default_pal;
}

void    MGLAPI XWIN_realizePalette(MGLDC *dc,palette_t *pal,int num,int index,int waitVRT)
/****************************************************************************
*
* Function:		XWIN8_realizePalette
* Parameters:	dc		- Device context
*               pal		- Palette of values to program
*               num		- Number of entries to program
*				index	- Index to start programming at
*
* Description:	Program the X11 colormap. First we need to translate the
*				values from the MGL internal format into the 16 bit RGB
*				format used by X11.
*
****************************************************************************/
{
	int i;
	XColor *cols;
	xwindc_vars *v = &dc->wm.xwindc;
	
	if(v->depth > 8) /* No colormap for true color modes */
		return;

	cols = MGL_malloc(sizeof(XColor) * num);
	for(i=0; i<num; i++){
		cols[i].flags = DoRed | DoGreen | DoBlue;
		cols[i].pixel = i+index;
		cols[i].red   = pal[i].red << 8;
		cols[i].green = pal[i].green << 8;
		cols[i].blue  = pal[i].blue << 8;
	}
	XStoreColors(v->dpy, v->hpal, cols, num);
	XFlush(v->dpy);
	MGL_free(cols);
}

void MGLAPI XWIN_putImage(MGLDC *dc,int left,int top,int right,
	int bottom,int dstLeft,int dstTop,int op,void *surface,
	int bytesPerLine,MGLDC *src)
/****************************************************************************
*
* Function:		XWIN_putImage
* Parameters:	dc		- Device context
*
* Description:	Blt's a system memory buffer DC of the same pixel depth
*				as the display device context to the display DC.
*
****************************************************************************/
{
	MGL_HDC	gc = dc->wm.xwindc.gc;
	Window wnd = dc->wm.xwindc.wnd;
	Display *dpy = dc->wm.xwindc.dpy;
	xwindc_vars *v = &dc->wm.xwindc;
	
	static XImage *im = NULL;
	int func;

	switch(op){
	case MGL_REPLACE_MODE:
		func = GXcopy;
		break;
	case MGL_AND_MODE:
		func = GXand;
		break;
	case MGL_OR_MODE:
		func = GXor;
		break;
	case MGL_XOR_MODE:
		func = GXxor;
		break;
	}
	XSetFunction(dpy,gc,func);
	if(im && im->width==src->mi.xRes && im->height==src->mi.yRes && im->depth==v->depth){
		im->data = (char*) surface;
	}else{
		if(im){
			im->data = NULL;
			XDestroyImage(im);
		}
		im = XCreateImage(dpy, DefaultVisualOfScreen(v->scr), v->depth, ZPixmap, 0, surface, src->mi.xRes, src->mi.yRes, 8, src->mi.bytesPerLine);
		XInitImage(im);
	}
	XPutImage(dpy, wnd, gc, im, 0,0, dstLeft, dstTop, src->mi.xRes, src->mi.yRes);
}
