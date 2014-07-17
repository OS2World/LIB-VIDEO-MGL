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
* Description:  Module to implement the code to perform fullscreen
*				centering and refresh rate control using the Nucleus
*				Graphics Architecture functions.
*
****************************************************************************/

#include "nucleus/gatest.h"
#include "ztimer.h"

/*---------------------------- Global Variables ---------------------------*/

#define	HSTEP			8
#define	MIN_HBLANK_TIME (10 * HSTEP)
#define	MIN_VBLANK_TIME	10
#define	MIN_HSYNC_START (0 * HSTEP)
#define	MIN_HSYNC_END 	(1 * HSTEP)
#define	MIN_VSYNC_START 0
#define	MIN_VSYNC_END 	5
#define	SYNC_COUNT		5

static int				vRefresh;		/* Vertical refresh rate		*/

#ifdef DEBUG_BLANKS
extern int hBlankStart,hBlankEnd,vBlankStart,vBlankEnd;
#endif

/*----------------------------- Implementation ----------------------------*/

/****************************************************************************
REMARKS:
Function to check if the vSync line is active
****************************************************************************/
static ibool checkForVSync(void)
{
	ibool	active = false;
	ibool	vSync = driver.IsVSync();

	/* Check for changing vSync signal */
	LZTimerOn();
	while (LZTimerLap() < 100000UL) {
		if (driver.IsVSync() != vSync) {
			active = true;
			break;
			}
		}
	LZTimerOff();
	return active;
}

/****************************************************************************
REMARKS:
Function to measure the refresh rate from the hardware
****************************************************************************/
static void ReadRefresh(void)
{
	int		i;
#ifdef	__WINDOWS32__
	DWORD   oldclass;
	HANDLE  hprocess;

	/* Set the priority of the process to maximum for accurate timing */
	hprocess = GetCurrentProcess();
	oldclass = GetPriorityClass(hprocess);
	SetPriorityClass(hprocess, REALTIME_PRIORITY_CLASS);
#endif

	if (checkForVSync()) {
		driver.WaitVSync();
		driver.WaitVSync();
		LZTimerOn();
		for (i = 0; i < SYNC_COUNT; i++)
			driver.WaitVSync();
		LZTimerOff();
		vRefresh = (int)((SYNC_COUNT * 10000000.0) / LZTimerCount());
		}
	else {
		vRefresh = 0;
		}

#ifdef	__WINDOWS32__
	SetPriorityClass(hprocess, oldclass);
#endif
}

/****************************************************************************
REMARKS:
Dumps the CRTC timings to a file.
****************************************************************************/
static void dumpCRTCValues(
	GA_CRTCInfo *crtc)
{
	FILE	*f = fopen("center.log", "at+");

	fprintf(f,"\nCRTC values for %dx%d %d bit @%d.%dHz\n",
		modeInfo.XResolution,modeInfo.YResolution, modeInfo.BitsPerPixel,
		vRefresh / 10, vRefresh % 10);
	fprintf(f,"\n");
	fprintf(f,"  hTotal      = %d\n", crtc->HorizontalTotal);
	fprintf(f,"  hSyncStart  = %d\n", crtc->HorizontalSyncStart);
	fprintf(f,"  hSyncEnd    = %d\n", crtc->HorizontalSyncEnd);
#ifdef DEBUG_BLANKS
	fprintf(f,"  hBlankStart  = %d\n", hBlankStart);
	fprintf(f,"  hBlankEnd    = %d\n", hBlankEnd);
#endif
	fprintf(f,"\n");
	fprintf(f,"  vTotal      = %d\n", crtc->VerticalTotal);
	fprintf(f,"  vSyncStart  = %d\n", crtc->VerticalSyncStart);
	fprintf(f,"  vSyncEnd    = %d\n", crtc->VerticalSyncEnd);
#ifdef DEBUG_BLANKS
	fprintf(f,"  vBlankStart  = %d\n", vBlankStart);
	fprintf(f,"  vBlankEnd    = %d\n", vBlankEnd);
#endif
	fprintf(f,"\n");
	fprintf(f,"  Interlaced  = %s\n", (crtc->Flags & gaInterlaced) ? "Yes" : "No");
	fprintf(f,"  Double scan = %s\n", (crtc->Flags & gaDoubleScan) ? "Yes" : "No");
	fprintf(f,"  H sync pol  = %s\n", (crtc->Flags & gaHSyncNeg) ? "-" : "+");
	fprintf(f,"  V sync pol  = %s\n", (crtc->Flags & gaVSyncNeg) ? "-" : "+");
	fprintf(f,"\n");
	fprintf(f,"  Dot Clock   = %d.%02dMhz\n",	(int)crtc->PixelClock / 1000000, (int)crtc->PixelClock / 10000);
	fclose(f);
}

