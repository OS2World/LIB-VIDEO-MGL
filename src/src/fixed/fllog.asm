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
;* Language:	80387 Assembler
;* Environment:	IBM PC (MS DOS)
;*
;* Description:	This module contains routines for high performance
;*				floating point logarithms using the 80387.
;*
;*					FLlog10	- floating point base 10 logarithm
;*                  FLlog	- floating point natural logarithm
;*
;****************************************************************************

		IDEAL

include "scitech.mac"			; Memory model macros

header	fllog					; Set up memory model

begcodeseg	fllog

;----------------------------------------------------------------------------
; real FXlog10(real f);
;----------------------------------------------------------------------------
cprocstart	FXlog10

		fldlg2					; ST(0) := log10(2)
		fld		[REAL esp+4]	; ST(0) := f
		fyl2x					; ST(0) := log10(f)
		ret

cprocend

;----------------------------------------------------------------------------
; real FXlog(real f);
;----------------------------------------------------------------------------
cprocstart	FXlog

		fldln2					; ST(0) := loge(2)
		fld		[REAL esp+4]	; ST(0) := f
		fyl2x					; ST(0) := loge(f)
		ret

cprocend

endcodeseg	fllog

		END						; End of module
