;****************************************************************************
;*
;*          The SuperVGA Kit - UniVBE Software Development Kit
;*
;*  ========================================================================
;*
;*    The contents of this file are subject to the SciTech MGL Public
;*    License Version 1.0 (the "License"); you may not use this file
;*    except in compliance with the License. You may obtain a copy of
;*    the License at http://www.scitechsoft.com/mgl-license.txt
;*
;*    Software distributed under the License is distributed on an
;*    "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
;*    implied. See the License for the specific language governing
;*    rights and limitations under the License.
;*
;*    The Original Code is Copyright (C) 1991-1998 SciTech Software, Inc.
;*
;*    The Initial Developer of the Original Code is SciTech Software, Inc.
;*    All Rights Reserved.
;*
;*  ========================================================================
;*
;* Language:    TASM 4.0 or NASM
;* Environment: IBM PC 32 bit Protected Mode.
;*
;* Description: Module to implement the import subts for all the WinDirect
;*              API functions for Intel binary compatible drivers.
;*
;****************************************************************************

		IDEAL

include "scitech.mac"           ; Memory model macros

BEGIN_IMPORTS_DEF	_WD_imports
DECLARE_IMP WD_startFullScreen
DECLARE_IMP WD_changeResolution
DECLARE_IMP WD_inFullScreen
DECLARE_IMP WD_restoreGDI
DECLARE_IMP WD_setSuspendAppCallback
DECLARE_IMP WD_isSafeMode
DECLARE_IMP WD_getEvent
DECLARE_IMP WD_peekEvent
DECLARE_IMP WD_postEvent
DECLARE_IMP WD_flushEvent
DECLARE_IMP WD_haltEvent
DECLARE_IMP WD_setTimerTick
DECLARE_IMP WD_isKeyDown
DECLARE_IMP WD_getMousePos
DECLARE_IMP WD_setMousePos
DECLARE_IMP WD_setMouseCallback
END_IMPORTS_DEF

		END
