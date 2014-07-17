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
* Description:  Module to implement the code to test the Nucleus Graphics
*				Architecture.
*
****************************************************************************/

#include "nucleus/gasdk.h"
#ifndef	__WIN32_VXD__
#include <stdarg.h>
#endif

/*---------------------------- Global Variables ---------------------------*/

static ibool		forceSoftwareCount = 0;
ibool				softwareOnly = false;
N_uint16			cntMode;
N_int32				x,y,maxX,maxY,maxPage,activePage,visualPage,maxMem;
N_int32 			pageSize,virtualX,virtualY,bytesPerLine,bytesPerPixel;
N_int32				isStereo,useBlueCode,blueCodeIndex;
GA_color			defcolor,maxcolor,foreColor,backColor;
GA_modeInfo			modeInfo;
REF2D_driver		*ref2d;
GA_devCtx			*dc;
GA_2DStateFuncs		drv_state2d,state2d;
GA_2DRenderFuncs	drv_draw2d,draw2d;
GA_buffer			smallBuf;

/*----------------------------- Implementation ----------------------------*/

/****************************************************************************
REMARKS:
Returns the value representing the color. The value is converted from 24 bit
RGB space into the appropriate color for the video mode.
****************************************************************************/
GA_color rgbColor(
	uchar r,
	uchar g,
	uchar b)
{
	return ((ulong)((r >> modeInfo.PixelFormat.RedAdjust)   & modeInfo.PixelFormat.RedMask)   << modeInfo.PixelFormat.RedPosition)
		 | ((ulong)((g >> modeInfo.PixelFormat.GreenAdjust) & modeInfo.PixelFormat.GreenMask) << modeInfo.PixelFormat.GreenPosition)
		 | ((ulong)((b >> modeInfo.PixelFormat.BlueAdjust)  & modeInfo.PixelFormat.BlueMask)  << modeInfo.PixelFormat.BluePosition);
}

/****************************************************************************
REMARKS:
Returns the real color value for the specified color. In RGB modes we look
up the color value from the palette and pack into the proper format.
****************************************************************************/
GA_color realColor(
	int index)
{
	if (modeInfo.BitsPerPixel <= 8)
		return index;
	return rgbColor(VGA8_defPal[index].Red,
					VGA8_defPal[index].Green,
					VGA8_defPal[index].Blue);
}

/****************************************************************************
REMARKS:
Set the current foreground color for both the hardware driver and the
reference rasteriser.
****************************************************************************/
void SetForeColor(
	GA_color color)
{
	state2d.SetForeColor(color);
	foreColor = color;
}

/****************************************************************************
REMARKS:
Set the current background color for both the hardware driver and the
reference rasteriser.
****************************************************************************/
void SetBackColor(
	GA_color color)
{
	state2d.SetBackColor(color);
	backColor = color;
}

/****************************************************************************
REMARKS:
Set the current mix for both the hardware driver and the reference rasteriser.
****************************************************************************/
void SetMix(
	N_int32 mix)
{
	state2d.SetMix(mix);
}

/****************************************************************************
REMARKS:
Set the 8x8 monochrome bitmap pattern for both the hardware driver and the
reference rasteriser.
****************************************************************************/
void Set8x8MonoPattern(
	N_int32 index,
	GA_pattern *pattern)
{
	state2d.Set8x8MonoPattern(index,pattern);
}

/****************************************************************************
REMARKS:
Select an 8x8 monochrome bitmap pattern for both the hardware driver and the
reference rasteriser.
****************************************************************************/
void Use8x8MonoPattern(
	N_int32 index)
{
	state2d.Use8x8MonoPattern(index);
}

/****************************************************************************
REMARKS:
Select an 8x8 monochrome bitmap pattern for both the hardware driver and the
reference rasteriser.
****************************************************************************/
void Use8x8TransMonoPattern(
	N_int32 index)
{
	state2d.Use8x8TransMonoPattern(index);
}

/****************************************************************************
REMARKS:
Set the 8x8 color bitmap pattern for both the hardware driver and the
reference rasteriser.
****************************************************************************/
void Set8x8ColorPattern(
	N_int32 index,
	GA_colorPattern *pattern)
{
	state2d.Set8x8ColorPattern(index,pattern);
}

/****************************************************************************
REMARKS:
Select an 8x8 color bitmap pattern for both the hardware driver and the
reference rasteriser.
****************************************************************************/
void Use8x8ColorPattern(
	N_int32 index)
{
	state2d.Use8x8ColorPattern(index);
}

/****************************************************************************
REMARKS:
Select an 8x8 color bitmap pattern for both the hardware driver and the
reference rasteriser.
****************************************************************************/
void Use8x8TransColorPattern(
	N_int32 index,
	GA_color transparent)
{
	state2d.Use8x8TransColorPattern(index,transparent);
}

