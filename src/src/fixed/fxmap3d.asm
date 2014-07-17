;****************************************************************************
;*
;*				  High Speed Fixed/Floating Point Library
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
;*
;* Language:	80386 Assembler
;* Environment:	IBM PC (MS DOS)
;*
;* Description:	This module contains routines for high performance 16.16
;*				fixed point 3D vector mapping routines:
;*
;*					F386_map3Dto3D		- Map a 3D point given a 4x4 matrix
;*					F386_map3Dto4D		- Map a 3D point given a 4x4 matrix
;*					F386_map4Dto4D		- Map a 4D point given a 4x4 matrix
;*					F386_mapVec3Dto3D	- Map a 3D vector given a 4x4 matrix
;*					F386_mapVec3Dto4D	- Map a 3D vector given a 4x4 matrix
;*					F386_mapVec4Dto4D	- Map a 4D vector given a 4x4 matrix
;*
;*				All of the 4x4 matrix routines take into account special
;*				case matrices to obtain higher performance operation. The
;*				special case flag is passed by the higher level interface.
;*
;*
;****************************************************************************

		IDEAL

include "scitech.mac"			; Memory model macros
include "fxmacs.mac"			; Fixed point macros

header	fxmap3d					; Set up memory model

begcodeseg	fxmap3d

;----------------------------------------------------------------------------
; void F386_map3Dto3D(long *m,long *result,long *p,bool special);
;----------------------------------------------------------------------------
; Maps a 3D point 'p' by the 4x4 transformation matrix 'm' to obtain the
; resultant point 'result'. In this routine we take advantage of the fact
; that the matrix may be in a special (no perspective transforms) and
; we can leave early. Since we are working with 3D points here, we will
; need to homogenise the resulting coordinate back into 3 space if the
; matrix is not special.
;----------------------------------------------------------------------------
cprocstart	F386_map3Dto3D

		ARG 	m:DPTR, result:DPTR, p:DPTR, special:BOOL
		LOCAL   temp:ULONG, temp2:ULONG, temp3:ULONG = LocalSize

		enter_c

		mov		esi,[m]			; esi -> transform matrix
		mov		edi,[p]			; edi -> point to transform

; Set up a macro to code the unrolled loop to perform the mapping. Here
; we perform the mapping of the x,y and z coordinates.

soff=0							; Source offset
doff=0							; Destination offset
		REPT	3				; Do once for each destination coordinate

		mov		eax,[esi+soff]	; EAX := mat[soff][0]
		imul	[ULONG edi]		; EDX:EAX := p.x * mat[soff][0]
		mov		ecx,eax			; Set running total
		mov		ebx,edx

		mov		eax,[esi+soff+4]; EAX := mat[soff][1]
		imul	[ULONG edi+4]	; EDX:EAX = p.y * mat[soff][1]
		add		ecx,eax			; Update running total
		adc		ebx,edx

		mov		eax,[esi+soff+8]; EAX := mat[soff][2]
		imul	[ULONG edi+8]	; EDX:EAX = p.z * mat[soff][2]
		add		eax,ecx			; Update running total
		adc		edx,ebx
		ROUNDIT					; EAX := rounded result
		add		eax,[esi+soff+12]; Add in translation (mat[soff][3]).

		mov		[temp+doff],eax	; Store in destination buffer

soff=soff+16					; Point to next source offset
doff=doff+4						; Point to next destination offset
		ENDM

		cmp		[special],0		; Do we have a special case matrix?
		je		@@SlowMapping	; No, so we need to divide through by w

		mov		edi,[result]	; edi -> resultant point
		lea		esi,[temp]		; esi -> temporary buffer
		mov		ecx,3			; Need to move 3 doublewords
	rep	movsd					; Move all three longs at once

		jmp		@@Exit			; We are done...

; Compute the new w coordinate for the vector, and divide all coordinates
; through by it.

