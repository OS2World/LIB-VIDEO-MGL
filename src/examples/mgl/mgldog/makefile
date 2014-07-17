############################################################################
#
#               Copyright (C) 1991-1997 SciTech Software, Inc.
#                            All rights reserved.
#
# Descripton:   Makefile for the MGLDOG program. Requires the SciTech
#               Software makefile definitions package to be installed.
#
#############################################################################

.INCLUDE: "../common.mak"

OBJECTS         = mgldog$O getopt$O
EXELIBS         = $(BASELIBS)

.IF $(MGLWIN)
OBJECTS         += mgldog$R
.ENDIF

mgldog$E: $(OBJECTS)

DEPEND_OBJ      = $(OBJECTS)

# Define the list of object files to create dependency information for

.INCLUDE: "$(SCITECH)/makedefs/common.mk"
