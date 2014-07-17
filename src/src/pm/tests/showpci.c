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
* Environment:	any
*
* Description:  Test program to test the PCI library functions.
*
****************************************************************************/

#include "pmapi.h"
#include "pcilib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/*------------------------- Global Variables ------------------------------*/

static PCIDeviceInfo	PCI[MAX_PCI_DEVICES];
static int				NumPCI = -1;
static int				BridgeIndex[MAX_PCI_DEVICES] = {0};
static int				NumBridges;
static PCIBridgeInfo	*AGPBridge = NULL;
static int				DeviceIndex[MAX_PCI_DEVICES] = {0};
static int				NumDevices;

/*-------------------------- Implementation -------------------------------*/

/****************************************************************************
REMARKS:
Enumerates the PCI bus and dumps the PCI configuration information to the
log file.
****************************************************************************/
static void EnumeratePCI(void)
{
	int				i,index;
	PCIBridgeInfo	*info;

	printf("Displaying enumeration of PCI bus (%d devices, %d display devices)\n",
		NumPCI, NumDevices);
	for (index = 0; index < NumDevices; index++)
		printf("  Display device %d is PCI device %d\n",index,DeviceIndex[index]);
	printf("\n");
	printf("Bus Slot Fnc DeviceID  SubSystem Rev Class IRQ Int Cmd\n");
	for (i = 0; i < NumPCI; i++) {
		printf("%2d   %2d  %2d  %04X:%04X %04X:%04X %02X  %02X:%02X %02X  %02X  %04X   ",
			PCI[i].slot.p.Bus,
			PCI[i].slot.p.Device,
			PCI[i].slot.p.Function,
			PCI[i].VendorID,
			PCI[i].DeviceID,
			PCI[i].SubSystemVendorID,
			PCI[i].SubSystemID,
			PCI[i].RevID,
			PCI[i].BaseClass,
			PCI[i].SubClass,
			PCI[i].InterruptLine,
			PCI[i].InterruptPin,
			PCI[i].Command);
		for (index = 0; index < NumDevices; index++) {
			if (DeviceIndex[index] == i)
				break;
			}
		if (index < NumDevices)
			printf("<- %d\n", index);
		else
			printf("\n");
		}
	printf("\n");
	printf("DeviceID  Stat Ifc Cch Lat Hdr BIST\n");
	for (i = 0; i < NumPCI; i++) {
		printf("%04X:%04X %04X  %02X  %02X  %02X  %02X  %02X   ",
			PCI[i].VendorID,
			PCI[i].DeviceID,
			PCI[i].Status,
			PCI[i].Interface,
			PCI[i].CacheLineSize,
			PCI[i].LatencyTimer,
			PCI[i].HeaderType,
			PCI[i].BIST);
		for (index = 0; index < NumDevices; index++) {
			if (DeviceIndex[index] == i)
				break;
			}
		if (index < NumDevices)
			printf("<- %d\n", index);
		else
			printf("\n");
		}
	printf("\n");
	printf("DeviceID  Base10h  Base14h  Base18h  Base1Ch  Base20h  Base24h  ROMBase\n");
	for (i = 0; i < NumPCI; i++) {
		printf("%04X:%04X %08X %08X %08X %08X %08X %08X %08X ",
			PCI[i].VendorID,
			PCI[i].DeviceID,
			PCI[i].BaseAddress10,
			PCI[i].BaseAddress14,
			PCI[i].BaseAddress18,
			PCI[i].BaseAddress1C,
			PCI[i].BaseAddress20,
			PCI[i].BaseAddress24,
			PCI[i].ROMBaseAddress);
		for (index = 0; index < NumDevices; index++) {
			if (DeviceIndex[index] == i)
				break;
			}
		if (index < NumDevices)
			printf("<- %d\n", index);
		else
			printf("\n");
		}
	printf("\n");
	printf("DeviceID  BAR10Len BAR14Len BAR18Len BAR1CLen BAR20Len BAR24Len ROMLen\n");
	for (i = 0; i < NumPCI; i++) {
		printf("%04X:%04X %08X %08X %08X %08X %08X %08X %08X ",
			PCI[i].VendorID,
			PCI[i].DeviceID,
			PCI[i].BaseAddress10Len,
			PCI[i].BaseAddress14Len,
			PCI[i].BaseAddress18Len,
			PCI[i].BaseAddress1CLen,
			PCI[i].BaseAddress20Len,
			PCI[i].BaseAddress24Len,
			PCI[i].ROMBaseAddressLen);
		for (index = 0; index < NumDevices; index++) {
			if (DeviceIndex[index] == i)
				break;
			}
		if (index < NumDevices)
			printf("<- %d\n", index);
		else
			printf("\n");
		}
	printf("\n");
	printf("Displaying enumeration of %d bridge devices\n",NumBridges);
	printf("\n");
	printf("DeviceID  P# S# B# IOB  IOL  MemBase  MemLimit PreBase  PreLimit Ctrl\n");
	for (i = 0; i < NumBridges; i++) {
		info = (PCIBridgeInfo*)&PCI[BridgeIndex[i]];
		printf("%04X:%04X %02X %02X %02X %04X %04X %08X %08X %08X %08X %04X\n",
			info->VendorID,
			info->DeviceID,
			info->PrimaryBusNumber,
			info->SecondayBusNumber,
			info->SubordinateBusNumber,
			((u16)info->IOBase << 8) & 0xF000,
			info->IOLimit ?
				((u16)info->IOLimit << 8) | 0xFFF : 0,
			((u32)info->MemoryBase << 16) & 0xFFF00000,
			info->MemoryLimit ?
				((u32)info->MemoryLimit << 16) | 0xFFFFF : 0,
			((u32)info->PrefetchableMemoryBase << 16) & 0xFFF00000,
			info->PrefetchableMemoryLimit ?
				((u32)info->PrefetchableMemoryLimit << 16) | 0xFFFFF : 0,
			info->BridgeControl);
		}
	printf("\n");
}

