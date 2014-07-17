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
* Environment:  Any 32-bit protected mode environment
*
* Description:  Module to implement the code to test the Nucleus Graphics
*				Architecture.
*
****************************************************************************/

#ifndef	__NUCLEUS_PROFILE_H
#define	__NUCLEUS_PROFILE_H

#include "nucleus/gasdk.h"

/*---------------------- Macros and type definitions ----------------------*/

typedef struct {
    float   intLinesPerSec;
	float   bresLinesPerSec;
	float   intStippleLinesPerSec;
	float   bresStippleLinesPerSec;
	float   clearsMbPerSec;
    float   monoPattMbPerSec;
    float   transMonoPattMbPerSec;
    float   colorPattMbPerSec;
	float   transColorPattMbPerSec;
	float	putMonoImageMbPerSec;
	float   bitBltMbPerSec;
	float   bitBltSysMbPerSec;
	float   readsCPUMbPerSec;
	float   getImagesCPUMbPerSec;
	float   putImagesCPUMbPerSec;
	float   clearCPUMbPerSec;
	float   reverseClearCPUMbPerSec;
    float   clearBaseLineMbPerSec;
    float   reverseClearBaseLineMbPerSec;
    float   readBaseLineMbPerSec;
    float   copyBaseLineMbPerSec;
    ibool   thrashCache;
    } ProfileRecord;

/*---------------------------- Global Variables ---------------------------*/

#ifdef  __cplusplus
extern "C" {            /* Use "C" linkage when in C++ mode */
#endif

extern GA_initFuncs    init;

/*------------------------- Function Prototypes ---------------------------*/

/* Function to profile a particular display mode */

ibool ProfileIt(int x,int y,int bits,char *testName,ProfileRecord *R);
ibool ProfileHWOnly(int x,int y,int bits,ProfileRecord *R);

/* Function to compute a GA-Mark from the profile results */

float ComputeGAMark(ProfileRecord *R);

#ifdef  __cplusplus
}                       /* End of "C" linkage for C++   */
#endif

#endif	/* __NUCLEUS_PROFILE_H */
