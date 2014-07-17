/****************************************************************************
*
*					SciTech Multi-platform Graphics Library
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
* Environment:	Any
*
* Description:	Main 24 bit accelerated device driver file. This file
*				will be linked in when a call to MGL_registerDriver() is
*				made to register this device driver for use. This driver
*				requires linear framebuffer access for non-accelerated
*				code, so will only work with if a virtual or real linear
*				framebuffer is present.
*
****************************************************************************/

#include "mgldd.h"
#ifndef	__WIN386__
#include "drivers/win/ddraw24.h"

/*---------------------------- Driver Tables -----------------------------*/

drivertype	_VARAPI DDRAW24_driver = {
	IDENT_FLAG,
	MGL_DDRAW24NAME,
	MGL_DDRAW24NAME,
	"\r\n\r\nVersion " DRIVER_VERSION_STR " - " __DATE__ "\r\n"
	"Copyright (C) 1996 SciTech Software\r\n",
	DRIVER_MAJOR_VERSION,
	DRIVER_MINOR_VERSION,
    grDDRAW,
	DDRAW24_detect,
	DDRAW24_initDriver,
	};

/* List of mode specific device context information */

PRIVATE gmode_t DDRAW24_modes[] = {
	{319,199,24,1,0xFFFFFFL,0,0,1200,0,0},
	{319,239,24,1,0xFFFFFFL,0,0,1000,0,0},
	{319,399,24,1,0xFFFFFFL,0,0,600,0,0},
	{319,479,24,1,0xFFFFFFL,0,0,500,0,0},
	{399,299,24,1,0xFFFFFFL,0,0,1000,0,0},
	{511,383,24,1,0xFFFFFFL,0,0,1000,0,0},
	{639,349,24,1,0xFFFFFFL,0,0,1371,0,0},
	{639,399,24,1,0xFFFFFFL,0,0,1200,0,0},
	{639,479,24,1,0xFFFFFFL,0,0,1000,0,0},
	{799,599,24,1,0xFFFFFFL,0,0,1000,0,0},
	{1023,767,24,1,0xFFFFFFL,0,0,1000,0,0},
	{1151,863,24,1,0xFFFFFFL,0,0,1000,0,0},
	{1279,959,24,1,0xFFFFFFL,0,0,1000,0,0},
	{1279,1023,24,1,0xFFFFFFL,0,0,938,0,0},
	{1599,1199,24,1,0xFFFFFFL,0,0,1000,0,0},
	};

PRIVATE drv_vec	*g_state = NULL;	/* Global state buffer pointer	*/

/*------------------------- Implementation --------------------------------*/

ibool MGLAPI DDRAW24_detect(int id,int force,int *driver,int *mode,
	modetab modes)
/****************************************************************************
*
* Function:		DDRAW24_detect
* Parameters:	id		- Id of this driver for storing in mode table
*				force	- Highest performance driver to be used
*               driver	- Place to store detected driver id
*				mode	- Place to store recommended video mode
*               modes	- Place to store list of supported modes
* Returns:		TRUE if the device was found, FALSE if not.
*
* Description:	Determines if an VGA or compatible adapter is present, and
*				if so fills in the mode table with all of the modes
*				supported by this device driver.
*
*				If the value of 'driver' is less than the value of our
*               device driver number, then we return false since the user
*				has requested to ignore our devices modes.
*
****************************************************************************/
{
	if (force >= grDDRAW && DDRAW_detect(driver,&DDRAW24_driver.driverId)) {
		/* Fill in information here for the available video modes by
		 * querying the DDRAW.DRV for the information.
		 */
		DDRAW_useMode(modes,grSVGA_320x200x16m,id,&DDRAW24_modes[0]);
		DDRAW_useMode(modes,grSVGA_320x240x16m,id,&DDRAW24_modes[1]);
		DDRAW_useMode(modes,grSVGA_320x400x16m,id,&DDRAW24_modes[2]);
		DDRAW_useMode(modes,grSVGA_320x480x16m,id,&DDRAW24_modes[3]);
		DDRAW_useMode(modes,grSVGA_400x300x16m,id,&DDRAW24_modes[4]);
		DDRAW_useMode(modes,grSVGA_512x384x16m,id,&DDRAW24_modes[5]);
		DDRAW_useMode(modes,grSVGA_640x350x16m,id,&DDRAW24_modes[6]);
		DDRAW_useMode(modes,grSVGA_640x400x16m,id,&DDRAW24_modes[7]);
		DDRAW_useMode(modes,grSVGA_640x480x16m,id,&DDRAW24_modes[8]);
		DDRAW_useMode(modes,grSVGA_800x600x16m,id,&DDRAW24_modes[9]);
		DDRAW_useMode(modes,grSVGA_1024x768x16m,id,&DDRAW24_modes[10]);
		DDRAW_useMode(modes,grSVGA_1152x864x16m,id,&DDRAW24_modes[11]);
		DDRAW_useMode(modes,grSVGA_1280x960x16m,id,&DDRAW24_modes[12]);
		DDRAW_useMode(modes,grSVGA_1280x1024x16m,id,&DDRAW24_modes[13]);
		DDRAW_useMode(modes,grSVGA_1600x1200x16m,id,&DDRAW24_modes[14]);
		return true;
		}
	mode = mode;
	return false;
}

