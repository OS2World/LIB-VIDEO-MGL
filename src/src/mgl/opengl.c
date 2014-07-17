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
* Description:  Platform independant module to support the OpenGL API in
*				the MGL. We compile in platform dependant code to handle
*				stuff specific to that OS platform.
*
****************************************************************************/

#include "mgldd.h"
#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/mglmesa.h"
#include "GL/mglsgi.h"

/*--------------------------- Global Variables ----------------------------*/

// TODO: All this needs to be moved into the per device structures, so that
//		 we can have separate OpenGL libraries loaded for each controller!!

#ifndef	DEBUG_OPENGL
static ibool		OpenGLLoaded = false;
static PE_MODULE	*_MGL_hModGL = NULL;
static PE_MODULE	*_MGL_hModGLU = NULL;
int					_MGL_whichOpenGL = MGL_GL_AUTO;
int					_MGL_cntOpenGL;
MESA_exports		_VARAPI _MESA_exports;
SGI_exports			_VARAPI _SGI_exports;
MGL_glFuncs			_VARAPI _MGL_glFuncs;
MGL_glFuncs			*_MGL_glFuncsPtr = NULL;
MGL_gluFuncs		_VARAPI _MGL_gluFuncs;
MGL_gluFuncs		*_MGL_gluFuncsPtr = NULL;
extern PM_imports 	_VARAPI _PM_imports;

/* Functions to import into the Mesa DLL */

MESA_imports _VARAPI _MESA_imports = {
	sizeof(MESA_imports),
	MGL_makeCurrentDC,
	MGL_setActivePage,
	MGL_setVisualPage,
	MGL_surfaceAccessType,
	MGL_isDisplayDC,
	MGL_isWindowedDC,
	MGL_isMemoryDC,
	MGL_createMemoryDC,
	MGL_destroyDC,
	MGL_bitBltCoord,
	MGL_setPaletteEntry,
	MGL_setPalette,
	MGL_getPalette,
	MGL_realizePalette,
	};
#endif

/*------------------ Platform dependent Implementation --------------------*/

#ifndef	DEBUG_OPENGL
#if		defined(__SMX32__)
#include "mglsmx/opengl.c"
#elif	defined(__RTTARGET__)
#include "mglrtt/opengl.c"
#elif	defined(__REALDOS__)
#include "mgldos/opengl.c"
#elif	defined(__WINDOWS32__)
#include "mglwin/opengl.c"
#elif	defined(__OS2__)
#include "mglos2/opengl.c"
#elif	defined(__LINUX__)
#include "mgllinux/opengl.c"
#elif	defined(__QNX__)
#include "mglqnx/opengl.c"
#elif	defined(__BEOS__)
/*#include "mglbeos/opengl.c"*/
#else
#error	OpenGL module not ported to this platform yet!
#endif
#endif

/*------------------------- Implementation --------------------------------*/

/* Define the names of the OpenGL implementations we can work with */

#define	SGIGL_DLL	"sgigl.bpd"
#define	SGIGLU_DLL	"sgiglu.bpd"
#define	MESAGL_DLL	"mesagl.bpd"
#define	MESAGLU_DLL	"mesaglu.bpd"

#ifndef	DEBUG_OPENGL
/****************************************************************************
REMARKS:
Fatal error handler for non-exported MGL_glFuncs.
****************************************************************************/
static void _GL_fatalErrorHandler(void)
{
	PM_fatalError("Unsupported OpenGL export function called!\n");
}

/****************************************************************************
REMARKS:
Fatal error handler for non-exported _MGL_gluFuncs.
****************************************************************************/
static void _GLU_fatalErrorHandler(void)
{
	PM_fatalError("Unsupported OpenGL GLU export function called!\n");
}

/****************************************************************************
REMARKS:
Function to initialise the exported OpenGL functions for the OpenGL
implementation we have connected to. The mechanism we use here ensures that
we can support forwards and backwards binary compatibility.
****************************************************************************/
static void _MGL_initGLFuncs(
	MGL_glFuncs *glFuncs)
{
	int		i,max;
	ulong 	*p;

	max = sizeof(MGL_glFuncs)/sizeof(GA_initLibrary_t);
	for (i = 0,p = (ulong*)&_MGL_glFuncs; i < max; i++)
		*p++ = (ulong)_GL_fatalErrorHandler;
	memcpy(&_MGL_glFuncs,glFuncs,MIN((int)sizeof(MGL_glFuncs),(int)glFuncs->dwSize));
}

/****************************************************************************
REMARKS:
Function to initialise the exported OpenGL functions for the OpenGL
implementation we have connected to. The mechanism we use here ensures that
we can support forwards and backwards binary compatibility.
****************************************************************************/
static void _MGL_initGLUFuncs(
	MGL_gluFuncs *gluFuncs)
{
	int		i,max;
	ulong 	*p;

	max = sizeof(MGL_gluFuncs)/sizeof(GA_initLibrary_t);
	for (i = 0,p = (ulong*)&_MGL_gluFuncs; i < max; i++)
		*p++ = (ulong)_GLU_fatalErrorHandler;
	memcpy(&_MGL_gluFuncs,gluFuncs,MIN((int)sizeof(MGL_gluFuncs),(int)gluFuncs->dwSize));
}
#endif

