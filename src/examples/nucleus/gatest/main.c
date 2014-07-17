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
*  |This copyrighted computer code is a proprietary trade secret of     |
*  |SciTech Software, Inc., located at 505 Wall Street, Chico, CA 95928 |
*  |USA (www.scitechsoft.com).  ANY UNAUTHORIZED POSSESSION, USE,       |
*  |VIEWING, COPYING, MODIFICATION OR DISSEMINATION OF THIS CODE IS     |
*  |STRICTLY PROHIBITED BY LAW.  Unless you have current, express       |
*  |written authorization from SciTech to possess or use this code, you |
*  |may be subject to civil and/or criminal penalties.                  |
*  |                                                                    |
*  |If you received this code in error or you would like to report      |
*  |improper use, please immediately contact SciTech Software, Inc. at  |
*  |530-894-8400.                                                       |
*  |                                                                    |
*  |REMOVAL OR MODIFICATION OF THIS HEADER IS STRICTLY PROHIBITED BY LAW|
*  ======================================================================
*
* Language:     ANSI C
* Environment:  Any 32-bit protected mode environment
*
* Description:  Command line driven, text based front end for the Nucleus
*				test program.
*
****************************************************************************/

#include "nucleus/gatest.h"
#include "getopt.h"

#ifndef _MAX_PATH
#define _MAX_PATH 256
#endif

/*--------------------------- Global variables ----------------------------*/

static int		textModeX = 80;
static int		textModeY = 50;
int 			numDevices = 0,cntDevice = 0;
ibool			fullSpeed = false;
ibool			breakPoint = false;
ibool			portrait = false;
ibool			flipped = false;
ibool			invertColors = false;
ibool			softwareEmulate = false;
ibool			useGTF = false;
int				xResMax,yResMax;
ulong			planeMask = 0xFFFFFFFF;
GA_devCtx		*primaryDC = NULL;
GA_modeInfo		miMax;
GA_CRTCInfo		crtcMax;
GA_initFuncs	init;
GA_driverFuncs	driver;
char			accelTestName[_MAX_PATH] = "all";
static PM_HWND	hwndConsole;
static void		*stateBuf;
int				multiModeX = 0;
int				multiModeY = 0;

#define NO_BUILD
#include "sdd/sddver.c"

const char *gaGetReleaseDate(void)
{ return release_date; };

const char *gaGetReleaseDate2(void)
{ return release_date2; };

#ifdef	ISV_LICENSE
#include "isv.c"
#endif

#define	MAX_MENU	40

/*----------------------------- Implementation ----------------------------*/

/****************************************************************************
REMARKS:
Displays the copyright signon banner on the screen
****************************************************************************/
static void banner(void)
{
#ifdef	LITE
	CON_printf("GACtrl - SciTech Nucleus Graphics Driver Control Center\n");
#else
	CON_printf("GATest - SciTech Nucleus Graphics Driver Compliance Test\n");
#endif
	CON_printf("         Release %s.%s (%s)\n\n",release_major,release_minor,release_date);
	CON_printf("%s\n",copyright_str);
	CON_printf("\n");
}

/****************************************************************************
REMARKS:
Sets the text mode with the specified dimensions
****************************************************************************/
static void SetTextMode(
	GA_devCtx *dc,
	int x,
	int y)
{
	GA_modeInfo     modeInfo;
	GA_CRTCInfo		crtc;
	N_uint16		*modes;
	int				xChars,yChars,textMode = -1;

	/* Search for the text mode to use with the same resolution */
	for (modes = dc->AvailableModes; *modes != 0xFFFF; modes++) {
		modeInfo.dwSize = sizeof(modeInfo);
		if (init.GetVideoModeInfo(*modes,&modeInfo) != 0)
			continue;
		if (!(modeInfo.Attributes & gaIsTextMode))
			continue;
		xChars = modeInfo.XResolution / modeInfo.XCharSize;
		yChars = modeInfo.YResolution / modeInfo.YCharSize;
		if (xChars == x && yChars == y) {
			textMode = *modes;
			break;
			}
		}
	if (textMode == -1) {
		SetTextMode(dc,80,25);
		}
	else {
		/* Set text mode and initialise console library */
		virtualX = virtualY = bytesPerLine = -1;
		init.SetVideoMode(textMode,&virtualX,&virtualY,&bytesPerLine,&maxMem,0,&crtc);
		}
}

/****************************************************************************
REMARKS:
Resets the active device to 0 and displays the fatal error message.
****************************************************************************/
void PMAPI FatalErrorCleanup(void)
{
	if (numDevices > 0)
		GA_setActiveDevice(0);
	if (primaryDC)
		SetTextMode(primaryDC,80,50);
	EVT_exit();
	CON_restoreMode();
	PM_restoreConsoleState(stateBuf,hwndConsole);
	PM_closeConsole(hwndConsole);
}

