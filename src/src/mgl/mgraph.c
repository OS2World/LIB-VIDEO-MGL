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
* Description:	Main platform independant interface and setup code.
*
****************************************************************************/

#include "mgl.h"				/* Private declarations					*/

/*--------------------------- Global Variables ----------------------------*/

/* Private global variables */

static ibool	inited = false;		/* True if the library is inited	*/

/* Public global variables */

MGLDC	_MGL_dc;					/* Global device context 			*/
MGLDC *	_MGL_dcPtr = NULL;			/* Pointer to active context		*/
int		_MGL_bufSize = DEF_MGL_BUFSIZE;
void *	_MGL_buf;					/* Internal MGL scratch buffer		*/
int		__MGL_result;				/* Result of last operation			*/
ibool	__MGL_useLinearBlits = true;/* True if linear blits are used	*/
char	_MGL_path[_MAX_PATH];		/* Root directory for all MGL files	*/
int		_MGL_blueCodeIndex = 0xFF;	/* Blue code index for stereo		*/
int     _MGL_surfLock = -1;			/* Surface lock count				*/
LIST *	_MGL_memDCList = NULL;		/* List of memory contexts			*/
LIST *	_MGL_winDCList = NULL;		/* List of windowed contexts		*/
ibool	_MGL_fullScreen = true;		/* True if in fullscreen mode		*/
ibool	_MGL_isBackground = false;	/* True if running in background	*/

/* Pre-defined pixel formats table */

pixel_format_t _MGL_pixelFormats[] = {
	{0x1F,0x0A,3,0x1F,0x05,3,0x1F,0x00,3,0x01,0x0F,7}, 	/* 555 15bpp	*/
	{0x1F,0x0B,3,0x3F,0x05,2,0x1F,0x00,3,0x00,0x00,0},	/* 565 16bpp    */
	{0xFF,0x10,0,0xFF,0x08,0,0xFF,0x00,0,0x00,0x00,0}, 	/* RGB 24bpp    */
	{0xFF,0x00,0,0xFF,0x08,0,0xFF,0x10,0,0x00,0x00,0}, 	/* BGR 24bpp    */
	{0xFF,0x10,0,0xFF,0x08,0,0xFF,0x00,0,0xFF,0x18,0}, 	/* ARGB 32bpp   */
	{0xFF,0x00,0,0xFF,0x08,0,0xFF,0x10,0,0xFF,0x18,0}, 	/* ABGR 32bpp   */
	{0xFF,0x18,0,0xFF,0x10,0,0xFF,0x08,0,0xFF,0x00,0}, 	/* RGBA 32bpp   */
	{0xFF,0x08,0,0xFF,0x10,0,0xFF,0x18,0,0xFF,0x00,0}, 	/* BGRA 32bpp   */
	};

/* Table of all display devices enumerated in the system */

int				_MGL_numDevices = 1;
int				_MGL_cntDevice = 0;
device_entry 	_MGL_deviceTable[MAX_DISPLAY_DEVICES] = {{{{""}}}};

/* Table of static device drivers, not associated with a hardware device */

driverent _MGL_staticDrivers[] = {
	{MGL_PACKED8NAME,true,false,NULL,NULL,&PACKED8_driver,NULL},
	{MGL_PACKED16NAME,true,false,NULL,NULL,&PACKED16_driver,NULL},
	{MGL_PACKED24NAME,true,false,NULL,NULL,&PACKED24_driver,NULL},
	{MGL_PACKED32NAME,true,false,NULL,NULL,&PACKED32_driver,NULL},
#ifdef	__WINDOWS__
	{MGL_WINDOWEDNAME,true,false,NULL,NULL,&WINGDI_driver,NULL},
#endif
	{"",false,false,NULL,NULL,NULL,NULL}
	};

/*------------------------- Implementation --------------------------------*/

#include "mgl/license.h"

/****************************************************************************
REMARKS:
This function registers a single MGL device driver for use in the device
driver detection list.
****************************************************************************/
static void _MGL_registerDriver(
	const char *name,
	ibool enabled,
	ibool opengl,
	drivertype *driver)
{
	int i = DEV.numDrivers++;
	strcpy(DEV.driverTable[i].name,name);
	DEV.driverTable[i].enabled = enabled;
	DEV.driverTable[i].opengl = opengl;
	DEV.driverTable[i].hModBPD = NULL;
	DEV.driverTable[i].hModOS = NULL;
	DEV.driverTable[i].driver = driver;
	DEV.driverTable[i].data = NULL;
}

