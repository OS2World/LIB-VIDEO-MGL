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
* Description:  Win32 specific OpenGL implementation.
*
****************************************************************************/

/*--------------------------- Global Variables ----------------------------*/

#define	MSGL_DLL						"opengl32.dll"
#define	MSGLU_DLL						"glu32.dll"
#define	GDI32_DLL						"gdi32.dll"
#define _PFD_GENERIC_ACCELERATED         0x00001000

static	HINSTANCE	hInstOpenGL = NULL;	/* Handle to loaded OpenGL DLL	*/
static 	HINSTANCE	hInstGLU = NULL;	/* Handle to loaded GLU DLL		*/
static	ibool		hwOpenGL = -1;		/* Is OpenGL hardware accel?	*/

/* OpenGL wgl Win32 binding functions */
static HGLRC 	(APIENTRYP pwglCreateContext)(HDC);
static BOOL  	(APIENTRYP pwglDeleteContext)(HGLRC);
static PROC  	(APIENTRYP pwglGetProcAddress)(LPCSTR);
static BOOL  	(APIENTRYP pwglMakeCurrent)(HDC, HGLRC);

#define	wglCreateContext(dc)	pwglCreateContext(dc)
#define	wglDeleteContext(rc)	pwglDeleteContext(rc)
#define	wglGetProcAddress(s)	pwglGetProcAddress(s)
#define	wglMakeCurrent(dc, rc)	pwglMakeCurrent(dc, rc)

#define	WGL_PROC_ADDRESS(name)	\
		p##name = SafeProcAddress(hInstOpenGL,#name)

/* Tables to build 8-bit OpenGL compatible color palette */

static uchar threeto8[8] = {
	0, 0111>>1, 0222>>1, 0333>>1, 0444>>1, 0555>>1, 0666>>1, 0377
	};
static uchar twoto8[4] = { 0, 0x55, 0xAA, 0xFF };
static uchar oneto8[2] = { 0, 255 };
static int defaultOverride[13] = {
	0, 3, 24, 27, 64, 67, 88, 173, 181, 236, 247, 164, 91
	};
static PALETTEENTRY defaultPal[20] = {
	{ 0,   0,   0,    0 },
	{ 0x80,0,   0,    0 },
	{ 0,   0x80,0,    0 },
	{ 0x80,0x80,0,    0 },
	{ 0,   0,   0x80, 0 },
	{ 0x80,0,   0x80, 0 },
	{ 0,   0x80,0x80, 0 },
	{ 0xC0,0xC0,0xC0, 0 },

	{ 192, 220, 192,  0 },
	{ 166, 202, 240,  0 },
	{ 255, 251, 240,  0 },
	{ 160, 160, 164,  0 },

	{ 0x80,0x80,0x80, 0 },
	{ 0xFF,0,   0,    0 },
	{ 0,   0xFF,0,    0 },
	{ 0xFF,0xFF,0,    0 },
	{ 0,   0,   0xFF, 0 },
	{ 0xFF,0,   0xFF, 0 },
	{ 0,   0xFF,0xFF, 0 },
	{ 0xFF,0xFF,0xFF, 0 }
	};

/*------------------------- Implementation --------------------------------*/

/****************************************************************************
RETURNS:
True if the DC is an OpenGL capable device context.
****************************************************************************/
ibool _MGL_isOpenGLDC(
	MGLDC *dc)
{
	return dc->deviceType == MGL_WINDOWED_DEVICE ||
		   dc->deviceType == MGL_OPENGL_DEVICE ||
		   dc->deviceType == MGL_FS_OPENGL_DEVICE;
}

/****************************************************************************
RETURNS:
Pointer to loaded procedure address

REMARKS:
Attempts to get the address of a specified procedure, and if it failes we
simply bomb out with an error message.
****************************************************************************/
static void *SafeProcAddress(
	HINSTANCE hInst,
	char *name)
{
	char	msg[80];

	FARPROC	pFunc = GetProcAddress(hInst,name);
	if (!pFunc) {
		sprintf(msg,"GetProcAddress(\"%s\") failed!", name);
		MGL_fatalError(msg);
		}
	return pFunc;
}

