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
;* Language:   	80386 Assembler (TASM ideal mode)
;* Environment:	IBM PC 32 bit Protected Mode.
;*
;* Description:	Macros and type definitions for the Nucleus public device
;*				context block.
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
typedef N_flt32		dword

typedef	N_pattern	N_uint8
typedef	N_stipple	N_uint16
typedef N_color		N_uint32

;----------------------------------------------------------------------------
; Public device context structure
;----------------------------------------------------------------------------

struc	GA_devCtx_s

;------------------------------------------------------------------
; Device driver header block
;------------------------------------------------------------------
Signature				uchar 20 dup (?)
GAVersion				N_uint32	?
DriverRev           	N_uint32	?
OemVendorName			uchar 80 dup (?)
OemCopyright			uchar 80 dup (?)
AvailableModes			dptr		?
DeviceIndex				N_uint32	?
TotalMemory         	N_uint32	?
Attributes          	N_uint32	?
WorkArounds				N_uint32	?
TextSize                N_uint32	?
TextBasePtr             N_uint32	?
BankSize            	N_uint32	?
BankedBasePtr			N_uint32	?
LinearSize				N_uint32	?
LinearBasePtr       	N_uint32	?
ZBufferSize             N_uint32	?
ZBufferBasePtr          N_uint32	?
TexBufferSize           N_uint32	?
TexBufferBasePtr        N_uint32	?
LockedMemSize           N_uint32	?
IOBase                 	N_uint32	?
MMIOBase				N_uint32 4 dup (?)
MMIOLen					N_uint32 4 dup (?)
DriverStart				cptr		?
DriverSize				N_uint32 	?
res1					N_uint32 20 dup (?)

;------------------------------------------------------------------
; Near pointers mapped by loader for driver
;------------------------------------------------------------------
IOMemMaps				dptr 4 dup (?)
TextMem					dptr		?
BankedMem				dptr		?
LinearMem               dptr		?
ZBufferMem              dptr		?
TexBufferMem            dptr		?
LockedMem               dptr		?
LockedMemPhys           N_uint32	?
TextFont8x8             dptr		?
TextFont8x14            dptr		?
TextFont8x16            dptr		?
VGAPal4                 dptr		?
VGAPal8                 dptr		?
res2					N_uint32 20 dup (?)

;------------------------------------------------------------------
; Driver initialization functions
;------------------------------------------------------------------
InitDriver				cptr		?
QueryFunctions			cptr		?

ends    GA_devCtx_s

GA_devCtx = (GA_devCtx_s PTR DS:EAX)
