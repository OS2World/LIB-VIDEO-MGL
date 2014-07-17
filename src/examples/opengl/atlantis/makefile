############################################################################
#
#                   Copyright (c) 1996 SciTech Software.
#                           All rights reserved.
#
# Descripton:   Makefile for the OpenGL demos.
#
# $Date:   04 Apr 1997 19:19:32  $ $Author:   KendallB  $
#
#############################################################################

.INCLUDE: "../common.mak"

OBJECTS         = atlantis$O dolphin$O whale$O shark$O swim$O
EXELIBS         = $(BASELIBS)

.IF $(MGLWIN)
OBJECTS         += atlantis$R
.ENDIF

atlantis$E: $(OBJECTS)

# Define the list of object files to create dependency information for

DEPEND_OBJ      = $(OBJECTS)

.INCLUDE: "$(SCITECH)/makedefs/common.mk"