/****************************************************************************
REMARKS:
Draw the background image for the centering and refresh display
****************************************************************************/
static void drawBackground(
	GA_CRTCInfo *crtc,
	ibool readRefresh)
{
	char   		buf[80];
	int     	i,x,y,min,max,maxx,maxy,attr;
	long		range;
	GA_palette	pal[256],*p;

	if (modeInfo.BitsPerPixel == 0) {
		CON_init();
		CON_setAttr(CON_makeAttr(CON_YELLOW,CON_BLUE));
		CON_fillText(0, 0, CON_maxx(), CON_maxy(), CON_makeAttr(CON_RED, CON_BLUE), '±');
		attr = CON_makeAttr(CON_WHITE, CON_BLUE);
		maxx = CON_maxx();
		maxy = CON_maxy();
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
		x = CON_maxx()/2 - 30;
		y = CON_maxy()/2 - 9;
		sprintf(buf,"Text Mode: %d x %d (%dx%d character cell)",
			modeInfo.XResolution / modeInfo.XCharSize,
			modeInfo.YResolution / modeInfo.YCharSize,
			modeInfo.XCharSize, modeInfo.YCharSize);
		CON_gotoxy(x,y);
		CON_puts(buf); CON_gotoxy(x,y+=2);
		CON_puts("Adjust mode parameters with the following keys:"); CON_gotoxy(x,y+=2);
		CON_puts("      \x1B Move image left"); CON_gotoxy(x,y+=1);
		CON_puts("      \x1A Move image right"); CON_gotoxy(x,y+=1);
		CON_puts("      \x18 Move image up"); CON_gotoxy(x,y+=1);
		CON_puts("      \x19 Move image down"); CON_gotoxy(x,y+=1);
		CON_puts(" Ctrl \x1B Increase image size horizontally"); CON_gotoxy(x,y+=1);
		CON_puts(" Ctrl \x1A Decrease image size horizontally"); CON_gotoxy(x,y+=1);
		CON_puts("      h Toggle horizontal sync polarity"); CON_gotoxy(x,y+=1);
		CON_puts("      v Toggle vertical sync polarity"); CON_gotoxy(x,y+=1);
		CON_puts("      + Increase vertical refresh"); CON_gotoxy(x,y+=1);
		CON_puts("      - Decrease vertical refresh"); CON_gotoxy(x,y+=1);
		CON_puts("      r Restore original values"); CON_gotoxy(x,y+=1);
		CON_puts("Press <Enter> to accept changes, ESC to quit without saving"); CON_gotoxy(x,y+=1);
		sprintf(buf,"hSync '%s', vSync '%s'",
			(crtc->Flags & gaHSyncNeg) ? "-" : "+",
			(crtc->Flags & gaVSyncNeg) ? "-" : "+");
		CON_gotoxy(CON_maxx()-20,2);
		CON_puts(buf);
		if (readRefresh)
			ReadRefresh();
		sprintf(buf,"Refresh Rate: %d.%dHz", vRefresh / 10, vRefresh % 10);
		CON_gotoxy(CON_maxx() - 20,1);
		CON_puts(buf);
		}
	else {
		if (modeInfo.BitsPerPixel == 4) {
			ClearPage(0);
			moire(defcolor);
			}
		else {
			min = 32;
			max = 253;
			range = max - min;
			for (i = 0; i < 254; i++) {
				pal[i].Red = 0;
				pal[i].Blue = (((i*range)/254)+min);
				pal[i].Green = 0;
				}
			pal[254].Red = pal[254].Green = pal[254].Blue = 128;
			pal[255].Red = pal[255].Green = pal[255].Blue = 255;
			if (modeInfo.BitsPerPixel > 8) {
				for (i = 0; i < maxY; i++) {
					p = &pal[(i * 254L) / maxY];
					SetForeColor(rgbColor(p->Red,p->Green,p->Blue));
					draw2d.DrawLineInt(0,i,maxX,i,true);
					}
				}
			else {
				driver.SetPaletteData(pal,256,0,false);
				for (i = 0; i < maxY; i++) {
					SetForeColor((i * 254L) / maxY);
					draw2d.DrawLineInt(0,i,maxX,i,true);
					}
				defcolor = 255;
				}
			SetForeColor(defcolor);
			draw2d.DrawLineInt(0,0,maxX,0,true);
			draw2d.DrawLineInt(0,0,0,maxY,true);
			draw2d.DrawLineInt(maxX,0,maxX,maxY,true);
			draw2d.DrawLineInt(0,maxY,maxX,maxY,true);
			}
		if (maxX < 639 || maxY < 479) {
			x = maxX/2 - 145;
			y = maxY/2 - 65;
			sprintf(buf,"Mode: %d x %d %d bits per pixel",(int)maxX+1,(int)maxY+1,(int)modeInfo.BitsPerPixel);
			WriteText(x,y,buf,defcolor);    y += 32;
			WriteText(x,y,"     \x1B Move image left",defcolor); y += 16;
			WriteText(x,y,"     \x1A Move image right",defcolor); y += 16;
			WriteText(x,y,"     \x18 Move image up",defcolor); y += 16;
			WriteText(x,y,"     \x19 Move image down",defcolor); y += 16;
			WriteText(x,y,"     r Restore original values",defcolor); y += 16;
			y += 8;
			WriteText(x,y,"Press <Enter> to accept, ESC to quit",defcolor);
			}
		else {
			x = maxX/2 - 240;
			y = maxY/2 - 146;
			sprintf(buf,"Video mode: %d x %d %d bits per pixel",(int)maxX+1,(int)maxY+1,(int)modeInfo.BitsPerPixel);
			WriteText(x,y,buf,defcolor);    y += 32;
			WriteText(x,y,"Adjust mode parameters with the following keys:",defcolor); y += 32;
			WriteText(x,y,"      \x1B Move image left",defcolor); y += 16;
			WriteText(x,y,"      \x1A Move image right",defcolor); y += 16;
			WriteText(x,y,"      \x18 Move image up",defcolor); y += 16;
			WriteText(x,y,"      \x19 Move image down",defcolor); y += 16;
			WriteText(x,y," Ctrl \x1B Increase image size horizontally",defcolor); y += 16;
			WriteText(x,y," Ctrl \x1A Decrease image size horizontally",defcolor); y += 16;
			WriteText(x,y,"      h Toggle horizontal sync polarity",defcolor); y += 16;
			WriteText(x,y,"      v Toggle vertical sync polarity",defcolor); y += 16;
			WriteText(x,y,"      + Increase vertical refresh",defcolor); y += 16;
			WriteText(x,y,"      - Decrease vertical refresh",defcolor); y += 16;
			WriteText(x,y,"      r Restore original values",defcolor); y += 16;
#ifdef DEBUG_BLANKS
			WriteText(x,y,"     [] Adjust horizontal blank start",defcolor); y += 16;
			WriteText(x,y,"     {} Adjust horizontal blank end",defcolor); y += 16;
			WriteText(x,y,"     ,. Adjust vertical blank start",defcolor); y += 16;
			WriteText(x,y,"     <> Adjust vertical blank end",defcolor); y += 16;
#endif
			y += 16;
			WriteText(x,y,"Press <Enter> to accept changes, ESC to quit without saving",defcolor);
			sprintf(buf,"hSync '%s', vSync '%s'",
				(crtc->Flags & gaHSyncNeg) ? "-" : "+",
				(crtc->Flags & gaVSyncNeg) ? "-" : "+");
			WriteText(maxX-180,24,buf,defcolor);
			}
		if (readRefresh)
			ReadRefresh();
		sprintf(buf,"Refresh Rate: %d.%dHz", vRefresh / 10, vRefresh % 10);
		WriteText(maxX-180,8,buf,defcolor);
		}
}

