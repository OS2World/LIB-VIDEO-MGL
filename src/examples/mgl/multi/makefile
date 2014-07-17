############################################################################
#
#               Copyright (C) 1991-1997 SciTech Software, Inc.
#                            All rights reserved.
#
# Descripton:   Makefile for sample programs. Requires the SciTech
#               Software makefile definitions package to be installed.
#
#############################################################################

.INCLUDE: "../common.mak"

EXELIBS		= $(BASELIBS)

all: multi$E

multi$E:    multi$O

#----------------------------------------------------------------------------
# Target to build and install conformance program for DOS
#----------------------------------------------------------------------------

.PHONY mksdd:
	@build wc11-d32 -u multi.exe OPT=1
	@k_cp multi.exe $(PRIVATE)/src/graphics/conform/dos
    @dmake cleanexe

.INCLUDE: "$(SCITECH)/makedefs/common.mk"
