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
* Description:	Common header file for internal device driver code used
*				by all device drivers.
*
****************************************************************************/

#ifndef	__DRIVERS_COMMON_COMMON_H
#define	__DRIVERS_COMMON_COMMON_H

/*---------------------- Macro and type definitions -----------------------*/

#define	DRV_useMode(modes,mode,id,_xRes,_yRes,_bits,pgs,flgs)	\
   {if (modes[mode].driver == 0xFF) {							\
	modes[mode].driver = id;									\
	modes[mode].xRes = _xRes;									\
	modes[mode].yRes = _yRes;									\
	modes[mode].bits = _bits;									\
	modes[mode].pages = pgs;									\
	modes[mode].flags = flgs;}}

#define	DRV_removeMode(modes,mode)			\
   {modes[mode].driver = 0xFF;				\
	modes[mode].xRes = 0;					\
	modes[mode].yRes = 0;					\
	modes[mode].bits = 0;					\
	modes[mode].pages = 0;					\
	modes[mode].flags = 0;}

#define	SET_PTR(d,p)	*((ulong*)&(d)) = (ulong)(p)

#define	PACKED24_pixelAddr(x,y)	\
	 ((void*)((uchar*)DC.surface + ((long)y * DC.mi.bytesPerLine) + x*3))

#define	PACKED24_pixelAddr2(dc,x,y)	\
	 ((void*)((uchar*)(dc)->surface + ((long)y * (dc)->mi.bytesPerLine) + x*3))

#define	MGL_packColorRGB(R,G,B)										\
 (((R) & 0xFF0000L) | (((G) >> 8) & 0xFF00L) | ((B) >> 16))

#define	MGL_packColorBGR(R,G,B)										\
 (((B) & 0xFF0000L) | (((G) >> 8) & 0xFF00L) | ((R) >> 16))

extern palette_t _VARAPI _VGA8_defPal[256];
extern palette_t _VARAPI _VGA4_defPal[16];

/*------------------------- Function Prototypes ---------------------------*/

/* Generic device driver C helper routines */

void	DRV_setNucleusRenderingVectors(MGLDC *dc,REF2D_driver *ref2d);
void 	MGLAPI DRV_makeCurrent(MGLDC *dc,ibool partial);
ulong	MGLAPI DRV_getWinDC(MGLDC *dc);
void 	* MGLAPI DRV_getDefaultPalette(MGLDC *dc);

#endif	/* __DRIVERS_COMMON_COMMON_H */