/****************************************************************************
REMARKS:
Handles the choice of display mode common to all display modes.
****************************************************************************/
ibool doChoice(
	GA_devCtx *dc,
	int *menu,
	int maxmenu,
	int bitsPerPixel,
	ulong planeMask,
	doTestType doTest)
{
	int			i,xRes,yRes,mode,choice,refreshRate = 0;
	GA_modeInfo	mi;
	char		buf[40];
	float		maxRefresh,refresh = 0;
	ibool		interlaced;
	GA_CRTCInfo	crtc;

	CON_printf("  [Esc] - Quit\n\n");
	CON_printf("Choice: ");
	choice = EVT_getch();
	choice = tolower(choice);
	if (choice == 0x1B)
		return true;
	if (choice >= 'a')
		choice = choice - 'a' + 10;
	else
		choice -= '0';
	if (0 <= choice && choice < maxmenu) {
		mode = menu[choice];
		if (mode == -1) {
			xRes = xResMax;
			yRes = yResMax;
			mi = miMax;
			crtc = crtcMax;
			mode = 0;
			}
		else if (mode == -2) {
			/* Select a custom display mode and refresh rate */
			CON_clrscr();
			banner();
			CON_printf("Enter Custom X Resolution: ");
			if (GetString(buf,sizeof(buf)) == -1)
				return false;
			xRes = atoi(buf);
			CON_printf("Enter Custom Y Resolution: ");
			if (GetString(buf,sizeof(buf)) == -1)
				return false;
			yRes = atoi(buf);
			CON_printf("Enter Custom Refresh Rate: ");
			if (GetString(buf,sizeof(buf)) == -1)
				return false;
			refresh = atof(buf);
			interlaced = false;
			if (refresh < 0) {
				refresh = -refresh;
				interlaced = true;
				}
			mi.dwSize = sizeof(mi);
			if (init.GetCustomVideoModeInfo(xRes,yRes,-1,-1,bitsPerPixel,&mi) != 0) {
				CON_printf("\n");
				CON_printf("ERROR: Hardware device not capable of requested mode!\n\n");
				CON_printf("\nPress any key to continue...\n");
				EVT_getch();
				return false;
				}
			if (!GA_computeCRTCTimings(dc,&mi,refresh,interlaced,&crtc,false)) {
				CON_printf("\n");
				CON_printf("ERROR: Could not compute CRTC timings!\n\n");
				CON_printf("\nPress any key to continue...\n");
				EVT_getch();
				return false;
				}
			mode = 0;
			}
		else {
			/* Select a pre-defined display mode and refresh rate */
			xRes = yRes = bitsPerPixel = -1;
			mi.dwSize = sizeof(mi);
			init.GetVideoModeInfo(mode,&mi);
			crtc.RefreshRate = 0;
			CON_clrscr();
			banner();
			if (useGTF) {
				GA_getMaxRefreshRate(dc,&mi,false,&maxRefresh);
				CON_printf("Choose refresh rate for %d x %d %d bits per pixel (max %3.2fHz):\n\n",
					mi.XResolution, mi.YResolution, mi.BitsPerPixel, maxRefresh);
				CON_printf("  [0] - User Default\n");
				if (maxRefresh >= 60)	CON_printf("  [1] - 60 Hz\n");
				if (maxRefresh >= 65)	CON_printf("  [2] - 65 Hz\n");
				if (maxRefresh >= 70)	CON_printf("  [3] - 70 Hz\n");
				if (maxRefresh >= 75)	CON_printf("  [4] - 75 Hz\n");
				if (maxRefresh >= 80)	CON_printf("  [5] - 80 Hz\n");
				if (maxRefresh >= 85)	CON_printf("  [6] - 85 Hz\n");
				if (maxRefresh >= 90)	CON_printf("  [7] - 90 Hz\n");
				if (maxRefresh >= 95)	CON_printf("  [8] - 95 Hz\n");
				if (maxRefresh >= 100)	CON_printf("  [9] - 100 Hz\n");
				if (maxRefresh >= 105)	CON_printf("  [A] - 105 Hz\n");
				if (maxRefresh >= 110)	CON_printf("  [B] - 110 Hz\n");
				if (maxRefresh >= 115)	CON_printf("  [C] - 115 Hz\n");
				if (maxRefresh >= 120)	CON_printf("  [D] - 120 Hz\n");
				CON_printf("  [E] - Custom\n");
				CON_printf("\nChoice: ");
				choice = EVT_getch();
				if (tolower(choice) == 'q' || choice == 0x1B)
					return false;
				interlaced = false;
				switch (tolower(choice)) {
					case '1':   refresh = 60;	break;
					case '2':   refresh = 65;	break;
					case '3':   refresh = 70;	break;
					case '4':   refresh = 75;	break;
					case '5':   refresh = 80;	break;
					case '6':   refresh = 85;	break;
					case '7':   refresh = 90;	break;
					case '8':   refresh = 95;	break;
					case '9':   refresh = 100;	break;
					case 'a':   refresh = 105;	break;
					case 'b':   refresh = 110;	break;
					case 'c':   refresh = 115;	break;
					case 'd':   refresh = 120;	break;
					case 'e':
						CON_printf("\n\nEnter Refresh Rate (- for interlaced): ");
						if (GetString(buf,sizeof(buf)) == -1)
							return false;
						refresh = atof(buf);
						if (refresh < 0) {
							refresh = -refresh;
							interlaced = true;
							}
						if (refresh < 40 || refresh > 200)
							refresh = 0;
						break;
					}
				if (refresh != 0) {
					if (!GA_computeCRTCTimings(dc,&mi,refresh,interlaced,&crtc,false)) {
						CON_printf("\n");
						CON_printf("ERROR: Could not compute CRTC timings!\n\n");
						CON_printf("\nPress any key to continue...\n");
						EVT_getch();
						return false;
						}
					mode |= gaRefreshCtrl;
					}
				}
			else {
				CON_printf("Choose refresh rate for %d x %d %d bits per pixel:\n\n",
					mi.XResolution, mi.YResolution, mi.BitsPerPixel);
				CON_printf("  [0] - User Default (%d Hz)\n", mi.DefaultRefreshRate);
				for (i = 0; mi.RefreshRateList[i] != -1; i++) {
					choice = i+1;
					if (choice < 10)
						choice = '0' + choice;
					else
						choice = 'A' + choice - 10;
					if (mi.RefreshRateList[i] < 0)
						CON_printf("  [%c] - %ld Hz Interlaced%s\n", choice,-mi.RefreshRateList[i],
							mi.RefreshRateList[i] == mi.DefaultRefreshRate ? " *" : "");
					else
						CON_printf("  [%c] - %ld Hz%s\n", choice,mi.RefreshRateList[i],
							mi.RefreshRateList[i] == mi.DefaultRefreshRate ? " *" : "");
					}
				CON_printf("\nChoice: ");
				choice = EVT_getch();
				if (tolower(choice) == 'q' || choice == 0x1B)
					return false;
				if (choice == 0x0D)
					choice = 0;
				else if (choice >= 'a')
					choice = choice - 'a' + 10;
				else
					choice -= '0';
				if (choice == 0)
					refreshRate = 0;
				else
					refreshRate = ABS(mi.RefreshRateList[choice-1]);
				}
			}
		if (!(mi.Attributes & gaIsTextMode) && (mi.Attributes & gaHaveLinearBuffer))
			mode |= gaLinearBuffer;
		if (!doTest(dc,xRes,yRes,bitsPerPixel,mode,refreshRate,&crtc,planeMask)) {
			CON_printf("\n");
			CON_printf("ERROR: Video mode did not set correctly!\n\n");
			CON_printf("\nPress any key to continue...\n");
			EVT_getch();
			}
		}
	return false;
}

