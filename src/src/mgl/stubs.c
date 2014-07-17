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
* Description:	C language stubs to pass control to the	device context
*				routines etc. We also have an internal set of macro or
*				inline versions of these for efficiency internally in the
*				MGL.
*
****************************************************************************/

#include "mgl.h"

/*------------------------- Implementation --------------------------------*/

/****************************************************************************
DESCRIPTION:
Enables direct framebuffer access.

HEADER:
mgraph.h

REMARKS:
Enables direct framebuffer access so that you can directly rasterize to the linear
framebuffer memory using your own custom routines. Note that calling this
function is absolutely necessary when using hardware acceleration, as this function
correctly arbitrates
between the hardware accelerator graphics engine and your direct framebuffer
rasterizing code.

SEE ALSO:
SV_endDirectAccess

****************************************************************************/
void MGLAPI MGL_beginDirectAccess(void)
{
	if (DC.r.beginDirectAccess)
		DC.r.beginDirectAccess();
}

/****************************************************************************
DESCRIPTION:
Disables direct framebuffer access.

HEADER:
mgraph.h

REMARKS:
Disables direct framebuffer access so that you can use the accelerator functions to
draw to the framebuffer memory. Note that calling this function is absolutely
necessary when using hardware acceleration, as this function and the corresponding
MGL_beginDirectAccess correctly arbitrate between the hardware accelerator
graphics engine and your direct framebuffer writes.

SEE ALSO:
MGL_beginDirectAccess
****************************************************************************/
void MGLAPI MGL_endDirectAccess(void)
{
	if (DC.r.endDirectAccess)
		DC.r.endDirectAccess();
}

/****************************************************************************
DESCRIPTION:
Setup for high speed pixel drawing.

HEADER:
mgraph.h

REMARKS:
Sets up the video hardware for plotting single pixels as fast a possible. You must
call this routine before calling any of the MGL_pixel and
MGL_getPixel routines to ensure correct operation, and you
must call the MGL_endPixel routine after you have finished.

This routine is intended primarily to ensure fast operation if you need to plot more
than a single pixel at a time.

SEE ALSO:
MGL_endPixel, MGL_pixel, MGL_getPixel.
****************************************************************************/
void MGLAPI MGL_beginPixel(void)
{
	if (DC.r.beginDirectAccess)
		DC.r.beginDirectAccess();
}

/****************************************************************************
DESCRIPTION:
Ends high speed pixel drawing operation.

HEADER:
mgraph.h

REMARKS:
This function ends a set of high speed pixel drawing operations, started with a call
to MGL_beginPixel. This routine is intended primarily to ensure fast operation if
you intend to plot more than a single pixel at a time.

SEE ALSO:
MGL_beginPixel

****************************************************************************/
void MGLAPI MGL_endPixel(void)
{
	if (DC.r.endDirectAccess)
		DC.r.endDirectAccess();
}
