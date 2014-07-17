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
* Description:	Device driver helper routines common to all display
*				device drivers.
*
****************************************************************************/

#include "mgldd.h"
#include "drivers/common/common.h"

/*------------------------- Implementation --------------------------------*/

/****************************************************************************
PARAMETERS:
dc	- MGL device context to get HDC for

REMARKS:
This function returns a copy of the internal window handle device context
from the MGL to the application.
****************************************************************************/
ulong MGLAPI DRV_getWinDC(
	MGLDC *dc)
{
	return (ulong)dc->wm.gendc.hdc;
}

/****************************************************************************
PARAMETERS:
dc	- MGL device context to get HDC for

RETURNS:
Pointer to the default palette for the DC, NULL if none available.
****************************************************************************/
void * MGLAPI DRV_getDefaultPalette(
	MGLDC *dc)
{
	if (dc->mi.bitsPerPixel >= 8)
		return _VGA8_defPal;
	else if (dc->mi.bitsPerPixel == 4)
		return _VGA4_defPal;
	return NULL;
}

/****************************************************************************
PARAMETERS:
dc		- MGL device context to get HDC for
state2d	- Pointer to Nucleus 2D state functions
draw2d	- Pointer to Nucleus 2D drawing functions
ref2d	- Pointer to Nucleus 2D reference driver

RETURNS:
Pointer to the default palette for the DC, NULL if none available.
****************************************************************************/
void DRV_setNucleusRenderingVectors(
	MGLDC *dc,
	REF2D_driver *ref2d)
{
	GA_2DStateFuncs 	state2d;
	GA_2DRenderFuncs	draw2d;

	/* Get the function group from the reference rasteriser */
	state2d.dwSize = sizeof(state2d);
	ref2d->QueryFunctions(GA_GET_2DSTATEFUNCS,&state2d);
	draw2d.dwSize = sizeof(draw2d);
	ref2d->QueryFunctions(GA_GET_2DRENDERFUNCS,&draw2d);

	/* Nucleus compatible 2D state management vectors */
	dc->r.SetDrawSurface 			= ref2d->SetDrawSurface;
	dc->r.SetDrawBuffer 			= state2d.SetDrawBuffer;
	dc->r.SetMix 					= state2d.SetMix;
	dc->r.SetForeColor 				= state2d.SetForeColor;
	dc->r.SetBackColor				= state2d.SetBackColor;
	dc->r.Set8x8MonoPattern         = state2d.Set8x8MonoPattern;
	dc->r.Use8x8MonoPattern         = state2d.Use8x8MonoPattern;
	dc->r.Use8x8TransMonoPattern    = state2d.Use8x8TransMonoPattern;
	dc->r.Set8x8ColorPattern        = state2d.Set8x8ColorPattern;
	dc->r.Use8x8ColorPattern        = state2d.Use8x8ColorPattern;
	dc->r.Use8x8TransColorPattern   = state2d.Use8x8TransColorPattern;
	dc->r.SetLineStipple            = state2d.SetLineStipple;
	dc->r.SetLineStippleCount       = state2d.SetLineStippleCount;
	dc->r.SetPlaneMask              = state2d.SetPlaneMask;
	dc->r.SetSrcBlendFunc           = state2d.SetSrcBlendFunc;
	dc->r.SetDstBlendFunc           = state2d.SetDstBlendFunc;
	dc->r.SetAlphaValue             = state2d.SetAlphaValue;

	/* SciTech Nucleus compatible rendering vectors */
	dc->r.GetPixel					= draw2d.GetPixel;
	dc->r.PutPixel					= draw2d.PutPixel;
	dc->r.DrawRectLin				= draw2d.DrawRectLin;
	dc->r.DrawLineInt				= draw2d.DrawLineInt;
	dc->r.DrawBresenhamLine			= draw2d.DrawBresenhamLine;
	dc->r.DrawStippleLineInt		= draw2d.DrawStippleLineInt;
	dc->r.DrawBresenhamStippleLine	= draw2d.DrawBresenhamStippleLine;
	dc->r.DrawEllipse				= draw2d.DrawEllipse;
	dc->r.ClipEllipse				= draw2d.ClipEllipse;
	dc->r.PutMonoImage              = draw2d.PutMonoImageMSBSys;
	dc->r.ClipMonoImage             = draw2d.ClipMonoImageMSBSys;
	dc->r.BitBlt                    = draw2d.BitBlt;
	dc->r.BitBltLin                 = draw2d.BitBltLin;
	dc->r.BitBltSys                 = draw2d.BitBltSys;
	dc->r.SrcTransBlt               = draw2d.SrcTransBlt;
	dc->r.SrcTransBltLin            = draw2d.SrcTransBltLin;
	dc->r.SrcTransBltSys            = draw2d.SrcTransBltSys;
	dc->r.DstTransBlt               = draw2d.DstTransBlt;
	dc->r.DstTransBltLin            = draw2d.DstTransBltLin;
	dc->r.DstTransBltSys            = draw2d.DstTransBltSys;
	dc->r.StretchBlt                = draw2d.StretchBlt;
	dc->r.StretchBltLin             = draw2d.StretchBltLin;
	dc->r.StretchBltSys             = draw2d.StretchBltSys;
	dc->r.ConvertBltSys             = draw2d.ConvertBltSys;
	dc->r.StretchConvertBltSys      = draw2d.StretchConvertBltSys;
	dc->r.BitBltFxTest              = draw2d.BitBltFxTest;
	dc->r.BitBltFx                  = draw2d.BitBltFx;
	dc->r.BitBltFxLin               = draw2d.BitBltFxLin;
	dc->r.BitBltFxSys               = draw2d.BitBltFxSys;
	dc->r.GetBitmapSys              = draw2d.GetBitmapSys;
	dc->r.solid.DrawScanList		= draw2d.DrawScanList;
	dc->r.patt.DrawScanList			= draw2d.DrawPattScanList;
	dc->r.colorPatt.DrawScanList    = draw2d.DrawColorPattScanList;
	dc->r.solid.DrawEllipseList		= draw2d.DrawEllipseList;
	dc->r.patt.DrawEllipseList		= draw2d.DrawPattEllipseList;
	dc->r.colorPatt.DrawEllipseList = draw2d.DrawColorPattEllipseList;
	dc->r.solid.DrawFatEllipseList	= draw2d.DrawFatEllipseList;
	dc->r.patt.DrawFatEllipseList	= draw2d.DrawPattFatEllipseList;
	dc->r.colorPatt.DrawFatEllipseList = draw2d.DrawColorPattFatEllipseList;
	dc->r.solid.DrawRect			= draw2d.DrawRect;
	dc->r.patt.DrawRect				= draw2d.DrawPattRect;
	dc->r.colorPatt.DrawRect    	= draw2d.DrawColorPattRect;
	dc->r.solid.DrawTrap			= draw2d.DrawTrap;
	dc->r.patt.DrawTrap				= draw2d.DrawPattTrap;
	dc->r.colorPatt.DrawTrap    	= draw2d.DrawColorPattTrap;
}

