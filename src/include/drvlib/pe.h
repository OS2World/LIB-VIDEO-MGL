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
* Description:	Internal header file describing the PE File Format
*				structures.
*
****************************************************************************/

#ifndef	__PE_H
#define	__PE_H

#ifndef	__WIN32_VXD__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include "scitech.h"
#endif

/*---------------------- Macros and type definitions ----------------------*/

#pragma pack(1)

/* Structure defining the MS-DOS executeable file header */

typedef struct {
	ushort signature;
	ushort low;
	ushort high;
	ushort reloc;
	ushort hdr_para;
	} EXE_HDR;

/* Structure defining the COFF file header */

typedef struct {
	ushort  Machine;
	ushort  NumberOfSections;
	ulong   TimeDateStamp;
	ulong   PointerToSymbolTable;
	ulong   NumberOfSynbols;
	ushort	SizeOfOptionalHeader;
	ushort  Characteristics;
	} FILE_HDR;

/* Flags for the machine type in the COFF file header */

#ifndef IMAGE_FILE_MACHINE_UNKNOWN
#define IMAGE_FILE_MACHINE_UNKNOWN	0		/* Machine type is unknown 	*/
#define IMAGE_FILE_MACHINE_I386		0x14c	/* Intel 386 or later		*/
#define IMAGE_FILE_MACHINE_R3000	0x162	/* MIPS R3000 little endian	*/
#define IMAGE_FILE_MACHINE_R4000	0x166	/* MIPS R4000 little endian	*/
#define IMAGE_FILE_MACHINE_R10000	0x168	/* MIPS R10000 little endian*/
#define IMAGE_FILE_MACHINE_ALPHA	0x184	/* DEC Alpha AXP			*/
#define IMAGE_FILE_MACHINE_M68K		0x268	/* Motorola 68000 series	*/
#define IMAGE_FILE_MACHINE_POWERPC	0x1F0	/* Power PC, little endian	*/
#define IMAGE_FILE_MACHINE_SH3		0x1a2	/* Hitachi SH3				*/
#define IMAGE_FILE_MACHINE_SH4		0x1a6	/* Hitachi SH4				*/
#define IMAGE_FILE_MACHINE_ARM		0x1c0	/* ARM Processor			*/
#endif

/* Flags for the characteristic in the COFF file header */

#define IMAGE_FILE_RELOCS_STRIPPED			0x0001
#define IMAGE_FILE_EXECUTABLE_IMAGE			0x0002
#define IMAGE_FILE_LINE_NUMS_STRIPPED		0x0004
#define IMAGE_FILE_LOCAL_SYMS_STRIPPED		0x0008
#define IMAGE_FILE_AGGRESSIVE_WS_TRIM		0x0010
#define IMAGE_FILE_LARGE_ADDRESS_AWARE		0x0020
#define IMAGE_FILE_16BIT_MACHINE			0x0040
#define IMAGE_FILE_BYTES_REVERSED_LO		0x0080
#define IMAGE_FILE_32BIT_MACHINE			0x0100
#define IMAGE_FILE_DEBUG_STRIPPED			0x0200
#define IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP	0x0400
#define IMAGE_FILE_SYSTEM					0x1000
#define IMAGE_FILE_DLL						0x2000
#define IMAGE_FILE_UP_SYSTEM_ONLY			0x4000
#define IMAGE_FILE_BYTES_REVERSED_HI		0x8000

/* Structure of an entry in the data directory */

typedef struct {
	ulong	RelVirtualAddress;
	ulong	Size;
	} DATA_DIRECTORY;

/* Structure defining the optional PE header for Win32 */

typedef struct {
	ushort  		Magic;
	uchar   		MajorLinkerVersion;
	uchar			MinorLinkerVersion;
	ulong   		SizeOfCode;
	ulong   		SizeOfInitializedData;
	ulong   		SizeOfUninitializedData;
	ulong   		AddressOfEntryPoint;
	ulong   		BaseOfCode;
	ulong   		BaseOfData;
	ulong   		ImageBase;
	ulong   		SectionAlignment;
	ulong   		FileAlignment;
	ushort  		MajorOperatingSystemVersion;
	ushort  		MinorOperatingSystemVersion;
	ushort  		MajorImageVersion;
	ushort  		MinorImageVersion;
	ushort  		MajorSubsystemVersion;
	ushort  		MinorSubsystemVersion;
	ulong   		Reserved1;
	ulong   		SizeOfImage;
	ulong   		SizeOfHeaders;
	ulong   		CheckSum;
	ushort  		Subsystem;
	ushort  		DllCharacteristics;
	ulong   		SizeOfStackReserve;
	ulong   		SizeOfStackCommit;
	ulong   		SizeOfHeapReserve;
	ulong   		SizeOfHeapCommit;
	ulong   		LoaderFlags;
	ulong   		NumberOfRvaAndSizes;
	DATA_DIRECTORY	DataDirectory[16];
	} OPTIONAL_HDR;

/* Definitions for the SubSystem field of the optional header */

#define IMAGE_SUBSYSTEM_UNKNOWN			0	/* Unknown subsystem		*/
#define IMAGE_SUBSYSTEM_NATIVE			1	/* Device drivers			*/
#define IMAGE_SUBSYSTEM_WINDOWS_GUI		2	/* Windows GUI				*/
#define IMAGE_SUBSYSTEM_WINDOWS_CUI		3	/* Windows Console			*/
#define IMAGE_SUBSYSTEM_POSIX_CUI		7	/* POSIX Console			*/
#define IMAGE_SUBSYSTEM_WINDOWS_CE_GUI	9	/* Windows CE GUI			*/

