############################################################################
#
#                   Copyright (c) 1996 SciTech Software.
#                           All rights reserved.
#
# Descripton:   Makefile for the OpenGL demos.
#
# $Date:   25 Jul 1997 01:31:52  $ $Author:   KendallB  $
#
#############################################################################

# Allow for compiling with GLUT library instead of Game Framework
.IMPORT .IGNORE: USE_GLUT

.INCLUDE: "../common.mak"

.IF $(USE_GLUT)
CFLAGS          += -DNO_GL_MACROS -DNO_GLU_MACROS
.IF $(USE_WIN32)
CFLAGS			+= -D_WIN32
.ENDIF
.ENDIF

OBJECTS         = skyfly$O fly$O database$O perfdraw$O        \
				  image$O random$O
.IF $(USE_GLUT)
OBJECTS         += glutmain$O
.ELSE
OBJECTS         += gm_main$O
.ENDIF

.IF $(USE_GLUT)
EXELIBS         = $(LL)glut32$(LE) \
				  $(LL)opengl32$(LE) $(LL)glu32$(LE)
.ELSE
EXELIBS         = $(LL)gm$(LE) $(BASELIBS)
.ENDIF

.IF $(USE_WIN32) && !$(USE_GLUT)
RCFILE          = skyfly$R
.ELSE
RCFILE          =
.ENDIF

skyfly$E: $(OBJECTS) $(RCFILE)

# Define the list of object files to create dependency information for

DEPEND_OBJ      = $(OBJECTS)

.INCLUDE: "$(SCITECH)/makedefs/common.mk"

