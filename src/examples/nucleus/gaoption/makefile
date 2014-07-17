############################################################################
#
#                   Copyright (C) 1998 SciTech Software.
#                           All rights reserved.
#
# Descripton:   Makefile script to build Nucleus gaoption program.
#
#############################################################################

#----------------------------------------------------------------------------
# DOS extender dependant flags
#----------------------------------------------------------------------------

CFLAGS      += $(DX_CFLAGS)
ASFLAGS   	+= $(DX_ASFLAGS)
.IF $(USE_QNX4)
LDFLAGS		+= -T1
.ENDIF

#----------------------------------------------------------------------------
# Define the lists of object files
#----------------------------------------------------------------------------

OBJECTS  	= gaoption$O

EXELIBS    	= $(LL)n_ga$(LE)

#----------------------------------------------------------------------------
# Define the targets to build the test programs and analysis tools
#----------------------------------------------------------------------------

gaoption$E: $(OBJECTS)

#----------------------------------------------------------------------------
# Target to build and install conformance program for DOS
#----------------------------------------------------------------------------

.PHONY mksdd:
	@build wc11-d32 -u gaoption.exe PRO=1 OPT=1
	@k_cp gaoption.exe $(PRIVATE)/src/graphics/conform/dos
	@dmake cleanexe

.PHONY stripexe:
	@wstrip gaoption.exe

#----------------------------------------------------------------------------
# Define the list of object files to create dependency information for
#----------------------------------------------------------------------------

DEPEND_OBJ      = $(OBJECTS)

.INCLUDE: "$(SCITECH)/makedefs/common.mk"
