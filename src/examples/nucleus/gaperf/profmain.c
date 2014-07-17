/****************************************************************************
*
*                    SciTech Nucleus Performance Profiler
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
* Description:  Simple program to profile the speed of certain operations
*               for the SciTech Nucleus, Graphics Architecture. This is
*               a great way to test the performance of different SuperVGA
*               cards and different compiler configurations.
*
*               Note, this library uses the Zen Timer Library for
*               microsecond accuracy timing of the routines.
*
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "getopt.h"
#include "nucleus/profile.h"
#include "console.h"
#include "ztimer.h"

/*--------------------------- Global variables ----------------------------*/

static ibool	thrashCache = true;
static int		numDevices = 0;
static PM_HWND	hwndConsole;
static void    	*stateBuf;
static char		testName[80] = "All";
GA_initFuncs	init;
GA_driverFuncs	driver;

#define NO_BUILD
#include "sdd/sddver.c"

const char *gaGetReleaseDate(void)
{ return release_date; };

const char *gaGetReleaseDate2(void)
{ return release_date2; };

/*----------------------------- Implementation ----------------------------*/

/****************************************************************************
REMARKS:
Resets the active device to 0 and displays the fatal error message.
****************************************************************************/
void PMAPI FatalErrorCleanup(void)
{
	if (numDevices > 0)
		GA_setActiveDevice(0);
	CON_restoreMode();
	PM_restoreConsoleState(stateBuf,hwndConsole);
	PM_closeConsole(hwndConsole);
}

/****************************************************************************
REMARKS:
Display the command line help.
****************************************************************************/
void help(void)
{
	FatalErrorCleanup();
	printf("GAPerf - SciTech Nucleus Graphics Performance Profiler\n");
	printf("         Release %s.%s (%s)\n\n",release_major,release_minor,release_date);
	printf("%s\n",copyright_str);
	printf("\n");
	printf("Options are:\n");
	printf("    -l       - List available graphics modes\n");
	printf("    -t<name> - Run test 'name' - An empy name will list all tests\n");
	printf("    -c       - Thrash the system memory cache during BitBlt's\n");
	printf("    -s       - Force software only rendering (no acceleration)\n");
	printf("\n");
	printf("Usage: profile [-lt] <xres> <yres> <bits>\n");
	exit(1);
}

/****************************************************************************
REMARKS:
Display a list of available resolutions
****************************************************************************/
void AvailableModes(void)
{
    N_uint16    *modes;

    printf("Available graphics modes:\n");
    for (modes = dc->AvailableModes; *modes != 0xFFFF; modes++) {
        modeInfo.dwSize = sizeof(modeInfo);
		if (init.GetVideoModeInfo(*modes,&modeInfo) != 0)
            continue;
        if (modeInfo.Attributes & gaIsTextMode)
            continue;
        if (modeInfo.BitsPerPixel < 8)
            continue;
        printf(" %4d x %4d %d bits per pixel\n",
            modeInfo.XResolution, modeInfo.YResolution, modeInfo.BitsPerPixel);
        }
	FatalErrorCleanup();
    exit(1);
}

/****************************************************************************
REMARKS:
Parse the command line arguments.
****************************************************************************/
void parseArguments(
    int argc,
    char *argv[])
{
    int     option;
    char    *argument;

    /* Parse command line options */
    do {
		option = getopt(argc,argv,"lcst:",&argument);
        switch (option) {
            case 'l':
                AvailableModes();
                break;
			case 't':
				strcpy(testName,argument);
				break;
			case 'c':
                thrashCache = false;
				break;
			case 's':
				softwareOnly = true;
				break;
            case ALLDONE:
                break;
            case PARAMETER:
                break;
            case 'h':
            case INVALID:
            default:
                help();
            }
        } while (option != ALLDONE && option != PARAMETER);
}

