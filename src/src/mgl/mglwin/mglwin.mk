#############################################################################
#
#               Copyright (C) 1991-1997 SciTech Software, Inc.
#                            All rights reserved.
#
# Descripton:   Specialised definitions for building the MGLWIN target
#               version of the MGL.
#
#############################################################################

# Define the name of the library file

.IF $(BUILD_DLL)
MGL_DLL         = 1
.ELIF $(IMPORT_DLL)
MGL_DLL         = 1
.ENDIF

# Build Standard Windows DLL (C calling conventions)

.IF $(MGL_DLL)
CFLAGS          += -DMGL_DLL    # Build the standard DLL
LIBFILE         = mgli$L
DLLFILE         = mgl$D

# Build standard library

.ELSE
.IF $(STKCALL)
LIBFILE         = $(LP)smgl$L
LIBFILECPP      = $(LP)smglcpp$L
.ELSE
LIBFILE         = $(LP)mgl$L
LIBFILECPP      = $(LP)mglcpp$L
.ENDIF
.ENDIF

# Target to build the library file

all: $(LIBFILE)

# The MGLWIN version uses PC based 386 assembler code

PCASM           = 1

# List of all MGLWIN C & assembler object files

LOCALCOBJ       = mglwin$O opengl$O public$O
LOCALASMOBJ     =

# List of all MGLWIN driver C & assembler object files

LOCALDRVCOBJ    = common$O vgapal$O gnucleus$O nucleus$O nuclvbe$O gawin32$O
LOCALDRVASMOBJ  =

# Define the directories to search for local source files

LOCALSRC        = mglwin drivers\common drivers\nucleus drivers\windows

# Create the local include file dependencies

LDEPEND_OBJ     = $(LOCALCOBJ) $(LOCALDRVCOBJ) $(LOCALASMOBJ) $(LOCALDRVASMOBJ)
LDEPEND_SRC     = $(DEPEND_SRC);mglwin;drivers\common;drivers\nucleus;drivers\windows;$(SCITECH)\src\common

depend:
	@rm -f mglwin\mglwin.dep
	@makedep -amglwin\mglwin.dep -r -s -S@$(mktmp $(MGLDEPEND_SRC:s/\/\\)) @$(mktmp $(MGLDEPEND_OBJ:t"\n")\n)
	@makedep -amglwin\mglwin.dep -r -s -S@$(mktmp $(LDEPEND_SRC:s/\/\\)) @$(mktmp $(LDEPEND_OBJ:t"\n")\n)
	+@echo MGLWIN object file dependency information generated.

# Local include file dependencies

.INCLUDE .IGNORE: "mglwin\mglwin.dep"
