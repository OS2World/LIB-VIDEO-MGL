#############################################################################
#
#               Copyright (C) 1991-1997 SciTech Software, Inc.
#                            All rights reserved.
#
# Descripton:   Specialised definitions for building the MGLsmx target
#               version of the MGL.
#
#############################################################################

# Indicate that we are compiling for real DOS

ASFLAGS         += -d__REALDOS__

# Find the name of the library files

.IF $(STKCALL)
LIBFILE         = $(LP)smgl$L
LIBFILECPP      = $(LP)smglcpp$L
.ELSE
LIBFILE         = $(LP)mgl$L
LIBFILECPP      = $(LP)mglcpp$L
.ENDIF

# Target to build the library file

all: $(LIBFILE)

# The MGLSMX version uses PC based 386 assembler code

PCASM           = 1

# List of all MGLSMX C & assembler object files

LOCALCOBJ       = mglsmx$O opengl$O
LOCALASMOBJ     =

# List of all MGLSMX driver C & assembler object files

LOCALDRVCOBJ    = common$O vgapal$O gnucleus$O nucleus$O nuclvbe$O gasmx$O
LOCALDRVASMOBJ  =

# Define the directories to search for local source files

LOCALSRC        = mglsmx drivers\common drivers\nucleus

# Create the local include file dependencies

LDEPEND_OBJ     = $(LOCALCOBJ) $(LOCALDRVCOBJ) $(LOCALASMOBJ) $(LOCALDRVASMOBJ)
LDEPEND_SRC     = $(DEPEND_SRC);mglsmx;drivers\common;drivers\nucleus;$(SCITECH)\src\common

depend:
	@rm -f mglsmx\mglsmx.dep
	@makedep -amglsmx\mglsmx.dep -r -s -S@$(mktmp $(MGLDEPEND_SRC:s/\/\\)) @$(mktmp $(MGLDEPEND_OBJ:t"\n")\n)
	@makedep -amglsmx\mglsmx.dep -r -s -S@$(mktmp $(LDEPEND_SRC:s/\/\\)) @$(mktmp $(LDEPEND_OBJ:t"\n")\n)
	+@echo MGLSMX object file dependency information generated.

# Local include file dependencies

.INCLUDE .IGNORE: "mglsmx\mglsmx.dep"
