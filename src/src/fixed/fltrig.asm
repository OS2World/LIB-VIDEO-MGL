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
;* Language:	80387 Assembler
;* Environment:	IBM PC (MS DOS)
;*
;* Description:	This module contains routines for high performance
;*				realing point trig operations by directly programming the
;*				80387 FPU (parameters passed as either floats or doubles
;*				depending on which compilation option is selected).
;*
;*                  FXsin		- floating point sine (degrees)
;*					FXcos		- floating point cosine (degrees)
;*					FXsincos	- floating point sine and cosine (degrees)
;*					FXtan		- floating point tangent (degrees)
;*
;*
;****************************************************************************

		IDEAL

include "scitech.mac"			; Memory model macros

		P387					; Turn on i387 instructions

header	fltrig					; Set up memory model

begdataseg	fltrig

		cextern	FXpi_180,REAL	; Real value for PI/180

enddataseg	fltrig

begcodeseg	fltrig

;----------------------------------------------------------------------------
; real FXsin(real angle)
;----------------------------------------------------------------------------
cprocstart	FXsin

		fld		[REAL esp+4]	; ST(0) := angle in degrees
		fmul    [REAL FXpi_180]	; ST(0) := angle in radians
		fsin					; ST(0) := sine of angle
		ret

cprocend

;----------------------------------------------------------------------------
; real FXcos(real angle)
;----------------------------------------------------------------------------
cprocstart	FXcos

		fld		[REAL esp+4]	; ST(0) := angle in degrees
		fmul    [REAL FXpi_180]	; ST(0) := angle in radians
		fcos					; ST(0) := cosine of angle
		ret

cprocend

;----------------------------------------------------------------------------
; void FXsincos(real angle,real& Sin,real& Cos)
;----------------------------------------------------------------------------
; Computes both the sine and cosine of an angle as fast as possible.
;----------------------------------------------------------------------------
cprocstart	FXsincos

		ARG		angle:REAL, sin:DPTR, cos:DPTR

		push	ebp
		mov		ebp,esp
		push	ebx

		fld		[REAL angle]	; ST(0) := angle in degrees
		fmul    [REAL FXpi_180]	; ST(0) := angle in radians
		fsincos					; ST(1) := sine of angle
								; ST(0) := cosine of angle
		wait					; Avoid bugs in fsincos instruction ;-(
		mov		ebx,[cos]		; ES:_BX -> place for cosine
		fstp	[REAL ebx]		; Store cosine value
		mov		ebx,[sin]		; ES:_BX -> place for sine
		fstp	[REAL ebx]		; Store sine value

		pop		ebx
		pop		ebp
		ret

cprocend

;----------------------------------------------------------------------------
; real FXtan(real angle)
;----------------------------------------------------------------------------
cprocstart	FXtan

		fld		[REAL esp+4]	; ST(0) := angle in degrees
		fmul    [REAL FXpi_180]	; ST(0) := angle in radians
		fptan					; ST(0) := 1.0
								; ST(1) := tangent
		wait					; Avoid bugs with FPTAN
		nop
		fstp	st0				; Clean up FPU stack
		ret

cprocend

endcodeseg	fltrig

		END						; End of module
