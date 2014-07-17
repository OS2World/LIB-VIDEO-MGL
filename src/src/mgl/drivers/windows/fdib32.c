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
* Description:	Main 32 bit accelerated device driver file. This file
*				will be linked in when a call to MGL_registerDriver() is
*				made to register this device driver for use. This driver
*				requires linear framebuffer access for non-accelerated
*				code, so will only work with if a virtual or real linear
*				framebuffer is present.
*
****************************************************************************/

#include "mgldd.h"
#include "drivers/win/fdib32.h"

/*---------------------------- Driver Tables -----------------------------*/

drivertype	_VARAPI FULLDIB32_driver = {
	IDENT_FLAG,
	MGL_FULLDIB32NAME,
	MGL_FULLDIB32NAME,
	"\r\n\r\nVersion " DRIVER_VERSION_STR " - " __DATE__ "\r\n"
	"Copyright (C) 1996 SciTech Software\r\n",
	DRIVER_MAJOR_VERSION,
	DRIVER_MINOR_VERSION,
    grFULLDIB,
	FULLDIB32_detect,
	FULLDIB32_initDriver,
	};

/* List of mode specific device context information */

PRIVATE gmode_t FULLDIB32_modes[] = {
	{639,479,32,1,0xFFFFFFFFL,0,0,1000,0,0},
	{799,599,32,1,0xFFFFFFFFL,0,0,1000,0,0},
	{1023,767,32,1,0xFFFFFFFFL,0,0,1000,0,0},
	{1151,863,32,1,0xFFFFFFFFL,0,0,1000,0,0},
	{1279,959,32,1,0xFFFFFFFFL,0,0,1000,0,0},
	{1279,1023,32,1,0xFFFFFFFFL,0,0,938,0,0},
	{1599,1199,32,1,0xFFFFFFFFL,0,0,1000,0,0},
	};

PRIVATE drv_vec	*g_state = NULL;	/* Global state buffer pointer	*/

/*------------------------- Implementation --------------------------------*/

ibool MGLAPI FULLDIB32_detect(int id,int force,int *driver,int *mode,
	modetab modes)
/****************************************************************************
*
* Function:		FULLDIB32_detect
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
	if (force >= grFULLDIB && FULLDIB_detect()) {
		/* Fill in information here for the available video modes by
		 * querying the FULLDIB.DRV for the information.
		 */
		FULLDIB_useMode(modes,grSVGA_640x480x4G,id,&FULLDIB32_modes[0]);
		FULLDIB_useMode(modes,grSVGA_800x600x4G,id,&FULLDIB32_modes[1]);
		FULLDIB_useMode(modes,grSVGA_1024x768x4G,id,&FULLDIB32_modes[2]);
		FULLDIB_useMode(modes,grSVGA_1152x864x4G,id,&FULLDIB32_modes[3]);
		FULLDIB_useMode(modes,grSVGA_1280x960x4G,id,&FULLDIB32_modes[4]);
		FULLDIB_useMode(modes,grSVGA_1280x1024x4G,id,&FULLDIB32_modes[5]);
		FULLDIB_useMode(modes,grSVGA_1600x1200x4G,id,&FULLDIB32_modes[6]);
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
	if (--dc->v->d.refCount == 0) {
		if (dc->v != g_state)
			MGL_fatalError("Internal error: dc->v != g_state!\n");
		MGL_free(dc->v);
		g_state = NULL;
		}
	/* If we have a palette associated with this DC, then destroy it */
	if (dc->wm.windc.hpal)
		DeleteObject(dc->wm.windc.hpal);
	if (dc->wm.windc.hdc)
		ReleaseDC(dc->wm.windc.hwnd,dc->wm.windc.hdc);
}

ibool MGLAPI FULLDIB32_initDriver(MGLDC *dc,int driverId,int modeId,ulong hwnd,
	int virtualX,int virtualY,int numBuffers,ibool stereo,int refreshRate)
/****************************************************************************
*
* Function:		FULLDIB32_initDriver
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
	FULLDIB_state	*state;
	display_vec		*d;

	/* Allocate a single buffer for the driver specific state buffer */
	if (stereo)
		return false;
	dc->mi = FULLDIB32_modes[modeId - grSVGA_640x480x4G];
	if (g_state == NULL) {
		if ((g_state = MGL_calloc(1,sizeof(FULLDIB_state))) == NULL) {
			_MGL_result = grNoMem;
			return false;
			}
		}
	state = (FULLDIB_state*)(dc->v = g_state);
	d = &dc->v->d;
	d->refCount++;
	d->graphDriver = driverId;
	d->graphMode = modeId;
	dc->wm.windc.hwnd = (HWND)hwnd;
	dc->wm.windc.hpal = NULL;
	dc->mi.maxPage = 0;
	dc->mi.bytesPerLine = 0;
	dc->mi.pageSize = 0;
	dc->surface = dc->surfaceStart = NULL;

	/* Load the device context with the device information for the
	 * selected video mode. By default we set up the device to use the
	 * software rendering routines, and the FULLDIB_setMode() function
	 * will re-vector the appropriate routines to the hardware accelerated
	 * versions depending on what the hardware supports.
	 */
	dc->deviceType 				= MGL_FULLDIB_DEVICE;
	dc->xInch 					= 9000;
	dc->yInch 					= 7000;
	dc->ellipseFixup 			= false;
	d->hardwareCursor 			= FALSE;
	d->restoreTextMode 			= FULLDIB_restoreTextMode;
	d->restoreGraphMode			= DRV_stubVector;
	d->makeCurrent				= FULLDIB_makeCurrent;
	d->destroy					= destroyDC;
	d->makeOffscreenDC 			= NULL;
	d->makeLinearOffscreenDC 	= NULL;

	/* Fill in required device context vectors */
	dc->r.getWinDC				= WIN_getWinDC;
	dc->r.setClipRect 			= DRV_setClipRect;

	/* Fill in remaining device vectors */
	dc->r.putImage				= WIN_putImage;
	dc->r.stretchBlt1x2			= WIN_stretchBlt1x2;
	dc->r.stretchBlt2x2			= WIN_stretchBlt2x2;
	dc->r.stretchBlt			= WIN_stretchBlt;
#ifndef	MGL_LITE
	dc->r.translateImage		= WIN_translateImage;
#endif
	return FULLDIB_setMode(dc);
}

