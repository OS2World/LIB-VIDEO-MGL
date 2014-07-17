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
* Description:  Generic utility funcitons used by the Nucleus test programs.
*
****************************************************************************/

#include "nucleus/gatest.h"
#include "pmapi.h"

/*----------------------------- Implementation ----------------------------*/

/****************************************************************************
REMARKS:
Sets the 640x480x256 linear framebuffer SVGA graphics mode.
****************************************************************************/
int SetGraphicsMode(
	GA_devCtx *_dc)
{
	GA_CRTCInfo		crtc;
	N_uint16		*modes;
	int				oldMode,mode = -1;

	/* Search for the 640x480x256 graphics mode */
	dc = _dc;
	for (modes = dc->AvailableModes; *modes != 0xFFFF; modes++) {
		modeInfo.dwSize = sizeof(modeInfo);
		if (init.GetVideoModeInfo(*modes,&modeInfo) != 0)
			continue;
		if (modeInfo.Attributes & gaIsTextMode)
			continue;
		if (modeInfo.XResolution == 640 && modeInfo.YResolution == 480 && modeInfo.BitsPerPixel == 8) {
			mode = *modes;
			if (modeInfo.Attributes & gaHaveLinearBuffer)
				mode |= gaLinearBuffer;
			break;
			}
		}
	if (mode == -1)
		PM_fatalError("Unable to set 640x480x256 graphics mode!");

	/* Set text mode and initialise console library */
	virtualX = virtualY = bytesPerLine = -1;
	oldMode = init.GetVideoMode();
	init.SetVideoMode(mode,&virtualX,&virtualY,&bytesPerLine,&maxMem,0,&crtc);
	cntMode = mode;
	InitSoftwareRasterizer(cntDevice,1);
	return oldMode;
}

/****************************************************************************
REMARKS:
Restore the old display mode active before graphics mode was started.
****************************************************************************/
void RestoreMode(
	GA_devCtx *dc,
	int oldMode)
{
	GA_CRTCInfo		crtc;

	/* Set text mode and initialise console library */
	virtualX = virtualY = bytesPerLine = -1;
	init.SetVideoMode(oldMode,&virtualX,&virtualY,&bytesPerLine,&maxMem,0,&crtc);
}

/****************************************************************************
REMARKS:
Function to find the name of a display mode from the mode information.
****************************************************************************/
int GetModeName(
	char *buf,
	GA_modeInfo *mi)
{
	char    buf1[80];
	int     attr = mi->Attributes;
	int		pages = mi->MaxScanLines / mi->YResolution;
	int		xChars,yChars;

	/* Build the name of the mode */
	if (attr & gaIsTextMode) {
		xChars = mi->XResolution / mi->XCharSize;
		yChars = mi->YResolution / mi->YCharSize;
		sprintf(buf,"%dx%d Text Mode (%dx%d character cell)",
			xChars,yChars,mi->XCharSize,mi->YCharSize);
		}
	else {
		if (!(attr & gaHaveLinearBuffer))
			strcpy(buf1,", Banked");
		else
			strcpy(buf1,"");
		sprintf(buf,"%dx%dx%d, %d page%s",mi->XResolution,mi->YResolution,
			mi->BitsPerPixel,pages,buf1);
		if (attr & gaHaveAccel2D)
			strcat(buf,", Accel");
		if (attr & gaHaveTripleBuffer)
			strcat(buf,", TripleBuf");
		if (attr & gaHaveStereo)
			strcat(buf,", Stereo");
		if (attr & gaHaveNonVGAMode)
			strcat(buf,", NonVGA");
		}
	return true;
}

/****************************************************************************
REMARKS:
Initialise the mouse driver
****************************************************************************/
void InitMouse(
	int xRes,
	int yRes)
{
	EVT_setMouseRange(xRes,yRes);
}

/****************************************************************************
REMARKS:
Gets the current mouse cursor position from the mouse driver.
****************************************************************************/
void GetMousePos(
	int *x,
	int *y)
{
	EVT_getMousePos(x,y);
}

/****************************************************************************
REMARKS:
Simple utility function to use the event library to check if a key has
been hit. We check for keydown and keyrepeat events, and we also flush the
event queue of all non keydown events to avoid it filling up.
****************************************************************************/
int EVT_kbhit(void)
{
	int		hit;
	event_t	evt;

	hit = EVT_peekNext(&evt,EVT_KEYDOWN | EVT_KEYREPEAT);
	EVT_flush(~(EVT_KEYDOWN | EVT_KEYREPEAT));
	return hit;
}

/****************************************************************************
REMARKS:
Simple utility function to use the event library to read an ASCII keypress
and return it. This function will block until a key is hit.
****************************************************************************/
int EVT_getch(void)
{
	event_t	evt;

	do {
		EVT_halt(&evt,EVT_KEYDOWN | EVT_KEYREPEAT);
		} while (EVT_asciiCode(evt.message) == 0);
	return EVT_asciiCode(evt.message);
}

/****************************************************************************
REMARKS:
Function to read a string from the keyboard, using the console library
for output.
****************************************************************************/
int GetString(
	char *str,
	int maxLen)
{
	int	x,y,c,len = 0;

	x = CON_wherex();
	y = CON_wherey();
	for (;;) {
		if ((c = EVT_getch()) == 0xD) {
			str[len] = 0;
			CON_printf("\n");
			return 1;
			}
		if (c == 0x1B)
			return -1;
		if (c == 0x08) {
			if (len > 0) {
				len--;
				x--;
				CON_gotoxy(x,y);
				CON_putc(' ');
				CON_gotoxy(x,y);
				}
			}
		else if (isprint(c)) {
			if (len < maxLen) {
				str[len++] = c;
				CON_gotoxy(x,y);
				CON_putc(c & 0xFF);
				x++;
				}
			}
		}
}