/****************************************************************************
REMARKS:
Unloads any currently loaded platform specific OpenGL library components.
****************************************************************************/
static void __MGL_glUnloadOpenGL(void)
{
	if (hInstGLU) {
		FreeLibrary(hInstGLU);
		hInstGLU = NULL;
		}
	if (hInstOpenGL) {
		FreeLibrary(hInstOpenGL);
		hInstOpenGL = NULL;
		}
}

/****************************************************************************
PARAMETERS:
GLName	- Place to store full path to OpenGL library
GLUName	- Place to store full path to OpenGL GLU library

RETURNS:
True if the OpenGL libraries are found, false if not.

REMARKS:
Attempts to load the Microsoft OpenGL libraries from disk. We keep them
loaded so that we only do the load and unload as little as possible.
****************************************************************************/
static ibool LoadOpenGL(
	char *GLName,
	char *GLUName)
{
	if (hInstOpenGL && hInstGLU)
		return true;
	if (((hInstOpenGL = LoadLibrary(GLName)) != NULL) &&
		((hInstGLU = LoadLibrary(GLUName)) != NULL)) {
		WGL_PROC_ADDRESS(wglCreateContext);
		WGL_PROC_ADDRESS(wglDeleteContext);
		WGL_PROC_ADDRESS(wglGetProcAddress);
		WGL_PROC_ADDRESS(wglMakeCurrent);
		RESET_DEFAULT_CW();
		return true;
		}
	RESET_DEFAULT_CW();
	__MGL_glUnloadOpenGL();
	return false;
}

/****************************************************************************
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
static ibool __MGL_glHaveHWOpenGL(void)
{
	HDC	hdc;
	int (WINAPI *Describe)(HDC, int, UINT, LPPIXELFORMATDESCRIPTOR);
	int i,count;

	if (hwOpenGL != -1)
		return hwOpenGL;
	hwOpenGL = false;
	if (LoadOpenGL(MSGL_DLL,MSGLU_DLL) && (Describe = (void*)GetProcAddress(hInstOpenGL,"wglDescribePixelFormat")) != NULL) {
		hdc = GetDC(NULL);
		count = Describe(hdc, 0, 0, NULL);
		for (i = 1; i <= count; i++) {
			PIXELFORMATDESCRIPTOR pfd;
			if (Describe(hdc, i, sizeof(pfd), &pfd)) {
				BOOL genericFormat = (pfd.dwFlags & PFD_GENERIC_FORMAT) != 0;
				BOOL genericAccel = (pfd.dwFlags & _PFD_GENERIC_ACCELERATED) != 0;
				/* Determine hardware driver status by the combination of these
				 * two flags. Both FALSE means ICD, both TRUE means MCD.
				 */
				if (genericFormat == genericAccel) {
					hwOpenGL = true;
					break;
					}
				}
			}
		ReleaseDC(NULL, hdc);
		}
	return hwOpenGL;
}

/****************************************************************************
PARAMETERS:
GLName	- Place to store full path to OpenGL library
GLUName	- Place to store full path to OpenGL GLU library

RETURNS:
True if the OpenGL libraries are found, false if not.

REMARKS:
Attempts to find the platform specific OpenGL library, and if found
fills in the names of the OpenGL and GLU libraries. We actually open up
and load the default OpenGL DLL's in here, to avoid unloading them and
re-loading them again later.
****************************************************************************/
static ibool __MGL_glFindDefaultOpenGL(
	char *GLName,
	char *GLUName)
{
	strcpy(GLName,MSGL_DLL);
	strcpy(GLUName,MSGLU_DLL);
	return LoadOpenGL(GLName,GLUName);
}

/****************************************************************************
PARAMETERS:
GLName	- Place to store full path to OpenGL library
GLUName	- Place to store full path to OpenGL GLU library

RETURNS:
True if the OpenGL libraries are found, false if not.

REMARKS:
Attempts to find the Windows specific GLDirect game library, and if found
fills in the names of the OpenGL and GLU libraries.
****************************************************************************/
static ibool __MGL_glFindGLDirect(
	char *GLName,
	char *GLUName)
{
	/* No GLDirect OpenGL on Win32 yet! */
	(void)GLName;
	(void)GLUName;
	return false;
}

