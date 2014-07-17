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
* Description:	Device context creation code.
*
****************************************************************************/

#include "mgl.h"

/*--------------------------- Global Variables ----------------------------*/

static MGL_suspend_cb_t	_MGL_suspendApp = NULL;
ibool					_MGL_inSuspendApp = false;

/*------------------------- Implementation --------------------------------*/

/****************************************************************************
PARAMETERS:
dc			- Device context to fill in
driver		- Pointer to device driver driver to use
mode		- Pointer to mode entry for driver (NULL if not needed)
hwnd		- Handle to OS fullscreen window
virtualX	- Virtual X coordinate for display DC
virutalY	- Virtual Y coordinate for display DC
numBufffers	- Number of display buffers requested
stereo		- True if stereo should be enabled
refreshRate	- Refresh rate to set for the mode

RETURNS:
True on success, false on failure.

REMARKS:
Attempts to initialize the low level device driver. If this is a display
device driver, it will start the specified graphics mode. If this is
a memory device driver, it will allocate the memory for the display surface.
First we allocate all resources for the device driver, and then intialise it
to fill in the device context vector tables.
{secret}
****************************************************************************/
ibool _MGL_initDC(
	MGLDC *dc,
	driverent *driver,
	modeent *mode,
	MGL_HWND hwnd,
	int virtualX,
	int virtualY,
	int numBuffers,
	ibool stereo,
	int refreshRate)
{
	/* Check to ensure that the driver was actually found. If the driver
	 * was not registered, then we need to catch this.
     */
	__MGL_result = grOK;
	if (!driver->driver) {
		LOGFATAL("Driver not registered!");
		return false;
		}

	/* Clear out some important variables */
	dc->flags = 0;

	/* Initialise the driver and set the graphics mode */
	if (driver->driver->initDriver(driver->data,dc,mode,(ulong)hwnd,virtualX,
			virtualY,numBuffers,stereo,refreshRate,__MGL_useLinearBlits)) {
		/* Initialise internal dimensions */
		dc->size.left = 0;
		dc->size.top = 0;
		dc->size.right = dc->mi.xRes+1;
		dc->size.bottom = dc->mi.yRes+1;

		/* Allocate color lookup table cache - make sure it is cleared */
		if ((dc->colorTab = MGL_calloc(1,sizeof(color_t) * 256)) == NULL) {
			FATALERROR(grNoMem);
			return __MGL_result;
			}

		/* Initialise the default palette */
		if (dc->deviceType != MGL_MEMORY_DEVICE) {
			MGL_setDefaultPalette(dc);
			MGL_realizePalette(dc,MGL_getPaletteSize(dc),0,false);
			}
		MGL_defaultAttributes(dc);
		}
	return __MGL_result == grOK;
}

/****************************************************************************
PARAMETERS:
mode	- New display mode to make active

RETURNS:
True if the mode is available, false if mode is invalid.

REMARKS:
This function changes the current fullscreen display mode used by MGL.
This function is called whenever the fullscreen display mode is changed
for a device context.
****************************************************************************/
static ibool _MGL_changeDisplayMode(
	int mode)
{
	/* Check if the mode is currently available */
	__MGL_result = grOK;
	if (mode >= DEV.numModes || DEV.availableModes[mode].driver == 0xFF) {
		SETERROR(grInvalidMode);
		return false;
		}

	/* Uncache the current DC */
	MGL_makeCurrentDC(NULL);

	/* Destroy all active offscreen contexts for this device */
	while (DEV.offDCList)
		_MGL_destroyOffscreenDC(_LST_first(DEV.offDCList));

	/* Destroy all active offscreen buffers for this device */
	while (DEV.offBufList)
		MGL_destroyBuffer(_LST_first(DEV.offBufList));

	/* Destroy all active display contexts, which restores text mode */
	while (DEV.dispDCList)
		_MGL_destroyDisplayDC(_LST_first(DEV.dispDCList));

	/* Free the offscreen memory space region */
	if (DEV.freeSpace) {
		MGL_freeRegion(DEV.freeSpace);
		DEV.freeSpace = NULL;
		}

	return true;
}

