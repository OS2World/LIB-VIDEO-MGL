#############################################################################
#
#                   Copyright (C) 1996 SciTech Software.
#                           All rights reserved.
#
# Descripton:   Generic makefile for the MegaGraph Graphics Library.
#               Requires the SciTech Software makefile definitions package
#               to be installed.
#
#               Builds the appropriate version depending on what target
#               system is selected.
#
#############################################################################

# DOS Extender dependant flags

CFLAGS          += $(DX_CFLAGS)
ASFLAGS         += $(DX_ASFLAGS)

LIBCLEAN        = mgl*.lib mgl*.a smgl*.lib mgl*.dll

# Import variables from environment

.IMPORT .IGNORE: NO_ASM BUILD_ASSEMBLER ELFOBJ SAMPLE TEST_HARNESS GM_SAMPLE
.IMPORT .IGNORE: DEBUG_MGL DEBUG_JPEG DEBUG_PNG DEBUG_FREETYPE DEBUG_OPENGL

# Target to build a sample program for debugging

.IF $(SAMPLE)
.PHONY sample: $(SAMPLE)$E
.ENDIF

.IF $(GM_SAMPLE)
.PHONY gmsample: $(GM_SAMPLE)$E
.ENDIF

# Compile for No Assembler if requested

.IF $(NO_ASM)
CFLAGS          += -DNO_ASSEMBLER
.ENDIF

# List of all generic object files common to all versions

COBJ			= mgraph$O createdc$O devctx$O buffer$O state$O palette$O 	\
				  glyph$O line$O bitblt$O putbmp$O 							\
				  clipline$O cliplfx$O scanline$O pixel$O rect$O mglfile$O 	\
				  viewport$O stubs$O list$O cursor$O memset$O random$O 		\
				  color$O mgldll$O halftone$O rtrav$O blocklst$O			\
				  ellipse$O ellipsea$O fatline$O polygon$O polyline$O		\
				  rectangl$O rspan$O rseg$O region$O rdraw$O runion$O 		\
				  rsect$O rdiff$O rmisc$O rrect$O rline$O rpoly$O rfpoly$O 	\
				  rellip$O rellipa$O text$O wtext$O texthelp$O 				\
				  vecfont$O bitfont$O font$O icon$O bitmap$O pcx$O jpeg$O 	\
				  tiff$O png$O cplxpoly$O cnvxpoly$O polyhlp$O mouse$O

DRVCOBJ			= packed8$O packed16$O packed24$O packed32$O

CPPOBJ			= mglcpp$O mglpoint$O mglrect$O mglrgn$O

.IF $(NO_ASM)
.ELSE
ASMOBJ			= _fix32$O _memset$O _polyhlp$O _random$O
DRVASMOBJ		=
.ENDIF

# Enable Nucleus source debugging when building a test harness

.IF $(TEST_HARNESS)
CFLAGS			+= -DTEST_HARNESS
DEBUG_MGL		:= 1
EXELIBS 		+= $(LL)n_ref2d$(LE) $(LL)n_driver$(LE)
.ELSE
DRVCOBJ			+= galib$O peloader$O libcimp$O
DRVASMOBJ		+= _gatimer$O _ga_imp$O
.ENDIF

# Compile in stubs to BPD libraries when not building debug libraries

.IF $(DEBUG_MGL)
EXELIBS 		+=
.ELSE
BPDOBJ			+=
.ENDIF

.IF $(DEBUG_JPEG)
EXELIBS 		+= $(LL)jpeg$(LE)
.ELSE
BPDOBJ			+= jpeglib$O _jpg_exp$O
.ENDIF

.IF $(DEBUG_PNG)
EXELIBS 		+= $(LL)png$(LE)
.ELSE
BPDOBJ			+= pnglib$O _png_exp$O
.ENDIF

.IF $(DEBUG_FREETYPE)
EXELIBS 		+= $(LL)freetype$(LE)
.ELSE
BPDOBJ			+= ttlib$O _tt_exp$O
.ENDIF

.IF $(DEBUG_OPENGL)
CFLAGS			+= -DDEBUG_OPENGL -DNO_GL_MACROS -DNO_GLU_MACROS -DNO_MGL_IMPORTS
EXELIBS 		+= $(LL)mesagl$(LE) $(LL)mesaglu$(LE)
.ELSE
BPDOBJ			+= _opengl$O _glu$O
.ENDIF

# Define the list of object files to create dependency information for

MGLDEPEND_OBJ   = $(COBJ) $(DRVCOBJ) $(ASMOBJ) $(DRVASMOBJ) $(CPPOBJ)
MGLDEPEND_SRC   = emulate;asm386;drivers/packed;$(SCITECH)/src/common

# Include definitions for the target system

.IF $(USE_SMX32)
.INCLUDE: "mglsmx/mglsmx.mk"
.ELIF $(USE_WIN32)
.INCLUDE: "mglwin/mglwin.mk"
.ELIF $(USE_RTTARGET)
.INCLUDE: "mglrtt/mglrtt.mk"
.ELIF $(USE_OS232)
.IF $(USE_OS2GUI)
.INCLUDE: "mglpm/mglpm.mk"
.ELSE
.INCLUDE: "mglos2/mglos2.mk"
.ENDIF
.ELIF $(USE_QNX)
.INCLUDE: "mglqnx/mglqnx.mk"
.ELIF $(USE_LINUX)
.IF $(USE_X11)
.INCLUDE: "mglx/mglx11.mk"
.ELSE
.INCLUDE: "mgllinux/mgllinux.mk"
.ENDIF
.ELSE
.INCLUDE: "mgldos/mgldos.mk"
.ENDIF