/****************************************************************************
RETURNS:
True if OpenGL is loaded, false if not.

REMARKS:
Attempts to dynamically load the OpenGL implementation DLL and set our
vectors to point to the corresponding OpenGL functions.
{secret}
****************************************************************************/
ibool MGL_glLoadOpenGL(
	MGLDC *dc)
{
#ifndef	DEBUG_OPENGL
	char					GLName[_MAX_PATH] = "";
	char					GLUName[_MAX_PATH] = "";
	char					bpdpath[_MAX_PATH];
	SGI_initLibrary_t		SGI_initLibrary;
	SGIGLU_initLibrary_t	SGIGLU_initLibrary;
	MESA_initLibrary_t		MESA_initLibrary;
	MESAGLU_initLibrary_t	MESAGLU_initLibrary;
	MGL_glFuncs 			*glFuncs;
	MGL_gluFuncs 			*gluFuncs;

	if (!_MGL_hModGL) {
		/* Choose which implementation to use depending on user settings,
		 * detected environment etc. Note that some versions are not
		 * available on all operating environments.
		 */
		_MGL_cntOpenGL = _MGL_whichOpenGL;
		while (GLName[0] == 0) {
			switch (_MGL_cntOpenGL) {
				case MGL_GL_AUTO:
					/* Select default OpenGL if we have hardware acceleration,
					 * or SGI OpenGL if not.
					 */
					if (MGL_glHaveHWOpenGL())
						_MGL_cntOpenGL = MGL_GL_DEFAULT;
					else
						_MGL_cntOpenGL = MGL_GL_SGI;
					break;
				case MGL_GL_DEFAULT:
					if (!__MGL_glFindDefaultOpenGL(GLName,GLUName))
						_MGL_cntOpenGL = MGL_GL_SGI;
					break;
				case MGL_GL_SGI:
					if (!PM_findBPD(SGIGL_DLL,bpdpath))
						_MGL_cntOpenGL = MGL_GL_MESA;
					else {
						strcpy(GLName,bpdpath);
						strcat(GLName,SGIGL_DLL);
						strcpy(GLUName,bpdpath);
						strcat(GLUName,SGIGLU_DLL);
						}
					break;
				case MGL_GL_MESA:
					if (!PM_findBPD(MESAGL_DLL,bpdpath))
						MGL_fatalError("Unable to load OpenGL!");
					else {
						strcpy(GLName,bpdpath);
						strcat(GLName,MESAGL_DLL);
						strcpy(GLUName,bpdpath);
						strcat(GLUName,MESAGLU_DLL);//SGIGLU_DLL;
						}
					break;
				case MGL_GL_GLDIRECT:
					if (!__MGL_glFindGLDirect(GLName,GLUName))
						_MGL_cntOpenGL = MGL_GL_SGI;
					break;
				case MGL_GL_GLDIRECTCAD:
					if (!__MGL_glFindGLDirectCAD(GLName,GLUName))
						_MGL_cntOpenGL = MGL_GL_SGI;
					break;
				}
			}

		/* Load the OpenGL libraries from disk */
		if (_MGL_cntOpenGL == MGL_GL_DEFAULT) {
			__MGL_loadDefaultOpenGL(GLName,GLUName);
			}
		else if (_MGL_cntOpenGL == MGL_GL_SGI) {
			/* Load and initialise the SGI software rendering libraries */
			_MGL_hModGL = PE_loadLibrary(GLName,false);
			_MGL_hModGLU = PE_loadLibrary(GLUName,false);
			if (!_MGL_hModGL || !_MGL_hModGLU)
				MGL_fatalError("Unable to load OpenGL!");
			if ((SGI_initLibrary = (SGI_initLibrary_t)PE_getProcAddress(_MGL_hModGL,"_SGI_initLibrary")) == NULL)
				MGL_fatalError("Unable to init SGIGL!");
			if ((SGIGLU_initLibrary = (SGIGLU_initLibrary_t)PE_getProcAddress(_MGL_hModGLU,"_SGIGLU_initLibrary")) == NULL)
				MGL_fatalError("Unable to init SGIGLU!");
			_SGI_exports.dwSize = sizeof(_SGI_exports);
			if ((glFuncs = SGI_initLibrary(&_PM_imports,&_SGI_exports)) == NULL)
				MGL_fatalError("SGI_initLibrary failed!");
			_MGL_initGLFuncs(glFuncs);
			if ((gluFuncs = SGIGLU_initLibrary(&_PM_imports,glFuncs)) == NULL)
				MGL_fatalError("SGIGLU_initLibrary failed!");
			_MGL_initGLUFuncs(gluFuncs);

			}
		else if (_MGL_cntOpenGL == MGL_GL_MESA) {
			/* Load and initialise the Mesa software rendering libraries */
			_MGL_hModGL = PE_loadLibrary(GLName,false);
			_MGL_hModGLU = PE_loadLibrary(GLUName,false);
			if (!_MGL_hModGL || !_MGL_hModGLU)
				MGL_fatalError("Unable to load OpenGL!");
			if ((MESA_initLibrary = (MESA_initLibrary_t)PE_getProcAddress(_MGL_hModGL,"_MESA_initLibrary")) == NULL)
				MGL_fatalError("Unable to init MesaGL!");
			if ((MESAGLU_initLibrary = (MESAGLU_initLibrary_t)PE_getProcAddress(_MGL_hModGLU,"_MESAGLU_initLibrary")) == NULL)
				MGL_fatalError("Unable to init MesaGLU!");
			_MESA_exports.dwSize = sizeof(_MESA_exports);
			if ((glFuncs = MESA_initLibrary(&_PM_imports,&_MESA_imports,&_MESA_exports)) == NULL)
				MGL_fatalError("MESA_initLibrary failed!");
			_MGL_initGLFuncs(glFuncs);
			if ((gluFuncs = MESAGLU_initLibrary(&_PM_imports,glFuncs)) == NULL)
				MGL_fatalError("MESAGLU_initLibrary failed!");
			_MGL_initGLUFuncs(gluFuncs);
			}
		else if (_MGL_cntOpenGL == MGL_GL_GLDIRECT) {
			__MGL_loadGLDirect(GLName,GLUName);
			}
		else if (_MGL_cntOpenGL == MGL_GL_GLDIRECTCAD) {
			__MGL_loadGLDirectCAD(GLName,GLUName);
			}

		/* If the user app has registered a local copy of the function pointers,
		 * update that local copy here.
		 */
		_MGL_glFuncs.dwSize = sizeof(MGL_glFuncs);
		_MGL_gluFuncs.dwSize = sizeof(MGL_gluFuncs);
		if (_MGL_glFuncsPtr)
			*_MGL_glFuncsPtr = _MGL_glFuncs;
		if (_MGL_gluFuncsPtr)
			*_MGL_gluFuncsPtr = _MGL_gluFuncs;
		}
	(void)dc;
	return OpenGLLoaded = true;
#else
	return true;
#endif
}

/****************************************************************************
REMARKS:
Unloads the currently loaded OpenGL library.
{secret}
****************************************************************************/
void MGL_glUnloadOpenGL(void)
{
#ifndef	DEBUG_OPENGL
	__MGL_glUnloadOpenGL();
	if (_MGL_hModGL) {
		PE_freeLibrary(_MGL_hModGL);
		PE_freeLibrary(_MGL_hModGLU);
		_MGL_hModGL = _MGL_hModGLU = NULL;
		}
	OpenGLLoaded = false;
#endif
}

/****************************************************************************
DESCRIPTION:
Checks for OpenGL hardware acceleration.

HEADER:
mgraph.h

RETURNS:
True if OpenGL is hardware accelerated on the target platform.

REMARKS:
This function will load the system OpenGL libraries and attempt to determine
if hardware acceleration is available or not. On the Win32 platform, this
involves loading the Microsoft OpenGL libraries and checking to see if
OpenGL is accelerated via an ICD or MCD hardware device driver. On other
platforms we check to see if acceleration is available via the system
OpenGL drivers. This function is most useful for determining if OpenGL is
hardware accelerated when running in windowed modes.
****************************************************************************/
ibool MGLAPI MGL_glHaveHWOpenGL(void)
{
#ifndef	DEBUG_OPENGL
	return __MGL_glHaveHWOpenGL();
#else
	return true;
#endif
}

