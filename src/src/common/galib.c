/****************************************************************************
*
*           		SciTech Nucleus Graphics Architecture
*
*               Copyright (C) 1991-1998 SciTech Software, Inc.
*                            All rights reserved.
*
*  ======================================================================
*  |REMOVAL OR MODIFICATION OF THIS HEADER IS STRICTLY PROHIBITED BY LAW|
*  |                                                                    |
*  |This copyrighted computer code contains proprietary technology      |
*  |owned by SciTech Software, Inc., located at 505 Wall Street,        |
*  |Chico, CA 95928 USA (http://www.scitechsoft.com).                   |
*  |                                                                    |
*  |The contents of this file are subject to the SciTech Nucleus        |
*  |License; you may *not* use this file or related software except in  |
*  |compliance with the License. You may obtain a copy of the License   |
*  |at http://www.scitechsoft.com/nucleus-license.txt                   |
*  |                                                                    |
*  |Software distributed under the License is distributed on an         |
*  |"AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or      |
*  |implied. See the License for the specific language governing        |
*  |rights and limitations under the License.                           |
*  |                                                                    |
*  |REMOVAL OR MODIFICATION OF THIS HEADER IS STRICTLY PROHIBITED BY LAW|
*  ======================================================================
*
* Language:     ANSI C
* Environment:  Any 32-bit protected mode environment
*
* Description:  C module for the Graphics Accelerator Driver API. Uses
*               the SciTech PM library for interfacing with DOS
*               extender specific functions.
*
****************************************************************************/

#include "nucleus/graphics.h"
#ifdef __WIN32_VXD__
#include "sdd/sddhelp.h"
#else
#include <stdio.h>
#include <stdlib.h>
#endif

/*---------------------------- Global Variables ---------------------------*/

#ifdef	TEST_HARNESS
extern PM_imports 	_VARAPI _PM_imports;
#else
GA_exports	_VARAPI _GA_exports;
static int			loaded = false;
static PE_MODULE	*hModBPD = NULL;

#ifdef	__DRIVER__
extern PM_imports _PM_imports;
#else
#include "pmimp.h"
#endif

static N_imports _N_imports = {
	sizeof(N_imports),
	_OS_delay,
	};

#ifdef	__DRIVER__
extern GA_imports _GA_imports;
#else
static GA_imports _GA_imports = {
	sizeof(GA_imports),
	GA_getSharedInfo,
	GA_TimerInit,
	GA_TimerRead,
	GA_TimerDifference,
	};
#endif
#endif

/*----------------------------- Implementation ----------------------------*/

#define	DLL_NAME 		"graphics.bpd"

#ifndef	TEST_HARNESS
/****************************************************************************
REMARKS:
Fatal error handler for non-exported GA_exports.
****************************************************************************/
static void _GA_fatalErrorHandler(void)
{
	PM_fatalError("Unsupported Nucleus export function called! Please upgrade your copy of Nucleus!\n");
}

/****************************************************************************
PARAMETERS:
shared	- True to load the driver into shared memory.

REMARKS:
Loads the Nucleus binary portable DLL into memory and initilises it.
****************************************************************************/
static ibool LoadDriver(
	ibool shared)
{
	GA_initLibrary_t	GA_initLibrary;
	GA_exports			*gaExp;
	char				filename[PM_MAX_PATH];
	char				bpdpath[PM_MAX_PATH];
	int					i,max;
	ulong 				*p;

	/* Check if we have already loaded the driver */
	if (loaded)
		return true;
	PM_init();

	/* First try to see if we can find the system wide shared exports
	 * if they are available. Under Windows this connects to our global
	 * shared Nucleus loader in the helper VxD.
	 */
	_GA_exports.dwSize = sizeof(_GA_exports);
	if (GA_getSharedExports(&_GA_exports))
		return loaded =true;

	/* Open the BPD file */
	if (!PM_findBPD(DLL_NAME,bpdpath))
		return false;
	strcpy(filename,bpdpath);
	strcat(filename,DLL_NAME);
	if ((hModBPD = PE_loadLibrary(filename,shared)) == NULL)
		return false;
	if ((GA_initLibrary = (GA_initLibrary_t)PE_getProcAddress(hModBPD,"_GA_initLibrary")) == NULL)
		return false;
	bpdpath[strlen(bpdpath)-1] = 0;
	if (strcmp(bpdpath,PM_getNucleusPath()) == 0)
		strcpy(bpdpath,PM_getNucleusConfigPath());
	else {
		PM_backslash(bpdpath);
		strcat(bpdpath,"config");
		}
	if ((gaExp = GA_initLibrary(shared,bpdpath,filename,&_PM_imports,&_N_imports,&_GA_imports)) == NULL)
		PM_fatalError("GA_initLibrary failed!\n");

	/* Initialize all default imports to point to fatal error handler
	 * for upwards compatibility, and copy the exported functions.
	 */
	max = sizeof(_GA_exports)/sizeof(GA_initLibrary_t);
	for (i = 0,p = (ulong*)&_GA_exports; i < max; i++)
		*p++ = (ulong)_GA_fatalErrorHandler;
	memcpy(&_GA_exports,gaExp,MIN(sizeof(_GA_exports),gaExp->dwSize));
	loaded = true;
	_GA_postLoadDriver();
	return true;
}

