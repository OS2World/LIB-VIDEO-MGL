#############################################################################
#
#               Copyright (C) 1991-1997 SciTech Software, Inc.
#                            All rights reserved.
#
# Descripton:   Makefile for the MGL sample Smacker file player program.
#
# $Date:   01 Oct 1997 03:13:06  $ $Author:   KendallB  $
#
#############################################################################

.INCLUDE: "../common.mak"

OBJECTS         = play$O
EXELIBS         = $(BASELIBS)

.IF $(MGLWIN)
OBJECTS         += play$R
.ENDIF

# Define the Smacker libraries to link with

.IF $(USE_WIN16)
EXELIBS         += ztimer$L smackw16$L
.ELIF $(USE_WIN32)
EXELIBS         += ztimer$L smackw32$L
.ELSE
EXELIBS         += ztimer$L smack$L msslite$L
.ENDIF

# Define the target to build

play$E: $(OBJECTS)

# Define the list of object files to create dependency information for

DEPEND_OBJ      = $(OBJECTS)

.INCLUDE: "$(SCITECH)/makedefs/common.mk"


