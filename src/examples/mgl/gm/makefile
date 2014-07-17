#############################################################################
#
#               Copyright (C) 1991-1997 SciTech Software, Inc.
#                            All rights reserved.
#
# Descripton:   Generic makefile for the gm library. Requires the
#               SciTech Software makefile definitions package to be
#               installed.
#
#############################################################################

.INCLUDE: "../common.mak"

# Name of library and object files required to build it

EXELIBS         = $(LL)gm$(LE) $(LL)mglcpp$(LE) $(BASELIBS)

.IF $(USE_WIN32)
RCFILE          = demo$R
.ELSE
RCFILE          =
.ENDIF

all: bounce$E sprite$E gears$E

bounce$E: bounce$O $(RCFILE)
sprite$E: sprite$O $(RCFILE)
gears$E: gears$O $(RCFILE)

DEPEND_OBJ := bounce$O sprite$O gears$O

.INCLUDE: "$(SCITECH)/makedefs/common.mk"