/****************************************************************************
PARAMETERS:
GLName	- Place to store full path to OpenGL library
GLUName	- Place to store full path to OpenGL GLU library

RETURNS:
True if the OpenGL libraries are found, false if not.

REMARKS:
Attempts to find the Windows specific GLDirect CAD library, and if found
fills in the names of the OpenGL and GLU libraries.
****************************************************************************/
static ibool __MGL_glFindGLDirectCAD(
	char *GLName,
	char *GLUName)
{
	/* No GLDirect OpenGL on Win32 yet! */
	(void)GLName;
	(void)GLUName;
	return false;
}

/****************************************************************************
PARAMETERS:
GLName	- Place to store full path to OpenGL library
GLUName	- Place to store full path to OpenGL GLU library

REMARKS:
Attempts to load and initialise the platform specific default OpenGL
libraries.
****************************************************************************/
static void __MGL_loadDefaultOpenGL(
	const char *GLName,
	const char *GLUName)
{
	if (!hInstOpenGL) {
		if (!LoadOpenGL(GLName,GLUName))
			PM_fatalError("Unable to load OpenGL!");
		}
}

/****************************************************************************
PARAMETERS:
GLName	- Place to store full path to OpenGL library
GLUName	- Place to store full path to OpenGL GLU library

REMARKS:
Attempts to load and initialise the Windows specific GLDirect libraries.
****************************************************************************/
static void __MGL_loadGLDirect(
	const char *GLName,
	const char *GLUName)
{
	/* No GLDirect on Win32 yet! */
	(void)GLName;
	(void)GLUName;
}

/****************************************************************************
PARAMETERS:
GLName	- Place to store full path to OpenGL library
GLUName	- Place to store full path to OpenGL GLU library

REMARKS:
Attempts to load and initialise the Windows specific GLDirect CAD libraries.
****************************************************************************/
static void __MGL_loadGLDirectCAD(
	const char *GLName,
	const char *GLUName)
{
	/* No GLDirect on Win32 yet! */
	(void)GLName;
	(void)GLUName;
}

/****************************************************************************
PARAMETERS:
dc		- MGL device context for visual
visual	- MGLVisual to convert to window pixel format
pf		- Windows pixel format info block to fill in

REMARKS:
Converts an MGLVisual pixel format block to the Win32 format so that we can
call ChoosePixelFormat/SetPixelFormat.
****************************************************************************/
static void VisualToPF(
	MGLDC *dc,
	MGLVisual *visual,
	PIXELFORMATDESCRIPTOR *pf)
{
    /* Initialize the default values */
	memset(pf, 0, sizeof(*pf));
	pf->nSize = sizeof(*pf);
	pf->nVersion = 1;
	pf->dwFlags = PFD_SUPPORT_OPENGL;
	pf->iPixelType = PFD_TYPE_RGBA;
	pf->cColorBits = dc->mi.bitsPerPixel;
	if (pf->cColorBits == 15)
		pf->cColorBits = 16;
	pf->iLayerType = PFD_MAIN_PLANE;

    /* Fill in information from passed in visual */ 
	if (dc->mi.bitsPerPixel <= 8 && !visual->rgb_flag)
		pf->iPixelType = PFD_TYPE_COLORINDEX;
	if (visual->db_flag && !MGL_isMemoryDC(dc))
		pf->dwFlags |= PFD_DOUBLEBUFFER;
	if (visual->stereo_flag && !MGL_isMemoryDC(dc))
		pf->dwFlags |= PFD_STEREO;
	if (visual->accum_size)
		pf->cAccumBits = visual->accum_size;
	if (visual->alpha_flag)
		pf->cAlphaBits = 8;
	if (visual->depth_size)
		pf->cDepthBits = visual->depth_size;
	if (visual->stencil_size)
		pf->cStencilBits = visual->stencil_size;
	pf->dwFlags &= ~PFD_NEED_PALETTE;
	if (dc->mi.bitsPerPixel == 8)
		pf->dwFlags |= PFD_NEED_PALETTE;

	/* Force OpenGL to render to a bitmap in software when not using hardware
	 * acceleration.
	 */
	pf->dwFlags &= ~(PFD_DRAW_TO_WINDOW | PFD_DRAW_TO_BITMAP | PFD_SUPPORT_GDI);
	if (!_MGL_isOpenGLDC(dc))
		pf->dwFlags |= PFD_DRAW_TO_BITMAP | PFD_GENERIC_FORMAT;
	else
		pf->dwFlags |= PFD_DRAW_TO_WINDOW;
}

