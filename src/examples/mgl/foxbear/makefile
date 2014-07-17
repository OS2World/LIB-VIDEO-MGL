#############################################################################
#
#               Copyright (C) 1991-1997 SciTech Software, Inc.
#                            All rights reserved.
#
# Descripton:   Generic makefile for the ATI Fox & Bear demo ported to the
#               C++ and the MGL.
#
#############################################################################

.INCLUDE: "../common.mak"

# Set objects to be included in the make process
USE_CXX_LINKER	:= 1
OBJECTS         = foxbear$O fox$O bear$O apple$O fxsprite$O fxplane$O
DOSSND_OBJS     = err$O
SOUND_OBJS      = sound$O
WIN_OBJS        = foxbear$R
TOTAL_OBJS      = $(OBJECTS)
.IF $(STKCALL)
EXELIBS         = $(LL)sgm$(LE) $(LL)mglcpp$(LE) $(BASELIBS)
.ELSE
EXELIBS         = $(LL)gm$(LE) $(LL)mglcpp$(LE) $(BASELIBS)
.ENDIF

.IF $(USE_DWSTK)
CFLAGS          += -DUSE_DWSTK
TOTAL_OBJS      += $(SOUND_OBJS)
.ENDIF

.IF $(MGLWIN)

# Building Windows version

.IF $(USE_DWSTK)
EXELIBS         += $(LL)dwsw32$(LE)
TOTAL_OBJS      += $(WIN_OBJS)
.ENDIF

.ELSE

# Building DOS version

.IF $(USE_DWSTK)
TOTAL_OBJS      += $(DOSSND_OBJS)
EXELIBS         += $(LL)dwsp$(LE)
.ENDIF

.ENDIF

foxbear$E: $(TOTAL_OBJS)

#----------------------------------------------------------------------------
# Target to build and install conformance program for DOS
#----------------------------------------------------------------------------

.PHONY mksdd:
	@build wc11-d32 -u foxbear.exe OPT=1
	@k_cp foxbear.exe $(PRIVATE)/src/graphics/conform/dos
	@dmake cleanexe

#----------------------------------------------------------------------------
# Define the list of object files to create dependency information for
#----------------------------------------------------------------------------

DEPEND_OBJ      = $(OBJECTS) $(WIN_OBJS) $(DOSSND_OBJS) $(SOUND_OBJS)

# Define the list of object files to create dependency information for

.INCLUDE: "$(SCITECH)/makedefs/common.mk"
