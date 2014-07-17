#############################################################################
#
#                   Copyright (C) 1999 SciTech Software
#                           All rights reserved.
#
# Descripton:   Generic makefile for the FreeType library. Requires the
#               SciTech Software makefile definitions package to be
#               installed.
#
#############################################################################

#----------------------------------------------------------------------------
# Define the lists of object files
#----------------------------------------------------------------------------

CFLAGS			+= -Iextend

DLL_OBJS		= dllstart$O ttinit$O
OBJ_MAIN		= ttapi$O ttcache$O ttcalc$O ttcmap$O ttdebug$O \
				  ttfile$O ttgload$O ttinterp$O ttload$O \
				  ttmemory$O ttmutex$O ttobjs$O ttraster$O ttextend$O
OBJ_EXT			= ftxgasp$O ftxkern$O ftxpost$O ftxcmap$O ftxwidth$O
OBJECTS         = $(OBJ_MAIN) $(OBJ_EXT)

.IF $(BUILD_DLL)
CFLAGS      	+= -I$(PRIVATE)\include\drvlib -I$(SCITECH)\include\drvlib -D__DRIVER__
ASFLAGS   		+= -d__DRIVER__
EXELIBS			= drvlib$L
.ELSE
LIBFILE         = $(LP)freetype$L
LIBCLEAN  		= *.dll *.lib *.a
.ENDIF

#----------------------------------------------------------------------------
# Target to build the Binary Portable DLL target
#----------------------------------------------------------------------------

freetype.dll: $(DLL_OBJS) $(OBJECTS)

ftdump$E: ftdump$O common$O $(LIBFILE)

#----------------------------------------------------------------------------
# Target to build all Intel binary drivers
#----------------------------------------------------------------------------

.PHONY mksdd:
	@build wc11-w32 freetype.dll -u BUILD_DLL=1 NO_RUNTIME=1 OPT=1
	@k_cp freetype.dll $(SCITECH)\drivers\freetype.bpd
	@dmake cleanexe

.PHONY db:
	@build wc11-w32 freetype.dll BUILD_DLL=1 NO_RUNTIME=1 OPT=1
	@k_cp freetype.dll $(SCITECH)\drivers\freetype.bpd

#----------------------------------------------------------------------------
# Define the list of object files to create dependency information for
#----------------------------------------------------------------------------

DEPEND_OBJ      = $(DLL_OBJS) $(OBJECTS)
DEPEND_SRC   	= extend;test
.SOURCE: 		  extend test

.INCLUDE: "$(SCITECH)/makedefs/common.mk"
