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
* Environment:  Any
*
* Description:  RTTarget32 specific code bindings for the MGL. 
				Based on MSDOS specific version.
*
****************************************************************************/

#include "mgl.h"
#include "mglrtt\internal.h"

/*--------------------------- Global Variables ----------------------------*/

/* Globals private to this module */

static 	ibool	inited = false;		/* Is the MGL inited?				*/
static 	ibool	installed = false;	/* Is the MGL installed?			*/
static	ulong	multiplier = 1193*2L;/* Timer 0 multiplier factor		*/
uchar	_VARAPI *_MGL_biosPtr = 0;	/* Pointer to BIOS data area     	*/

/*------------------------- Implementation --------------------------------*/

/* {secret} */
unsigned MGLAPI _MGL_readtimer(void);

/* {secret} */
void MGLHW_init(void)
/****************************************************************************
*
* Function:		MGLHW_init
*
* Description:	Initialise the basic PC display device driver stuff. If
*				we have not already done so, we obtain a pointer to the
*				start of video memory and the BIOS data area.
*
****************************************************************************/
{
	int	i;

	if (!_MGL_biosPtr) {
		_MGL_biosPtr = PM_getBIOSPointer();
		if (_MGL_biosPtr)
			VBE_init();

		/* Now initialize the timer multiplier factor that we use for
		 * timing in the _EVT_delay routine. This code determines if the
		 * timer is in mode 2 or mode 3. In mode 2 the timer can have both
		 * odd and even values, while in mode 3 only even values.
		 */
		for (i = 0; i < 100; i++)
			if ((_MGL_readtimer() & 1) == 0) {
				multiplier = 1193L;
				break;
				}
		}
}

/****************************************************************************
DESCRIPTION:
Declare a fatal error and exit gracefully.

HEADER:
mgraph.h

PARAMETERS:
msg	- Message to display

REMARKS:
A fatal internal error has occurred, so we shutdown the graphics systems, display
the error message and quit. You should call this function to display your own
internal fatal errors.
****************************************************************************/
void MGL_fatalError(
	const char *msg)
{
	MGL_exit();
	fprintf(stderr,"FATAL ERROR: %s\n",msg);
	exit(EXIT_FAILURE);
}

/* {secret} */
void _MGL_disableAutoPlay(void) {}
/* {secret} */
void _MGL_restoreAutoPlay(void) {}

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
Destroys a specified device context, freeing up all resources allocated by the device
context. This can fail for a number of reasons, so check the MGL_result code to
determine the cause of the failure.

If the device context that was destroyed was the last active display device context,
the video mode is reset back to the original video mode (or back to the normal GDI
desktop for Windows). Note that calling MGL_exit automatically destroys all
currently allocated device contexts.

SEE ALSO:
MGL_createDisplayDC, MGL_createOffscreenDC, MGL_createLinearOffscreenDC,
MGL_createMemoryDC, MGL_createWindowedDC
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
		case MGL_LINOFFSCREEN_DEVICE:
			return _MGL_destroyOffscreenDC(dc);
		case MGL_MEMORY_DEVICE:
			return _MGL_destroyMemoryDC(dc);
		}
	__MGL_result = grInvalidDC;
	return false;
}

