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
;* Language:    80386 Assembler, NASM or TASM
;* Environment: IBM PC 32 bit Protected Mode.
;*
;* Description:	Assembly support functions for the Nucleus library for
;*				the high resolution timing support functions provided by
;*				the Intel Pentium and compatible processors.
;*
;****************************************************************************

		IDEAL

include	"scitech.mac"			; Memory model macros

header  _gatimer

begcodeseg  _gatimer

ifdef	USE_NASM
%macro mCPU_ID 0
db	00Fh,0A2h
%endmacro
else
MACRO	mCPU_ID
db	00Fh,0A2h
ENDM
endif

ifdef	USE_NASM
%macro mRDTSC 0
db	00Fh,031h
%endmacro
else
MACRO	mRDTSC
db	00Fh,031h
ENDM
endif

;----------------------------------------------------------------------------
; bool _GA_haveCPUID(void)
;----------------------------------------------------------------------------
; Determines if we have support for the CPUID instruction.
;----------------------------------------------------------------------------
cprocstart	_GA_haveCPUID

		enter_c
		pushfd						; Get original EFLAGS
		pop		eax
		mov 	ecx, eax
		xor		eax, 200000h		; Flip ID bit in EFLAGS
		push	eax					; Save new EFLAGS value on stack
		popfd						; Replace current EFLAGS value
		pushfd						; Get new EFLAGS
		pop     eax					; Store new EFLAGS in EAX
		xor     eax, ecx			; Can not toggle ID bit,
		jnz     @@1					; Processor=80486
		mov		eax,0				; We dont have CPUID support
		jmp		@@Done
@@1:	mov		eax,1				; We have CPUID support
@@Done:	leave_c
		ret

cprocend

;----------------------------------------------------------------------------
; uint _GA_getCPUIDFeatures(void)
;----------------------------------------------------------------------------
; Determines the CPU type using the CPUID instruction.
;----------------------------------------------------------------------------
cprocstart	_GA_getCPUIDFeatures

		enter_c

		xor     eax, eax			; Set up for CPUID instruction
		mCPU_ID						; Get and save vendor ID
		cmp     eax, 1				; Make sure 1 is valid input for CPUID
		jl      @@Fail				; We dont have the CPUID instruction
		xor     eax, eax
		inc		eax
		mCPU_ID						; Get family/model/stepping/features
		mov		eax, edx
@@Done:	leave_c
		ret

@@Fail:	xor		eax,eax
		jmp		@@Done

cprocend

;----------------------------------------------------------------------------
; void  _GA_readTimeStamp(GA_largeInteger *time)
;----------------------------------------------------------------------------
; Reads the time stamp counter and returns the 64-bit result.
;----------------------------------------------------------------------------
cprocstart	_GA_readTimeStamp

		mRDTSC
		mov		ecx,[esp+4]		; Access directly without stack frame
		mov		[ecx],eax
		mov		[ecx+4],edx
		ret

cprocend

;----------------------------------------------------------------------------
; N_uint32 GA_TimerDifference(GA_largeInteger *a,GA_largeInteger *b)
;----------------------------------------------------------------------------
; Computes the difference between two 64-bit numbers (a-b)
;----------------------------------------------------------------------------
cprocstart	GA_TimerDifference

		ARG		a:DPTR, b:DPTR, t:DPTR

		enter_c

		mov		ecx,[a]
		mov 	eax,[ecx]		; EAX := b.low
		mov		ecx,[b]
		sub 	eax,[ecx]
		mov 	edx,eax			; EDX := low difference
		mov		ecx,[a]
		mov 	eax,[ecx+4]		; ECX := b.high
		mov		ecx,[b]
		sbb 	eax,[ecx+4]		; EAX := high difference
		mov		eax,edx			; Return low part

		leave_c
		ret

cprocend

; Macro to delay briefly to ensure that enough time has elapsed between
; successive I/O accesses so that the device being accessed can respond
; to both accesses even on a very fast PC.

ifdef	USE_NASM
%macro	DELAY_TIMER 0
		jmp		short $+2
		jmp		short $+2
		jmp		short $+2
%endmacro
else
macro	DELAY_TIMER
		jmp		short $+2
		jmp		short $+2
		jmp		short $+2
