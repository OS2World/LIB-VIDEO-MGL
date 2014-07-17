/****************************************************************************
*
*                         SciTech Display Doctor
*
*               Copyright (C) 1991-1998 SciTech Software, Inc.
*                            All rights reserved.             
*
*  ======================================================================
*  |REMOVAL OR MODIFICATION OF THIS HEADER IS STRICTLY PROHIBITED BY LAW|
*  |                                                                    |
*  |This copyrighted computer code is a proprietary trade secret of     |
*  |SciTech Software, Inc., located at 505 Wall Street, Chico, CA 95928 |
*  |USA (www.scitechsoft.com).  ANY UNAUTHORIZED POSSESSION, USE,       |
*  |VIEWING, COPYING, MODIFICATION OR DISSEMINATION OF THIS CODE IS     |
*  |STRICTLY PROHIBITED BY LAW.  Unless you have current, express       |
*  |written authorization from SciTech to possess or use this code, you |
*  |may be subject to civil and/or criminal penalties.                  |
*  |                                                                    |
*  |If you received this code in error or you would like to report      |
*  |improper use, please immediately contact SciTech Software, Inc. at  |
*  |530-894-8400.                                                       |
*  |                                                                    |
*  |REMOVAL OR MODIFICATION OF THIS HEADER IS STRICTLY PROHIBITED BY LAW|
*  ======================================================================
*
* Language:		ANSI C
* Environment:	Any
*
* Description:	Support library for implementing hardware and software
*				stereo support using Nucleus drivers and IHV DirectDraw
*				drivers under Win32.
*
****************************************************************************/

#ifndef	__DDSTEREO_H
#define	__DDSTEREO_H

#include "nucleus/graphics.h"

/*--------------------------- Macros and Typedefs -------------------------*/

/* We use C calling conventions for all our functions */

#define	DDSAPI	_ASMAPI

/****************************************************************************
REMARKS:
Error codes returned by DDS_init function to indicate driver load status
if loading the device driver failed. Most of these error codes are just
duplicates of the codes returned by the Nucleus device driver loader library.

HEADER:
ddstereo.h

MEMBERS:
ddsOK				- No error
ddsNotDetected		- Hardware not detected
ddsNotPOSTed		- Hardware has not been POSTed
ddsDriverNotFound	- Driver file not found
ddsCorruptDriver	- File loaded not a driver file
ddsLoadMem			- Not enough memory to load driver
ddsOldVersion		- Driver file is an older version
ddsIOError			- General I/O error
ddsMemMapError		- Could not map physical memory areas
ddsStereoDisabled	- Stereo support is disabled
ddsStereoStarted	- DDS_start has already been called
ddsUnableToFindCRTC	- Unable to compute valid CRTC timings using GTF
ddsLockFailed		- DirectDraw lock of a surface failed
ddsInternalError	- Internal error in the DirectDraw stereo library
ddsUnknownMode		- Unknown stereo mode
ddsVideoMemError	- Not enough video memory to support stereo mode
****************************************************************************/
typedef enum {
	ddsOK,
	ddsNotDetected,
	ddsNotPOSTed,
	ddsDriverNotFound,
	ddsCorruptDriver,
	ddsLoadMem,
	ddsOldVersion,
	ddsMemMapError,
	ddsIOError,
	ddsStereoDisabled,
	ddsStereoStarted,
	ddsUnableToFindCRTC,
	ddsLockFailed,
	ddsInternalError,
	ddsUnknownMode,
	ddsVideoMemError,
	} DDS_errorType;

#ifdef	__DDRAW_INCLUDED__
/* {secret} */
typedef	LPDIRECTDRAW4			DDS_LPDD;
/* {secret} */
typedef	LPDIRECTDRAWSURFACE4	DDS_LPDDSURF;
/* {secret} */
typedef	HWND					DDS_HWND;
#else
/* {secret} */
typedef	void					*DDS_LPDD;
/* {secret} */
typedef	void					*DDS_LPDDSURF;
/* {secret} */
typedef	void					*DDS_HWND;
#endif

/*--------------------------- Function Prototypes -------------------------*/

#ifdef  __cplusplus
extern "C" {            /* Use "C" linkage when in C++ mode */
#endif

int		DDSAPI DDS_init(int deviceIndex);
void 	DDSAPI DDS_forceGlassesType(int type);
void 	DDSAPI DDS_forceStereoMode(int mode);
void 	DDSAPI DDS_forceWindowedStereoMode(int mode);
void 	DDSAPI DDS_forceRefreshRate(int refresh);
void 	DDSAPI DDS_forceInterlacedRefreshRate(int refresh);
void	DDSAPI DDS_forceBlankInterval(int interval);
int		DDSAPI DDS_getStereoMode(void);
int		DDSAPI DDS_getWindowedStereoMode(void);
int		DDSAPI DDS_getBlankInterval(void);
int		DDSAPI DDS_start(DDS_HWND hwndMain,DDS_LPDD lpDD,DDS_LPDDSURF lpPrimarySurf);
void	DDSAPI DDS_stereoOn(void);
int		DDSAPI DDS_scheduleFlip(DDS_LPDDSURF lpLeftSurf,DDS_LPDDSURF lpRightSurf);
int		DDSAPI DDS_getFlipStatus(void);
void	DDSAPI DDS_waitTillFlipped(void);
void 	DDSAPI DDS_stereoOff(void);
void 	DDSAPI DDS_stop(void);
void	DDSAPI DDS_restoreDisplayMode(void);
void 	DDSAPI DDS_exit(void);

#ifdef	__cplusplus
}						/* End of "C" linkage for C++	*/
#endif

#endif /* __DDSTEREO_H */