/****************************************************************************
DESCRIPTION:
Changes the current fullscreen mode or switches to windowed mode.

HEADER:
mgldos.h, mglwin.h

PARAMETERS:
mode	- New display mode to use

RETURNS:
True if the mode is available, false if mode is invalid.

REMARKS:
This function changes the current fullscreen display mode used by MGL, or informs
MGL that you are about to switch to windowed mode (for Windows versions). The
application should destroy all display and offscreen device contexts currently
allocated before calling this function, and then re-create all the required device
contexts for the new mode after calling this function. A typical code sequence to
change display modes would be as follows:

	MGLDC *dc;
	... init MGL and create DC as per normal ...
	MGL_destroyDC(dc);
	MGL_changeDisplayMode(grSVGA_640x480x256);
	dc = MGL_createDisplayDC();
	... mode has been changed to the new mode ...


Note that if there are any active display device contexts and offscreen device
contexts when this function is called, they will be destroyed by this call and the
system will be reset back to text mode. However none of the device contexts will be
re-created and it is up to the application to recreate all necessary device contexts.

If you are using this function to change display modes on the fly in MGL and you
wish to allow the user to switch to a windowed mode under Windows, you must call
this function with the grWINDOWED parameter before you create your windowed
window, or call MGL_exit after finishing in fullscreen modes. For example the
following code might be used to switch to a windowed mode.

	// Destroy the existing fullscreen mode and DC's
	MGL_destroyDC (mgldc);
	MGL_destroyDC (memdc);
	mgldc = memdc = NULL;

	// Signal to MGL that we are going windowed
	MGL_changeDisplayMode(grWINDOWED);

	// Create the windowed window
	window = CreateWindow(...);
	ShowWindow(window, SW_SHOWDEFAULT);

SEE ALSO:
MGL_init, MGL_createDisplayDC
****************************************************************************/
ibool MGLAPI MGL_changeDisplayMode(
	int mode)
{
	__MGL_result = grOK;

	/* Check if the mode is currently available */
	if (mode >= grMAXMODE || DEV.availableModes[mode].driver == 0xFF) {
		__MGL_result = grInvalidMode;
		return false;
		}
	DEV.modeId = mode;

	/* Uncache the current DC */
	MGL_makeCurrentDC(NULL);

	/* Destroy all active offscreen contexts for this device */
	while (DEV.offDCList)
		_MGL_destroyOffscreenDC(_LST_first(DEV.offDCList));

	/* Destroy all active display contexts, which restores text mode */
	while (DEV.dispDCList)
		_MGL_destroyDisplayDC(_LST_first(DEV.dispDCList));
	return true;
}

/****************************************************************************
REMARKS:
Function to initialise the internals of the MGL for normal operation. This
should only need to be done once for the life of a program.
****************************************************************************/
void _MGL_initInternal(void)
{
	/* Install event handling hooks */
	if (!inited) {
		MGLHW_init();
		_MGL_initMalloc();
		EVT_init(_MS_moveCursor);
		_MS_init();

		/* Create the internal scratch buffer */
		if ((_MGL_buf = MGL_malloc(_MGL_bufSize)) == NULL)
			MGL_fatalError("Not enough memory to allocate scratch buffer!\n");

		/* Create all region algebra memory pools */
		_MGL_createSegmentList();
		_MGL_createSpanList();
		_MGL_createRegionList();

		/* Override the default fatal error cleanup function for PM library */
		PM_setFatalErrorCleanup(MGL_exit);

		inited = true;
		}
}

/****************************************************************************
DESCRIPTION:
Initializes MGL for fullscreen and windowed operation.

HEADER:
mgldos.h, mglwin.h

PARAMETERS:
pDriver	- Place to store detected graphics device driver id
pMode	- Place to store suggested graphics mode id
mglpath	- Path to standard MGL resource files

RETURNS:
True if successful, false on error.

REMARKS:
This function initializes MGL for fullscreen operation, and sets it up to use the
specified fullscreen device driver and video mode. If you pass the value grDETECT
in the driver parameter, this function will automatically call the MGL_detectGraph
routine to detect the installed video hardware, and initialize itself accordingly
(please refer to MGL_detectGraph for more information on the detection process). The
video mode used in this case will be the one suggested by the installed video device
driver, and will be returned in the mode parameter.

Before you can call this routine, you must ensure that you have registered the
appropriate device drivers that you are interested in using in your code with the
MGL_registerDriver function. The process of registering the device drivers ensure
that the code will be linked in when you link your application (by default the code
will not be linked, to save space in the resulting executable). You can simply call
MGL_registerAllDispDrivers.  If you wish to start a video mode other than the
default one suggested by the video device driver, you should call the
MGL_detectGraph routine first, and pass the value returned for driver and your
selected video mode in mode to initialize the specific mode. The fullscreen device
drivers currently supported by MGL are enumerated in MGL_driverType.

The mglpath variable is used by MGL to locate all MGL resources files in their
standard locations. The value passed in here should point to the base directory
where all the standard MGL resources are located, which may simply be the current
directory (i.e. a value of "."). When MGL is searching for resource files (bitmaps,
icons, fonts and cursors) it will first attempt to find the files just by using the
filename itself. Hence if you wish to look for a specific bitmap or font file, you
should pass the full pathname to the file that you want. If the filename is a simple
relative filename (i.e. "MYFONT.FNT"), MGL will then search in the standard
directories relative to the path specified in mglpath. As a final resort MGL will also
look for the files relative to the MGL_ROOT environment variable, which can be
set to point to a standard location where all MGL files will be stored on the user's
machine. The standard locations that MGL will look for the resource files are as
follows:

Resource	- Base pathname
Bitmaps		- mglpath\BITMAPS
Fonts		- mglpath\FONTS
Icons		- mglpath\ICONS
Cursors		- mglpath\CURSORS

If anything went wrong during the initialization process, MGL will return a result
code via the MGL_result routine. You can then use this result code to determine the
cause of the problem, and use the MGL_errorMsg routine to display an appropriate
error message for the user.

SEE ALSO:
MGL_initWindowed, MGL_detectGraph, MGL_result
****************************************************************************/
ibool MGLAPI MGL_init(
	int *pDriver,
	int *pMode,
	const char *mglpath)
{
	__MGL_result = grOK;

	MGL_detectGraph(pDriver,pMode);
	if (*pDriver == grNONE) {
		__MGL_result = grNotDetected;
        return false;
		}

	/* Save selected driver and mode number for later */
	DEV.driverId = *pDriver;
	if (*pMode != grDETECT) {
		if (!MGL_changeDisplayMode(*pMode))
			return false;
		}

	/* Initialise the internals of the MGL */
	_MGL_initInternal();

	/* Save the path to all MGL files for later */
	strcpy(_MGL_path, mglpath);
	installed = true;
	return true;
}