/****************************************************************************
REMARKS:
Set the line stipple pattern for both the hardware driver and the
reference rasteriser.
****************************************************************************/
void SetLineStipple(
	GA_stipple stipple)
{
	state2d.SetLineStipple(stipple);
}

/****************************************************************************
REMARKS:
Set the line stipple count for both the hardware driver and the
reference rasteriser.
****************************************************************************/
void SetLineStippleCount(
	N_uint32 count)
{
	state2d.SetLineStippleCount(count);
}

/****************************************************************************
REMARKS:
Set the plane mask for both the hardware driver and the reference rasteriser.
****************************************************************************/
void SetPlaneMask(N_uint32 planeMask)
{
	state2d.SetPlaneMask(planeMask);
}

/****************************************************************************
REMARKS:
Find the draw buffer for the active drawing page in hardware video memory.
****************************************************************************/
static void GetActiveDrawBuffer(
	int page,
	GA_buffer *drawBuf)
{
	if (smallBuf.dwSize != 0) {
		drawBuf->dwSize = sizeof(GA_buffer);
		drawBuf->Offset = smallBuf.Offset;
		drawBuf->Stride = smallBuf.Stride;
		drawBuf->Width = smallBuf.Width;
		drawBuf->Height = smallBuf.Height;
		return;
		}

	if (isStereo != gaNoStereo) {
		if (page & gaRightBuffer)
			page = ((page & ~gaRightBuffer) * 2) + 1;
		else
			page *= 2;
		}
	drawBuf->dwSize = sizeof(GA_buffer);
	drawBuf->Offset = page * pageSize;
	drawBuf->Stride = bytesPerLine;
	drawBuf->Width = virtualX;
	drawBuf->Height = virtualY;
}

/****************************************************************************
REMARKS:
Set the currently active drawing page for both the hardware driver and
the reference rasteriser. We use this to implement hardware page flipping.
****************************************************************************/
void SetActivePage(
	int page)
{
	GA_buffer drawBuf;

	activePage = page;
	GetActiveDrawBuffer(page,&drawBuf);
	if (state2d.SetDrawBuffer(&drawBuf) != 0)
		PM_fatalError("SetDrawBuffer failed!");
}

/****************************************************************************
REMARKS:
Set the currently visible page for the hardware driver. We use this to
implement hardware page flipping.
****************************************************************************/
void SetVisualPage(
	int page,
	int waitVRT)
{
	int 	oldActivePage = activePage;
	ulong	addr,blueCodeColor;

	visualPage = page;
	if (isStereo != gaNoStereo && useBlueCode) {
		/* Get the color for the blue code */
		if (maxcolor > 255)
			blueCodeColor = rgbColor(0,0,255);
		else
			blueCodeColor = blueCodeIndex;

		/* Draw the left image blue code (25% of width) */
		SetActivePage(page | gaLeftBuffer);
		SetForeColor(blueCodeColor);
		draw2d.DrawLineInt(0,maxY,maxX / 4,maxY,true);
		SetForeColor(0);
		draw2d.DrawLineInt(maxX / 4,maxY,maxX,maxY,true);

		/* Draw the right image blue code (75% of width) */
		SetActivePage(page | gaRightBuffer);
		SetForeColor(blueCodeColor);
		draw2d.DrawLineInt(0,maxY,(maxX * 3) / 4,maxY,true);
		SetForeColor(0);
		draw2d.DrawLineInt((maxX * 3) / 4,maxY,maxX,maxY,true);
		SetActivePage(oldActivePage);
		}
	if (isStereo != gaNoStereo)
		page *= 2;
	if (maxPage <= 1 && waitVRT == gaTripleBuffer)
		waitVRT = gaWaitVRT;

	/* Ensure that previous display start has taken hold if we have hardware
	 * triple buffering. We do this by polling the GetDisplayStartStatus
	 * function until it tells us that the last frame has taken hold. In most
	 * cases unless the framerate is running close to the refresh rate of
	 * the video mode, this function loop will terminate immediately and we
	 * never wait. If we do wait, it is because we are running too fast!
	 */
	if (isStereo == gaSoftStereo) {
		if (waitVRT == gaTripleBuffer)
			GA_softStereoWaitTillFlipped();
		}
	else {
		if (modeInfo.Attributes & gaHaveTripleBuffer && (waitVRT == gaTripleBuffer)) {
			while (driver.GetDisplayStartStatus() == 0)
				;
			}
		}
	addr = page * pageSize;
	if (modeInfo.BitsPerPixel < 8)
		addr *= 8;
	if (isStereo == gaSoftStereo) {
		GA_softStereoScheduleFlip(addr,addr+pageSize);
		if (waitVRT == gaWaitVRT)
			GA_softStereoWaitTillFlipped();
		}
	else if (isStereo == gaHWStereo)
		driver.SetStereoDisplayStart(addr,addr+pageSize,(waitVRT == gaWaitVRT));
	else
		driver.SetDisplayStart(addr,(waitVRT == gaWaitVRT));
}

