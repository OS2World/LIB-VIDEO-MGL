############################################################################
#
#                   Copyright (c) 1996 SciTech Software.
#                           All rights reserved.
#
# Descripton:   Makefile for the OpenGL demos.
#
# $Date:   04 Apr 1997 19:20:54  $ $Author:   KendallB  $
#
#############################################################################

.INCLUDE: "../common.mak"

# Names of all executable files built

EXEFILES        = glbounce$E isosurf$E morph3d$E wave$E gears2$E  \
                  moth$E rings$E steam$E fractals$E mech$E \
				  gears$E morph2$E logo$E
EXELIBS         = $(BASELIBS)

.IF $(USE_WIN32)
RCFILE          = demo$R
.ELSE
RCFILE          =
.ENDIF

all: $(EXEFILES)

glbounce$E:  glbounce$O $(RCFILE)
isosurf$E:   isosurf$O $(RCFILE)
morph3d$E:   morph3d$O $(RCFILE)
wave$E:      wave$O $(RCFILE)
gears2$E:    gears2$O $(RCFILE)
moth$E:      moth$O $(RCFILE)
rings$E:     rings$O $(RCFILE)
steam$E:     steam$O $(RCFILE)
fractals$E:  fractals$O fviewer$O $(RCFILE)
mech$E:      mech$O $(RCFILE)
gears$E:     gears$O $(RCFILE)
morph2$E:    morph2$O $(RCFILE)
logo$E:      opengl_logo$O def_logo$O $(RCFILE)

.INCLUDE: "$(SCITECH)/makedefs/common.mk"

