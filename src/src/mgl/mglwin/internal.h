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
* Description:	Internal header file for the Win32 bindings for the MGL.
*
****************************************************************************/

#ifndef	__MGLWIN_INTERNAL_H
#define	__MGLWIN_INTERNAL_H

#define	STRICT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#define NONAMELESSUNION
#include "mgl\ddraw.h"

/*---------------------- Macro and type definitions -----------------------*/

/* Custom window messages */

#define	WM_DO_SUSPEND_APP			WM_USER
#define	WM_MGL_DEACTIVATE			0
#define	WM_MGL_RESTORE_FULLSCREEN	1
#define	WM_MGL_START_CHECK_SWITCH	2

/* Windows LOGPALETTE palette structure */

typedef struct {
	WORD 			version;
	WORD 			entries;
	PALETTEENTRY	pe[256];
	} LogPal;

/* Windows internal HBITMAP structure */

typedef struct {
	short		bmType;             /* 0 for main memory                    */
	ushort 		bmWidth;     		/* Width of bitmap in pixels        	*/
	ushort 		bmHeight;    		/* Height of bitmap in pixels			*/
	ushort		bmWidthBytes;		/* #bytes per scan line					*/
	uchar		bmPlanes;			/* # of planes in bitmap				*/
	uchar		bmBitsPixel;		/* # bits per pixel						*/
	uchar		*bmBits;			/* pointer to bits of main mem bitmap	*/
	ulong		bmWidthPlanes;		/* bmWidthBytes * bmHeight				*/
	uchar		*bmlpPDevice;		/* Pointer to associated pDevice		*/
	ushort		bmSegmentIndex;		/* Index to planes next segment 		*/
	ushort		bmScanSegment;		/* Number of scans per segment			*/
	ushort		bmFillBytes;		/* Number of unused bytes per segment	*/
	} win_BITMAP;

/*--------------------------- Global Variables ----------------------------*/

/*------------------------- Function Prototypes ---------------------------*/

#endif	/* __MGLWIN_INTERNAL_H */
