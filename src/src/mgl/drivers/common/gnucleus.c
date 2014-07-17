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
* Description:	Common C module for Graphics Accelerator routines used
*				by all SciTech Nucleus accelerated device drivers.
*
****************************************************************************/

#include "mgl.h"
#include "drivers/common/gnucleus.h"

/*------------------------- Global Variables ------------------------------*/

/* Currently active pointers which are set for every makeCurrent call */

void (NAPIP _MGL_gaWaitTillIdle)(void) = NULL;
void (NAPIP _MGL_gaEnableDirectAccess)(void) = NULL;
void (NAPIP _MGL_gaDisableDirectAccess)(void) = NULL;

/*-------------------------- Implementation -------------------------------*/

/****************************************************************************
RETURNS:
Pointer to intialised instance data.

REMARKS:
This function initialises any necessary instance data for the device.
****************************************************************************/
void * MGLAPI NUCL_initInstance(
	void *inst)
{
	NUCL_data	*instance = (NUCL_data*)inst;

	if (instance) {
		/* Flag that this driver has not been inst->svga.detected yet */
		instance->svga.detected = -1;
		instance->svga.accelerated = false;
		}
	return inst;
}

/****************************************************************************
PARAMETERS:
dc	- Device context

REMARKS:
Destroys all internal memory allocated for the device context.
****************************************************************************/
static void MGLAPI NUCL_destroyDC(
	MGLDC *dc)
{
	if (--dc->v->d.refCount == 0) {
		/* Nothing to do in here */
		}
}

/****************************************************************************
PARAMETERS:
inst	- Pointer to the device instance data
id		- Driver id to store in table
mode	- Index into mode association table
modes	- Video mode/device driver association table
md		- Nucleus mode information block to fill in

REMARKS:
Searches through the list of accelerated modes supported by the loaded
driver for one that matches the desired resolution and pixel depth.
****************************************************************************/
static ibool NUCL_useMode(
	NUCL_data *inst,
	int id,
	int mode,
	modetab modes,
	GA_mdEntry	*md,
	ibool accelerated)
{
	ulong	flags = 0;

	/* Fail the mode if we require acceleration or vice versa */
	if (accelerated != ((md->Attributes & gaHaveAccel2D) != 0))
		return false;

	/* Extract hardware flags for the mode */
	if (md->HaveRefresh)
		flags |= MGL_HAVE_REFRESH_CTRL;
	if (md->Attributes & gaHaveLinearBuffer)
		flags |= MGL_HAVE_LINEAR;
	if (md->Attributes & gaHaveInterlaced)
		flags |= MGL_HAVE_INTERLACED;
	if (md->Attributes & gaHaveDoubleScan)
		flags |= MGL_HAVE_DOUBLE_SCAN;
	if (md->Attributes & gaHaveTripleBuffer)
		flags |= MGL_HAVE_TRIPLEBUFFER;
	if (md->Attributes & gaHaveStereo)
		flags |= MGL_HAVE_STEREO | MGL_HAVE_STEREO_DUAL;
	if (md->Attributes & gaHaveHWStereoSync)
		flags |= MGL_HAVE_STEREO_HWSYNC;
	if (md->Attributes & gaHaveEVCStereoSync)
		flags |= MGL_HAVE_STEREO_EVCSYNC;
	if (md->Attributes & gaHaveHWCursor)
		flags |= MGL_HAVE_HWCURSOR;
	if (md->Attributes & gaHaveAccel2D)
		flags |= MGL_HAVE_ACCEL_2D;
	if (md->Attributes & gaHaveAccel3D)
		flags |= MGL_HAVE_ACCEL_3D;
	if (md->Attributes & gaHaveAccelVideo)
		flags |= MGL_HAVE_ACCEL_VIDEO;

	/* Ok, the mode is valid so enable it */
	DRV_useMode(modes,mode,id,md->XResolution,md->YResolution,
		md->BitsPerPixel,md->MaxBuffers,flags);
	(void)inst;
	return true;
}

/****************************************************************************
RETURNS:
True if the video card is a supported Accelerated SuperVGA

REMARKS:
Determines if a configured ACCEL.DRV driver is available to be loaded. If
the driver is found, we load the driver file into our applications code
space.
****************************************************************************/
ibool MGLAPI NUCL_detect(
	NUCL_data *inst,
	ibool accelerated,
	int id,
	int *numModes,
	modetab availableModes)
{
	GA_modeInfo		modeInfo;
	int				i;
	N_uint16  		*p;
	GA_mdEntry		*md;
	GA_initFuncs	*init;
	GA_driverFuncs	*drv;

	/* Check if we have already detected this driver */
	if (inst->svga.detected != -1)
		return inst->svga.detected;

	/* Load and the Nuclues driver if it has not already been loaded */
	if (!DEV.gaPtr) {
		if ((DEV.gaPtr = GA_loadDriver(_MGL_cntDevice,false)) == NULL)
			return inst->svga.detected = false;
		}

	/* Initialise the driver if it has not already been initialised */
	if (!DEV.gaInfo) {
		/* Allocate memory for the Nucleus dection info, and get functions */
		init = &inst->svga.init;
		init->dwSize = sizeof(*init);
		if (!DEV.gaPtr->QueryFunctions(GA_GET_INITFUNCS,0,init))
			MGL_fatalError("QueryFunctions(GA_GET_INITFUNCS) failed!");
		drv = &inst->svga.drv;
		drv->dwSize = sizeof(*drv);
		if (!DEV.gaPtr->QueryFunctions(GA_GET_DRIVERFUNCS,0,drv))
			MGL_fatalError("QueryFunctions(GA_GET_DRIVERFUNCS) failed!");

		/* Enumerate all display modes available in the driver */
		p = DEV.gaPtr->AvailableModes;
		for (i = 0,md = inst->svga.modeList; *p != 0xFFFF; p++) {
			modeInfo.dwSize = sizeof(modeInfo);
			if (init->GetVideoModeInfo(*p,&modeInfo) != 0)
				continue;
			if (modeInfo.Attributes & gaIsTextMode)
				continue;
			if (i >= MAX_DRIVER_MODES)
				break;
			if (modeInfo.Attributes & gaHaveAccel2D)
				inst->svga.accelerated = true;
			md->mode 					= *p;
			md->XResolution				= modeInfo.XResolution;
			md->YResolution				= modeInfo.YResolution;
			md->BitsPerPixel			= modeInfo.BitsPerPixel;
			md->HaveRefresh 			= modeInfo.RefreshRateList[1] != -1;
			md->Attributes 				= modeInfo.Attributes;
			md->BytesPerScanLine 		= modeInfo.BytesPerScanLine;
			md->PixelFormat 			= modeInfo.PixelFormat;
			md->MaxBuffers 				= modeInfo.MaxBuffers;
			md->BitmapStartAlign		= modeInfo.BitmapStartAlign;
			md->BitmapStridePad			= modeInfo.BitmapStridePad;
			i++;
			md++;
			}
		md->mode = -1;

		/* Save the mode list so we can reuse it for other drivers */
		DEV.gaInfo = &inst->svga;
		}
	else {
		/* We have already detected the Nucleus interface for a different
		 * driver, so copy the cached mode information from the other driver.
		 */
		inst->svga = *((NUCL_detectInfo*)DEV.gaInfo);
		}

	/* Now register all compatible display modes for this driver */
	for (md = inst->svga.modeList; md->mode != -1; md++) {
		if (*numModes >= MAX_DRIVER_MODES)
			break;
		if (NUCL_useMode(inst,id,*numModes,availableModes,md,accelerated))
			(*numModes)++;
		}
	return inst->svga.detected = true;
}

/****************************************************************************
REMARKS:
This function call the Nucleus driver to set the graphics mode given the
passed in parameters. If the refresh rate is not set to the adapter default
setting, we attempt to find a suitable refresh rate using the GTF timing
formulas.
****************************************************************************/
static ibool GA_setMode(
	MGLDC *dc,
	int mode,
	N_int32 *virtualX,
	N_int32 *virtualY,
	N_int32 *bytesPerLine,
	N_int32	*maxMem,
	GA_CRTCInfo *crtc)
{
	GA_initFuncs *init = &SVGA(dc).init;
	*bytesPerLine = -1;
	return (init->SetVideoMode(mode,virtualX,virtualY,bytesPerLine,maxMem,0,crtc) == 0);
}

/****************************************************************************
REMARKS:
Update all cached global variables in the driver.
****************************************************************************/
static void NUCL_updateCachedGlobals(
	MGLDC *dc)
{
	NUCL_detectInfo *svga = &SVGA(dc);
	SET_PTR(_MGL_gaWaitTillIdle,svga->WaitTillIdle);
	SET_PTR(_MGL_gaEnableDirectAccess,svga->EnableDirectAccess);
	SET_PTR(_MGL_gaDisableDirectAccess,svga->DisableDirectAccess);
}

