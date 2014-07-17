/****************************************************************************
*
*                SciTech Nucleus Command Line Options program
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
* Description:  Simple program that allows the Nucleus opt to be
*				changed from the command line.
*
****************************************************************************/

#include "nucleus/graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*---------------------------- Global Variables ---------------------------*/

static GA_devCtx		*dc = NULL;
static GA_initFuncs		init;
static GA_configInfo	info;
static GA_options		opt;
static GA_globalOptions	gOpt;

/*----------------------------- Implementation ----------------------------*/

/****************************************************************************
REMARKS:
Display the sign on banner.
****************************************************************************/
static void banner(void)
{
	printf("GAOption - SciTech Nucleus Option Selection Program\n");
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
	printf("  gaoption show     <device>\n");
	printf("  gaoption invert   [on|off] <device>\n");
	printf("  gaoption portrait [on|off] <device>\n");
	printf("  gaoption flipped  [on|off] <device>\n");
	printf("  gaoption prefer16 [on|off] <device>\n");
	printf("  gaoption prefer32 [on|off] <device>\n");
	printf("  gaoption vbe      [on|off]\n");
	printf("  gaoption vga      [on|off]\n");
	printf("  gaoption noncert  [on|off]\n");
	printf("  gaoption nowcomb  [on|off]\n");
	printf("  gaoption virtual  [on|off] <[xRes] [yRes] [horz count] [vert count]>\n");
	exit(-1);
}

/****************************************************************************
REMARKS:
Load the global device driver options.
****************************************************************************/
static void LoadGlobalOptions(void)
{
	gOpt.dwSize = sizeof(gOpt);
	GA_getGlobalOptions(&gOpt);
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
		opt.dwSize = sizeof(opt);
		init.GetOptions(&opt);
		LoadGlobalOptions();
		}
}

/****************************************************************************
REMARKS:
Function to enable virtual display mode. This is a simple function that
can enable any number of displays in a rectangular layout with each
display using the same resolution.
****************************************************************************/
void EnableVirtualMode(
	int argc,
	char *argv[])
{
	int x,y,i,xRes,yRes,numX,numY;

	if (argc != 7)
		help();
	xRes = atoi(argv[3]);
	yRes = atoi(argv[4]);
	numX = atoi(argv[5]);
	numY = atoi(argv[6]);
	gOpt.virtualSize.left = gOpt.virtualSize.top = 0;
	gOpt.virtualSize.right = xRes * numX;
	gOpt.virtualSize.bottom = yRes * numY;
	memset(&gOpt.resolutions,0,sizeof(gOpt.resolutions));
	memset(&gOpt.bounds,0,sizeof(gOpt.bounds));
	for (i = x = 0; x < numX; x++) {
		for (y = 0; y < numY; y++, i++) {
			gOpt.resolutions[i].left = gOpt.resolutions[i].top = 0;
			gOpt.resolutions[i].right = xRes;
			gOpt.resolutions[i].bottom = yRes;
			gOpt.bounds[i].left = x * xRes;
			gOpt.bounds[i].top = y * yRes;
			gOpt.bounds[i].right = (x+1) * xRes;
			gOpt.bounds[i].bottom = (y+1) * yRes;
			}
		}
	gOpt.bVirtualDisplay = true;
}

/****************************************************************************
REMARKS:
Show the current settings for the device.
****************************************************************************/
static void ShowGlobalSettings(void)
{
	int	i;

	LoadGlobalOptions();
	printf(
		"Global options for all devices:\n\n"
		"  Force VBE Fallback ...... %s\n"
		"  Force VGA Fallback ...... %s\n"
		"  Allow non-certified ..... %s\n"
		"  Disable write combining . %s\n",
		gOpt.bVBEOnly     	    ? "Yes" : "No",
		gOpt.bVGAOnly           ? "Yes" : "No",
		gOpt.bAllowNonCertified ? "Yes" : "No",
		gOpt.bNoWriteCombine    ? "Yes" : "No");
	if (gOpt.bVirtualDisplay) {
		printf("  Virtual Display.......... Yes\n");
		for (i = 0; i < GA_MAX_VIRTUAL_DISPLAYS; i++) {
			if (gOpt.bounds[i].right == 0)
				break;
			printf("    %2d: %4d x %4d (%4d,%4d,%4d,%4d)\n",
				i,
				gOpt.resolutions[i].right,
				gOpt.resolutions[i].bottom,
				gOpt.bounds[i].left,
				gOpt.bounds[i].top,
				gOpt.bounds[i].right,
				gOpt.bounds[i].bottom);
			}
		}
	else
		printf("  Virtual Display.......... No\n");
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
		"Options for %s %s (device %d):\n\n"
		"  Invert .................. %s\n"
		"  Portrait ................ %s\n"
		"  Flipped ................. %s\n"
		"  Prefer 16 bit per pixel.. %s\n"
		"  Prefer 32 bit per pixel.. %s\n\n",
		info.ManufacturerName, info.ChipsetName,
		deviceIndex,
		opt.bInvertColors 	    ? "Yes" : "No",
		opt.bPortrait     	    ? "Yes" : "No",
		opt.bFlipped      	    ? "Yes" : "No",
		opt.bPrefer16bpp		? "Yes" : "No",
		opt.bPrefer32bpp		? "Yes" : "No");
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
		else if (tolower(argv[2][0]) == 'g')
			ShowGlobalSettings();
		else
			ShowSettings(atoi(argv[2]));
		}
	else if (argc >= 3) {
		ibool 	enable = stricmp(argv[2],"on") == 0;
		int 	device = (argc == 3) ? 0 : atoi(argv[3]);

		LoadGlobalOptions();
		if (stricmp(argv[1],"vbe") == 0)
			gOpt.bVBEOnly = enable;
		else if (stricmp(argv[1],"vga") == 0)
			gOpt.bVGAOnly = enable;
		else if (stricmp(argv[1],"noncert") == 0)
			gOpt.bAllowNonCertified = enable;
		else if (stricmp(argv[1],"nowcomb") == 0)
			gOpt.bNoWriteCombine = enable;
		else if (stricmp(argv[1],"virtual") == 0) {
			if (enable)
				EnableVirtualMode(argc,argv);
			else
				gOpt.bVirtualDisplay = false;
			}
		else {
			LoadDriver(device);
			if (stricmp(argv[1],"invert") == 0)
				opt.bInvertColors = enable;
			else if (stricmp(argv[1],"portrait") == 0)
				opt.bPortrait = enable;
			else if (stricmp(argv[1],"flipped") == 0)
				opt.bFlipped = enable;
			init.SetOptions(&opt);
			GA_saveOptions(dc,&opt);
			ShowSettings(device);
			}
		GA_saveGlobalOptions(&gOpt);
		ShowGlobalSettings();
		}
	else
		help();
	if (dc)
		GA_unloadDriver(dc);
	return 0;
}