/****************************************************************************
DESCRIPTION:
Sets the OpenGL implementation type to use.

HEADER:
mgraph.h

PARAMETERS:
type	- New OpenGL type to set (MGL_glOpenGLType)

REMARKS:
This function sets the current OpenGL implementation type to use
according to the passed in type parameter (MGL_glOpenGLType enumeration).
In the AUTO mode we automatically determine which version of OpenGL to
use depending on the target runtime system. Unless there is hardware
acceleration available we choose Silicon Graphic's OpenGL, but if hardware
acceleration is present we use the system provided OpenGL implementation
so we can utilize the hardware (which is Microsoft OpenGL for Win32).

Note:	If you wish to be able to choose between the registered MGL hardware
		OpenGL drivers within your application, use the
		MGL_glEnumerateDrivers and MGL_glSetDriver functions instead.

SEE ALSO:
MGL_glSetDriver, MGL_glEnumerateDrivers
****************************************************************************/
void MGLAPI MGL_glSetOpenGLType(
	int type)
{
#ifndef	DEBUG_OPENGL
	MGL_glUnloadOpenGL();
	_MGL_whichOpenGL = type;
#endif
}

/****************************************************************************
DESCRIPTION:
Enumerates the names of all available OpenGL implementations.

HEADER:
mgraph.h

RETURNS:
Pointer to a NULL terminated list of available OpenGL driver names.

REMARKS:
This function returns a NULL terminated list of OpenGL driver names to the
application program, which describes all the available OpenGL drivers on
the system. Once you have obtained the driver names, you can change the
current OpenGL driver with MGL_glSetDriver.

Note that this function will always returns built in names for 'auto',
'microsoft', 'sgi' and 'mesa' and setting a driver to these names corresponds
to changing the OpenGL driver type with MGL_glSetOpenGLType. However if
there are multiple fullscreen hardware OpenGL drivers on the system and
registered with the MGL, they will be listed one after the other. For
example if you have an ATI 3DRage, a 3Dfx Voodoo and an NEC PowerVR installed
in your system and there are fullscreen OpenGL drivers for all these
boards registered with the MGL, the names for these drivers would be
returned in this list also.

SEE ALSO:
MGL_glSetDriver, MGL_glSetOpenGLType
****************************************************************************/
const char ** MGLAPI MGL_glEnumerateDrivers(void)
{
	static char *driverNames[] = {
		"auto",
		"default",
		"sgi",
		"mesa",
		"gldirect",
		"gldirectcad",
		NULL,
		};
	return (const char **)driverNames;
}

/****************************************************************************
DESCRIPTION:
Sets the currently active OpenGL driver

HEADER:
mgraph.h

PARAMETERS:
name	- Name of the OpenGL driver to make the current driver

RETURNS:
True on success, false on failure.

REMARKS:
This function changes the currently active OpenGL driver to the name you
pass in. Note that the name you pass in must be one of the names returned
by the MGL_glEnumerateDrivers, or the function will fail.

SEE ALSO:
MGL_glEnumerateDrivers, MGL_glSetOpenGLType
****************************************************************************/
ibool MGLAPI MGL_glSetDriver(
	const char *name)
{
	int type;
	if (stricmp(name,"auto") == 0)
		type = MGL_GL_AUTO;
	else if (stricmp(name,"default") == 0)
		type = MGL_GL_DEFAULT;
	else if (stricmp(name,"sgi") == 0)
		type = MGL_GL_SGI;
	else if (stricmp(name,"mesa") == 0)
		type = MGL_GL_MESA;
	else if (stricmp(name,"gldirect") == 0)
		type = MGL_GL_GLDIRECT;
	else if (stricmp(name,"gldirectcad") == 0)
		type = MGL_GL_GLDIRECTCAD;
	else
		return false;
	MGL_glSetOpenGLType(type);
	return true;
}

/****************************************************************************
DESCRIPTION:
Set the OpenGL rendering functions table pointer

HEADER:
GL/gl.h

PARAMETERS:
glFuncs		- Pointer to MGL_glFuncs structure to fill in
gluFuncs	- Pointer to MGL_gluFuncs structure to fill in

REMARKS:
This function is called by the user application if the MGL libraries are
stored in a DLL. By letting the MGL know about the OpenGL function pointer
table, when the OpenGL implementation is swapped by the MGL it automatically
updates the table in the user DLL to point to the newly loaded OpenGL
entry points. This allows the code in the DLL to run with maximum performance
for calls to OpenGL via function pointers.
****************************************************************************/
void APIENTRY MGL_setOpenGLFuncs(
	MGL_glFuncs *glFuncs,
	MGL_gluFuncs *gluFuncs)
{
#ifndef	DEBUG_OPENGL
	if (glFuncs) {
		if ((size_t)glFuncs->dwSize > sizeof(MGL_glFuncs))
			MGL_fatalError("Incorrect structure size passed to MGL_setOpenGLFuncs");
		*glFuncs = _MGL_glFuncs;
		}
	if (gluFuncs) {
		if ((size_t)gluFuncs->dwSize > sizeof(MGL_gluFuncs))
			MGL_fatalError("Incorrect structure size passed to MGL_setOpenGLFuncs");
		*gluFuncs = _MGL_gluFuncs;
		}
	_MGL_glFuncsPtr = glFuncs;
	_MGL_gluFuncsPtr = gluFuncs;
#endif
}

#ifndef	DEBUG_OPENGL
/****************************************************************************
PARAMETERS:
x,y,w,h	- Coordinates
color	- Color to fill rectangle with

REMARKS:
Framebuffer clear function for SGI OpenGL.
****************************************************************************/
static void MGLAPI _MGLSGI_fillRect(
	int x,
	int y,
	int w,
	int h,
	uint color)
{
	DC.r.SetForeColor(color);
	DC.r.cur.DrawRect(x,y,w,h);
}

