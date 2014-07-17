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
* Description:	Header file describing the OS specific driver import
*				functions, which are passed into the loaded drivers by the
*				OS specific loader code to implement OS specific features.
*
****************************************************************************/

#ifndef	__DRIVERS_COMMON_IMPORTS_H
#define	__DRIVERS_COMMON_IMPORTS_H

#include "drvlib/peloader.h"

/*---------------------- Macro and type definitions -----------------------*/

typedef struct {
	/* Size of the driver structures */
	long		dwSize;

	/* Pointers to useful information passed into driver */
	drivertype	*drvPacked;
	void		*_MGL_buf;

	/* OS specific memory allocation functions */
	void * 	(MGLAPI *MGL_malloc)(long size);
	void *	(MGLAPI *MGL_calloc)(long s,long n);
	void 	(MGLAPI *MGL_free)(void *p);

	/* OS specific bitmap manipulation functions */
	long 	(MGLAPI *MGL_allocateDIB)(MGLDC *dc,winBITMAPINFO *info);
	void 	(MGLAPI *MGL_freeDIB)(MGLDC *dc);
	void	(MGLAPI *PACKED8_realizePalette)(MGLDC *dc,palette_t *pal,int num,int index,int waitVRT);
	} DRV_imports;

/* Macros to calls functions via our import table for the driver code */

#define	MGL_malloc(s)				_DRV_imports.MGL_malloc(s)
#define	MGL_calloc(s,n)				_DRV_imports.MGL_calloc(s,n)
#define	MGL_free(p)					_DRV_imports.MGL_free(p)
#define MGL_allocateDIB(dc,info)	_DRV_imports.MGL_allocateDIB(dc,info)
#define MGL_freeDIB(dc)				_DRV_imports.MGL_freeDIB(dc)

/* Define a macro to easily access global device context */

#define	DC				_MGL_dc

typedef drivertype * (_CDECL *MGL_initDriver_t)(PM_imports *pmImp,N_imports *nImp,GA_imports *gaImp,DRV_imports *drvImp);

/*--------------------------- Global Variables ----------------------------*/

extern PM_imports	_VARAPI *_PM_imports;
extern N_imports	_VARAPI *_N_imports;
extern GA_imports	_VARAPI *_GA_imports;
extern DRV_imports	_VARAPI _DRV_imports;
extern MGLDC		_VARAPI _MGL_dc;
extern drivertype	DriverHeader;

#endif	/* __DRIVERS_COMMON_IMPORTS_H */