/****************************************************************************
REMARKS:
Does the hard work to create the actual display device context.
****************************************************************************/
static MGLDC * _MGL_createDisplayDC(
	int mode,
	int virtualX,
	int virtualY,
	int numBuffers,
    ibool stereo,
	int refreshRate)
{
	MGLDC		*dc;
	driverent	*entry;
	modeent		*me = &DEV.availableModes[mode];
	PM_HWND		hwndConsole = 0;

	/* Switch display modes */
	if (!_MGL_changeDisplayMode(mode))
		return NULL;

	/* Allocate memory for the new DC */
	if ((dc = _LST_newNode(sizeof(MGLDC))) == NULL) {
		FATALERROR(grNoMem);
		return NULL;
		}

	/* The display device list is currently empty, so create the new
	 * list and start the specified graphics mode
	 */
	if ((DEV.dispDCList = _LST_create()) == NULL) {
		FATALERROR(grNoMem);
		goto Error;
		}

	/* Open a new fullscreen console for the mode, and save it's state */
	hwndConsole = PM_openConsole((PM_HWND)NULL,_MGL_cntDevice,me->xRes,me->yRes,me->bits,true);
	if ((DEV.stateBuf = MGL_malloc(PM_getConsoleStateSize())) == NULL) {
		FATALERROR(grNoMem);
		goto Error;
		}
	PM_saveConsoleState(DEV.stateBuf,hwndConsole);

	/* Now intialise the driver */
	entry = &DEV.driverTable[me->driver];
	if (!_MGL_initDC(dc,entry,me,(MGL_HWND)hwndConsole,virtualX,virtualY,numBuffers,stereo,refreshRate))
		goto Error;

	/* Set the suspend application callback for the console */
	PM_setSuspendAppCallback(_MGL_suspendAppProc);

	/* Set up the mouse cursor */
	_MS_setDisplayDC(dc);
	MS_setCursor(MGL_DEF_CURSOR);
	MS_moveTo(dc->mi.xRes/2,dc->mi.yRes/2);

	/* Now clear the device page */
	MGL_makeCurrentDC(dc);
	MGL_clearDevice();

	/* And clear the event queue of any extraneous events */
	EVT_flush(EVT_EVERYEVT);
	_MGL_disableAutoPlay();

	/* Add the new DC to the start of the DC chain */
	RESET_DEFAULT_CW();
	_LST_addToHead(DEV.dispDCList,dc);
	return dc;

Error:
	if (hwndConsole) {
		if (DEV.stateBuf) {
			PM_restoreConsoleState(DEV.stateBuf,hwndConsole);
			MGL_free(DEV.stateBuf);
			DEV.stateBuf = NULL;
			}
		PM_closeConsole(hwndConsole);
		}
	DEV.fullScreen = false;
	if (DEV.dispDCList && DEV.dispDCList->count == 0) {
		_LST_destroy(DEV.dispDCList,_LST_freeNode);
		DEV.dispDCList = NULL;
		}
	_LST_freeNode(dc);
	return NULL;
}

/****************************************************************************
DESCRIPTION:
Create a new hardware scrolling display device context.

HEADER:
mgraph.h

PARAMETERS:
mode		- Graphics mode to initialise
virtualX	- Virtual width of desired mode
virtualY	- Virtual height of desired mode
numBuffers	- Number of buffers for multibuffering
refreshRate	- Requested refresh rate for the graphics mode

RETURNS:
Pointer to the newly created hardware scrolling display device context,
NULL on failure.

REMARKS:
Creates a new scrolling display device context for drawing information
directly to the hardware display device in fullscreen graphics modes.
Essentially this function is identical to MGL_createDisplayDC, however
hardware scrolling (or panning) is supported. Some hardware devices may
not support hardware scrolling, in which case this function will fail and
return a NULL. In these cases you should provide an alternative method of
scrolling the display, such as drawing to a memory device context and
copying the appropriate portion of the image to the display with MGL_bitBlt.

When the device context is created, the MGL will start the graphics mode
specified in the mode parameter and initialize the specific device driver.
If any prior display device contexts exist, they will all be destroyed before
switching to the new display mode.

Once you have created a hardware scrolling device context, the display
starting coordinate will be set to (0,0) within the virtual image. To
hardware pan around within the virtual image, you can use the
MGL_setDisplayStart function to change the display starting x and y
coordinates.

The refresh rate value that you pass in is a /suggested/ value in that
the MGL will attempt to set the refresh rate to this value, however if the
hardware does not support that refresh rate the next lowest available refresh
rate will be used instead. In some situations where no refresh rate control
is available, the value will be ignored and the adapter default refresh rate
will be used. If you dont care about the refresh rate and want to use the
adapter default setting, pass in a value of MGL_DEFAULT_REFRESH.

Note:	To set an interlaced refresh rate, pass in the refresh rate as a
		negative value. Ie: Pass a value of -87 for 87Hz interlaced.

SEE ALSO:
MGL_createMemoryDC, MGL_createDisplayDC, MGL_createStereoDisplayDC,
MGL_cloneDisplayDC, MGL_destroyDC
****************************************************************************/
MGLDC * MGLAPI MGL_createScrollingDC(
	int mode,
	int virtualX,
	int virtualY,
	int numBuffers,
	int refreshRate)
{
	return _MGL_createDisplayDC(mode,virtualX,virtualY,numBuffers,false,refreshRate);
}