/****************************************************************************
PARAMETERS:
dc			- Device context to initialise
mode		- Accelerated video mode to set
oldMode		- Place to store old BIOS mode number
virtualX	- Virtual X coordinate for the display mode
virtualY	- Virtual Y coordinate for the display mode
numBuffers	- Number of buffers to allocate for the mode
stereo		- True if stereo is enabled
refreshRate	- Refresh rate for the display mode

RETURNS:
True on success, false on failure.

REMARKS:
Initialises the video mode by calling the loaded accelerated device driver
to initialise the video mode. We also save the state of the old video mode
in the specified parameters.
****************************************************************************/
static ibool NUCL_setMode(
	MGLDC *dc,
	N_uint32 mode,
	N_uint32 *oldMode,
	N_int32 virtualX,
	N_int32 virtualY,
	N_int32 numBuffers,
	ibool stereo,
	N_int32 refreshRate,
	ibool useLinearBlits)
{
	display_vec			*d = &dc->v->d;
	int					isStereo,scrolling,interlaced,startLine,startOffset;
	float 				maxRefresh;
	N_int32				maxMem,bytesPerLine = -1;
	GA_modeInfo			modeInfo;
	GA_CRTCInfo			crtc;
	GA_options			opt;
	NUCL_detectInfo 	*svga = &SVGA(dc);
	GA_initFuncs 		*init = &svga->init;
	GA_2DStateFuncs		drvState2d;
	GA_2DRenderFuncs	drvDraw2d;

	/* Get the driver options so we can determine what type of LC glasses
	 * the user has connected to the computer.
	 */
	opt.dwSize = sizeof(opt);
	init->GetOptions(&opt);

	/* If stereo mode is enabled, check what type of stereo mode we support
	 * and set things up for stereo operation.
	 */
	isStereo = _MGL_NO_STEREO;
	if (stereo) {
		isStereo = _MGL_SOFT_STEREO;
		if (dc->mi.modeFlags & MGL_HAVE_STEREO) {
			/* We have hardware stereo page flipping, so use it */
			isStereo = _MGL_HW_STEREO;
			}

		/* Configure the stereo type depending on the glasses attached.
		 * Unless the user has glasses attached to the hardware sync port
		 * we have to use either blue codes, keyframes or fall back on
		 * pure software stereo flipping (for I/O port and VSync glasses).
		 */
		switch (opt.glassesType) {
			case gaGlassesBlueCode:
				isStereo |= _MGL_STEREO_BLUECODE;
				break;
			case gaGlassesIOPort:
			case gaGlassesVSync:
				isStereo = _MGL_SOFT_STEREO;
				break;
			}
		numBuffers *= 2;
		dc->flags |= MGL_STEREO_ACCESS;
		}

	/* Check that we are withing the virtual dimensions */
	if (numBuffers == 0)
		return false;
	if (virtualX != -1 && virtualY != -1) {
		if (virtualX <= dc->mi.xRes || virtualY <= dc->mi.yRes)
			return false;
		scrolling = true;
		}
	else {
		virtualX = virtualY = -1;
		scrolling = false;
		}

	/* Determine if we are doing multi-buffering */
	if (numBuffers && ((numBuffers-1) <= dc->mi.maxPage) && dc->mi.maxPage > 0) {
		if (stereo)
			dc->mi.maxPage = (numBuffers/2)-1;
		else
			dc->mi.maxPage = numBuffers-1;
		}
	else
		dc->mi.maxPage = 0;

	/* Save the old video mode */
	*oldMode = init->GetVideoMode();

	/* Compute CRTC timings if refresh rate is specified */
	modeInfo.dwSize = sizeof(modeInfo);
	init->GetVideoModeInfo(mode & gaModeMask,&modeInfo);
	if (refreshRate != MGL_DEFAULT_REFRESH) {
		GA_getMaxRefreshRate(DEV.gaPtr,&modeInfo,false,&maxRefresh);
		if (refreshRate < 0) {
			refreshRate = -refreshRate;
			if (refreshRate > (int)maxRefresh/2)
				refreshRate = (int)maxRefresh/2;
			if (refreshRate < 87)
				refreshRate = 87;
			interlaced = true;
			}
		else {
			if (refreshRate > (int)maxRefresh)
				refreshRate = (int)maxRefresh;
			if (refreshRate < 56)
				refreshRate = 56;
			interlaced = false;
			}
		if (!GA_computeCRTCTimings(DEV.gaPtr,&modeInfo,refreshRate,interlaced,&crtc,false))
			return false;
		mode |= gaRefreshCtrl;
		}

	/* Set the display mode */
	if (!GA_setMode(dc,mode,&virtualX,&virtualY,&bytesPerLine,&maxMem,&crtc))
		return false;

	/* Now load the Nucleus reference rasteriser */
	if (!GA_loadRef2d(DEV.gaPtr,false,&modeInfo,dc->mi.pageSize * (dc->mi.maxPage+1),&DEV.ref2d))
		return false;

	/* Initialise the internals for the mode */
	dc->mi.xRes = virtualX - 1;
	dc->mi.yRes = virtualY - 1;
	dc->startX = dc->startY = 0;
	dc->mi.bytesPerLine = bytesPerLine;
	dc->mi.pageSize = virtualY * bytesPerLine;
	if (scrolling) {
		dc->mi.maxPage = (modeInfo.MaxScanLines / (dc->mi.yRes+1))-1;
		dc->virtualX = virtualX;
		dc->virtualY = virtualY;
		}
	else {
		dc->virtualX = -1;
		dc->virtualY = -1;
		}
	dc->numBuffers = numBuffers;
	dc->surface = dc->surfaceStart = DEV.gaPtr->LinearMem;
	dc->flags |= MGL_LINEAR_ACCESS;
	if (!(mode & gaLinearBuffer))
		dc->flags |= MGL_SHADOW_BUFFER;

	/* If we are running software stereo, initialise the Nucleus
	 * software stereo page flipper.
	 */
	if (isStereo & _MGL_SOFT_STEREO) {
		if (!GA_softStereoInit(DEV.gaPtr))
			return false;
		}

	/* Initialise pointers to device driver vectors */
	svga->drv.dwSize = sizeof(svga->drv);
	DEV.ref2d->QueryFunctions(GA_GET_DRIVERFUNCS,&svga->drv);
	svga->cursor.dwSize = sizeof(svga->cursor);
	DEV.ref2d->QueryFunctions(GA_GET_CURSORFUNCS,&svga->cursor);
	svga->video.dwSize = sizeof(svga->video);
	DEV.gaPtr->QueryFunctions(GA_GET_VIDEOFUNCS,0,&svga->video);
	drvState2d.dwSize = sizeof(drvState2d);
	DEV.gaPtr->QueryFunctions(GA_GET_2DSTATEFUNCS,0,&drvState2d);
	drvDraw2d.dwSize = sizeof(drvDraw2d);
	DEV.gaPtr->QueryFunctions(GA_GET_2DRENDERFUNCS,0,&drvDraw2d);

	/* Now fill in device context information specific to display DC's */
	d->refCount++;
	d->device						= _MGL_cntDevice;
	d->maxProgram					= 256;
	d->widePalette 					= (dc->mi.scratch2 & gaHave8BitDAC) != 0;
	dc->activePage      			= 0;
	dc->visualPage					= 0;
	if (dc->mi.scratch2 & gaHaveHWCursor)
		dc->v->d.hardwareCursor 	= true;
	else
		d->hardwareCursor 			= false;
	dc->v->d.setCursor 				= NUCL_setCursor;
	dc->v->d.setCursorColor 		= NUCL_setCursorColor;
	dc->v->d.setCursorPos 			= NUCL_setCursorPos;
	dc->v->d.showCursor 			= NUCL_showCursor;
	d->isStereo						= isStereo;
	d->stereoRunning				= false;
	d->refreshRate					= refreshRate;
	d->useLinearBlits				= useLinearBlits;
	d->restoreTextMode 				= NUCL_restoreTextMode;
	d->restoreGraphMode 			= NUCL_restoreGraphMode;
	d->setActivePage				= NUCL_setActivePage;
	d->setVisualPage        		= NUCL_setVisualPage;
	if (svga->drv.WaitVSync)
		d->vSync              		= NUCL_vSync;
	if (svga->drv.IsVSync)
		d->isVSync              	= NUCL_isVSync;
	if (svga->drv.GetCurrentScanLine)
		d->getCurrentScanLine   	= NUCL_getCurrentScanLine;
	d->setDisplayStart      		= NUCL_setDisplayStart;
	d->startStereo          		= NUCL_startStereo;
	d->stopStereo           		= NUCL_stopStereo;
	if (dc->mi.scratch2 & gaIsVirtualMode) {
		/* In virtual display modes we don't have direct framebuffer
		 * access, and we also never want to change the active device.
		 */
		dc->flags 					&= ~MGL_LINEAR_ACCESS;
		d->setActiveDevice			= NULL;
		}
	else
		d->setActiveDevice			= GA_setActiveDevice;
	if (svga->drv.SetGammaCorrectData) {
		dc->r.setGammaRamp			= NUCL_setGammaRamp;
		dc->r.getGammaRamp			= NUCL_getGammaRamp;
		}
	dc->r.getWinDC					= DRV_getWinDC;
	dc->r.getDefaultPalette 		= DRV_getDefaultPalette;
	dc->r.realizePalette			= NUCL_realizePalette;
	dc->r.makeCurrent				= NUCL_makeCurrent;

	/* Setup all rendering vectors for Nucleus functions */
	DRV_setNucleusRenderingVectors(dc,DEV.ref2d);

	/* Setup all non-Nucleus internal rendering functions */
	dc->r.BitBltDC					= NUCL_BitBltDC;
	dc->r.SrcTransBltDC				= NUCL_SrcTransBltDC;
	dc->r.DstTransBltDC				= NUCL_DstTransBltDC;
	dc->r.StretchBltDC				= NUCL_StretchBltDC;
	dc->r.BltBuffer					= NUCL_BltBuffer;
	dc->r.SrcTransBltBuffer			= NUCL_SrcTransBltBuffer;
	dc->r.DstTransBltBuffer			= NUCL_DstTransBltBuffer;
	dc->r.StretchBuffer				= NUCL_StretchBuffer;

	/* Setup flags for accelerated display modes */
	if (dc->mi.scratch2 & gaHaveAccel2D) {
		svga->WaitTillIdle			= drvState2d.WaitTillIdle;
		svga->EnableDirectAccess	= drvState2d.EnableDirectAccess;
		svga->DisableDirectAccess	= drvState2d.DisableDirectAccess;
		if (drvState2d.EnableDirectAccess) {
			dc->r.beginDirectAccess	= NUCL_beginDirectAccessSLOW;
			dc->r.endDirectAccess   = NUCL_endDirectAccessSLOW;
			}
		else {
			dc->r.beginDirectAccess	= NUCL_beginDirectAccess;
			}
		if (drvDraw2d.DrawRect)
			dc->flags |= MGL_HW_RECT;
		if (drvDraw2d.DrawPattRect)
			dc->flags |= MGL_HW_PATT_RECT;
		if (drvDraw2d.DrawColorPattRect)
			dc->flags |= MGL_HW_CLRPATT_RECT;
		if (drvDraw2d.DrawLineInt)
			dc->flags |= MGL_HW_LINE;
		if (drvDraw2d.DrawStippleLineInt)
			dc->flags |= MGL_HW_STIPPLE_LINE;
		if (drvDraw2d.PutMonoImageMSBSys)
			dc->flags |= MGL_HW_MONO_BLT;
		if (drvDraw2d.BitBlt)
			dc->flags |= MGL_HW_SCR_BLT;
		if (drvDraw2d.BitBltSys)
			dc->flags |= MGL_HW_SYS_BLT;
		if (drvDraw2d.SrcTransBlt)
			dc->flags |= MGL_HW_SRCTRANS_BLT;
		if (drvDraw2d.SrcTransBltSys)
			dc->flags |= MGL_HW_SRCTRANS_SYS_BLT;
		if (drvDraw2d.DstTransBlt)
			dc->flags |= MGL_HW_DSTTRANS_BLT;
		if (drvDraw2d.DstTransBltSys)
			dc->flags |= MGL_HW_DSTTRANS_SYS_BLT;
		if (drvDraw2d.StretchBlt)
			dc->flags |= MGL_HW_STRETCH_BLT;
		if (drvDraw2d.StretchBltSys)
			dc->flags |= MGL_HW_STRETCH_SYS_BLT;

		/* Configure offscreen display memory */
		if (!(dc->mi.scratch2 & gaIsVirtualMode)) {
			startLine = (dc->mi.yRes+1) * (dc->mi.maxPage+1);
			startOffset = startLine * dc->mi.bytesPerLine;
			d->createOffscreenBuffer 	= NUCL_createOffscreenBuffer;
			d->destroyOffscreenBuffer 	= NUCL_destroyOffscreenBuffer;
			dc->offscreenStartY 		= startLine;
			dc->r.restoreBuffer 		= NUCL_restoreBuffer;
			if (drvState2d.EnableDirectAccess) {
				dc->r.lockBuffer		= NUCL_lockBufferSLOW;
				dc->r.unlockBuffer 		= NUCL_unlockBufferSLOW;
				}
			else {
				dc->r.lockBuffer		= NUCL_lockBuffer;
				dc->r.unlockBuffer 		= NUCL_unlockBuffer;
				}
			if (useLinearBlits && drvDraw2d.BitBltLin) {
				DEV.linearMin = startOffset;
				DEV.linearCur = startOffset;
				DEV.linearMax = modeInfo.MaxLinearOffset;
				DEV.compacted = true;
				}
			else
				DEV.freeSpace = MGL_rgnSolidRectCoord(0,0,dc->mi.xRes+1,modeInfo.MaxScanLines-startLine);
			}
		}

	/* Cache current instance device globals to global structure */
	NUCL_updateCachedGlobals(dc);

	/* Ensure active and visual pages are set to 0 and set the software
	 * rasteriser draw buffer.
	 */
	dc->activePage = -1;
	dc->v->d.setActivePage(dc,0);
	DEV.ref2d->SetDrawBuffer(&DEV.drawBuf,DEV.gaPtr->LinearMem,modeInfo.BitsPerPixel,&modeInfo.PixelFormat,DEV.gaPtr,false);
	dc->v->d.setVisualPage(dc,0,MGL_dontWait);
	return true;
}

