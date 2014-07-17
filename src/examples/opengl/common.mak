############################################################################
#
#                   Copyright (C) 1995 SciTech Software.
#                           All rights reserved.
#
# Descripton:   Common makefile definitions for the OpenGL sample programs.
#               Requires the SciTech Software makefile definitions package
#               to be installed.
#
#############################################################################

# Link with standard MGL libraries

.INCLUDE: "../../mgl.mak"

# Link with common libraries

.IF $(STKCALL)
BASELIBS         += $(LL)sglut$(LE)
.ELSE
BASELIBS         += $(LL)glut$(LE)
.ENDIF
