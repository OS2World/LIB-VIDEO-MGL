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
;* Language:    ANSI C
;* Environment: 16-bit Windows driver environment.
;*
;* Description: Header file for the 16->32 bit thunks for 16-bit display
;*				drivers calling the 32-bit flat model Nucleus drivers.
;*
;****************************************************************************

;----------------------------------------------------------------------------
; Fundamental types
;----------------------------------------------------------------------------

typedef N_int8		byte
typedef N_int16		word
typedef	N_int32		dword
typedef N_uint8		byte
typedef N_uint16	word
typedef	N_uint32	dword
typedef N_fix32		dword
typedef N_flt		dword

;----------------------------------------------------------------------------
; Nucleus Graphics Architecture ASM functions structure
;----------------------------------------------------------------------------

;----------------------------------------------------------------------------
; REF2D_driver functions structure
;----------------------------------------------------------------------------

struc	REF2D_driver_s
QueryFunctionsRef2d_Asm	cptr       ?
SetDrawBufferRef2d		cptr       ?
RotateBitmapRef2d_Asm	cptr       ?
MapSharedRef2d			cptr       ?
UnloadDriverRef2d		cptr       ?
ends	REF2D_driver_s

;----------------------------------------------------------------------------
; Device Driver Hardware 2D State Management functions structure
;----------------------------------------------------------------------------

struc	GA_2DStateFuncs_s
dwSize					ulong		?
SetDrawBuffer 			cptr		?
IsIdle                  cptr		?
WaitTillIdle            cptr		?
EnableDirectAccess      cptr		?
DisableDirectAccess     cptr		?
SetMix                  cptr		?
SetForeColor            cptr		?
SetBackColor            cptr		?
Set8x8MonoPattern       cptr		?
Use8x8MonoPattern       cptr		?
Use8x8TransMonoPattern  cptr		?
Set8x8ColorPattern      cptr		?
Use8x8ColorPattern      cptr		?
Use8x8TransColorPattern cptr		?
SetLineStipple          cptr		?
SetLineStippleCount     cptr		?
SetPlaneMask			cptr		?
SetSrcBlendFunc			cptr		?
SetDstBlendFunc         cptr		?
SetAlphaValue           cptr		?
ends	GA_2DStateFuncs_s

;----------------------------------------------------------------------------
; Device Driver Hardware 2D Rendering functions structure
;----------------------------------------------------------------------------

struc	GA_2DRenderFuncs_s
dwSize						ulong		?
GetPixel					cptr		?
PutPixel                    cptr		?
DrawScanList                cptr		?
DrawPattScanList            cptr		?
DrawColorPattScanList       cptr		?
DrawEllipseList             cptr		?
DrawPattEllipseList         cptr		?
DrawColorPattEllipseList    cptr		?
DrawFatEllipseList          cptr		?
DrawPattFatEllipseList      cptr		?
DrawColorPattFatEllipseList	cptr		?
DrawRect                    cptr		?
DrawRectLin                 cptr		?
DrawPattRect                cptr		?
DrawColorPattRect           cptr		?
DrawTrap                    cptr		?
DrawPattTrap                cptr		?
DrawColorPattTrap           cptr		?
DrawLineInt                 cptr		?
DrawBresenhamLine			cptr		?
DrawStippleLineInt          cptr		?
DrawBresenhamStippleLine	cptr		?
DrawEllipse                 cptr		?
ClipEllipse                 cptr		?
PutMonoImageMSBSys          cptr		?
PutMonoImageMSBLin          cptr		?
PutMonoImageMSBBM           cptr		?
PutMonoImageLSBSys          cptr		?
PutMonoImageLSBLin          cptr		?
PutMonoImageLSBBM           cptr		?
ClipMonoImageMSBSys         cptr		?
ClipMonoImageMSBLin         cptr		?
ClipMonoImageMSBBM          cptr		?
ClipMonoImageLSBSys         cptr		?
ClipMonoImageLSBLin         cptr		?
ClipMonoImageLSBBM          cptr		?
BitBlt                      cptr		?
BitBltLin                   cptr		?
BitBltSys                   cptr		?
BitBltBM                    cptr		?
BitBltPatt					cptr		?
BitBltPattLin               cptr		?
BitBltPattSys               cptr		?
BitBltPattBM                cptr		?
BitBltColorPatt             cptr		?
BitBltColorPattLin          cptr		?
BitBltColorPattSys          cptr		?
BitBltColorPattBM           cptr		?
SrcTransBlt                 cptr		?
SrcTransBltLin              cptr		?
SrcTransBltSys              cptr		?
SrcTransBltBM               cptr		?
DstTransBlt                 cptr		?
DstTransBltLin              cptr		?
DstTransBltSys              cptr		?
DstTransBltBM               cptr		?
StretchBlt                  cptr		?
StretchBltLin               cptr		?
StretchBltSys               cptr		?
StretchBltBM                cptr		?
ConvertBltSys               cptr		?
ConvertBltBM                cptr		?
StretchConvertBltSys        cptr		?
StretchConvertBltBM         cptr		?
BitBltFxTest                cptr		?
BitBltFx                    cptr		?
BitBltFxLin                 cptr		?
BitBltFxSys                 cptr		?
BitBltFxBM                  cptr		?
GetBitmapSys                cptr		?
GetBitmapBM                 cptr		?
ends	GA_2DRenderFuncs_s

