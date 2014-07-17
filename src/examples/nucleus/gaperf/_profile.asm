;****************************************************************************
;*
;*                         SciTech Display Doctor
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
;* Language:    80386 Assembler
;* Environment: Intel x86
;*
;* Description: Assembly language test routines for the Nucleus
;*              performance profiling program.
;*
;****************************************************************************

        IDEAL

include "scitech.mac"

header  _profile                    ; Set up memory model

begcodeseg  _profile

;----------------------------------------------------------------------------
; void clearBufLin(void *buffer,long value,uint len)
;----------------------------------------------------------------------------
cprocstart   clearBufLin

		ARG     buffer:DPTR, value:ULONG, len:UINT

		enter_c

		mov     edi,[buffer]    ; edi -> memory block
		mov     eax,[value]     ; EAX := value to store
		mov		ah,al
		mov		ecx,eax
		shl		ecx,16
		or		eax,ecx
		mov     ecx,[len]
		shr     ecx,2           ; Convert from bytes to DWORDS
		cld
	rep stosd                   ; Store all middle DWORD's fast!

		leave_c
		ret

cprocend

;----------------------------------------------------------------------------
; void clearBufDownLin(void *buffer,long value,uint len)
;----------------------------------------------------------------------------
cprocstart   clearBufDownLin

		ARG     buffer:DPTR, value:ULONG, len:UINT

		enter_c

		mov     edi,[buffer]    ; edi -> memory block
		mov     eax,[value]     ; EAX := value to store
		mov		ah,al
		mov		ecx,eax
		shl		ecx,16
		or		eax,ecx
		mov     ecx,[len]
		add		edi,ecx
		sub		edi,4			; edi -> end of memory block
		shr     ecx,2           ; Convert from bytes to DWORDS
		std						; Go down in video memory
	rep stosd                   ; Store all middle DWORD's fast!

		leave_c
		ret

cprocend

;----------------------------------------------------------------------------
; void readBufLin(void *buffer,uint len)
;----------------------------------------------------------------------------
cprocstart   readBufLin

		ARG     buffer:DPTR, len:UINT

		enter_c

		mov     edi,[buffer]    ; edi -> memory block
		mov     ecx,[len]
		shr		ecx,2			; Convert to DWORDs

@@Loop:	mov		eax,[edi]
		add		edi,4
		dec		ecx
		jnz		@@Loop

		leave_c
		ret

cprocend

;----------------------------------------------------------------------------
; void copyBufLin(void *buffer,char *image,uint len)
;----------------------------------------------------------------------------
cprocstart	copyBufLin

		ARG     buffer:DPTR, image:DPTR, len:UINT

		enter_c

		mov     edi,[buffer]    ; edi -> destination block
		mov     esi,[image]     ; esi -> source block
		mov     ecx,[len]
		shr     ecx,2           ; Convert from bytes to DWORDS
		cld
	rep movsd                   ; Copy all DWORDS's fast!

		leave_c
		ret

cprocend

endcodeseg  _profile

		END
