#############################################################################
#
#               Copyright (C) 1991-1999 SciTech Software, Inc.
#                            All rights reserved.
#
# Descripton:   Specialised definitions for building the QNX fullscreen
#				and Photon versions of the MGL.
#
#############################################################################

# Find the name of the library files

LIBFILE         = $(LP)mgl$L
LIBFILECPP      = $(LP)mglcpp$L

# Target to build the library file

all: $(LIBFILE)

# The QNX 4 version uses PC based 386 assembler code

PCASM           = 1

# List of all QNX C & assembler object files

LOCALCOBJ		= mglqnx$O opengl$O gaqnx$O
LOCALASMOBJ     =

# List of all MGLDOS driver C & assembler object files

LOCALDRVCOBJ    = common$O vgapal$O gnucleus$O nucleus$O nuclvbe$O

LOCALDRVASMOBJ  =

# Define the directories to search for local source files

LOCALSRC        = mglqnx drivers/common drivers/nucleus

# Create the local include file dependencies

LDEPEND_OBJ     = $(LOCALCOBJ) $(LOCALDRVCOBJ) $(LOCALASMOBJ) $(LOCALDRVASMOBJ)
LDEPEND_SRC     = $(DEPEND_SRC);mglqnx;drivers/common;drivers/nucleus;$(SCITECH)/src/common

depend:
	@rm -f mglqnx\mglqnx.dep
	@makedep -amgldos\mglqnx.dep -r -s -S@$(mktmp $(MGLDEPEND_SRC:s/\/\\)) @$(mktmp $(MGLDEPEND_OBJ:t"\n")\n)
	@makedep -amgldos\mglqnx.dep -r -s -S@$(mktmp $(LDEPEND_SRC:s/\/\\)) @$(mktmp $(LDEPEND_OBJ:t"\n")\n)
	+@echo MGL QNX object file dependency information generated.

# Local include file dependencies

.INCLUDE .IGNORE: "mglqnx/mglqnx.dep"