/****************************************************************************
RETURNS:
Number of bits in the value
****************************************************************************/
static int NumBits(
	uint value)
{
	int count = 0;
	while (value & 1) {
		count++;
		value >>= 1;
		}
	return count;
}

/****************************************************************************
RETURNS:
The aspect ratio for the mode when displayed on the monitor. We assume
all 16:9 modes are displayed with a 1:1 aspect ratio, and others are
computed for a standard 4:3 VGA monitor configuration.

Ideally we probably want to get the aspect ratio from Nucleus one day,
where it can adjust the aspect ratio for modes based on the dimensions of
the actual display device.
****************************************************************************/
static int AspectRatio(
	GA_mdEntry *md)
{
	int ratio = (md->XResolution * 1000L * 3) / (md->YResolution * 4);
	if (ratio == 1333)
		ratio = 1000;		/* Special case for 16:9 modes 			*/
	if (ratio == 562 || ratio == 421)
		ratio = 1000;		/* Special case for portrait modes		*/
	return ratio;
}

/****************************************************************************
RETURNS:
The maximum color value for the display mode.
****************************************************************************/
static color_t MaxColor(
	int bits)
{
	switch (bits) {
		case 4:		return 0xFUL;
		case 8:		return 0xFFUL;
		case 15:	return 0x7FFFUL;
		case 16:	return 0xFFFFUL;
		case 24:	return 0xFFFFFFUL;
		case 32:	return 0xFFFFFFFFUL;
		}
	return 0;
}

/****************************************************************************
PARAMETERS:
inst			- Instance data for the driver
dc				- Device context to initialise
mode			- Display mode to set
hwnd			- Handle to fullscreen console
virtualX		- Virtual X screen dimensions (-1 if not scrolling)
virtualY        - Virtual Y screen dimensions (-1 if not scrolling)
numBuffers		- Number of buffers to allocate
stereo			- True if stereo should be enabled
refreshRate		- Desired refresh rate for mode

RETURNS:
True on sucess, false on failure.

REMARKS:
This function initialise the device driver for use with the passed in
device context. We also switch the system into graphics mode when this
function is called.
****************************************************************************/
ibool MGLAPI NUCL_initDriver(
	NUCL_data *inst,
	MGLDC *dc,
	modeent *mode,
	ulong hwnd,
	int virtualX,
	int virtualY,
	int numBuffers,
	ibool stereo,
	int refreshRate,
	ibool useLinearBlits)
{
	NUCL_state			*state = &inst->state;
	display_vec			*d;
	gmode_t				*mi = &dc->mi;
	GA_mdEntry			*md;

	/* Find the mode in our internal mode list */
	for (md = inst->svga.modeList; md->mode != -1; md++) {
		if (md->XResolution == mode->xRes && md->YResolution == mode->yRes
				&& md->BitsPerPixel == mode->bits)
			break;
		}
	if (md->mode == -1)
		return false;

	/* Fill in the device context mode information block */
	mi->xRes					= md->XResolution-1;
	mi->yRes					= md->YResolution-1;
	mi->bitsPerPixel			= md->BitsPerPixel;
	mi->maxColor				= MaxColor(md->BitsPerPixel);
	mi->maxPage 				= md->MaxBuffers-1;
	mi->bytesPerLine 			= md->BytesPerScanLine;
	mi->aspectRatio				= AspectRatio(md);
	mi->pageSize 				= md->YResolution * md->BytesPerScanLine;
	mi->scratch1 				= md->mode;
	mi->scratch2 				= md->Attributes;
	if (md->Attributes & gaHaveLinearBuffer)
		mi->scratch1 			|= gaLinearBuffer;
	mi->modeFlags				= mode->flags;
	mi->bitmapStartAlign 		= md->BitmapStartAlign;
	mi->bitmapStridePad 		= md->BitmapStridePad;

	/* Copy DirectColor mask information to mi block */
	if (md->BitsPerPixel <= 8) {
		mi->redMaskSize 		= 8;
		mi->redFieldPosition 	= 16;
		mi->greenMaskSize 		= 8;
		mi->greenFieldPosition 	= 8;
		mi->blueMaskSize 		= 8;
		mi->blueFieldPosition 	= 0;
		mi->rsvdMaskSize 		= 0;
		mi->rsvdFieldPosition	= 0;
		}
	else {
		mi->redMaskSize = NumBits(md->PixelFormat.RedMask);
		mi->redFieldPosition = md->PixelFormat.RedPosition;
		mi->greenMaskSize = NumBits(md->PixelFormat.GreenMask);
		mi->greenFieldPosition = md->PixelFormat.GreenPosition;
		mi->blueMaskSize = NumBits(md->PixelFormat.BlueMask);
		mi->blueFieldPosition = md->PixelFormat.BluePosition;
		mi->rsvdMaskSize = NumBits(md->PixelFormat.RsvdMask);
		mi->rsvdFieldPosition = md->PixelFormat.RsvdPosition;
		}

	/* Fill in remaing device context information */
	dc->deviceType 			= MGL_DISPLAY_DEVICE;
	dc->xInch 				= 9000;
	dc->yInch 				= 7000;
	_MGL_computePF(dc);
	dc->v = (drv_vec*)state;
	d = &dc->v->d;
	d->destroy				= NUCL_destroyDC;
	d->hwnd 				= hwnd;

	/* Set the video mode and return status */
	return NUCL_setMode(dc,dc->mi.scratch1,&state->oldMode,virtualX,virtualY,
		numBuffers,stereo,refreshRate,useLinearBlits);
}