/****************************************************************************
PARAMETERS:
dc		- MGL device context
visual	- MGL Visual to create surface information for

REMARKS:
Constructs a surface information block to be passed to SGI OpenGL from the
MGL device context.
****************************************************************************/
static MGLSGISurfaceInfo *GetMGLGLSurfaceInfo(
	MGLDC *dc,
	MGLVisual *visual)
{
	static MGLSGISurfaceInfo 	surface;
	MGLDC						*gldc;

	if (MGL_surfaceAccessType(dc) != MGL_LINEAR_ACCESS || (dc->mi.maxPage == 0 && visual->db_flag)) {
		/* If we dont have direct access to the device context surface, then
		 * create a memory back buffer if we dont already have one for the
		 * surface and use that. Note that we also do this if we only have
		 * one page for the device context and the programmer has requested
		 * double buffering. 
		 */
		if (!dc->memdc) {
			MGL_makeCurrentDC(NULL);
			dc->memdc = MGL_createMemoryDC(dc->mi.xRes+1,dc->mi.yRes+1,dc->mi.bitsPerPixel,&dc->pf);
			if (!dc->memdc)
				return NULL;
			}
		}
	if (dc->memdc)
		gldc = dc->memdc;
	else
    	gldc = dc;
	surface.frontSurface = gldc->surface;
	if (gldc->mi.maxPage > 0)
		surface.backSurface = gldc->surface;
	else
		surface.backSurface = NULL;
	surface.width = gldc->mi.xRes+1;
	surface.height = gldc->mi.yRes+1;
	surface.bytesPerLine = gldc->mi.bytesPerLine;
	surface.bitsPerPixel = gldc->mi.bitsPerPixel;
	surface.pf = gldc->pf;
	surface.pal = (palette_t*)gldc->colorTab;
	if (gldc->r.beginDirectAccess == NULL) {
		surface.BeginDirectAccess = NULL;
		surface.EndDirectAccess = NULL;
		}
	else {
		surface.BeginDirectAccess = gldc->r.beginDirectAccess;
		surface.EndDirectAccess = gldc->r.endDirectAccess;
		}
	surface.Fill = _MGLSGI_fillRect;
	return &surface;
}
#endif

/****************************************************************************
DESCRIPTION:
Choose an OpenGL visual to best match the passed in visual.

PARAMETERS:
dc		- MGL device context
visual	- Structure containing OpenGL visual information

REMARKS:
This function examines the visual passed in and modifies the values to
best match the capabilities of the underlying OpenGL implementation.
If a requested capability is not supported, the structure will be modified
for the capabilities that the OpenGL implementation does support (ie: lowering
the depth buffer size to 16 bits etc).

SEE ALSO:
MGL_glSetVisual, MGL_glCreateContext
****************************************************************************/
void MGLAPI MGL_glChooseVisual(
	MGLDC *dc,
	MGLVisual *visual)
{
#ifndef	DEBUG_OPENGL
	MGL_glLoadOpenGL(dc);
	if (_MGL_cntOpenGL == MGL_GL_MESA) {
		/* Mesa fullscreen and windowed modes */
		_MESA_exports.MGLMesaChooseVisual(dc,visual);
		}
	else if (_MGL_cntOpenGL == MGL_GL_SGI) {
		/* SGI OpenGL fullscreen and windowed modes */
		MGLSGISurfaceInfo *surfaceInfo = GetMGLGLSurfaceInfo(dc,visual);
		if (!surfaceInfo)
			goto Error;
		_SGI_exports.MGLSGIChooseVisual(surfaceInfo,visual);
		goto Error;
		}
	else {
		/* Default system OpenGL implementation */
		if (!__MGL_glChooseVisual(dc,visual))
			goto Error;
		}
	return;

Error:
	memset(visual,0,sizeof(*visual));
#else
	MGLMesaChooseVisual(dc,visual);
#endif
}

/****************************************************************************
DESCRIPTION:
Attempts to set the specified OpenGL visual for the MGL device context.

HEADER:
mgraph.h

PARAMETERS:
dc		- MGL device context
visual	- Structure containing OpenGL visual information

RETURNS:
True on success, false if visual not supported by OpenGL implementation.

REMARKS:
This function sets the passed in OpenGL visual for the MGL device context
and makes it the visual that will be used in the call to MGL_glCreateContext.
Note that this function may fail if the OpenGL visual requested is invalid,
and you should call MGL_glChooseVisual first to find a visual that best
matches the underlying OpenGL implementation. For instance if the OpenGL
implementation only supports a 16-bit z-buffer, yet you request a 32-bit
z-buffer this function will fail.

The OpenGL visual is used to define the visual capabilities of the OpenGL
rendering context that will be created with the MGL_glCreateContext function,
and includes information such as whether the mode should be an RGB mode or
color index mode, whether it should be single buffered or double buffered,
whether a depth buffer (zbuffer) should be used and how many bits it should
be etc.

Note:	You can only set the visual for a context once, and it is an
		error to call MGL_glSetVisual more than once for an MGL device
		context, and you also cannot change a visual once you have set it
		without first destroying the OpenGL rendering context.

SEE ALSO:
MGL_glChooseVisual, MGL_glCreateContext
****************************************************************************/
ibool MGLAPI MGL_glSetVisual(
	MGLDC *dc,
	MGLVisual *visual)
{
#ifndef	DEBUG_OPENGL
	MGL_glLoadOpenGL(dc);
	if (MGL_isMemoryDC(dc) && visual->db_flag)
		return false;
	if (_MGL_cntOpenGL == MGL_GL_MESA) {
		/* Mesa windowed and fullscreen modes */
		MGL_makeCurrentDC(NULL);
		dc->cntVis = *visual;
		if (!_MESA_exports.MGLMesaSetVisual(dc,visual))
			goto Error;
		return true;
		}
	else if (_MGL_cntOpenGL == MGL_GL_SGI) {
		/* SGI OpenGL fullscreen and windowed modes */
		MGLSGISurfaceInfo *surfaceInfo = GetMGLGLSurfaceInfo(dc,visual);
		if (!surfaceInfo)
			goto Error;
		if (!_SGI_exports.MGLSGICheckVisual(surfaceInfo,visual))
			return false;
		MGL_makeCurrentDC(NULL);
		dc->cntVis = *visual;
		return true;
		}
	else {
		/* Default system OpenGL implementation */
		if (!__MGL_glSetVisual(dc,visual))
			goto Error;
		}

Error:
	if (dc->memdc) {
		MGL_destroyDC(dc->memdc);
		dc->memdc = NULL;
		}
	return false;
#else
	return MGLMesaSetVisual(dc,visual);
#endif
}

/****************************************************************************
DESCRIPTION:
Returns the current OpenGL visual for the device context.

HEADER:
mgraph.h

PARAMETERS:
dc		- MGL device context
visual	- Place to store the OpenGL visual information

REMARKS:
This function returns the current OpenGL visual that has been set for the
MGL device context.

SEE ALSO:
MGL_glSetVisual, MGL_glCreateContext
****************************************************************************/
void MGLAPI MGL_glGetVisual(
	MGLDC *dc,
	MGLVisual *visual)
{
	*visual = dc->cntVis;
}

