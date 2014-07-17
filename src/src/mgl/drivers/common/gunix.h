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
* Description:	Common header file for all Unix device drivers.
*
****************************************************************************/


#ifndef	__DRIVERS_COMMON_GUNIX_H
#define	__DRIVERS_COMMON_GUNIX_H

#include "drivers/common/common.h"

/*---------------------- Macro and type definitions -----------------------*/

/*------------------------- Function Prototypes ---------------------------*/

ibool MGLAPI  XWIN_detect(void);
void          XWIN_initInternal(MGLDC *dc);
void  MGLAPI  XWIN_useMode(modetab modes,int mode,int id,gmode_t *mi);
ulong MGLAPI XWIN_getWinDC(MGLDC *dc);
void * MGLAPI XWIN_getDefaultPalette(MGLDC *dc);
void   MGLAPI XWIN_realizePalette(MGLDC *dc,palette_t *pal,int num,int index,int waitVRT);
void MGLAPI XWIN_putImage(MGLDC *dc,int left,int top,int right,
						   int bottom,int dstLeft,int dstTop,int op,void *surface,
						   int bytesPerLine,MGLDC *src);


#endif
