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
* Environment:	IBM PC (MS DOS)
*
* Description:	Header file for the MGLUNIX bindings for UNIX-like systems.
*
****************************************************************************/

#ifndef	__MGLUNIX_H
#define	__MGLUNIX_H

#ifndef MGLLINUX
#define MGLLINUX
#endif

#ifdef __X11__

#ifndef MGLX11
#define	MGLX11
#endif

#include <X11/Xlib.h>
/* {secret} */
typedef	GC           MGL_HDC;
/* {secret} */
typedef	Window		MGL_HWND;

typedef XImage      *MGL_HBITMAP;

/* {secret} */
typedef	Colormap	MGL_HPALETTE;

#else

/* {secret} */
typedef	void*       MGL_HDC;
/* {secret} */
typedef	void*		MGL_HWND;

typedef void*       MGL_HBITMAP;

/* {secret} */
typedef	void* 	    MGL_HPALETTE;

#endif

/* {secret} */
typedef	void		*MGL_HINSTANCE;
/* {secret} */
typedef	void		*MGL_HGLRC;
/* {secret} */
typedef long 		(* MGL_WNDPROC)(MGL_HWND, uint, uint, long);

/*------------------------- Function Prototypes ---------------------------*/

#ifdef	__cplusplus
extern "C" {			/* Use "C" linkage when in C++ mode	*/
#endif

MGLDC * MGLAPI MGL_createWindowedDC(
	MGL_HWND hwnd);

#ifdef __X11__
void MGLAPI MGL_setX11Display(Display *dpy);
#endif

#ifdef	__cplusplus
}						/* End of "C" linkage for C++	*/
#endif

#endif	/* __MGLUNIX_H */