PRIVATE void MGLAPI destroyDC(MGLDC *dc)
/****************************************************************************
*
* Function:		destroyDC
* Parameters:	dc	- Device context
*
* Description:	Destroys all internal memory allocated by the device
*				context.
*
****************************************************************************/
{
	FULLSCREEN_destroyHDC(dc);
	if (--dc->v->d.refCount == 0) {
		if (dc->v != g_state)
			MGL_fatalError("Internal error: dc->v != g_state!\n");
		MGL_free(dc->v);
		g_state = NULL;
		}
}

PUBLIC void DDRAW24_initInternal(MGLDC *dc)
/****************************************************************************
*
* Function:		DDRAW24_initInternal
* Parameters:	dc	- Device context.
* Returns:		True if the device was correctly initialised.
*
* Description:	Initialises the device driver internal tables for 8 bit
*				unaccelerated driver routines.
*
****************************************************************************/
{
	/* Fill in required device context vectors */
    dc->r.getWinDC				= FULLSCREEN_getWinDC;
	dc->r.getDefaultPalette 	= DDRAW_getDefaultPalette;
	dc->r.setColor 				= DRV_setColor;
	dc->r.setBackColor 			= DRV_setBackColor;
	dc->r.setWriteMode			= DRV_setWriteMode;
	dc->r.setClipRect 			= DRV_setClipRect;

#ifndef	MGL_LITE
	dc->r.setPenStyle			= DRV_setPenStyle;
	dc->r.setPenBitmapPattern	= DRV_setPenBitmapPattern;
	dc->r.setPenPixmapPattern	= DRV_setPenPixmapPattern;
	dc->r.setLineStipple		= DRV_setLineStipple;
	dc->r.setLineStippleCount	= DRV_setLineStippleCount;
#endif

	_MGL_computePF(dc);
	PACKED24_initInternal(dc);
}

ibool MGLAPI DDRAW24_initDriver(MGLDC *dc,int driverId,int modeId,ulong hwnd,
	int virtualX,int virtualY,int numBuffers,ibool stereo,int refreshRate)
/****************************************************************************
*
* Function:		DDRAW24_initDriver
* Parameters:	dc	- Device context.
* Returns:		True if the device was correctly initialised.
*
* Description:	Initialises the device driver, and starts the specified
*				graphics mode. This is also where we fill in all of the
*				vectors in the device context to initialise our device
*				context properly.
*
****************************************************************************/
{
	DDRAW_state	*state;
	display_vec	*d;

	/* Allocate a single buffer for the driver specific state buffer */
	dc->mi = DDRAW24_modes[modeId - grSVGA_320x200x16m];
	if (g_state == NULL) {
		if ((g_state = MGL_calloc(1,sizeof(DDRAW_state))) == NULL) {
			_MGL_result = grNoMem;
			return false;
			}
		}
	state = (DDRAW_state*)(dc->v = g_state);
	d = &dc->v->d;
	d->refCount++;
	d->hwnd = hwnd;
	d->graphDriver = driverId;
	d->graphMode = modeId;

	/* Load the device context with the device information for the
	 * selected video mode. By default we set up the device to use the
	 * software rendering routines, and the DDRAW_setMode() function
	 * will re-vector the appropriate routines to the hardware accelerated
	 * versions depending on what the hardware supports.
	 */
	dc->deviceType 			= MGL_DISPLAY_DEVICE;
	dc->xInch 				= 9000;
	dc->yInch 				= 7000;
	d->hardwareCursor 		= FALSE;
	d->restoreTextMode 		= DDRAW_restoreTextMode;
	d->restoreGraphMode		= DRV_stubVector;
	d->makeCurrent			= DDRAW_makeCurrent;
	d->destroy				= destroyDC;
	d->makeOffscreenDC 		= NULL;
	d->makeLinearOffscreenDC = NULL;

	/* Set the video mode */
	if (!DDRAW_setMode(dc,virtualX,virtualY,numBuffers,stereo,refreshRate))
		return false;

	DDRAW24_initInternal(dc);
	DDRAW_setAccelVectors(dc);
	return true;
}

#endif	/* !__WIN386__ */