/****************************************************************************
RETURNS:
Number of display devices found.

REMARKS:
This function enumerates the number of available display devices on the
PCI bus, and returns the number found.
****************************************************************************/
static int PCI_enumerateDevices(void)
{
	int				i,j;
	PCIBridgeInfo	*info;

	// If this is the first time we have been called, enumerate all
	// devices on the PCI bus.
	if (NumPCI == -1) {
		for (i = 0; i < MAX_PCI_DEVICES; i++)
			PCI[i].dwSize = sizeof(PCI[i]);
		if ((NumPCI = PCI_enumerate(PCI,MAX_PCI_DEVICES)) == 0)
			return -1;

		// Build a list of all PCI bridge devices
		for (i = 0,NumBridges = 0,BridgeIndex[0] = -1; i < NumPCI; i++) {
			if (PCI[i].BaseClass == PCI_BRIDGE_CLASS) {
				if (NumBridges < MAX_PCI_DEVICES)
					BridgeIndex[NumBridges++] = i;
				}
			}

		// Now build a list of all display class devices
		for (i = 0,NumDevices = 1,DeviceIndex[0] = -1; i < NumPCI; i++) {
			if (((PCI[i].BaseClass == PCI_DISPLAY_CLASS && PCI[i].SubClass == PCI_DISPLAY_VGA_SUBCLASS)
					|| (PCI[i].BaseClass == PCI_EARLY_VGA_CLASS && PCI[i].SubClass == PCI_EARLY_VGA_SUBCLASS))) {
				if ((PCI[i].Command & 0x3) == 0x3) {
					DeviceIndex[0] = i;
					}
				else {
					if (NumDevices < MAX_PCI_DEVICES)
						DeviceIndex[NumDevices++] = i;
					}
				if (PCI[i].slot.p.Bus != 0) {
					// This device is on a different bus than the primary
					// PCI bus, so it is probably an AGP device. Find the
					// AGP bus device that controls that bus so we can
					// control it.
					for (j = 0; j < NumBridges; j++) {
						info = (PCIBridgeInfo*)&PCI[BridgeIndex[j]];
						if (info->SecondayBusNumber == PCI[i].slot.p.Bus) {
							AGPBridge = info;
							break;
							}
						}
					}
				}
			}

		// Enumerate all PCI and bridge devices to standard output
		EnumeratePCI();
		}
	return NumDevices;
}

int main(void)
{
	// Enumerate all PCI devices
	PM_init();
	if (PCI_enumerateDevices() < 1) {
		printf("No PCI display devices found!\n");
		return -1;
		}
	return 0;
}