/****************************************************************************
DESCRIPTION:
Creates the OpenGL rendering context for the MGL device context.

HEADER:
mgraph.h

PARAMETERS:
dc		- MGL device context to enable OpenGL API for
flags	- Flags to force type of OpenGL library used

RETURNS:
True if context was created successfully, false otherwise

REMARKS:
This function creates an OpenGL rendering context for the MGL device context,
and enables support for OpenGL functions for the MGL device context. To
provide a quick and easy way to get the MGL up and running with OpenGL
support, you can pass in some simple flags that let the MGL know what
OpenGL features you want enabled in the OpenGL visual for the MGL device
context, by combining values in the MGL_glContextType enumeration. For
instance if you wanted to start OpenGL with support for an RGB, double
buffered and z-buffered mode you would pass in:

 MGL_GL_RGB | MGL_GL_DOUBLE | MGL_GL_DEPTH

If you wish to have complete control over the OpenGL visual that is used
for the MGL device context, you can call MGL_glChooseVisual and MGL_glSetVisual
before calling MGL_glCreateContext, and then pass in a value of MGL_GL_VISUAL
to tell the MGL to use the visual you have already set for the device context
instead of trying to create one from the passed in flags.

Note:	Once you have created an OpenGL rendering context for the MGL
		device context, you must first call MGL_glMakeCurrent to make that
		specific MGL device context the current OpenGL rendering context
        before you call any core OpenGL API functions. 

Note:	You /must/ call this function first before you attemp to make any
		calls to the core OpenGL API functions (such as glVertex3f etc),
		as the MGL will not have initialised its internal dispatch tables
		until this call is made.

SEE ALSO:
MGL_glChooseVisual, MGL_glSetVisual, MGL_glMakeCurrent
****************************************************************************/
ibool MGLAPI MGL_glCreateContext(
	MGLDC *dc,
	int flags)
{
	MGLVisual	visual = dc->cntVis;

	if (!(flags & MGL_GL_VISUAL)) {
		/* Select the visual for the programmer based on the passed in
		 * flags for easy setup and initialization.
		 */
		memset(&visual,0,sizeof(visual));
		visual.rgb_flag = (flags & MGL_GL_INDEX) == 0;
		visual.alpha_flag = (flags & MGL_GL_ALPHA) != 0;
		visual.db_flag = (flags & MGL_GL_DOUBLE) != 0;
		visual.stereo_flag = (flags & MGL_GL_STEREO) != 0;
		if (flags & MGL_GL_DEPTH)
			visual.depth_size = 16;
		if (flags & MGL_GL_STENCIL)
			visual.stencil_size = 8;
		if (flags & MGL_GL_ACCUM)
			visual.accum_size = 16;
		if (!MGL_glSetVisual(dc,&visual))
			return false;
		}

#ifndef	DEBUG_OPENGL
	/* Create a 3:3:2 RGB palette for the device context for 8bpp RGB modes */
	if (dc->mi.bitsPerPixel == 8) {
		palette_t pal[256];
		if (dc->cntVis.rgb_flag) {
			if (_MGL_cntOpenGL == MGL_GL_MESA)
				MGL_getHalfTonePalette(pal);
			else
				__MGL_glCreateRGBPalette(dc,pal);
			}
		else
			MGL_getPalette(dc,pal,256,0);
		MGL_glSetPalette(dc,pal,256,0);
		MGL_glRealizePalette(dc,256,0,-1);
		}

	if (_MGL_cntOpenGL == MGL_GL_MESA) {
		/* Mesa windowed and fullscreen modes */
		return _MESA_exports.MGLMesaCreateContext(dc,flags & MGL_GL_FORCEMEM);
		}
	else if (_MGL_cntOpenGL == MGL_GL_SGI) {
		/* SGI OpenGL fullscreen and windowed modes */
		MGLSGISurfaceInfo *surfaceInfo = GetMGLGLSurfaceInfo(dc,&dc->cntVis);
		if (!surfaceInfo)
			return false;
		dc->wm.gendc.SGICtx = _SGI_exports.MGLSGICreateContext(surfaceInfo,&dc->cntVis);
		dc->wm.gendc.SGISurf = _SGI_exports.MGLSGICreateSurface(surfaceInfo,&dc->cntVis);

		/* Initialize double buffering */
		if (dc->cntVis.db_flag && !dc->memdc) {
			MGL_setVisualPage(dc,dc->frontBuffer = 0,false);
			MGL_setActivePage(dc,dc->backBuffer = 1);
			dc->glDrawBuffer = GL_BACK;
			}
		else
			dc->glDrawBuffer = GL_FRONT;
		return (dc->wm.gendc.SGICtx && dc->wm.gendc.SGISurf);
		}
	else {
		/* Default system OpenGL implementation */
		return __MGL_glCreateContext(dc,flags);
		}
#else
	/* Create a 3:3:2 RGB palette for the device context for 8bpp RGB modes */
	if (dc->mi.bitsPerPixel == 8) {
		palette_t pal[256];
		if (dc->cntVis.rgb_flag)
			MGL_getHalfTonePalette(pal);
		else
			MGL_getPalette(dc,pal,256,0);
		MGL_glSetPalette(dc,pal,256,0);
		MGL_glRealizePalette(dc,256,0,-1);
		}
	return MGLMesaCreateContext(dc,flags & MGL_GL_FORCEMEM);
#endif
}

/****************************************************************************
DESCRIPTION:
Makes a MGL device context the current OpenGL rendering context.

HEADER:
mgraph.h

PARAMETERS:
dc	- Device context to make the current OpenGL rendering context.

REMARKS:
This function makes the MGL device context the current rendering context for
all OpenGL rendering commands. You must first call MGL_glCreateContext for the
MGL device context to create a valid OpenGL rendering context before you can
call this function to make it the rendering current OpenGL rendering context.

The MGL and OpenGL allow you to create multiple rendering context, and to
switch between them for output you must use this function to make one of
them current at a time. You cannot have more than one OpenGL rendering
context current at one time. For instance you may be drawing to a fullscreen
OpenGL rendering context, but have an MGL memory device context that you
wish to render a small 3D scene into, so you would make the memory device
context the current OpenGL rendering context with a call to this function.
The any subsequent OpenGL commands will draw to the memory device context
instead of the display device context.

SEE ALSO:
MGL_glCreateContext, MGL_glDeleteContext
****************************************************************************/
void MGLAPI MGL_glMakeCurrent(
	MGLDC *dc)
{
#ifndef	DEBUG_OPENGL
	if (_MGL_cntOpenGL == MGL_GL_MESA) {
		/* Mesa windowed and fullscreen modes */
		_MESA_exports.MGLMesaMakeCurrent(dc);
		}
	else if (_MGL_cntOpenGL == MGL_GL_SGI) {
		/* SGI OpenGL fullscreen and windowed modes */
		MGL_makeCurrentDC(dc);
		if (dc) {
			void *frontPtr,*backPtr;
			if (dc->memdc) {
				frontPtr = dc->memdc->surface;
                backPtr = NULL;
				}
			else {
				MGL_setActivePage(dc,dc->frontBuffer);
				frontPtr = dc->surface;
				if (dc->cntVis.db_flag) {
					MGL_setActivePage(dc,dc->backBuffer);
					backPtr = dc->surface;
					}
				else
					backPtr = NULL;
				}
			_SGI_exports.MGLSGIMakeCurrent(dc->wm.gendc.SGICtx,dc->wm.gendc.SGISurf);
			_SGI_exports.MGLSGISetSurfacePtr(dc->wm.gendc.SGISurf,frontPtr,backPtr);
			if (dc->memdc)
				MGL_makeCurrentDC(dc->memdc);
			glDrawBuffer(dc->glDrawBuffer);
			}
		}
	else {
		/* Default system OpenGL implementation */
		__MGL_glMakeCurrent(dc);
		}
#else
	MGLMesaMakeCurrent(dc);
#endif
}