/****************************************************************************
PARAMETERS:
dc			- Device context to initialise

REMARKS:
Restore the previous display mode active when the graphics mode was
started.
****************************************************************************/
void MGLAPI NUCL_restoreTextMode(
	MGLDC *dc)
{
	N_int32		virtualX = -1,virtualY = -1,bpl = -1,maxMem;
	NUCL_data	*inst = (NUCL_data*)dc->v;
	NUCL_state	*state = &inst->state;

	/* Switch the active display device for Multi-Monitor support */
	_MGL_cntDevice = dc->v->d.device;
	GA_setActiveDevice(_MGL_cntDevice);

	/* Restore text mode */
	GA_setMode(dc,state->oldMode,&virtualX,&virtualY,&bpl,&maxMem,NULL);
	PM_setOSCursorLocation(0,0);

	/* Unload the graphics driver */
	GA_unloadRef2d(DEV.gaPtr);
	DEV.ref2d = NULL;
	DEV.hModRef2d = NULL;

	/* Free the offscreen memory space region */
	if (DEV.freeSpace) {
		MGL_freeRegion(DEV.freeSpace);
		DEV.freeSpace = NULL;
		}
}

/****************************************************************************
REMARKS:
Restore graphics mode again after resetting to text mode. This is used
by Windows to reset the display mode after coming back from an Alt-Tab
event.
****************************************************************************/
ibool MGLAPI NUCL_restoreGraphMode(
	MGLDC *dc)
{
	N_uint32	oldMode;

	if (!NUCL_setMode(dc,dc->mi.scratch1,&oldMode,dc->virtualX,dc->virtualY,
			dc->numBuffers,dc->v->d.isStereo != _MGL_NO_STEREO,
			dc->v->d.refreshRate,dc->v->d.useLinearBlits))
		return false;
	if (dc->v->d.stereoRunning)
		NUCL_startStereo(dc);
	return true;
}

/****************************************************************************
REMARKS:
Make the new device context the currently active device context for
rendering, which includes updating any necessary hardware state that
is cached in the hardware.
****************************************************************************/
void MGLAPI NUCL_makeCurrent(
	MGLDC *dc,
	ibool partial)
{
	/* Check if the active device is being changed */
	if (dc->v->d.device != (_MGL_cntDevice & ~MM_MODE_MIXED)) {
		int mixedMode = (_MGL_cntDevice & MM_MODE_MIXED);

		/* Switch the active display device for Multi-Monitor support */
		_MGL_cntDevice = dc->v->d.device | mixedMode;
		if (!mixedMode)
			GA_setActiveDevice(_MGL_cntDevice);

		/* Cache instance variable for device to global variables */
		NUCL_updateCachedGlobals(dc);
		}
	dc->r.SetDrawBuffer(&DEV.drawBuf);
	DRV_makeCurrent(dc,partial);
}

/****************************************************************************
REMARKS:
Enables free running stereo mode. We also re-program the blue color color
index to the correct value in here, so that the developer can enable and
disable support for blue codes at runtime with calls to MGL_setBlueCodeIndex
and MGL_startStereo.
****************************************************************************/
void MGLAPI NUCL_startStereo(
	MGLDC *dc)
{
	GA_driverFuncs *drv = &SVGA(dc).drv;

	if (dc->v->d.isStereo != _MGL_NO_STEREO) {
		MAKE_ACTIVE_DEVICE(dc);
		if (dc->v->d.isStereo & _MGL_STEREO_BLUECODE) {
			/* Program the blue code palette entry to pure blue */
			MGL_setPaletteEntry(dc,_MGL_blueCodeIndex,0,0,255);
			MGL_realizePalette(dc,256,0,false);
			}
		if (dc->v->d.isStereo & _MGL_HW_STEREO) {
			/* Enable hardware stereo page flipping */
			drv->EnableStereoMode(true);
			}
		if (dc->v->d.isStereo & _MGL_SOFT_STEREO) {
			/* Enable software page flip interrupt handler */
			GA_softStereoOn();
			}
		dc->v->d.stereoRunning = true;
		MGL_setVisualPage(dc,dc->visualPage,MGL_dontWait);
		RESTORE_ACTIVE_DEVICE();
		}
}

/****************************************************************************
REMARKS:
Disables free running stereo mode, so the display controller always
displays from the left eye image.
****************************************************************************/
void MGLAPI NUCL_stopStereo(
	MGLDC *dc)
{
	GA_driverFuncs *drv = &SVGA(dc).drv;

	if (dc->v->d.isStereo != _MGL_NO_STEREO && dc->v->d.stereoRunning) {
		MAKE_ACTIVE_DEVICE(dc);
		if (dc->v->d.isStereo & _MGL_HW_STEREO) {
			/* Disable hardware stereo page flipping */
			drv->EnableStereoMode(false);
			}
		if (dc->v->d.isStereo & _MGL_SOFT_STEREO) {
			/* Disable software page flip interrupt handler */
			GA_softStereoOff();
			}
		dc->v->d.stereoRunning = false;
		RESTORE_ACTIVE_DEVICE();
		}
}

/****************************************************************************
PARAMETERS:
dc		- Device context
page	- Active page number

REMARKS:
Sets the current active page for the device context to the specified page
value.
****************************************************************************/
void MGLAPI NUCL_setActivePage(
	MGLDC *dc,
	int page)
{
	display_vec	*d = &dc->v->d;
	int 		ipage = (page & ~MGL_RIGHT_BUFFER);
	GA_buffer	drawBuf;

	if (dc->activePage == page)
		return;
	if (ipage >= 0 && ipage <= dc->mi.maxPage) {
		dc->activePage = page;
		if (d->isStereo != _MGL_NO_STEREO) {
			if (page & MGL_RIGHT_BUFFER)
				ipage = ((page & ~MGL_RIGHT_BUFFER) * 2) + 1;
			else
				ipage *= 2;
			}
		dc->activeStartY = ipage * (dc->mi.yRes+1);
		dc->offscreenY = dc->offscreenStartY - dc->activeStartY;
		drawBuf.dwSize = sizeof(GA_buffer);
		drawBuf.Offset = dc->mi.pageSize * ipage;
		drawBuf.Stride = dc->mi.bytesPerLine;
		drawBuf.Width = dc->mi.xRes+1;
		drawBuf.Height = dc->mi.yRes+1;
		DC_DEV(dc).drawBuf = drawBuf;
		dc->surface = dc->surfaceStart + drawBuf.Offset;
		dc->originOffset = (ulong)dc->surface;
		dc->r.SetDrawBuffer(&drawBuf);
		}
}

/****************************************************************************
PARAMETERS:
dc		- Device context
page	- Active page number

REMARKS:
Sets the current visual page for the device context to the specified page
value.
****************************************************************************/
void MGLAPI NUCL_setVisualPage(
	MGLDC *dc,
	int page,
	int waitVRT)
{
	int				oldActivePage;
	color_t			blueCodeColor;
	GA_driverFuncs	*drv = &SVGA(dc).drv;

	if (page >= 0 && page <= dc->mi.maxPage) {
		dc->visualPage = page;
		if (dc->v->d.isStereo & _MGL_STEREO_BLUECODE) {
			MGLDC *olddc = MGL_makeCurrentDC(dc);
			/* Get the color for the blue code */
			if (dc->mi.maxColor > 255)
				blueCodeColor = MGL_packColor(&dc->pf,0,0,255);
			else
				blueCodeColor = _MGL_blueCodeIndex;

			/* Save the old color and active page for later */
			oldActivePage = dc->activePage;

// TODO: We should optimise this blue code drawing to avoid buffer switching
//		 by drawing the blue codes with fast rep_stosd commands direct to
// 	 	 the framebuffer. MGL_setActivePage is a bit expensive because
//       of the calls to Nucleus to change the draw buffer (not that
//		 expensive, but it could be optimised).
//
//		 Also note that this requires a context flush to the global DC for
//		 each call as well, which can be expensive!
//
//		 Maybe a good way around this is to add a new device driver function
//		 to specifically draw the blue codes independent of the current
//		 drawing context??

			/* Draw the left image blue code (25% of width) */
// TODO: This is broken!!!
			MGL_setActivePage(dc,page);
			dc->r.SetForeColor(blueCodeColor);
			dc->r.solid.DrawRect(dc->mi.yRes,0,dc->mi.xRes/4,1);
			dc->r.SetForeColor(0);
			dc->r.solid.DrawRect(dc->mi.yRes,dc->mi.xRes/4,(dc->mi.xRes*3)/4,1);

			/* Draw the right image blue code (75% of width) */
			MGL_setActivePage(dc,page | MGL_RIGHT_BUFFER);
			dc->r.SetForeColor(blueCodeColor);
			dc->r.solid.DrawRect(dc->mi.yRes,0,(dc->mi.xRes*3)/4,1);
			dc->r.SetForeColor(0);
			dc->r.solid.DrawRect(dc->mi.yRes,(dc->mi.xRes*3)/4,dc->mi.xRes/4,1);

			/* Restore the old color and active page */
			dc->r.SetForeColor(dc->a.color);
			MGL_setActivePage(dc,oldActivePage);
			MGL_makeCurrentDC(olddc);
			}

		/* Disable triple buffering if only two pages */
		MAKE_ACTIVE_DEVICE(dc);
		if (dc->mi.maxPage == 1 && waitVRT == MGL_tripleBuffer)
			waitVRT = MGL_waitVRT;

		/* Double base page index if running in stereo mode */
		if (dc->v->d.isStereo != _MGL_NO_STEREO)
			page *= 2;

		/* Wait until the previous flip has occured if doing triple buffering */
		if (waitVRT == MGL_tripleBuffer) {
			if (dc->v->d.isStereo & _MGL_SOFT_STEREO) {
				if (dc->v->d.stereoRunning)
					GA_softStereoWaitTillFlipped();
				}
			else if (dc->mi.modeFlags & MGL_HAVE_TRIPLEBUFFER) {
				while (drv->GetDisplayStartStatus() == 0)
					;
				}
			}
		if (dc->virtualX != -1) {
			/* Virtual scrolling is active, so use set display start */
			dc->CRTCBase = (dc->mi.pageSize * page);
			dc->v->d.setDisplayStart(dc,dc->startX,dc->startY,waitVRT);
			}
		else {
			/* Program the display start address */
			ulong addr = dc->mi.pageSize * page;
			if (dc->v->d.isStereo & _MGL_HW_STEREO)
				drv->SetStereoDisplayStart(addr,addr+dc->mi.pageSize,(waitVRT == MGL_waitVRT));
			else if (dc->v->d.isStereo & _MGL_SOFT_STEREO) {
				GA_softStereoScheduleFlip(addr,addr+dc->mi.pageSize);
				if (dc->v->d.stereoRunning) {
					if (waitVRT == MGL_waitVRT)
						GA_softStereoWaitTillFlipped();
					}
				else
					drv->SetDisplayStart(addr,(waitVRT == MGL_waitVRT));
				}
			else
				drv->SetDisplayStart(addr,(waitVRT == MGL_waitVRT));
			}
		RESTORE_ACTIVE_DEVICE();
		}
}

