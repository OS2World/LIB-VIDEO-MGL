#############################################################################
#
#               Copyright (C) 1991-1999 SciTech Software, Inc.
#                            All rights reserved.
#
# Descripton:   Specialised definitions for building the Linux fullscreen
#				version of the MGL.
#
#############################################################################

# Find the name of the library files

LIBFILE         = $(LP)mgl$L
LIBFILECPP      = $(LP)mglcpp$L

# Target to build the library file

all: $(LIBFILE)

# The Linux version uses PC based 386 assembler code

PCASM           = 1

# List of all Linux C & assembler object files

LOCALCOBJ		= mgllinux$O opengl$O galinux$O
LOCALASMOBJ     =

# List of all Linux driver C & assembler object files

LOCALDRVCOBJ    = common$O vgapal$O gnucleus$O nucleus$O nuclvbe$O

LOCALDRVASMOBJ  =

# Define the directories to search for local source files

LOCALSRC        = mgllinux drivers/common drivers/nucleus

# Create the local include file dependencies

LDEPEND_OBJ     = $(LOCALCOBJ) $(LOCALDRVCOBJ) $(LOCALASMOBJ) $(LOCALDRVASMOBJ)
LDEPEND_SRC     = $(DEPEND_SRC);mgllinux;drivers/common;drivers/nucleus;$(SCITECH)/src/common

depend:
	@rm -f mglqnx\mgllinux.dep
	@makedep -amgldos\mgllinux.dep -r -s -S@$(mktmp $(MGLDEPEND_SRC:s/\/\\)) @$(mktmp $(MGLDEPEND_OBJ:t"\n")\n)
	@makedep -amgldos\mgllinux.dep -r -s -S@$(mktmp $(LDEPEND_SRC:s/\/\\)) @$(mktmp $(LDEPEND_OBJ:t"\n")\n)
	+@echo MGL QNX object file dependency information generated.

# Local include file dependencies

.INCLUDE .IGNORE: "mgllinux/mgllinux.dep"

