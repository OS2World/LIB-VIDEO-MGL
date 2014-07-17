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
* Environment:  Any 32-bit protected choice environment
*
* Description:  Module to implement the code to test the Nucleus Graphics
*				Architecture DPMS and DDC functions.
*
****************************************************************************/

#include "nucleus/gatest.h"

/*----------------------------- Implementation ----------------------------*/

/****************************************************************************
REMARKS:
Displays the menu of DPMS tests and allows the states to be tested.
****************************************************************************/
void DPMSTests(
	GA_devCtx *dc)
{
	int    			oldMode,choice,state;
	N_int16			supportedStates;
	GA_DPMSFuncs	dpms;

	/* Detect if the DPMS interface is active */
	dpms.dwSize = sizeof(dpms);
	if (!dc->QueryFunctions(GA_GET_DPMSFUNCS,0,&dpms) || !dpms.DPMSdetect(&supportedStates) || supportedStates == 0) {
		CON_printf("\nDPMS services not available...\n");
		EVT_getch();
		return;
		}

	/* For NonVGA controllers we need to run this test in graphics modes
	 * to ensure that we are running on the proper controller hardware
	 * that we support DPMS on (ie: the Windows driver hardware).
	 */
	oldMode = SetGraphicsMode(dc);

	/* Display menu information on screen */
	gmoveto(40,40);
	gprintf("DPMS support active. Supports the following states:");
	if (supportedStates & DPMS_standby)
		gprintf("  STAND BY");
	if (supportedStates & DPMS_suspend)
		gprintf("  SUSPEND");
	if (supportedStates & DPMS_off)
		gprintf("  OFF");
	gnewline();
	gprintf("Enter which choice (ESC to quit):");
	gprintf("  [0]   - ON");
	gprintf("  [1]   - Stand-By");
	gprintf("  [2]   - Suspend");
	gprintf("  [3]   - Off");
	gprintf("  [Esc] - Quit");
	gnewline();
	gprintf("Which: ");

	/* Loop around trying the different DPMS states */
	for (;;) {
		choice = EVT_getch();
		if (choice == 0x1B || tolower(choice) == 'q')
			break;
		switch (choice) {
			case '1': state = DPMS_standby;	break;
			case '2': state = DPMS_suspend;	break;
			case '3': state = DPMS_off;		break;
			default:  state = DPMS_on;
			}
		dpms.DPMSsetState(state);
		}

	/* Reset to ON state before we exit! */
	dpms.DPMSsetState(DPMS_on);

	/* Restore text mode for NonVGA controllers */
	RestoreMode(dc,oldMode);
}
