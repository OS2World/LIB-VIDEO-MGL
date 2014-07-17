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
* Description:  MSDOS specific code bindings for the MGL.
*
****************************************************************************/

#include "mgl.h"
#include "mgldos\internal.h"

/*--------------------------- Global Variables ----------------------------*/

static ibool installed = false;

/*------------------------- Implementation --------------------------------*/

/****************************************************************************
RETURNS:
Number of display devices attached to the system.

REMARKS:
This function detects the number of display devices attached to the system,
and uses whatever mechanism is available on the target OS to determine
this (usually a combination of Nucleus and the OS provided services).
****************************************************************************/
int	_MGL_enumerateDevices(void)
{
	return GA_enumerateDevices(false);
}

/****************************************************************************
REMARKS:
Function to initialise the internals of the MGL for normal operation.
****************************************************************************/
void _MGL_initInternal(void)
{
	installed = true;
}

/****************************************************************************
PARAMETERS:
device	- New display device to make active

REMARKS:
This function sets the active display device in the system to be used for
device numeration and detection.
****************************************************************************/
int	_MGL_setActiveDevice(
	int device)
{
	return GA_setActiveDevice(device);
}

/****************************************************************************
REMARKS:
Temporarily disables AutoPlay operation while we are running in fullscreen
graphics modes for systems that support this (such as Windows).
****************************************************************************/
void _MGL_disableAutoPlay(void)
{
}

/****************************************************************************
REMARKS:
Re-enables AutoPlay operation when returning to regular desktop mode.
****************************************************************************/
void _MGL_restoreAutoPlay(void)
{
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
Closes down the internals of the graphics library specific to this OS.
****************************************************************************/
void _MGL_exitInternal(void)
{
	installed = false;
}

/****************************************************************************
REMARKS:
Initialise the bitmap handle as necessary for the memory DC.
****************************************************************************/
void _MGL_initBitmapHandle(
	MGLDC *dc,
	MGL_HBITMAP hbm)
{
	(void)dc;
	(void)hbm;
}

/****************************************************************************
PARAMETERS:
dc	- Device context.

RETURNS:
Total size of the device context surface

REMARKS:
Allocate the memory for the device surface. This version
simply uses calloc() to allocate a buffer large enough.
****************************************************************************/
long _MGL_allocateSurface(
	MGLDC *dc)
{
	long size = (long)dc->mi.bytesPerLine * (dc->mi.yRes+1);
	if ((dc->surface = MGL_calloc(1,size)) == NULL) {
		FATALERROR(grNoMem);
		return 0;
		}
	return size;
}

/****************************************************************************
PARAMETERS:
dc	- Memory device context with surface to destroy

REMARKS:
Destroys the packed pixel device surface for a memory device context.
****************************************************************************/
void _MGL_freeSurface(
	MGLDC *dc)
{
	MGL_free(dc->surface);
}

/****************************************************************************
PARAMETERS:
dc		- Device context
pal		- Palette to realize
num		- Number of colors to realise
index	- First color to realize

REMARKS:
Realizes the palette for an 8 bit memory device context. If anything
special needs to be handled by the OS, it should be done in here.
****************************************************************************/
void MGLAPI PACKED8_realizePalette(
	MGLDC *dc,
	palette_t *pal,
	int num,
	int index,
	int waitVRT)
{
	(void)dc;
	(void)pal;
	(void)num;
	(void)index;
	(void)waitVRT;
}