/****************************************************************************
DESCRIPTION:
Create a new display device context.

HEADER:
mgraph.h

PARAMETERS:
mode		- Graphics mode to initialise
numBuffers	- Number of buffers to allocate for double/multi-buffering.
refreshRate	- Requested refresh rate for the graphics mode

RETURNS:
Pointer to the newly created display device context, NULL on failure

REMARKS:
Creates a new display device context for drawing information directly to
the hardware display device in fullscreen graphics modes. When the device
context is created, the MGL will start the graphics mode specified in the
mode parameter and initialize the specific device driver. If any prior
display device contexts exist, they will all be destroyed before switching
to the new display mode.

If you intend to use double or multi-buffered graphics using the display
device, you should set the numBuffers to the number of buffers that you
require, so that the device will be properly configured for multi-buffered
operation. If you request more buffers than is currently available, this
function will fail. Hence you should first call MGL_availablePages to
determine how many buffers can be used in the desired graphics mode.

The refresh rate value that you pass in is a /suggested/ value in that
the MGL will attempt to set the refresh rate to this value, however if the
hardware does not support that refresh rate the next lowest available refresh
rate will be used instead. In some situations where no refresh rate control
is available, the value will be ignored and the adapter default refresh rate
will be used. If you dont care about the refresh rate and want to use the
adapter default setting, pass in a value of MGL_DEFAULT_REFRESH.

Once the display device context has been allocated, the surface pointer
of the MGLDC structure can be used to directly access the surface of
the device context as a linear block of memory. The dimensions and pixel
format of the device context surface are stored in the gmode_t field of the
MGLDC structure, and you should use these values to write your own direct
rendering code.

Note that all device contexts have an associated color palette, even RGB
device contexts. In RGB modes the color palette is used for converting color
index pixel values to RGB values during BitBlt operations and with the
MGL_realColor and MGL_setColorCI function.

Note:	To set an interlaced refresh rate, pass in the refresh rate as a
		negative value. Ie: Pass a value of -87 for 87Hz interlaced.

SEE ALSO:
MGL_createMemoryDC, MGL_createScrollingDisplayDC, MGL_createStereoDisplayDC,
MGL_cloneDisplayDC, MGL_destroyDC
****************************************************************************/
MGLDC * MGLAPI MGL_createDisplayDC(
	int mode,
	int numBuffers,
	int refreshRate)
{
	return _MGL_createDisplayDC(mode,-1,-1,numBuffers,false,refreshRate);
}

/****************************************************************************
DESCRIPTION:
Create a new display device context for stereo LC shutter glasses

HEADER:
mgraph.h

PARAMETERS:
mode		- Graphics mode to initialise
numBuffers	- Number of buffers to allocate for double/multi-buffering.
refreshRate	- Requested refresh rate for the graphics mode

RETURNS:
Pointer to the newly created display device context, NULL on failure.

REMARKS:
Creates a new display device context for drawing information directly to the
hardware display device in fullscreen graphics modes. Essentially this
function is identical to MGL_createDisplayDC, however support for LC shutter
glasses is provided and the MGL will take care of automatically flipping
between the left and right images to create the stereo display. In some
cases we may not be able to initialise support for LC shutter glasses, so
this function will fail.

When the device context is created, the MGL will start the graphics mode
specified in the mode parameter and initialize the specific device driver.
If any prior display device contexts exist, they will all be destroyed before
switching to the new display mode.

When running in stereo mode, the MGL actually allocates twice the number of
buffers that you request for drawing images, since we need one buffer for the
left eye image and another buffer for the right eye image (ie: if you request
two stereo buffers for double buffering, the MGL will actually allocate room
for four). The reason for this is that when displaying one of the stereo
buffers, the MGL will automatically /swap/ between the left and right eye
images at every vertical retrace. It also sends a signal to the LC shutter
glasses to tell them to block out the image for the eye that should not be
seeing the image on the screen (ie: when the left image is being displayed,
the shutter over the right eye will be blacked out). Hence by drawing images
with slightly different viewing parameters (ie: as viewed from the left or
right eye when doing 3D rendering), the user sees a single image with
complete with visual depth cues!

When running in stereo mode, you have to tell the MGL which buffer you want
to draw to when drawing the left or right eye images. Just like you normally
do in double and multi-buffering, you use the MGL_setActivePage function to
tell the MGL the active display page you wish to draw to. However in stereo
modes you must also pass in the MGL_LEFT_BUFFER or MGL_RIGHT_BUFFER values
to tell the MGL which eye you are drawing for. For instance to draw to
stereo page 1, left eye you would use MGL_setActivePage(1 | MGL_LEFT_BUFFER),
and for the right eye you would use MGL_setActivePage(1 | MGL_RIGHT_BUFFER).

Note:	In OpenGL rendering modes, changing the draw buffer is done with the
		OpenGL glDrawBuffer(GL_BACK_LEFT) and glDrawBuffer(GL_BACK_RIGHT)
		functions instead of using MGL_setActivePage.

One of the biggest drawbacks to viewing stereo images using LC shutter
glasses is that the refresh rate viewed in each eye is exactly half that
of the refresh rate of the display mode. Hence if running in a display mode
with a 60Hz refresh rate, the user will experience an overall refresh rate
of 30Hz per eye! As you can image this can be extremely tiresome for
extended viewing periods, so to get around this the MGL allows you to pass
in a value to request a higher refresh rate for the mode. Ideally you want
to try and use a refresh rate that is twice the desired refresh rate per
eye, such as 120Hz for viewing images at 60Hz, however you /must/ allow
the user to override or suggest a desired refresh rate as many older
monitors may not be capable of displaying an image at a high refresh rate
like 120Hz.

The refresh rate value that you pass in is a /suggested/ value in that
the MGL will attempt to set the refresh rate to this value, however if the
hardware does not support that refresh rate the next lowest available refresh
rate will be used instead. In some situations where no refresh rate control
is available, the value will be ignored and the adapter default refresh rate
will be used. If you dont care about the refresh rate and want to use the
adapter default setting, pass in a value of MGL_DEFAULT_REFRESH.

Note:	In the USA and Canada, the main power frequency runs at 60Hz, and all
		fluorescent lights will be illuminating your room at frequency of
		60Hz. If you use a refresh rate that is not a multiple of the mains
		frequency and you are viewing the image in a room with fluorescent
		lights, you may experience severe /beating/ at a frequency that is
		the difference between the monitor refresh rate and the fluorescent
		light frequency (ie: at 100Hz you will experience a 20Hz annoying
		beat frequency). In order to get around this problem, always try to
		use a frequency that is double the mains frequency such as 120Hz
		to avoid the beating, or have the user turn off their fluorescent
		lights!

When you create a stereo display device context, the MGL does not
automatically start stereo page flipping, and you must start this with a
call to MGL_startStereo. You can also turn stereo mode on an off at any
time (ie: you can turn it off when you go to your menu system) using the
MGL_stopStereo and MGL_startStereo functions. Note that when stereo mode is
disabled, the MGL always displays from the left eye buffer.

SEE ALSO:
MGL_createDisplayDC, MGL_destroyDC, MGL_changeDisplayMode, MGL_startStereo,
MGL_stopStereo, MGL_setBlueCodeIndex
****************************************************************************/
MGLDC * MGLAPI MGL_createStereoDisplayDC(
	int mode,
	int numBuffers,
	int refreshRate)
{
	return _MGL_createDisplayDC(mode,-1,-1,numBuffers,true,refreshRate);
}