/****************************************************************************
REMARKS:
This function registers all MGL device drivers by default in the device
detection list.
****************************************************************************/
static void _MGL_registerDrivers(void)
{
//#ifdef	DEBUG_MGL
	_MGL_registerDriver(MGL_VBENAME,true,false,&VBE_driver);
#ifdef	__WINDOWS__
//	_MGL_registerDriver(MGL_DDRAWNAME,true,false,&DDRAW_driver);
#endif
	_MGL_registerDriver(MGL_NUCLEUSNAME,true,false,&NUCLEUS_driver);
#ifdef	__WINDOWS__
//	_MGL_registerDriver(MGL_OPENGLNAME,false,true,&OPENGL_driver);
//	_MGL_registerDriver(MGL_GLDIRECTNAME,false,true,&GLDIRECT_driver);
#endif
//#else
	// TODO: Register the name of all drivers found in the driver directory
	//		 for dynamic loading in the system! Probably need some new
	//		 PM functions for directory traversal to handle this.
//#endif
}

/****************************************************************************
DESCRIPTION:
Initializes MGL graphics library.

HEADER:
mgraph.h

PARAMETERS:
mglpath	- Path to standard MGL resource files
server	- Name of server to connect to (NULL for local system)

RETURNS:
Number of display devices attached, or 0 on error.

REMARKS:
This function initializes MGL for use, and should always be the first
function called in MGL applications. The mglpath parameter is used by the
MGL to locate all MGL resource files in their standard locations. The value
passed in here should point to the base directory where all the resources
are located. This may be the current directory if all resources are relative
to the current directory of the application (i.e. a value of "." passed in
the mglpath parameter).

After you have called MGL_init, the MGL will automatically detect the
installed graphics hardware when any of MGL_availablePages,
MGL_modeResolution, MGL_modeFlags or MGL_findMode are called. The priority
ordering of which driver will be used depends on the capabilities of the
underlying display hardware, and the drivers will be chosen in the following
order (first one in table is selected in preference to ones below it):

Driver			- Highest performance driver selected
Nucleus			- SciTech Nucleus driver
DirectDraw		- Microsoft DirectDraw driver
VBE2			- SciTech Nucleus VBE/Core 2.0 driver
VBE1			- SciTech Nucleus VBE/Core 1.2 driver
StandardVGA		- SciTech Nucleus VGA driver

For instance if you had DirectDraw installed on your system and MGL found a
SciTech Nucleus driver, those modes supported by the SciTech Nucleus drivers
will use the SciTech Nucleus driver in preference to the DirectDraw driver.
If however the DirectDraw driver has additional modes not supported by the
SciTech Nucleus driver, the DirectDraw drivers would be used for those modes.
If however the user has DirectDraw installed and only has a VBE 2.0 driver
available, the DirectDraw drivers will be used for all modes whenever
possible.

To change this default behavior you can selectively disable those drivers
that you do not wish to be used immediately after calling MGL_init. A typical
sequence of code to register drivers and allows the program to disable
SciTech Nucleus might be as follows:

	int 	mode;
	MGLDC	*dc;

	if (MGL_init(".",NULL) == 0)
		MGL_fatalError(MGL_errorMsg(MGL_result()));
	MGL_disableDriver(MGL_NUCLEUSNAME);
	if ((mode = MGL_findMode(640,480,8)) == -1)
		MGL_fatalError(MGL_errorMsg(MGL_result()));
	dc = MGL_createDisplayDC(mode,1,MGL_DEFAULT_REFRESH);
	...

When the MGL is searching for resource files (bitmaps, icons, fonts and
cursors), it will first attempt to find the files just by using the filename
itself. Hence if you wish to look for a specific bitmap or font file, you
should pass the full pathname to the file that you want. If the filename is
a simple relative filename (i.e. "myfont.fnt"), MGL will then search in the
standard directories relative to the path specified in mglpath. The standard
locations that MGL will look for the resource files are as follows:

Resource	- Base pathname
Bitmaps		- mglpath/bitmaps
Fonts		- mglpath/fonts
Icons		- mglpath/icons
Cursors		- mglpath/cursors

As a final resort the MGL will also look for the files relative to the
MGL_ROOT environment variable. The MGL_ROOT variable can be set to point to
a standard base directory where all MGL resources will be stored on the end
user's machine. For MGL developer machines, MGL_ROOT is usually set to point
to the directory where the SciTech MGL files were installed (ie: c:\scitech
by default for DOS, Windows and OS/2 installations, $HOME/scitech for
Unix installations).

The server parameter is used to connect to a remote server to display the
output of the of the MGL program on a remote machine. For remote
connections, the server name should either be the DNS name or TCP/IP address
of the machine to connect to. For local operation on the client machine,
this parameter should simply be set to NULL.

Once you have initialised the MGL, you then need to find an appropriate
display mode to use with MGL_findDisplayMode, and then create a device
context for all output, using MGL_createDisplayDC (for fullscreen display
device contexts). To support multiple display controllers, please see the
documenation for MGL_selectDisplayDevice to initialise all installed
display devices.

If anything goes wrong during the initialization process, MGL will return a
result code via the MGL_result routine. You can then use this result code
to determine the cause of the problem, and use the MGL_errorMsg routine to
display an appropriate error message for the user.

Note:	To get up and running quickly with your first MGL program, check
		out the MGL_quickInit() function which allows you to start the
		MGL with a single line of code! Or alternatively check our the
		Game Framework API if you are developing game type applications.

Note:	Remote client/server operation is not supported at this time. This
		is something scheduled for a future release of the MGL.

SEE ALSO:
MGL_quickInit, MGL_selectDisplayDevice, MGL_enableAllDrivers,
MGL_disableDriver, MGL_availablePages, MGL_modeResolution, MGL_findMode,
MGL_createDisplayDC, MGL_exit, MGL_result
****************************************************************************/
int MGLAPI MGL_init(
	const char *mglpath,
	const char *server)
{
	int	i;

	/* Register an ISV license if being compiled into the library */
#ifndef	EMPTY_LICENSE
	GA_registerLicense(OemLicense,false);
#endif

	/* Ensure that we only get initialised once! */
	__MGL_result = grOK;
	if (inited)
		MGL_fatalError("MGL_init() called twice!");

	/* Find the number of devices attached to the system, and populate the
	 * global device driver tables for all secondary controllers by copying
	 * the default data from the primary controller's table.
	 */
	_MGL_numDevices = _MGL_enumerateDevices();
	_MGL_registerDrivers();
	for (i = 1; i < _MGL_numDevices; i++)
		memcpy(&_MGL_deviceTable[i],&_MGL_deviceTable[0],sizeof(_MGL_deviceTable[0]));

	/* Initialise the OS specific internals of the MGL */
	strcpy(_MGL_path, mglpath);
	_MGL_initInternal();

	/* Initialise all internal modules */
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

	/* Indicate that we have initialised ourselves */
	inited = true;

	/* Return number of display devices found attached */
	(void)server;
	return _MGL_numDevices;
}