@@SlowMapping:
		mov		eax,[esi+soff]	; EAX := mat[soff][0]
		imul	[ULONG edi]		; EDX:EAX := p.x * mat[soff][0]
		mov		ecx,eax			; Set running total
		mov		ebx,edx

		mov		eax,[esi+soff+4]; EAX := mat[soff][1]
		imul	[ULONG edi+4]	; EDX:EAX = p.y * mat[soff][1]
		add		ecx,eax			; Update running total
		adc		ebx,edx

		mov		eax,[esi+soff+8]; EAX := mat[soff][2]
		imul	[ULONG edi+8]	; EDX:EAX = p.z * mat[soff][2]
		add		eax,ecx			; Update running total
		adc		edx,ebx
		ROUNDIT					; EAX := rounded result
		add		eax,[esi+soff+12]; Add in translation (mat[soff][3]).
		mov		ecx,eax

		xor		eax,eax
		mov		edx,1
		idiv	ecx				; Find inverse of w
		mov		ecx,eax			; ECX := 1/w

		mov		edi,[result]	; ES:edi -> resultant point
		lea		esi,[temp]		; DS:esi -> temporary buffer

off = 0
		REPT	3
		mov		eax,[esi+off]	; EAX := next coordinate
		imul	ecx				; EAX := coord / w
		ROUNDIT					; EAX := rounded result
		mov		[edi+off],eax
off = off + 4
		ENDM

@@Exit:	leave_c
		ret

cprocend

;----------------------------------------------------------------------------
; void F386_map3Dto4D(long *m,long *result,long *p,bool special);
;----------------------------------------------------------------------------
; Maps a 3D point 'p' by the 4x4 transformation matrix 'm' to obtain the
; resultant point 'result'. In this routine we take advantage of the fact
; that the matrix may be in a special (no perspective transforms) and
; we can leave early. The result is a homogenous coordinate that may have
; a w value other than 1.0.
;----------------------------------------------------------------------------
cprocstart	F386_map3Dto4D

		ARG 	m:DPTR, result:DPTR, p:DPTR, special:BOOL
		LOCAL   temp:ULONG, temp2:ULONG, temp3:ULONG, temp4:ULONG = LocalSize

		enter_c

		mov		esi,[m]			; esi -> transform matrix
		mov		edi,[p]			; edi -> point to transform

; Set up a macro to code the unrolled loop to perform the mapping. Here
; we perform the mapping of the x,y and z coordinates.

soff=0							; Source offset
doff=0							; Destination offset
		REPT	3				; Do once for each destination coordinate

		mov		eax,[esi+soff]	; EAX := mat[soff][0]
		imul	[ULONG edi]		; EDX:EAX := p.x * mat[soff][0]
		mov		ecx,eax			; Set running total
		mov		ebx,edx

		mov		eax,[esi+soff+4]; EAX := mat[soff][1]
		imul	[ULONG edi+4]	; EDX:EAX = p.y * mat[soff][1]
		add		ecx,eax			; Update running total
		adc		ebx,edx

		mov		eax,[esi+soff+8]; EAX := mat[soff][2]
		imul	[ULONG edi+8]	; EDX:EAX = p.z * mat[soff][2]
		add		eax,ecx			; Update running total
		adc		edx,ebx
		ROUNDIT					; EAX := rounded result
		add		eax,[esi+soff+12]; Add in translation (mat[soff][3]).

		mov		[temp+doff],eax	; Store in destination buffer

soff=soff+16					; Point to next source offset
doff=doff+4						; Point to next destination offset
		ENDM

		cmp		[special],0		; Do we have a special case matrix?
		je		@@ComputeW		; No, so we need to compute W

; The w coordinate will be the same as the W coordinate of the input
; which is 1.0, as the input point is in 3D coordinate space.

		mov		[temp+doff],10000h
		jmp		@@Exit			; We are done...

; Compute the new w coordinate for the vector.

