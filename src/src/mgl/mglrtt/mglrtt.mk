#############################################################################
#
#               Copyright (C) 1991-1997 SciTech Software, Inc.
#                            All rights reserved.
#
# Descripton:   Specialised definitions for building the MGLRTT target
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

# The MGLRTT version uses PC based 386 assembler code

PCASM           = 1

# List of all MGLRTT C & assembler object files

LOCALCOBJ       = mglrtt$O opengl$O
LOCALASMOBJ     =

# List of all MGLRTT driver C & assembler object files

LOCALDRVCOBJ    = common$O vgapal$O vga$O ports$O vga4$O vga8$O vgax$O      \
				  svga$O svga4$O svga8$O svga16$O svga24$O svga32$O			\
				  linear8$O linear16$O linear24$O linear32$O				\
				  vesavbe$O gaccel$O gtfcalc$O     							\
				  accel8$O accel16$O accel24$O accel32$O galib$O gados$O 	\
				  peloader$O libcimp$O

LOCALDRVASMOBJ  = _vga$O _ports$O _vga4$O _vgax$O                           \
				  _svga$O _svga8$O _svga16$O _svga24$O _svga32$O            \
				  _gastubs$O _gados$O _gatimer$O _ga_imp$O

# Define the directories to search for local source files

LOCALSRC        = mglrtt drivers\common drivers\vga drivers\svga drivers\accel

# Special target to build assembler files via pre-TASM'ed code if using NASM

.IF $(USE_NASM)

LOCAL_ASM_TARGET = 1

.IF $(USE_GCC)
%$O: %$A ; +obj2bfd -u -O coff-go32 -o $@ tasmobj/$*.obj
.ELSE
%$O: %$A ; +cp tasmobj/$*.obj $@
.ENDIF

.ENDIF

# Create the local include file dependencies

LDEPEND_OBJ     = $(LOCALCOBJ) $(LOCALDRVCOBJ) $(LOCALASMOBJ) $(LOCALDRVASMOBJ)
LDEPEND_SRC     = $(DEPEND_SRC);mglrtt;drivers\common;drivers\vga;drivers\svga;drivers\accel

depend:
    @rm -f mglrtt\mglrtt.dep
    @makedep -amglrtt\mglrtt.dep -r -s -S$(MGLDEPEND_SRC:s/\/\\) @$(mktmp $(MGLDEPEND_OBJ:t"\n")\n)
    @makedep -amglrtt\mglrtt.dep -r -s -S@$(mktmp $(LDEPEND_SRC:s/\/\\)) @$(mktmp $(LDEPEND_OBJ:t"\n")\n)
    +@echo MGLRTT Object file dependency information generated.

# Local include file dependencies

.INCLUDE .IGNORE: "mglrtt\mglrtt.dep"
