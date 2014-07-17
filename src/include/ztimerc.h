/****************************************************************************
*
*						   The Zen Timer Library
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
* Description:	Header file for the Zen Timer library. Provides a number
*				of routines to accurately time segments of code. A long
*				period timer is provided to time code that takes up to
*				one hour to execute, with microsecond precision, and an
*				ultra long period timer for timing code that takes up to
*				24 hours to execute (raytracing etc).
*
*				This library also includes routines for querying the CPU
*				type, CPU speed and CPU features, and includes support for
*				high precision timing on Pentium based systems using the
*				Read Time Stamp Counter.
*
****************************************************************************/

#ifndef	__ZTIMERC_H
#define	__ZTIMERC_H

#include "scitech.h"

/*--------------------- Macros and type definitions -----------------------*/

/* Define the calling conventions - C always */

#define	ZAPI	_ASMAPI

/****************************************************************************
REMARKS:
Defines the types of processors returned by CPU_getType.

HEADER:
ztimer.h

MEMBERS:
CPU_i386			- Intel 80386 processor
CPU_i486			- Intel 80486 processor
CPU_Pentium			- Intel Pentium(R) processor
CPU_PentiumPro		- Intel PentiumPro(R) processor
CPU_PentiumII		- Intel PentiumII(R) processor
CPU_Celeron			- Intel Celeron(R) processor
CPU_PentiumIII		- Intel PentiumIII(R) processor
CPU_UnkIntel		- Unknown Intel processor
CPU_Cyrix6x86		- Cyrix 6x86 processor
CPU_Cyrix6x86MX     - Cyrix 6x86MX processor
CPU_CyrixMediaGX	- Cyrix MediaGX processor
CPU_CyrixMediaGXm	- Cyrix MediaGXm processor
CPU_UnkCyrix		- Unknown Cyrix processor
CPU_AMDAm486		- AMD Am486 processor
CPU_AMDAm5x86		- AMD Am5x86 processor
CPU_AMDK5			- AMD K5 processor
CPU_AMDK6			- AMD K6 processor
CPU_AMDK6_2			- AMD K6-2 processor
CPU_AMDK6_III		- AMD K6-III processor
CPU_AMDAthlon		- AMD Athlon processor
CPU_UnkAMD			- Unknown AMD processor
CPU_WinChipC6		- IDT WinChip C6 processor
CPU_WinChip2		- IDT WinChip 2 processor
CPU_UnkIDT			- Unknown IDT processor
CPU_Alpha			- DEC Alpha processor
CPU_Mips			- MIPS processor
CPU_PowerPC			- PowerPC processor
CPU_mask			- Mask to remove flags and get CPU type
CPU_IDT				- This bit is set if the processor vendor is IDT
CPU_Cyrix			- This bit is set if the processor vendor is Cyrix
CPU_AMD				- This bit is set if the processor vendor is AMD
CPU_Intel			- This bit is set if the processor vendor is Intel
CPU_familyMask		- Mask to isolate CPU family
CPU_steppingMask	- Mask to isolate CPU stepping
CPU_steppingShift	- Shift factor for CPU stepping
****************************************************************************/
typedef enum {
	CPU_i386			= 0,
	CPU_i486			= 1,
	CPU_Pentium			= 2,
	CPU_PentiumPro		= 3,
	CPU_PentiumII		= 4,
	CPU_Celeron			= 5,
	CPU_PentiumIII		= 6,
	CPU_UnkIntel		= 7,
	CPU_Cyrix6x86		= 100,
	CPU_Cyrix6x86MX		= 101,
	CPU_CyrixMediaGX	= 102,
	CPU_CyrixMediaGXm	= 104,
	CPU_UnkCyrix		= 105,
	CPU_AMDAm486		= 200,
	CPU_AMDAm5x86		= 201,
	CPU_AMDK5			= 202,
	CPU_AMDK6			= 203,
	CPU_AMDK6_2			= 204,
	CPU_AMDK6_III		= 205,
	CPU_AMDAthlon		= 206,
	CPU_UnkAMD			= 207,
	CPU_WinChipC6		= 300,
	CPU_WinChip2		= 301,
	CPU_UnkIDT			= 302,
	CPU_Alpha			= 500,
	CPU_Mips			= 600,
	CPU_PowerPC			= 700,
	CPU_mask			= 0x00000FFF,
	CPU_IDT				= 0x00001000,
	CPU_Cyrix			= 0x00002000,
	CPU_AMD				= 0x00004000,
	CPU_Intel           = 0x00008000,
	CPU_familyMask		= 0x00FFF000,
	CPU_steppingMask	= 0x0F000000,
	CPU_steppingShift	= 24
	} CPU_processorType;

/****************************************************************************
REMARKS:
Defines the structure for holding 64-bit integers used for storing the values
returned by the Intel RDTSC instruction.

HEADER:
ztimer.h

MEMBERS:
low		- Low 32-bits of the 64-bit integer
high	- High 32-bits of the 64-bit integer
****************************************************************************/
typedef struct {
	ulong	low;
	ulong	high;
	} CPU_largeInteger;

/****************************************************************************
REMARKS:
Defines the structure for an LZTimerObject which contains the starting and
ending timestamps for the timer. By putting the timer information into a
structure the Zen Timer can be used for multiple timers running
simultaneously.

HEADER:
ztimer.h

MEMBERS:
start	- Starting 64-bit timer count
end		- Ending 64-bit timer count
****************************************************************************/
typedef struct {
	CPU_largeInteger	start;
	CPU_largeInteger	end;
	} LZTimerObject;

/*-------------------------- Function Prototypes --------------------------*/

#define	LZTIMER_RES		1e-6			/* Seconds in a tick			*/

#ifdef	__cplusplus
extern "C" {			/* Use "C" linkage when in C++ mode	*/
#endif

/* Routines to obtain CPU information */

uint	ZAPI CPU_getProcessorType(void);
ibool	ZAPI CPU_haveMMX(void);
ibool	ZAPI CPU_have3DNow(void);
ibool	ZAPI CPU_haveSSE(void);
ibool	ZAPI CPU_haveRDTSC(void);
ulong	ZAPI CPU_getProcessorSpeed(ibool accurate);
char *	ZAPI CPU_getProcessorName(void);

/* Routine to initialise the library - you MUST call this first! */

void	ZAPI ZTimerInit(void);
void	ZAPI ZTimerInitExt(ibool accurate);

/* Long period timing routines (times up to 1 hour) */

void 	ZAPI LZTimerOn(void);
ulong	ZAPI LZTimerLap(void);
void 	ZAPI LZTimerOff(void);
ulong 	ZAPI LZTimerCount(void);

/* Long period timing routines with passed in timer object */

void 	ZAPI LZTimerOnExt(LZTimerObject *tm);
ulong	ZAPI LZTimerLapExt(LZTimerObject *tm);
void 	ZAPI LZTimerOffExt(LZTimerObject *tm);
ulong 	ZAPI LZTimerCountExt(LZTimerObject *tm);

/* Ultra long period timing routines (times up to 65,000 hours) */

void	ZAPI ULZTimerOn(void);
ulong	ZAPI ULZTimerLap(void);
void	ZAPI ULZTimerOff(void);
ulong	ZAPI ULZTimerCount(void);
ulong	ZAPI ULZReadTime(void);
ulong	ZAPI ULZElapsedTime(ulong start,ulong finish);
void	ZAPI ULZTimerResolution(float *resolution);

#ifdef	__cplusplus
}						/* End of "C" linkage for C++	*/
#endif

#endif	/* __ZTIMERC_H */