@@ComputeW:
		mov		eax,[esi+soff]	; EAX := mat[soff][0]
		imul	[ULONG edi]		; EDX:EAX := p.x * mat[soff][0]
		mov		ecx,eax			; Set running total
		mov		ebx,edx

		mov		eax,[esi+soff+4]; EAX := mat[soff][1]
		imul	[ULONG edi+4]	; EDX:EAX = p.y * mat[soff][1]
		add		ecx,eax			; Update running total
		adc		ebx,edx

		mov		eax,[esi+soff+8]; EAX := mat[soff][2]
		imul	[ULONG edi+8]	; EDX:EAX = p.z * mat[soff][2]
		add		eax,ecx			; Update running total
		adc		edx,ebx
		ROUNDIT					; EAX := rounded result
		add		eax,[esi+soff+12]; Add in translation (mat[soff][3]).

		mov		[temp+doff],eax	; Store w coordinate

@@Exit:	mov		edi,[result]	; ES:edi -> resultant point
		lea		esi,[temp]		; DS:esi -> temporary buffer
		mov		ecx,4			; Need to move 4 doublewords
	rep	movsd					; Move all three longs at once

		leave_c
		ret

cprocend

;----------------------------------------------------------------------------
; void F386_map4Dto4D(long *m,long *result,long *p,bool special);
;----------------------------------------------------------------------------
; Maps a 4D point 'p' by the 4x4 transformation matrix 'm' to obtain the
; resultant point 'result'. In this routine we take advantage of the fact
; that the matrix may be in a special (no perspective transforms) and
; we can leave early. The result may very well be a homogenouse coordinate.
;----------------------------------------------------------------------------
cprocstart	F386_map4Dto4D

		ARG 	m:DPTR, result:DPTR, p:DPTR, special:BOOL
		LOCAL   temp:ULONG, temp2:ULONG, temp3:ULONG, temp4:ULONG = LocalSize

		enter_c
		use_ds

		mov		esi,[m]			; esi -> transform matrix
		mov		edi,[p]			; edi -> point to transform

; Set up a macro to code the unrolled loop to perform the mapping. Here
; we perform the mapping of the x,y and z coordinates.

soff=0							; Source offset
doff=0							; Destination offset
		REPT	3				; Do once for each destination coordinate

		mov		eax,[esi+soff]	; EAX := mat[soff][0]
		imul	[ULONG edi]		; EDX:EAX := p.x * mat[soff][0]
		mov		ecx,eax			; Set running total
		mov		ebx,edx

		mov		eax,[esi+soff+4]; EAX := mat[soff][1]
		imul	[ULONG edi+4]	; EDX:EAX = p.y * mat[soff][1]
		add		ecx,eax			; Update running total
		adc		ebx,edx

		mov		eax,[esi+soff+8]; EAX := mat[soff][2]
		imul	[ULONG edi+8]	; EDX:EAX = p.z * mat[soff][2]
		add		ecx,eax			; Update running total
		adc		ebx,edx

		mov		eax,[esi+soff+12]; EAX := mat[soff][3]
		imul	[ULONG edi+12]	; EDX:EAX = p.w * mat[soff][3]
		add		eax,ecx			; Update running total
		adc		edx,ebx
		ROUNDIT					; EAX := rounded result

		mov		[temp+doff],eax	; Store in destination buffer

soff=soff+16					; Point to next source offset
doff=doff+4						; Point to next destination offset
		ENDM

		cmp		[special],0		; Do we have a special case matrix?
		je		@@ComputeW		; No, so we need to compute W

; The w coordinate will be the same as the W coordinate of the input
; point, so just copy it across

		mov		eax,[ULONG edi+12]
		mov		[temp+doff],eax
		jmp		@@Exit			; We are done...

; Compute the new w coordinate for the vector.