/****************************************************************************
DESCRIPTION:
Suspend low level interrupt handling.

HEADER:
mgldos.h

REMARKS:
This function suspends the low level interrupt handling code used by the SciTech MGL when it
is initialized since MGL takes over the keyboard and mouse interrupt handlers to
manage it's own event queue. If you wish to shell out to DOS or to spawn another
application program temporarily, you must call this function to suspend interrupt
handling or else the spawned application will not be able to access the keyboard and
mouse correctly.

SEE ALSO:
MGL_resume
****************************************************************************/
void MGLAPI MGL_suspend(void)
{
	if (installed) {
		EVT_suspend();
		}
}

/****************************************************************************
DESCRIPTION:
Resume low level event handling code.

HEADER:
mgldos.h

REMARKS:
Resumes the event handling code for MGL. This function should be used to re-
enable the MGL event handling code after shelling out to DOS from your
application code or running another application.

SEE ALSO:
MGL_suspend, MGL_init
****************************************************************************/
void MGLAPI MGL_resume(void)
{
	if (installed) {
		_MS_init();
		EVT_resume();
		}
}

/****************************************************************************
REMARKS:
Destroys all device context for the currently active display device.
****************************************************************************/
static void _MGL_destroyDevice(void)
{
	/* Uncache the current DC */
	MGL_makeCurrentDC(NULL);

	/* Destroy all active offscreen contexts */
	while (DEV.offDCList)
		_MGL_destroyOffscreenDC(_LST_first(DEV.offDCList));

	/* Destroy all active display contexts, which restores text mode */
	while (DEV.dispDCList)
		_MGL_destroyDisplayDC(_LST_first(DEV.dispDCList));

	/* Destroy all active memory device contexts */
	while (_MGL_memDCList)
		_MGL_destroyMemoryDC(_LST_first(_MGL_memDCList));

	/* Unregister all drivers for this device */
	MGL_unregisterAllDrivers();
}

/****************************************************************************
REMARKS:
Closes down the internals of the graphics library.
****************************************************************************/
void _MGL_exitInternal(void)
{
	int	i;

	if (inited) {
		/* Uncache the current DC */
		MGL_makeCurrentDC(NULL);

		/* Hide mouse cursor */
		MS_hide();

		/* Remove event handling hooks */
		EVT_exit();

		/* Destroy all display DC and offscreen DC's for all devices */
		for (i = _MGL_numDevices-1; i >= 0; i--) {
			MGL_selectDisplayDevice(i);
			_MGL_destroyDevice();
			}

		/* Destory all region algebra memory pools */
		_MGL_freeSegmentList();
		_MGL_freeSpanList();
		_MGL_freeRegionList();

		/* Destroy the memory buffer */
		if (_MGL_buf) {
			MGL_free(_MGL_buf);
			_MGL_buf = NULL;
			}

		inited = false;
		}
}

/****************************************************************************
DESCRIPTION:
Closes down the graphics subsystem.

HEADER:
mgraph.h

REMARKS:
This function closes down the graphics subsystem, deallocating any memory
allocated for use by MGL, and restoring the system back into the original text mode
that was active before MGL was started. This routine also properly removes all
interrupt handlers and other system services that MGL hooked when it was
initialized.

You must call this routine before you exit your application, to ensure that the
system is properly terminated.

SEE ALSO:
MGL_init, MGL_initWindowed
****************************************************************************/
void MGLAPI MGL_exit(void)
{
	_MGL_destroyDevice();
	_MGL_exitInternal();
}

