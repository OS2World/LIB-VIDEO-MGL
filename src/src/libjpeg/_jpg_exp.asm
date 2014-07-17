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
;* Description: Module to implement the export stubs for all the JPEG
;*				image library.
;*
;****************************************************************************

		IDEAL

include "scitech.mac"           ; Memory model macros

BEGIN_IMPORTS_DEF	_JPEG_exports
DECLARE_IMP __jpeg_std_error	; Handled in C code!
DECLARE_IMP jpeg_CreateCompress
DECLARE_IMP jpeg_CreateDecompress
DECLARE_IMP jpeg_destroy_compress
DECLARE_IMP jpeg_destroy_decompress
DECLARE_IMP jpeg_stdio_dest
DECLARE_IMP jpeg_stdio_src
DECLARE_IMP jpeg_set_defaults
DECLARE_IMP jpeg_set_colorspace
DECLARE_IMP jpeg_default_colorspace
DECLARE_IMP jpeg_set_quality
DECLARE_IMP jpeg_set_linear_quality
DECLARE_IMP jpeg_add_quant_table
DECLARE_IMP jpeg_quality_scaling
DECLARE_IMP jpeg_simple_progression
DECLARE_IMP jpeg_suppress_tables
DECLARE_IMP jpeg_alloc_quant_table
DECLARE_IMP jpeg_alloc_huff_table
DECLARE_IMP jpeg_start_compress
DECLARE_IMP jpeg_write_scanlines
DECLARE_IMP jpeg_finish_compress
DECLARE_IMP jpeg_write_raw_data
DECLARE_IMP jpeg_write_marker
DECLARE_IMP jpeg_write_tables
DECLARE_IMP jpeg_read_header
DECLARE_IMP jpeg_start_decompress
DECLARE_IMP jpeg_read_scanlines
DECLARE_IMP jpeg_finish_decompress
DECLARE_IMP jpeg_read_raw_data
DECLARE_IMP jpeg_has_multiple_scans
DECLARE_IMP jpeg_start_output
DECLARE_IMP jpeg_finish_output
DECLARE_IMP jpeg_input_complete
DECLARE_IMP jpeg_new_colormap
DECLARE_IMP jpeg_consume_input
DECLARE_IMP jpeg_calc_output_dimensions
DECLARE_IMP jpeg_set_marker_processor
DECLARE_IMP jpeg_read_coefficients
DECLARE_IMP jpeg_write_coefficients
DECLARE_IMP jpeg_copy_critical_parameters
DECLARE_IMP jpeg_abort_compress
DECLARE_IMP jpeg_abort_decompress
DECLARE_IMP jpeg_abort
DECLARE_IMP jpeg_destroy
DECLARE_IMP jpeg_resync_to_restart
END_IMPORTS_DEF

		END