/****************************************************************************
REMARKS:
Function to add a mode to the menu list and generate the name for the mode.
****************************************************************************/
static int addMode(
	int *menu,
	int maxmenu,
	GA_modeInfo *mi,
	int mode)
{
	int 	num;
	char	buf[80];

	/* Get name for mode and mode number for initialising it */
	if (maxmenu < 10)
		num = '0' + maxmenu;
	else
		num = 'A' + maxmenu - 10;
	if (GetModeName(buf,mi) == 0)
		return maxmenu;
	CON_printf("  [%c] - %s\n",num,buf);
	menu[maxmenu++] = mode;
	return maxmenu;
}

/****************************************************************************
REMARKS:
Round a value to the specified integer boundary
****************************************************************************/
static ulong RoundDown(
	ulong value,
	ulong boundary)
{
	return (value / boundary) * boundary;
}

/****************************************************************************
REMARKS:
Function to add a mode to the menu list and generate the name for the mode.
****************************************************************************/
static int addMaxMode(
	GA_devCtx *dc,
	int bitsPerPixel,
	int *menu,
	int maxmenu)
{
	int 	num;
	char	buf[80];

	/* Find the maximum resolution at this color depth */
	miMax.dwSize = sizeof(miMax);
	if (init.GetCustomVideoModeInfo(640,480,-1,-1,bitsPerPixel,&miMax) != 0)
		return maxmenu;
	for (xResMax = RoundDown(miMax.MaxScanLineWidth,8); xResMax > 640; xResMax -= 8) {
		yResMax = (xResMax * 3) / 4;
		if (init.GetCustomVideoModeInfo(xResMax,yResMax,-1,-1,bitsPerPixel,&miMax) != 0)
			continue;
		if (!GA_computeCRTCTimings(dc,&miMax,60,false,&crtcMax,false))
			continue;
		break;
		}
	if (xResMax == 640)
		return maxmenu;

	/* Get name for mode and mode number for initialising it */
	if (maxmenu < 10)
		num = '0' + maxmenu;
	else
		num = 'A' + maxmenu - 10;
	if (GetModeName(buf,&miMax) == 0)
		return maxmenu;
	CON_printf("  [%c] - %s (Max @ 60Hz)\n",num,buf);
	menu[maxmenu++] = -1;
	return maxmenu;
}

/****************************************************************************
REMARKS:
Function to add a mode to the menu list and generate the name for the mode.
****************************************************************************/
static int addCustomMode(
	int *menu,
	int maxmenu)
{
	int	num;

	/* Get name for mode and mode number for initialising it */
	if (maxmenu == 0)
		return 0;
	if (maxmenu < 10)
		num = '0' + maxmenu;
	else
		num = 'A' + maxmenu - 10;
	CON_printf("  [%c] - Custom\n",num);
	menu[maxmenu++] = -2;
	return maxmenu;
}

/****************************************************************************
REMARKS:
Main menu for testing all 16 color modes
****************************************************************************/
static void testText(
	GA_devCtx *dc,
	doTestType doTest)
{
	int			maxmenu,menu[MAX_MENU];
	N_uint16	*modes;
	GA_modeInfo	mi;

	for (;;) {
		CON_clrscr();
		banner();
		CON_printf("Which text mode to test:\n\n");
		maxmenu = 0;
		for (modes = dc->AvailableModes; *modes != 0xFFFF; modes++) {
			mi.dwSize = sizeof(mi);
			if (init.GetVideoModeInfo(*modes,&mi) != 0)
				continue;
			if (!(mi.Attributes & gaIsTextMode))
				continue;
			maxmenu = addMode(menu,maxmenu,&mi,*modes);
			}
		maxmenu = addCustomMode(menu,maxmenu);
		if (doChoice(dc,menu,maxmenu,0,0xFFFFFFFF,doTest))
			break;
		}
}

/****************************************************************************
REMARKS:
Main menu for testing all 16 color modes
****************************************************************************/
static void test16(
	GA_devCtx *dc,
	doTestType doTest)
{
	int			maxmenu,menu[MAX_MENU];
	N_uint16	*modes;
	GA_modeInfo	mi;

	for (;;) {
		CON_clrscr();
		banner();
		CON_printf("Which 4 bit video mode to test:\n\n");
		maxmenu = 0;
		for (modes = dc->AvailableModes; *modes != 0xFFFF; modes++) {
			mi.dwSize = sizeof(mi);
			if (init.GetVideoModeInfo(*modes,&mi) != 0)
				continue;
			if (mi.BitsPerPixel != 4)
				continue;
			maxmenu = addMode(menu,maxmenu,&mi,*modes);
			}
		maxmenu = addMaxMode(dc,4,menu,maxmenu);
		maxmenu = addCustomMode(menu,maxmenu);
		if (doChoice(dc,menu,maxmenu,4,0xFFFFFFFF,doTest))
			break;
		}
}

/****************************************************************************
REMARKS:
Main menu for testing all 256 color modes
****************************************************************************/
static void test256(
	GA_devCtx *dc,
	doTestType doTest)
{
	int			maxmenu,menu[MAX_MENU];
	N_uint16	*modes;
	GA_modeInfo	mi;

	for (;;) {
		CON_clrscr();
		banner();
		CON_printf("Which 8 bit video mode to test:\n\n");
		maxmenu = 0;
		for (modes = dc->AvailableModes; *modes != 0xFFFF; modes++) {
			mi.dwSize = sizeof(mi);
			if (init.GetVideoModeInfo(*modes,&mi) != 0)
				continue;
			if (mi.BitsPerPixel != 8)
				continue;
			maxmenu = addMode(menu,maxmenu,&mi,*modes);
			}
		maxmenu = addMaxMode(dc,8,menu,maxmenu);
		maxmenu = addCustomMode(menu,maxmenu);
		if (doChoice(dc,menu,maxmenu,8,planeMask,doTest))
			break;
		}
}

