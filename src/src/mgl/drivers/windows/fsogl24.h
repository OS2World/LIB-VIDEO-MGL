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
* Description:	Header file for the 24 bit accelerated driver.
*
****************************************************************************/

#ifndef	__DRIVERS_FSOPENGL_FSOPENGL24_H
#define	__DRIVERS_FSOPENGL_FSOPENGL24_H

#ifndef	__DRIVERS_COMMON_GOPENGL_H
#include "drivers/common/gopengl.h"
#endif

#ifndef	__DRIVERS_PACKED_PACKED24_H
#include "drivers/packed/packed24.h"
#endif

/*------------------------- Function Prototypes ---------------------------*/

/* 8 bit C based routines */

ibool 	MGLAPI FSOPENGL24_detect(int id,int *force,int *driver,int *mode,modetab availableModes);
ibool 	MGLAPI FSOPENGL24_initDriver(MGLDC *dc,int driverId,int modeId,ulong hwnd,int virtualX,int virtualY,int numBuffers,ibool stereo,int refreshRate,ibool useLinearBlits);

#endif	/* __DRIVERS_FSOPENGL_FSOPENGL24_H */