/****************************************************************************
REMARKS:
Make the new device context the currently active device context for
rendering, which includes updating any necessary hardware state that
is cached in the Nucleus drivers.
****************************************************************************/
void MGLAPI DRV_makeCurrent(
	MGLDC *dc,
	ibool partial)
{
	int	i;

	/* Now update those attributes that are cached in Nucleus */
	if (!partial) {
		dc->r.SetMix(dc->a.writeMode);
		dc->r.SetForeColor(dc->a.color);
		dc->r.SetBackColor(dc->a.backColor);
		for (i = 0; i < 8; i++) {
			dc->r.Set8x8MonoPattern(i,GAMONOPAT(&dc->a.penPat[i]));
			dc->r.Set8x8ColorPattern(i,GACLRPAT(&dc->a.penPixPat[i]));
			}
		if (dc->a.penStyle == MGL_BITMAP_OPAQUE)
			dc->r.Use8x8MonoPattern(dc->a.cntPenPat);
		else if (dc->a.penStyle == MGL_BITMAP_TRANSPARENT)
			dc->r.Use8x8TransMonoPattern(dc->a.cntPenPat);
		else if (dc->a.penStyle == MGL_PIXMAP)
			dc->r.Use8x8ColorPattern(dc->a.cntPenPixPat);
		else if (dc->a.penStyle == MGL_PIXMAP_TRANSPARENT)
			dc->r.Use8x8TransColorPattern(dc->a.cntPenPixPat,dc->a.penPixPatTrans);
		dc->r.SetLineStipple(dc->a.lineStipple);
		dc->r.SetLineStippleCount(dc->a.stippleCount);
		}
	dc->r.SetPlaneMask(dc->a.planeMask);
	dc->r.SetSrcBlendFunc(dc->a.srcBlendFunc);
	dc->r.SetDstBlendFunc(dc->a.dstBlendFunc);
	dc->r.SetAlphaValue(dc->a.alphaValue);
	dc->r.SetDrawSurface(dc->surface,dc->mi.xRes+1,dc->mi.yRes+1,dc->mi.bytesPerLine,dc->mi.bitsPerPixel,GAPF(&dc->pf));
}
