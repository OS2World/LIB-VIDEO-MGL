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
* Description:	Header file for the OS specific function to set attribute
*				flags.
*
****************************************************************************/

#ifndef	__ATTRIB_H
#define	__ATTRIB_H

#ifndef	__SYS_CDECL_H
#include "drvlib/sys/cdecl.h"
#endif

/* Attribue bits for _OS_setfileattr */

#define __A_NORMAL   0x00   /* Normal file, no attributes */
#define __A_RDONLY   0x01   /* Read only attribute */
#define __A_HIDDEN   0x02   /* Hidden file */
#define __A_SYSTEM   0x04	/* System file */

#ifdef	__cplusplus
extern "C" {
#endif

void 	_CDECL _OS_setfileattr(const char *filename,unsigned attrib);
ulong 	_CDECL _OS_getcurrentdate(void);

#ifdef	__cplusplus
}
#endif	/* __cplusplus */

#endif	/* __ATTRIB_H */
