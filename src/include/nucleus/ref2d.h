/****************************************************************************
*
*           		SciTech Nucleus Graphics Architecture
*
*               Copyright (C) 1991-1998 SciTech Software, Inc.
*                            All rights reserved.
*
*  ======================================================================
*  |REMOVAL OR MODIFICATION OF THIS HEADER IS STRICTLY PROHIBITED BY LAW|
*  |                                                                    |
*  |This copyrighted computer code contains proprietary technology      |
*  |owned by SciTech Software, Inc., located at 505 Wall Street,        |
*  |Chico, CA 95928 USA (http://www.scitechsoft.com).                   |
*  |                                                                    |
*  |The contents of this file are subject to the SciTech Nucleus        |
*  |License; you may *not* use this file or related software except in  |
*  |compliance with the License. You may obtain a copy of the License   |
*  |at http://www.scitechsoft.com/nucleus-license.txt                   |
*  |                                                                    |
*  |Software distributed under the License is distributed on an         |
*  |"AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or      |
*  |implied. See the License for the specific language governing        |
*  |rights and limitations under the License.                           |
*  |                                                                    |
*  |REMOVAL OR MODIFICATION OF THIS HEADER IS STRICTLY PROHIBITED BY LAW|
*  ======================================================================
*
* Language:     ANSI C
* Environment:  Any
*
* Description:  Header file for the Nucleus 2D Reference Rasteriser.
*
****************************************************************************/

#ifndef __NUCLEUS_REF2D_H
#define __NUCLEUS_REF2D_H

#include "nucleus/graphics.h"
#include "drvlib/peloader.h"

/*---------------------- Macros and type definitions ----------------------*/

#pragma pack(4)

/* Function pointer prototypes */

typedef GA_color (*REF2D_readPixel_t)(uchar *addr);
typedef void (*REF2D_drawPixel_t)(uchar *addr,GA_color color);
typedef void (*REF2D_drawPixelR3_t)(uchar *addr,GA_color color,GA_color pattern);
typedef GA_color (*REF2D_blendColor_t)(GA_color S,GA_color D);

/****************************************************************************
REMARKS:
2D refrence rasteriser functions returned by the REF2D_loadDriver function.
{secret}
****************************************************************************/
typedef struct _REF2D_driver {
	ibool	(NAPIP QueryFunctions)(N_uint32 id,void *funcs);
	N_int32	(NAPIP SetDrawBuffer)(GA_buffer *drawBuf,void *framebuffer,N_int32 bitsPerPixel,GA_pixelFormat *pf,GA_devCtx *hwCtx,N_int32 softwareOnly);
	void	(NAPIP SetDrawSurface)(void *surface,N_int32 xRes,N_int32 yRes,N_int32 bytesPerLine,N_int32 bitsPerPixel,GA_pixelFormat *pf);
	void * 	(NAPIP RotateBitmap)(void *src,N_int32 bitsPerPixel,N_int32 *stride,N_int32 width,N_int32 height);
	void	(NAPIP MapShared)(void);
	void	(NAPIP UnloadDriver)(void);
	ibool	(NAPIP InitFilter)(GA_modeInfo *modeInfo,N_int32 transferStart,struct _REF2D_driver *ref2d,struct _REF2D_driver **drv);
	void	(NAPIP DeInitFilter)(void);
	void	(NAPIP GlobalOptionsUpdated)(void);

	/* Internal pointer to the driver state buffer - used by filter drivers */
	void				_FAR_ *DriverState;

	/* Pointer to the start of driver and size of driver in bytes */
	void				_FAR_ *DriverStart;
	N_uint32			DriverSize;

	/* Currently active pixel drawing functions (register calling conventions!) */
	REF2D_readPixel_t	readPixel;
	REF2D_drawPixel_t	drawPixel;
	REF2D_drawPixel_t	drawPixelTable[16];
	REF2D_drawPixelR3_t	drawPixelR3;
	REF2D_drawPixelR3_t	drawPixelR3Table[256];
	REF2D_blendColor_t	blendSrcTable[16];
	REF2D_blendColor_t	blendDstTable[16];
	void 				(*drawPixelBlended)(uchar *addr,GA_color color);
	void 				(*drawPixelBlendedClamped)(uchar *addr,GA_color color);
	GA_color			(*blendSrc)(GA_color S,GA_color D);
	GA_color			(*blendDst)(GA_color S,GA_color D);
	} REF2D_driver;

#pragma pack()

typedef ibool (NAPIP GA_setActiveDevice_t)(N_int32 deviceIndex);

#ifndef	__16BIT__
/****************************************************************************
REMARKS:
Function pointer type for the Binary Portable DLL initialisation entry point.
{secret}
****************************************************************************/
typedef REF2D_driver * (NAPIP REF2D_initLibrary_t)(
	GA_devCtx *hwCtx,
	ibool shared,
	GA_globalOptions *opt,
	PM_imports *pmImp);

