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
* Environment:	QNX 4
*
* Description:  Photon specific code bindings for the MGL.
*
****************************************************************************/

#include "mgl.h"
#include "mglqnx/internal.h"
#include <i86.h>

extern LIST	*_MGL_winDCList;	/* List of Photon windowed contexts	*/
extern drivertype _VARAPI PHWIN_driver;	/* Photon windowed device driver	*/

ibool MGL_initWindowed(const char *mglpath)
{
	static ibool installed = false;

	_MGL_result = grOK;

	if (installed)
		return true;

	_MGL_initMalloc();

	/* Create the internal scratch buffer */
	if ((_MGL_buf = MGL_malloc(_MGL_bufSize)) == NULL)
		MGL_fatalError("Not enough memory to allocate scratch buffer!\n");

	/* Create all region algebra memory pools */
	_MGL_createSegmentList();
	_MGL_createSpanList();
	_MGL_createRegionList();

	/* Save the path to all MGL files for later */
	strcpy(_MGL_path, mglpath);

#if PHOTON
	_MGL_photon_widget = window;
#endif
	_EVT_resume();

	return installed = true;
}

/*******************************************************************************
DESCRIPTION:
Create a new windowed device context.

HEADER:
mglwin.h

PARAMETERS:
hwnd	- Window handle with which to associate new device context

RETURNS:
Pointer to the allocated windowed device context, or NULL if not enough memory.

REMARKS:
Creates a new windowed device context for drawing information into a window on
the Photon desktop. When you create a windowed device context, you associate 
it with a standard Photon PtWidget_t for the window that you wish MGL to display 
it’s output on. Windowed device contexts are special device contexts in that you 
cannot directly access the surface for the device, nor can you actually use the MGL 
rasterizing functions to draw on the device surface. The only rasterizing functions
supported are the MGL_bitBlt and MGL_stretchBlt for blt’ing datafrom memory device
contexts to the window the desktop.

However in order to change the color palette values for the data copied to the
window, you must use the MGL palette functions on the windowed display device
context. Note that MGL automatically takes care of creating a proper Photon
identity palette for the windowed device context, so as long as you program the
same palette values for the windowed device and the memory device you should get
the maximum performance blt’ing speed.

SEE ALSO:
MGL_createMemoryDC, MGL_createDisplayDC, MGL_destroyDC,
MGL_setWinDC, MGL_activatePalette, MGL_initWindowed
*******************************************************************************/
MGLDC * MGLAPI MGL_createWindowedDC(
	MGL_HWND hwnd)
{
	MGLDC	*dc;
        driverent de;

	if ((dc = _LST_newNode(sizeof(MGLDC))) == NULL) {
		_MGL_result = grNoMem;
		return NULL;
		}
	if (_MGL_winDCList == NULL) {
		if ((_MGL_winDCList = _LST_create()) == NULL) {
			_MGL_result = grNoMem;
			goto Error;
			}
		}
	
	de.driver = &PHWIN_driver;
	if (!_MGL_initDC(dc,&de,hwnd,-1,-1,1,false,MGL_DEFAULT_REFRESH))
		goto Error;

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

#ifdef PHOTON
/*
 * MGL_exit will call this; this will remove any photon state, if
 * applicable
 */
void
_MGL_unhookPhoton(void)
{
	if (photon_widget != NULL)
		PtRemoveEventHandler(widget, Ph_EV_KEY | , 
		    motion_button_callbacks, NULL);
}
#endif
