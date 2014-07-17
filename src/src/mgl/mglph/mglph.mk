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

.IF $(USE_PHOTON)
LIBFILE         = $(LP)mglph$L
.ELIF $(USE_X11)
LIBFILE         = $(LP)mglx11$L
.ELSE
LIBFILE         = $(LP)mgl$L
.ENDIF
LIBFILECPP      = $(LP)mglcpp$L

# Target to build the library file

all: $(LIBFILE)

# The QNX 4 version uses PC based 386 assembler code

PCASM           = 1

# List of all QNX C & assembler object files

LOCALCOBJ		= mglqnx$O maskcode$O event$O mouse$O opengl$O
LOCALASMOBJ     =

# List of all QNX driver C & assembler object files

LOCALDRVCOBJ    = common$O vgapal$O vga$O ports$O vga4$O vga8$O vgax$O      \
				  svga$O svga4$O svga8$O svga16$O svga24$O svga32$O			\
				  linear8$O linear16$O linear24$O linear32$O				\
				  vesavbe$O gaccel$O gtfcalc$O     							\
				  accel8$O accel16$O accel24$O accel32$O galib$O gaqnx$O 	\
				  peloader$O libcimp$O ulztimer$O cpuinfo$O

LOCALDRVASMOBJ  = _vga$O _ports$O _vga4$O _vgax$O                           \
				  _svga$O _svga8$O _svga16$O _svga24$O _svga32$O            \
				  _gastubs$O _gatimer$O _ga_imp$O _cpuinfo$O

# Define the directories to search for local source files

LOCALSRC        = mglqnx drivers/common drivers/vga drivers/svga drivers/accel

# Special target to build assembler files via pre-TASM'ed code from DOS

.IF (USE_QNXNTO)
%$O: %$A ; +cp elfobj/$*.o $@
.ELSE
%$O: %$A ; +cp tasmobj/$*.obj $@
.ENDIF

# Create the local include file dependencies

LDEPEND_OBJ     = $(LOCALCOBJ) $(LOCALDRVCOBJ) $(LOCALASMOBJ) $(LOCALDRVASMOBJ)
LDEPEND_SRC     = $(DEPEND_SRC);mglqnx;drivers/common;drivers/vga;drivers/svga;drivers/accel

depend:
	@rm -f mglqnx/mglqnx.dep
	@makedep -amglqnx/mglqnx.dep -r -s -S$(MGLDEPEND_SRC:s/\/\\) @$(mktmp $(MGLDEPEND_OBJ:t"\n")\n)
	@makedep -amglqnx/mglqnx.dep -r -s -S@$(mktmp $(LDEPEND_SRC:s/\/\\)) @$(mktmp $(LDEPEND_OBJ:t"\n")\n)
	+@echo MGL QNX Object file dependency information generated.

# Local include file dependencies

.INCLUDE .IGNORE: "mglqnx/mglqnx.dep"