/****************************************************************************
PARAMETERS:
dc			- Device context
x,y			- Display start coordinates
waitFlag	- Flag whether to wait for retrace.

REMARKS:
Sets the current display start address for the screen. The value of waitFlag
can be one of the following:

	-1	- Set coordinates but dont change hardware
	0	- Set coordinates and hardware; no wait for retrace
	1	- Set coordinates and hardware; wait for retrace

Passing a waitFlag of -1 is used to implement double buffering and virtual
scrolling at the same time. You call this function first to set the display
start X and Y coordinates, then you call SVGA_setVisualPage() to swap
display pages and the new start address takes hold with that call.
****************************************************************************/
void MGLAPI NUCL_setDisplayStart(
	MGLDC *dc,
	int x,
	int y,
	int waitFlag)
{
	ulong 			start;
	GA_driverFuncs	*drv = &SVGA(dc).drv;

	if (dc->virtualX != -1) {
		/* Update current display start coordinates */
		dc->startX = x;
		dc->startY = y;
		if (waitFlag == -1)
			return;

		/* Compute starting address for display mode */
		MAKE_ACTIVE_DEVICE(dc);
		switch (dc->mi.bitsPerPixel) {
			case 8:
				start = dc->CRTCBase + dc->mi.bytesPerLine * y + x;
				break;
			case 15:
			case 16:
				start = dc->CRTCBase + dc->mi.bytesPerLine * y + x * 2;
				break;
			case 24:
				start = dc->CRTCBase + dc->mi.bytesPerLine * y + x * 3;
				break;
			default: /* case 32: */
				start = dc->CRTCBase + dc->mi.bytesPerLine * y + x * 4;
				break;
			}
		drv->SetDisplayStart(start,(waitFlag == MGL_waitVRT));
		RESTORE_ACTIVE_DEVICE();
		}
}

/****************************************************************************
PARAMETERS:
dc		- Device context
pal		- Palette of values to program
num		- Number of entries to program
index	- Index to start programming at
waitVRT	- True to wait for the vertical retrace

REMARKS:
Program the hardware gamma ramp.
****************************************************************************/
void MGLAPI NUCL_setGammaRamp(
	MGLDC *dc,
	palette_ext_t *pal,
	int num,
	int index,
	int waitVRT)
{
	GA_driverFuncs *drv = &SVGA(dc).drv;

	MAKE_ACTIVE_DEVICE(dc);
	drv->SetGammaCorrectData((GA_palette*)pal,num,index,waitVRT);
	RESTORE_ACTIVE_DEVICE();
}

/****************************************************************************
PARAMETERS:
dc		- Device context
pal		- Palette of values to program
num		- Number of entries to program
index	- Index to start programming at
waitVRT	- True to wait for the vertical retrace

REMARKS:
Read the hardware gamma ramp.
****************************************************************************/
void MGLAPI NUCL_getGammaRamp(
	MGLDC *dc,
	palette_ext_t *pal,
	int num,
	int index)
{
	GA_driverFuncs *drv = &SVGA(dc).drv;

	MAKE_ACTIVE_DEVICE(dc);
	drv->GetGammaCorrectData((GA_palette*)pal,num,index);
	RESTORE_ACTIVE_DEVICE();
}

/****************************************************************************
PARAMETERS:
dc		- Device context
pal		- Palette of values to program
num		- Number of entries to program
index	- Index to start programming at
waitVRT	- True to wait for the vertical retrace

REMARKS:
Program the hardware palette.
****************************************************************************/
void MGLAPI NUCL_realizePalette(
	MGLDC *dc,
	palette_t *pal,
	int num,
	int index,
	int waitVRT)
{
	int				count;
	uchar			*p = (uchar*)&pal[index];
	int				maxProg = dc->v->d.maxProgram;
	GA_driverFuncs	*drv = &SVGA(dc).drv;

	/* Determine if we need to wait for the vertical retrace */
	MAKE_ACTIVE_DEVICE(dc);
	if (!waitVRT)
		count = num;
	else
		count = (num > maxProg) ? maxProg : num;
	while (num) {
		drv->SetPaletteData((GA_palette*)p,count,index,waitVRT);
		index += count;
		p += count*4;
		num -= count;
		count = (num > maxProg) ? maxProg : num;
		}
	RESTORE_ACTIVE_DEVICE();
}

/****************************************************************************
PARAMETERS:
dc		- Device context

REMARKS:
Syncs to a vertical interrupt.
****************************************************************************/
void MGLAPI NUCL_vSync(
	MGLDC *dc)
{
	GA_driverFuncs *drv = &SVGA(dc).drv;

	MAKE_ACTIVE_DEVICE(dc);
	drv->WaitVSync();
	RESTORE_ACTIVE_DEVICE();
}

/****************************************************************************
PARAMETERS:
dc		- Device context

RETURNS:
True if the vertical retrace is active, false if not.
****************************************************************************/
int MGLAPI NUCL_isVSync(
	MGLDC *dc)
{
	int 			vSync;
	GA_driverFuncs	*drv = &SVGA(dc).drv;

	MAKE_ACTIVE_DEVICE(dc);
	vSync = drv->IsVSync();
	RESTORE_ACTIVE_DEVICE();
	return vSync;
}

/****************************************************************************
PARAMETERS:
dc		- Device context

RETURNS:
Current scanline counter returned from the hardware.
****************************************************************************/
int	MGLAPI NUCL_getCurrentScanLine(
	MGLDC *dc)
{
	int 			line;
	GA_driverFuncs	*drv = &SVGA(dc).drv;

	MAKE_ACTIVE_DEVICE(dc);
	line = drv->GetCurrentScanLine();
	RESTORE_ACTIVE_DEVICE();
	return line;
}

/****************************************************************************
PARAMETERS:
dc		- Device context
curs	- Cursor image to download

REMARKS:
This function downloads the new cursor image to the hardware. Note that
the MGL cursor images are 32x32, while Nucleus supports 64x64 cursors so
we have to convert the format of the image data before downloading it.
****************************************************************************/
void MGLAPI NUCL_setCursor(
	MGLDC *dc,
	cursor_t *curs)
{
	GA_cursorFuncs *cursor = &SVGA(dc).cursor;

	MAKE_ACTIVE_DEVICE(dc);
	cursor->SetMonoCursor((GA_monoCursor*)curs->m.xorMask);
	RESTORE_ACTIVE_DEVICE();
}

/****************************************************************************
PARAMETERS:
dc		- Device context
x,y		- New position for the mouse cursor

REMARKS:
This function moves the hardware cursor to the new location.
****************************************************************************/
void MGLAPI NUCL_setCursorPos(
	MGLDC *dc,
	int x,
	int y)
{
	GA_cursorFuncs *cursor = &SVGA(dc).cursor;

	MAKE_ACTIVE_DEVICE(dc);
	cursor->SetCursorPos(x,y);
	RESTORE_ACTIVE_DEVICE();
}

/****************************************************************************
PARAMETERS:
dc		- Device context
visible	- Flag to show or hide the cursor

REMARKS:
This function displays or hides the hardware mouse cursor.
****************************************************************************/
void MGLAPI NUCL_showCursor(
	MGLDC *dc,
	ibool visible)
{
	GA_cursorFuncs *cursor = &SVGA(dc).cursor;

	MAKE_ACTIVE_DEVICE(dc);
	cursor->ShowCursor(visible);
	RESTORE_ACTIVE_DEVICE();
}