/****************************************************************************
REMARKS:
Main program entry point.
****************************************************************************/
int main(
    int argc,
    char *argv[])
{
    ProfileRecord   R;
	int             x,y,bits,success;

	/* Save the state of the console */
	hwndConsole = PM_openConsole(0,0,0,0,0,true);
	if ((stateBuf = malloc(PM_getConsoleStateSize())) == NULL)
        PM_fatalError("Out of memory!");
	PM_saveConsoleState(stateBuf,hwndConsole);

	/* Register our fatal error cleanup handler */
	PM_setFatalErrorCleanup(FatalErrorCleanup);

	/* Load the device driver for this device */
	if ((numDevices = GA_enumerateDevices(false)) == 0)
		PM_fatalError(GA_errorMsg(GA_status()));
	if ((dc = GA_loadDriver(0,false)) == NULL)
		PM_fatalError(GA_errorMsg(GA_status()));
	init.dwSize = sizeof(init);
	if (!dc->QueryFunctions(GA_GET_INITFUNCS,0,&init))
		PM_fatalError("Unable to get device driver functions!");
	driver.dwSize = sizeof(driver);
	if (!dc->QueryFunctions(GA_GET_DRIVERFUNCS,0,&driver))
		PM_fatalError("Unable to get device driver functions!");

	/* Display available modes for invalid command line */
	memset(&R,0,sizeof(R));
	parseArguments(argc,argv);
	R.thrashCache = thrashCache;
	argc -= (nextargv-1);
	if (argc != 4)
		help();
	x = atoi(argv[nextargv]);
	y = atoi(argv[nextargv+1]);
	bits = atoi(argv[nextargv+2]);
	CON_init();

	ZTimerInit();

	/* Now do the performance profiling */
	success = ProfileIt(x,y,bits,testName,&R);
	CON_clrscr();

	/* Unload the device driver */
	GA_unloadDriver(dc);

	/* Restore the console */
	CON_restoreMode();
	PM_restoreConsoleState(stateBuf,hwndConsole);
	PM_closeConsole(hwndConsole);
#if	defined(__LINUX__)
	/* Coming back from graphics mode on Linux also restored the previous
	 * text mode console contents, so we need to clear the screen to get
	 * around this since the cursor does not get homed by our code.
	 */
	printf("\033[H\033[J");
#endif
	if (success) {
        printf("Profiling results for mode %ldx%ld %ld color.\n",
            maxX+1,maxY+1,maxcolor+1);
        printf("\n");
		printf("Hardware accelerated functions:\n\n");
		printf(" Integer lines (solid)              => %10.2f lines/s\n",   R.intLinesPerSec);
		printf(" Bresenham lines (solid)            => %10.2f lines/s\n",   R.bresLinesPerSec);
		printf(" Integer lines (patterned)          => %10.2f lines/s\n",   R.intStippleLinesPerSec);
		printf(" Bresenham lines (patterned)        => %10.2f lines/s\n",   R.bresStippleLinesPerSec);
		printf(" Clears (solid pattern)             => %10.2f Mb/s\n",      R.clearsMbPerSec);
		printf(" Clears (mono pattern)              => %10.2f Mb/s\n",      R.monoPattMbPerSec);
		printf(" Clears (transparent mono pattern)  => %10.2f Mb/s\n",      R.transMonoPattMbPerSec);
		printf(" Clears (color pattern)             => %10.2f Mb/s\n",      R.colorPattMbPerSec);
		printf(" Clears (transparent color pattern) => %10.2f Mb/s\n",      R.transColorPattMbPerSec);
		printf(" BitBlt screen->screen (scrolls)    => %10.2f Mb/s\n",      R.bitBltMbPerSec);
		printf(" BitBlt memory->screen              => %10.2f Mb/s\n",      R.bitBltSysMbPerSec);
		printf(" Mono image memory->screen (text)   => %10.2f Mb/s\n",      R.putMonoImageMbPerSec);
		printf("\n");
		printf("CPU Direct to Video Memory functions:\n\n");
		printf(" CPU copies to VRAM                 => %10.2f Mb/s\n",      R.putImagesCPUMbPerSec);
		printf(" CPU copies from VRAM               => %10.2f Mb/s\n",      R.getImagesCPUMbPerSec);
		printf(" CPU reads from VRAM                => %10.2f Mb/s\n",      R.readsCPUMbPerSec);
		printf(" CPU clears                         => %10.2f Mb/s\n",      R.clearCPUMbPerSec);
		printf(" CPU reverse clears                 => %10.2f Mb/s\n",      R.reverseClearCPUMbPerSec);
		printf("\n");
		printf("Baseline values for system memory:\n\n");
		printf(" clears (REP STOSD)     => %10.2f Mb/s\n",      R.clearBaseLineMbPerSec);
		printf(" reverse clears         => %10.2f Mb/s\n",      R.reverseClearBaseLineMbPerSec);
		printf(" reads  (MOV EAX,[EDI]) => %10.2f Mb/s\n",      R.readBaseLineMbPerSec);
		printf(" copies (REP MOVSD)     => %10.2f Mb/s\n",      R.copyBaseLineMbPerSec);
		printf("\n");
		printf("Overall GA-Mark: %.2f\n", ComputeGAMark(&R));
		}
    else {
        printf("Unable to set desired graphics mode\n");
        }
    return 0;
}