/****************************************************************************
PARAMETERS:
dc		- MGL device context for visual
visual	- MGLVisual to convert to window pixel format
pf		- Windows pixel format info block to fill in

REMARKS:
Converts a Win32 pixel format to an MGLVisual.
****************************************************************************/
static void PFToVisual(
	MGLDC *dc,
	MGLVisual *visual,
	PIXELFORMATDESCRIPTOR *pf)
{
	memset(visual,0,sizeof(*visual));
	visual->rgb_flag = (pf->iPixelType == PFD_TYPE_RGBA);
	visual->alpha_flag = (pf->cAlphaBits != 0);
	visual->db_flag = (pf->dwFlags & PFD_DOUBLEBUFFER) != 0;
	visual->stereo_flag = (pf->dwFlags & PFD_STEREO) != 0;
	visual->depth_size = pf->cDepthBits;
	visual->stencil_size = pf->cStencilBits;
	visual->accum_size = pf->cAccumBits;
}

/****************************************************************************
PARAMETERS:
dc		- MGL device context

REMARKS:
Returns the Windows HDC for the device context. For display DC's we create
a memory back buffer (a DIB section) which we can use for creating an OpenGL
device context.
****************************************************************************/
static HDC GetOpenGL_HDC(
	MGLDC *dc)
{
	if (dc->deviceType == MGL_DISPLAY_DEVICE) {
		/* We are running in a fullscreen mode with Microsoft OpenGL, so we
		 * need to create a memory DC back buffer for our rendering. We do
		 * this so that we can also gain direct access to the framebuffer
		 * even in a window, so the MGL becomes more tightly integrated with
		 * the OpenGL libraries. It also gives us the flexibility of working
		 * on any system, even without DirectDraw or DCI support present (ie:
		 * legacy NT systems).
		 */
		if (!dc->memdc) {
			MGL_makeCurrentDC(NULL);
			dc->memdc = MGL_createMemoryDC(dc->mi.xRes+1,dc->mi.yRes+1,dc->mi.bitsPerPixel,&dc->pf);
			if (!dc->memdc)
				return NULL;
			}
		return MGL_getWinDC(dc->memdc);
		}
	return MGL_getWinDC(dc);
}

/****************************************************************************
PARAMETERS:
dc		- MGL Device context
visual	- Place to store the visual information

REMARKS:
Implements the glChooseVisual function for the system default OpenGL
implementation.
****************************************************************************/
static ibool __MGL_glChooseVisual(
	MGLDC *dc,
	MGLVisual *visual)
{
	PIXELFORMATDESCRIPTOR	pf;
	HDC						hdc = GetOpenGL_HDC(dc);

	if (!hdc)
		return false;
	VisualToPF(dc,visual,&pf);
	if (dc->memdc) {
		pf.dwFlags &= ~(PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW);
		pf.dwFlags |= PFD_DRAW_TO_BITMAP | PFD_GENERIC_FORMAT;
		}
	if (ChoosePixelFormat(hdc, &pf) == 0) {
		/* If this fails, we don't even have software support for the mode */
		pf.dwFlags |= PFD_GENERIC_FORMAT;
		if (ChoosePixelFormat(hdc, &pf) == 0)
			return false;
		}
	if (visual->db_flag)
		pf.dwFlags |= PFD_DOUBLEBUFFER;
	if (visual->stereo_flag)
		pf.dwFlags |= PFD_STEREO;
	PFToVisual(dc,visual,&pf);
	return true;
}

