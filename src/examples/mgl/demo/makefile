#############################################################################
#
#               Copyright (C) 1991-1997 SciTech Software, Inc.
#                            All rights reserved.
#
# Descripton:   Generic makefile for the MGL demo program. Requires the
#               SciTech Software makefile definitions package to be
#               installed.
#
#               NOTE: The floodfill code will break unless a LARGE stack is
#                     used to link the program. Run make with the command
#                     make STKSIZE=50000 or some other large number to
#                     link with a large stack for compilers other than
#                     Borland C++.
#
#############################################################################

.INCLUDE: "../common.mak"

# Comment the following out if you dont want pre-compiled headers

CFLAGS          += -DPC_HEADERS

# Name of libraries needed in link phase

.IF $(STKCALL)
EXELIBS         = $(LL)smvis$(LE) $(LL)stech$(LE) $(LL)smglcpp$(LE) $(BASELIBS)
.ELSE
EXELIBS         = $(LL)mvis$(LE) $(LL)tech$(LE) $(LL)mglcpp$(LE) $(BASELIBS)
.ENDIF

# Object files requied to build the demo

OBJECTS         = demo$O main$O explwind$O titlwind$O demoarea$O    \
				  demowind$O getopt$O

.IF $(MGLWIN)
OBJECTS         += demo$R
.ENDIF

USE_CXX_LINKER 	:= 1

demo$E: $(OBJECTS)

#----------------------------------------------------------------------------
# Target to build and install conformance program for DOS
#----------------------------------------------------------------------------

.PHONY mksdd:
	@build wc11-d32 -u demo.exe OPT=1
	@k_cp demo.exe $(PRIVATE)/src/graphics/conform/dos
	@dmake cleanexe

#----------------------------------------------------------------------------
# Define the list of object files to create dependency information for
#----------------------------------------------------------------------------

DEPEND_OBJ      = $(OBJECTS)

.INCLUDE: "$(SCITECH)/makedefs/common.mk"
