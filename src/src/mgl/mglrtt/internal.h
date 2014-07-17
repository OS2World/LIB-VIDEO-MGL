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
* Environment:  Any
*
* Description:	Internal header file for the MGLDOS bindings for the MGL.
*				For all MGLDOS display drivers, we cache the selector for
*				the display memory in GS.
*
****************************************************************************/

#ifndef	__MGLDOS_INTERNAL_H
#define	__MGLDOS_INTERNAL_H

#include "vesavbe.h"

/*---------------------- Macro and type definitions -----------------------*/

#define	IBMPC					/* Use IBM PC assembler routines		*/
#define	PC_FULLSCREEN			/* Include fullscreen mode support		*/
#define	PC_ACCELERATED			/* Include accelerated mode support		*/

/*--------------------------- Global Variables ----------------------------*/

extern uchar _VARAPI *_MGL_biosPtr;		/* Pointer to BIOS data area		*/

/*------------------------- Function Prototypes ---------------------------*/

#endif	/* __MGLDOS_INTERNAL_H */