/****************************************************************************
DESCRIPTION:
Quick initialisation function for first time MGL users.

HEADER:
mgraph.h

PARAMETERS:
xRes			- Horizontal resolution for the display mode in pixels
yRes			- Vertical resolution for the display mode in lines
bitsPerPixel	- Color depth for the display mode
numPages		- Number of display pages to use

RETURNS:
Pointer to a fullscreen display device context.

REMARKS:
This function is intended to help first time MGL users get up and running
quickly. Using this utility function you can initialise the MGL for use
in a fullscreen display mode with a single line of code. Note that this
function will automatically bail out with an error message if any of the
initialisation code fails, so you don't need to check for error conditions
on return from this function.

A small 'Hello World' type application using the MGL might be coded as
follows:

	int main(void)
	{
		MGLDC	*dc;
		event_t	evt;
		font_t	*font;

		dc = MGL_quickInit(640,480,8,1);
		if ((font = MGL_loadFont("pc8x8.fnt")) == NULL)
			MGL_fatalError(MGL_errorMsg(MGL_result()));
		MGL_useFont(font);
		MGL_drawStrXY(0,0,"Hello World!");
		EVT_halt(&evt,EVT_KEYDOWN);
		MGL_exit();
	}

Note:	Once you are more familiar with the MGL, we highly recommend you
		add proper initialisation code to your program usin MGL_init and
		related functions. Unless your application is running on a dedicated
		system, it is not usually a good idea to hard code a resolution
		and color depth into the application, but rather allow the user
		to select the mode to be used via configuration files or menus.

Note:	This function

SEE ALSO:
MGL_init, MGL_exit
****************************************************************************/
MGLDC * MGLAPI MGL_quickInit(
	int xRes,
	int yRes,
	int bitsPerPixel,
	int numPages)
{
	MGLDC	*dc;
	int		mode;

	if (MGL_init(".",NULL) == 0)
		MGL_fatalError(MGL_errorMsg(MGL_result()));
	if ((mode = MGL_findMode(xRes,yRes,bitsPerPixel)) == -1) {
		/* If we are selecting 640x480x8 (the default for all examples) and
		 * it is not available, select the first mode in the list. We do
		 * that so that licensed drivers will work properly.
		 */
		if (xRes == 640 && yRes == 480 && bitsPerPixel == 8)
			mode = 0;
		else
			MGL_fatalError(MGL_errorMsg(MGL_result()));
		}
	if ((dc = MGL_createDisplayDC(mode,numPages,MGL_DEFAULT_REFRESH)) == NULL)
		MGL_fatalError(MGL_errorMsg(MGL_result()));
	MGL_makeCurrentDC(dc);
	return dc;
}