/****************************************************************************
PARAMETERS:
dc			- Device context
cursorColor	- Foreground color for the cursor

REMARKS:
This routine sets the cursor color as a color index or as the RGB values
depending on the display mode.
****************************************************************************/
void MGLAPI NUCL_setCursorColor(
	MGLDC *dc,
	color_t cursorColor)
{
	GA_palette  	fg,bg = {0,0,0,0};
	GA_cursorFuncs 	*cursor = &SVGA(dc).cursor;

	if (dc->mi.bitsPerPixel == 8) {
		if (dc->mi.scratch2 & gaHave8bppRGBCursor) {
			palette_t *pal = (palette_t*)&dc->colorTab[cursorColor];
			fg.Red = pal->red;
			fg.Green = pal->green;
			fg.Blue =pal->blue;
			}
		else
			fg.Red = cursorColor;
		}
	else {
		MGL_unpackColorFast(&dc->pf,cursorColor,fg.Red,fg.Green,fg.Blue);
		}
	MAKE_ACTIVE_DEVICE(dc);
	cursor->SetMonoCursorColor(&fg,&bg);
	RESTORE_ACTIVE_DEVICE();
}

static int         	bufWidth;	/* Height of bitmap to allocate			*/
static int         	bufHeight;  /* Width of bitmap to allocate			*/
static ibool       	foundSpace; /* True if we found a spot              */
static rect_t		bestBounds; /* Bounds for the best fit rectangle    */
static rect_t		testRect;   /* Rectangle we are testing             */
static region_t		*rgn;		/* Region represting the allocated mem  */

/****************************************************************************
PARAMETERS:
r   - Rectangle to test

REMARKS:
Tries to expand the testRect rectangle vertically with all rectangles
vertically adjacent to it.
****************************************************************************/
static void MGLAPI expandVert(
	const rect_t *r)
{
	if (r->top == testRect.bottom && r->left <= testRect.left
			&& r->right >= testRect.left + bufWidth) {
		testRect.bottom = r->bottom;
        }
}

/****************************************************************************
PARAMETERS:
r   - Rectangle to test

REMARKS:
Checks the test rectangle against the size of our bitmap, and if it is
larger, then we have found some free space. We also check t find the first
rectangle with the best fit for the bitmap. Note that the bestBounds
returned is the bounds of the free space that we found, and will most likely
be larger than the size of our bitmap!
****************************************************************************/
static void MGLAPI findFreeRect(
	const rect_t *r)
{
	if ((r->right - r->left) >= bufWidth) {
		testRect = *r;
		if ((testRect.bottom-testRect.top) < bufHeight) {
			/* We have found a rectangle big enough in the width to hold
			 * our buffer, so we need to search for and coallesce all
			 * rectangles below this rectangle that are also wide enough
			 * to hold our buffer.
			 */
			MGL_traverseRegion(rgn,expandVert);
            }
		if ((testRect.bottom-testRect.top) >= bufHeight) {
			/* We have found a rectangle big enough to hold our bitmap */
			if (foundSpace) {
				if ((testRect.right-testRect.left) < (bestBounds.right-bestBounds.left)
						&& (testRect.bottom-testRect.top) < (bestBounds.bottom-bestBounds.top)) {
					bestBounds = testRect;
					}
                }
            else {
                bestBounds = testRect;
                foundSpace = true;
                }
            }
        }
}

/****************************************************************************
REMARKS:
Round a value to the specified integer boundary
****************************************************************************/
static ulong RoundUp(
	ulong value,
	ulong boundary)
{
	return ((value + (boundary-1)) / boundary) * boundary;
}

/****************************************************************************
PARAMETERS:
buf			- Offscreen buffer to create
dev			- Device entry
pitch		- Place to store the new pitch for the block
linearStart	- Place to store the linear start address for the block

RETURNS:
True if the buffer can be allocated, false if not.

REMARKS:
Internal function to find a place to store a new linear block of memory.
****************************************************************************/
static ibool FindLinearMem(
	MGLBUF *buf,
	device_entry *dev,
	int *pitch,
	int *size,
	long *linearStart)
{
	*pitch = BYTESPERLINE(buf->width,buf->dc->mi.bitsPerPixel);
	*pitch = RoundUp(*pitch,buf->dc->mi.bitmapStridePad);
	*size = *pitch * buf->height;
	if ((dev->linearCur + *size) > dev->linearMax)
		return false;
	*linearStart = dev->linearCur;
	return true;
}

/****************************************************************************
PARAMETERS:
buf		- Offscreen buffer to create

RETURNS:
True if the buffer was allocated, false if not.

REMARKS:
Internal function to allocate a linear memory bitmap.
****************************************************************************/
static ibool AllocateLinearMem(
	MGLBUF *buf)
{
	MGLDC			*dc = buf->dc;
	device_entry	*dev = &DC_DEV(dc);
	int 			size;

	/* Allocate the buffer in offscreen memory */
	if (!FindLinearMem(buf,dev,&buf->bytesPerLine,&size,&buf->linearStart))
		return false;
	buf->surface = dc->surfaceStart + buf->linearStart;
	buf->startX = -1;
	buf->startY = -1;

	/* Now align the start address for the next bitmap to the
	 * boundary required by the hardware.
	 */
	dev->linearCur = RoundUp(dev->linearCur+size,dc->mi.bitmapStartAlign);
	return true;
}

/****************************************************************************
PARAMETERS:
buf		- Offscreen buffer to create

RETURNS:
True if the buffer was allocated, false if not.

REMARKS:
Creates a buffer in offscreen display memory. We support both rectangular
and linear offscreen buffers.
****************************************************************************/
ibool MGLAPI NUCL_createOffscreenBuffer(
	MGLBUF *buf)
{
	MGLDC			*dc = buf->dc;
	device_entry	*dev = &DC_DEV(dc);
	region_t		*freeSpace = dev->freeSpace;
	rect_t			r;
	MGLBUF			*b;
	int				pitch,size;
	long			linearStart;
	GA_buffer		drawBuf = {sizeof(GA_buffer)};

	if (freeSpace) {
		/* Allocate the buffer from the rectangular heap. To do this we
		 * traverse the region of empty space (always traverses left to
		 * right, top to bottom) looking for the best fit rectangle for our
		 * buffer. If we find some free space, allocate the space and remove
		 * it from the freespace region.
		 */
		bufWidth = buf->width;
		bufHeight = buf->height;
		foundSpace = false;
		MGL_traverseRegion(rgn = freeSpace,findFreeRect);
		if (!foundSpace)
			return false;
		buf->startX = bestBounds.left;
		buf->startY = bestBounds.top;
		buf->bytesPerLine = dc->mi.bytesPerLine;
		buf->surface = PIXEL_ADDR(buf->startX,buf->startY+dc->offscreenStartY,dc->surfaceStart,dc->mi.bytesPerLine,dc->mi.bitsPerPixel);
		buf->linearStart = -1;
		r.left = buf->startX;
		r.right = r.left + buf->width;
		r.top = buf->startY;
		r.bottom = r.top + buf->height;
		MGL_diffRegionRect(freeSpace,&r);
		MGL_optimizeRegion(freeSpace);
		return true;
		}
	else {
		/* Allocate the buffer in linear memory. If we run out of memory,
		 * we try compacting the heap on the fly to make room for any
		 * new bitmaps.
		 */
		if (!AllocateLinearMem(buf)) {
			/* We are out of memory if the heap is already compacted */
			if (dev->compacted)
				return false;

			/* Try compacting the heap. Note that it is important that
			 * we process the bitmaps in LIFO order, so that we will
			 * always be moving the bitmaps *DOWN* in memory during
			 * compaction. Hence all bitmaps on our list are added to the
			 * end of the list, so we can efficiently traverse it.
			 */
			for (b = _LST_first(dev->offBufList); b; b = _LST_next(b)) {
				/* Ignore system memory only bitmaps */
				if (b->surface == b->surfaceCache)
					continue;

				/* Find new location for bitmap */
				if (!FindLinearMem(b,dev,&pitch,&size,&linearStart))
					MGL_fatalError("Unable to re-allocate linear memory!");

				/* If the bitmap has moved, blit it to the new location */
				if (b->linearStart != linearStart) {
					drawBuf.Offset = linearStart;
					drawBuf.Stride = pitch;
					drawBuf.Width = b->width;
					drawBuf.Height = b->height;
					dc->r.SetDrawBuffer(&drawBuf);
					dc->r.BitBltLin(linearStart,pitch,0,0,b->width,b->height,0,0,GA_REPLACE_MIX);
					}

				/* Now reallocate the bitmap at the new location */
				if (!AllocateLinearMem(b))
					MGL_fatalError("Unable to re-allocate linear memory!");
				}
			dev->compacted = true;

			/* Restore the current hardware draw buffer */
			dc->r.SetDrawBuffer(&DEV.drawBuf);
			return AllocateLinearMem(buf);
			}
		return true;
		}
}

/****************************************************************************
PARAMETERS:
buf		- Offscreen buffer to destroy

REMARKS:
Destroys an offscreen buffer, freeing up the memory used by the buffer.
If compactHeap is set to true, we compact the heap to free up any
free memory blocks.
****************************************************************************/
void MGLAPI NUCL_destroyOffscreenBuffer(
	MGLBUF *buf)
{
	rect_t			r;
	MGLDC			*dc = buf->dc;
	device_entry	*dev = &DC_DEV(dc);
	region_t		*freeSpace = dev->freeSpace;
	int				size;
	long			nextStart;

	/* We only need to do anything if the offscreen buffer is rectanular.
	 * For the linear heap, we compact the heap when we try to allocate
	 * a bitmap and it fails.
	 */
	if (freeSpace) {
		/* Free the region from the rectangular heap */
		r.left = buf->startX;
		r.right = buf->startX + buf->width;
		r.top = buf->startY;
		r.bottom = buf->startY + buf->height;
		MGL_unionRegionRect(freeSpace,&r);
		MGL_optimizeRegion(freeSpace);
		}
	else {
		size = buf->bytesPerLine * buf->height;
		nextStart = RoundUp(buf->linearStart + size,dc->mi.bitmapStartAlign);
		if (nextStart == dev->linearCur) {
			/* Bitmap is the last one on the heap, so simply decrement
			 * the linear cursor.
			 */
			dev->linearCur = buf->linearStart;
			}
		else {
			/* Bitmap is somewhere in the middle of the heap, so we need
			 * to do a heap compaction at some later date.
			 */
			dev->compacted = false;
			}
		}
}

