#############################################################################
#
#                   Copyright (C) 1999 SciTech Software
#                           All rights reserved.
#
# Descripton:   Generic makefile for zlibdows 2.0. Currently supports both
#               the MGL native target and the native Win32 target.
#
#############################################################################

# Name of library and object files required to build it

.IF $(BUILD_DLL)
DLLFILE         = zlib$D
LIBFILE         = zlib$L
.ELSE
.IF $(STKCALL)
LIBFILE         = $(LP)szlib$L
.ELSE
LIBFILE         = $(LP)zlib$L
.ENDIF
.ENDIF
LIBCLEAN        = *.lib *.a

# define library sources

OBJECTS			= adler32$O compress$O crc32$O gzio$O uncompr$O deflate$O \
				  trees$O zutil$O inflate$O infblock$O inftrees$O \
				  infcodes$O infutil$O inffast$O

# Target to build all libraries for SciTech Display Doctor

.PHONY mksdd:
	@build bc50-w32 install -u OPT_SIZE=1 CHECKED=1 DBG=1
	@build bc50-w32 install -u OPT_SIZE=1
	@dmake cleanexe

# Define the list of object files to create dependency information for

DEPEND_OBJ      = $(OBJECTS)

.INCLUDE: "$(SCITECH)/makedefs/common.mk"