/* Definitions for the DllCharacteristics field of the optional header */

#define IMAGE_DLLCHARACTERISTICS_WDM_DRIVER	0x2000	/* WDM device driver*/

/* Offsets of the data directories in optional header */

#define DIR_EXPORT         	0
#define DIR_IMPORT         	1
#define DIR_RESOURCE       	2
#define DIR_EXCEPTION      	3
#define DIR_CERTIFICATE   	4
#define DIR_BASERELOC      	5
#define DIR_DEBUG          	6
#define DIR_ARCHITECTURE	7
#define DIR_GLOBALPTR      	8
#define DIR_TLS            	9
#define DIR_LOAD_CONFIG   	10
#define	DIR_BOUNDIMPORT		11
#define DIR_IAT				12
#define DIR_DELAYIMPORT		13

typedef struct {
	char    	Name[8];
	ulong   	VirtualSize;
	ulong   	VirtualAddress;
	ulong   	SizeOfRawData;
	ulong   	PointerToRawData;
	ulong   	PointerToRelocations;
	ulong   	PointerToLineNumbers;
	ushort  	NumberOfRelocations;
	ushort    	NumberOfLineNumbers;
	ulong   	Characteristics;
	} SECTION_HDR;

/* Macro to round a value to a 4Kb boundary */

#define	ROUND_4K(s)	(((ulong)(s) + 0xFFF) & ~0xFFF)

/* Definitions for the Characteristics for the section header */

#define IMAGE_SCN_TYPE_REG					0x00000000
#define IMAGE_SCN_TYPE_DSECT				0x00000001
#define IMAGE_SCN_TYPE_NOLOAD				0x00000002
#define IMAGE_SCN_TYPE_GROUP				0x00000004
#define IMAGE_SCN_TYPE_NO_PAD				0x00000008
#define IMAGE_SCN_TYPE_COPY					0x00000010
#define IMAGE_SCN_CNT_CODE					0x00000020
#define IMAGE_SCN_CNT_INITIALIZED_DATA		0x00000040
#define IMAGE_SCN_CNT_UNINITIALIZED_DATA	0x00000080
#define IMAGE_SCN_LNK_OTHER					0x00000100
#define IMAGE_SCN_LNK_INFO					0x00000200
#define IMAGE_SCN_TYPE_OVER					0x00000400
#define IMAGE_SCN_LNK_REMOVE				0x00000800
#define IMAGE_SCN_LNK_COMDAT				0x00001000
#define IMAGE_SCN_MEM_FARDATA				0x00008000
#define IMAGE_SCN_MEM_PURGEABLE				0x00020000
#define IMAGE_SCN_MEM_16BIT					0x00020000
#define IMAGE_SCN_MEM_LOCKED				0x00040000
#define IMAGE_SCN_MEM_PRELOAD				0x00080000
#define IMAGE_SCN_ALIGN_1BYTES				0x00100000
#define IMAGE_SCN_ALIGN_2BYTES				0x00200000
#define IMAGE_SCN_ALIGN_4BYTES				0x00300000
#define IMAGE_SCN_ALIGN_8BYTES				0x00400000
#define IMAGE_SCN_ALIGN_16BYTES				0x00500000
#define IMAGE_SCN_ALIGN_32BYTES				0x00600000
#define IMAGE_SCN_ALIGN_64BYTES				0x00700000
#define IMAGE_SCN_LNK_OVFL					0x01000000
#define IMAGE_SCN_MEM_DISCARDABLE			0x02000000
#define IMAGE_SCN_MEM_NOT_CACHED			0x04000000
#define IMAGE_SCN_MEM_NOT_PAGED				0x08000000
#define IMAGE_SCN_MEM_SHARED				0x10000000
#define IMAGE_SCN_MEM_EXECUTE				0x20000000
#define IMAGE_SCN_MEM_READ					0x40000000
#define IMAGE_SCN_MEM_WRITE					0x80000000

/* Structure defining a fixup table entry block in the .reloc section */

typedef struct {
	ulong	PageRVA;
	ulong	BlockSize;
	} BASE_RELOCATION;

/* Defines for the type field of the RELOC structure (top 4 bits) */

#define IMAGE_REL_BASED_ABSOLUTE		0
#define IMAGE_REL_BASED_HIGH			1
#define IMAGE_REL_BASED_LOW				2
#define IMAGE_REL_BASED_HIGHLOW			3
#define IMAGE_REL_BASED_HIGHADJ			4
#define IMAGE_REL_BASED_MIPS_JMPADDR	5

/* Structure defining the export directory table */

typedef struct {
	ulong 	ExportFlags;
	ulong  	TimeDateStamp;
	ushort  MajorVersion;
	ushort  MinorVersion;
	ulong  	NameRVA;
	ulong   OrdinalBase;
	ulong   AddressTableEntries;
	ulong   NumberOfNamePointers;
	ulong   AddressTableRVA;
	ulong   NameTableRVA;
	ulong	OrdinalTableRVA;
	} EXPORT_DIRECTORY;

#pragma pack()

#endif	/* __PE_H */
