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
* Description:	Header file declaring all the PM imports structure for the
*				current version of the PM library. Included in all code
*				that needs to pass the PM imports to BPD files.
*
****************************************************************************/

PM_imports	_VARAPI _PM_imports = {
	sizeof(PM_imports),
	PM_getModeType,
	PM_getBIOSPointer,
	PM_getA0000Pointer,
	PM_mapPhysicalAddr,
	PM_mallocShared,
	PM_mapShared,
	PM_freeShared,
	PM_mapToProcess,
	PM_mapRealPointer,
	PM_allocRealSeg,
	PM_freeRealSeg,
	PM_allocLockedMem,
	PM_freeLockedMem,
	PM_callRealMode,
	PM_int86,
	PM_int86x,
	DPMI_int86,
	PM_availableMemory,
	PM_getVESABuf,
	PM_getOSType,
	PM_fatalError,
	PM_setBankA,
	PM_setBankAB,
	PM_setCRTStart,
	PM_getCurrentPath,
	PM_getVBEAFPath,
	PM_getNucleusPath,
	PM_getNucleusConfigPath,
	PM_getUniqueID,
	PM_getMachineName,
	VF_available,
	VF_init,
	VF_exit,
	PM_openConsole,
	PM_getConsoleStateSize,
	PM_saveConsoleState,
	PM_restoreConsoleState,
	PM_closeConsole,
	PM_setOSCursorLocation,
	PM_setOSScreenWidth,
	PM_enableWriteCombine,
	PM_backslash,
	PM_lockDataPages,
	PM_unlockDataPages,
	PM_lockCodePages,
	PM_unlockCodePages,
	PM_setRealTimeClockHandler,
	PM_setRealTimeClockFrequency,
	PM_restoreRealTimeClockHandler,
	PM_doBIOSPOST,
	PM_getBootDrive,
	PM_freePhysicalAddr,
	PM_inpb,
	PM_inpw,
	PM_inpd,
	PM_outpb,
	PM_outpw,
	PM_outpd,
	PM_changeResolution,
	PM_setSuspendAppCallback,
	PM_haveBIOSAccess,
	PM_kbhit,
	PM_getch,
	PM_findBPD,
	PM_getPhysicalAddr,
	PM_sleep,
	PM_getCOMPort,
	PM_getLPTPort,
	PM_loadLibrary,
	PM_getProcAddress,
	PM_freeLibrary,
	PCI_enumerate,
	PCI_accessReg,
	PCI_setHardwareIRQ,
	PCI_generateSpecialCyle,
	PCI_mapShared,
	PCIBIOS_getEntry,
	CPU_getProcessorType,
	CPU_haveMMX,
	CPU_have3DNow,
	CPU_haveSSE,
	CPU_haveRDTSC,
	CPU_getProcessorSpeed,
	ZTimerInit,
	LZTimerOn,
	LZTimerLap,
	LZTimerOff,
	LZTimerCount,
	LZTimerOnExt,
	LZTimerLapExt,
	LZTimerOffExt,
	LZTimerCountExt,
	ULZTimerOn,
	ULZTimerLap,
	ULZTimerOff,
	ULZTimerCount,
	ULZReadTime,
	ULZElapsedTime,
	ULZTimerResolution,
	PM_findFirstFile,
	PM_findNextFile,
	PM_findClose,
	PM_makepath,
	PM_splitpath,
	PM_driveValid,
	PM_getdcwd,
	PM_setFileAttr,
	PM_mkdir,
	PM_rmdir,
	PM_getFileAttr,
	PM_getFileTime,
	PM_setFileTime,
	CPU_getProcessorName,
	PM_getVGAStateSize,
	PM_saveVGAState,
	PM_restoreVGAState,
	PM_vgaBlankDisplay,
	PM_vgaUnblankDisplay,
	};
