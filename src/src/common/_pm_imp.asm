;****************************************************************************
;*
;*					SciTech OS Portability Manager Library
;*
;*               Copyright (C) 1991-1998 SciTech Software, Inc.
;*                            All rights reserved.
;*
;*  ======================================================================
;*  |REMOVAL OR MODIFICATION OF THIS HEADER IS STRICTLY PROHIBITED BY LAW|
;*  |                                                                    |
;*  |This copyrighted computer code contains proprietary technology      |
;*  |owned by SciTech Software, Inc., located at 505 Wall Street,        |
;*  |Chico, CA 95928 USA (http://www.scitechsoft.com).                   |
;*  |                                                                    |
;*  |The contents of this file are subject to the SciTech Nucleus        |
;*  |License; you may *not* use this file or related software except in  |
;*  |compliance with the License. You may obtain a copy of the License   |
;*  |at http://www.scitechsoft.com/nucleus-license.txt                   |
;*  |                                                                    |
;*  |Software distributed under the License is distributed on an         |
;*  |"AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or      |
;*  |implied. See the License for the specific language governing        |
;*  |rights and limitations under the License.                           |
;*  |                                                                    |
;*  |REMOVAL OR MODIFICATION OF THIS HEADER IS STRICTLY PROHIBITED BY LAW|
;*  ======================================================================
;*
;* Language:    TASM 4.0 or NASM
;* Environment: IBM PC 32 bit Protected Mode.
;*
;* Description: Module to implement the import stubs for all the PM
;*              API functions for Intel binary portable drivers.
;*
;****************************************************************************

		IDEAL

include "scitech.mac"           ; Memory model macros

BEGIN_IMPORTS_DEF	_PM_imports
DECLARE_IMP PM_getModeType
DECLARE_IMP PM_getBIOSPointer
DECLARE_IMP PM_getA0000Pointer
DECLARE_IMP PM_mapPhysicalAddr
DECLARE_IMP PM_mallocShared
DECLARE_IMP	PM_mapShared
DECLARE_IMP PM_freeShared
DECLARE_IMP PM_mapToProcess
DECLARE_IMP PM_mapRealPointer
DECLARE_IMP PM_allocRealSeg
DECLARE_IMP PM_freeRealSeg
DECLARE_IMP PM_allocLockedMem
DECLARE_IMP PM_freeLockedMem
DECLARE_IMP PM_callRealMode
DECLARE_IMP PM_int86
DECLARE_IMP PM_int86x
DECLARE_IMP DPMI_int86
DECLARE_IMP PM_availableMemory
DECLARE_IMP PM_getVESABuf
DECLARE_IMP PM_getOSType
DECLARE_IMP PM_fatalError
DECLARE_IMP PM_setBankA
DECLARE_IMP PM_setBankAB
DECLARE_IMP	PM_setCRTStart
DECLARE_IMP PM_getCurrentPath
DECLARE_IMP PM_getVBEAFPath
DECLARE_IMP PM_getNucleusPath
DECLARE_IMP PM_getNucleusConfigPath
DECLARE_IMP PM_getUniqueID
DECLARE_IMP PM_getMachineName
DECLARE_IMP VF_available
DECLARE_IMP VF_init
DECLARE_IMP VF_exit
DECLARE_IMP PM_openConsole
DECLARE_IMP PM_getConsoleStateSize
DECLARE_IMP PM_saveConsoleState
DECLARE_IMP PM_restoreConsoleState
DECLARE_IMP PM_closeConsole
DECLARE_IMP PM_setOSCursorLocation
DECLARE_IMP PM_setOSScreenWidth
DECLARE_IMP PM_enableWriteCombine
DECLARE_IMP PM_backslash
DECLARE_IMP PM_lockDataPages
DECLARE_IMP PM_unlockDataPages
DECLARE_IMP PM_lockCodePages
DECLARE_IMP PM_unlockCodePages
DECLARE_IMP PM_setRealTimeClockHandler
DECLARE_IMP PM_setRealTimeClockFrequency
DECLARE_IMP PM_restoreRealTimeClockHandler
DECLARE_IMP PM_doBIOSPOST
DECLARE_IMP PM_getBootDrive
DECLARE_IMP PM_freePhysicalAddr
DECLARE_IMP PM_inpb
DECLARE_IMP PM_inpw
DECLARE_IMP PM_inpd
DECLARE_IMP PM_outpb
DECLARE_IMP PM_outpw
DECLARE_IMP PM_outpd
DECLARE_IMP PM_changeResolution
DECLARE_IMP PM_setSuspendAppCallback
DECLARE_IMP PM_haveBIOSAccess
DECLARE_IMP PM_kbhit
DECLARE_IMP PM_getch
DECLARE_IMP PM_findBPD
DECLARE_IMP PM_getPhysicalAddr
DECLARE_IMP PM_sleep
DECLARE_IMP PM_getCOMPort
DECLARE_IMP PM_getLPTPort
DECLARE_IMP PM_loadLibrary
DECLARE_IMP PM_getProcAddress
DECLARE_IMP PM_freeLibrary
DECLARE_IMP PCI_enumerate
DECLARE_IMP PCI_accessReg
DECLARE_IMP PCI_setHardwareIRQ
DECLARE_IMP PCI_generateSpecialCyle
DECLARE_IMP PCI_mapShared
DECLARE_IMP PCIBIOS_getEntry
DECLARE_IMP CPU_getProcessorType
DECLARE_IMP CPU_haveMMX
DECLARE_IMP CPU_have3DNow
DECLARE_IMP CPU_haveSSE
DECLARE_IMP CPU_haveRDTSC
DECLARE_IMP CPU_getProcessorSpeed
DECLARE_IMP ZTimerInit
DECLARE_IMP LZTimerOn
DECLARE_IMP LZTimerLap
DECLARE_IMP LZTimerOff
DECLARE_IMP LZTimerCount
DECLARE_IMP LZTimerOnExt
DECLARE_IMP LZTimerLapExt
DECLARE_IMP LZTimerOffExt
DECLARE_IMP LZTimerCountExt
DECLARE_IMP ULZTimerOn
DECLARE_IMP ULZTimerLap
DECLARE_IMP ULZTimerOff
DECLARE_IMP ULZTimerCount
DECLARE_IMP ULZReadTime
DECLARE_IMP ULZElapsedTime
DECLARE_IMP ULZTimerResolution
DECLARE_IMP PM_findFirstFile
DECLARE_IMP PM_findNextFile
DECLARE_IMP PM_findClose
DECLARE_IMP PM_makepath
DECLARE_IMP PM_splitpath
DECLARE_IMP PM_driveValid
DECLARE_IMP PM_getdcwd
DECLARE_IMP PM_setFileAttr
DECLARE_IMP PM_mkdir
DECLARE_IMP PM_rmdir
DECLARE_IMP PM_getFileAttr
DECLARE_IMP PM_getFileTime
DECLARE_IMP PM_setFileTime
DECLARE_IMP CPU_getProcessorName
DECLARE_IMP PM_getVGAStateSize
DECLARE_IMP PM_saveVGAState
DECLARE_IMP PM_restoreVGAState
DECLARE_IMP PM_vgaBlankDisplay
DECLARE_IMP PM_vgaUnblankDisplay
END_IMPORTS_DEF

		END