/****************************************************************************
DESCRIPTION:
Selects the specified display device as the active device.

HEADER:
mgraph.h

PARAMETERS:
device	- New display device index to make active

RETURNS:
Index of previously active device, or -1 on failure.

REMARKS:
This function is used to select one of the attached display devices as the
active display device from this point forward. This function is used to
change the active display device after a call to MGL_init, and all
subsequent calls to device initialisation functions work with the new
active device. The device numbering starts at 0 for the primary display
controller, and increments by one for each supports display controller. Ie:
the second controller is device 1 while the third controller is device 2
etc. The maximum number of display devices is defined by the compile time
constant MAX_DISPLAY_DEVICES.

You can also select a 'mixed' mode of operation by passing in the
MM_MODE_MIXED parameter, which fully enables the primary display controller
but only enables the secondary display controllers relocateable memory mapped
regions. If this mode is selected, you must have first initialised all
attached secondary controllers to a graphics display mode or this call
will fail.

Note:	Some older display controllers cannot support mixed mode as the
		VGA compatible resources cannot be disabled. These controllers will
		still work fine, however there is more overhead involved in the
		calls to MGL_makeCurrentDC as this function must switch the active
		display device every time it is called. If mixed mode is enabled,
		the MGL_makeCurrentDC function does not have any extra overhead for
		multiple display controller support.

The general function call sequence to use multiple controllers in the MGL
is as follows:

	int 	numDevices,device,mode;
	MGLDC	*dc[MAX_DISPLAY_DEVICES];

	// Initialise the library
	if ((numDevices = MGL_init(".",NULL)) == 0)
		MGL_fatalError(MGL_errorMsg(MGL_result()));
	for (device = 0; device < numDevices; device++) {
		MGL_selectDisplayDevice(device);
		if ((mode = MGL_findMode(640,480,8)) == -1)
			MGL_fatalError(MGL_errorMsg(MGL_result()));
		dc[device] = MGL_createDisplayDC(mode,1,MGL_DEFAULT_REFRESH);
		if (!dc[device])
			MGL_fatalError(MGL_errorMsg(MGL_result()));
		}
	MGL_selectDisplayDevice(MM_MODE_MIXED);

	// Now draw something on each device
	for (device = 0; device < numDevices; device++) {
		MGL_makeCurrentDC(dc[0]);
		MGL_line(0,0,10,10);
		}

	// Now close down the MGL
	MGL_exit();

Note:	Once you have initialised the display and created a display device
		context for each of the displays in the system, you do not need
		to call this function to switch the active display from within
		your rendering code. This is handled automatically by the
		MGL_makeCurrentDC function every time you switch the active
		MGL device context.

SEE ALSO:
MGL_init, MGL_enableAllDrivers, MGL_disableDriver, MGL_availablePages,
MGL_modeResolution, MGL_findMode, MGL_createDisplayDC, MGL_exit, MGL_result
****************************************************************************/
int MGLAPI MGL_selectDisplayDevice(
	int device)
{
	/* Switch the active display device for Multi-Monitor support */
	if (_MGL_numDevices > 1 && (device & ~MM_MODE_MIXED) < _MGL_numDevices) {
		if (device == _MGL_cntDevice)
			return _MGL_cntDevice;
		if (_MGL_setActiveDevice(device)) {
			int oldDevice = _MGL_cntDevice;
			_MGL_cntDevice = device;
			return oldDevice;
			}
		}
	return -1;
}

/****************************************************************************
DESCRIPTION:
Enables all available non-OpenGL MGL device drivers for use.

HEADER:
mgraph.h

REMARKS:
This function enables all non-OpenGL specific MGL device drivers available
on the target system for use, so that they will be used as part of the
dynamic hardware detection process. This is normally the default case
when you first initialise the MGL, so you only need to call this function
if you have disabled any of the device drivers to modify the detection
process. To enable OpenGL hardware support, use the MGL_enableOpenGLDrivers
function.

SEE ALSO:
MGL_enableOpenGLDrivers, MGL_disableDriver
****************************************************************************/
void MGLAPI MGL_enableAllDrivers(void)
{
	int	i;

	for (i = 0; i < DEV.numDrivers; i++) {
		driverent *entry = &DEV.driverTable[i];
		if (!entry->opengl) {
			entry->enabled = true;
			if (entry->driver) {
				/* Destroy and unload the driver if it is currently loaded */
				if (entry->data)
					entry->driver->destroyInstance(entry->data);
				if (entry->hModOS)
					PM_freeLibrary(entry->hModOS);
				else if (entry->hModBPD)
					PE_freeLibrary(entry->hModBPD);
				}
			entry->hModOS = NULL;
			entry->hModBPD = NULL;
#ifndef	DEBUG_MGL
//			entry->driver = NULL;	// TODO: Unregister driver!
#endif
			entry->data = NULL;
			}
		}

	/* Ensure hardware detection runs next time round */
	DEV.numModes = 0;
}