@@ComputeW:
		mov		eax,[esi+soff]	; EAX := mat[soff][0]
		imul	[ULONG edi]		; EDX:EAX := p.x * mat[soff][0]
		mov		ecx,eax			; Set running total
		mov		ebx,edx

		mov		eax,[esi+soff+4]; EAX := mat[soff][1]
		imul	[ULONG edi+4]	; EDX:EAX = p.y * mat[soff][1]
		add		ecx,eax			; Update running total
		adc		ebx,edx

		mov		eax,[esi+soff+8]; EAX := mat[soff][2]
		imul	[ULONG edi+8]	; EDX:EAX = p.z * mat[soff][2]
		add		ecx,eax			; Update running total
		adc		ebx,edx

		mov		eax,[esi+soff+12]; EAX := mat[soff][3]
		imul	[ULONG edi+12]	; EDX:EAX = p.w * mat[soff][3]
		add		eax,ecx			; Update running total
		adc		edx,ebx
		ROUNDIT					; EAX := rounded result

		mov		[temp+doff],eax	; Store w coordinate

@@Exit:	mov		edi,[result]	; edi -> resultant point
		lea		esi,[temp]		; esi -> temporary buffer
		mov		ecx,4			; Need to move 4 doublewords
	rep	movsd					; Move all three longs at once

		unuse_ds
		leave_c
		ret

cprocend

;----------------------------------------------------------------------------
; void F386_mapVec3Dto3D(long *m,long *result,long *v,bool special);
;----------------------------------------------------------------------------
; Maps a 3D vector 'v' by the 4x4 transformation matrix 'm' to obtain the
; resultant vector 'result'. In this routine we take advantage of the fact
; that the matrix may be in a special (no perspective transforms) and
; we can leave early. Since we are working with 3D vectors here, we will
; need to homogenise the resulting coordinate back into 3 space if the
; matrix is not special. Note that since we are applying this to a vector,
; we cannot include the translational components.
;----------------------------------------------------------------------------
cprocstart	F386_mapVec3Dto3D

		ARG 	m:DPTR, result:DPTR, p:DPTR, special:BOOL
		LOCAL   temp:ULONG, temp2:ULONG, temp3:ULONG = LocalSize

		enter_c

		mov		esi,[m]			; esi -> transform matrix
		mov		edi,[p]			; edi -> point to transform

; Set up a macro to code the unrolled loop to perform the mapping. Here
; we perform the mapping of the x,y and z coordinates.

soff=0							; Source offset
doff=0							; Destination offset
		REPT	3				; Do once for each destination coordinate

		mov		eax,[esi+soff]	; EAX := mat[soff][0]
		imul	[ULONG edi]		; EDX:EAX := p.x * mat[soff][0]
		mov		ecx,eax			; Set running total
		mov		ebx,edx

		mov		eax,[esi+soff+4]; EAX := mat[soff][1]
		imul	[ULONG edi+4]	; EDX:EAX = p.y * mat[soff][1]
		add		ecx,eax			; Update running total
		adc		ebx,edx

		mov		eax,[esi+soff+8]; EAX := mat[soff][2]
		imul	[ULONG edi+8]	; EDX:EAX = p.z * mat[soff][2]
		add		eax,ecx			; Update running total
		adc		edx,ebx
		ROUNDIT					; EAX := rounded result

		mov		[temp+doff],eax	; Store in destination buffer

soff=soff+16					; Point to next source offset
doff=doff+4						; Point to next destination offset
		ENDM

		cmp		[special],0		; Do we have a special case matrix?
		je		@@SlowMapping	; No, so we need to divide through by w

		mov		edi,[result]	; edi -> resultant point
		lea		esi,[temp]		; esi -> temporary buffer
		mov		ecx,3			; Need to move 3 doublewords
	rep	movsd					; Move all three longs at once

		jmp		@@Exit			; We are done...

; Compute the new w coordinate for the vector, and divide all coordinates
; through by it.

