#############################################################################
#
#               Copyright (C) 1991-1997 SciTech Software, Inc.
#                            All rights reserved.
#
# Descripton:   Specialised definitions for building the OS/2 PM
#				target version of the MGL.
#
#############################################################################

# Find the name of the library files

.IF $(STKCALL)
LIBFILE         = $(LP)smglpm$L
LIBFILECPP      = $(LP)smglcpp$L
.ELSE
LIBFILE         = $(LP)mglpm$L
LIBFILECPP      = $(LP)mglcpp$L
.ENDIF

# Target to build the library file

all: $(LIBFILE)

# The MGLOS2 version uses PC based 386 assembler code

PCASM           = 1

# List of all MGLOS2 C & assembler object files

LOCALCOBJ       = mglos2$O maskcode$O event$O mouse$O opengl$O
LOCALASMOBJ     = _timer$O

# List of all MGLOS2 driver C & assembler object files

LOCALDRVCOBJ    = common$O vgapal$O vga$O ports$O vga4$O vga8$O vgax$O      \
				  svga$O svga4$O svga8$O svga16$O svga24$O svga32$O			\
				  linear8$O linear16$O linear24$O linear32$O				\
				  vesavbe$O gaccel$O gtfcalc$O     							\
				  accel8$O accel16$O accel24$O accel32$O galib$O gaos2$O 	\
				  peloader$O libcimp$O ulztimer$O cpuinfo$O

LOCALDRVASMOBJ  = _vga$O _ports$O _vga4$O _vgax$O                           \
				  _svga$O _svga8$O _svga16$O _svga24$O _svga32$O            \
				  _gastubs$O _gatimer$O _ga_imp$O _cpuinfo$O

# Define the directories to search for local source files

LOCALSRC        = mglos2 drivers\common drivers\vga drivers\svga drivers\accel

# Create the local include file dependencies

LDEPEND_OBJ     = $(LOCALCOBJ) $(LOCALDRVCOBJ) $(LOCALASMOBJ) $(LOCALDRVASMOBJ)
LDEPEND_SRC     = $(DEPEND_SRC);mglos2;drivers\common;drivers\vga;drivers\svga;drivers\accel

depend:
	@rm -f mglos2\mglos2.dep
	@makedep -amglos2\mglos2.dep -r -s -S$(MGLDEPEND_SRC:s/\/\\) @$(mktmp $(MGLDEPEND_OBJ:t"\n")\n)
	@makedep -amglos2\mglos2.dep -r -s -S@$(mktmp $(LDEPEND_SRC:s/\/\\)) @$(mktmp $(LDEPEND_OBJ:t"\n")\n)
	+@echo MGL OS/2 Object file dependency information generated.

# Local include file dependencies

.INCLUDE .IGNORE: "mglos2\mglos2.dep"