/****************************************************************************
DESCRIPTION:
Create a new windowed device context.

HEADER:
mglwin.h

PARAMETERS:
hwnd	- Window handle with which to associate new device context

RETURNS:
Pointer to the allocated windowed device context, or NULL if not enough memory.

REMARKS:
Creates a new windowed device context for drawing information into a window
on the Windows desktop. When you create a Windowed device context, you
associate it with a standard Windows HWND for the window that you wish MGL
to display it's output on. Windowed device contexts are special device
contexts in that you cannot directly access the surface for the device, nor
can you actually use the MGL rasterizing functions to draw on the device
surface. The only rasterizing functions supported are the MGL_bitBlt and
MGL_stretchBlt for blt'ing data from memory device contexts to the window on
the desktop.

However in order to change the color palette values for the data copied to
the window, you must use the MGL palette functions on the windowed display
device context. Note that MGL automatically takes care of creating a proper
Windows identity palette for the windowed device context, so as long as you
program the same palette values for the windowed device and the memory
device you should get the maximum performance blt'ing speed.

SEE ALSO:
MGL_createMemoryDC, MGL_createDisplayDC, MGL_destroyDC, MGL_setWinDC,
MGL_activatePalette
****************************************************************************/
MGLDC * MGLAPI MGL_createWindowedDC(
	MGL_HWND hwnd)
{
	MGLDC		*dc;
	driverent 	*driver;

	/* Switch display modes */
	if (!_MGL_changeDisplayMode(-1))
		return NULL;

	/* Allocate memory for the new DC */
	if ((dc = _LST_newNode(sizeof(MGLDC))) == NULL) {
		FATALERROR(grNoMem);
		return NULL;
		}

	/* The display device list is currently empty, so create the new
	 * list and start the specified graphics mode
	 */
	if (_MGL_winDCList == NULL) {
		if ((_MGL_winDCList = _LST_create()) == NULL) {
			FATALERROR(grNoMem);
			goto Error;
			}
		}

	/* Find the device driver to use */
#ifdef	__WINDOWS32__
	if ((driver = _MGL_findStaticDriver(MGL_WINDOWEDNAME)) == NULL) {
		FATALERROR(grDriverNotFound);
		goto Error;
		}
#else
	driver = NULL;
#endif
	if (!driver) {
		FATALERROR(grDriverNotFound);
		goto Error;
		}

	/* Store the mode information and initialise the device context */
	if (!driver->data) {
		if ((driver->data = driver->driver->createInstance()) == NULL) {
			FATALERROR(grLoadMem);
			return NULL;
			}
		}
	if (!_MGL_initDC(dc,driver,NULL,hwnd,-1,-1,1,false,MGL_DEFAULT_REFRESH))
		goto Error;
	RESET_DEFAULT_CW();

	/* Add the new DC to the start of the DC chain */
	_LST_addAfter(_MGL_winDCList,dc,LST_HEAD(_MGL_winDCList));
	return dc;

Error:
	if (_MGL_winDCList && _MGL_winDCList->count == 0) {
		_LST_destroy(_MGL_winDCList,_LST_freeNode);
		_MGL_winDCList = NULL;
		}
	_LST_freeNode(dc);
	return NULL;
}