/****************************************************************************
REMARKS:
Restore the contents of an offscreen buffer from the buffer cache.
****************************************************************************/
void MGLAPI NUCL_restoreBuffer(
	MGLBUF *buf)
{
	int 	height = buf->height,widthBytes;
	uchar 	*src,*dst;

	/* Now copy the bitmap information */
	src = buf->surfaceCache;
	dst = buf->surface;
	widthBytes = BYTESPERLINE(buf->width,buf->dc->mi.bitsPerPixel);
	NUCL_lockBuffer(buf);
	while (height--) {
		MGL_memcpy(dst,src,widthBytes);
		src += widthBytes;
		dst += buf->bytesPerLine;
		}
	NUCL_unlockBuffer(buf);
}

/****************************************************************************
REMARKS:
Enable direct access to the an offscreen buffer
****************************************************************************/
void MGLAPI NUCL_lockBuffer(
	MGLBUF *buf)
{
	(void)buf;
	_MGL_gaWaitTillIdle();
}

/****************************************************************************
REMARKS:
Disable direct access to an offscreen buffer
****************************************************************************/
void MGLAPI NUCL_unlockBuffer(
	MGLBUF *buf)
{
	(void)buf;
}

/****************************************************************************
REMARKS:
Enable direct access to the an offscreen buffer
****************************************************************************/
void MGLAPI NUCL_lockBufferSLOW(
	MGLBUF *buf)
{
	(void)buf;
	if (++_MGL_surfLock == 0)
		_MGL_gaEnableDirectAccess();
}

/****************************************************************************
REMARKS:
Disable direct access to an offscreen buffer
****************************************************************************/
void MGLAPI NUCL_unlockBufferSLOW(
	MGLBUF *buf)
{
	(void)buf;
	if (--_MGL_surfLock == -1)
		_MGL_gaDisableDirectAccess();
}

/****************************************************************************
REMARKS:
Enable direct access to the framebuffer
****************************************************************************/
void MGLAPI NUCL_beginDirectAccess(void)
{
	_MGL_gaWaitTillIdle();
}

/****************************************************************************
REMARKS:
Enable direct access to the framebuffer
****************************************************************************/
void MGLAPI NUCL_beginDirectAccessSLOW(void)
{
	if (++_MGL_surfLock == 0)
		_MGL_gaEnableDirectAccess();
}

/****************************************************************************
REMARKS:
Disable direct access to the framebuffer
****************************************************************************/
void MGLAPI NUCL_endDirectAccessSLOW(void)
{
	if (--_MGL_surfLock == -1)
		_MGL_gaDisableDirectAccess();
}

/****************************************************************************
REMARKS:
Blits the contents of display DC's or offscreen DC's between each other.
****************************************************************************/
void MGLAPI NUCL_BitBltDC(
	MGLDC *src,
	MGLDC *dst,
	N_int32 srcLeft,
	N_int32 srcTop,
	N_int32 width,
	N_int32 height,
	N_int32 dstLeft,
	N_int32 dstTop,
	N_int32 mix)
{
	int			adjustX,adjustY;
	MGLBUF		*buf;
	GA_buffer	drawBuf;

	if (src->deviceType == MGL_OFFSCREEN_DEVICE && dst->deviceType == MGL_DISPLAY_DEVICE) {
		/* OffscreenDC -> DisplayDC */
		buf = src->offBuf;
		if (buf->linearStart == -1) {
			dst->r.BitBlt(srcLeft+buf->startX,srcTop+dst->offscreenY+buf->startY,
				width,height,dstLeft,dstTop,mix);
			}
		else {
			dst->r.BitBltLin(buf->linearStart,buf->bytesPerLine,
				srcLeft,srcTop,width,height,dstLeft,dstTop,mix);
			}
		}
	else if (src->deviceType == MGL_DISPLAY_DEVICE && dst->deviceType == MGL_OFFSCREEN_DEVICE) {
		/* DisplayDC -> OffscreenDC */
		buf = dst->offBuf;
		if (buf->linearStart == -1) {
			dst->r.BitBlt(srcLeft,srcTop,width,height,dstLeft+buf->startX,
				dstTop+src->offscreenY+buf->startY,mix);
			}
		else {
			MAKE_GA_BUF(drawBuf,buf);
			src->r.SetDrawBuffer(&drawBuf);
			src->r.BitBltLin(buf->linearStart,buf->bytesPerLine,
				srcLeft,srcTop,width,height,dstLeft,dstTop,mix);
			}
		}
	else if (src->deviceType == MGL_DISPLAY_DEVICE && dst->deviceType == MGL_DISPLAY_DEVICE) {
		/* DisplayDC -> DisplayDC */
		adjustY = src->activeStartY - dst->activeStartY;
		dst->r.BitBlt(srcLeft,srcTop+adjustY,width,height,dstLeft,
			dstTop,mix);
		}
	else if (src->deviceType == MGL_OFFSCREEN_DEVICE && dst->deviceType == MGL_OFFSCREEN_DEVICE) {
		/* OffscreenDC -> OffscreenDC */
		buf = src->offBuf;
		if (buf->linearStart == -1) {
			adjustX = buf->startX - dst->offBuf->startX;
			adjustY = buf->startY - dst->offBuf->startY + buf->dc->offscreenY;
			dst->r.BitBlt(srcLeft+adjustX,srcTop+adjustY,
				width,height,dstLeft,dstTop,mix);
			}
		else {
			dst->r.BitBltLin(buf->linearStart,buf->bytesPerLine,
				srcLeft,srcTop,width,height,dstLeft,dstTop,mix);
			}
		}
}

/****************************************************************************
REMARKS:
Blits the contents of display DC's or offscreen DC's between each other.
****************************************************************************/
void MGLAPI NUCL_SrcTransBltDC(
	MGLDC *src,
	MGLDC *dst,
	N_int32 srcLeft,
	N_int32 srcTop,
	N_int32 width,
	N_int32 height,
	N_int32 dstLeft,
	N_int32 dstTop,
	N_int32 mix,
	GA_color transparent)
{
	int			adjustX,adjustY;
	MGLBUF		*buf;
	GA_buffer	drawBuf;

	if (src->deviceType == MGL_OFFSCREEN_DEVICE && dst->deviceType == MGL_DISPLAY_DEVICE) {
		/* OffscreenDC -> DisplayDC */
		buf = src->offBuf;
		if (buf->linearStart == -1) {
			dst->r.SrcTransBlt(srcLeft+buf->startX,srcTop+dst->offscreenY+buf->startY,
				width,height,dstLeft,dstTop,mix,transparent);
			}
		else {
			dst->r.SrcTransBltLin(buf->linearStart,buf->bytesPerLine,
				srcLeft,srcTop,width,height,dstLeft,dstTop,mix,transparent);
			}
		}
	else if (src->deviceType == MGL_DISPLAY_DEVICE && dst->deviceType == MGL_OFFSCREEN_DEVICE) {
		/* DisplayDC -> OffscreenDC */
		buf = dst->offBuf;
		if (buf->linearStart == -1) {
			dst->r.SrcTransBlt(srcLeft,srcTop,width,height,dstLeft+buf->startX,
				dstTop+src->offscreenY+buf->startY,mix,transparent);
			}
		else {
			MAKE_GA_BUF(drawBuf,buf);
			src->r.SetDrawBuffer(&drawBuf);
			src->r.SrcTransBltLin(buf->linearStart,buf->bytesPerLine,
				srcLeft,srcTop,width,height,dstLeft,dstTop,mix,transparent);
			}
		}
	else if (src->deviceType == MGL_DISPLAY_DEVICE && dst->deviceType == MGL_DISPLAY_DEVICE) {
		/* DisplayDC -> DisplayDC */
		adjustY = src->activeStartY - dst->activeStartY;
		dst->r.SrcTransBlt(srcLeft,srcTop+adjustY,width,height,dstLeft,
			dstTop,mix,transparent);
		}
	else if (src->deviceType == MGL_OFFSCREEN_DEVICE && dst->deviceType == MGL_OFFSCREEN_DEVICE) {
		/* OffscreenDC -> OffscreenDC */
		buf = src->offBuf;
		if (buf->linearStart == -1) {
			adjustX = buf->startX - dst->offBuf->startX;
			adjustY = buf->startY - dst->offBuf->startY + buf->dc->offscreenY;
			dst->r.SrcTransBlt(srcLeft+adjustX,srcTop+adjustY,
				width,height,dstLeft,dstTop,mix,transparent);
			}
		else {
			dst->r.SrcTransBltLin(buf->linearStart,buf->bytesPerLine,
				srcLeft,srcTop,width,height,dstLeft,dstTop,mix,transparent);
			}
		}
}

