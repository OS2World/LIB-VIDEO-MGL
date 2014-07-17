############################################################################
#
#                   Copyright (c) 1996 SciTech Software.
#                           All rights reserved.
#
# Descripton:   Makefile for the OpenGL demos.
#
#############################################################################

.INCLUDE: "../common.mak"

OBJECTS         = a$O b$O d$O e$O f$O h$O i$O m$O n$O o$O \
                  p$O r$O s$O t$O w$O d_holder$O d_lamp$O \
                  d_logo$O d_logol$O d_logos$O track$O
EXELIBS         = $(BASELIBS)

.IF $(MGLWIN)
OBJECTS         += ideas$R
.ENDIF

ideas$E: $(OBJECTS)

# Define the list of object files to create dependency information for

DEPEND_OBJ      = $(OBJECTS)

.INCLUDE: "$(SCITECH)/makedefs/common.mk"