/****************************************************************************
REMARKS:
Main menu for testing all direct color modes
****************************************************************************/
static void testDirectColor(
	GA_devCtx *dc,
	long colors,
	doTestType doTest)
{
	int			maxmenu,numbits,menu[MAX_MENU];
	N_uint16	*modes;
	GA_modeInfo	mi;

	for (;;) {
		CON_clrscr();
		banner();
		if (colors == 0x7FFFL)
			numbits = 15;
		else if (colors == 0xFFFFL)
			numbits = 16;
		else if (colors == 0xFFFFFFL)
			numbits = 24;
		else
			numbits = 32;
		CON_printf("Which %d bit video mode to test:\n\n", numbits);
		maxmenu = 0;
		for (modes = dc->AvailableModes; *modes != 0xFFFF; modes++) {
			mi.dwSize = sizeof(mi);
			if (init.GetVideoModeInfo(*modes,&mi) != 0)
				continue;
			if (mi.BitsPerPixel == numbits) {
				maxmenu = addMode(menu,maxmenu,&mi,*modes);
				}
			}
		maxmenu = addMaxMode(dc,numbits,menu,maxmenu);
		maxmenu = addCustomMode(menu,maxmenu);
		if (doChoice(dc,menu,maxmenu,numbits,planeMask,doTest))
			break;
		}
}

/****************************************************************************
REMARKS:
Main menu for all interactive testing functions
****************************************************************************/
static void InteractiveTests(
	GA_devCtx *dc)
{
	int	choice;

	for (;;) {
		CON_clrscr();
		banner();
		CON_printf("Vendor Name:  %s\n",dc->OemVendorName);
		CON_printf("Copyright:    %s\n",dc->OemCopyright);
		CON_printf("Version:      %d.%d with %d Kb memory\n",
			(int)dc->Version >> 8,(int)dc->Version & 0xFF,(int)dc->TotalMemory);
		CON_printf("\n");
		CON_printf(". 2D Acceleration:        %-3s     ", dc->Attributes & gaHaveAccel2D ? "Yes" : "No");
		CON_printf(". Hardware Cursor:        %-3s\n", dc->Attributes & gaHaveHWCursor ? "Yes" : "No");
		CON_printf(". Multi Buffering:        %-3s     ", dc->Attributes & gaHaveDisplayStart ? "Yes" : "No");
		CON_printf(". Virtual Scrolling:      %-3s\n", dc->Attributes & gaHaveDisplayStart ? "Yes" : "No");
		CON_printf(". HW Triple Buffering:    %-3s     ", dc->Attributes & gaHaveTripleBuffer ? "Yes" : "No");
		CON_printf(". HW Stereo Flipping:     %-3s\n", dc->Attributes & gaHaveStereo ? "Yes" : "No");
		CON_printf(". HW Stereo Sync:         %-3s     ", dc->Attributes & gaHaveHWStereoSync ? "Yes" : "No");
		CON_printf(". HW Stereo Sync (EVC):   %-3s\n", dc->Attributes & gaHaveEVCStereoSync ? "Yes" : "No");
		CON_printf(". 8 bit wide DAC support: %-3s     ", dc->Attributes & gaHave8BitDAC ? "Yes" : "No");
		CON_printf(". NonVGA Controller:      %-3s\n", dc->Attributes & gaHaveNonVGAMode ? "Yes" : "No");
		CON_printf(". Linear framebuffer:     ");
		if (dc->LinearBasePtr) {
			CON_printf("%ldMb\n", (ulong)dc->LinearBasePtr >> 20);
			}
		else
			CON_printf("No\n");
		CON_printf("\n");
		CON_printf("Select color mode to test:\n\n");
		CON_printf("  [0] - 4 bits per pixel modes      [3] - 16 bits per pixel modes\n");
		CON_printf("  [1] - 8 bits per pixel modes      [4] - 24 bits per pixel modes\n");
		CON_printf("  [2] - 15 bits per pixel modes     [5] - 32 bits per pixel modes\n");
		CON_printf("  [6] - Text modes\n");
		CON_printf("  [Q] - Quit\n\n");
		CON_printf("Choice: ");
		choice = EVT_getch();
		if (choice == 'q' || choice == 'Q' || choice == 0x1B)
			break;
		switch (choice) {
			case '0':	test16(dc,doTest);						break;
			case '1':	test256(dc,doTest);						break;
			case '2':	testDirectColor(dc,0x7FFFL,doTest);		break;
			case '3':	testDirectColor(dc,0xFFFFL,doTest);		break;
			case '4':	testDirectColor(dc,0xFFFFFFL,doTest);	break;
			case '5':	testDirectColor(dc,0xFFFFFFFFL,doTest);	break;
			case '6':	testText(dc,doTextTest);				break;
			}
		}
}

/****************************************************************************
REMARKS:
Main menu for all interactive testing functions
****************************************************************************/
static void CenteringAndRefresh(
	GA_devCtx *dc)
{
	int	choice;

	for (;;) {
		CON_clrscr();
		banner();
		CON_printf("Vendor Name:  %s\n",dc->OemVendorName);
		CON_printf("Copyright:    %s\n",dc->OemCopyright);
		CON_printf("Version:      %d.%d with %d Kb memory\n",
			(int)dc->Version >> 8,(int)dc->Version & 0xFF,(int)dc->TotalMemory);
		CON_printf("\n");
		CON_printf("Select color mode to center:\n\n");
		CON_printf("  [0] - 4 bits per pixel modes      [3] - 16 bits per pixel modes\n");
		CON_printf("  [1] - 8 bits per pixel modes      [4] - 24 bits per pixel modes\n");
		CON_printf("  [2] - 15 bits per pixel modes     [5] - 32 bits per pixel modes\n");
		CON_printf("  [6] - Text modes\n");
		CON_printf("  [Q] - Quit\n\n");
		CON_printf("Choice: ");
		choice = EVT_getch();
		if (choice == 'q' || choice == 'Q' || choice == 0x1B)
			break;
		switch (choice) {
			case '0':	test16(dc,doCenter);						break;
			case '1':	test256(dc,doCenter);						break;
			case '2':	testDirectColor(dc,0x7FFFL,doCenter);		break;
			case '3':	testDirectColor(dc,0xFFFFL,doCenter);		break;
			case '4':	testDirectColor(dc,0xFFFFFFL,doCenter);		break;
			case '5':	testDirectColor(dc,0xFFFFFFFFL,doCenter);	break;
			case '6':	testText(dc,doCenter);						break;
			}
		}
}

