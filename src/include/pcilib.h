/****************************************************************************
*
*					SciTech OS Portability Manager Library
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
* Description:  Header file for interface routines to the PCI bus.
*
****************************************************************************/

#ifndef __PCILIB_H
#define __PCILIB_H

#include "scitech.h"

/*---------------------- Macros and type definitions ----------------------*/

#pragma pack(1)

/* Structure defining a PCI slot identifier */

typedef union {
	struct {
		uint    Zero:2;
		uint	Register:6;
		uint	Function:3;
		uint    Device:5;
		uint    Bus:8;
		uint    Reserved:7;
		uint	Enable:1;
		} p;
	ulong   i;
	} PCIslot;

/* Structure defining the PCI configuration space information for a
 * single PCI device on the PCI bus. We enumerate all this information
 * for all PCI devices on the bus.
 */

typedef struct {
	ulong	dwSize;
	PCIslot	slot;
	ulong	mech1;
	ushort	VendorID;
	ushort	DeviceID;
	ushort	Command;
	ushort	Status;
	uchar  	RevID;
	uchar   Interface;
	uchar   SubClass;
	uchar   BaseClass;
	uchar	CacheLineSize;
	uchar   LatencyTimer;
	uchar   HeaderType;
	uchar   BIST;
	ulong   BaseAddress10;
	ulong   BaseAddress14;
	ulong   BaseAddress18;
	ulong   BaseAddress1C;
	ulong   BaseAddress20;
	ulong   BaseAddress24;
	ulong	CardbusCISPointer;
	ushort  SubSystemVendorID;
	ushort	SubSystemID;
	ulong   ROMBaseAddress;
	ulong	reserved1;
	ulong	reserved2;
	uchar	InterruptLine;
	uchar   InterruptPin;
	uchar   Min_Gnt;
	uchar	Max_Lat;
	ulong   BaseAddress10Len;
	ulong   BaseAddress14Len;
	ulong   BaseAddress18Len;
	ulong   BaseAddress1CLen;
	ulong   BaseAddress20Len;
	ulong   BaseAddress24Len;
	ulong   ROMBaseAddressLen;
	} PCIDeviceInfo;

/* Structure defining the PCI bridge configuration space information.
 * PCI bridges use a different configuration space to regular PCI devices.
 */

typedef struct {
	ulong	dwSize;
	PCIslot	slot;
	ulong	mech1;
	ushort	VendorID;
	ushort	DeviceID;
	ushort	Command;
	ushort	Status;
	uchar  	RevID;
	uchar   Interface;
	uchar   SubClass;
	uchar   BaseClass;
	uchar	CacheLineSize;
	uchar   PrimaryLatencyTimer;
	uchar   HeaderType;
	uchar   BIST;
	ulong   BaseAddress10;
	ulong   BaseAddress14;
	uchar	PrimaryBusNumber;
	uchar	SecondayBusNumber;
	uchar	SubordinateBusNumber;
	uchar	SecondaryLatencyTimer;
	uchar	IOBase;
	uchar	IOLimit;
	ushort	SecondaryStatus;
	ushort	MemoryBase;
	ushort	MemoryLimit;
	ushort	PrefetchableMemoryBase;
	ushort	PrefetchableMemoryLimit;
	ulong	PrefetchableBaseHi;
	ulong	PrefetchableLimitHi;
	ushort	IOBaseHi;
	ushort	IOLimitHi;
	uchar	CapabilitiesPointer;
	uchar	reserved1;
	uchar	reserved2;
	uchar	reserved3;
	ulong   ROMBaseAddress;
	uchar	InterruptLine;
	uchar   InterruptPin;
	ushort	BridgeControl;
	ulong   BaseAddress10Len;
	ulong   BaseAddress14Len;
	ulong   BaseAddress18Len;
	ulong   BaseAddress1CLen;
	ulong   BaseAddress20Len;
	ulong   BaseAddress24Len;
	ulong   ROMBaseAddressLen;
	} PCIBridgeInfo;