/****************************************************************************
REMARKS:
Start stereo page flipping mode.
****************************************************************************/
ibool StartStereo(void)
{
	if (isStereo == gaNoStereo) {
		if (!(modeInfo.Attributes & gaHaveStereo)) {
			/* Enable software stereo page flipping using a timer interrupt */
			if (!GA_softStereoInit(dc))
				return false;
			isStereo = gaSoftStereo;
			useBlueCode = true;
			}
		else {
			/* Enable support for hardware stereo mode */
			isStereo = gaHWStereo;
			}
		if (useBlueCode && modeInfo.BitsPerPixel == 8) {
			/* Enable support for blue code software stereo sync signal */
			VGA8_defPal[blueCodeIndex].Red = 0;
			VGA8_defPal[blueCodeIndex].Green = 0;
			VGA8_defPal[blueCodeIndex].Blue = 0xFF;
			driver.SetPaletteData(VGA8_defPal,256,0,false);
			}
		if (isStereo == gaHWStereo) {
			/* Enable hardware stereo support */
			driver.EnableStereoMode(true);
			}
		else {
			/* Enable software stereo page flipping */
			GA_softStereoOn();
			}
		}
	return true;
}

/****************************************************************************
REMARKS:
Stops stereo page flipping mode.
****************************************************************************/
void StopStereo(void)
{
	if (isStereo != gaNoStereo) {
		if (isStereo == gaHWStereo) {
			/* Disable hardware stereo mode */
			driver.EnableStereoMode(false);
			}
		else {
			/* Disable software stereo page flipping */
			GA_softStereoExit();
			}
		isStereo = gaNoStereo;
		}
}

/****************************************************************************
REMARKS:
Clears the current display page.
****************************************************************************/
void ClearPage(
	GA_color color)
{
	SetForeColor(color);
	draw2d.DrawRect(0,0,maxX+1,maxY+1);
}

/****************************************************************************
REMARKS:
Writes the string to the display
****************************************************************************/
void WriteText(
	int x,
	int y,
	char *s,
	GA_color color)
{
	uchar	*font,*image;
	int     length,ch;

	SetForeColor(color);
	font = dc->TextFont8x16;
	length = strlen(s);
	while (length--) {
		ch = *s++;
		image = (font + ch * 16);
		if (x <= 0 || y <= 0 || x+8 >= modeInfo.XResolution || y+16 >= modeInfo.YResolution) {
			draw2d.ClipMonoImageMSBSys(x,y,8,16,1,image,true,
				0,0,modeInfo.XResolution,modeInfo.YResolution);
			}
		else {
			draw2d.PutMonoImageMSBSys(x,y,8,16,1,image,true);
			}
		x += 8;
		}
}

/****************************************************************************
PARAMETERS:
x,y	- Location to move text cursor to

REMARKS:
Moves the current text location to the specified position.
****************************************************************************/
void gmoveto(
	int _x,
	int _y)
{
	x = _x;
	y = _y;
}

/****************************************************************************
REMARKS:
Moves the current text location to the next line down
****************************************************************************/
void gnewline(void)
{ y += 16; }

/****************************************************************************
REMARKS:
Returns the current x position
****************************************************************************/
int ggetx(void)
{ return x; }

/****************************************************************************
REMARKS:
Returns the current y position
****************************************************************************/
int ggety(void)
{ return y; }

/****************************************************************************
PARAMETERS:
fmt     - Format string
...     - Standard printf style parameters

REMARKS:
Simple printf style output routine for sending text to the current graphics
modes. It begins drawing the string at the current location, and moves to
the start of the next logical line.
****************************************************************************/
int gprintf(
	char *fmt,
	...)
{
	va_list argptr;
	char    buf[255];
	int     cnt;

	va_start(argptr,fmt);
	cnt = vsprintf(buf,fmt,argptr);
	WriteText(x,y,buf,defcolor);
	y += 16;
	va_end(argptr);
	(void)argptr;
	return cnt;
}

