#############################################################################
#
#                   Copyright (C) 1999 SciTech Software
#                           All rights reserved.
#
# Descripton:   Generic makefile for libpng library.
#
#############################################################################

#----------------------------------------------------------------------------
# Define the list of object files
#----------------------------------------------------------------------------

# Objects for building Binary Portable DLL
DLL_OBJS		= dllstart$O pnginit$O

# PNG library sources
PNG_OBJS 		= pngutil$O pngread$O pngrtran$O pngrutil$O pngpread$O \
				  pngtrans$O pngwrite$O pngwtran$O pngwutil$O pngerror$O \
				  pngmem$O pngwio$O pngrio$O pngget$O pngset$O

# ZLIB library sources
ZLIB_OBJS		= adler32$O compress$O crc32$O uncompr$O deflate$O \
				  trees$O zutil$O inflate$O infblock$O inftrees$O \
				  infcodes$O infutil$O inffast$O

# These objectfiles are included in png.lib
OBJECTS			= $(PNG_OBJS) $(ZLIB_OBJS)

CFLAGS			+= -I../zlib
.IF $(BUILD_DLL)
CFLAGS      	+= -I$(PRIVATE)\include\drvlib -I$(SCITECH)\include\drvlib -D__DRIVER__
ASFLAGS   		+= -d__DRIVER__
EXELIBS			= drvlib$L
.ELSE
LIBFILE         = $(LP)png$L
LIBCLEAN  		= *.dll *.lib *.a
.ENDIF

#----------------------------------------------------------------------------
# Target to build the Binary Portable DLL target
#----------------------------------------------------------------------------

png.dll: $(DLL_OBJS) $(OBJECTS)

#----------------------------------------------------------------------------
# Target to build all Intel binary drivers
#----------------------------------------------------------------------------

.PHONY mksdd:
	@build wc11-w32 png.dll -u BUILD_DLL=1 NO_RUNTIME=1 OPT=1
	@k_cp png.dll $(SCITECH)\drivers\png.bpd
	@dmake cleanexe

.PHONY db:
	@build wc11-w32 png.dll BUILD_DLL=1 NO_RUNTIME=1 OPT=1
	@k_cp png.dll $(SCITECH)\drivers\png.bpd

#----------------------------------------------------------------------------
# Define the list of object files to create dependency information for
#----------------------------------------------------------------------------

DEPEND_OBJ      = $(OBJECTS)
DEPEND_SRC   	:= ../zlib
.SOURCE:           ../zlib

.INCLUDE: "$(SCITECH)/makedefs/common.mk"