/****************************************************************************
DESCRIPTION:
Clone an existing display device context.

HEADER:
mgraph.h

PARAMETERS:
dc		- MGL display device context to clone

RETURNS:
Pointer to the newly created display device context, NULL on failure

REMARKS:
This function is used to clone an existing display device context, allowing
you to have multiple device contexts with completely sepatare rendering
states connected to the same display device. When you switch the active
display device context with MGL_makeCurrentDC, the hardware state is updated
to reflect the state stored in the new device context.

One of the most useful uses for this function, is to create a second
display device context that can be setup with with a different active
drawing page. Then you can use the MGL_bitBlt function to copy screen
images from one display page to another (which you can't do with a single
device context).

If a display device context has not yet been created, this function will
fail.

SEE ALSO:
MGL_createMemoryDC, MGL_createScrollingDisplayDC, MGL_createStereoDisplayDC,
MGL_createDisplayDC, MGL_destroyDC
****************************************************************************/
MGLDC * MGLAPI MGL_cloneDisplayDC(
	MGLDC *dc)
{
	MGLDC	*cloneDC;

	/* Check that the DC is a valid display DC to clone */
	__MGL_result = grOK;
	CHECK(dc != NULL);
	if (dc->deviceType != MGL_DISPLAY_DEVICE) {
		FATALERROR(grInvalidDC);
		return NULL;
		}

	/* Allocate memory for the new DC */
	if ((cloneDC = _LST_newNode(sizeof(MGLDC))) == NULL) {
		FATALERROR(grNoMem);
		return NULL;
		}

	/* Now clone the display DC */
	*cloneDC = *dc;
	cloneDC->v->d.refCount++;
	cloneDC->size.left = 0;
	cloneDC->size.top = 0;
	cloneDC->size.right = cloneDC->mi.xRes+1;
	cloneDC->size.bottom = cloneDC->mi.yRes+1;
	cloneDC->a.clipRegion = NULL;
	cloneDC->intClipRegion = NULL;
	if ((cloneDC->colorTab = MGL_calloc(1,sizeof(color_t) * 256)) == NULL) {
		FATALERROR(grNoMem);
		goto Error;
		}

	/* Reset all the attributes to defaults */
	MGL_defaultAttributes(cloneDC);
	MGL_setDefaultPalette(cloneDC);

	/* Add the new DC to the start of the DC chain */
	_LST_addToHead(DEV.dispDCList,cloneDC);
	return cloneDC;

Error:
	_LST_freeNode(cloneDC);
	return NULL;
}

/****************************************************************************
DESCRIPTION:
Creates a new offscreen display device context.

HEADER:
mgraph.h

PARAMETERS:
dc		- MGL display or windowed device context to create offscreen DC from
width	- Width of the offscreen device context to create
height	- Height of the offscreen device context to create

RETURNS:
Pointer to the newly created offscreen device context, NULL if not valid.

REMARKS:
Creates a new offscreen display device context for rendering to offscreen
video memory when running in hardware accelerated video modes. You must
have already created a valid display device context before this function is
called, otherwise this function will return NULL. Also if the display device
does not support hardware accelerated offscreen display memory, this
function will also return NULL. Finally if there is no more available
offscreen display memory to fullfill the request, this function will fail.
See the MGL_result error code for more information if the function failed.

An offscreen device context can be used just like any other MGL device
context, so you can copy data around with MGL_bitBlt, or draw on the
device context using any of the MGL rendering functions. If you simply
want to store bitmaps in offscreen memory for fast blitting, you should
instead create the bitmaps as MGL buffers with the MGL_createBuffer function.
MGL buffers are similar to offscreen device contexts, except they can only
be used for blitting and cannot be used as a rendering target. MGL buffers
also use a lot less system memory resources than a full offscreen DC.

SEE ALSO:
MGL_createMemoryDC, MGL_createScrollingDisplayDC, MGL_createStereoDisplayDC,
MGL_createDisplayDC, MGL_destroyDC, MGL_createBuffer
****************************************************************************/
MGLDC * MGLAPI MGL_createOffscreenDC(
	MGLDC *dc,
	int width,
	int height)
{
	MGLDC	*offDC;
	MGLBUF	*buf;

	/* Create an offscreen buffer for the DC */
	if ((buf = MGL_createBuffer(dc,width,height,-2)) == NULL)
		return NULL;

	/* Now allocate memory for the device context */
	if ((offDC = _LST_newNode(sizeof(MGLDC))) == NULL) {
		FATALERROR(grNoMem);
		return NULL;
		}
	if (DEV.offDCList == NULL) {
		if ((DEV.offDCList = _LST_create()) == NULL) {
			FATALERROR(grNoMem);
			goto Error;
			}
		}

	/* Clone the first display DC, but allocate a new color table */
	*offDC = *dc;
	offDC->v->d.refCount++;
	offDC->deviceType = MGL_OFFSCREEN_DEVICE;
	offDC->mi.xRes = buf->width-1;
	offDC->mi.yRes = buf->height-1;
	offDC->mi.maxPage = 0;
	offDC->mi.bytesPerLine = buf->bytesPerLine;
	offDC->surface = buf->surface;
	offDC->offBuf = buf;
	offDC->a.clipRegion = NULL;
	offDC->intClipRegion = NULL;
	if ((offDC->colorTab = MGL_malloc(sizeof(color_t) * 256)) == NULL) {
		FATALERROR(grNoMem);
		goto Error;
		}
	MGL_defaultAttributes(offDC);
	MGL_setDefaultPalette(offDC);

	/* Add the new DC to the start of the DC chain */
	_LST_addToHead(DEV.offDCList,offDC);
	return offDC;

Error:
	if (buf)
		MGL_destroyBuffer(buf);
	if (DEV.offDCList && DEV.offDCList->count == 0) {
		_LST_destroy(DEV.offDCList,_LST_freeNode);
		DEV.offDCList = NULL;
		}
	_LST_freeNode(offDC);
	return NULL;
}