endm
endif

;----------------------------------------------------------------------------
; void _OS_delay8253(N_uint32 microSeconds);
;----------------------------------------------------------------------------
; Delays for the specified number of microseconds, by directly programming
; the 8253 timer chips.
;----------------------------------------------------------------------------
cprocstart	_OS_delay8253

		ARG     microSec:UINT

		enter_c

; Start timer 2 counting

		mov		_ax,[microSec]		; EAX := count in microseconds
		mov		ecx,1196
		mul    	ecx
		mov		ecx,1000
		div		ecx
		mov     ecx,eax				; ECX := count in timer ticks
		in      al,61h
		or      al,1
		out     61h,al

; Set the timer 2 count to 0 again to start the timing interval.

		mov		al,10110100b		; set up to load initial (timer 2)
		out		43h,al				; timer count
		DELAY_TIMER
		sub		al,al
		out		42h,al				; load count lsb
		DELAY_TIMER
		out		42h,al				; load count msb

@@LoopStart:
		mov		al,00000000b		; latch timer 0
		out		43h,al
		DELAY_TIMER
		in		al,42h				; least significant byte
		DELAY_TIMER
		mov		ah,al
		in		al,42h				; most significant byte
		xchg	ah,al
		neg		ax					; Convert from countdown remaining
									;  to elapsed count
		cmp     ax,cx				; Has delay expired?
		jb		@@LoopStart			; No, so loop till done

; Stop timer 2 from counting

		in      al,61H
		and     al,0FEh
		out     61H,al

		leave_c
		ret

cprocend

;----------------------------------------------------------------------------
; int _OS_inpb(int port)
;----------------------------------------------------------------------------
; Reads a byte from the specified port
;----------------------------------------------------------------------------
cprocstart	_OS_inpb

		ARG		port:UINT

		push	_bp
		mov		_bp,_sp
		xor		_ax,_ax
		mov		_dx,[port]
		in		al,dx
		pop		_bp
		ret

cprocend

;----------------------------------------------------------------------------
; int _OS_inpw(int port)
;----------------------------------------------------------------------------
; Reads a word from the specified port
;----------------------------------------------------------------------------
cprocstart	_OS_inpw

		ARG		port:UINT

		push	_bp
		mov		_bp,_sp
		xor		_ax,_ax
		mov		_dx,[port]
		in		ax,dx
		pop		_bp
		ret

cprocend

;----------------------------------------------------------------------------
; ulong _OS_inpd(int port)
;----------------------------------------------------------------------------
; Reads a word from the specified port
;----------------------------------------------------------------------------
cprocstart	_OS_inpd

		ARG		port:UINT

		push	_bp
		mov		_bp,_sp
		mov		_dx,[port]
		in		eax,dx
ifndef flatmodel
		shld	edx,eax,16		; DX:AX = result
endif
		pop		_bp
		ret

cprocend

;----------------------------------------------------------------------------
; void _OS_outpb(int port,int value)
;----------------------------------------------------------------------------
; Write a byte to the specified port.
;----------------------------------------------------------------------------
cprocstart	_OS_outpb

		ARG		port:UINT, value:UINT

		push	_bp
		mov		_bp,_sp
		mov		_dx,[port]
		mov		_ax,[value]
		out		dx,al
		pop		_bp
		ret

cprocend

;----------------------------------------------------------------------------
; void _OS_outpw(int port,int value)
;----------------------------------------------------------------------------
; Write a word to the specified port.
;----------------------------------------------------------------------------
cprocstart	_OS_outpw

		ARG		port:UINT, value:UINT

		push	_bp
		mov		_bp,_sp
		mov		_dx,[port]
		mov		_ax,[value]
		out		dx,ax
		pop		_bp
		ret

cprocend

;----------------------------------------------------------------------------
; void _OS_outpd(int port,ulong value)
;----------------------------------------------------------------------------
; Write a word to the specified port.
;----------------------------------------------------------------------------
cprocstart	_OS_outpd

		ARG		port:UINT, value:ULONG

		push	_bp
		mov		_bp,_sp
		mov		_dx,[port]
		mov		eax,[value]
		out		dx,eax
		pop		_bp
		ret

cprocend

endcodeseg  _gatimer

		END
