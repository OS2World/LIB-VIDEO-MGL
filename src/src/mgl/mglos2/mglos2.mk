#############################################################################
#
#               Copyright (C) 1991-1997 SciTech Software, Inc.
#                            All rights reserved.
#
# Descripton:   Specialised definitions for building the OS/2 fullscreen
#				target version of the MGL.
#
#############################################################################

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

# The MGLOS2 version uses PC based 386 assembler code

PCASM           = 1

# List of all MGLOS2 C & assembler object files

LOCALCOBJ       = mglos2$O opengl$O gaos2$O
LOCALASMOBJ     =

# List of all MGLOS2 driver C & assembler object files

LOCALDRVCOBJ    = common$O vgapal$O gnucleus$O nucleus$O nuclvbe$O

LOCALDRVASMOBJ  =

# Define the directories to search for local source files

LOCALSRC        = mglos2 drivers\common drivers\nucleus

# Create the local include file dependencies

LDEPEND_OBJ     = $(LOCALCOBJ) $(LOCALDRVCOBJ) $(LOCALASMOBJ) $(LOCALDRVASMOBJ)
LDEPEND_SRC     = $(DEPEND_SRC);mglos2;drivers\common;drivers\nucleus;$(SCITECH)\src\common

depend:
	@rm -f mglos2\mglos2.dep
	@makedep -amglos2\mglos2.dep -r -s -S@$(mktmp $(MGLDEPEND_SRC:s/\/\\)) @$(mktmp $(MGLDEPEND_OBJ:t"\n")\n)
	@makedep -amglos2\mglos2.dep -r -s -S@$(mktmp $(LDEPEND_SRC:s/\/\\)) @$(mktmp $(LDEPEND_OBJ:t"\n")\n)
	+@echo MGL OS2 object file dependency information generated.

# Local include file dependencies

.INCLUDE .IGNORE: "mglos2\mglos2.dep"