/****************************************************************************
DESCRIPTION:
Generate a beep on the PC speaker.

HEADER:
mgldos.h, mglwin.h

PARAMETERS:
freq	- frequency of the beep
milliseconds	- length of beep in milliseconds

REMARKS:
Beeps the PC speaker at the specified frequency for the specified length of time.
****************************************************************************/
void MGLAPI MGL_beep(
	int freq,
	int msecs)
{
	_MGL_sound(freq);
	MGL_delay(msecs);
	_MGL_nosound();
}

/****************************************************************************
DESCRIPTION:
Do nothing for a specified amount of time.

HEADER:
mgldos.h, mglwin.h

PARAMETERS:
milliseconds	- Number of milliseconds to delay

REMARKS:
Delay processing for the specified number of milliseconds.
****************************************************************************/
void MGL_delay(
	int msecs)
{
	ulong 	stop;
	uint	cur,prev;

	stop = (prev = _MGL_readtimer()) + (msecs * multiplier);

	while ((cur = _MGL_readtimer()) < stop) {
		if (cur < prev) {		/* Check for timer wraparound	*/
			if (stop < 0x10000L)
				break;
			stop -= 0x10000L;
			}
		prev = cur;
		}
}

/****************************************************************************
DESCRIPTION:
Get the current timer tick count.

HEADER:
mgldos.h, mglwin.h

RETURNS:
Current timer tick count as a 32 bit integer.

REMARKS:
This function returns the current timer tick as a 32-bit integer value.
The number of ticks in a single second can be determined with the
MGL_getTickResolution function.

SEE ALSO:
MGL_getTickResolution
****************************************************************************/
ulong MGLAPI MGL_getTicks(void)
{
	return PM_getLong(_MGL_biosPtr+0x6C);
}

/****************************************************************************
DESCRIPTION:
Get duration of a timer tick.

HEADER:
mgldos.h, mglwin.h

RETURNS:
Number of seconds in a timer tick * 1,000,000.

REMARKS:
This function returns an unsigned long value indicating the duration of a timer tick
in seconds multiplied by one million. The duration of a timer tick changes
depending on the target environment, so you should use this function to convert the
value to a standard representation.

SEE ALSO:
MGL_getTicks
****************************************************************************/
ulong MGLAPI MGL_getTickResolution(void)
{
	return 54925UL;
}

/****************************************************************************
DESCRIPTION:
Sets the fullscreen suspend application callback function.

HEADER:
mgraph.h

PARAMETERS:
saveState	- New suspend app callback to be used.

REMARKS:
This function is used to register an application suspend callback function. This is
used in fullscreen modes under Windows and is called by MGL when the
application’s fullscreen window has lost the input focus and the system has returned
to the normal GDI desktop. The focus can be lost due to the user hitting a System
Key combination such as Alt-Tab or Ctrl-Esc which forces your fullscreen
application into the background. MGL takes care of all the important details such as
saving and restoring the state of the hardware, so all your suspend application
callback needs to do is save the current state of your program so that when the
request is made to re-activate your application, you can redraw the screen and
continue from where you left off.

When MGL detects that your application has been suspended it will call the
registered callback with a combination of the following flags:

Flag			- Meaning
MGL_DEACTIVATE	- This flag will be sent when your application has lost
				  the input focus and has been suspended.
MGL_REACTIVATE	- This flag will be sent when the user re-activates
				  your fullscreen application again indicating that the
				  fullscreen mode has now been restored and the application
				  must redraw the display ready to continue on.

By default if you have not installed a suspend callback handler, MGL will simply
restore the display to the original state with the screen cleared to black when the
application is re-activated. If your application is a game or animation that is
continuously updating the screen, you won’t need to do anything as the next frame
in the animation will re-draw the screen correctly. If however your application is
caching bitmaps in offscreen video memory, all of the bitmaps will need to be
restored to the offscreen display device context when the application is restored (the
offscreen memory will be cleared to black also).

Note:	By the time your callback is called, the display memory may have already
		been lost under DirectDraw so you cannot save and restore the contents of the
		display memory, but must be prepared to redraw the entire display when the
		callback is called with the MGL_REACTIVATE flag set.
****************************************************************************/
void MGLAPI MGL_setSuspendAppCallback(
	MGL_suspend_cb_t saveState)
{ saveState = saveState; }