;----------------------------------------------------------------------------
; Win16 functions structure
;----------------------------------------------------------------------------

struc 	GA_win16Funcs_s
dwSize						ulong		?
WaitTillIdleW16    			cptr		?
EnableDirectAccessW16   	cptr		?
DisableDirectAccessW16  	cptr		?
SetCursorPosAsmW16     		cptr		?
WaitTillIdleW16_Len			ulong		?
EnableDirectAccessW16_Len	ulong		?
DisableDirectAccessW16_Len	ulong		?
SetCursorPosAsmW16_Len		ulong		?
CursorState					ulong		?
ends	GA_win16Funcs_s

;----------------------------------------------------------------------------
; Cursor functions structure
;----------------------------------------------------------------------------

struc	GA_cursorFuncs_s
dwSize					ulong		?
SetMonoCursor       	cptr		?
SetMonoCursorColor      cptr		?
SetColorCursor          cptr		?
SetCursorPos            cptr		?
ShowCursor              cptr		?
BeginAccess				cptr		?
EndAccess               cptr		?
ends	GA_cursorFuncs_s

;----------------------------------------------------------------------------
; Video functions structure
;----------------------------------------------------------------------------

struc	GA_videoFuncs_s
dwSize					ulong		?
SetVideoInput      		cptr		?
SetVideoOutput          cptr		?
SetVideoColorKey        cptr		?
StartVideoFrame         cptr		?
EndVideoFrame           cptr		?
ends	GA_videoFuncs_s

;----------------------------------------------------------------------------
; Init functions structure
;----------------------------------------------------------------------------

struc	GA_initFuncs_s
dwSize					ulong		?
GetConfigInfo           cptr		?
GetModeProfile          cptr		?
SetModeProfile          cptr		?
GetOptions              cptr		?
SetOptions              cptr		?
GetVideoModeInfo        cptr		?
SetVideoMode            cptr		?
GetVideoMode            cptr		?
GetCustomVideoModeInfo  cptr		?
SetCustomVideoMode      cptr		?
GetClosestPixelClock    cptr		?
GetCRTCTimings          cptr		?
SetCRTCTimings          cptr		?
SetGlobalRefresh        cptr		?
SaveRestoreState        cptr		?
SetDisplayOutput        cptr		?
GetDisplayOutput        cptr		?
SetSoftwareRenderFuncs  cptr		?
GetUniqueFilename       cptr		?
GetCRTCTable            cptr		?
GetMonitorInfo          cptr		?
SetMonitorInfo          cptr		?
ends	GA_initFuncs_s

;----------------------------------------------------------------------------
; Driver functions structure
;----------------------------------------------------------------------------

struc	GA_driverFuncs_s
dwSize					ulong		?
SetBank                 cptr		?
SetDisplayStart         cptr		?
SetDisplayStartXY       cptr		?
SetStereoDisplayStart   cptr		?
GetDisplayStartStatus   cptr		?
EnableStereoMode        cptr		?
IsVSync                 cptr		?
WaitVSync               cptr		?
SetVSyncWidth           cptr		?
GetVSyncWidth           cptr		?
SetPaletteData          cptr		?
GetPaletteData          cptr		?
SetGammaCorrectData     cptr		?
GetGammaCorrectData     cptr		?
GetCurrentScanLine      cptr		?
SetPaletteDataExt       cptr		?
GetPaletteDataExt       cptr		?
SetGammaCorrectDataExt  cptr		?
GetGammaCorrectDataExt  cptr		?
ends	GA_driverFuncs_s
