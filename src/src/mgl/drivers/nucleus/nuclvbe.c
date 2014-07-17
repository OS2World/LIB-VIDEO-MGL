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
* Description:	Source file for the SciTech VGA and VBE dumb framebuffer
*				driver. We ignore any accelerated graphics modes in this
*				driver, and can support VGA, VBE 1.2 banked and VBE 2.0/3.0
*				linear modes.
*
****************************************************************************/

#include "mgl.h"
#include "drivers/nucleus/nuclvbe.h"

/*---------------------------- Driver Tables -----------------------------*/

drivertype VBE_driver = {
	MGL_VBENAME,
	"SciTech Nucleus VGA/VBE Driver",
	"\r\n\r\nVersion " DRIVER_VERSION_STR " - " __DATE__ "\r\n"
	"Copyright (C) 1996-1999 SciTech Software\r\n",
	DRIVER_MAJOR_VERSION,
	DRIVER_MINOR_VERSION,
	NUCLVBE_createInstance,
	NUCLVBE_detect,
	NUCLVBE_initDriver,
	NUCLVBE_destroyInstance,
	};

/*------------------------- Implementation --------------------------------*/

/****************************************************************************
RETURNS:
Pointer to intialised instance data.

REMARKS:
This function initialises any necessary instance data for the device.
****************************************************************************/
void * MGLAPI NUCLVBE_createInstance(void)
{
	return NUCL_initInstance(MGL_calloc(1,sizeof(NUCL_data)));
}

/****************************************************************************
PARAMETERS:
data			- Instance data for the driver
id				- Id of this driver for storing in mode table
numModes		- Pointer to the number of modes already detected
availableModes	- Table to fill in with available mode information

RETURNS:
True if the device was found, false if not.

REMARKS:
Detects if a driver with the desired capabilities is present, and if so
register all the modes reported by the driver with the MGL.
****************************************************************************/
ibool MGLAPI NUCLVBE_detect(
	void *data,
	int id,
	int *numModes,
	modetab availableModes)
{
	return NUCL_detect((NUCL_data*)data,false,id,numModes,availableModes);
}

/****************************************************************************
PARAMETERS:
data			- Instance data for the driver
dc				- Device context to initialise
mode			- Display mode to set
hwnd			- Handle to OS window for the mode
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
ibool MGLAPI NUCLVBE_initDriver(
	void *data,
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
	return NUCL_initDriver((NUCL_data*)data,dc,mode,hwnd,virtualX,virtualY,numBuffers,stereo,refreshRate,useLinearBlits);
}

/****************************************************************************
REMARKS:
Frees the internal data structures allocated during the detection process
****************************************************************************/
void MGLAPI NUCLVBE_destroyInstance(
	void *data)
{
	if (data)
		MGL_free(data);
}
