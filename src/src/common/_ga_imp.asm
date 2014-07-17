;****************************************************************************
;*
;*           		SciTech Nucleus Graphics Architecture
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
;* Description: Module to implement the import stubs for all the Nucleus
;*              Graphics API functions for Intel binary compatible drivers.
;*
;****************************************************************************

		IDEAL

include "scitech.mac"           ; Memory model macros

BEGIN_IMPORTS_DEF   _GA_exports
DECLARE_IMP __GA_status						; Implemented in C code
DECLARE_IMP __GA_errorMsg                   ; Implemented in C code
DECLARE_IMP __GA_getDaysLeft				; Implemented in C code
DECLARE_IMP __GA_registerLicense			; Implemented in C code
DECLARE_IMP __GA_enumerateDevices			; Implemented in C code
DECLARE_IMP __GA_loadDriver					; Implemented in C code
DECLARE_IMP GA_setActiveDevice
DECLARE_IMP __GA_mapShared					; Implemented in C code
DECLARE_IMP GA_unloadDriver
DECLARE_IMP REF2D_loadDriver
DECLARE_IMP REF2D_unloadDriver
DECLARE_IMP GA_loadRef2d
DECLARE_IMP GA_unloadRef2d
DECLARE_IMP GA_softStereoInit
DECLARE_IMP GA_softStereoOn
DECLARE_IMP GA_softStereoScheduleFlip
DECLARE_IMP GA_softStereoGetFlipStatus
DECLARE_IMP GA_softStereoWaitTillFlipped
DECLARE_IMP GA_softStereoOff
DECLARE_IMP GA_softStereoExit
DECLARE_IMP GA_saveModeProfile
DECLARE_IMP GA_saveOptions
DECLARE_IMP GA_saveCRTCTimings
DECLARE_IMP GA_restoreCRTCTimings
DECLARE_IMP DDC_init
DECLARE_IMP DDC_readEDID
DECLARE_IMP EDID_parse
DECLARE_IMP MCS_begin
DECLARE_IMP MCS_getCapabilitiesString
DECLARE_IMP MCS_isControlSupported
DECLARE_IMP MCS_enableControl
DECLARE_IMP MCS_getControlMax
DECLARE_IMP MCS_getControlValue
DECLARE_IMP MCS_getControlValues
DECLARE_IMP MCS_setControlValue
DECLARE_IMP MCS_setControlValues
DECLARE_IMP MCS_resetControl
DECLARE_IMP MCS_saveCurrentSettings
DECLARE_IMP MCS_getTimingReport
DECLARE_IMP MCS_getSelfTestReport
DECLARE_IMP MCS_end
DECLARE_IMP __GA_loadInGUI				; Implemented in C code
DECLARE_IMP DDC_writeEDID
DECLARE_IMP GA_useDoubleScan
DECLARE_IMP GA_getMaxRefreshRate
DECLARE_IMP GA_computeCRTCTimings
DECLARE_IMP GA_addMode
DECLARE_IMP GA_addRefresh
DECLARE_IMP GA_delMode
DECLARE_IMP GA_getLogName
DECLARE_IMP GA_log
DECLARE_IMP MDBX_getErrCode
DECLARE_IMP MDBX_getErrorMsg
DECLARE_IMP __MDBX_open						; Implemented in C code
DECLARE_IMP MDBX_close
DECLARE_IMP MDBX_first
DECLARE_IMP MDBX_last
DECLARE_IMP MDBX_next
DECLARE_IMP MDBX_prev
DECLARE_IMP MDBX_insert
DECLARE_IMP MDBX_update
DECLARE_IMP MDBX_flush
DECLARE_IMP MDBX_importINF
DECLARE_IMP __GA_getGlobalOptions			; Implemented in C code
DECLARE_IMP __GA_setGlobalOptions			; Implemented in C code
DECLARE_IMP GA_saveGlobalOptions
DECLARE_IMP GA_getInternalName
DECLARE_IMP GA_getNucleusConfigPath
END_IMPORTS_DEF

		END