/****************************************************************************
REMARKS:
Main menu for all interactive testing functions
****************************************************************************/
static void GlobalRefresh(
	GA_devCtx *dc)
{
	char	buf[40];
	int		choice,refresh;

	CON_clrscr();
	banner();
	CON_printf("Vendor Name:  %s\n",dc->OemVendorName);
	CON_printf("Copyright:    %s\n",dc->OemCopyright);
	CON_printf("Version:      %d.%d with %d Kb memory\n",
		(int)dc->Version >> 8,(int)dc->Version & 0xFF,(int)dc->TotalMemory);
	CON_printf("\n");
	CON_printf("Select new global refresh rate:\n\n");
	CON_printf("  [0] - 60 Hz\n");
	CON_printf("  [1] - 65 Hz\n");
	CON_printf("  [2] - 70 Hz\n");
	CON_printf("  [3] - 75 Hz\n");
	CON_printf("  [4] - 80 Hz\n");
	CON_printf("  [5] - 85 Hz\n");
	CON_printf("  [6] - 90 Hz\n");
	CON_printf("  [7] - 95 Hz\n");
	CON_printf("  [8] - 100 Hz\n");
	CON_printf("  [9] - Custom\n");
	CON_printf("  [Q] - Quit\n\n");
	CON_printf("Choice: ");
	choice = EVT_getch();
	if (tolower(choice) == 'q' || choice == 0x1B)
		return;
	switch (tolower(choice)) {
		case '0':   refresh = 60;	break;
		case '1':   refresh = 65;	break;
		case '2':   refresh = 70;	break;
		case '3':   refresh = 75;	break;
		case '4':   refresh = 80;	break;
		case '5':   refresh = 85;	break;
		case '6':   refresh = 90;	break;
		case '7':   refresh = 95;	break;
		case '8':   refresh = 100;	break;
		case '9':
			CON_printf("\n\nEnter Refresh Rate: ");
			if (GetString(buf,sizeof(buf)) == -1)
				return;
			refresh = atof(buf);
			if (refresh < 40 || refresh > 200)
				refresh = 0;
			break;
		}
	if (refresh != 0) {
		init.SetGlobalRefresh(refresh);
		GA_saveCRTCTimings(dc);
		}
}

/****************************************************************************
REMARKS:
Sets the text mode with the specified dimensions
****************************************************************************/
static int FindMode(
	GA_devCtx *dc,
	int xRes,
	int yRes,
	int bits)
{
	GA_modeInfo     modeInfo;
	N_uint16		*modes;

	/* Search for the text mode to use with the same resolution */
	for (modes = dc->AvailableModes; *modes != 0xFFFF; modes++) {
		modeInfo.dwSize = sizeof(modeInfo);
		if (init.GetVideoModeInfo(*modes,&modeInfo) != 0)
			continue;
		if (modeInfo.Attributes & gaIsTextMode)
			continue;
		if (xRes == modeInfo.XResolution && yRes == modeInfo.YResolution && bits == modeInfo.BitsPerPixel) {
			if (modeInfo.Attributes & gaHaveLinearBuffer)
				return *modes | gaLinearBuffer;
			return *modes;
			}
		}
	return -1;
}

/****************************************************************************
REMARKS:
Main menu for all interactive testing functions
****************************************************************************/
static void TVOutputControl(
	GA_devCtx *dc)
{
	int			choice,mode,changed = false,cntMode = init.GetVideoMode();
	GA_options	opt,orgOpt;
	GA_CRTCInfo	crtc;

	opt.dwSize = sizeof(opt);
	init.GetOptions(&opt);
	orgOpt = opt;
	for (;;) {
		CON_clrscr();
		banner();
		CON_printf("Vendor Name:  %s\n",dc->OemVendorName);
		CON_printf("Copyright:    %s\n",dc->OemCopyright);
		CON_printf("Version:      %d.%d with %d Kb memory\n",
			(int)dc->Version >> 8,(int)dc->Version & 0xFF,(int)dc->TotalMemory);
		CON_printf("\n");
		CON_printf("Select TV value to control:\n\n");
		CON_printf("  [0] - Select TV only mode\n");
		CON_printf("  [1] - Select Simultaneous mode\n");
		CON_printf("  [2] - Select CRT only mode\n");
		CON_printf("  [3] - Center 640x480 TV image and adjust brightness\n");
		CON_printf("  [4] - Center 800x600 TV image and adjust brightness\n");
		CON_printf("  [5] - Set output mode to underscan\n");
		CON_printf("  [6] - Set output mode to overscan\n");
		CON_printf("  [7] - Set output mode to NTSC\n");
		CON_printf("  [8] - Set output mode to PAL\n");
		CON_printf("  [Q] - Quit\n\n");
		CON_printf("Choice: ");
		choice = EVT_getch();
		if (tolower(choice) == 'q' || choice == 0x1B)
			break;
		switch (tolower(choice)) {
			case '0':
				init.SetDisplayOutput(gaOUTPUT_TV);
				changed = true;
				break;
			case '1':
				init.SetDisplayOutput(gaOUTPUT_TV | gaOUTPUT_CRT);
				changed = true;
				break;
			case '2':
				init.SetDisplayOutput(gaOUTPUT_CRT);
				changed = true;
				break;
			case '3':
				if ((mode = FindMode(dc,640,480,8)) != -1)
					CenterTVMode(dc,mode);
				break;
			case '4':
				if ((mode = FindMode(dc,800,600,8)) != -1)
					CenterTVMode(dc,mode);
				break;
			case '5':
				init.SetDisplayOutput(opt.outputDevice & ~gaOUTPUT_TVOVERSCAN);
				changed = true;
				break;
			case '6':
				init.SetDisplayOutput(opt.outputDevice | gaOUTPUT_TVOVERSCAN);
				changed = true;
				break;
			case '7':
				init.SetDisplayOutput((opt.outputDevice & ~gaOUTPUT_TVCOLORMASK) | gaOUTPUT_TVNTSC);
				changed = true;
				break;
			case '8':
				init.SetDisplayOutput((opt.outputDevice & ~gaOUTPUT_TVCOLORMASK) | gaOUTPUT_TVPAL);
				changed = true;
				break;
			}

		/* Reset current display mode */
		virtualX = virtualY = bytesPerLine = -1;
		init.SetVideoMode(cntMode | gaDontClear,&virtualX,&virtualY,&bytesPerLine,&maxMem,0,&crtc);
		}

	// Check if the settings should be saved
	if (changed) {
		CON_clrscr();
		banner();
		CON_printf("Save settings (Y/N)?");
		choice = EVT_getch();
		if (tolower(choice) == 'y') {
			init.GetOptions(&opt);
			GA_saveOptions(dc,&opt);
			}
		else
			init.SetOptions(&orgOpt);
		}
}

