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
* Environment:  Unix / X11
*
* Description:  Internal header file for the MGLX bindings for the MGL.
*
****************************************************************************/

#ifndef __MGLX_INTERNAL_H
#define __MGLX_INTERNAL_H

/*---------------------- Macro and type definitions -----------------------*/

#ifndef  MGLX11
#define  MGLX11
#endif

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>


/*--------------------------- Global Variables ----------------------------*/

/* The X11 Display */
extern Display *globalDisplay;

extern int                      _VARAPI _MGL_deskX;             /* Desktop resolution                           */
extern int                      _VARAPI _MGL_deskY;
extern int                      _VARAPI _MGL_mx;                /* Mouse coordinates                            */
extern int                      _VARAPI _MGL_my;
extern int                      _VARAPI _MGL_xRes;              /* Video mode resolution                        */
extern int                      _VARAPI _MGL_yRes;
/*------------------------- Function Prototypes ---------------------------*/

void _EVT_init(MGLDC *);
void _EVT_suspend(void);
void _EVT_resume(void);

void MGLAPI _MGL_delay(int milliseconds);

#endif  /* __MGLX_INTERNAL_H */
