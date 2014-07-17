/****************************************************************************
*
*                         SciTech Display Doctor
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
* Environment:  Any
*
* Description:  Module to implement the code to test the Nucleus Graphics
*				Architecture text mode support.
*
****************************************************************************/

#include "nucleus/gatest.h"

/*----------------------------- Implementation ----------------------------*/

/****************************************************************************
REMARKS:
Display the information about the video mode.
****************************************************************************/
static void displayModeInfo(
	GA_modeInfo *mi)
{
	int		i,maxx,maxy,len,attr = CON_makeAttr(CON_WHITE, CON_BLUE);
	char	buf[80];

	maxx = (mi->XResolution/mi->XCharSize)-1;
	maxy = (mi->YResolution/mi->YCharSize)-1;
	CON_writec(0,0,attr,'É');
	CON_writec(maxx,0,attr,'»');
	CON_writec(maxx,maxy,attr,'¼');
	CON_writec(0,maxy,attr,'È');
	for (i = 1; i < maxx; i++) {
		CON_writec(i,0,attr,'Í');
		CON_writec(i,maxy,attr,'Í');
		}
	for (i = 1; i < maxy; i++) {
		CON_writec(0,i,attr,'º');
		CON_writec(maxx,i,attr,'º');
		}
	sprintf(buf,"Text Mode: %d x %d (%dx%d character cell)",
		mi->XResolution / mi->XCharSize,
		mi->YResolution / mi->YCharSize,
		mi->XCharSize, mi->YCharSize);
	len = strlen(buf);
	CON_gotoxy((CON_maxx()-len)/2,CON_maxy()/2);
	CON_puts(buf);
}

/****************************************************************************
REMARKS:
Main function to do the interactive tests.
****************************************************************************/
ibool doTextTest(
	GA_devCtx *_dc,
	int xRes,
	int yRes,
	int bitsPerPixel,
	N_uint32 flags,
	int refreshRate,
	GA_CRTCInfo *crtc,
	N_uint32 planeMask)
{
	int	i,oldMode = init.GetVideoMode();

	/* Obtain the mode information and set the display mode */
	dc = _dc;
	virtualX = virtualY = bytesPerLine = -1;
	modeInfo.dwSize = sizeof(modeInfo);
	if (xRes == -1) {
		if (init.GetVideoModeInfo(flags,&modeInfo) != 0)
			return false;
		if (init.SetVideoMode(flags,&virtualX,&virtualY,&bytesPerLine,&maxMem,refreshRate,crtc) != 0)
			return false;
		}
	else {
		if (init.GetCustomVideoModeInfo(xRes,yRes,-1,-1,bitsPerPixel,&modeInfo) != 0)
			return false;
		if (init.SetCustomVideoMode(xRes,yRes,bitsPerPixel,flags,&virtualX,&virtualY,&bytesPerLine,&maxMem,crtc) != 0)
			return false;
		}
	CON_init();
	CON_setAttr(CON_makeAttr(CON_YELLOW,CON_BLUE));
	CON_fillText(0, 0, CON_maxx(), CON_maxy(),
		CON_makeAttr(CON_RED, CON_BLUE), '±');

	displayModeInfo(&modeInfo);
	if (EVT_getch() == 0x1B)
		goto DoneTest;
	CON_setBackground(CON_makeAttr(CON_CYAN, CON_BLUE), '°');
	for (i = 0; i < CON_maxy() + 1; i++)
		CON_scroll(CON_SCROLL_UP, 1);

	displayModeInfo(&modeInfo);
	if (EVT_getch() == 0x1B)
		goto DoneTest;
	CON_setBackground(CON_makeAttr(CON_RED, CON_BLUE), '²');
	for (i = 0; i < CON_maxy() + 1; i++)
		CON_scroll(CON_SCROLL_DOWN, 1);

	displayModeInfo(&modeInfo);
	EVT_getch();

	/* Return to text mode, restore the state of the console and exit */
DoneTest:
	virtualX = virtualY = bytesPerLine = -1;
	init.SetVideoMode(oldMode,&virtualX,&virtualY,&bytesPerLine,&maxMem,0,crtc);
	CON_init();
	CON_setAttr(CON_makeAttr(CON_LIGHTGRAY, CON_BLACK));
	CON_setBackground(CON_makeAttr(CON_LIGHTGRAY, CON_BLACK), ' ');
	return true;
}