/****************************************************************************
REMARKS:
Main menu for all interactive testing functions
****************************************************************************/
static void LCDOutputControl(
	GA_devCtx *dc)
{
	int			choice,changed = false,cntMode = init.GetVideoMode();
	GA_options	opt,orgOpt;
	GA_CRTCInfo	crtc;

	for (;;) {
		opt.dwSize = sizeof(opt);
		init.GetOptions(&opt);
		CON_clrscr();
		banner();
		CON_printf("Vendor Name:  %s\n",dc->OemVendorName);
		CON_printf("Copyright:    %s\n",dc->OemCopyright);
		CON_printf("Version:      %d.%d with %d Kb memory\n",
			(int)dc->Version >> 8,(int)dc->Version & 0xFF,(int)dc->TotalMemory);
		CON_printf("\n");
		CON_printf("Select LCD value to control:\n\n");
		CON_printf("  [0] - Select LCD only mode\n");
		CON_printf("  [1] - Select Simultaneous mode\n");
		CON_printf("  [2] - Select CRT only mode\n");
		CON_printf("  [Q] - Quit\n\n");
		CON_printf("Choice: ");
		choice = EVT_getch();
		if (tolower(choice) == 'q' || choice == 0x1B)
			break;
		switch (tolower(choice)) {
			case '0':
				init.SetDisplayOutput(gaOUTPUT_LCD);
				changed = true;
				break;
			case '1':
				init.SetDisplayOutput(gaOUTPUT_LCD | gaOUTPUT_CRT);
				changed = true;
				break;
			case '2':
				init.SetDisplayOutput(gaOUTPUT_CRT);
				changed = true;
				break;
			}
		/* Reset current display mode */
		virtualX = virtualY = bytesPerLine = -1;
		init.SetVideoMode(cntMode | gaDontClear,&virtualX,&virtualY,&bytesPerLine,&maxMem,0,&crtc);
		}

	// Check if the settings should be saved
	if (changed) {
		CON_clrscr();
		banner();
		CON_printf("Save settings (Y/N)?");
		choice = EVT_getch();
		if (tolower(choice) == 'y') {
			init.GetOptions(&opt);
			GA_saveOptions(dc,&opt);
			}
		else
			init.SetOptions(&orgOpt);
		}
}

/****************************************************************************
REMARKS:
Function to build a gamma correction color ramp
****************************************************************************/
static void BuildGammaRamp(
	GA_paletteExt *pal,
	float gamma)
{
	int 	i;
	float	igamma = 1.0 / gamma;

	for (i = 0; i < 256; i++) {
		pal[i].Red = pal[i].Green = pal[i].Blue =
			(uchar)((pow(i / 65535.0,igamma) * 65535.0) + 0.5);
		}
}

/****************************************************************************
REMARKS:
Main menu for gamma corrcetion control
****************************************************************************/
static void GammaControl(
	GA_devCtx *dc)
{
	char			buf[40];
	float			gamma;
	GA_options		opt;

	opt.dwSize = sizeof(opt);
	init.GetOptions(&opt);
	CON_clrscr();
	banner();
	CON_printf("Vendor Name:  %s\n",dc->OemVendorName);
	CON_printf("Copyright:    %s\n",dc->OemCopyright);
	CON_printf("Version:      %d.%d with %d Kb memory\n",
		(int)dc->Version >> 8,(int)dc->Version & 0xFF,(int)dc->TotalMemory);
	CON_printf("\n");
	if (driver.SetGammaCorrectData) {
		CON_printf("Enter gamma correction factor (ie: 2.3): ");
		if (GetString(buf,sizeof(buf)) == -1)
			return;
		if ((gamma = atof(buf)) == 0.0)
			return;
		BuildGammaRamp(opt.gammaRamp,gamma);
		init.SetOptions(&opt);
		GA_saveOptions(dc,&opt);
		}
	else {
		CON_printf("Gamma correction not available...");
		EVT_getch();
		return;
		}
}

#ifndef	LITE
/****************************************************************************
REMARKS:
Main menu for all interactive testing functions
****************************************************************************/
static void ScrollingTest(
	GA_devCtx *dc)
{
	int	choice;

	for (;;) {
		CON_clrscr();
		banner();
		CON_printf("Vendor Name:  %s\n",dc->OemVendorName);
		CON_printf("Copyright:    %s\n",dc->OemCopyright);
		CON_printf("Version:      %d.%d with %d Kb memory\n",
			(int)dc->Version >> 8,(int)dc->Version & 0xFF,(int)dc->TotalMemory);
		CON_printf("\n");
		CON_printf("Select color mode to test:\n\n");
		CON_printf("  [0] - 4 bits per pixel modes      [3] - 16 bits per pixel modes\n");
		CON_printf("  [1] - 8 bits per pixel modes      [4] - 24 bits per pixel modes\n");
		CON_printf("  [2] - 15 bits per pixel modes     [5] - 32 bits per pixel modes\n");
		CON_printf("  [Q] - Quit\n\n");
		CON_printf("Choice: ");
		choice = EVT_getch();
		if (choice == 'q' || choice == 'Q' || choice == 0x1B)
			break;
		switch (choice) {
			case '0':	test16(dc,doScrollTest);						break;
			case '1':	test256(dc,doScrollTest);						break;
			case '2':	testDirectColor(dc,0x7FFFL,doScrollTest);		break;
			case '3':	testDirectColor(dc,0xFFFFL,doScrollTest);		break;
			case '4':	testDirectColor(dc,0xFFFFFFL,doScrollTest);		break;
			case '5':	testDirectColor(dc,0xFFFFFFFFL,doScrollTest);	break;
			}
		}
}

