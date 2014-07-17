;****************************************************************************
;*
;*					SciTech OS Portability Manager Library
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
;* Language:	80386 Assembler, TASM 4.0 or NASM
;* Environment:	16/32 bit Ring 0 device driver
;*
;* Description:	Assembler support routines for the Memory Type Range Register
;*				(MTRR) module.
;*
;****************************************************************************

		IDEAL

include "scitech.mac"			; Memory model macros

header      _mtrr              	; Set up memory model

begcodeseg  _mtrr              	; Start of code segment

P586

;----------------------------------------------------------------------------
; ibool _MTRR_isRing0(void);
;----------------------------------------------------------------------------
; Checks to see if we are running at ring 0. This check is only relevant
; for 32-bit DOS4GW and compatible programs. If we are not running under
; DOS4GW, then we simply assume we are a ring 0 device driver.
;----------------------------------------------------------------------------
cprocnear	_MTRR_isRing0

; Are we running under CauseWay?

ifdef	DOS4GW
		enter_c
		mov     ax,cs
		and   	eax,3
		xor		eax,3
		jmp		@@Exit
@@Fail:	xor		eax,eax
@@Exit:	leave_c
		ret
else
ifdef	DJGPP
		xor		eax,eax					; Cant do this under DJGPP!
else
		mov		eax,1					; Assume ring 0 for not 32-bit DOS
endif
		ret
endif

cprocend

;----------------------------------------------------------------------------
; ulong _MTRR_disableInt(void);
;----------------------------------------------------------------------------
; Return processor interrupt status and disable interrupts.
;----------------------------------------------------------------------------
cprocstart	_MTRR_disableInt

		pushfd                  ; Put flag word on stack
		cli                     ; Disable interrupts!
		pop		eax             ; deposit flag word in return register
		ret

cprocend

;----------------------------------------------------------------------------
; void _MTRR_restoreInt(ulong ps);
;----------------------------------------------------------------------------
; Restore processor interrupt status.
;----------------------------------------------------------------------------
cprocstart	_MTRR_restoreInt

		ARG		ps:ULONG

		push    ebp
		mov     ebp,esp         ; Set up stack frame
		mov		ecx,[ps]
		test	ecx,200h		; SMP safe interrupt flag restore!
		jz		@@1
		sti
@@1:	pop     ebp
		ret

cprocend

;----------------------------------------------------------------------------
; void _MTRR_flushTLB(void);
;----------------------------------------------------------------------------
; Flush the CPU cache and Translation Lookaside Buffer
;----------------------------------------------------------------------------
cprocnear	_MTRR_flushTLB

		wbinvd
		ret

cprocend

;----------------------------------------------------------------------------
; ulong _MTRR_saveCR4(void);
;----------------------------------------------------------------------------
; Save the value of CR4 and clear the Page Global Enable (bit 7). We also
; disable and flush the caches.
;----------------------------------------------------------------------------
cprocstart	_MTRR_saveCR4

		enter_c

; Save value of CR4 and clear Page Global Enable (bit 7)

		mov		ebx,cr4
		mov		eax,ebx
		and		al,7Fh
		mov		cr4,eax

; Disable and flush caches

		mov     eax,cr0
		or		eax,40000000h
		wbinvd
		mov		cr0,eax
		wbinvd

; Return value from CR4

		mov		eax,ebx
		leave_c
		ret

cprocend

;----------------------------------------------------------------------------
; void _MTRR_restoreCR4(ulong cr4Val)
;----------------------------------------------------------------------------
; Save the value of CR4 and clear the Page Global Enable (bit 7). We also
; disable and flush the caches.
;----------------------------------------------------------------------------
cprocstart	_MTRR_restoreCR4

		ARG		cr4Val:ULONG

		enter_c

; Enable caches

		mov		eax,cr0
		and		eax,0BFFFFFFFh
		mov		cr0,eax
		mov		eax,[cr4Val]
		mov		cr4,eax
		leave_c
		ret

cprocend

;----------------------------------------------------------------------------
; uchar _MTRR_getCx86(uchar reg);
;----------------------------------------------------------------------------
; Read a Cyrix CPU indexed register
;----------------------------------------------------------------------------
cprocstart	_MTRR_getCx86

		ARG		reg:UCHAR

		enter_c
		mov		al,[reg]
		out		22h,al
		in		al,23h
		leave_c
		ret

cprocend

;----------------------------------------------------------------------------
; uchar _MTRR_setCx86(uchar reg,uchar val);
;----------------------------------------------------------------------------
; Write a Cyrix CPU indexed register
;----------------------------------------------------------------------------
cprocstart	_MTRR_setCx86

		ARG		reg:UCHAR, val:UCHAR

		enter_c
		mov		al,[reg]
		out		22h,al
		mov		al,[val]
		out		23h,al
		leave_c
		ret

cprocend

;----------------------------------------------------------------------------
; void _MTRR_readMSR(uong reg, ulong FAR *eax, ulong FAR *edx);
;----------------------------------------------------------------------------
; Writes the specific Machine Status Register used on the newer Intel
; Pentium Pro and Pentium II motherboards.
;----------------------------------------------------------------------------
cprocnear	_MTRR_readMSR

		ARG		reg:ULONG, v_eax:DPTR, v_edx:DPTR

		enter_c
		mov		ecx,[reg]
		rdmsr
		mov		ebx,[v_eax]
		mov		[ebx],eax
		mov		ebx,[v_edx]
		mov		[ebx],edx
		leave_c
		ret

cprocend

;----------------------------------------------------------------------------
; void _MTRR_writeMSR(uong reg, ulong eax, ulong edx);
;----------------------------------------------------------------------------
; Writes the specific Machine Status Register used on the newer Intel
; Pentium Pro and Pentium II motherboards.
;----------------------------------------------------------------------------
cprocnear	_MTRR_writeMSR

		ARG		reg:ULONG, v_eax:ULONG, v_edx:ULONG

		enter_c
		mov		ecx,[reg]
		mov		eax,[v_eax]
		mov		edx,[v_edx]
		wrmsr
		leave_c
		ret

cprocend

endcodeseg  _mtrr

		END						; End of module