/* Structure for obtaining the PCI IRQ routing information */

typedef struct {
	uchar	bus;
	uchar	device;
	uchar	linkA;
	ushort	mapA;
	uchar	linkB;
	ushort	mapB;
	uchar	linkC;
	ushort	mapC;
	uchar	linkD;
	ushort	mapD;
	uchar	slot;
	uchar	reserved;
	} PCIRouteInfo;

typedef struct {
	ushort			BufferSize;
	PCIRouteInfo	*DataBuffer;
	} PCIRoutingOptionsBuffer;

#define	MAX_PCI_DEVICES				20
#define	NUM_PCI_REG					(sizeof(PCIDeviceInfo) / 4) - 10
#define	PCI_BRIDGE_CLASS			0x06
#define	PCI_EARLY_VGA_CLASS			0x00
#define	PCI_EARLY_VGA_SUBCLASS		0x01
#define	PCI_DISPLAY_CLASS			0x03
#define	PCI_DISPLAY_VGA_SUBCLASS	0x00
#define	PCI_DISPLAY_XGA_SUBCLASS	0x01
#define	PCI_DISPLAY_OTHER_SUBCLASS	0x80
#define	PCI_MM_CLASS				0x04
#define	PCI_AUDIO_SUBCLASS			0x01

/* Function codes to pass to PCI_accessReg */

#define	PCI_READ_BYTE				0
#define	PCI_READ_WORD               1
#define	PCI_READ_DWORD              2
#define	PCI_WRITE_BYTE              3
#define	PCI_WRITE_WORD              4
#define	PCI_WRITE_DWORD             5

/* Macros to read/write PCI registers. These assume a global PCI array
 * of device information.
 */

#define PCI_readPCIRegB(index,device)	\
	PCI_accessReg(index,0,0,&PCI[DeviceIndex[device]])

#define PCI_readPCIRegW(index,device)	\
	PCI_accessReg(index,0,1,&PCI[DeviceIndex[device]])

#define PCI_readPCIRegL(index,device)	\
	PCI_accessReg(index,0,2,&PCI[DeviceIndex[device]])

#define PCI_writePCIRegB(index,value,device)	\
	PCI_accessReg(index,value,3,&PCI[DeviceIndex[device]])

#define PCI_writePCIRegW(index,value,device)	\
	PCI_accessReg(index,value,4,&PCI[DeviceIndex[device]])

#define PCI_writePCIRegL(index,value,device)	\
	PCI_accessReg(index,value,5,&PCI[DeviceIndex[device]])

#pragma pack()

/*-------------------------- Function Prototypes --------------------------*/

#ifdef  __cplusplus
extern "C" {            			/* Use "C" linkage when in C++ mode */
#endif

/* Function to map any shared memory used by the PCI modules */

void	_ASMAPI PCI_mapShared(void);

/* Function to enumerate all device on the PCI bus */

int		_ASMAPI PCI_enumerate(PCIDeviceInfo info[],int maxDevices);

/* Function to access PCI configuration registers */

ulong 	_ASMAPI PCI_accessReg(int index,ulong value,int func,PCIDeviceInfo *info);

/* Function to get PCI IRQ routing options for a card */

int		_ASMAPI PCI_getIRQRoutingOptions(int numDevices,PCIRouteInfo *buffer);

/* Function to re-route the PCI IRQ setting for a device */

ibool	_ASMAPI PCI_setHardwareIRQ(PCIDeviceInfo *info,uint intPin,uint IRQ);

/* Function to generate a special cyle on the specified PCI bus */

void	_ASMAPI PCI_generateSpecialCyle(uint bus,ulong specialCycleData);

/* Function to return the 32-bit PCI BIOS entry point */

ulong 	_ASMAPI PCIBIOS_getEntry(void);

#ifdef  __cplusplus
}                       			/* End of "C" linkage for C++   	*/
#endif

#endif  /* __PCILIB_H */