/****************************************************************************
REMARKS:
Main menu for all interactive testing functions
****************************************************************************/
static void StereoTest(
	GA_devCtx *dc)
{
	int	choice;

	for (;;) {
		CON_clrscr();
		banner();
		CON_printf("Vendor Name:  %s\n",dc->OemVendorName);
		CON_printf("Copyright:    %s\n",dc->OemCopyright);
		CON_printf("Version:      %d.%d with %d Kb memory\n",
			(int)dc->Version >> 8,(int)dc->Version & 0xFF,(int)dc->TotalMemory);
		CON_printf("\n");
		CON_printf(". HW Triple Buffering:    %-3s     ", dc->Attributes & gaHaveTripleBuffer ? "Yes" : "No");
		CON_printf(". HW Stereo Flipping:     %-3s\n", dc->Attributes & gaHaveStereo ? "Yes" : "No");
		CON_printf(". HW Stereo Sync:         %-3s     ", dc->Attributes & gaHaveHWStereoSync ? "Yes" : "No");
		CON_printf(". HW Stereo Sync (EVC):   %-3s\n", dc->Attributes & gaHaveEVCStereoSync ? "Yes" : "No");
		CON_printf("\n");
		CON_printf("Select color mode to test:\n\n");
		CON_printf("  [0] - 4 bits per pixel modes      [3] - 16 bits per pixel modes\n");
		CON_printf("  [1] - 8 bits per pixel modes      [4] - 24 bits per pixel modes\n");
		CON_printf("  [2] - 15 bits per pixel modes     [5] - 32 bits per pixel modes\n");
		CON_printf("  [Q] - Quit\n\n");
		CON_printf("Choice: ");
		choice = EVT_getch();
		if (choice == 'q' || choice == 'Q' || choice == 0x1B)
			break;
		switch (choice) {
			case '0':	test16(dc,doStereoTest);						break;
			case '1':	test256(dc,doStereoTest);						break;
			case '2':	testDirectColor(dc,0x7FFFL,doStereoTest);		break;
			case '3':	testDirectColor(dc,0xFFFFL,doStereoTest);		break;
			case '4':	testDirectColor(dc,0xFFFFFFL,doStereoTest);		break;
			case '5':	testDirectColor(dc,0xFFFFFFFFL,doStereoTest);	break;
			}
		}
}
#endif

/****************************************************************************
REMARKS:
Writes the currently configured options to disk.
****************************************************************************/
void SaveOptions(
	GA_devCtx *dc)
{
	GA_options	opt;

	opt.dwSize = sizeof(opt);
	init.GetOptions(&opt);
	GA_saveOptions(dc,&opt);
}

/****************************************************************************
REMARKS:
Displays the main menu screen
****************************************************************************/
void MainScreen(void)
{
	int         		choice;
	GA_devCtx			*dc;
	GA_configInfo		info;
	GA_globalOptions	opt;
	N_uint32 			oldMode;

	/* If portrait, flipped or invert colors is enabled, enable it in
	 * the driver
	 */
	opt.dwSize = sizeof(opt);
	GA_getGlobalOptions(&opt);
	if (portrait)
		opt.bPortrait = true;
	if (flipped)
		opt.bFlipped = true;
	if (invertColors)
		opt.bInvertColors = true;
	if (multiModeX) {
		/* Setup a simple side/side virtual multi-controller mode */
		opt.bVirtualDisplay = true;
		opt.virtualSize.left = opt.virtualSize.top = 0;
		opt.virtualSize.right = multiModeX * 2;
		opt.virtualSize.bottom = multiModeY;
		opt.resolutions[0].left = opt.resolutions[0].top = 0;
		opt.resolutions[0].right = multiModeX;
		opt.resolutions[0].bottom = multiModeY;
		opt.resolutions[1].left = opt.resolutions[1].top = 0;
		opt.resolutions[1].right = multiModeX;
		opt.resolutions[1].bottom = multiModeY;
		opt.bounds[0].left = opt.bounds[0].top = 0;
		opt.bounds[0].right = multiModeX;
		opt.bounds[0].bottom = multiModeY;
		opt.bounds[1].left = multiModeX;
		opt.bounds[1].top = 0;
		opt.bounds[1].right = multiModeX * 2;
		opt.bounds[1].bottom = multiModeY;
		}
	GA_setGlobalOptions(&opt);

	/* Load the device driver for this device */
	if ((dc = GA_loadDriver(cntDevice,false)) == NULL)
		PM_fatalError(GA_errorMsg(GA_status()));
	init.dwSize = sizeof(init);
	if (!dc->QueryFunctions(GA_GET_INITFUNCS,0,&init))
		PM_fatalError("Unable to get device driver functions!");
	driver.dwSize = sizeof(driver);
	if (!dc->QueryFunctions(GA_GET_DRIVERFUNCS,0,&driver))
		PM_fatalError("Unable to get device driver functions!");
	if (cntDevice == 0)
		primaryDC = dc;

	/* Get the initial text mode */
	oldMode = init.GetVideoMode();

	/* Set the text mode and initialise the console library */
	SetTextMode(dc,textModeX,textModeY);
	CON_init();
	for (;;) {
		CON_clrscr();
		banner();
		CON_printf("Vendor Name:  %s\n",dc->OemVendorName);
		CON_printf("Copyright:    %s\n",dc->OemCopyright);
		info.dwSize = sizeof(info);
		init.GetConfigInfo(&info);
		info.dwSize = sizeof(info);
		CON_printf("Version:      %d.%d with %d Kb memory\n",
			(int)dc->Version >> 8,(int)dc->Version & 0xFF, (int)dc->TotalMemory);
		CON_printf("\n");
		CON_printf("Manufacturer: %s\n", info.ManufacturerName);
		CON_printf("Chipset:      %s\n", info.ChipsetName);
		CON_printf("RAMDAC:       %s\n", info.DACName);
		CON_printf("Clock:        %s\n", info.ClockName);
		CON_printf("Version:      %s\n", info.VersionInfo);
		CON_printf("Build:        %s", info.BuildDate);
		if (info.Certified)
			CON_printf(" (Certified %s)\n", info.CertifiedDate);
		else
			CON_printf("\n");
		CON_printf("\n");
		CON_printf("Select test to perform:\n\n");
		CON_printf("  [0] - Interactive Tests\n");
		CON_printf("  [1] - Centering and Refresh Control\n");
		CON_printf("  [2] - Global Refresh Control\n");
		CON_printf("  [3] - TV Output Control\n");
		CON_printf("  [4] - LCD Panel Output Control\n");
		CON_printf("  [5] - Color Correction (Gamma) Control\n");
		CON_printf("  [6] - VESA DPMS Power Management Tests\n");
		CON_printf("  [7] - VESA DDC Display Data Channel Tests\n");
#ifndef	LITE
		CON_printf("  [8] - VESA DDC/CI Interactive Monitor Control Tests\n");
		CON_printf("  [9] - Hardware Scrolling Test\n");
		CON_printf("  [A] - Stereo LC glasses Test\n");
#endif
		CON_printf("  [Q] - Quit\n\n");
		CON_printf("Choice: ");
		choice = tolower(EVT_getch());
		if (choice == 'q' || choice == 'Q' || choice == 0x1B)
			break;
		switch (choice) {
			case '0':   InteractiveTests(dc);		break;
			case '1':   CenteringAndRefresh(dc);	break;
			case '2':   GlobalRefresh(dc);			break;
			case '3':   TVOutputControl(dc);		break;
			case '4':   LCDOutputControl(dc);		break;
			case '5':   GammaControl(dc);			break;
			case '6':   DPMSTests(dc);				break;
			case '7':   DDCTests(dc);				break;
#ifndef	LITE
			case '8':   DDCCITests(dc);				break;
			case '9':   ScrollingTest(dc);			break;
			case 'a':   StereoTest(dc);				break;
#endif
			}
		CON_printf("\n");
		}

	/* Restore the original text mode */
	virtualX = virtualY = bytesPerLine = -1;
	init.SetVideoMode(oldMode,&virtualX,&virtualY,&bytesPerLine,&maxMem,0,NULL);
	CON_clrscr();

	/* Unload the device driver for this device */
	GA_unloadDriver(dc);
}

