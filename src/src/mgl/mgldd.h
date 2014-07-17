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
* Description:	Header file to include extra headers for device driver
*				code. This header will bring in the windows system headers
*				when compiling for Windows, so use sparingly.
*
****************************************************************************/

#ifndef	__MGLDD_H
#define	__MGLDD_H

#include "mgl.h"
#if 	defined(MGLSMX)
#include "mglsmx/internal.h"
#elif 	defined(MGLWIN)
#include "mglwin/internal.h"
#elif	defined(MGLQNX)
#include "mglqnx/internal.h"
#elif 	defined(MGLLINUX)
#include "mgllinux/internal.h"
#elif 	defined(MGLX11)
#include "mglx11/internal.h"
#elif 	defined(MGLDOS)
#include "mgldos/internal.h"
#elif 	defined(MGLOS2)
#include "mglos2/internal.h"
#else
#error	MGL not ported to this platform yet!
#endif

#endif	/* __MGLDD_H */