/****************************************************************************
DESCRIPTION:
Enables all available OpenGL specific MGL device drivers for use.

HEADER:
mgraph.h

REMARKS:
This function enables all OpenGL specific MGL device drivers available
on the target system for use, so that they will be used as part of the
dynamic hardware detection process. If you wish to use OpenGL hardware
acceleration in your application, you should call this function to
register all the OpenGL specific drivers. OpenGL specific drivers are
drivers that work with OS specific OpenGL hardware device drivers, and
should only be used if the application is doing OpenGL rendering on the
target OS. For non OpenGL applications, those drivers will be less
efficient than the regular 2D only drivers.

Note:	This function needs to be called for *every* device in the system,
		since each device contains a separate list of enabled device
		drivers to use.

SEE ALSO:
MGL_enableAllDrivers, MGL_disableDriver
****************************************************************************/
void MGLAPI MGL_enableOpenGLDrivers(void)
{
	int	i;

	for (i = 0; i < DEV.numDrivers; i++) {
		driverent *entry = &DEV.driverTable[i];
		if (entry->opengl) {
			entry->enabled = true;
			if (entry->driver) {
				/* Destroy and unload the driver if it is currently loaded */
				if (entry->data)
					entry->driver->destroyInstance(entry->data);
				if (entry->hModOS)
					PM_freeLibrary(entry->hModOS);
				else if (entry->hModBPD)
					PE_freeLibrary(entry->hModBPD);
				}
			entry->hModOS = NULL;
			entry->hModBPD = NULL;
#ifndef	DEBUG_MGL
//			entry->driver = NULL;	// TODO: Unregister driver!
#endif
			entry->data = NULL;
			}
		}

	/* Ensure hardware detection runs next time round */
	DEV.numModes = 0;
}

/****************************************************************************
PARAMETERS:
name	- Name of the driver to find

RETURNS:
Address of the driver, or NULL if not found.

REMARKS:
This function finds the entry for a driver in the global static device driver
table from the name of the driver.
{secret}
****************************************************************************/
driverent *_MGL_findStaticDriver(
	const char *name)
{
	int	i;

	for (i = 0; _MGL_staticDrivers[i].name[0] != 0; i++) {
		if (!strcmp(_MGL_staticDrivers[i].name,name) && _MGL_staticDrivers[i].driver)
			return &_MGL_staticDrivers[i];
		}
	return NULL;
}

/****************************************************************************
DESCRIPTION:
Disables a driver from being used in the detection process.

HEADER:
mgraph.h

PARAMETERS:
name	- Name of the driver to register

RETURNS:
grOK on success, error code on failure.

REMARKS:
This function disables a specific device driver so that it will no longer
be used as part of the dynamic detection process for the currently
active device. By default all device drivers are enabled, and this function
allows the programmer to control the device detection process. For instance
SciTech Nucleus is used in preference to DirectDraw on Windows systems. To
disable the SciTech Nucleus driver and allow DirectDraw to be used, you
would do the following:

	MGL_disableDriver(MGL_NUCLEUSNAME);

The names of the standard device drivers currently supported are:

Driver				- Description
MGL_VBENAME			- SciTech Nucleus VGA and VESA VBE display driver
MGL_NUCLEUSNAME		- SciTech Nucleus Accelerated display driver
MGL_DDRAWNAME		- Microsoft DirectDraw accelerated display driver
MGL_OPENGLNAME      - Microsoft hardware OpenGL display driver
MGL_GLDIRECTNAME    - SciTech GLDirect hardware OpenGL display driver

SEE ALSO:
MGL_enableAllDrivers, MGL_enableAllOpenGLDrivers
****************************************************************************/
int MGLAPI MGL_disableDriver(
	const char *name)
{
	int	i,found = false;

	for (i = 0; i < DEV.numDrivers; i++) {
		if (!strcmp(DEV.driverTable[i].name,name) && DEV.driverTable[i].driver) {
			found = true;
			DEV.driverTable[i].enabled = false;
			break;
			}
		}
	if (!found)
		FATALERROR(grInvalidName);
	return __MGL_result;
}

/****************************************************************************
REMARKS:
This function autodetects the presence of all the standard graphics adapters
supported by MGL. If no suitable hardware is detected, it returns false.
****************************************************************************/
static ibool _MGL_detectHardware(void)
{
	int	i,found = false;

	/* Set the global OS type flag */
	___drv_os_type = PM_getOSType();

	/* Initialise the mode table to empty */
	for (i = 0; i < MAX_DRIVER_MODES; i++) {
		DEV.availableModes[i].driver = 0xFF;
		DEV.availableModes[i].xRes = 0;
		DEV.availableModes[i].yRes = 0;
		DEV.availableModes[i].bits = 0;
		DEV.availableModes[i].pages = 0;
		DEV.availableModes[i].flags = 0;
		}
	DEV.numModes = 0;

	/* Call the driver detection code for all enabled drivers in order */
	__MGL_result = grNotDetected;
	for (i = 0; i < DEV.numDrivers; i++) {
		driverent *entry = &DEV.driverTable[i];
		if (entry->enabled) {
#ifndef	DEBUG_MGL
			// TODO: Dynamically load the driver from disk as necessary!
#endif
			if (!entry->data) {
				/* Create an instance of the driver */
				if ((entry->data = entry->driver->createInstance()) == NULL) {
					FATALERROR(grLoadMem);
					return false;
					}
				}
			if (entry->driver->detect(entry->data,i,&DEV.numModes,DEV.availableModes)) {
				__MGL_result = grOK;
				found = true;
				}
			else {
				/* Destroy the instance of the driver if it was not detected */
				entry->driver->destroyInstance(entry->data);
				entry->data = NULL;
				if (entry->hModOS)
					PM_freeLibrary(entry->hModOS);
				else if (entry->hModBPD)
					PE_freeLibrary(entry->hModBPD);
				}
			}
		}
	if (__MGL_result != grOK)
		FATALERROR(__MGL_result);
	return found;
}

