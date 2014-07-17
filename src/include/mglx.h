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
* Environment:	Unix X11
*
* Description:	Header file for the MGLX bindings for the X11 evironment.
*
****************************************************************************/

#error Not supported yet!

#ifndef	__MGLX_H
#define	__MGLX_H

/*------------------------- Function Prototypes ---------------------------*/

#ifdef	__cplusplus
extern "C" {			/* Use "C" linkage when in C++ mode	*/
#endif

/* Device context management */

MGLDC	* PUBAPI MGL_createDisplayDC(HWINDOW window);
MGLDC 	* PUBAPI MGL_createMemoryDC(int xRes,int yRes,int bitsPerPixel);
void	PUBAPI MGL_destroyDC(MGLDC *dc);

#ifdef	__cplusplus
}						/* End of "C" linkage for C++	*/
#endif

#endif	/* __MGLX_H */