@@SlowMapping:
		mov		eax,[esi+soff]	; EAX := mat[soff][0]
		imul	[ULONG edi]		; EDX:EAX := p.x * mat[soff][0]
		mov		ecx,eax			; Set running total
		mov		ebx,edx

		mov		eax,[esi+soff+4]; EAX := mat[soff][1]
		imul	[ULONG edi+4]	; EDX:EAX = p.y * mat[soff][1]
		add		ecx,eax			; Update running total
		adc		ebx,edx

		mov		eax,[esi+soff+8]; EAX := mat[soff][2]
		imul	[ULONG edi+8]	; EDX:EAX = p.z * mat[soff][2]
		add		eax,ecx			; Update running total
		adc		edx,ebx
		ROUNDIT					; EAX := rounded result
		mov		ecx,eax

		xor		eax,eax
		mov		edx,1
		idiv	ecx				; Find inverse of w
		mov		ecx,eax			; ECX := 1/w

		mov		edi,[result]	; edi -> resultant point
		lea		esi,[temp]		; esi -> temporary buffer

off = 0
		REPT	3
		mov		eax,[esi+off]	; EAX := next coordinate
		imul	ecx				; EAX := coord / w
		ROUNDIT					; EAX := rounded result
		mov		[edi+off],eax
off = off + 4
		ENDM

@@Exit:	leave_c
		ret

cprocend

;----------------------------------------------------------------------------
; void F386_mapVec3Dto4D(long *m,long *result,long *v,bool special);
;----------------------------------------------------------------------------
; Maps a 3D vector 'v' by the 4x4 transformation matrix 'm' to obtain the
; resultant vector 'result'. In this routine we take advantage of the fact
; that the matrix may be in a special (no perspective transforms) and
; we can leave early. The result is a homogenous coordinate that may have
; a w value other than 1.0.
;----------------------------------------------------------------------------
cprocstart	F386_mapVec3Dto4D

		ARG 	m:DPTR, result:DPTR, p:DPTR, special:BOOL
		LOCAL   temp:ULONG, temp2:ULONG, temp3:ULONG, temp4:ULONG = LocalSize

		enter_c

		mov		esi,[m]			; esi -> transform matrix
		mov		edi,[p]			; edi -> point to transform

; Set up a macro to code the unrolled loop to perform the mapping. Here
; we perform the mapping of the x,y and z coordinates.

soff=0							; Source offset
doff=0							; Destination offset
		REPT	3				; Do once for each destination coordinate

		mov		eax,[esi+soff]	; EAX := mat[soff][0]
		imul	[ULONG edi]		; EDX:EAX := p.x * mat[soff][0]
		mov		ecx,eax			; Set running total
		mov		ebx,edx

		mov		eax,[esi+soff+4]; EAX := mat[soff][1]
		imul	[ULONG edi+4]	; EDX:EAX = p.y * mat[soff][1]
		add		ecx,eax			; Update running total
		adc		ebx,edx

		mov		eax,[esi+soff+8]; EAX := mat[soff][2]
		imul	[ULONG edi+8]	; EDX:EAX = p.z * mat[soff][2]
		add		eax,ecx			; Update running total
		adc		edx,ebx
		ROUNDIT					; EAX := rounded result

		mov		[temp+doff],eax	; Store in destination buffer

soff=soff+16					; Point to next source offset
doff=doff+4						; Point to next destination offset
		ENDM

		cmp		[special],0		; Do we have a special case matrix?
		je		@@ComputeW		; No, so we need to compute W

; The w coordinate will be the same as the W coordinate of the input
; which is 0, as the input vector is in 3D coordinate space.

		mov		[temp+doff],0h
		jmp		@@Exit			; We are done...

; Compute the new w coordinate for the vector.

@@ComputeW:
		mov		eax,[esi+soff]	; EAX := mat[soff][0]
		imul	[ULONG edi]		; EDX:EAX := p.x * mat[soff][0]
		mov		ecx,eax			; Set running total
		mov		ebx,edx

		mov		eax,[esi+soff+4]; EAX := mat[soff][1]
		imul	[ULONG edi+4]	; EDX:EAX = p.y * mat[soff][1]
		add		ecx,eax			; Update running total
		adc		ebx,edx

		mov		eax,[esi+soff+8]; EAX := mat[soff][2]
		imul	[ULONG edi+8]	; EDX:EAX = p.z * mat[soff][2]
		add		eax,ecx			; Update running total
		adc		edx,ebx
		ROUNDIT					; EAX := rounded result

		mov		[temp+doff],eax	; Store w coordinate