/****************************************************************************
PARAMETERS:
dc		- MGL Device context
visual	- Place to store the visual information

REMARKS:
Implements the glSetVisual function for the system default OpenGL
implementation.
****************************************************************************/
static ibool __MGL_glSetVisual(
	MGLDC *dc,
	MGLVisual *visual)
{
	PIXELFORMATDESCRIPTOR	pf;
	HDC						hdc = GetOpenGL_HDC(dc);
	int						pixelFormat;

	if ((MGL_isDisplayDC(dc) || MGL_isWindowedDC(dc)) && !visual->db_flag)
		return false;
	VisualToPF(dc,visual,&pf);
	if (dc->memdc) {
		pf.dwFlags &= ~(PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW);
		pf.dwFlags |= PFD_DRAW_TO_BITMAP | PFD_GENERIC_FORMAT;
		}
	if ((pixelFormat = ChoosePixelFormat(hdc, &pf)) == 0) {
		/* If this fails, we don't even have software support for the mode */
		pf.dwFlags |= PFD_GENERIC_FORMAT;
		if ((pixelFormat = ChoosePixelFormat(hdc, &pf)) == 0)
			return false;
		}
	if (SetPixelFormat(hdc, pixelFormat, &pf) == false)
		return false;
	if (visual->db_flag)
		pf.dwFlags |= PFD_DOUBLEBUFFER;
	if (visual->stereo_flag)
		pf.dwFlags |= PFD_STEREO;
	PFToVisual(dc,&dc->cntVis,&pf);
	return true;
}

/****************************************************************************
REMARKS:
Creates an RGB palette components for the 8bpp RGB palette
****************************************************************************/
static uchar ComponentFromIndex(
	int i,
	uint nbits,
	uint shift)
{
	uchar val = (uchar)(i >> shift);
	switch (nbits) {
		case 1:	return oneto8[val & 0x1];
		case 2:	return twoto8[val & 0x3];
		case 3:	return threeto8[val & 0x7];
		}
	return 0;
}

