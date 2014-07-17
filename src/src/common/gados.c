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
* Environment:  MSDOS
*
* Description:  OS specific Nucleus Graphics Architecture services for
*				the MSDOS operating system.
*
****************************************************************************/

#include "pm_help.h"
#include "pmapi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*-------------------------- Implementation -------------------------------*/

/****************************************************************************
REMARKS:
This function returns a pointer to the common graphics driver loaded in the
helper VxD. The memory for the VxD is shared between all processes via
the VxD, so that the VxD, 16-bit code and 32-bit code all see the same
state when accessing the graphics binary portable driver.
****************************************************************************/
GA_sharedInfo * NAPI GA_getSharedInfo(
	int device)
{
	VXD_regs	regs;

	if (PMHELP_getVersion() == 0)
		return NULL;
	memset(&regs,0,sizeof(regs));
	regs.eax = PMHELP_GETSHAREDINFO;
	regs.ebx = device;
	PM_VxDCall(&regs);
	if (regs.eax != PMHELP_GETSHAREDINFO)
		return (GA_sharedInfo*)regs.eax;
	return NULL;
}

/****************************************************************************
REMARKS:
Nothing special for this OS.
****************************************************************************/
ibool NAPI GA_getSharedExports(
	GA_exports *gaExp)
{
	(void)gaExp;
	return false;
}

/****************************************************************************
REMARKS:
Nothing special for this OS.
****************************************************************************/
void NAPI _GA_postLoadDriver(void)
{
}

/****************************************************************************
REMARKS:
This function initialises the high precision timing functions for the DOS
Nucleus loader library.
****************************************************************************/
ibool NAPI GA_TimerInit(void)
{
	if (_GA_haveCPUID() && (_GA_getCPUIDFeatures() & CPU_HaveRDTSC) != 0)
		return true;
	return false;
}

/****************************************************************************
REMARKS:
This function reads the high resolution timer.
****************************************************************************/
void NAPI GA_TimerRead(
	GA_largeInteger *value)
{
	_GA_readTimeStamp(value);
}