/****************************************************************************
REMARKS:
Blits the contents of display DC's or offscreen DC's between each other.
****************************************************************************/
void MGLAPI NUCL_DstTransBltDC(
	MGLDC *src,
	MGLDC *dst,
	N_int32 srcLeft,
	N_int32 srcTop,
	N_int32 width,
	N_int32 height,
	N_int32 dstLeft,
	N_int32 dstTop,
	N_int32 mix,
	GA_color transparent)
{
	int			adjustX,adjustY;
	MGLBUF		*buf;
	GA_buffer	drawBuf;

	if (src->deviceType == MGL_OFFSCREEN_DEVICE && dst->deviceType == MGL_DISPLAY_DEVICE) {
		/* OffscreenDC -> DisplayDC */
		buf = src->offBuf;
		if (buf->linearStart == -1) {
			dst->r.DstTransBlt(srcLeft+buf->startX,srcTop+dst->offscreenY+buf->startY,
				width,height,dstLeft,dstTop,mix,transparent);
			}
		else {
			dst->r.DstTransBltLin(buf->linearStart,buf->bytesPerLine,
				srcLeft,srcTop,width,height,dstLeft,dstTop,mix,transparent);
			}
		}
	else if (src->deviceType == MGL_DISPLAY_DEVICE && dst->deviceType == MGL_OFFSCREEN_DEVICE) {
		/* DisplayDC -> OffscreenDC */
		buf = dst->offBuf;
		if (buf->linearStart == -1) {
			dst->r.DstTransBlt(srcLeft,srcTop,width,height,dstLeft+buf->startX,
				dstTop+src->offscreenY+buf->startY,mix,transparent);
			}
		else {
			MAKE_GA_BUF(drawBuf,buf);
			src->r.SetDrawBuffer(&drawBuf);
			src->r.DstTransBltLin(buf->linearStart,buf->bytesPerLine,
				srcLeft,srcTop,width,height,dstLeft,dstTop,mix,transparent);
			}
		}
	else if (src->deviceType == MGL_DISPLAY_DEVICE && dst->deviceType == MGL_DISPLAY_DEVICE) {
		/* DisplayDC -> DisplayDC */
		adjustY = src->activeStartY - dst->activeStartY;
		dst->r.DstTransBlt(srcLeft,srcTop+adjustY,width,height,dstLeft,
			dstTop,mix,transparent);
		}
	else if (src->deviceType == MGL_OFFSCREEN_DEVICE && dst->deviceType == MGL_OFFSCREEN_DEVICE) {
		/* OffscreenDC -> OffscreenDC */
		buf = src->offBuf;
		if (buf->linearStart == -1) {
			adjustX = buf->startX - dst->offBuf->startX;
			adjustY = buf->startY - dst->offBuf->startY + buf->dc->offscreenY;
			dst->r.DstTransBlt(srcLeft+adjustX,srcTop+adjustY,
				width,height,dstLeft,dstTop,mix,transparent);
			}
		else {
			dst->r.DstTransBltLin(buf->linearStart,buf->bytesPerLine,
				srcLeft,srcTop,width,height,dstLeft,dstTop,mix,transparent);
			}
		}
}

/****************************************************************************
REMARKS:
Blits the contents of display DC's or offscreen DC's between each other.
****************************************************************************/
void MGLAPI NUCL_StretchBltDC(
	MGLDC *src,
	MGLDC *dst,
	N_int32 srcLeft,
	N_int32 srcTop,
	N_int32 srcWidth,
	N_int32 srcHeight,
	N_int32 dstLeft,
	N_int32 dstTop,
	N_int32 dstWidth,
	N_int32 dstHeight,
	N_int32 doClip,
	N_int32 clipLeft,
	N_int32 clipTop,
	N_int32 clipRight,
	N_int32 clipBottom,
	N_int32 mix)
{
	int			adjustX,adjustY;
	MGLBUF		*buf;
	GA_buffer	drawBuf;

	if (src->deviceType == MGL_OFFSCREEN_DEVICE && dst->deviceType == MGL_DISPLAY_DEVICE) {
		/* OffscreenDC -> DisplayDC */
		buf = src->offBuf;
		if (buf->linearStart == -1) {
			dst->r.StretchBlt(srcLeft+buf->startX,srcTop+dst->offscreenY+buf->startY,
				srcWidth,srcHeight,dstLeft,dstTop,dstWidth,dstHeight,
				doClip,clipLeft,clipTop,clipRight,clipBottom,mix);
			}
		else {
			dst->r.StretchBltLin(buf->linearStart,buf->bytesPerLine,
				srcLeft,srcTop,srcWidth,srcHeight,
				dstLeft,dstTop,dstWidth,dstHeight,
				doClip,clipLeft,clipTop,clipRight,clipBottom,mix);
			}
		}
	else if (src->deviceType == MGL_DISPLAY_DEVICE && dst->deviceType == MGL_OFFSCREEN_DEVICE) {
		/* DisplayDC -> OffscreenDC */
		buf = dst->offBuf;
		if (buf->linearStart == -1) {
			dst->r.StretchBlt(srcLeft,srcTop,srcWidth,srcHeight,
				dstLeft+buf->startX,dstTop+src->offscreenY+buf->startY,
				dstWidth,dstHeight,doClip,
				clipLeft,clipTop,clipRight,clipBottom,mix);
			}
		else {
			MAKE_GA_BUF(drawBuf,buf);
			src->r.SetDrawBuffer(&drawBuf);
			src->r.StretchBltLin(buf->linearStart,buf->bytesPerLine,
				srcLeft,srcTop,srcWidth,srcHeight,
				dstLeft,dstTop,dstWidth,dstHeight,
				doClip,clipLeft,clipTop,clipRight,clipBottom,mix);
			}
		}
	else if (src->deviceType == MGL_DISPLAY_DEVICE && dst->deviceType == MGL_DISPLAY_DEVICE) {
		/* DisplayDC -> DisplayDC */
		adjustY = src->activeStartY - dst->activeStartY;
		dst->r.StretchBlt(srcLeft,srcTop+adjustY,srcWidth,srcHeight,
			dstLeft,dstTop,dstWidth,dstHeight,doClip,
			clipLeft,clipTop,clipRight,clipBottom,mix);
		}
	else if (src->deviceType == MGL_OFFSCREEN_DEVICE && dst->deviceType == MGL_OFFSCREEN_DEVICE) {
		/* OffscreenDC -> OffscreenDC */
		buf = src->offBuf;
		if (buf->linearStart == -1) {
			adjustX = buf->startX - dst->offBuf->startX;
			adjustY = buf->startY - dst->offBuf->startY + buf->dc->offscreenY;
			dst->r.StretchBlt(srcLeft+adjustX,srcTop+adjustY,
				srcWidth,srcHeight,dstLeft,dstTop,dstWidth,dstHeight,doClip,
				clipLeft,clipTop,clipRight,clipBottom,mix);
			}
		else {
			dst->r.StretchBltLin(buf->linearStart,buf->bytesPerLine,
				srcLeft,srcTop,srcWidth,srcHeight,
				dstLeft,dstTop,dstWidth,dstHeight,
				doClip,clipLeft,clipTop,clipRight,clipBottom,mix);
			}
		}
}

/****************************************************************************
REMARKS:
Blits the contents of an offscreen buffer to a DC.
****************************************************************************/
void MGLAPI NUCL_BltBuffer(
	MGLBUF *src,
	MGLDC *dst,
	N_int32 srcLeft,
	N_int32 srcTop,
	N_int32 width,
	N_int32 height,
	N_int32 dstLeft,
	N_int32 dstTop,
	N_int32 mix)
{
	dst->r.BitBlt(srcLeft+src->startX,srcTop+dst->offscreenY+src->startY,
		width,height,dstLeft,dstTop,mix);
}

/****************************************************************************
REMARKS:
Blits the contents of an offscreen buffer to a DC.
****************************************************************************/
void MGLAPI NUCL_SrcTransBltBuffer(
	MGLBUF *src,
	MGLDC *dst,
	N_int32 srcLeft,
	N_int32 srcTop,
	N_int32 width,
	N_int32 height,
	N_int32 dstLeft,
	N_int32 dstTop,
	N_int32 mix,
	GA_color transparent)
{
	dst->r.SrcTransBlt(srcLeft+src->startX,srcTop+dst->offscreenY+src->startY,
		width,height,dstLeft,dstTop,mix,transparent);
}

/****************************************************************************
REMARKS:
Blits the contents of an offscreen buffer to a DC.
****************************************************************************/
void MGLAPI NUCL_DstTransBltBuffer(
	MGLBUF *src,
	MGLDC *dst,
	N_int32 srcLeft,
	N_int32 srcTop,
	N_int32 width,
	N_int32 height,
	N_int32 dstLeft,
	N_int32 dstTop,
	N_int32 mix,
	GA_color transparent)
{
	dst->r.DstTransBlt(srcLeft+src->startX,srcTop+dst->offscreenY+src->startY,
		width,height,dstLeft,dstTop,mix,transparent);
}

/****************************************************************************
REMARKS:
Blits the contents of an offscreen buffer to a DC.
****************************************************************************/
void MGLAPI NUCL_StretchBuffer(
	MGLBUF *src,
	MGLDC *dst,
	N_int32 srcLeft,
	N_int32 srcTop,
	N_int32 srcWidth,
	N_int32 srcHeight,
	N_int32 dstLeft,
	N_int32 dstTop,
	N_int32 dstWidth,
	N_int32 dstHeight,
	N_int32 doClip,
	N_int32 clipLeft,
	N_int32 clipTop,
	N_int32 clipRight,
	N_int32 clipBottom,
	N_int32 mix)
{
	dst->r.StretchBlt(srcLeft+src->startX,srcTop+dst->offscreenY+src->startY,
		srcWidth,srcHeight,dstLeft,dstTop,dstWidth,dstHeight,doClip,
		clipLeft,clipTop,clipRight,clipBottom,mix);
}