/****************************************************************************
DESCRIPTION:
Determine the number of available video pages for a specific graphics mode.

PARAMETERS:
mode	- MGL mode number to query

HEADER:
mgraph.h

RETURNS:
Number of available display pages for mode, -1 for invalid mode number.

REMARKS:
Returns the number of pages of physical display pages available for a
specific MGL graphics mode. You may call this routine before creating
a display device context with MGL_createDisplayDC, but you cannot call this
function until after you have called MGL_init. This function allows you to
ignore display modes that do not have the required number of hardware
display pages that your application requires.

Note:	If the hardware has not been detected when this call is made,
		the MGL will automatically detect the installed hardware the first
		time this function is called.

SEE ALSO:
MGL_init, MGL_modeResolution, MGL_findMode, MGL_createDisplayDC
****************************************************************************/
int MGLAPI MGL_availablePages(
	int mode)
{
	if (DEV.numModes == 0) {
		if (!_MGL_detectHardware())
			return -1;
		}
	if (mode < 0 || mode >= DEV.numModes) {
		__MGL_result = grInvalidMode;
		return -1;
		}
	__MGL_result = grOK;
	return DEV.availableModes[mode].pages;
}

/****************************************************************************
DESCRIPTION:
Returns the resolution and pixel depth of a specific graphics mode.

HEADER:
mgraph.h

PARAMETERS:
mode			- graphics mode number to get resolution for
xRes			- Place to store the x resolution
yRes			- Place to store the y resolution
bitsPerPixel	- Place to store the pixel depth

RETURNS:
True on success, false for an invalid graphics mode number.

REMARKS:
This function returns the resolution and color depth of the specified MGL
graphics mode. This function is useful for displaying the list of available
modes to the user, or to search for specific graphics modes for use by an
application program depending on the desired resolution or color depth.

Note:	If the hardware has not been detected when this call is made,
		the MGL will automatically detect the installed hardware the first
		time this function is called.

SEE ALSO:
MGL_init, MGL_availablePages, MGL_findMode, MGL_createDisplayDC
****************************************************************************/
ibool MGLAPI MGL_modeResolution(
	int mode,
	int *xRes,
	int *yRes,
	int *bitsPerPixel)
{
	if (DEV.numModes == 0) {
		if (!_MGL_detectHardware())
			return false;
		}
	if (mode < 0 || mode >= DEV.numModes) {
		__MGL_result = grInvalidMode;
		return false;
		}
	__MGL_result = grOK;
	*xRes = DEV.availableModes[mode].xRes;
	*yRes = DEV.availableModes[mode].yRes;
	*bitsPerPixel = DEV.availableModes[mode].bits;
    return true;
}

/****************************************************************************
DESCRIPTION:
Returns the mode flags for the specific graphics mode.

HEADER:
mgraph.h

PARAMETERS:
mode	- graphics mode number to get flags for

RETURNS:
Flags for the specific graphics mode, or 0 for invalid mode.

REMARKS:
This function returns mode flags for the specified mode. The mode flags
are available after calling MGL_init, and provides information about the
hardware capabilities of the graphics mode, such as whether it supports 2D
or 3D acceleration, video acceleration, refresh rate control and hardware
stereo support. You can use these flags to make choices about the graphics
mode to use for your application prior to initialising a specific graphics
mode.

Specific mode flags are enumerated in MGL_modeFlagsType.

Note:	If the hardware has not been detected when this call is made,
		the MGL will automatically detect the installed hardware the first
		time this function is called.

SEE ALSO:
MGL_init, MGL_availablePages, MGL_modeResolution, MGL_findMode,
MGL_createDisplayDC
****************************************************************************/
ulong MGLAPI MGL_modeFlags(
	int mode)
{
	if (DEV.numModes == 0) {
		if (!_MGL_detectHardware())
			return 0;
		}
	if (mode < 0 || mode >= DEV.numModes) {
		__MGL_result = grInvalidMode;
		return 0;
		}
	__MGL_result = grOK;
	return DEV.availableModes[mode].flags;
}