/****************************************************************************
DESCRIPTION:
Delete the OpenGL rendering context associated with the MGL device contex.

HEADER:
mgraph.h

PARAMETERS:
dc	- MGL device context to delete OpenGL rendering context for

REMARKS:
This function destroys the OpenGL rendering context for the MGL device
context, and calls OpenGL to ensure that no rendering context is still
current. You must call this function before you destroy and MGL device
context if you have enabled OpenGL via MGL_glCreateContext.

Note:	After you have called this function, it is an error to make calls to
		the OpenGL API as OpenGL will not have a current rendering context
		active.

SEE ALSO:
MGL_glCreateContext, MGL_glMakeCurrent
****************************************************************************/
void MGLAPI MGL_glDeleteContext(
	MGLDC *dc)
{
#ifndef	DEBUG_OPENGL
	if (_MGL_cntOpenGL == MGL_GL_MESA) {
		/* Mesa fullscreen and windowed modes */
		_MESA_exports.MGLMesaDestroyContext(dc);
		}
	else if (_MGL_cntOpenGL == MGL_GL_SGI) {
		/* SGI OpenGL fullscreen and windowed modes */
		if (dc->wm.gendc.SGICtx) {
			_SGI_exports.MGLSGIMakeCurrent(NULL,NULL);
			if (dc->wm.gendc.SGISurf)
				_SGI_exports.MGLSGIDeleteSurface(dc->wm.gendc.SGISurf);
			_SGI_exports.MGLSGIDeleteContext(dc->wm.gendc.SGICtx);
			if (dc->memdc) {
				/* Destroy the software back buffer */
				MGL_makeCurrentDC(NULL);
				MGL_destroyDC(dc->memdc);
				dc->memdc = NULL;
				MGL_makeCurrentDC(dc);
				}
			}
		}
	else {
		/* Default system OpenGL implementation */
		__MGL_glDeleteContext(dc);
		}
#else
	MGLMesaDestroyContext(dc);
#endif
}

/****************************************************************************
DESCRIPTION:
Swaps the display buffers for OpenGL rendering.

HEADER:
mgraph.h

PARAMETERS:
dc		- MGL device context to swap display buffers for
waitVRT	- True to wait for vertical retrace

REMARKS:
This function swaps the OpenGL rendering buffers, and the current back buffer
becomes the front buffer and vice versa. If you are running in a window,
the context of the back buffer will be copied to the current window. If you
are running in a fullscreen graphics mode with hardware page flipping, this
function will do a hardware page flip to swap the buffers.

When the OpenGL buffers are swapped, you should normally allow MGL/OpenGL
driver to sync to the vertical retrace to ensure that the change occurs in the
correct place, and that you don't get flicker effects on the display. You may
however turn off the vertical retrace synching if you are synching up with the
retrace period using some other means, or if you are measuring the performance
of your application.
****************************************************************************/
void MGLAPI MGL_glSwapBuffers(
	MGLDC *dc,
	int waitVRT)
{
	glFlush();
#ifndef	DEBUG_OPENGL
	if (_MGL_cntOpenGL == MGL_GL_MESA) {
		/* Mesa windowed and fullscreen modes */
		_MESA_exports.MGLMesaSwapBuffers(dc,waitVRT);
		}
	else if (_MGL_cntOpenGL == MGL_GL_SGI) {
		/* SGI OpenGL fullscreen and windowed modes */
		void *frontPtr,*backPtr;
		if (dc->memdc) {
			/* We have a memory back buffer so blit it to the screen and flip */
			MGL_bitBltCoord(dc,dc->memdc,0,0,dc->mi.xRes+1,dc->mi.yRes+1,0,0,MGL_REPLACE_MODE);
			}
		else {
			/* We are doing direct surface access so swap the buffers and
			 * update the surface pointers in the SGI OpenGL drivers.
             */
			dc->frontBuffer = dc->backBuffer;
			dc->backBuffer = (dc->backBuffer+1)%(dc->mi.maxPage+1);
			MGL_setActivePage(dc,dc->frontBuffer);
			frontPtr = dc->surface;
			MGL_setActivePage(dc,dc->backBuffer);
			backPtr = dc->surface;
			if (dc->glDrawBuffer == GL_FRONT)
				MGL_setActivePage(dc,dc->frontBuffer);
			MGL_setVisualPage(dc,dc->frontBuffer,waitVRT);
			_SGI_exports.MGLSGISetSurfacePtr(dc->wm.gendc.SGISurf,frontPtr,backPtr);
			}
		}
	else {
		/* Default system OpenGL implementation */
		__MGL_glSwapBuffers(dc,waitVRT);
		}
#else
	MGLMesaSwapBuffers(dc,waitVRT);
#endif
}

/****************************************************************************
DESCRIPTION:
Resizes the OpenGL buffers for the windowed device context. 

HEADER:
mgraph.h

PARAMETERS:
dc	- MGL device context to resize the buffers for

REMARKS:
This function informs the MGL that the dimensions of a windowed device
context have changed, and that the OpenGL rendering buffers need to be
re-sized to the new dimensions of the window. It is up to the application
programmer to capture the WM_SIZE messages in windowed modes, and call
this function when the window size changes to let the MGL correctly
update the buffer dimensions.

Note:	This function is not necessary in fullscreen modes. 
****************************************************************************/
void MGLAPI MGL_glResizeBuffers(
	MGLDC *dc)
{
#ifndef	DEBUG_OPENGL
	__MGL_glResizeBuffers(dc);
	if (_MGL_cntOpenGL == MGL_GL_MESA) {
		/* Mesa windowed and fullscreen modes */
		_MESA_exports.MGLMesaResizeBuffers();
		}
	else {
		/* All other OpenGL implementations */
		if (dc->memdc) {
			/* We have a memory back buffer so re-size it */
			MGL_destroyDC(dc->memdc);
			dc->memdc = MGL_createMemoryDC(dc->mi.xRes+1,dc->mi.yRes+1,dc->mi.bitsPerPixel,&dc->pf);
			if (!dc->memdc)
				MGL_fatalError("Out of memory re-sizing OpenGL buffers!");
			if (_MGL_dcPtr == dc)
				DC.memdc = dc->memdc;
			if (_MGL_cntOpenGL == MGL_GL_SGI) {
				/* SGI OpenGL fullscreen and windowed modes */
				_SGI_exports.MGLSGIResizeBuffers(dc->wm.gendc.SGISurf,
					dc->memdc->mi.xRes+1,dc->memdc->mi.yRes+1,
					dc->memdc->mi.bytesPerLine,
					dc->memdc->surface,dc->memdc->surface);
				}
			}
		}
#else
	MGLMesaResizeBuffers();
#endif
}

