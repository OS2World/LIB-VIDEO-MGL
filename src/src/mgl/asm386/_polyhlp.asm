;****************************************************************************
;*
;*					SciTech Multi-platform Graphics Library
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
;* Language:    80386 Assembler, TASM or NASM
;* Environment:	Intel x86
;*
;* Description:	32 bit optimised assembly language routines for the polygon
;*				rendering helper functions.
;*
;****************************************************************************

		IDEAL

include	"scitech.mac"			; Memory model macros

header	_polyhlp				; Setup for MGL hybrid 16/32 bit segment

begcodeseg	_polyhlp

;----------------------------------------------------------------------------
; int _MGL_computeSlope(fxpoint_t *v1,fxpoint_t *v2,fix32_t *slope)
;----------------------------------------------------------------------------
; Compute the slope of the edge and return the sign of the slope.
;
; Entry:		v1		- First vertex in edge
;				v2		- Second vertex in edge
;               slope	- Pointer to place to store slope
;
; Exit:      	_AX		- Sign of slope (1 = +ve, 0, -1 = i-ve)
;
;----------------------------------------------------------------------------
cprocstart	_MGL_computeSlope

		ARG 	v1:DPTR, v2:DPTR, slope:DPTR

		push	ebp
		mov		ebp,esp
		push	ebx
		push	esi

		mov		ecx,[v2]
		mov		ebx,[v1]
		mov		esi,[ecx+4]
		sub		esi,[ebx+4]		; ESI := v2.y - v1.y
		jle		@@InvalidEdge	; Check for negative/0 edges
		cmp		esi,10000h
		jle		@@QuickSlope	; Handle divide overflow

		mov		eax,[ecx]
		sub		eax,[ebx]		; EAX := v2.x - v1.x
		mov		edx,eax
		xor		eax,eax
		mov		ebx,[slope]
		shrd	eax,edx,16		; position so that result ends up
		sar		edx,16			; in EAX
		idiv	esi
		mov		[ebx],eax		; Store the resulting slope
		mov		eax,1			; Positive edge

@@Exit:
		pop		esi
		pop		ebx
		pop		ebp
		ret

@@QuickSlope:
		mov		eax,[ecx]
		sub		eax,[ebx]		; EAX := v2.x - v1.x
		mov		ebx,[slope]
		mov		[ebx],eax		; Store the resulting slope
		mov		eax,1			; Positive edge
		jmp		@@Exit

@@InvalidEdge:
		mov     eax,-1			; Negative edge
		jl		@@Exit
		xor		eax,eax			; Zero height edge
		jmp		@@Exit

cprocend

endcodeseg	_polyhlp

		END						; End of module
