/****************************************************************************
*
*               SciTech Nucleus Memory Clock Selection program
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
* Description:  Simple program that allows the memory clock settings to
*				be changed for SciTech Nucleus program.
*
****************************************************************************/

#include "nucleus/graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*---------------------------- Global Variables ---------------------------*/

static GA_devCtx		*dc = NULL;
static GA_initFuncs		init;
static GA_configInfo	info;
static GA_options		options;

/*----------------------------- Implementation ----------------------------*/

/****************************************************************************
REMARKS:
Display the sign on banner.
****************************************************************************/
static void banner(void)
{
	printf("GAMemClk - SciTech Nucleus Memory Clock Selection Program\n");
	printf("           Copyright (C) 1998-1999 SciTech Software, Inc.\n");
	printf("\n");
}

/****************************************************************************
REMARKS:
Display the help screen.
****************************************************************************/
static void help(void)
{
	banner();
	printf("Valid commands are:\n");
	printf("\n");
	printf("  gamemclk show [device]\n");
	printf("\n");
	printf("     To show the current settings for the device.\n");
	printf("\n");
	printf("  gamemclk set <memory clock> [device]\n");
	printf("\n");
	printf("     To change the current memory clock for the device.\n");
	exit(-1);
}

/****************************************************************************
REMARKS:
Load the Nucleus driver and intialise it.
****************************************************************************/
static void LoadDriver(
	int deviceIndex)
{
	if (!dc) {
		if ((dc = GA_loadDriver(deviceIndex,false)) == NULL)
			PM_fatalError(GA_errorMsg(GA_status()));
		init.dwSize = sizeof(init);
		if (!dc->QueryFunctions(GA_GET_INITFUNCS,0,&init))
			PM_fatalError("Unable to get device driver functions!");
		info.dwSize = sizeof(info);
		init.GetConfigInfo(&info);
		options.dwSize = sizeof(options);
		init.GetOptions(&options);
		}
}

/****************************************************************************
REMARKS:
Show the current settings for the device.
****************************************************************************/
static void ShowSettings(
	int deviceIndex)
{
	LoadDriver(deviceIndex);
	printf(
		"Memory clock settings for %s %s (device %d):\n\n"
		"  Default: %.2f Mhz\n"
		"  Current: %.2f Mhz\n"
		"  Maximum: %.2f Mhz\n",
		info.ManufacturerName, info.ChipsetName,
		deviceIndex,
		options.defaultMemoryClock / 65536.0,
		options.memoryClock / 65536.0,
		options.maxMemoryClock / 65536.0);
}

/****************************************************************************
REMARKS:
Show the current settings for the device.
****************************************************************************/
static void SetMemClock(
	float memClock,
	int deviceIndex)
{
	N_fix32	fxMemClock = N_REAL(memClock);

	LoadDriver(deviceIndex);
	if (fxMemClock > options.maxMemoryClock)
		printf("Memory clock too high!\n");
	else {
		options.memoryClock = fxMemClock;
		init.SetOptions(&options);
		GA_saveOptions(dc,&options);
		ShowSettings(deviceIndex);
		}
}

/****************************************************************************
REMARKS:
Main program entry point
****************************************************************************/
int main(
	int argc,
	char *argv[])
{
	/* Now handle the query */
	if (argc >= 2 && stricmp(argv[1],"show") == 0) {
		if (argc == 2)
			ShowSettings(0);
		else
			ShowSettings(atoi(argv[2]));
		}
	else if (argc >= 3 && stricmp(argv[1],"set") == 0) {
		if (argc == 3)
			SetMemClock(atof(argv[2]),0);
		else
			SetMemClock(atof(argv[2]),atoi(argv[3]));
		}
	else
		help();
	if (dc)
		GA_unloadDriver(dc);
	return 0;
}
