/****************************************************************************
*
*           		SciTech Nucleus Driver Architecture
*
*               Copyright (C) 1991-1998 SciTech Software, Inc.
*                            All rights reserved.
*
*  ======================================================================
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
*  ======================================================================
*
* Language:     ANSI C
* Environment:  Any 32-bit protected mode environment
*
* Description:  Common header file for the SciTech Nucleus Driver
*				Architecture, shared by all device driver level layers.
*
*				All Nucleus drivers are loaded by the appropriate loader
*				library for the desired type of device support. All loader
*				libraries are Binary Portable DLL's that export an API
*				to load and unload drivers. The loader libraries all live
*				in the following standard locations:
*
*					MS-DOS:    	c:\nucleus
*					Windows:	x:\windows\system\nucleus
*					OS/2:		x:\os2\drivers\nucleus
*					Unix:       /usr/lib/nucleus
*
*				where x: is the drive where the operating system is
*				installed, and the 'windows' or 'os2' directories are the
*				standard names for these operating systems (the OS functions
*				are used to determine the exact location).
*
****************************************************************************/

#ifndef __NUCLEUS_COMMON_H
#define __NUCLEUS_COMMON_H

#ifndef	__WINDOWS16__
#include "pmapi.h"
#include "pcilib.h"
#endif

/*---------------------- Macros and type definitions ----------------------*/

/* Define the calling conventions for the code in this module */

#ifdef	__16BIT__
#define	_FAR_	far
#else
#define	_FAR_
#endif

#define	NAPI  	_ASMAPI				/* 'C' calling conventions always	*/
#ifdef	PTR_DECL_IN_FRONT
#define	NAPIP	* _ASMAPI _FAR_
#else
#define	NAPIP	_ASMAPI _FAR_ *
#endif

/* Name of Nucleus environment variable */

#define	NUCLEUS_PATH		"NUCLEUS_PATH"

/* Macros to convert between integer and 32 bit fixed point format */

#define	N_FIX_1				0x10000L
#define	N_FIX_2				0x20000L
#define	N_FIX_HALF			0x08000L
#define	N_TOFIX(i)			((long)(i) << 16)
#define N_FIXTOINT(f)		((N_int32)((f) >> 16))
#define	N_FIXROUND(f) 		((N_int32)(((f) + N_FIX_HALF) >> 16))
#define N_REAL(num)			(long)((num) * 65536.0 + 0.5)

/****************************************************************************
REMARKS:
Fundamental type definition for an 8-bit signed value.

HEADER:
nucleus/common.h
****************************************************************************/
typedef	char N_int8;

/****************************************************************************
REMARKS:
Fundamental type definition for a 16-bit signed value.

HEADER:
nucleus/common.h
****************************************************************************/
typedef short N_int16;

/****************************************************************************
REMARKS:
Fundamental type definition for a 32-bit signed value.

HEADER:
nucleus/common.h
****************************************************************************/
typedef long N_int32;

/****************************************************************************
REMARKS:
Fundamental type definition for an 8-bit unsigned value.

HEADER:
nucleus/common.h
****************************************************************************/
typedef	unsigned char N_uint8;

/****************************************************************************
REMARKS:
Fundamental type definition for a 16-bit unsigned value.

HEADER:
nucleus/common.h
****************************************************************************/
typedef unsigned short N_uint16;

/****************************************************************************
REMARKS:
Fundamental type definition for a 32-bit unsigned value.

HEADER:
nucleus/common.h
****************************************************************************/
typedef unsigned long N_uint32;

/****************************************************************************
REMARKS:
Fundamental type definition for an 32-bit fixed point value. The fixed point
value is interprested as a 16.16 fixed point number, with 16 integral bits
and 16 fractional bits.

HEADER:
nucleus/common.h
****************************************************************************/
typedef	long N_fix32;

/****************************************************************************
REMARKS:
Fundamental type definition for a 32-bit floating point number. The number
is stored as an IEEE 754 floating point number with 1 sign bit, 8 exponent
bits and 23 mantissa bits.

HEADER:
nucleus/common.h
****************************************************************************/
typedef	float N_flt32;

/****************************************************************************
REMARKS:
Error codes returned by N_status to indicate driver load status if loading
the device driver failed.

HEADER:
nucleus/common.h

MEMBERS:
nOK				- No error
nNotDetected	- Hardware not detected
nNotPOSTed		- Hardware has not been POSTed
nDriverNotFound	- Driver file not found
nCorruptDriver	- File loaded not a driver file
nLoadMem		- Not enough memory to load driver
nOldVersion		- Driver file is an older version
nMemMapError	- Could not map physical memory areas
nIOError		- General I/O error
nIRQHookFailed	- Could not hook required hardware IRQ
nNotCertified	- Driver is not certified
****************************************************************************/
typedef enum {
	nOK,
	nNotDetected,
	nNotPOSTed,
	nDriverNotFound,
	nCorruptDriver,
	nLoadMem,
	nOldVersion,
	nMemMapError,
	nIOError,
	nIRQHookFailed,
	nNotCertified,
	nMaxError,
	} N_errorType;

#ifndef	__WINDOWS16__
/****************************************************************************
REMARKS:
Structure defining all the Nucleus Generic functions as imported into
the Binary Portable DLL.
{secret}
****************************************************************************/
typedef struct {
	ulong		dwSize;
	void		(NAPIP _OS_delay)(N_uint32 microSeconds);
	} N_imports;
#endif

/*-------------------------- Function Prototypes --------------------------*/

#ifdef  __cplusplus
extern "C" {            			/* Use "C" linkage when in C++ mode */
#endif

/* Generic Nucleus helper functions. These functions are implemented
 * in the OS specific modules and exported to the Binary Portable DLL.
 */

/* {secret} */
void			NAPI _OS_delay(N_uint32 microSeconds);

/* Watcom C++ specific fixed point multiplication/divide routines */

#if defined(__WATCOMC__) && defined(__386__) && !defined(NOINLINEASM)
N_fix32	N_FixMul(N_fix32 a,N_fix32 b);
#pragma aux N_FixMul =			\
	"imul	edx"					\
	"add	eax,8000h"				\
	"adc	edx,0"					\
	"shrd	eax,edx,16"				\
	parm [eax] [edx]				\
	value [eax]						\
	modify exact [eax edx];

N_fix32	N_FixDiv(N_fix32 a,N_fix32 b);
#pragma aux N_FixDiv =			\
	"xor	eax,eax"				\
	"shrd	eax,edx,16"				\
	"sar	edx,16"					\
	"idiv	ebx"              		\
	parm [edx] [ebx] 				\
	value [eax]             		\
	modify exact [eax edx];

N_fix32	N_FixMulDiv(N_fix32 a,N_fix32 b,N_fix32 c);
#pragma aux N_FixMulDiv =			\
	"imul	ebx"					\
	"idiv	ecx"              		\
	parm [eax] [ebx] [ecx] 			\
	value [eax]             		\
	modify exact [eax edx];
#endif

#ifdef  __cplusplus
}                       			/* End of "C" linkage for C++   	*/
#endif

#endif  /* __NUCLEUS_COMMON_H */
