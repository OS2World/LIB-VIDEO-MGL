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
* Description:  8 bit packed pixel device context routines. This code will
*				be linked in if this driver is registered for use. When
*				compiling for Windows, we will use WinG or CreateDIBSection
*				to create a DIB that we can render directly into.
*
****************************************************************************/

#include "mgl.h"
#include "drivers/packed/packed8.h"

/*---------------------------- Driver Tables -----------------------------*/

drivertype PACKED8_driver = {
	MGL_PACKED8NAME,
	"SciTech Nucleus 8bpp Packed Pixel Driver",
	"\r\n\r\nVersion " DRIVER_VERSION_STR " - " __DATE__ "\r\n"
	"Copyright (C) 1996-1999 SciTech Software\r\n",
	DRIVER_MAJOR_VERSION,
	DRIVER_MINOR_VERSION,
	PACKED8_createInstance,
	NULL,
	PACKED8_initDriver,
	PACKED8_destroyInstance,
	};

/*------------------------- Implementation --------------------------------*/

/****************************************************************************
RETURNS:
Pointer to intialised instance data.

REMARKS:
This function initialises any necessary instance data for the device.
****************************************************************************/
void * MGLAPI PACKED8_createInstance(void)
{
	return MGL_calloc(1,sizeof(PACKED8_state));
}

/****************************************************************************
PARAMETERS:
dc	- Device context

REMARKS:
Destroys all internal memory allocated for the device context.
****************************************************************************/
static void MGLAPI PACKED8_destroyDC(
	MGLDC *dc)
{
	if (--dc->v->m.refCount == 0) {
		/* Unload the reference rasteriser when the last DC is destroyed */
		REF2D_unloadDriver(dc->v->m.ref2d,dc->v->m.hModRef2d);
		dc->v->m.ref2d = NULL;
		}
	if (dc->ownMemory)
		_MGL_freeSurface(dc);
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
ibool MGLAPI PACKED8_initDriver(
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
	PACKED8_state		*state = (PACKED8_state*)data;
	gmode_t				*mi = &dc->mi;
	REF2D_driver		*ref2d;
#ifndef	TEST_HARNESS
	GA_buffer			drawBuf;
#endif

	/* Load the reference rasteriser if this is the first instance. We
	 * share the reference rasteriser with all packed pixel memory context
	 * of the same color depth, so we only need to load it once.
	 */
	dc->v = (drv_vec*)state;
	dc->v->m.destroy = PACKED8_destroyDC;
	if (dc->v->m.refCount++ == 0) {
		if (!REF2D_loadDriver(NULL,mi->bitsPerPixel,false,&ref2d,
				&dc->v->m.hModRef2d,&dc->v->m.ref2dSize)) {
			FATALERROR(grFailLoadRef2d);
			return false;
			}
		dc->v->m.ref2d = ref2d;
		}
	else {
		ref2d = dc->v->m.ref2d;
		}

	/* Load the device information	*/
	dc->deviceType 				= MGL_MEMORY_DEVICE;
	dc->flags 					|= MGL_LINEAR_ACCESS;
	mi->maxColor				= 0xFF;
	mi->maxPage 				= 0;
	mi->aspectRatio				= 1000;
	dc->r.getWinDC				= DRV_getWinDC;
	dc->r.getDefaultPalette 	= DRV_getDefaultPalette;
	dc->r.realizePalette		= PACKED8_realizePalette;
	dc->r.makeCurrent			= DRV_makeCurrent;

	/* Setup all rendering vectors for Nucleus functions */
	DRV_setNucleusRenderingVectors(dc,ref2d);

	/* Allocate the device context surface and return */
	if (!dc->surface) {
		/* Allocate the device context surface if we have not done this */
		dc->mi.bytesPerLine = (dc->mi.xRes + 4) & ~3;	/* DWORD align data	*/
		dc->ownMemory = true;
		if (_MGL_allocateSurface(dc) == 0)
			return false;

#ifndef	TEST_HARNESS
		/* Set the draw buffer to the entire memory DC */
		drawBuf.dwSize = sizeof(drawBuf);
		drawBuf.Offset = 0;
		drawBuf.Stride = dc->mi.bytesPerLine;
		drawBuf.Width = dc->mi.xRes+1;
		drawBuf.Height = dc->mi.yRes+1;
		ref2d->SetDrawBuffer(&drawBuf,dc->surface,dc->mi.bitsPerPixel,GAPF(&dc->pf),NULL,true);
#endif
		}
	(void)mode;
	(void)virtualX;
	(void)virtualY;
	(void)numBuffers;
	(void)stereo;
	(void)refreshRate;
	(void)useLinearBlits;
	(void)hwnd;
	return true;
}

/****************************************************************************
REMARKS:
Frees the internal data structures allocated during the detection process
****************************************************************************/
void MGLAPI PACKED8_destroyInstance(
	void *data)
{
	if (data)
		MGL_free(data);
}
