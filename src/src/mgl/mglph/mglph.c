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
* Environment:	QNX
*
* Description:  QNX specific code bindings for the MGL.
*
****************************************************************************/

#include "mgl.h"
#include "mglqnx/internal.h"

/*--------------------------- Global Variables ----------------------------*/

/* Globals private to this module */

static  ibool   inited = false;         /* Is the MGL inited?		 */
static 	ibool	installed = false;	/* Is the MGL installed?			*/
static	ulong   clockres = 0;
int		_MGL_driverId;				/* Driver selected					*/
int		_MGL_modeId;				/* Mode selected					*/
ibool	_VARAPI _MGL_fullScreen = true;
#ifdef __PHOTON__
struct _Ph_ctrl *_MGL_ph_channel = NULL;
#endif

/*------------------------- Implementation --------------------------------*/

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
	int     i;

	VBE_init();
}

void MGL_fatalError(const char *msg)
{
	MGL_exit();
	fprintf(stderr,"FATAL ERROR: %s\n",msg);
	exit(EXIT_FAILURE);
}

/* {secret} */
void _MGL_disableAutoPlay(void) {}
/* {secret} */
void _MGL_restoreAutoPlay(void) {}

ibool MGLAPI MGL_destroyDC(MGLDC *dc)
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
	_MGL_result = grInvalidDC;
	return false;
}

ibool MGLAPI MGL_changeDisplayMode(int mode)
{
	_MGL_result = grOK;

	/* Check if the mode is currently available */
	if (mode >= grMAXMODE || DEV.availableModes[mode].driver == 0xFF) {
		_MGL_result = grInvalidMode;
		return false;
		}
	DEV.modeId = mode;

	/* Uncache the current DC */
	MGL_makeCurrentDC(NULL);

	/* Destroy all active offscreen contexts */
	while (DEV.offDCList)
		_MGL_destroyOffscreenDC(_LST_first(DEV.offDCList));

	/* Destroy all active display contexts, which restores text mode */
	while (DEV.dispDCList)
		_MGL_destroyDisplayDC(_LST_first(DEV.dispDCList));
	return true;
}

void _MGL_initInternal(void)
{
	if (inited==false) {
		/* Install event handling hooks */
		_MGL_initMalloc();
		_EVT_init();

		/* Create the internal scratch buffer */
		if ((_MGL_buf = MGL_malloc(_MGL_bufSize)) == NULL)
			MGL_fatalError("Not enough memory to allocate scratch buffer!\n");

		/* Create all region algebra memory pools */
		_MGL_createSegmentList();
		_MGL_createSpanList();
		_MGL_createRegionList();
		inited=true;
		}
}

ibool MGLAPI MGL_init(int *pDriver,int *pMode,const char *mglpath)
{
	_MGL_result = grOK;

#ifdef __PHOTON__
	if ((_MGL_ph_channel = PhAttach(NULL, NULL)) == NULL)
		MGL_fatalError("MGL_init: PhAttach failed");
#endif
	MGL_detectGraph(pDriver,pMode);
	if (*pDriver == grNONE) {
		_MGL_result = grNotDetected;
		return false;
		}

	/* Save selected driver and mode number for later */
	_MGL_driverId = *pDriver;
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
		_EVT_suspend();
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
void MGL_resume(void)
{
	if (installed) {
		_EVT_resume();
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
	int i;

	if (inited) {

		/* Uncache the current DC */
		MGL_makeCurrentDC(NULL);

		/* Hide mouse cursor */
		MS_hide();

		/* Remove event handling hooks */
		MGL_suspend();

		/* Destroy all display DC and offscreen DC's for all devices */
		for (i = 0; i < _MGL_numDevices; i++) {
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
#ifdef __PHOTON
                if (_MGL_ph_channel != NULL)
                        PhDetach(_MGL_ph_channel);
#endif
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

void MGLAPI MGL_beep(int freq,int msecs)
{
	sound(freq);
	delay(msecs);
	nosound();
}

void MGL_delay(int msecs)
{
	delay(msecs);
}

void MGLAPI MGL_setSuspendAppCallback(MGL_suspend_cb_t saveState)
{ saveState = saveState; }

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
	static unsigned starttime = 0;
	struct timespec t;

	if (!clockres)
		MGL_getTickResolution();
	clock_gettime(CLOCK_REALTIME, &t);
	if (starttime == 0)
		starttime = t.tv_sec * 1000000 + t.tv_nsec / 1000;
	return ((t.tv_sec * 1000000 + t.tv_nsec / 1000) - starttime) / clockres;
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
	struct timespec res;

	if (clockres)
		return (clockres);
	clock_getres(CLOCK_REALTIME, &res);
	clockres = res.tv_nsec/1000;
	return clockres;
}
