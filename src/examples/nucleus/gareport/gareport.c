/****************************************************************************
*
*                       SciTech Nucleus Report program
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
* Description:  Simple program that reports the detected hardware
*               configuration from Nucleus (to be used in scripts).
*
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "nucleus/gasdk.h"

/*----------------------------- Implementation ----------------------------*/

int main(int argc,char *argv[])
{
	int				deviceIndex = 0;
	GA_devCtx		*dc;
	GA_initFuncs	init;
	GA_configInfo	info;

	// Get device index from command line
	if (argc > 1)
		deviceIndex = atoi(argv[1]);

	// Load the device driver for this device
	if ((dc = GA_loadDriver(deviceIndex,false)) == NULL)
		PM_fatalError(GA_errorMsg(GA_status()));
	init.dwSize = sizeof(init);
	if (!dc->QueryFunctions(GA_GET_INITFUNCS,0,&init))
		PM_fatalError("Unable to get device driver functions!");
	info.dwSize = sizeof(info);
	init.GetConfigInfo(&info);
	printf(
		"Manufacturer:    %s\n"
		"Chipset:         %s\n"
		"Memory:          %d Kb\n"
		"DAC:             %s\n"
		"Clock:           %s\n"
		"Driver Revision: %s\n"
		"Driver Build:    %s\n",
		info.ManufacturerName, info.ChipsetName, dc->TotalMemory,
		info.DACName, info.ClockName, info.VersionInfo, info.BuildDate
		);
	GA_unloadDriver(dc);
	return 0;
}
