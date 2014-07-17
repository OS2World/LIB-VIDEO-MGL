#############################################################################
#
#                   Copyright (C) 1995 SciTech Software
#                           All rights reserved.
#
# Descripton:   Generic makefile for the console library. Requires the
#               SciTech Software makefile definitions package to be
#               installed.
#
#############################################################################

# DOS extender dependant flags

CFLAGS          += $(DX_CFLAGS)
ASFLAGS         += $(DX_ASFLAGS)

# Name of library and object files required to build it

.IF $(CURSES)
OBJECTS         = concurses$O
EXELIBS			+= -lncurses
LIBFILE         = $(LP)concurses$L
.ELSE
OBJECTS         = console$O
LIBFILE         = $(LP)console$L
.IF $(USE_QNX4)
LDFLAGS			+= -T1
.ENDIF
.ENDIF

# Define the list of object files to create dependency information for

DEPEND_OBJ      = $(OBJECTS) test$O

test$E: test$O $(LIBFILE)

# Target to build all DOS libraries

.PHONY mkall: mkdos

.PHONY mkdos: mkdos16 mkdos32 cleanexe

# Target to build all DOS 16-bit libraries

.PHONY mkdos16:
	@build bc31-d16 install -u OPT_SIZE=1
	@build bc45-d16 install -u OPT_SIZE=1
	@build bc50-d16 install -u OPT_SIZE=1
	@build vc40-d16 install -u OPT_SIZE=1
	@build wc10-d16 install -u OPT_SIZE=1
	@build wc11-d16 install -u OPT_SIZE=1

# Target to build all DOS 32-bit libraries

.PHONY mkdos32:
	@build bc45-d32 install -u OPT_SIZE=1
	@build bc50-d32 install -u OPT_SIZE=1
	@build wc10-d32 install -u OPT_SIZE=1
	@build wc11-d32 install -u OPT_SIZE=1
	@build vc40-tnt install -u OPT_SIZE=1
	@build vc50-tnt install -u OPT_SIZE=1
	@build dj20-d32 install -u OPT_SIZE=1

# Target to build all libraries for SciTech Display Doctor

.PHONY mksdd:
	@build_db bc50-d16 install -u DBG=1
	@build    bc50-d16 install -u OPT_SIZE=1
	@build_db wc10-d32 install -u DBG=1
	@build    wc10-d32 install -u OPT_SIZE=1
	@build_db wc10-w32 install -u DBG=1
	@build    wc10-w32 install -u OPT_SIZE=1
	@build_db wc11-d32 install -u DBG=1
	@build    wc11-d32 install -u OPT_SIZE=1
	@dmake cleanexe

.PHONY mksddos2:
	@build_db va30-o32 install -u DBG=1
	@build    va30-o32 install -u OPT_SIZE=1
	@dmake cleanexe

.INCLUDE: "$(SCITECH)/makedefs/common.mk"