/* The following are stub entry points that the application calls to
 * initialise the Nucleus loader library, and we use this to load our
 * driver DLL from disk and initialise the library using it.
 */

/* {secret} */
int NAPI GA_status(void)
{
	if (!loaded)
		return nDriverNotFound;
	return _GA_exports.GA_status();
}

/* {secret} */
const char * NAPI GA_errorMsg(
	N_int32 status)
{
	if (!loaded)
		return "Unable to load Nucleus device driver!";
	return _GA_exports.GA_errorMsg(status);
}

/* {secret} */
int NAPI GA_getDaysLeft(N_int32 shared)
{
	if (!LoadDriver(shared))
		return -1;
	return _GA_exports.GA_getDaysLeft(shared);
}

/* {secret} */
int NAPI GA_registerLicense(uchar *license,N_int32 shared)
{
	if (!LoadDriver(shared))
		return 0;
	return _GA_exports.GA_registerLicense(license,shared);
}

/* {secret} */
ibool NAPI GA_loadInGUI(N_int32 shared)
{
	if (!LoadDriver(shared))
		return false;
	return _GA_exports.GA_loadInGUI(shared);
}

/* {secret} */
int NAPI GA_enumerateDevices(N_int32 shared)
{
	if (!LoadDriver(shared))
		return 0;
	return _GA_exports.GA_enumerateDevices(shared);
}

/* {secret} */
GA_devCtx * NAPI GA_loadDriver(N_int32 deviceIndex,N_int32 shared)
{
	if (!LoadDriver(shared))
		return NULL;
	return _GA_exports.GA_loadDriver(deviceIndex,shared);
}

/* {secret} */
void NAPI GA_mapShared(
	GA_devCtx *dc)
{
	if (hModBPD) {
		PM_mapShared(hModBPD);
		_GA_exports.GA_mapShared(dc);
		}
}

/* {secret} */
ibool NAPI MDBX_open(const char *filename,ibool shared)
{
	if (!LoadDriver(shared))
		return -1;
	return _GA_exports.MDBX_open(filename,shared);
}

/* {secret} */
void NAPI GA_getGlobalOptions(
	GA_globalOptions *options)
{
	if (LoadDriver(false))
		_GA_exports.GA_getGlobalOptions(options);
}

/* {secret} */
void NAPI GA_setGlobalOptions(
	GA_globalOptions *options)
{
	if (LoadDriver(false))
		_GA_exports.GA_setGlobalOptions(options);
}

#endif

void NAPI _OS_delay8253(N_uint32 microSeconds);

/****************************************************************************
REMARKS:
This function delays for the specified number of microseconds
****************************************************************************/
void NAPI _OS_delay(
	N_uint32 microSeconds)
{
	static ibool 	inited = false;
	LZTimerObject	tm;

	if (_GA_haveCPUID() && (_GA_getCPUIDFeatures() & CPU_HaveRDTSC) != 0) {
		if (!inited) {
			ZTimerInit();
			inited = true;
			}
		LZTimerOnExt(&tm);
		while (LZTimerLapExt(&tm) < microSeconds)
			;
		LZTimerOnExt(&tm);
		}
	else
		_OS_delay8253(microSeconds);
}
