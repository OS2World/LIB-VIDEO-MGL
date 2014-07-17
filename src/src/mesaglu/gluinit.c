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
* Description:	This file contains the code specific to initialising the
*				SciTech MGL Binary Portable DLL.
*
****************************************************************************/

#include "GL/mglmesa.h"
#include <stdio.h>
#include <string.h>

/*------------------------- Global Variables ------------------------------*/

PM_imports _VARAPI 		_PM_imports;
MGL_glFuncs _VARAPI		_MGL_glFuncs;

/*------------------------- Implementation --------------------------------*/

/****************************************************************************
REMARKS:
Fatal error handler called when a non-imported function is called by the
driver. We leave this to a runtime error so that older applications and
shell drivers will work with newer bpd drivers provided no newer functions
are required by the driver itself. If they are, the application or shell
driver needs to be recompiled.
****************************************************************************/
static void _PM_fatalErrorHandler(void)
{
	_PM_imports.PM_fatalError("Unsupported PM_imports import function called! Please re-compile!\n");
}

/****************************************************************************
REMARKS:
Fatal error handler called when a non-imported function is called by the
driver. We leave this to a runtime error so that older applications and
shell drivers will work with newer bpd drivers provided no newer functions
are required by the driver itself. If they are, the application or shell
driver needs to be recompiled.
****************************************************************************/
static void _GL_fatalErrorHandler(void)
{
	_PM_imports.PM_fatalError("Unsupported MGL_glFuncs import function called! Please re-compile!\n");
}

/****************************************************************************
PARAMETERS:

RETURNS:
Pointer to exported function list

REMARKS:
This function initialises the GL image library and returns the list of
loader library exported functions.
{secret}
****************************************************************************/
MGL_gluFuncs * _CEXPORT MESAGLU_initLibrary(
	PM_imports *pmImp,
	MGL_glFuncs *glFuncs)
{
	static MGL_gluFuncs _MGL_gluFuncs = {
		sizeof(MGL_gluFuncs),
        gluBeginCurve,
        gluBeginPolygon,
        gluBeginSurface,
        gluBeginTrim,
		gluBuild1DMipmaps,
		gluBuild2DMipmaps,
        gluCylinder,
        gluDeleteNurbsRenderer,
		gluDeleteQuadric,
        gluDeleteTess,
        gluDisk,
		gluEndCurve,
        gluEndPolygon,
		gluEndSurface,
        gluEndTrim,
        gluErrorString,
        gluGetNurbsProperty,
		gluGetString,
		gluGetTessProperty,
        gluLoadSamplingMatrices,
        gluLookAt,
        gluNewNurbsRenderer,
        gluNewQuadric,
        gluNewTess,
        gluNextContour,
        gluNurbsCallback,
		gluNurbsCurve,
        gluNurbsProperty,
        gluNurbsSurface,
        gluOrtho2D,
        gluPartialDisk,
        gluPerspective,
        gluPickMatrix,
        gluProject,
        gluPwlCurve,
        gluQuadricCallback,
        gluQuadricDrawStyle,
        gluQuadricNormals,
        gluQuadricOrientation,
        gluQuadricTexture,
        gluScaleImage,
        gluSphere,
		gluTessBeginContour,
		gluTessBeginPolygon,
		gluTessCallback,
		gluTessEndContour,
		gluTessEndPolygon,
		gluTessNormal,
		gluTessProperty,
		gluTessVertex,
        gluUnProject,
		};
	int		i,max;
	ulong 	*p;

	// Initialize all default imports to point to fatal error handler
	// for upwards compatibility.
	max = sizeof(_PM_imports)/sizeof(MESA_initLibrary_t);
	for (i = 0,p = (ulong*)&_PM_imports; i < max; i++)
		*p++ = (ulong)_PM_fatalErrorHandler;
	max = sizeof(_MGL_glFuncs)/sizeof(MESA_initLibrary_t);
	for (i = 0,p = (ulong*)&_MGL_glFuncs; i < max; i++)
		*p++ = (ulong)_GL_fatalErrorHandler;

	// Now copy all our imported functions
	memcpy(&_PM_imports,pmImp,MIN(sizeof(_PM_imports),pmImp->dwSize));
	memcpy(&_MGL_glFuncs,glFuncs,MIN(sizeof(_MGL_glFuncs),glFuncs->dwSize));
	return &_MGL_gluFuncs;
}
