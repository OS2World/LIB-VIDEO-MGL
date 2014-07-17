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
;* Description:	OpenGL stub functions to call our dynamically linked
;*				code to either Microsoft or SGI OpenGL.
;*
;****************************************************************************

		IDEAL

include "scitech.mac"           ; Memory model macros

BEGIN_IMPORTS_DEF	_MGL_gluFuncs
DECLARE_STDCALL gluBeginCurve,4
DECLARE_STDCALL gluBeginPolygon,4
DECLARE_STDCALL gluBeginSurface,4
DECLARE_STDCALL gluBeginTrim,4
DECLARE_STDCALL gluBuild1DMipmaps,24
DECLARE_STDCALL gluBuild2DMipmaps,28
DECLARE_STDCALL gluCylinder,36
DECLARE_STDCALL gluDeleteNurbsRenderer,4
DECLARE_STDCALL gluDeleteQuadric,4
DECLARE_STDCALL gluDeleteTess,4
DECLARE_STDCALL gluDisk,28
DECLARE_STDCALL gluEndCurve,4
DECLARE_STDCALL gluEndPolygon,4
DECLARE_STDCALL gluEndSurface,4
DECLARE_STDCALL gluEndTrim,4
DECLARE_STDCALL gluErrorString,4
DECLARE_STDCALL gluGetNurbsProperty,12
DECLARE_STDCALL gluGetString,4
DECLARE_STDCALL gluGetTessProperty,12
DECLARE_STDCALL gluLoadSamplingMatrices,16
DECLARE_STDCALL gluLookAt,72
DECLARE_STDCALL gluNewNurbsRenderer,0
DECLARE_STDCALL gluNewQuadric,0
DECLARE_STDCALL gluNewTess,0
DECLARE_STDCALL gluNextContour,8
DECLARE_STDCALL gluNurbsCallback,12
DECLARE_STDCALL gluNurbsCurve,28
DECLARE_STDCALL gluNurbsProperty,12
DECLARE_STDCALL gluNurbsSurface,44
DECLARE_STDCALL gluOrtho2D,32
DECLARE_STDCALL gluPartialDisk,44
DECLARE_STDCALL gluPerspective,32
DECLARE_STDCALL gluPickMatrix,36
DECLARE_STDCALL gluProject,48
DECLARE_STDCALL gluPwlCurve,20
DECLARE_STDCALL gluQuadricCallback,12
DECLARE_STDCALL gluQuadricDrawStyle,8
DECLARE_STDCALL gluQuadricNormals,8
DECLARE_STDCALL gluQuadricOrientation,8
DECLARE_STDCALL gluQuadricTexture,8
DECLARE_STDCALL gluScaleImage,36
DECLARE_STDCALL gluSphere,20
DECLARE_STDCALL gluTessBeginContour,4
DECLARE_STDCALL gluTessBeginPolygon,8
DECLARE_STDCALL gluTessCallback,12
DECLARE_STDCALL gluTessEndContour,4
DECLARE_STDCALL gluTessEndPolygon,4
DECLARE_STDCALL gluTessNormal,28
DECLARE_STDCALL gluTessProperty,16
DECLARE_STDCALL gluTessVertex,12
DECLARE_STDCALL gluUnProject,48
END_IMPORTS_DEF

		END						; End of module
