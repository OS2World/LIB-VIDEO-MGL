/****************************************************************************
*
*						MegaGraph Graphics Library
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
* Description:  Internal header file for Silicon Graphics OpenGL for Windows
*				bindings for the MGL. These are specific bindings to get
*				the SGI OpenGL libraries to render directly to MGL surfaces.
*
****************************************************************************/

#ifndef	__GL_MGLSGI_H
#define	__GL_MGLSGI_H

#include "GL/gl.h"
#include "GL/glu.h"
#include "mgraph.h"
#include "pmapi.h"

/*---------------------- Macro and type definitions -----------------------*/

/* Type pointers to internal MGLSGI objects */

typedef void *MGLSGISurface;
typedef void *MGLSGIContext;

/* The following structure is used to communicate with SGI OpenGL to let
 * it know about our surface information for fullscreen modes.
 */
#pragma pack(1)
typedef struct {
	void            *frontSurface;  /* Pointer to start of front buffer         */
	void            *backSurface;   /* Pointer to start of back buffer          */
	int             width;          /* Surface width in pixels                  */
	int             height;         /* Surface height in pixels                 */
	int             bytesPerLine;   /* Scanline pitch for the surface           */
	int             bitsPerPixel;   /* Pixel bits for mode (15 = 5:5:5 HiColor) */
	pixel_format_t  pf;             /* Pixel format information                 */
	palette_t       *pal;       	/* Hardware palette to be used              */

	/* Buffer arbitration functions */
	void            (MGLAPIP BeginDirectAccess)(void);
	void            (MGLAPIP EndDirectAccess)(void);
	void            (MGLAPIP Fill)(int x,int y,int w,int h,uint color);
	} MGLSGISurfaceInfo;

/* Exports for all MGLSGI specific functions */

typedef struct {
	long	dwSize;
	void 	(MGLAPIP MGLSGIChooseVisual)(MGLSGISurfaceInfo *info, MGLVisual *visual);
	ibool 	(MGLAPIP MGLSGICheckVisual)(MGLSGISurfaceInfo *info, MGLVisual *visual);
	MGLSGIContext (MGLAPIP MGLSGICreateContext)(MGLSGISurfaceInfo *info, MGLVisual *visual);
	MGLSGISurface (MGLAPIP MGLSGICreateSurface)(MGLSGISurfaceInfo *info, MGLVisual *visual);
	ibool 	(MGLAPIP MGLSGIDeleteSurface)(MGLSGISurface surface);
	ibool 	(MGLAPIP MGLSGIDeleteContext)(MGLSGIContext ctx);
	ibool 	(MGLAPIP MGLSGIMakeCurrent)(MGLSGIContext ctx, MGLSGISurface surface);
	void 	(MGLAPIP MGLSGISetSurfacePtr)(MGLSGISurface surface,void *frontSurface, void *backSurface);
	void 	(MGLAPIP MGLSGIResizeBuffers)(MGLSGISurface surface,int width,int height,int byteWidth,void *frontSurface,void *backSurface);
	void *	(MGLAPIP MGLSGIGetProcAddress)(const char *procName);
	} SGI_exports;
#pragma pack()

typedef MGL_glFuncs * (PMAPIP SGI_initLibrary_t)(PM_imports *pmImp,SGI_exports *glExp);
typedef MGL_gluFuncs * (PMAPIP SGIGLU_initLibrary_t)(PM_imports *pmImp,MGL_glFuncs *glFuncs);

/*------------------------- Function Prototypes ---------------------------*/

#ifdef	__cplusplus
extern "C" {			/* Use "C" linkage when in C++ mode	*/
#endif

void 	MGLAPI MGLSGIChooseVisual(MGLSGISurfaceInfo *info, MGLVisual *visual);
ibool 	MGLAPI MGLSGICheckVisual(MGLSGISurfaceInfo *info, MGLVisual *visual);
MGLSGIContext MGLAPI MGLSGICreateContext(MGLSGISurfaceInfo *info, MGLVisual *visual);
MGLSGISurface MGLAPI MGLSGICreateSurface(MGLSGISurfaceInfo *info, MGLVisual *visual);
ibool 	MGLAPI MGLSGIDeleteSurface(MGLSGISurface surface);
ibool 	MGLAPI MGLSGIDeleteContext(MGLSGIContext ctx);
ibool 	MGLAPI MGLSGIMakeCurrent(MGLSGIContext ctx, MGLSGISurface surface);
void 	MGLAPI MGLSGISetSurfacePtr(MGLSGISurface surface,void *frontSurface, void *backSurface);
void 	MGLAPI MGLSGIResizeBuffers(MGLSGISurface surface,int width,int height,int byteWidth,void *frontSurface,void *backSurface);
void *	MGLAPI MGLSGIGetProcAddress(const char *procName);

#ifdef	__cplusplus
}						/* End of "C" linkage for C++	*/
#endif	/* __cplusplus */

#endif	/* __GL_MGLSGI_H */
