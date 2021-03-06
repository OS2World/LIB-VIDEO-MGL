/****************************************************************************
*
*					SciTech OS Portability Manager Library
*
*  ========================================================================
*
*    The contents of this file are subject to the SciTech MGL Public
*    License Version 1.0 (the "License"); you may not use this file
*    except in compliance with the License. You may obtain a copy of
*    the License at http://www.scitechsoft.com/mgl-license.txt
*
*    Software distributed under the License is distributed on an
*    "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
*    implied. See the License for the specific language governing
*    rights and limitations under the License.
*
*    The Original Code is Copyright (C) 1991-1998 SciTech Software, Inc.
*
*    The Initial Developer of the Original Code is SciTech Software, Inc.
*    All Rights Reserved.
*
*  ========================================================================
*
* Language:		ANSI C
* Environment:	any
*
* Description:  Test program to check the ability to install a C based
*				Real Time Clock interrupt handler.
*
*				Functions tested:	PM_setRealTimeClockHandler()
*									PM_restoreRealTimeClockHandler()
*
****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "pmapi.h"

volatile long count = 0;

#pragma off (check_stack)			/* No stack checking under Watcom	*/

void PMAPI RTCHandler(void)
{
	count++;
}

int main(void)
{
	long		oldCount;

	printf("Program running in ");
	switch (PM_getModeType()) {
		case PM_realMode:
			printf("real mode.\n\n");
			break;
		case PM_286:
			printf("16 bit protected mode.\n\n");
			break;
		case PM_386:
			printf("32 bit protected mode.\n\n");
			break;
		}

	/* Install our timer handler and lock handler pages in memory. It is
	 * difficult to get the size of a function in C, but we know our
	 * function is well less than 100 bytes (and an entire 4k page will
	 * need to be locked by the server anyway).
	 */
	PM_lockCodePages((__codePtr)RTCHandler,100);
	PM_lockDataPages((void*)&count,sizeof(count));
	PM_installBreakHandler();			/* We *DONT* want Ctrl-Breaks! */
	PM_setRealTimeClockHandler(RTCHandler,128);
	printf("RealTimeClock interrupt handler installed - Hit ESC to exit\n");
	oldCount = count;
	while (1) {
		if (PM_kbhit() && (PM_getch() == 0x1B))
			break;
		if (count != oldCount) {
			printf("Tick, Tock: %ld\n", count);
			oldCount = count;
			}
		}

	PM_restoreRealTimeClockHandler();
	PM_restoreBreakHandler();
	PM_unlockDataPages((void*)&count,sizeof(count));
	PM_unlockCodePages((__codePtr)RTCHandler,100);
	printf("RealTimeClock handler was called %ld times\n", count);
	return 0;
}
