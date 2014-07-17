############################################################################
#
#                   Copyright (C) 1998 SciTech Software.
#                           All rights reserved.
#
# Descripton:   Makefile script to build all Nucleus Hello World program.
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

OBJECTS  	= hello$O

EXELIBS    	= $(LL)n_ga$(LE)

#----------------------------------------------------------------------------
# Define the targets to build the test programs and analysis tools
#----------------------------------------------------------------------------

hello$E: 	$(OBJECTS)

#----------------------------------------------------------------------------
# Define the list of object files to create dependency information for
#----------------------------------------------------------------------------

DEPEND_OBJ      = $(OBJECTS)

.INCLUDE: "$(SCITECH)/makedefs/common.mk"
