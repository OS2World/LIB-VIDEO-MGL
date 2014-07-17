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
* Description:  Module to implement the code to test the Nucleus Graphics
*				Architecture DPMS and DDC functions.
*
****************************************************************************/

#include "nucleus/gatest.h"

/*----------------------------- Implementation ----------------------------*/

static void removeNewLine(char *s)
{
	int len = strlen(s);
	if (s[len-1] == '\n')
		s[len-1] = 0;
}

static void safeGetLine(char *s,int len)
{
	GetString(s,len);
	removeNewLine(s);
}

/****************************************************************************
REMARKS:
Tests the MCS_isControlSupported function
****************************************************************************/
void testMCS_isControlSupported(void)
{
	int		controlCode;
	char	buf[80];

	gprintf("Enter control code: ");
	safeGetLine(buf,sizeof(buf));
	sscanf(buf,"%x",&controlCode);
	if (MCS_isControlSupported(controlCode))
		gprintf("Control is supported");
	else
		gprintf("Control is not supported");
	EVT_getch();
}

/****************************************************************************
REMARKS:
Tests the MCS_enableControl function
****************************************************************************/
void testMCS_enableControl(void)
{
	int		controlCode,enable;
	char	buf[80];

	gprintf("Enter control code: ");
	safeGetLine(buf,sizeof(buf));
	sscanf(buf,"%x",&controlCode);
	gprintf("Enter enable code (0 or 1): ");
	safeGetLine(buf,sizeof(buf));
	sscanf(buf,"%d",&enable);
	if (MCS_enableControl(controlCode,enable))
		gprintf("Control enable updated");
	else
		gprintf("MCS_enableControl failed!");
	EVT_getch();
}

/****************************************************************************
REMARKS:
Tests the MCS_getControlValue function
****************************************************************************/
void testMCS_getControlValue(void)
{
	int		controlCode;
	ushort	value,max;
	char	buf[80];

	gprintf("Enter control code: ");
	safeGetLine(buf,sizeof(buf));
	sscanf(buf,"%x",&controlCode);
	if (MCS_getControlMax(controlCode,&max) && MCS_getControlValue(controlCode,&value)) {
		gprintf("Control Values:");
		gprintf(" value = %d", value);
		gprintf(" max   = %d", max);
		}
	else
		gprintf("MCS_getControlValue failed!");
	EVT_getch();
}

/****************************************************************************
REMARKS:
Tests the MCS_setControlValue function
****************************************************************************/
void testMCS_setControlValue(void)
{
	int		controlCode;
	ushort	value;
	char	buf[80];

	gprintf("Enter control code: ");
	safeGetLine(buf,sizeof(buf));
	sscanf(buf,"%x",&controlCode);
	gprintf("Enter value: ");
	safeGetLine(buf,sizeof(buf));
	sscanf(buf,"%hd",&value);
	if (MCS_setControlValue(controlCode,value))
		gprintf("Control value updated");
	else
		gprintf("MCS_setControlValue failed!");
	EVT_getch();
}

/****************************************************************************
REMARKS:
Tests the MCS_resetControl function
****************************************************************************/
void testMCS_resetControl(void)
{
	int		controlCode;
	char	buf[80];

	gprintf("Enter control code: ");
	safeGetLine(buf,sizeof(buf));
	sscanf(buf,"%x",&controlCode);
	if (MCS_resetControl(controlCode))
		gprintf("Control value reset to factory default");
	else
		gprintf("MCS_resetControl failed!");
	EVT_getch();
}

/****************************************************************************
REMARKS:
Tests the MCS_getTimingReport function
****************************************************************************/
void testMCS_getTimingReport(void)
{
	uchar	flags;
	ushort	hFreq,vFreq;

	if (MCS_getTimingReport(&flags,&hFreq,&vFreq)) {
		gprintf("Timing Report:");
		gprintf(" flags = %02X", flags);
		gprintf(" hFreq = %.2f kHz", (hFreq+50.0) / 100.0);
		gprintf(" vFreq = %.2f Hz", (vFreq+50.0) / 100.0);
		gprintf(" hSync = %c", (flags & MCS_hSyncPositive) ? '+' : '-');
		gprintf(" vSync = %c", (flags & MCS_vSyncPositive) ? '+' : '-');
		}
	else
		gprintf("MCS_getTimingReport failed!");
	EVT_getch();
}