/****************************************************************************
DESCRIPTION:
Finds the number of a mode given the resolution and color depth.

HEADER:
mgraph.h

PARAMETERS:
xRes			- Horizontal resolution for the display mode in pixels
yRes			- Vertical resolution for the display mode in lines
bitsPerPixel	- Color depth for the display mode

RETURNS:
MGL mode number for the mode, or -1 if not found.

REMARKS:
This function searches the list of available display modes in the MGL,
looking for one that matches the specified resolution and color depth. This
function is useful if your application always runs in a specific resolution
and color depth, allowing you to quickly initialise the MGL without needing
to write your own code to search the list of available modes.

Note:	If the hardware has not been detected when this call is made,
		the MGL will automatically detect the installed hardware the first
		time this function is called.

SEE ALSO:
MGL_init, MGL_availablePages, MGL_modeResolution, MGL_modeFlags,
MGL_createDisplayDC
****************************************************************************/
int	MGLAPI MGL_findMode(
	int xRes,
	int yRes,
	int bitsPerPixel)
{
	int 	i;
	modeent	*m = &DEV.availableModes[0];

	if (DEV.numModes == 0) {
		if (!_MGL_detectHardware())
			return -1;
		}
	__MGL_result = grInvalidMode;
	for (i = 0; i < DEV.numModes; i++,m++) {
		if (m->xRes == xRes && m->yRes == yRes && m->bits == bitsPerPixel) {
			__MGL_result = grOK;
			return i;
			}
		}
	return -1;
}

/****************************************************************************
REMARKS:
Destroys all device context for the currently active display device.
****************************************************************************/
static void _MGL_destroyDevice(void)
{
	/* Uncache the current DC */
	MGL_makeCurrentDC(NULL);

	/* Destroy all active overlay contexts */
	while (DEV.overDCList)
		_MGL_destroyOverlayDC(_LST_first(DEV.overDCList));

	/* Destroy all active offscreen contexts */
	while (DEV.offDCList)
		_MGL_destroyOffscreenDC(_LST_first(DEV.offDCList));

	/* Destroy all active offscreen buffers */
	while (DEV.offBufList)
		MGL_destroyBuffer(_LST_first(DEV.offBufList));

	/* Destroy all active display contexts, which restores text mode */
	while (DEV.dispDCList)
		_MGL_destroyDisplayDC(_LST_first(DEV.dispDCList));

	/* Disable any cached memory (which has been freed) */
	DEV.gaInfo = NULL;

	/* Unregister all drivers for this device */
	MGL_enableAllDrivers();
}

/****************************************************************************
DESCRIPTION:
Closes down the MGL.

HEADER:
mgraph.h

REMARKS:
This function closes down the MGL, deallocating any memory allocated for use
by the MGL, and restoring the system back into the original display mode that
was active before the MGL was started. This routine also properly removes
all interrupt handlers and other system services that MGL hooked when it
was initialized.

You must call this routine before you exit your application, to ensure that
the system is properly terminated.

SEE ALSO:
MGL_init
****************************************************************************/
void MGLAPI MGL_exit(void)
{
	int i;

// TODO: Unload and destroy all static drivers if we can!
// TODO: Unload and destroy all Nucleus drivers if we can!

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

		/* Destroy all active windowed device contexts */
		while (_MGL_winDCList)
			_MGL_destroyWindowedDC(_LST_first(_MGL_winDCList));

		/* Destroy all active memory device contexts */
		while (_MGL_memDCList)
			_MGL_destroyMemoryDC(_LST_first(_MGL_memDCList));

		/* Destory all region algebra memory pools */
		_MGL_freeSegmentList();
		_MGL_freeSpanList();
		_MGL_freeRegionList();

		/* Destroy the memory buffer */
		if (_MGL_buf) {
			MGL_free(_MGL_buf);
			_MGL_buf = NULL;
			}

		/* Perform any OS specific exit code */
		_MGL_exitInternal();

		/* Indicate that we have not exited */
		inited = false;
		}
}

/****************************************************************************
DESCRIPTION:
Sets the size of the internal MGL buffer.

HEADER:
mgraph.h

PARAMETERS:
size	- New size of the internal MGL buffer

REMARKS:
This function sets the size of the internal MGL scratch buffer, which the
MGL uses for local scratch space while rasterizing the primitives. The
default size of this buffer is 32Kb, which is adequate for most needs. If
however you attempt to render some primitives and MGL runs out of local
storage space you will need to increase the size of this internal buffer.

Note that this routine must be called before MGL_init is called for the
first time.

SEE ALSO:
MGL_init
****************************************************************************/
void MGLAPI MGL_setBufSize(
	unsigned size)
{
	_MGL_bufSize = size;
}

/****************************************************************************
DESCRIPTION:
Declare a fatal error and exit gracefully.

HEADER:
mgraph.h

PARAMETERS:
msg	- Message to display
...	- Variable argument list to display

REMARKS:
A fatal internal error has occurred, so we shutdown the graphics systems,
display the error message and quit. You should call this function to
display your own internal fatal errors. You can use this function like
printf(), allowing you to format the results for the output message, so
long as the entire message string is less than 1024 bytes in length.
****************************************************************************/
void MGLAPI MGL_fatalError(
	const char *msg,
	...)
{
	va_list	argptr;
	char	buf[1024];

	va_start(argptr, msg);
	vsprintf(buf, msg, argptr);
	va_end(argptr);
	PM_fatalError(buf);
}

