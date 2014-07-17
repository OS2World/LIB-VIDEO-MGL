/****************************************************************************
*
*                            Hello SciTech Nucleus!
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
* Description:  Simple 'Hello World' program to initialise a user
*               specified graphics mode, and display a simple moire pattern
*				using the SciTech Nucleus, Graphics Architecture and the
*				simple GASDK API.
*
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "nucleus/gasdk.h"

/*---------------------------- Global Variables ---------------------------*/

GA_initFuncs	init;
GA_driverFuncs	driver;

/*----------------------------- Implementation ----------------------------*/

/****************************************************************************
REMARKS:
Draw a simple moire pattern of lines on the display
****************************************************************************/
void DrawMoire(void)
{
	int     i,value;

	if (maxcolor >= 0x7FFFL) {
		for (i = 0; i < maxX; i++) {
			SetForeColor(rgbColor((uchar)((i*255L)/maxX),0,0));
			draw2d.DrawLineInt(maxX/2,maxY/2,i,0,rgbColor((uchar)((i*255L)/maxX),0,0));
			SetForeColor(rgbColor(0,(uchar)((i*255L)/maxX),0));
			draw2d.DrawLineInt(maxX/2,maxY/2,i,maxY,rgbColor(0,(uchar)((i*255L)/maxX),0));
			}
		for (i = 0; i < maxY; i++) {
			value = (int)((i*255L)/maxY);
			SetForeColor(rgbColor((uchar)value,0,(uchar)(255 - value)));
			draw2d.DrawLineInt(maxX/2,maxY/2,0,i,rgbColor((uchar)value,0,(uchar)(255 - value)));
			SetForeColor(rgbColor(0,(uchar)(255 - value),(uchar)value));
			draw2d.DrawLineInt(maxX/2,maxY/2,maxX,i,rgbColor(0,(uchar)(255 - value),(uchar)value));
			}
		}
	else {
		for (i = 0; i < maxX; i += 5) {
			SetForeColor(i % maxcolor);
			draw2d.DrawLineInt(maxX/2,maxY/2,i,0,true);
			SetForeColor((i+1) % maxcolor);
			draw2d.DrawLineInt(maxX/2,maxY/2,i,maxY,true);
			}
		for (i = 0; i < maxY; i += 5) {
			SetForeColor((i+2) % maxcolor);
			draw2d.DrawLineInt(maxX/2,maxY/2,0,i,true);
			SetForeColor((i+3) % maxcolor);
			draw2d.DrawLineInt(maxX/2,maxY/2,maxX,i,true);
			}
		}
	SetForeColor(defcolor);
	draw2d.DrawLineInt(0,0,maxX,0,true);
	draw2d.DrawLineInt(0,0,0,maxY,true);
	draw2d.DrawLineInt(maxX,0,maxX,maxY,true);
	draw2d.DrawLineInt(0,maxY,maxX,maxY,true);
}

/****************************************************************************
REMARKS:
Display a list of available resolutions
****************************************************************************/
void AvailableModes(void)
{
	N_uint16	*modes;

	printf("Available graphics modes:\n");
	for (modes = dc->AvailableModes; *modes != 0xFFFF; modes++) {
		modeInfo.dwSize = sizeof(modeInfo);
		if (init.GetVideoModeInfo(*modes,&modeInfo) != 0)
			continue;
		if (modeInfo.Attributes & gaIsTextMode)
			continue;
		printf("    %4d x %4d %d bits per pixel\n",
			modeInfo.XResolution, modeInfo.YResolution, modeInfo.BitsPerPixel);
		}
	printf("\nUsage: hello <xres> <yres> <bits>\n");
}

/****************************************************************************
REMARKS:
Initialise the graphics mode with the specified resolution and
color depth.
****************************************************************************/
int InitGraphics(
	int x,
	int y,
	int bits)
{
	int			oldMode;
	N_uint16	*modes;

	for (modes = dc->AvailableModes; *modes != 0xFFFF; modes++) {
		modeInfo.dwSize = sizeof(modeInfo);
		if (init.GetVideoModeInfo(*modes,&modeInfo) != 0)
			continue;
		if (modeInfo.Attributes & gaIsTextMode)
			continue;
		if (modeInfo.XResolution == x && modeInfo.YResolution == y && modeInfo.BitsPerPixel == bits) {
			cntMode = *modes;
			if (modeInfo.Attributes & gaHaveLinearBuffer)
				cntMode |= gaLinearBuffer;
			oldMode = init.GetVideoMode();
			virtualX = virtualY = bytesPerLine = -1;
			if (init.SetVideoMode(cntMode,&virtualX,&virtualY,&bytesPerLine,&maxMem,0,NULL) != 0)
				return -1;
			InitSoftwareRasterizer(0,1);
			return oldMode;
			}
		}
	return -1;
}

/****************************************************************************
REMARKS:
Main routine. Expects the x & y resolution of the desired video mode
to be passed on the command line. Will print out a list of available
video modes if no command line is present.
****************************************************************************/
int main(
	int argc,
	char *argv[])
{
	int		x,y,bpp,oldMode,console_fd;
	void	*stateBuf;

	// Load the device driver for this device
	if ((dc = GA_loadDriver(0,false)) == NULL)
		PM_fatalError(GA_errorMsg(GA_status()));
	init.dwSize = sizeof(init);
	if (!dc->QueryFunctions(GA_GET_INITFUNCS,0,&init))
		PM_fatalError("Unable to get device driver functions!");
	driver.dwSize = sizeof(driver);
	if (!dc->QueryFunctions(GA_GET_DRIVERFUNCS,0,&driver))
		PM_fatalError("Unable to get device driver functions!");

	/* Display available modes for invalid command line */
	if (argc != 4) {
		AvailableModes();
		GA_unloadDriver(dc);
		return 0;
		}

	// Save the state of the console
	console_fd = PM_openConsole(NULL,0,640,480,8,true);
	if ((stateBuf = malloc(PM_getConsoleStateSize())) == NULL)
		PM_fatalError("Out of memory!");
	PM_saveConsoleState(stateBuf,console_fd);

	/* Get requested resolution, start graphics and draw pattern */
	x = atoi(argv[1]);
	y = atoi(argv[2]);
	bpp = atoi(argv[3]);
	if ((oldMode = InitGraphics(x,y,bpp)) != -1) {
		DrawMoire();
		PM_getch();
		virtualX = virtualY = bytesPerLine = -1;
		init.SetVideoMode(oldMode,&virtualX,&virtualY,&bytesPerLine,&maxMem,0,NULL);
		}

	// Restore the console
	PM_restoreConsoleState(stateBuf,console_fd);
	PM_closeConsole(console_fd);
#ifdef	__LINUX__
	// Coming back from graphics mode on Linux also restored the previous
	// text mode console contents, so we need to clear the screen to get
	// around this since the cursor does not get homed by our code.
	printf("\033[H\033[J");
#endif

	// Unload the device driver
	GA_unloadDriver(dc);

	if (oldMode == -1)
		printf("Unable to find specified graphics mode, or error starting mode.\n");
	return 0;
}
