############################################################################
#
#                   Copyright (C) 1998 SciTech Software.
#                           All rights reserved.
#
# Descripton:   Makefile script to build all standalone Nucleus test
#				programs.
#
#############################################################################

.IMPORT .IGNORE : ISV_LICENSE

#----------------------------------------------------------------------------
# DOS extender dependant flags
#----------------------------------------------------------------------------

CFLAGS      += $(DX_CFLAGS)
ASFLAGS   	+= $(DX_ASFLAGS) -I$(PRIVATE)\include\nucleus
.IF $(USE_QNX4)
LDFLAGS		+= -T1
.ENDIF

.IF $(TEST_HARNESS)
CFLAGS		+= -DTEST_HARNESS
PRO			:= 1
.ENDIF

.IF $(LITE)
CFLAGS		+= -DLITE
.ENDIF

.IF $(PRO)
CFLAGS		+= -DPRO
.ENDIF

.IF $(ISV_LICENSE)
CFLAGS		+= -DISV_LICENSE
.ENDIF

#----------------------------------------------------------------------------
# Define the lists of object files
#----------------------------------------------------------------------------

GATST_OBJS  = main$O getopt$O util$O dpmstest$O ddctest$O ddccitst$O 		\
			  txttest$O tests$O center$O tv$O
.IF $(PRO)
GATST_OBJS  += 2dtests$O 2dconf$O
.IF $(UNIX)
.ELSE
ASFLAGS   	+= -I$(SCITECH)\include\nucleus
GATST_OBJS  += initthnk$O _thunks$O
.ENDIF
.ENDIF

EXELIBS     = $(LL)n_ga$(LE) $(LL)console$(LE)

#----------------------------------------------------------------------------
# Define the targets to build the test programs and analysis tools
#----------------------------------------------------------------------------

all: gatest$E

gatest$E: 	$(GATST_OBJS)

#----------------------------------------------------------------------------
# Target to build and install for Linux
#----------------------------------------------------------------------------

.IF $(UNIX)
install: gatest$E
.IF $(LIBC)
	@k_cp gatest$E $(SCITECH)/src/linux/libc5/nucleus/gatest
.ELSE
	@k_cp gatest$E $(SCITECH)/src/linux/glibc/nucleus/gatest
.ENDIF
.ENDIF

#----------------------------------------------------------------------------
# Target to build and install conformance program for DOS
#----------------------------------------------------------------------------

.PHONY mksdd:
	@build wc11-d32 -u gatest.exe PRO=1 OPT=1
	@k_cp gatest.exe $(PRIVATE)/src/graphics/conform/dos
	@dmake cleanexe

.PHONY stripexe:
	@wstrip gatest.exe

#----------------------------------------------------------------------------
# Define the list of object files to create dependency information for
#----------------------------------------------------------------------------

DEPEND_OBJ      = $(GATST_OBJS)
DEPEND_SRC   	= $(PRIVATE)/src/graphics;$(PRIVATE)/src/graphics/conform
.SOURCE: 		  $(PRIVATE)/src/graphics $(PRIVATE)/src/graphics/conform

.INCLUDE: "$(SCITECH)/makedefs/common.mk"