/****************************************************************************
DESCRIPTION:
Creates a new custom memory device context.

HEADER:
mgraph.h

PARAMETERS:
xSize			- X resolution for the memory context
ySize			- Y resolution for the memory context
bitsPerPixel	- Pixel depth for the memory context
pf				- Pixel format for memory context
bytesPerLine	- Buffer pitch for memory context
surface			- Pointer to surface memory for context
hbm				- Handle to HBITMAP for DIB section

RETURNS:
Pointer to the allocated memory device context.

REMARKS:
This function is useful for creating an MGL device context for a memory
block provided by an application. This allows the SciTech MGL to render to
memory it does not own (e.g. custom hardware framebuffers, or bitmaps
allocated by other runtime libraries).

For Windows if the hbm parameter is not NULL, it is assumed that the
original memory was created by a call to CreateDIBSection and the hbm
parameter is a handle to the bitmap object for the DIB section. This
parameter can be used to blit the memory DC image to a windowed DC using
the standard Windows GDI blit functions. The hbm parameter is not necessary
for fullscreen modes.

SEE ALSO:
MGL_createMemoryDC, MGL_destroyDC
****************************************************************************/
MGLDC * MGLAPI MGL_createCustomDC(
	int xSize,
	int ySize,
	int bitsPerPixel,
	pixel_format_t *pf,
	int bytesPerLine,
	void *surface,
	MGL_HBITMAP hbm)
{
	MGLDC		*dc;
	driverent 	*driver;

	/* Allocate memory for the new DC */
	if ((dc = _LST_newNode(sizeof(MGLDC))) == NULL) {
		FATALERROR(grNoMem);
		return NULL;
		}

	/* Create the memory DC list if it does not exist */
	if (_MGL_memDCList == NULL) {
		if ((_MGL_memDCList = _LST_create()) == NULL) {
			FATALERROR(grNoMem);
			goto Error;
			}
		}

	/* Find the device driver to use based on color depth */
	switch (bitsPerPixel) {
		case 8:	 	driver = _MGL_findStaticDriver(MGL_PACKED8NAME);	break;
		case 15:
		case 16: 	driver = _MGL_findStaticDriver(MGL_PACKED16NAME);	break;
		case 24: 	driver = _MGL_findStaticDriver(MGL_PACKED24NAME);	break;
		case 32: 	driver = _MGL_findStaticDriver(MGL_PACKED32NAME);	break;
		default:
			FATALERROR(grInvalidMode);
			goto Error;
		}
	if (!driver) {
		FATALERROR(grDriverNotFound);
		goto Error;
		}

	/* Store the mode information and initalise the device context	*/
	dc->size.left = 0;
	dc->size.top = 0;
	dc->size.right = xSize;
	dc->size.bottom = ySize;
	dc->mi.xRes = xSize-1;
	dc->mi.yRes = ySize-1;
	dc->mi.bitsPerPixel = bitsPerPixel;
	dc->mi.bytesPerLine = bytesPerLine;
	dc->surface = surface;
	if (pf)
		dc->pf = *pf;
	dc->ownMemory = false;
	if (!driver->data) {
		if ((driver->data = driver->driver->createInstance()) == NULL) {
			FATALERROR(grLoadMem);
			return NULL;
			}
		}
	if (!_MGL_initDC(dc,driver,NULL,NULL,-1,-1,1,false,MGL_DEFAULT_REFRESH))
		goto Error;

	/* Let the OS specific code handle selecting of the bitmap as necessary */
	_MGL_initBitmapHandle(dc,hbm);

	/* Add the new DC to the start of the DC chain */
	_LST_addToHead(_MGL_memDCList,dc);
	return dc;

Error:
	if (_MGL_memDCList && _MGL_memDCList->count == 0) {
		_LST_destroy(_MGL_memDCList,_LST_freeNode);
		_MGL_memDCList = NULL;
		}
	_LST_freeNode(dc);
	return NULL;
}

/****************************************************************************
DESCRIPTION:
Create a new memory device context.

HEADER:
mgraph.h

PARAMETERS:
xSize			- x resolution for the memory context
ySize			- y resolution for the memory context
bitsPerPixel	- Pixel depth for the memory context
pf				- Pixel format for memory context (NULL for 8 bits and below)

RETURNS:
Pointer to the allocated memory device context, NULL if not enough memory.

REMARKS:
Creates a new memory device context, allocating the necessary memory
resources to hold the surface of the memory device given the specified
resolution and pixel depth. The surface of a memory device context is
always allocated using the appropriate operating system specific functions,
and you can always directly access the surface of the device context via
the surface pointer of the MGLDC structure. If you do directly access the
surface, the dimensions and pixel format of the device context surface are
stored in the gmode_t field of the MGLDC structure, and you should use
these values to write your own direct rendering code.

For memory device contexts with pixel depths greater than 8 bits per pixel,
you must also pass a valid pixel_format_t structure which defines the pixel
format to be used for the device context. If you wish to create a memory
device context for your main rasterizing context which you then wish to Blt
to the screen, you must ensure that you use the same pixel format for the
memory device as the display device for the current graphics mode, otherwise
the pixel formats will be translated on the fly by the MGL_bitBlt function
resulting in very low performance. You can use the MGL_getPixelFormat
function to obtain the pixel format information for the display device
context you are using.

Note that all device contexts have an associated color palette, even RGB
device contexts. In RGB modes the color palette is used for converting
color index pixel values to RGB values during BitBlt operations and with
the MGL_realColor and MGL_setColorCI function.

SEE ALSO:
MGL_createCustomDC, MGL_createDisplayDC, MGL_destroyDC
****************************************************************************/
MGLDC * MGLAPI MGL_createMemoryDC(
	int xSize,
	int ySize,
	int bitsPerPixel,
	pixel_format_t *pf)
{
	return MGL_createCustomDC(xSize,ySize,bitsPerPixel,pf,0,0,NULL);
}

