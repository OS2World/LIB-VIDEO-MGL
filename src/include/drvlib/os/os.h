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
* Description:  General header file to define the global OS variable,
*				the the types of OS'es we recognise.
*
****************************************************************************/

#ifndef __OS_OS_H
#define __OS_OS_H

/* Defines for the runtime operating system. We define this
 * so that our OS neutral binaries can include code that is OS dependant
 * that will switch at runtime as necessary.
 */

#define	_OS_DOS			0x00000001	/* MS-DOS environments				*/
#define	_OS_WIN32VXD	0x00000002	/* Win32 VxD drivers				*/
#define	_OS_WIN95		0x00000004	/* Windows 95/98					*/
#define	_OS_WINNT		0x00000008	/* Windows NT						*/
#define	_OS_OS2			0x00000010	/* Class for OS/2					*/
#define	_OS_NETWARE		0x00000020	/* Class for Novell Netware 		*/
#define	_OS_LINUX		0x00000040	/* Linux 							*/
#define	_OS_FREEBSD		0x00000080	/* FreeBSD 							*/
#define	_OS_SCO			0x00000100	/* SCO Unix							*/
#define	_OS_SOLARIS		0x00000200	/* Sun Solaris						*/
#define	_OS_QNX			0x00000400	/* QNX Realtime OS					*/
#define	_OS_BEOS		0x00000800	/* BeOS								*/
#define	_OS_SMX			0x00001000	/* SMX Embedded OS					*/
#define	_OS_WIN31		0x00002000	/* Windows 3.1						*/
#define	_OS_PHARLAP		0x00004000	/* PharLap Embedded Real Time OS	*/
#define	_OS_OSE			0x00008000	/* Enea OSE Embedded OS				*/
#define	_OS_RTTARGET	0x00010000	/* RTTarget32 Embedded OS			*/
#define	_OS_WINCE		0x00020000	/* Windows CE Embedded OS			*/
#define	_OS_ALL			0x7FFFFFFF	/* All OS'es						*/
#define	_OS_UNSUPPORTED	-1			/* Environment not supported		*/

/* Defines for classes of operating systems */

#define	_OS_CLASS_DOS		(_OS_DOS)
#define	_OS_CLASS_WIN31		(_OS_WIN31)
#define	_OS_CLASS_WIN32		(_OS_WIN95 | _OS_WINNT | _OS_WINCE)
#define	_OS_CLASS_OS2		(_OS_OS2)
#define	_OS_CLASS_NETWARE	(_OS_NETWARE)
#define	_OS_CLASS_UNIX		(_OS_LINUX | _OS_FREEBSD | _OS_SCO | _OS_SOLARIS | _OS_QNX | _OS_BEOS)
#define	_OS_CLASS_EMBEDDED	(_OS_SMX | _OS_PHARLAP | _OS_OSE | _OS_RTTARGET)

/* Define a macro to test for a specific OS */

#define _IS_OS(f)		(___drv_os_type == (f))

/* Define a macro to test for a class of OS */

#define _IS_OS_CLASS(c)	(___drv_os_type & (c))

extern	long	_VARAPI ___drv_os_type;

#endif  /* __OS_OS_H */
