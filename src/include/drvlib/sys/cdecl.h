/****************************************************************************
*
*           		SciTech Nucleus Graphics Architecture
*
*               Copyright (C) 1991-1998 SciTech Software, Inc.
*                            All rights reserved.
*
*					  Portions Copyright P.J. Plauger.
*
*  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*  |REMOVAL OR MODIFICATION OF THIS HEADER IS STRICTLY PROHIBITED BY LAW|
*  |                                                                    |
*  |This copyrighted computer code contains proprietary technology      |
*  |owned by SciTech Software, Inc., located at 505 Wall Street,        |
*  |Chico, CA 95928 USA (http://www.scitechsoft.com).                   |
*  |                                                                    |
*  |The contents of this file are subject to the SciTech Nucleus        |
*  |License; you may *not* use this file or related software except in  |
*  |compliance with the License. You may obtain a copy of the License   |
*  |at http://www.scitechsoft.com/nucleus-license.txt                   |
*  |                                                                    |
*  |Software distributed under the License is distributed on an         |
*  |"AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or      |
*  |implied. See the License for the specific language governing        |
*  |rights and limitations under the License.                           |
*  |                                                                    |
*  |REMOVAL OR MODIFICATION OF THIS HEADER IS STRICTLY PROHIBITED BY LAW|
*  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*
* Language:     ANSI C
* Environment:  Any
*
* Description:  Header file to declare the _CDECL modifier for Binary
*				Portable DLL's.
*
****************************************************************************/

#ifndef	__SYS_CDECL_H
#define	__SYS_CDECL_H

#define __DRVLIB__
#include "scitech.h"

/* Maximum path length for portable code */

#ifndef	_MAX_PATH
#define	_MAX_PATH		256
#endif

/* Define calling conventions for standard C calling conventions */

#if	defined(__GNUC__) || (defined(__UNIX__) && !defined(__WATCOMC__))
#define	_CDECL
#define _CEXPORT
#else
#define	_CDECL			__cdecl
#ifdef	__DRIVER__
#define _CEXPORT		__cdecl __export
#else
#define _CEXPORT		__cdecl
#endif
#endif

#endif	/* __SYS_CDECL_H */