/****************************************************************************
REMARKS:
Function pointer type for the Binary Portable DLL initialisation entry point.
{secret}
****************************************************************************/
typedef REF2D_driver * (NAPIP MULDISP_initLibrary_t)(
	GA_devCtx **hwCtx,
	N_int32 numDevices,
	ibool shared,
	GA_globalOptions *opt,
	PM_imports *pmImp,
	GA_setActiveDevice_t setActiveDevice);

/****************************************************************************
REMARKS:
Function pointer type for the Binary Portable DLL initialisation entry point.
{secret}
****************************************************************************/
typedef ibool (NAPIP MULDISP_InitFilterExt_t)(
	GA_modeInfo *modeInfo,
	N_int32 transferStart,
	REF2D_driver **ref2d,
	REF2D_driver **drv);
#endif

/*-------------------------- Function Prototypes --------------------------*/

#ifdef  __cplusplus
extern "C" {            			/* Use "C" linkage when in C++ mode */
#endif

ibool	NAPI REF2D_loadDriver(GA_devCtx _FAR_ *hwCtx,N_int32 bitsPerPixel,N_int32 shared,REF2D_driver _FAR_ *_FAR_ *drv,PE_MODULE _FAR_ *_FAR_ *hModule,ulong _FAR_ *size);
void	NAPI REF2D_unloadDriver(REF2D_driver _FAR_ *drv,PE_MODULE _FAR_ *hModule);
ibool 	NAPI SHADOW_loadDriver(GA_devCtx _FAR_ *hwCtx,N_int32 shared,GA_globalOptions *opt,REF2D_driver _FAR_ * _FAR_ *drv,PE_MODULE _FAR_ * _FAR_ *hModule,ulong _FAR_ *size);
ibool 	NAPI PORTRAIT_loadDriver(GA_devCtx _FAR_ *hwCtx,N_int32 shared,GA_globalOptions *opt,REF2D_driver _FAR_ * _FAR_ *drv,PE_MODULE _FAR_ * _FAR_ *hModule,ulong _FAR_ *size);
ibool 	NAPI FLIPPED_loadDriver(GA_devCtx _FAR_ *hwCtx,N_int32 shared,GA_globalOptions *opt,REF2D_driver _FAR_ * _FAR_ *drv,PE_MODULE _FAR_ * _FAR_ *hModule,ulong _FAR_ *size);
ibool 	NAPI INVERT_loadDriver(GA_devCtx _FAR_ *hwCtx,N_int32 shared,GA_globalOptions *opt,REF2D_driver _FAR_ * _FAR_ *drv,PE_MODULE _FAR_ * _FAR_ *hModule,ulong _FAR_ *size);
ibool 	NAPI MULDISP_loadDriver(GA_devCtx _FAR_ * _FAR_ *hwCtx,N_int32 numDevices,N_int32 shared,GA_globalOptions *opt,REF2D_driver _FAR_ * _FAR_ *drv,PE_MODULE _FAR_ * _FAR_ *hModule,ulong _FAR_ *size,GA_setActiveDevice_t setActiveDevice);
#ifdef	__DRIVER__
REF2D_driver * _CEXPORT REF2D_initLibrary(GA_devCtx *hwCtx,ibool shared,GA_globalOptions *opt,PM_imports *pmImp);
REF2D_driver * _CEXPORT SHADOW_initLibrary(GA_devCtx *hwCtx,ibool shared,GA_globalOptions *opt,PM_imports *pmImp);
REF2D_driver * _CEXPORT PORTRAIT_initLibrary(GA_devCtx *hwCtx,ibool shared,GA_globalOptions *opt,PM_imports *pmImp);
REF2D_driver * _CEXPORT FLIPPED_initLibrary(GA_devCtx *hwCtx,ibool shared,GA_globalOptions *opt,PM_imports *pmImp);
REF2D_driver * _CEXPORT INVERT_initLibrary(GA_devCtx *hwCtx,ibool shared,GA_globalOptions *opt,PM_imports *pmImp);
REF2D_driver * _CEXPORT MULDISP_initLibrary(GA_devCtx **hwCtx,N_int32 numDevices,ibool shared,GA_globalOptions *opt,PM_imports *pmImp,GA_setActiveDevice_t setActiveDevice);
ibool	_CEXPORT MULDISP_InitFilterExt(GA_modeInfo *modeInfo,N_int32 transferStart,REF2D_driver **ref2d,REF2D_driver **drv);
#else
ibool	NAPI MULDISP_InitFilterExt(GA_modeInfo *modeInfo,N_int32 transferStart,REF2D_driver **ref2d,REF2D_driver **drv);
#endif

#ifdef  __cplusplus
}                       			/* End of "C" linkage for C++   	*/
#endif

#endif  /* __NUCLEUS_REF2D_H */
