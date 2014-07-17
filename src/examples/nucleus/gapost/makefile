############################################################################
#
#                   Copyright (C) 1998 SciTech Software.
#                           All rights reserved.
#
# Descripton:   Makefile script to build gapost program.
#
#############################################################################

.IMPORT .IGNORE : ISV_LICENSE

#----------------------------------------------------------------------------
# DOS extender dependant flags
#----------------------------------------------------------------------------

CFLAGS      += $(DX_CFLAGS)
ASFLAGS   	+= $(DX_ASFLAGS)
.IF $(USE_QNX4)
LDFLAGS		+= -T1
USE_BIOS	:= 1
.ENDIF

.IF $(ISV_LICENSE)
CFLAGS		+= -DISV_LICENSE
.ENDIF

#----------------------------------------------------------------------------
# Define the lists of object files
#----------------------------------------------------------------------------

GATST_OBJS  = gapost$O

EXELIBS     = $(LL)n_ga$(LE)

#----------------------------------------------------------------------------
# Define the targets to build the test programs and analysis tools
#----------------------------------------------------------------------------

all: gapost$E

gapost$E: 	$(GATST_OBJS)

#----------------------------------------------------------------------------
# Target to build and install for Linux
#----------------------------------------------------------------------------

.IF $(UNIX)
install: gapost$E
.IF $(LIBC)
	@k_cp gapost$E $(SCITECH)/src/linux/libc5/nucleus/gapost
.ELSE
	@k_cp gapost$E $(SCITECH)/src/linux/glibc/nucleus/gapost
.ENDIF
.ENDIF

#----------------------------------------------------------------------------
# Target to build and install conformance program for DOS
#----------------------------------------------------------------------------

.PHONY mksdd:
	@build wc11-d32 -u gapost.exe PRO=1 OPT=1
	@k_cp gapost.exe $(PRIVATE)/src/graphics/conform/dos
	@dmake cleanexe

#----------------------------------------------------------------------------
# Define the list of object files to create dependency information for
#----------------------------------------------------------------------------

DEPEND_OBJ      = $(GATST_OBJS)
DEPEND_SRC   	= $(PRIVATE)/src/graphics;$(PRIVATE)/src/graphics/conform
.SOURCE: 		  $(PRIVATE)/src/graphics $(PRIVATE)/src/graphics/conform

.INCLUDE: "$(SCITECH)/makedefs/common.mk"