/****************************************************************************
REMARKS:
Main function to do the interactive tests.
****************************************************************************/
ibool doCenter(
	GA_devCtx *_dc,
	int xRes,
	int yRes,
	int bitsPerPixel,
	N_uint32 flags,
	int refreshRate,
	GA_CRTCInfo *crtc,
	N_uint32 planeMask)
{
	ibool		updateTimings,updateRefresh;
	int			ch,oldMode = init.GetVideoMode();
	GA_CRTCInfo	defCRTC;
	event_t		evt;

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
	cntMode = flags;
	if (modeInfo.BitsPerPixel >= 4)
		InitSoftwareRasterizer(cntDevice,1);

	/* Draw the background image */
	init.GetCRTCTimings(crtc);
	init.SetCRTCTimings(crtc);
	defCRTC = *crtc;
	refreshRate = crtc->RefreshRate / 100;
	drawBackground(crtc,true);

	/* Now process key events */
	for (;;) {
		EVT_halt(&evt,EVT_KEYDOWN | EVT_KEYREPEAT);
		ch = EVT_scanCode(evt.message);
		if (ch == KB_esc || ch == KB_Q) {
			/* Restore the original timings */
			GA_restoreCRTCTimings(dc);
			break;
			}
		if (ch == KB_enter) {
			/* Save the timing changes */
			GA_saveCRTCTimings(dc);
			break;
			}

		/* Now handle the key event */
		updateTimings = false;
		updateRefresh = false;
		switch (ch) {
			case KB_H:			// Toggle hsync polarity
				crtc->Flags ^= gaHSyncNeg;
				drawBackground(crtc,false);
				updateTimings = true;
				break;
			case KB_V:			// Toggle vsync polarity
				crtc->Flags ^= gaVSyncNeg;
				drawBackground(crtc,false);
				updateTimings = true;
				break;
			case KB_R:			// reset to initial values
				*crtc = defCRTC;
				updateTimings = true;
				updateRefresh = true;
				break;
			case KB_D:			// Dump CRTC values to file
				dumpCRTCValues(crtc);
				break;
			case KB_equals:
			case KB_padPlus:
				if (useGTF) {
					refreshRate++;
					if (GA_computeCRTCTimings(dc,&modeInfo,refreshRate,crtc->Flags & gaInterlaced,crtc,false)) {
						updateTimings = true;
						updateRefresh = true;
						}
					}
				else if (crtc->PixelClock + 250000 <= modeInfo.MaxPixelClock) {
					crtc->PixelClock += 250000;
					updateTimings = true;
					updateRefresh = true;
					}
				break;
			case KB_minus:
			case KB_padMinus:
				if (useGTF) {
					refreshRate--;
					if (GA_computeCRTCTimings(dc,&modeInfo,refreshRate,crtc->Flags & gaInterlaced,crtc,false)) {
						updateTimings = true;
						updateRefresh = true;
						}
					}
				else {
					crtc->PixelClock -= 250000;
					updateTimings = true;
					updateRefresh = true;
					}
				break;
			case KB_up:
				if (evt.modifiers & EVT_CTRLSTATE) {
					// Increase image size vertically
					if (crtc->VerticalTotal > (modeInfo.YResolution + MIN_VBLANK_TIME)) {
						crtc->VerticalTotal--;
						updateTimings = true;
						}
					}
				else {
					// Move the image up
					if (crtc->VerticalSyncEnd < (crtc->VerticalTotal - MIN_VSYNC_END)) {
						crtc->VerticalSyncStart++;
						crtc->VerticalSyncEnd++;
						updateTimings = true;
						}
					}
				break;
			case KB_down:
				if (evt.modifiers & EVT_CTRLSTATE) {
					// Decrease image size vertically
					crtc->VerticalTotal++;
					updateTimings = true;
					}
				else {
					// Move the image down
					if (crtc->VerticalSyncStart > (modeInfo.YResolution + MIN_VSYNC_START)) {
						crtc->VerticalSyncStart--;
						crtc->VerticalSyncEnd--;
						updateTimings = true;
						}
					}
				break;
			case KB_left:
				if (evt.modifiers & EVT_CTRLSTATE) {
					// Decrease image size horizontally
					crtc->HorizontalTotal += HSTEP;
					updateTimings = true;
					}
				else {
					// Move the image left
					if (crtc->HorizontalSyncEnd < (crtc->HorizontalTotal - MIN_HSYNC_END)) {
						crtc->HorizontalSyncStart += HSTEP;
						crtc->HorizontalSyncEnd += HSTEP;
						updateTimings = true;
						}
					}
				break;
			case KB_right:
				if (evt.modifiers & EVT_CTRLSTATE) {
					// Increase image size horizontally
					if (crtc->HorizontalTotal > (modeInfo.XResolution + MIN_HBLANK_TIME)) {
						crtc->HorizontalTotal -= HSTEP;
						updateTimings = true;
						}
					}
				else {
					// Move the image right
					if (crtc->HorizontalSyncStart > (modeInfo.XResolution + MIN_HSYNC_START)) {
						crtc->HorizontalSyncStart -= HSTEP;
						crtc->HorizontalSyncEnd -= HSTEP;
						updateTimings = true;
						}
					}
				break;
#ifdef DEBUG_BLANKS
			case KB_leftSquareBrace:
				if (evt.modifiers & EVT_SHIFTKEY)
					hBlankEnd -= 8;
				else
					hBlankStart -= 8;
				updateTimings = true;
				break;
			case KB_rightSquareBrace:
				if (evt.modifiers & EVT_SHIFTKEY)
					hBlankEnd += 8;
				else
					hBlankStart += 8;
				updateTimings = true;
				break;
			case KB_comma:
				if (evt.modifiers & EVT_SHIFTKEY)
					vBlankEnd -= 8;
				else
					vBlankStart -= 8;
				updateTimings = true;
				break;
			case KB_period:
				if (evt.modifiers & EVT_SHIFTKEY)
					vBlankEnd += 8;
				else
					vBlankStart += 8;
				updateTimings = true;
				break;
#endif
			}

		/* Update the timings if they have changed */
		if (updateTimings) {
			init.SetCRTCTimings(crtc);
			if (updateRefresh)
				drawBackground(crtc,true);
			}
		}

	/* Return to text mode, restore the state of the console and exit */
	if (modeInfo.BitsPerPixel >= 4)
		ExitSoftwareRasterizer();
	virtualX = virtualY = bytesPerLine = -1;
	init.SetVideoMode(oldMode,&virtualX,&virtualY,&bytesPerLine,&maxMem,0,crtc);
	if (modeInfo.BitsPerPixel == 0) {
		CON_init();
		CON_setAttr(CON_makeAttr(CON_LIGHTGRAY, CON_BLACK));
		CON_setBackground(CON_makeAttr(CON_LIGHTGRAY, CON_BLACK), ' ');
		}
	return true;
}
