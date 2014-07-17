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
* Description:	Header file for the windowed X11 driver.
*
****************************************************************************/

#ifndef	__DRIVERS_UNIX_XWINDC_H
#define	__DRIVERS_UNIX_XWINDC_H

#include "mglunix.h"
#include "mgldd.h"

#include "drivers/common/gunix.h"

typedef struct {
	// What the hell do I have to put here ?
	Display *dpy;
	Window  w;
	//	XImage  *img;
} XWINDC_data;

/*------------------------- Function Prototypes ---------------------------*/

/* 8 bit C based routines */

ibool 	MGLAPI XWINDC_initDriver(void *data,MGLDC *dc,int driverId,int modeId,ulong hwnd,int virtualX,int virtualY,int numBuffers,ibool stereo,int refreshRate,ibool useLinearBlits);
void   *MGLAPI XWINDC_createInstance(void);
void    MGLAPI XWINDC_destroyInstance(void *data);

#endif /* __DRIVERS_UNIX_XWIN8_H */
