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
;* Description: Module to implement the export stubs for all the PNG
;*				image library.
;*
;****************************************************************************

		IDEAL

include "scitech.mac"           ; Memory model macros

BEGIN_IMPORTS_DEF	_PNG_exports
DECLARE_IMP png_set_sig_bytes
DECLARE_IMP __png_sig_cmp				; Handled in C code!
DECLARE_IMP png_check_sig
DECLARE_IMP __png_create_read_struct	; Handled in C code!
DECLARE_IMP __png_create_write_struct	; Handled in C code!
DECLARE_IMP png_write_chunk
DECLARE_IMP png_write_chunk_start
DECLARE_IMP png_write_chunk_data
DECLARE_IMP png_write_chunk_end
DECLARE_IMP png_create_info_struct
DECLARE_IMP png_write_info
DECLARE_IMP png_read_info
DECLARE_IMP png_convert_from_struct_tm
DECLARE_IMP png_convert_from_time_t
DECLARE_IMP png_set_expand
DECLARE_IMP png_set_bgr
DECLARE_IMP png_set_gray_to_rgb
DECLARE_IMP png_build_grayscale_palette
DECLARE_IMP png_set_strip_alpha
DECLARE_IMP png_set_swap_alpha
DECLARE_IMP png_set_invert_alpha
DECLARE_IMP png_set_filler
DECLARE_IMP png_set_swap
DECLARE_IMP png_set_packing
DECLARE_IMP png_set_packswap
DECLARE_IMP png_set_shift
DECLARE_IMP png_set_interlace_handling
DECLARE_IMP png_set_invert_mono
DECLARE_IMP png_set_background
DECLARE_IMP png_set_strip_16
DECLARE_IMP png_set_dither
DECLARE_IMP png_set_gamma
DECLARE_IMP png_set_flush
DECLARE_IMP png_write_flush
DECLARE_IMP png_start_read_image
DECLARE_IMP png_read_update_info
DECLARE_IMP png_read_rows
DECLARE_IMP png_read_row
DECLARE_IMP png_read_image
DECLARE_IMP png_write_row
DECLARE_IMP png_write_rows
DECLARE_IMP png_write_image
DECLARE_IMP png_write_end
DECLARE_IMP png_read_end
DECLARE_IMP png_destroy_info_struct
DECLARE_IMP png_destroy_read_struct
DECLARE_IMP png_destroy_write_struct
DECLARE_IMP png_set_crc_action
DECLARE_IMP png_set_filter
DECLARE_IMP png_set_filter_heuristics
DECLARE_IMP png_set_compression_level
DECLARE_IMP png_set_compression_mem_level
DECLARE_IMP png_set_compression_strategy
DECLARE_IMP png_set_compression_window_bits
DECLARE_IMP png_set_compression_method
DECLARE_IMP png_init_io
DECLARE_IMP png_set_error_fn
DECLARE_IMP png_get_error_ptr
DECLARE_IMP png_set_write_fn
DECLARE_IMP png_set_read_fn
DECLARE_IMP png_get_io_ptr
DECLARE_IMP png_set_read_status_fn
DECLARE_IMP png_set_write_status_fn
DECLARE_IMP png_set_read_user_transform_fn
DECLARE_IMP png_set_write_user_transform_fn
DECLARE_IMP png_set_progressive_read_fn
DECLARE_IMP png_get_progressive_ptr
DECLARE_IMP png_process_data
DECLARE_IMP png_progressive_combine_row
DECLARE_IMP png_memcpy_check
DECLARE_IMP png_memset_check
DECLARE_IMP png_error
DECLARE_IMP png_chunk_error
DECLARE_IMP png_warning
DECLARE_IMP png_chunk_warning
DECLARE_IMP png_get_valid
DECLARE_IMP png_get_rowbytes
DECLARE_IMP png_get_channels
DECLARE_IMP png_get_image_width
DECLARE_IMP png_get_image_height
DECLARE_IMP png_get_bit_depth
DECLARE_IMP png_get_color_type
DECLARE_IMP png_get_filter_type
DECLARE_IMP png_get_interlace_type
DECLARE_IMP png_get_compression_type
DECLARE_IMP png_get_pixels_per_meter
DECLARE_IMP png_get_x_pixels_per_meter
DECLARE_IMP png_get_y_pixels_per_meter
DECLARE_IMP png_get_pixel_aspect_ratio
DECLARE_IMP png_get_x_offset_pixels
DECLARE_IMP png_get_y_offset_pixels
DECLARE_IMP png_get_x_offset_microns
DECLARE_IMP png_get_y_offset_microns
DECLARE_IMP png_get_signature
DECLARE_IMP png_get_bKGD
DECLARE_IMP png_set_bKGD
DECLARE_IMP png_get_cHRM
DECLARE_IMP png_set_cHRM
DECLARE_IMP png_get_gAMA
DECLARE_IMP png_set_gAMA
DECLARE_IMP png_get_hIST
DECLARE_IMP png_set_hIST
DECLARE_IMP png_get_IHDR
DECLARE_IMP png_set_IHDR
DECLARE_IMP png_get_oFFs
DECLARE_IMP png_set_oFFs
DECLARE_IMP png_get_pCAL
DECLARE_IMP png_set_pCAL
DECLARE_IMP png_get_pHYs
DECLARE_IMP png_set_pHYs
DECLARE_IMP png_get_PLTE
DECLARE_IMP png_set_PLTE
DECLARE_IMP png_get_sBIT
DECLARE_IMP png_set_sBIT
DECLARE_IMP png_get_sRGB
DECLARE_IMP png_set_sRGB
DECLARE_IMP png_set_sRGB_gAMA_and_cHRM
DECLARE_IMP png_get_text
DECLARE_IMP png_set_text
DECLARE_IMP png_get_tIME
DECLARE_IMP png_set_tIME
DECLARE_IMP png_get_tRNS
DECLARE_IMP png_set_tRNS
END_IMPORTS_DEF

		END
