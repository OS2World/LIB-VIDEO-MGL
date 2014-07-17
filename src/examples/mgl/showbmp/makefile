############################################################################
#
#               Copyright (C) 1991-1997 SciTech Software, Inc.
#                            All rights reserved.
#
# Descripton:   Makefile for the SHOWBMP program. Requires the SciTech
#               Software makefile definitions package to be installed.
#
# $Date:   01 Oct 1997 03:13:04  $ $Author:   KendallB  $
#
#############################################################################

.INCLUDE: "../common.mak"

OBJECTS         = showbmp$O getopt$O
EXELIBS         = $(BASELIBS)

.IF $(MGLWIN)
OBJECTS         += showbmp$R
.ENDIF

showbmp$E: $(OBJECTS)

DEPEND_OBJ      = $(OBJECTS)

showbmp$E: $(OBJECTS)

# Define the list of object files to create dependency information for

.INCLUDE: "$(SCITECH)/makedefs/common.mk"
