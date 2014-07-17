/****************************************************************************
*
*               	  SciTech Nucleus Dial-A-Mode program
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
* Description:  Simple program that allows the user to add and remove
*				display modes from the SciTech Nucleus drivers.
*
****************************************************************************/

#include "nucleus/graphics.h"
#include "gtf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*--------------------------- Global variables ----------------------------*/

GA_initFuncs	init;
GA_driverFuncs	driver;

/*----------------------------- Implementation ----------------------------*/

/****************************************************************************
REMARKS:
Display the sign on banner.
****************************************************************************/
static void banner(void)
{
	printf("GAMode - SciTech Nucleus Dial-A-Mode Utility\n");
	printf("         Copyright (C) 1998-1999 SciTech Software, Inc.\n");
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
	printf("  gamode show [device]\n");
	printf("\n");
	printf("     To show the list of display modes for the device.\n");
	printf("\n");
	printf("  gamode add <xres> <yres> <bits> [device]\n");
	printf("\n");
	printf("     To add a new display mode.\n");
	printf("\n");
	printf("  gamode addref <xres> <yres> <refresh> [device]\n");
	printf("\n");
	printf("     To add a new refresh rate for a specific resolution. To specify an\n");
	printf("     interlaced refresh rate, use a negative value. Ie: -87 for 87Hz interlaced\n");
	printf("     mode.\n");
	printf("\n");
	printf("  gamode del <xres> <yres> <bits> [device]\n");
	printf("\n");
	printf("     To add a remove a specific display mode.\n");
	exit(-1);
}

/****************************************************************************
REMARKS:
Show the current settings for the device.
****************************************************************************/
static void ShowSettings(
	GA_devCtx *dc)
{
	GA_configInfo	info;
	GA_modeProfile	modes;
	GA_modeInfo		mi;
	N_uint16		*m;
	int				i,j;

	info.dwSize = sizeof(info);
	init.GetConfigInfo(&info);
	modes.dwSize = sizeof(modes);
	init.GetModeProfile(&modes);
	printf("Display modes for %s %s (device %d):\n\n",
		info.ManufacturerName, info.ChipsetName,
		dc->DeviceIndex);
	for (m = dc->AvailableModes; *m != 0xFFFF; m++) {
		mi.dwSize = sizeof(mi);
		if (init.GetVideoModeInfo(*m,&mi) != 0)
			continue;
		for (i = 0; i < modes.numModes; i++) {
			if (mi.Attributes & gaIsTextMode) {
				int xRes = mi.XResolution / mi.XCharSize;
				int yRes = mi.YResolution / mi.YCharSize;
				if (modes.modeList[i].xRes == xRes
					&& modes.modeList[i].yRes == yRes
					&& modes.modeList[i].bits == mi.BitsPerPixel) {
					printf("  %4d x %4d Text @ ",
						modes.modeList[i].xRes,
						modes.modeList[i].yRes);
					for (j = 0; mi.RefreshRateList[j] != -1; j++) {
						if (j > 0)
							printf(",");
						if (mi.RefreshRateList[j] < 0)
							printf("%di", -mi.RefreshRateList[j]);
						else
							printf("%d", mi.RefreshRateList[j]);
						}
					printf(" Hz\n");
					}
				}
			else if (modes.modeList[i].xRes == mi.XResolution
				&& modes.modeList[i].yRes == mi.YResolution
				&& modes.modeList[i].bits == mi.BitsPerPixel) {
				printf("  %4d x %4d x %2d @ ",
					modes.modeList[i].xRes,
					modes.modeList[i].yRes,
					modes.modeList[i].bits);
				for (j = 0; mi.RefreshRateList[j] != -1; j++) {
					if (j > 0)
						printf(",");
					if (mi.RefreshRateList[j] < 0)
						printf("%di", -mi.RefreshRateList[j]);
					else
						printf("%d", mi.RefreshRateList[j]);
					}
				printf(" Hz\n");
				}
			}
		}
}

/****************************************************************************
REMARKS:
Add a new mode to the driver.
****************************************************************************/
static void AddMode(
	GA_devCtx *dc,
	N_int32 xRes,
	N_int32 yRes,
	N_int32 bits)
{
	if (GA_addMode(dc,xRes,yRes,bits))
		printf("Mode %d x %d x %d successfully added.\n",xRes,yRes,bits);
	else
		printf("Failed to add mode %d x %d x %d!\n",xRes,yRes,bits);
}

/****************************************************************************
REMARKS:
Add a new refresh rate to the display driver.
****************************************************************************/
static void AddRefresh(
	GA_devCtx *dc,
	N_int32 xRes,
	N_int32 yRes,
	N_int32 refresh)
{
	if (GA_addRefresh(dc,xRes,yRes,refresh)) {
		if (refresh < 0) {
			printf("Mode %d x %d @ %d Hz interlaced successfully added.\n",xRes,yRes,refresh);
			}
		else {
			printf("Mode %d x %d @ %d Hz successfully added.\n",xRes,yRes,refresh);
			}
		}
	else
		printf("Could not add refresh rate %d x %d @ %d Hz!\n",xRes,yRes,refresh);
}

/****************************************************************************
REMARKS:
Delete a mode for the driver.
****************************************************************************/
static void DelMode(
	GA_devCtx *dc,
	N_int32 xRes,
	N_int32 yRes,
	N_int32 bits)
{
	if (GA_delMode(dc,xRes,yRes,bits))
		printf("Mode %d x %d x %d successfully deleted.\n",xRes,yRes,bits);
	else
		printf("Failed to delete mode %d x %d x %d!\n",xRes,yRes,bits);
}

/****************************************************************************
REMARKS:
Load the Nucleus driver and intialise it.
****************************************************************************/
static GA_devCtx *LoadDriver(
	int deviceIndex)
{
	GA_devCtx *dc;

	if ((dc = GA_loadDriver(deviceIndex,false)) == NULL)
		PM_fatalError(GA_errorMsg(GA_status()));
	init.dwSize = sizeof(init);
	if (!dc->QueryFunctions(GA_GET_INITFUNCS,0,&init))
		PM_fatalError("Unable to get device driver functions!");
	driver.dwSize = sizeof(driver);
	if (!dc->QueryFunctions(GA_GET_DRIVERFUNCS,0,&driver))
		PM_fatalError("Unable to get device driver functions!");
	return dc;
}

/****************************************************************************
REMARKS:
Main program entry point
****************************************************************************/
int main(
	int argc,
	char *argv[])
{
	GA_devCtx *dc = NULL;

	/* Now handle the query */
	if (argc >= 2 && stricmp(argv[1],"show") == 0) {
		dc = LoadDriver((argc > 2) ? atoi(argv[2]) : 0);
		ShowSettings(dc);
		}
	else if (argc >= 5 && stricmp(argv[1],"add") == 0) {
		dc = LoadDriver((argc > 5) ? atoi(argv[5]) : 0);
		AddMode(dc,atoi(argv[2]),atoi(argv[3]),atoi(argv[4]));
		}
	else if (argc >= 5 && stricmp(argv[1],"addref") == 0) {
		dc = LoadDriver((argc > 5) ? atoi(argv[5]) : 0);
		AddRefresh(dc,atoi(argv[2]),atoi(argv[3]),atoi(argv[4]));
		}
	else if (argc >= 5 && stricmp(argv[1],"del") == 0) {
		dc = LoadDriver((argc > 5) ? atoi(argv[5]) : 0);
		DelMode(dc,atoi(argv[2]),atoi(argv[3]),atoi(argv[4]));
		}
	else
		help();
	if (dc)
		GA_unloadDriver(dc);
	return 0;
}
