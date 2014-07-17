############################################################################
#
#                   Copyright (C) 1998 SciTech Software.
#                           All rights reserved.
#
# Descripton:   Makefile script to build all Nucleus profile program.
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

OBJECTS     = profmain$O getopt$O gaperf$O _profile$O

EXELIBS     = $(LL)n_ga$(LE) $(LL)console$(LE)

#----------------------------------------------------------------------------
# Define the targets to build the test programs and analysis tools
#----------------------------------------------------------------------------

gaperf$E:  $(OBJECTS)

#----------------------------------------------------------------------------
# Target to build and install performance program for DOS
#----------------------------------------------------------------------------

.PHONY mksdd:
	@build wc11-d32 -u gaperf.exe OPT=1
	@k_cp gaperf.exe $(PRIVATE)/src/graphics/conform/dos
	@dmake cleanexe

.PHONY stripexe:
	@wstrip gaperf.exe

#----------------------------------------------------------------------------
# Define the list of object files to create dependency information for
#----------------------------------------------------------------------------

DEPEND_OBJ      = $(OBJECTS)

.INCLUDE: "$(SCITECH)/makedefs/common.mk"