OBJECTS         = $(COBJ) $(DRVCOBJ) $(LOCALCOBJ) $(LOCALDRVCOBJ) $(BPDOBJ)
.IF $(PCASM)
OBJECTS         += $(ASMOBJ) $(DRVASMOBJ) $(LOCALASMOBJ) $(LOCALDRVASMOBJ)
.ENDIF

# Tell dmake where to find source files

.SOURCE: emulate asm386 drivers/packed $(SCITECH)/src/libjpeg $(SCITECH)/src/libpng $(SCITECH)/src/zlib $(SCITECH)/src/freetype $(LOCALSRC)

# Target to build a sample program for debugging

.IF $(SAMPLE)
.SOURCE: $(SCITECH)/examples/mgl/samples
$(SAMPLE)$E: $(SAMPLE)$O mglsamp$O getopt$O $(LIBFILE)
.ENDIF

.IF $(GM_SAMPLE)
.SOURCE: $(SCITECH)/examples/mgl/gm
EXELIBS         += $(LL)gm$(LE)
$(GM_SAMPLE)$E: $(GM_SAMPLE)$O  $(LIBFILE)
.ENDIF

# Target to build the library file

all: $(LIBFILE)

# Target to build the C++ library file

$(LIBFILECPP): $(CPPOBJ)

libcpp: $(LIBFILECPP)

installcpp: libcpp
	$(INSTALL) $(LIBFILECPP) $(LIB_DEST)

.INCLUDE: "$(SCITECH)/makedefs/common.mk"

# Target to build all DOS 32-bit libraries

.PHONY mkdos32:
	@build wc10-d32 install -u OPT=1
	@build wc11-d32 install -u OPT=1
	@build bc45-d32 install -u OPT=1
	@build bc50-d32 install -u OPT=1
	@build vc40-tnt install -u OPT=1
	@build vc50-tnt install -u OPT=1
	@build dj20-d32 install -u OPT=1
	@build wc10-d32 installcpp -u OPT=1
	@build wc11-d32 installcpp -u OPT=1
	@build bc45-d32 installcpp -u OPT=1
	@build bc50-d32 installcpp -u OPT=1
	@build vc40-tnt installcpp -u OPT=1
	@build vc50-tnt installcpp -u OPT=1
	@build dj20-d32 installcpp -u OPT=1

# Target to build all DOS libraries

.PHONY mkdos: mkdos32 cleanexe

# Target to build all smx 32-bit libraries

.PHONY mksmx:
	@build bc50-smx install -u OPT=1
	@build bc50-smx installcpp -u OPT=1

# Target to build all win 32-bit libraries

.PHONY mkwin32:
	@build wc10-w32 install -u OPT=1
	@build wc11-w32 install -u OPT=1
	@build bc45-w32 install -u OPT=1
	@build bc50-w32 install -u OPT=1
	@build vc40-w32 install -u OPT=1
	@build vc50-w32 install -u OPT=1
	@build wc10-w32 installcpp -u OPT=1
	@build wc11-w32 installcpp -u OPT=1
	@build bc45-w32 installcpp -u OPT=1
	@build bc50-w32 installcpp -u OPT=1
	@build vc40-w32 installcpp -u OPT=1
	@build vc50-w32 installcpp -u OPT=1

.PHONY mkdll32:
	@build bc50-w32 install -u BUILD_DLL=1 USE_MT=1 OPT=1
	@build bc45-w32 install IMPORT_DLL=1
	@build bc50-w32 install IMPORT_DLL=1
	@build wc10-w32 install IMPORT_DLL=1
	@build wc11-w32 install IMPORT_DLL=1
	@build vc40-w32 install IMPORT_DLL=1
	@build vc50-w32 install IMPORT_DLL=1

# Target to build all Windows libraries

.PHONY mkwin: mkwin32 mkdll32 cleanexe

# Target to build all libraries

.PHONY mkall: mkdos32 mkwin32 mkdll32 mksmx cleanexe

# Target to build all libraries for SciTech Display Doctor

.PHONY mksdd:
	@build_db wc10-d32 install -u DBG=1
	@build    wc10-d32 install -u OPT=1
	@build_db wc11-d32 install -u DBG=1
	@build    wc11-d32 install -u OPT=1
	@build_db wc10-d32 installcpp -u DBG=1
	@build    wc10-d32 installcpp -u OPT=1
	@build_db wc11-d32 installcpp -u DBG=1
	@build    wc11-d32 installcpp -u OPT=1

# Target to build all cross platform assembler modules

.PHONY buildasm:
	dmake BUILD_ASSEMBLER=1 -u
	p4p open tasmobj\*.obj
	cp *.obj tasmobj
	dmake BUILD_ASSEMBLER=1 ELFOBJ=1 -u
	p4p open elfobj\obj\*.obj
	cp *.obj elfobj\obj
	dmake BUILD_ASSEMBLER=1 IBMCOBJ=1 -u
	p4p open ibmcobj\*.obj
	cp *.obj ibmcobj

# Target to build all cross platform assembler modules

.PHONY mkasm:
	build wc10-d32 buildasm BUILD_ASSEMBLER=1 -u