/****************************************************************************
REMARKS:
Internal fatal error handler if the scratch buffer is too small
{secret}
****************************************************************************/
void _MGL_scratchTooSmall(void)
{
	MGL_fatalError("Scratch buffer too small for rendering operation!\n");
}

/****************************************************************************
DESCRIPTION:
Returns result code of the last graphics operation.

HEADER:
mgraph.h

RETURNS:
Result code of the last graphics operation

REMARKS:
This function returns the result code of the last graphics operation. The
internal result code is reset back to grOK on return from this routine, so
you should only call the routine once after the graphics operation. Error
codes returned by this function are enumerated in MGL_errorType.

SEE ALSO:
MGL_setResult, MGL_errorMsg
****************************************************************************/
int MGLAPI MGL_result(void)
{
	int	result;

	result = __MGL_result;
	__MGL_result = grOK;
	return result;
}

/****************************************************************************
DESCRIPTION:
Sets the internal MGL result flag.

HEADER:
mgraph.h

PARAMETERS:
result	- New internal result flag

REMARKS:
Sets the internal MGL result flag to the specified value. This routine is
primarily for extension libraries, but you can use it to add your own
extension functions to the MGL that will return result codes in the same
manner as the MGL itself;

SEE ALSO:
MGL_result, MGL_errorMsg
****************************************************************************/
void MGLAPI MGL_setResult(
	int result)
{
	__MGL_result = result;
}

/****************************************************************************
PARAMETERS:
enable	- Flag to enable and disable linear blits

REMARKS:
This internal function disables linear blit operations in the Nucleus
drivers, forcing the MGL to use rectangular blits. This is an internal
testing function to allow the proper testing of both linear and rectangular
blits for conformance testing purposes. It has little if any use for
real world applications (linear blits are always much better than
rectangular blits for offscreen memory buffers!).
{secret}
****************************************************************************/
void MGLAPI MGL_useLinearBlit(
	ibool enable)
{
	__MGL_useLinearBlits = enable;
}

/****************************************************************************
DESCRIPTION:
Returns a string describing an error condition code.

HEADER:
mgraph.h

PARAMETERS:
err	- Error code to obtain string for

RETURNS:
Pointer to string describing the error condition.

REMARKS:
Returns a pointer to a string describing a specified error code. You can use
this to convert the error codes from a numerical id return by MGL_result to
a string which you can display for the users of your programs.

SEE ALSO:
MGL_result
****************************************************************************/
const char * MGLAPI MGL_errorMsg(
	int err)
{
	static char	*msgs[] = {
		"No error",
		"Graphics driver has not been installed",
		"Graphics hardware not detected",
		"Couldn't find graphics driver",
		"Graphics driver file corrupted",
		"Not enough memory to load graphics driver",
		"Invalid mode for selected driver",
		"Invalid driver number",
		"General graphics error",
		"Invalid graphics driver name",
		"Not enough memory to perform operation",
		"Selected graphics mode not supported",
		"Invalid font data",
		"Font file corrupted",
		"Couldn't find font file",
		"Incorrect version for driver file",
		"Invalid device type for operation",
		"Invalid device context",
		"Invalid cursor file",
		"Cursor file was not found",
		"Invalid icon file",
		"Icon file was not found",
		"Invalid bitmap file",
		"Bitmap file was not found",
		"Invalid error number",
		"Only Windows 2.x font files supported",
		"MGL_endRegion() was not called",
		"Double buffering is not available",
		"OffscreenDC's require hardware bitBlt support",
		"No available Offscreen display memory",
		"Invalid pixel format for memory DC",
		"Invalid offscreen buffer",
		"Display DC has not been created",
		"2D reference rasteriser driver failed to load",
		};
	if (err > 0 || err < grLastError)
		err = grLastError-1;
	return msgs[-err];
}

/****************************************************************************
DESCRIPTION:
Get a the name of the device driver used for particular graphics mode.

HEADER:
mgraph.h

PARAMETERS:
mode	- graphics mode number

RETURNS:
Pointer to the name of device driver serving this mode

REMARKS:
This function returns the name of the device driver that is currently being
used to support the specified graphics mode. The MGL provides a number of
device drivers for supporting the fullscreen graphics mode resolutions
depending on the capabilities of the underlying hardware. This function
allows you to determine which driver is currently being used to support
each mode.
****************************************************************************/
const char * MGLAPI MGL_modeDriverName(
	int mode)
{
	if (0 <= mode && mode < DEV.numModes) {
		int driver = DEV.availableModes[mode].driver;
		if (driver != 0xFF && DEV.driverTable[driver].driver)
			return DEV.driverTable[driver].driver->realname;
		else
			return "Invalid graphics mode";
		}
	else
		return "Invalid graphics mode";
}
