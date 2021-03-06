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
* Description:	Header file for the SciTech VGA and VBE dumb framebuffer
*				driver. We ignore any accelerated graphics modes in this
*				driver, and can support VGA, VBE 1.2 banked and VBE 2.0/3.0
*				linear modes.
*
****************************************************************************/

#ifndef	__DRIVERS_NUCLEUS_NUCLVBE_H
#define	__DRIVERS_NUCLEUS_NUCLVBE_H

#include "drivers/common/gnucleus.h"

/*------------------------- Function Prototypes ---------------------------*/

void *	MGLAPI NUCLVBE_createInstance(void);
ibool 	MGLAPI NUCLVBE_detect(void *data,int id,int *numModes,modetab availableModes);
ibool 	MGLAPI NUCLVBE_initDriver(void *data,MGLDC *dc,modeent *mode,ulong hwnd,int virtualX,int virtualY,int numBuffers,ibool stereo,int refreshRate,ibool useLinearBlits);
void	MGLAPI NUCLVBE_destroyInstance(void *data);

#endif	/* __DRIVERS_NUCLEUS_NUCLVBE_H */
