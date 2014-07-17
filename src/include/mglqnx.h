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
* Description:	Header file for the MGLQNX bindings for QNX running on top
*				of the Photon MicroGUI.
*
****************************************************************************/

#ifndef	__MGLQNX_H
#define	__MGLQNX_H

#ifndef MGLQNX
#define	MGLQNX
#endif

/*---------------------- Macros and type definitions ----------------------*/

/* {secret} */
typedef	void		*MGL_HDC;
/* {secret} */
typedef	void		*MGL_HINSTANCE;
/* {secret} */
typedef	void		*MGL_HPALETTE;
/* {secret} */
typedef	void		*MGL_HGLRC;

typedef ibool 		(*_MGL_userEventFilter)(event_t *evt);

/*------------------------- Function Prototypes ---------------------------*/

#ifdef	__cplusplus
extern "C" {			/* Use "C" linkage when in C++ mode	*/
#endif

MGLDC * MGLAPI MGL_createWindowedDC(
	MGL_HWND hwnd);

#ifdef	__cplusplus
}						/* End of "C" linkage for C++	*/
#endif

#endif	/* __MGLQNX_H */