/****************************************************************************
PARAMETERS:
list	- List to destroy the context from
dc		- Device context to destroy

RETURNS:
True if the context was destroyed, false on error.

REMARKS:
Searches for the device context in the list of currently allocated contexts,
and destroys it if it exists. If there are now more contexts in the list,
the list itself is destroyed.
{secret}
****************************************************************************/
ibool __MGL_destroyDC(
	LIST **listHandle,
	MGLDC *dc)
{
	LIST	*list = *listHandle;
	MGLDC	*prev;

	/* Remove the DC from the list of allocated DC's */
	CHECK(dc != NULL);
	if ((prev = _LST_findNode(list,dc)) == NULL) {
		FATALERROR(grInvalidDC);
		return false;
		}
	_LST_removeNext(list,prev);

	/* Free up all memory occupied by the DC */
	if (dc->offBuf)
		MGL_destroyBuffer(dc->offBuf);
	MGL_free(dc->colorTab);
	dc->v->m.destroy(dc);
	_LST_freeNode(dc);

	/* If this is the last device context, destroy the list */
	if (list->count == 0) {
		_LST_destroy(list,_LST_freeNode);
		*listHandle = NULL;
		}
	return true;
}

/****************************************************************************
PARAMETERS:
dc	- Device context to destroy

RETURNS:
True if the context was destroyed, false on error.

REMARKS:
Searches for the display device context in the list of currently allocated
contexts, and destroys it if it exists. If this is the last display device
context, then we also shut down the graphics mode and restore the original
graphics mode.
{secret}
****************************************************************************/
ibool _MGL_destroyDisplayDC(
	MGLDC *dc)
{
	MGLDC	*prev;

	if (!dc) {
		FATALERROR(grInvalidDC);
		return false;
		}
	MAKE_ACTIVE_DEVICE(dc);
	if (DEV.dispDCList->count == 1) {
		/* There is only one active display context left, so restore
		 * the original display mode and destroy the entire display
		 * device context list
		 */
		if (dc != _LST_first(DEV.dispDCList)) {
			RESTORE_ACTIVE_DEVICE();
			FATALERROR(grInvalidDC);
			return false;
			}
		dc->v->d.restoreTextMode(dc);
		MGL_free(dc->colorTab);
		dc->v->d.destroy(dc);
		_LST_destroy(DEV.dispDCList,_LST_freeNode);
		DEV.dispDCList = NULL;

		/* Restore the console state and close it */
		PM_restoreConsoleState(DEV.stateBuf,(PM_HWND)dc->v->d.hwnd);
		MGL_free(DEV.stateBuf);
		DEV.stateBuf = NULL;
		PM_closeConsole((PM_HWND)dc->v->d.hwnd);

		/* We are no longer in Windows full screen mode but on the desktop */
		_MS_setDisplayDC(NULL);
		}
	else {
		/* There is more than one active display context, so simply
		 * remove the current one from the list and destroy it.
		 */
		if ((prev = _LST_findNode(DEV.dispDCList,dc)) == NULL) {
			RESTORE_ACTIVE_DEVICE();
			FATALERROR(grInvalidDC);
			return false;
			}
		MGL_free(dc->colorTab);
		dc->v->d.destroy(dc);
		_LST_removeNext(DEV.dispDCList,prev);
		_LST_freeNode(dc);
		}
	RESTORE_ACTIVE_DEVICE();
	return true;
}

/****************************************************************************
PARAMETERS:
dc	- Device context to destroy

RETURNS:
True if the context was destroyed, false on error.

REMARKS:
Searches for the windowed device context in the list of currently allocated
contexts, and destroys it if it exists.
****************************************************************************/
ibool _MGL_destroyWindowedDC(
	MGLDC *dc)
{
	MGLDC	*prev;

	/* Remove the DC from the list of allocated memory DC's */
	if ((prev = _LST_findNode(_MGL_winDCList,dc)) == NULL) {
		__MGL_result = grInvalidDC;
		return false;
		}
	_LST_removeNext(_MGL_winDCList,prev);

	/* Free up all memory occupied by the DC */
	MGL_free(dc->colorTab);
	dc->v->w.destroy(dc);
	_LST_freeNode(dc);
	if (_MGL_winDCList->count == 0) {
		/* This is the last windowed device context, so destroy the list */
		_LST_destroy(_MGL_winDCList,_LST_freeNode);
		_MGL_winDCList = NULL;
		}
	return true;
}