/****************************************************************************
PARAMETERS:
dc	- MGL Device context
pal	- Place to store the RGB palette

REMARKS:
Creates an RGB palette for the default system OpenGL driver for running in
8bpp display modes.
****************************************************************************/
static void __MGL_glCreateRGBPalette(
	MGLDC *dc,
	palette_t *pal)
{
	int     i;

    if (_MGL_cntOpenGL == MGL_GL_SGI) {
    	for (i = 0; i < 256; i++) {
		    pal[i].red = ComponentFromIndex(i, 3, 5);
		    pal[i].green = ComponentFromIndex(i, 3, 2);
		    pal[i].blue = ComponentFromIndex(i, 2, 0);
		    }
        }
    else {
        int                     n;
    	PIXELFORMATDESCRIPTOR	pfd;
	    HDC						hdc = GetOpenGL_HDC(dc);

    	n = GetPixelFormat(hdc);
    	DescribePixelFormat(hdc, n, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    	/* Create the basic palette */
    	for (i = 0; i < 256; i++) {
		    pal[i].red = ComponentFromIndex(i, pfd.cRedBits, pfd.cRedShift);
		    pal[i].green = ComponentFromIndex(i, pfd.cGreenBits, pfd.cGreenShift);
		    pal[i].blue = ComponentFromIndex(i, pfd.cBlueBits, pfd.cBlueShift);
		    }
        }

	/* Override the basic colors in a window to fix up the GDI palette */
	if (!MGL_isDisplayDC(dc)) {
        HDC hdc = GetDC(NULL);
		if (GetSystemPaletteUse(hdc) != SYSPAL_NOSTATIC) {
			for (i = 1 ; i <= 12 ; i++) {
				pal[defaultOverride[i]].red = defaultPal[i].peRed;
				pal[defaultOverride[i]].green = defaultPal[i].peGreen;
				pal[defaultOverride[i]].blue = defaultPal[i].peBlue;
				}
			}
        ReleaseDC(NULL,hdc);
		}
	else {
		/* For fullscreen modes we always use a static system palette */
		SetSystemPaletteUse(dc->wm.windc.hdc, SYSPAL_NOSTATIC);
		}
}

/****************************************************************************
PARAMETERS:
dc		- MGL Device context
flags	- Context creation flags

REMARKS:
Implements the glCreateContext function for the system default OpenGL
implementation.
****************************************************************************/
static ibool __MGL_glCreateContext(
	MGLDC *dc,
	int flags)
{
	dc->wm.gendc.glrc = wglCreateContext(GetOpenGL_HDC(dc));
	if (!dc->wm.gendc.glrc) {
		if (dc->memdc) {
			MGL_destroyDC(dc->memdc);
			dc->memdc = NULL;
			}
		return false;
		}

	/* Enable hardware double buffering if we have a memory back buffer */
	if (dc->memdc)
		MGL_doubleBuffer(dc);
	return true;
}

/****************************************************************************
PARAMETERS:
dc	- MGL Device context

REMARKS:
Implements the glMakeCurrent function for the system default OpenGL
implementation.
****************************************************************************/
static void __MGL_glMakeCurrent(
	MGLDC *dc)
{
	if (dc) {
		if (dc->memdc)
			MGL_makeCurrentDC(dc->memdc);
		wglMakeCurrent(GetOpenGL_HDC(dc),dc->wm.gendc.glrc);
		MGL_makeCurrentDC(NULL);
		MGL_makeCurrentDC(dc);
		}
	else {
		MGL_makeCurrentDC(NULL);
		}
}

/****************************************************************************
PARAMETERS:
dc	- MGL Device context

REMARKS:
Implements the glDeleteContext function for the system default OpenGL
implementation.
****************************************************************************/
static void __MGL_glDeleteContext(
	MGLDC *dc)
{
	HDC	hdc;

	if (dc->memdc) {
		/* Destroy the software back buffer for fullscreen modes */
		hdc = dc->memdc->wm.gendc.hdc;
		MGL_makeCurrentDC(NULL);
		MGL_destroyDC(dc->memdc);
		dc->memdc = NULL;
		MGL_makeCurrentDC(dc);
		}
	else
		hdc = dc->wm.gendc.hdc;
	if (dc->wm.gendc.glrc) {
		wglMakeCurrent(hdc,NULL);
		wglDeleteContext(dc->wm.gendc.glrc);
		}
	dc->wm.gendc.glrc = NULL;
}

/****************************************************************************
PARAMETERS:
dc		- MGL Device context
waitVRT	- Wait for vertical retrace

REMARKS:
Implements the glSwapBuffers function for the system default OpenGL
implementation.
****************************************************************************/
static void __MGL_glSwapBuffers(
	MGLDC *dc,
	int waitVRT)
{
	if (dc->memdc) {
		/* We have a memory back buffer so blit it to the screen and flip */
		MGL_bitBltCoord(dc,dc->memdc,0,0,dc->mi.xRes+1,dc->mi.yRes+1,0,0,MGL_REPLACE_MODE);
		MGL_swapBuffers(dc,waitVRT);
		}
	else
		SwapBuffers(MGL_getWinDC(dc));
}

/****************************************************************************
PARAMETERS:
dc		- MGL Device context

REMARKS:
Implements the MGL_glResizeBuffers function and does any OS specific stuff
in here if necessary to resize the OpenGL buffers. Normally OpenGL
system implementations hook into the system resize messages automatically
so this usually does not do much for the system OpenGL implementation.
****************************************************************************/
static void __MGL_glResizeBuffers(
	MGLDC *dc)
{
	if (dc->memdc) {
		/* We have a memory back buffer so re-size it */
		MGL_destroyDC(dc->memdc);
		dc->memdc = MGL_createMemoryDC(dc->mi.xRes+1,dc->mi.yRes+1,dc->mi.bitsPerPixel,&dc->pf);
		if (!dc->memdc)
			MGL_fatalError("Out of memory re-sizing OpenGL buffers!");
		if (_MGL_dcPtr == dc)
			DC.memdc = dc->memdc;
		}
}

/****************************************************************************
PARAMETERS:
procName	- Name to get the procedure address for

REMARKS:
Implements the glGetProcAddress function for the system default OpenGL
implementation. This function is used to get the address of OpenGL
extension functions from the system default OpenGL libraries.
****************************************************************************/
static void * __MGL_glGetProcAddress(
	const char *procName)
{
	return wglGetProcAddress(procName);
}