/****************************************************************************
REMARKS:
Tests the MCS_saveCurrentSettings function
****************************************************************************/
void testMCS_saveCurrentSettings(void)
{
	if (MCS_saveCurrentSettings())
		gprintf("Current settings saved in monitor NVRAM");
	else
		gprintf("MCS_saveCurrentSettings failed!");
	EVT_getch();
}

/****************************************************************************
REMARKS:
Tests the MCS_getSelfTestReport function
****************************************************************************/
void testMCS_getSelfTestReport(void)
{
	uchar	flags,data[256],length;

	if (MCS_getSelfTestReport(&flags,data,&length)) {
		gprintf("Self Test Report:");
		gprintf(" flags  = %02X", flags);
		gprintf(" length = %d", length);
		}
	else
		gprintf("MCS_getSelfTestReport failed!");
	EVT_getch();
}

void testMCS_getCapabilitiesString(void)
{
	char	data[1024];

	if (MCS_getCapabilitiesString(data,sizeof(data)) != -1) {
		gprintf("Capabilities String:");
		gprintf("%s", data);
		}
	else
		gprintf("MCS_getCapabilitiesString failed!");
	EVT_getch();
}

/****************************************************************************
REMARKS:
Displays the menu of DDC tests and allows the DDC functionality to be
tested.
****************************************************************************/
void DDCCITests(
	GA_devCtx *dc)
{
	int			oldMode,choice,err;
	GA_SCIFuncs	sci;

	/* For NonVGA controllers we need to run this test in graphics modes
	 * to ensure that we are running on the proper controller hardware
	 * that we support DPMS on (ie: the Windows driver hardware).
	 */
	oldMode = SetGraphicsMode(dc);

	/* Allow a 1.5 second delay before trying to do DDC/CI communications */
	_OS_delay(1500000);

	/* Initialise DDC/CI communications */
	sci.dwSize = sizeof(sci);
	if (!dc->QueryFunctions(GA_GET_SCIFUNCS,0,&sci) || (err = MCS_begin(dc)) != 0) {
		RestoreMode(dc,oldMode);
		CON_clrscr();
		if (err == 1)
			CON_printf("DDC/CI not supported!\n\n");
		else
			CON_printf("DDC/CI monitor not connected!\n\n");
		CON_printf("Press any key to continue");
		EVT_getch();
		return;
		}

	/* Display menu information on screen */
	for (;;) {
		ClearPage(0);
		gmoveto(40,40);
		gprintf("DDC/CI support active:");
		gnewline();
		gprintf("Enter function to test:");
		gprintf("  [0] - Is Control Supported");
		gprintf("  [1] - Enable Control");
		gprintf("  [2] - Get Control Value");
		gprintf("  [3] - Set Control Value");
		gprintf("  [4] - Reset Control");
		gprintf("  [5] - Get Timing Report");
		gprintf("  [6] - Save Current Settings");
		gprintf("  [7] - Get Self Test Report");
		gprintf("  [8] - Get Capabilities String");
		gprintf("  [Esc] - Quit");
		gprintf("Which: ");

		/* Loop around trying the different DPMS states */
		choice = EVT_getch();
		if (choice == 0x1B || tolower(choice) == 'q')
			break;
		gnewline();
		switch (choice) {
			case '0': testMCS_isControlSupported(); 	break;
			case '1': testMCS_enableControl();			break;
			case '2': testMCS_getControlValue();		break;
			case '3': testMCS_setControlValue();		break;
			case '4': testMCS_resetControl();			break;
			case '5': testMCS_getTimingReport();		break;
			case '6': testMCS_saveCurrentSettings();	break;
			case '7': testMCS_getSelfTestReport();		break;
			case '8': testMCS_getCapabilitiesString();	break;
			}
		}

	/* Close the DDC/CI communications channel */
	MCS_end();

	/* Restore text mode for NonVGA controllers */
	RestoreMode(dc,oldMode);
}