@@Exit:	mov		edi,[result]	; edi -> resultant point
		lea		esi,[temp]		; esi -> temporary buffer
		mov		ecx,4			; Need to move 4 doublewords
	rep	movsd					; Move all three longs at once

		leave_c
		ret

cprocend

;----------------------------------------------------------------------------
; void F386_mapVec4Dto4D(long *m,long *result,long *v,bool special);
;----------------------------------------------------------------------------
; Maps a 4D vector 'v' by the 4x4 transformation matrix 'm' to obtain the
; resultant vector 'result'. In this routine we take advantage of the fact
; that the matrix may be in a special (no perspective transforms) and
; we can leave early. The result may very well be a homogenouse coordinate.
;----------------------------------------------------------------------------
cprocstart	F386_mapVec4Dto4D

		ARG 	m:DPTR, result:DPTR, p:DPTR, special:BOOL
		LOCAL   temp:ULONG, temp2:ULONG, temp3:ULONG, temp4:ULONG = LocalSize

		enter_c

		mov		esi,[m]			; esi -> transform matrix
		mov		edi,[p]			; edi -> point to transform

; Set up a macro to code the unrolled loop to perform the mapping. Here
; we perform the mapping of the x,y and z coordinates.

soff=0							; Source offset
doff=0							; Destination offset
		REPT	3				; Do once for each destination coordinate

		mov		eax,[esi+soff]	; EAX := mat[soff][0]
		imul	[ULONG edi]		; EDX:EAX := p.x * mat[soff][0]
		mov		ecx,eax			; Set running total
		mov		ebx,edx

		mov		eax,[esi+soff+4]; EAX := mat[soff][1]
		imul	[ULONG edi+4]	; EDX:EAX = p.y * mat[soff][1]
		add		ecx,eax			; Update running total
		adc		ebx,edx

		mov		eax,[esi+soff+8]; EAX := mat[soff][2]
		imul	[ULONG edi+8]	; EDX:EAX = p.z * mat[soff][2]
		add		eax,ecx			; Update running total
		adc		edx,ebx
		ROUNDIT					; EAX := rounded result

		mov		[temp+doff],eax	; Store in destination buffer

soff=soff+16					; Point to next source offset
doff=doff+4						; Point to next destination offset
		ENDM

		cmp		[special],0		; Do we have a special case matrix?
		je		@@ComputeW		; No, so we need to compute W

; The w coordinate will be the same as the W coordinate of the input
; point, so just copy it across

		mov		eax,[ULONG edi+12]
		mov		[temp+doff],eax
		jmp		@@Exit			; We are done...

; Compute the new w coordinate for the vector.

@@ComputeW:
		mov		eax,[esi+soff]	; EAX := mat[soff][0]
		imul	[ULONG edi]		; EDX:EAX := p.x * mat[soff][0]
		mov		ecx,eax			; Set running total
		mov		ebx,edx

		mov		eax,[esi+soff+4]; EAX := mat[soff][1]
		imul	[ULONG edi+4]	; EDX:EAX = p.y * mat[soff][1]
		add		ecx,eax			; Update running total
		adc		ebx,edx

		mov		eax,[esi+soff+8]; EAX := mat[soff][2]
		imul	[ULONG edi+8]	; EDX:EAX = p.z * mat[soff][2]
		add		eax,ecx			; Update running total
		adc		edx,ebx
		ROUNDIT					; EAX := rounded result

		mov		[temp+doff],eax	; Store w coordinate

@@Exit:	mov		edi,[result]	; edi -> resultant point
		lea		esi,[temp]		; esi -> temporary buffer
		mov		ecx,4			; Need to move 4 doublewords
	rep	movsd					; Move all three longs at once

		leave_c
		ret

cprocend

endcodeseg	fxmap3d

		END						; End of module
