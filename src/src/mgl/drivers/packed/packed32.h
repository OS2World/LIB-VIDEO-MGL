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
* Description:	32 bit packed pixel memory device driver header file.
*
****************************************************************************/

#ifndef	__DRIVERS_PACKED_PACKED32_H
#define	__DRIVERS_PACKED_PACKED32_H

#include "drivers/common/common.h"

/*---------------------- Macro and type definitions -----------------------*/

typedef struct {
	mem_vec	v;					/* Memory device vectors and info		*/
	} PACKED32_state;

/*------------------------- Function Prototypes ---------------------------*/

void *	MGLAPI PACKED32_createInstance(void);
ibool 	MGLAPI PACKED32_initDriver(void *data,MGLDC *dc,modeent *mode,ulong hwnd,int virtualX,int virtualY,int numBuffers,ibool stereo,int refreshRate,ibool useLinearBlits);
void	MGLAPI PACKED32_destroyInstance(void *data);

#endif	/* __DRIVERS_PACKED_PACKED32_H */
