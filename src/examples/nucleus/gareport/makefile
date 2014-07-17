############################################################################
#
#                   Copyright (C) 1998 SciTech Software.
#                           All rights reserved.
#
# Descripton:   Makefile script to build all Nucleus gareport program.
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

OBJECTS  	= gareport$O

EXELIBS    	= $(LL)n_ga$(LE)

#----------------------------------------------------------------------------
# Define the targets to build the test programs and analysis tools
#----------------------------------------------------------------------------

gareport$E: 	$(OBJECTS)

.PHONY stripexe:
	@wstrip gareport.exe

#----------------------------------------------------------------------------
# Define the list of object files to create dependency information for
#----------------------------------------------------------------------------

DEPEND_OBJ      = $(OBJECTS)

.IF $(UNIX)
install: gareport$E
.IF $(LIBC)
	@k_cp gareport$E $(SCITECH)/src/linux/libc5/gareport
.ELSE
	@k_cp gareport$E $(SCITECH)/src/linux/glibc/gareport
.ENDIF
.ENDIF

.INCLUDE: "$(SCITECH)/makedefs/common.mk"