/****************************************************************************
DESCRIPTION:
Destroy a given device context.

HEADER:
mgraph.h

PARAMETERS:
dc	- Device context to be destroyed

RETURNS:
True if context was destroyed, false on error.

REMARKS:
Destroys a specified device context, freeing up all resources allocated
by the device context. This can fail for a number of reasons, so check the
MGL_result code to determine the cause of the failure.

If the device context that was destroyed was the last active display device
context, the video mode is reset back to the original video mode (or back
to the normal GDI desktop for Windows). Note that calling MGL_exit
automatically destroys all currently allocated device contexts.

SEE ALSO:
MGL_createDisplayDC, MGL_createOffscreenDC, MGL_createMemoryDC,
MGL_createWindowedDC
****************************************************************************/
ibool MGLAPI MGL_destroyDC(
	MGLDC *dc)
{
	if (!dc)
		return true;
	if (dc == _MGL_dcPtr)
		MGL_makeCurrentDC(NULL);
	switch (dc->deviceType) {
		case MGL_DISPLAY_DEVICE:
			return _MGL_destroyDisplayDC(dc);
		case MGL_OFFSCREEN_DEVICE:
			return _MGL_destroyOffscreenDC(dc);
		case MGL_OVERLAY_DEVICE:
			return _MGL_destroyOverlayDC(dc);
		case MGL_MEMORY_DEVICE:
			return _MGL_destroyMemoryDC(dc);
		case MGL_WINDOWED_DEVICE:
			return _MGL_destroyWindowedDC(dc);
		}
	FATALERROR(grInvalidDC);
	return false;
}

/****************************************************************************
PARAMETERS:
flags	- Flags indicating what to do

RETURNS:
Return code for suspend app callback.

REMARKS:
This is the PM library suspend application callback function for all MGL
routines. This function takes care of most of the important stuff such as
saving and restoring the state of the hardware etc.
{secret}
****************************************************************************/
int MGLAPI _MGL_suspendAppProc(
	int flags)
{
	int				retcode;
	MGLDC			*dc = _LST_first(DEV.dispDCList);
	static MGLDC	*prevDC;

	if (flags == PM_DEACTIVATE) {
		if (_MGL_suspendApp) {
			if ((retcode = _MGL_suspendApp(dc,flags)) == MGL_NO_DEACTIVATE)
				return PM_NO_DEACTIVATE;
			}
		else
			retcode = PM_SUSPEND_APP;
		_MGL_inSuspendApp = true;
		// TODO: Hide mouse cursor
		prevDC = _MGL_dcPtr;
		if (_MGL_isOpenGLDevice(dc))
			MGL_glMakeCurrent(NULL);
		else
			MGL_makeCurrentDC(NULL);
		dc->v->d.restoreTextMode(dc);
		_MGL_restoreAutoPlay();
		return retcode;
		}
	if (flags == PM_REACTIVATE) {
		int oldActivePage = MGL_getActivePage(dc);
		int oldVisualPage = MGL_getVisualPage(dc);
		dc->v->d.restoreGraphMode(dc);
		MGL_makeCurrentDC(dc);
		if (_MGL_isOpenGLDevice(dc))
			MGL_glMakeCurrent(prevDC);
		else
			MGL_makeCurrentDC(prevDC);
		MGL_realizePalette(dc,MGL_getPaletteSize(dc),0,false);
		MGL_setActivePage(dc,oldActivePage);
		MGL_setVisualPage(dc,oldVisualPage,false);
		_MGL_inSuspendApp = false;
		if (_MGL_suspendApp)
			_MGL_suspendApp(dc,flags);
		// TODO: Restore mouse cursor
		_MGL_disableAutoPlay();

		// TODO: Need to restore all managed offscreen buffers in here!!

		return PM_SUSPEND_APP;
		}
	return PM_SUSPEND_APP;
}

/****************************************************************************
DESCRIPTION:
Sets the fullscreen suspend application callback function.

HEADER:
mgraph.h

PARAMETERS:
saveState	- New suspend app callback to be used.

REMARKS:
This function is used to register an application suspend callback function.
This is used in fullscreen modes under Windows and is called by MGL when the
application's fullscreen window has lost the input focus and the system has
returned to the normal GUI desktop. The focus can be lost due to the user
hitting a System Key combination such as Alt-Tab or Ctrl-Esc which forces
your fullscreen application into the background. The MGL takes care of all
the important details such as saving and restoring the state of the hardware,
so all your suspend application callback needs to do is save the current
state of your program so that when the request is made to re-activate your
application, you can redraw the screen and continue from where you left off.

When the MGL detects that your application has been suspended it will call
the registered callback with a combination of the following flags:

Flag			- Meaning
MGL_DEACTIVATE	- This flag will be sent when your application has lost
				  the input focus and has been suspended.
MGL_REACTIVATE	- This flag will be sent when the user re-activates
				  your fullscreen application again indicating that the
				  fullscreen mode has now been restored and the application
				  must redraw the display ready to continue on.

By default if you have not installed a suspend callback handler, the MGL will
simply restore the display to the original state with the screen cleared to
black when the application is re-activated. If your application is a game or
animation that is continuously updating the screen, you won't need to do
anything as the next frame in the animation will re-draw the screen
correctly.

If your application is caching bitmaps in offscreen video memory however,
all of the bitmaps will need to be restored to the offscreen display device
context when the application is restored (the offscreen memory will be
cleared to black also).

Note:	By the time your callback is called, the display memory may have
		already been lost under DirectDraw. Hence you cannot save and
		restore the contents of the display memory, but must be prepared
		to redraw the entire display when the callback is called with the
		MGL_REACTIVATE flag set.
****************************************************************************/
void MGLAPI MGL_setSuspendAppCallback(
	MGL_suspend_cb_t saveState)
{
	_MGL_suspendApp = saveState;
}
