/****************************************************************************
*
*           		SciTech Nucleus Graphics Architecture
*
*               Copyright (C) 1991-1998 SciTech Software, Inc.
*                            All rights reserved.
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
* Language:		ANSI C
* Environment:	Any
*
* Description:	Header file for the simple Portable Executeable DLL loader
*				library. This library can be used to load PE DLL's under
*				any Intel based OS, provided the DLL's do not have any
*				imports in the import table.
*
*				NOTE: This loader module expects the DLL's to be built with
*					  Watcom C++ and may produce unexpected results with
*					  DLL's linked by another compiler.
*
****************************************************************************/

#ifndef	__PELOADER_H
#define	__PELOADER_H

#include "scitech.h"
#ifdef	__WIN32_VXD__
#include "vxdfile.h"
typedef long time_t;
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#endif

/*---------------------- Macros and type definitions ----------------------*/

#pragma pack(1)

#if	defined(__GNUC__) || (defined(__UNIX__) && !defined(__WATCOMC__))
#define	PEAPI
#else
#define	PEAPI	__cdecl
#endif

/* Structure of a module handle when loaded from disk */

typedef struct {
	uchar	*text;			/* Text section read from disk				*/
	uchar	*data;			/* Data section read from disk				*/
	uchar	*export;		/* Export section read from disk			*/
	ulong	textBase;		/* Base of text section in image			*/
	ulong	dataBase;		/* Base of data section in image			*/
	ulong	exportBase;		/* Offset of export section in image		*/
	ulong	exportDir;		/* Offset of export directory 				*/
	ibool	shared;			/* True if module loaded in shared memory	*/
	} PE_MODULE;

/****************************************************************************
REMARKS:
Defines the error codes returned by the library

HEADER:
peloader.h

MEMBERS:
PE_ok				- No error
PE_fileNotFound		- DLL file not found
PE_outOfMemory		- Out of memory loading DLL
PE_invalidDLLImage	- DLL image is invalid or corrupted
PE_unableToInitLibC	- Unable to initialise the C runtime library
****************************************************************************/
typedef enum {
	PE_ok,
	PE_fileNotFound,
	PE_outOfMemory,
	PE_invalidDLLImage,
	PE_unableToInitLibC,
	} PE_errorCodes;

#pragma pack()

/*------------------------- Function Prototypes ---------------------------*/

#ifdef	__cplusplus
extern "C" {			/* Use "C" linkage when in C++ mode	*/
#endif

ulong 		PEAPI PE_getFileSize(FILE *f,ulong startOffset);
PE_MODULE *	PEAPI PE_loadLibraryExt(FILE *f,ulong offset,ulong *size,ibool shared);
PE_MODULE *	PEAPI PE_loadLibrary(const char *szDLLName,ibool shared);
PE_MODULE *	PEAPI PE_loadLibraryMGL(const char *szDLLName,ibool shared);
void * 		PEAPI PE_getProcAddress(PE_MODULE *hModule,const char *szProcName);
void		PEAPI PE_mapShared(PE_MODULE *hModule);
void		PEAPI PE_freeLibrary(PE_MODULE *hModule);
int			PEAPI PE_getError(void);

#ifdef	__cplusplus
}						/* End of "C" linkage for C++	*/
#endif	/* __cplusplus */

#endif	/* __PELOADER_H */
