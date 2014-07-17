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
;*				floating point square root using the 80387 co-processor.
;*
;*                  FXsqrt	- floating point square root
;*
;*
;****************************************************************************

		IDEAL

include "scitech.mac"			; Memory model macros

		P387

header	flsqrt					; Set up memory model

begcodeseg	flsqrt

;----------------------------------------------------------------------------
; real FXsqrt(real f);
;----------------------------------------------------------------------------
cprocstart	FXsqrt

		fld		[REAL esp+4]	; ST(0) := f
		fsqrt					; ST(0) := sqrt(f)
		ret

cprocend

endcodeseg	flsqrt

		END						; End of module
