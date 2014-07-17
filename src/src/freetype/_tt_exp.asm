;****************************************************************************
;*
;*           		SciTech Nucleus Driver Architecture
;*
;*               Copyright (C) 1997-1998 SciTech Software, Inc.
;*                            All rights reserved.
;*
;*  ======================================================================
;*  |REMOVAL OR MODIFICATION OF THIS HEADER IS STRICTLY PROHIBITED BY LAW|
;*  |                                                                    |
;*  |This copyrighted computer code is a proprietary trade secret of     |
;*  |SciTech Software, Inc., located at 505 Wall Street, Chico, CA 95928 |
;*  |USA (www.scitechsoft.com).  ANY UNAUTHORIZED POSSESSION, USE,       |
;*  |VIEWING, COPYING, MODIFICATION OR DISSEMINATION OF THIS CODE IS     |
;*  |STRICTLY PROHIBITED BY LAW.  Unless you have current, express       |
;*  |written authorization from SciTech to possess or use this code, you |
;*  |may be subject to civil and/or criminal penalties.                  |
;*  |                                                                    |
;*  |If you received this code in error or you would like to report      |
;*  |improper use, please immediately contact SciTech Software, Inc. at  |
;*  |530-894-8400.                                                       |
;*  |                                                                    |
;*  |REMOVAL OR MODIFICATION OF THIS HEADER IS STRICTLY PROHIBITED BY LAW|
;*  ======================================================================
;*
;* Language:    TASM 4.0 or NASM
;* Environment: IBM PC 32 bit Protected Mode.
;*
;* Description: Module to implement the export stubs for all the X86
;*				emulator library.
;*
;****************************************************************************

		IDEAL

include "scitech.mac"           ; Memory model macros

BEGIN_IMPORTS_DEF	_TT_exports
DECLARE_IMP __TT_Init_FreeType	; Handled in C code
DECLARE_IMP __TT_Done_FreeType	; Handled in C code
DECLARE_IMP TT_Set_Raster_Gray_Palette
DECLARE_IMP TT_Open_Face
DECLARE_IMP TT_Open_Collection
DECLARE_IMP TT_Get_Face_Properties
DECLARE_IMP TT_Set_Face_Pointer
DECLARE_IMP TT_Get_Face_Pointer
DECLARE_IMP TT_Flush_Face
DECLARE_IMP TT_Get_Face_Metrics
DECLARE_IMP TT_Close_Face
DECLARE_IMP TT_Get_Font_Data
DECLARE_IMP TT_New_Instance
DECLARE_IMP TT_Set_Instance_Resolutions
DECLARE_IMP TT_Set_Instance_CharSize
DECLARE_IMP TT_Set_Instance_CharSizes
DECLARE_IMP TT_Set_Instance_PixelSizes
DECLARE_IMP TT_Set_Instance_Transform_Flags
DECLARE_IMP TT_Get_Instance_Metrics
DECLARE_IMP TT_Set_Instance_Pointer
DECLARE_IMP TT_Get_Instance_Pointer
DECLARE_IMP TT_Done_Instance
DECLARE_IMP TT_New_Glyph
DECLARE_IMP TT_Done_Glyph
DECLARE_IMP TT_Load_Glyph
DECLARE_IMP TT_Get_Glyph_Outline
DECLARE_IMP TT_Get_Glyph_Metrics
DECLARE_IMP TT_Get_Glyph_Big_Metrics
DECLARE_IMP TT_Get_Glyph_Bitmap
DECLARE_IMP TT_Get_Glyph_Pixmap
DECLARE_IMP TT_New_Outline
DECLARE_IMP TT_Done_Outline
DECLARE_IMP TT_Copy_Outline
DECLARE_IMP TT_Get_Outline_Bitmap
DECLARE_IMP TT_Get_Outline_Pixmap
DECLARE_IMP TT_Get_Outline_BBox
DECLARE_IMP TT_Transform_Outline
DECLARE_IMP TT_Translate_Outline
DECLARE_IMP TT_Transform_Vector
DECLARE_IMP TT_MulDiv
DECLARE_IMP TT_MulFix
DECLARE_IMP TT_Get_CharMap_Count
DECLARE_IMP TT_Get_CharMap_ID
DECLARE_IMP TT_Get_CharMap
DECLARE_IMP TT_Char_Index
DECLARE_IMP TT_Get_Name_Count
DECLARE_IMP TT_Get_Name_ID
DECLARE_IMP TT_Get_Name_String
END_IMPORTS_DEF

		END
