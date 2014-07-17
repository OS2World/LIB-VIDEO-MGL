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
;* Language:    NASM or TASM
;* Environment: IBM PC 32 bit Protected Mode.
;*
;* Description:	Assembly support functions for the Nucleus library for
;*				Win32. This mostly contains 32-bit thunks to call into
;*				Ring 0 for certain device driver helper functions that
;*				need to run with full hardware privledges.
;*
;****************************************************************************

		IDEAL

include	"scitech.mac"			; Memory model macros

header  _gawin32

begdataseg	_gawin32

	cextern		_GA_cntDevice,UINT

enddataseg	_gawin32

begcodeseg  _gawin32

MACRO	declare_thunk deviceIndex, func,
	cexternfunc func,NEAR
cprocstart	func&_&deviceIndex
		mov 	[_GA_cntDevice],deviceIndex
		jmp		func
cprocend
ENDM

MACRO	declare_thunks	deviceIndex
declare_thunk deviceIndex, _GA_SetOptions
declare_thunk deviceIndex, _GA_SetVideoMode
declare_thunk deviceIndex, _GA_SetCustomVideoMode
declare_thunk deviceIndex, _GA_SetCRTCTimings
declare_thunk deviceIndex, _GA_SaveRestoreState
declare_thunk deviceIndex, _GA_SetDisplayOutput
declare_thunk deviceIndex, _GA_SetBank
declare_thunk deviceIndex, _GA_SetDisplayStart
declare_thunk deviceIndex, _GA_SetDisplayStartXY
declare_thunk deviceIndex, _GA_SetStereoDisplayStart
declare_thunk deviceIndex, _GA_GetDisplayStartStatus
declare_thunk deviceIndex, _GA_EnableStereoMode
declare_thunk deviceIndex, _GA_IsVSync
declare_thunk deviceIndex, _GA_WaitVSync
declare_thunk deviceIndex, _GA_SetVSyncWidth
declare_thunk deviceIndex, _GA_GetVSyncWidth
declare_thunk deviceIndex, _GA_SetPaletteData
declare_thunk deviceIndex, _GA_GetPaletteData
declare_thunk deviceIndex, _GA_SetGammaCorrectData
declare_thunk deviceIndex, _GA_GetGammaCorrectData
declare_thunk deviceIndex, _GA_GetCurrentScanLine
declare_thunk deviceIndex, _GA_SetPaletteDataExt
declare_thunk deviceIndex, _GA_GetPaletteDataExt
declare_thunk deviceIndex, _GA_SetGammaCorrectDataExt
declare_thunk deviceIndex, _GA_GetGammaCorrectDataExt
ENDM

declare_thunks	0
declare_thunks	1
declare_thunks	2
declare_thunks	3

endcodeseg  _gawin32

		END