/****************************************************************************
PARAMETERS:
x	- X coordinate of the mouse cursor position (screen coordinates)
y	- Y coordinate of the mouse cursor position (screen coordinates)

REMARKS:
This gets called periodically to move the mouse. It will get called when
the mouse may not have actually moved, so check if it has before redrawing
it.
****************************************************************************/
void EVTAPI moveMouse(
	int x,
	int y)
{
}

/****************************************************************************
REMARKS:
Display the command line usage information.
****************************************************************************/
static void help(void)
{
#ifdef	LITE
	printf("GACtrl - SciTech Nucleus Graphics Driver Control Center\n");
#else
	printf("GATest - SciTech Nucleus Graphics Driver Compliance Test\n");
#endif
	printf("         Release %s.%s (%s)\n\n",release_major,release_minor,release_date);
	printf("%s\n",copyright_str);
	printf("\n");
	printf("Options are:\n");
	printf("    -s       - Run in software only mode\n");
	printf("    -e       - Emulate missing hardware features in software\n");
	printf("    -p       - Run in portrait mode\n");
	printf("    -l       - Run in flipped mode\n");
	printf("    -i       - Run in color inverted mode\n");
	printf("    -f       - Run tests at full speed (no wait for keypress)\n");
	printf("    -g       - Use VESA GTF for refresh rate control\n");
	printf("    -m<XxY>  - Use XxY text mode (ie: -t80x60 for 80x60 text mode)\n");
#if	defined(PRO) || defined(TEST_HARNESS)
	printf("    -t<name> - Skip to acceleration test 'name'\n");
	printf("    -b       - Execute debug break point at start of test\n");
	printf("    -k<mask> - Set plane mask for write operations\n");
	printf("    -c<XxY>  - Enable side/side multi-controller mode 2XxY\n");
#endif
	FatalErrorCleanup();
	exit(1);
}

/****************************************************************************
REMARKS:
Parses the command line arguments.
****************************************************************************/
static void parseArguments(
	int argc,
	char *argv[])
{
	int     option;
	char    *argument;

	/* Parse command line options */

	do {
		option = getopt(argc,argv,"segfplit:k:bc:",&argument);
		switch (option) {
			case 's':
				softwareOnly = true;
				break;
			case 'e':
				softwareEmulate = true;
				break;
			case 'p':
				portrait = true;
				break;
			case 'l':
				flipped = true;
				break;
			case 'i':
				invertColors = true;
				break;
			case 'f':
				fullSpeed = true;
				break;
			case 'g':
				useGTF = true;
				break;
			case 'm':
				sscanf(argument,"%dx%d", &textModeX, &textModeY);
				break;
			case 'c':
				sscanf(argument,"%dx%d", &multiModeX, &multiModeY);
				break;
			case 't':
				strcpy(accelTestName,argument);
				break;
			case 'k':
				sscanf(argument,"%lx", &planeMask);
				break;
			case 'b':
				breakPoint = true;
				break;
			case ALLDONE:
				break;
			case 'h':
			case PARAMETER:
			case INVALID:
			default:
				help();
			}
		} while (option != ALLDONE);
}

int main(int argc,char *argv[])
{
	int		i,choice;

	/* Save the state of the console */
	hwndConsole = PM_openConsole(0,0,0,0,0,true);
	if ((stateBuf = malloc(PM_getConsoleStateSize())) == NULL)
		PM_fatalError("Out of memory!");
	PM_saveConsoleState(stateBuf,hwndConsole);

	/* Initialise the event library */
	EVT_init(moveMouse);

	/* Register our fatal error cleanup handler */
	PM_setFatalErrorCleanup(FatalErrorCleanup);

	/* Register the ISV license file if desired */
#ifdef	ISV_LICENSE
	GA_registerLicense(OemLicense,false);
#endif

	/* Find the number of display devices attached */
	parseArguments(argc,argv);
	ZTimerInit();
	PM_init();
	if ((numDevices = GA_enumerateDevices(false)) > 1) {
		for (;;) {
			CON_init();
			CON_clrscr();
			banner();
			CON_printf("Select display adapter:\n\n");
			for (i = 0; i < numDevices; i++) {
				CON_printf("  [%d] - Display Adapter %d\n", i, i);
				}
			CON_printf("  [Q] - Quit\n\n");
			CON_printf("Choice: ");
			choice = EVT_getch();
			if (choice == 'q' || choice == 'Q' || choice == 0x1B)
				break;
			choice -= '0';
			if (choice >= 0 && choice < numDevices) {
				cntDevice = choice;
				GA_setActiveDevice(cntDevice);
				MainScreen();
				GA_setActiveDevice(0);
				}
			CON_restoreMode();
			}
		}
	else {
		CON_init();
		MainScreen();
		CON_restoreMode();
		}

	/* Exit the event library */
	EVT_exit();

	/* Restore the console */
	PM_restoreConsoleState(stateBuf,hwndConsole);
	PM_closeConsole(hwndConsole);
#ifdef	__LINUX__
	/* Coming back from graphics mode on Linux also restored the previous
	 * text mode console contents, so we need to clear the screen to get
	 * around this since the cursor does get homed by our code.
	 */
	printf("\033[H\033[J");
#endif
	return 0;
}