/****************************************************************************
REMARKS:
This function forces the Nucleus reference rasteriser functions to be
used for all drawing functions, overiding any hardware functions.
****************************************************************************/
void ForceSoftwareOnly(void)
{
	GA_buffer drawBuf;
	GetActiveDrawBuffer(activePage,&drawBuf);
	if (ref2d->SetDrawBuffer(&drawBuf,dc->LinearMem,modeInfo.BitsPerPixel,&modeInfo.PixelFormat,dc,true) != 0)
		PM_fatalError("REF2D_SetDrawBuffer failed!");
	forceSoftwareCount++;
}

/****************************************************************************
REMARKS:
Main function to initialise the software rasteriser module and the internal
library helper functions.
****************************************************************************/
void RevertSoftwareOnly(void)
{
	GA_buffer drawBuf;
	GetActiveDrawBuffer(activePage,&drawBuf);
	if (forceSoftwareCount)
		forceSoftwareCount--;
	if (forceSoftwareCount) {
		if (ref2d->SetDrawBuffer(&drawBuf,dc->LinearMem,modeInfo.BitsPerPixel,&modeInfo.PixelFormat,dc,true) != 0)
			PM_fatalError("REF2D_SetDrawBuffer failed!");
		}
	else {
		if (ref2d->SetDrawBuffer(&drawBuf,dc->LinearMem,modeInfo.BitsPerPixel,&modeInfo.PixelFormat,dc,softwareOnly) != 0)
			PM_fatalError("REF2D_SetDrawBuffer failed!");
		}
}

/****************************************************************************
REMARKS:
Main function to initialise the software rasteriser module and the internal
library helper functions.
****************************************************************************/
void InitSoftwareRasterizer(
	int deviceIndex,
	int reservePages)
{
	/* Initialise the dimensions for the screen and setup drawing buffer */
	maxX = virtualX-1;
	maxY = virtualY-1;
	pageSize = bytesPerLine * virtualY;
	maxPage = (modeInfo.MaxScanLines / modeInfo.YResolution) - 1;
	activePage = 0;
	isStereo = gaNoStereo;
	useBlueCode = true;
	blueCodeIndex = 255;

	/* Load the Nucleus reference rasteriser for the color depth */
	memset(&state2d,0,sizeof(state2d));
	memset(&draw2d,0,sizeof(draw2d));
	if (!GA_loadRef2d(dc,false,&modeInfo,pageSize * reservePages,&ref2d))
		PM_fatalError("Unable to load Nucleus Reference Rasteriser!");
	drv_state2d.dwSize = sizeof(drv_state2d);
	dc->QueryFunctions(GA_GET_2DSTATEFUNCS,0,&drv_state2d);
	drv_draw2d.dwSize = sizeof(drv_draw2d);
	dc->QueryFunctions(GA_GET_2DRENDERFUNCS,0,&drv_draw2d);
	driver.dwSize = sizeof(driver);
	ref2d->QueryFunctions(GA_GET_DRIVERFUNCS,&driver);
	state2d.dwSize = sizeof(state2d);
	ref2d->QueryFunctions(GA_GET_2DSTATEFUNCS,&state2d);
	draw2d.dwSize = sizeof(draw2d);
	ref2d->QueryFunctions(GA_GET_2DRENDERFUNCS,&draw2d);
	forceSoftwareCount = 0;
	RevertSoftwareOnly();

	/* Now initialise color depth specific values */
	switch (modeInfo.BitsPerPixel) {
		case 4:
			bytesPerPixel = 1;
			defcolor = maxcolor = 0xF;
			break;
		case 8:
			bytesPerPixel = 1;
			defcolor = 0xF;
			maxcolor = 0xFF;
			break;
		case 15:
		case 16:
			bytesPerPixel = 2;
			defcolor = maxcolor = rgbColor(0xFF,0xFF,0xFF);
			break;
		case 24:
			bytesPerPixel = 3;
			defcolor = maxcolor = rgbColor(0xFF,0xFF,0xFF);
			break;
		case 32:
			bytesPerPixel = 4;
			defcolor = maxcolor = rgbColor(0xFF,0xFF,0xFF);
			break;
		}

	/* Set the default mixes and state... */
	SetActivePage(0);
	SetVisualPage(0,gaDontWait);
	SetMix(GA_REPLACE_MIX);
	SetForeColor(defcolor);
	SetBackColor(0);

	/* Program the default color palette */
	if (modeInfo.BitsPerPixel == 8)
		driver.SetPaletteData(VGA8_defPal,256,0,false);
	else if (modeInfo.BitsPerPixel == 4)
		driver.SetPaletteData(VGA4_defPal,16,0,false);

	(void)deviceIndex;
}

/****************************************************************************
REMARKS:
Function to exit the software rasteriser module and clean up
****************************************************************************/
void ExitSoftwareRasterizer(void)
{
	GA_unloadRef2d(dc);
}