/****************************************************************************
DESCRIPTION:
Returns the address of an OpenGL extension function.

HEADER:
mgraph.h

PARAMETERS:
dc	- MGL device context to get the OpenGL extension function for

RETURNS:
Address of the specified extension function, NULL if not available.

REMARKS:
This function returns the address of an OpenGL extension function if
that extension is supported by the OpenGL implementation. Each OpenGL
implementation may export a number of OpenGL extension that may not be
supported by other OpenGL implementations, and this function is the
mechanism you can use to obtain the address of those extension functions.

Note that you should first check to see if an extension is available, but
calling the OpenGL function glGetString(GL_EXTENSIONS) to get a list of
all the available extensions. In order to check for a specific extension
by name, you can use the following code:

	ibool checkExtension(const char *name)
	{
		const char *p = (const char *)glGetString(GL_EXTENSIONS);
		while (p = strstr(p, name)) {
			const char *q = p + strlen(name);
			if (*q == ' ' || *q == '\0')
				return GL_TRUE;
			p = q;
			}
		return GL_FALSE;
	}

Note:	It is an error to call this function for an MGL device context that
		does not have an OpenGL rendering context associated with it via
		a call to MGL_glCreateContext.
****************************************************************************/
void * MGLAPI MGL_glGetProcAddress(
	const char *procName)
{
#ifndef	DEBUG_OPENGL
	if (_MGL_cntOpenGL == MGL_GL_MESA)
		return _MESA_exports.MGLMesaGetProcAddress(procName);
	else if (_MGL_cntOpenGL == MGL_GL_SGI)
		return _SGI_exports.MGLSGIGetProcAddress(procName);
	else
		return __MGL_glGetProcAddress(procName);
#else
	return MGLMesaGetProcAddress(procName);
#endif
}

/****************************************************************************
DESCRIPTION:
Sets the palette values for a device context when using OpenGL.

HEADER:
mgraph.h

PARAMETERS:
dc			- Device context to set palette for
pal			- Palette to set
numColors	- Number of colors in the palette
startIndex	- Starting index in the palette

REMARKS:
This functions sets the color palette for an MGL device context when
running in OpenGL rendering modes. This function is identical to the
regular MGL_setPalette function, however if you are running OpenGL you
must use this function instead.

SEE ALSO:
MGL_glRealizePalette, MGL_setPalette
****************************************************************************/
void MGLAPI MGL_glSetPalette(
	MGLDC *dc,
	palette_t *pal,
	int numColors,
	int startIndex)
{
#ifndef	DEBUG_OPENGL
	if (_MGL_cntOpenGL == MGL_GL_MESA) {
		/* Mesa fullscreen and windowed modes */
		_MESA_exports.MGLMesaSetPalette(dc,pal,numColors,startIndex);
		}
	else {
		/* All other windowed and fullscreen modes */
		MGL_setPalette(dc,pal,numColors,startIndex);
		if (dc->memdc)
			MGL_setPalette(dc->memdc,pal,numColors,startIndex);
		}
#else
	MGLMesaSetPalette(dc,pal,numColors,startIndex);
#endif
}

/****************************************************************************
DESCRIPTION:
Realizes the hardware palette for a device context when using OpenGL.

HEADER:
mgraph.h

PARAMETERS:
dc			- Device context to set palette for
numColors	- Number of colors in the palette
startIndex	- Starting index in the palette
vSync		- True if routine should sync to vertical retrace, false if not.

REMARKS:
This function realizes the hardware palette associated with a display device
context. Calls to MGL_glSetPalette only update the palette values in the
color palette for the device context structure, but do not actually program
the hardware palette for display device contexts in 4 and 8 bits per pixel
modes. In order to program the hardware palette you must call this routine.

When the hardware palette is realized, you normally need to sync to the
vertical retrace to ensure that the palette values are programmed without
the onset of snow (see MGL_setPaletteSnowLevel to adjust the number of
colors programmed per retrace period). If however you wish to perform double
buffered animation and change the hardware color palette at the same time,
you should call this routine immediately after calling either
MGL_setVisualPage or MGL_swapBuffers with the vSync flag set to false.

Note:	This function is identical to the regular MGL_realizePalette
		function, however if you are running OpenGL you must use this
		function instead.

SEE ALSO:
MGL_glSetPalette, MGL_realizePalette
****************************************************************************/
void MGLAPI MGL_glRealizePalette(
	MGLDC *dc,
	int numColors,
	int startIndex,
	int waitVRT)
{
#ifndef	DEBUG_OPENGL
	if (_MGL_cntOpenGL == MGL_GL_MESA) {
		/* Mesa fullscreen modes */
		_MESA_exports.MGLMesaRealizePalette(dc,numColors,startIndex,waitVRT);
		}
	else {
		/* All other windowed and fullscreen modes */
		MGL_realizePalette(dc,numColors,startIndex,waitVRT);
		if (dc->memdc)
			MGL_realizePalette(dc->memdc,numColors,startIndex,waitVRT);
		}
#else
	MGLMesaRealizePalette(dc,numColors,startIndex,waitVRT);
#endif
}

/****************************************************************************
DESCRIPTION:
Enables MGL 2D drawing functions using an OpenGL surface

HEADER:
mgraph.h

PARAMETERS:
dc			- Device context to enable 2D MGL functions for

REMARKS:
This function enable support for MGL 2D functions for hardware
accelerated OpenGL surfaces. In order to be able to draw to a hardware OpenGL
surface using 2D OpenGL drawing functions, the MGL needs to re-program the
state of the OpenGL rendering context such that it maps to a 2D integer
coordinate system rather than the 2D or 3D coordinate system the user
application code is using for OpenGL drawing. Hence this function saves the
the state of the OpenGL rendering context so it can be restored with
MGL_glDisableMGLFuncs, and sets the rendring context into a state that is
suitable for 2D drawing via internal functions in the MGL.

Although it is not necessary to call this function to use the MGL 2D drawing
functions (such as MGL_fillRect, MGL_bitBlt, MGL_drawStr etc), the process of
saving and restoring the rendering context is expensive. Internally the
MGL will call this function before doing any 2D operations, which means the
state is saved and restore for every 2D MGL function that is called. Hence
if you will be calling a number of 2D MGL drawing functions, you can bracket
your code with MGL_glEnableMGLFuncs and MGL_glDisableMGLFuncs to ensure that
the MGL only saves and restores the rendering context once for all MGL 2D
drawing functions you call.

SEE ALSO:
MGL_glDisableMGLFuncs
****************************************************************************/
void MGLAPI MGL_glEnableMGLFuncs(
	MGLDC *dc)
{
	uchar	R,G,B;

#ifndef	DEBUG_OPENGL
	if (!OpenGLLoaded)
		return;
#endif
	if (dc == _MGL_dcPtr)
    	dc = &_MGL_dc;
	if (!dc->mglEnabled) {
        /* Save current bitmap tranfer modes */
        glGetIntegerv(GL_UNPACK_SWAP_BYTES, &dc->glSwapBytes);
        glGetIntegerv(GL_UNPACK_LSB_FIRST, &dc->glLsbFirst);
        glGetIntegerv(GL_UNPACK_ROW_LENGTH, &dc->glRowLength);
        glGetIntegerv(GL_UNPACK_SKIP_ROWS, &dc->glSkipRows);
        glGetIntegerv(GL_UNPACK_SKIP_PIXELS, &dc->glSkipPixels);
        glGetIntegerv(GL_UNPACK_ALIGNMENT, &dc->glAlignment);
        dc->glLighting = glIsEnabled(GL_LIGHTING);
        dc->glDepthTest = glIsEnabled(GL_DEPTH_TEST);
        glGetIntegerv(GL_VIEWPORT,dc->glViewport);
        glGetDoublev(GL_CURRENT_COLOR,dc->glCntColor);

        /* Set the bitmap tranfer modes */
        glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
        glPixelStorei(GL_UNPACK_LSB_FIRST, GL_TRUE);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
        glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        /* Disable dc->glLighting and depth testing */
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);

        /* Set the current raster color */
		MGL_unpackColorFast(&dc->pf,dc->a.color,R,G,B);
		glColor3ub(R,G,B);

        /* Set the dc->glViewport and projection to othographic 2D mode */
        glViewport(0, 0, _MGL_dc.mi.xRes+1, _MGL_dc.mi.yRes+1);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glScalef(1,-1,1);
        glOrtho(0.0, _MGL_dc.mi.xRes, 0.0, _MGL_dc.mi.yRes, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        /* Flag that we have disabled OpenGL calls for the moment */
        dc->mglEnabled = true;
        }
}

/****************************************************************************
DESCRIPTION:
Disables MGL 2D drawing functions using an OpenGL surface

HEADER:
mgraph.h

PARAMETERS:
dc			- Device context to disable 2D MGL functions for

REMARKS:
This function disable support for MGL 2D functions for hardware
accelerated OpenGL surfaces. This does the opposite of the MGL_glEnableMGLFuncs
function.

SEE ALSO:
MGL_glEnableMGLFuncs
****************************************************************************/
void MGLAPI MGL_glDisableMGLFuncs(MGLDC *dc)
{
#ifndef	DEBUG_OPENGL
	if (!OpenGLLoaded)
		return;
#endif
	if (dc == _MGL_dcPtr)
    	dc = &_MGL_dc;
	if (dc->mglEnabled) {
        /* Restore dc->glViewport, modelview and projection matrices */
        glViewport(dc->glViewport[0], dc->glViewport[1], dc->glViewport[2], dc->glViewport[3]);
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        /* Restore dc->glLighting and depth testing */
        if (dc->glLighting)
            glEnable(GL_LIGHTING);
        if (dc->glDepthTest)
            glEnable(GL_DEPTH_TEST);

        /* Restore the current color */
     	glColor4dv(dc->glCntColor);

        /* Restore bitmap transfer modes */
        glPixelStorei(GL_UNPACK_SWAP_BYTES, dc->glSwapBytes);
        glPixelStorei(GL_UNPACK_LSB_FIRST, dc->glLsbFirst);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, dc->glRowLength);
        glPixelStorei(GL_UNPACK_SKIP_ROWS, dc->glSkipRows);
        glPixelStorei(GL_UNPACK_SKIP_PIXELS, dc->glSkipPixels);
        glPixelStorei(GL_UNPACK_ALIGNMENT, dc->glAlignment);

        /* Flag that we have re-enabled OpenGL calls */
        dc->mglEnabled = false;
		}
}

#ifndef	DEBUG_OPENGL
/****************************************************************************
REMARKS:
This function allows us to capture all calls to glDrawBuffer so that we can
keep track of which buffer is currently active in the MGL, allowing our
software rendering code to draw to the correct active buffer when using
OpenGL.
{secret}
****************************************************************************/
void APIENTRY glDrawBuffer(
	GLenum mode)
{
	if (_MGL_cntOpenGL == MGL_GL_SGI) {
		if (mode == GL_LEFT) {
			/* Switch to left buffer using existing front/back selection */
			if (DC.glDrawBuffer == GL_FRONT)
				mode = GL_FRONT_LEFT;
			else
				mode = GL_BACK_LEFT;
			}
		else if (mode == GL_RIGHT) {
			/* Switch to right buffer using existing front/back selection */
			if (DC.glDrawBuffer == GL_FRONT)
				mode = GL_FRONT_RIGHT;
			else
				mode = GL_BACK_RIGHT;
			}
		if (mode == GL_FRONT)
			MGL_setActivePage(&DC,DC.frontBuffer);
		else if (mode == GL_BACK)
			MGL_setActivePage(&DC,DC.backBuffer);
		else if (mode == GL_FRONT_LEFT)
			MGL_setActivePage(&DC,DC.frontBuffer | MGL_LEFT_BUFFER);
		else if (mode == GL_FRONT_RIGHT)
			MGL_setActivePage(&DC,DC.frontBuffer | MGL_RIGHT_BUFFER);
		else if (mode == GL_BACK_LEFT)
			MGL_setActivePage(&DC,DC.backBuffer | MGL_LEFT_BUFFER);
		else if (mode == GL_BACK_RIGHT)
			MGL_setActivePage(&DC,DC.backBuffer | MGL_RIGHT_BUFFER);
		}
	DC.glDrawBuffer = mode;
	_MGL_glFuncs.glDrawBuffer(mode);
}
#endif
