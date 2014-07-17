#############################################################################
#
#                   Copyright (C) 1998 SciTech Software.
#                           All rights reserved.
#
# Descripton:   Global makefile to build SciTech MGL libraries and install
#               archive components.
#
#############################################################################

# Determine if this is the build server or an engineer machine

.IMPORT .IGNORE: BUILDSERVER DEBUG

# Comment this out to convert to non-beta build

#BETA_FLAG   = BETA=1

# Define directories for build

.IF $(UNIX)

SCRIPTS     = $(PRIVATE)/scripts/mgl/
BUILDS      = /builds/mgl/
SOURCE      = $(SCITECH)/src/
EXAMPLES    = $(SCITECH)/examples/

.ELSE

SCRIPTS     = $(PRIVATE)\scripts\mgl\
BUILDS      = \builds\mgl\
SOURCE      = $(SCITECH)\src\
EXAMPLES    = $(SCITECH)\examples\

.ENDIF
 
# Define the name of all install archives

MGLVER      = 500
MGLVERS     = 50
MGLBASE     = mglb$(MGLVER).exe
MGLFONT     = mglf$(MGLVER).exe
MGLSRC      = mgls$(MGLVER).exe
MGLDEMO     = mgldm$(MGLVER).exe
OGLDEMO     = ogldm$(MGLVER).exe
MGLD_BC45   = md$(MGLVERS)bc45.exe
MGLD_BC50   = md$(MGLVERS)bc50.exe
MGLD_DJ20   = md$(MGLVERS)dj2.exe
MGLD_VC42   = md$(MGLVERS)vc42.exe
MGLD_VC50   = md$(MGLVERS)vc50.exe
MGLD_VC60   = md$(MGLVERS)vc60.exe
MGLD_WC10   = md$(MGLVERS)wc10.exe
MGLD_WC11   = md$(MGLVERS)wc11.exe
MGLW_BC45   = mw$(MGLVERS)bc45.exe
MGLW_BC50   = mw$(MGLVERS)bc50.exe
MGLW_DP20   = mw$(MGLVERS)dp20.exe
MGLW_VC42   = mw$(MGLVERS)vc42.exe
MGLW_VC50   = mw$(MGLVERS)vc50.exe
MGLW_VC60   = mw$(MGLVERS)vc60.exe
MGLW_WC10   = mw$(MGLVERS)wc10.exe
MGLW_WC11   = mw$(MGLVERS)wc11.exe

# Define command line to build the library. If DEBUG is defined, we build
# the debug libraries that do not use Binary Portable DLL's for anything
# except the core Nucleus drivers. This allows developers to debug all
# the code in the Binary Portable DLL's (such as the JPEG and FreeType
# libraries).

.IF $(MULTIPLE)
MKALL       = mkall
DEBUG		:= 1
.ELSE
.IF $(DEBUG)
MKALL       = install -u DEBUG=1
.ELSE
MKALL       = install -u
.ENDIF
.ENDIF

# Common targets for all OS'es

T_SYNC      =   targets/sync
T_PM        =   targets/pm
T_CONSOLE   =   targets/console
T_NUCLEUS   =   targets/nucleus
T_TECH      =   targets/tech
T_MVIS      =   targets/mvis
T_FIXED     =   targets/fixed
T_MESA      =   targets/mesa
T_MESAGLU   =   targets/mesaglu
T_GLUT      =   targets/glut
T_GM        =   targets/gm
T_LIBJPEG   =   targets/libjpeg
T_FREETYPE  =   targets/freetype
T_MGL       =   targets/mgl
T_SDDHELP   =   targets/sddhelp
T_MGLDOS    =   targets/mgldos
T_MGLWIN    =   targets/mglwin
T_MGLOS2    =   targets/mglos2
T_MGLLNX    =   targets/mgllinux
T_MGLX11    =   targets/mglx11
T_MGLQNX    =   targets/mglqnx
T_MGLPH     =   targets/mglph
T_MGLSMX    =   targets/mglsmx
T_MGLDOS_S  =   targets/mgldos_s
T_MGLWIN_S  =   targets/mglwin_s
T_MGLOS2_S  =   targets/mglos2_s
T_MGLLNX_S  =   targets/mgllnx_s
T_MGLQNX_S  =   targets/mglqnx_s
T_OGLDOS_S  =   targets/ogldos_s
T_OGLWIN_S  =   targets/oglwin_s
T_OGLOS2_S  =   targets/oglos2_s
T_OGLLNX_S  =   targets/ogllnx_s
T_OGLQNX_S  =   targets/oglqnx_s
T_WISE      =   targets/wise
T_BUILDINC  =   targets/buildinc

# Target to continue build of all libraries for current compiler

.PHONY single:
	@dmake all_single

# Target to build all libraries for current compiler.

.PHONY build: rmtargets
	@dmake all_single

# Target to build all libraries, for all compilers

.PHONY buildall:  rmtargets
	@dmake MULTIPLE=1 all

# Target to build libraries for one compiler

all_single: $(T_PM) $(T_CONSOLE) $(T_TECH) $(T_MVIS) $(T_FIXED) \
			$(T_MESA) $(T_MESAGLU) $(T_GLUT) $(T_GM) $(T_LIBJPEG) \
			$(T_FREETYPE) $(T_MGL)
	@k_echo =============================================================================
	@k_echo DONE: Single compiler MGL build completed successfully!

# Target to build libraries for all compilers

all:    $(T_SYNC) $(T_PM) $(T_CONSOLE) $(T_NUCLEUS) $(T_TECH) $(T_MVIS) \
		$(T_FIXED) $(T_MESA) $(T_MESAGLU) $(T_GLUT) $(T_GM) $(T_LIBJPEG) \
		$(T_FREETYPE) \
		$(T_SDDHELP) $(T_MGLDOS) $(T_MGLWIN) $(T_MGLOS2) $(T_MGLLNX) \
		$(T_MGLX11) $(T_MGLQNX) $(T_MGLPH) $(T_MGLDOS_S) $(T_MGLWIN_S) \
		$(T_MGLOS2_S) $(T_MGLLNX_S) $(T_MGLQNX_S) $(T_OGLDOS_S) \
		$(T_OGLWIN_S) $(T_OGLOS2_S) $(T_OGLLNX_S) $(T_OGLQNX_S) $(T_WISE) \
		$(T_BUILDINC)
	@k_rm -f mk0*
	@k_echo =============================================================================
	@k_echo DONE: Full MGL build completed successfully!

# Targets to build each component

$(T_SYNC):
	@k_echo =============================================================================
	@k_echo START: Syncing files to depot
	@+p4p sync
	@k_cp done $(T_SYNC)

$(T_PM):
	@k_echo =============================================================================
	@k_echo START: Building PM Library
	@cdit $(SOURCE)pm dmake $(MKALL)
	@k_cp done $(T_PM)

$(T_CONSOLE):
	@k_echo =============================================================================
	@k_echo START: Building Console Library
	@cdit $(SOURCE)console dmake $(MKALL)
	@k_cp done $(T_CONSOLE)

$(T_NUCLEUS):
	@k_echo =============================================================================
	@k_echo START: Building Nucleus Loader Library
	@cdit $(SOURCE)nucleus dmake $(MKALL)
	@k_cp done $(T_NUCLEUS)

$(T_SDDHELP):
	@k_echo =============================================================================
	@k_echo START: Building SDD Helper Device Driver
.IF $(USE_OS232)
	@cdit $(PRIVATE)/src/sddos2/sddhelp dmake mksdd
.ELSE
	@cdit $(PRIVATE)/src/sdd9x/sddhelp dmake mksdd
.ENDIF
	@k_cp done $(T_SDDHELP)

$(T_TECH):
	@k_echo =============================================================================
	@k_echo START: Building Techniques Class Library
	@cdit $(SOURCE)tech dmake $(MKALL)
	@k_cp done $(T_TECH)

$(T_MVIS):
	@k_echo =============================================================================
	@k_echo START: Building MegaVision GUI Library
	@cdit $(SOURCE)mvis dmake $(MKALL)
	@k_cp done $(T_MVIS)

$(T_FIXED):
	@k_echo =============================================================================
	@k_echo START: Building Vector/Matrix Library
	@cdit $(SOURCE)fixed dmake $(MKALL)
	@k_cp done $(T_FIXED)

$(T_MESA):
.IF $(DEBUG)
	@k_echo =============================================================================
	@k_echo START: Building Mesa OpenGL Library
	@cdit $(SOURCE)mesa dmake $(MKALL)
.ENDIF	
	@k_cp done $(T_MESA)

$(T_MESAGLU):
.IF $(DEBUG)
	@k_echo =============================================================================
	@k_echo START: Building Mesa OpenGL Utility Library
	@cdit $(SOURCE)mesaglu dmake $(MKALL)
.ENDIF	
	@k_cp done $(T_MESAGLU)

$(T_GLUT):
	@k_echo =============================================================================
	@k_echo START: Building MGL GLUT Library
	@cdit $(SOURCE)glut dmake $(MKALL)
	@k_cp done $(T_GLUT)

$(T_GM):
	@k_echo =============================================================================
	@k_echo START: Building Game Framework Library
	@cdit $(SOURCE)gm dmake $(MKALL)
	@k_cp done $(T_GM)

$(T_LIBJPEG):
.IF $(DEBUG)
	@k_echo =============================================================================
	@k_echo START: Building JPEG Library
	@cdit $(SOURCE)libjpeg dmake $(MKALL)
.ENDIF
	@k_cp done $(T_LIBJPEG)

$(T_FREETYPE):
.IF $(DEBUG)
	@k_echo =============================================================================
	@k_echo START: Building FreeType Library
	@cdit $(SOURCE)freetype dmake $(MKALL)
.ENDIF
	@k_cp done $(T_FREETYPE)

$(T_MGL):
	@k_echo =============================================================================
	@k_echo START: Building Core MGL Library
	@cdit $(SOURCE)mgl dmake install -u
	@cdit $(SOURCE)mgl dmake installcpp -u
	@k_cp done $(T_MGL)

$(T_MGLDOS):
	@k_echo =============================================================================
	@k_echo START: Building Core MGL DOS Library
	@cdit $(SOURCE)mgl dmake mkdos
	@k_cp done $(T_MGLDOS)

$(T_MGLWIN):
	@k_echo =============================================================================
	@k_echo START: Building Core MGL Windows Library
	@cdit $(SOURCE)mgl dmake mkwin
	@k_cp done $(T_MGLWIN)

$(T_MGLOS2):
	@k_echo =============================================================================
	@k_echo START: Building Core MGL OS/2 Library
	@cdit $(SOURCE)mgl dmake mkos2
	@k_cp done $(T_MGLOS2)

$(T_MGLLNX):
	@k_echo =============================================================================
	@k_echo START: Building Core MGL Linux Library
	@cdit $(SOURCE)mgl dmake mklinux
	@k_cp done $(T_MGLLNX)

$(T_MGLX11):
	@k_echo =============================================================================
	@k_echo START: Building Core MGL X11 Library
	@cdit $(SOURCE)mgl dmake mkx11
	@k_cp done $(T_MGLX11)

$(T_MGLQNX):
	@k_echo =============================================================================
	@k_echo START: Building Core MGL QNX Library
	@cdit $(SOURCE)mgl dmake mkqnx
	@k_cp done $(T_MGLQNX)

$(T_MGLPH):
	@k_echo =============================================================================
	@k_echo START: Building Core MGL QNX Photon Library
	@cdit $(SOURCE)mgl dmake mkphoton
	@k_cp done $(T_MGLPH)

$(T_MGLSMX):
	@k_echo =============================================================================
	@k_echo START: Building Core MGL smx Library
	@cdit $(SOURCE)mgl dmake mksmx
	@k_cp done $(T_MGLSMX)

$(T_MGLDOS_S):
	@k_echo =============================================================================
	@k_echo START: Building MGL DOS Samples Programs
	@cdit $(EXAMPLES)mgl dmake mkdos
	@k_cp done $(T_MGLDOS_S)

$(T_MGLWIN_S):
	@k_echo =============================================================================
	@k_echo START: Building MGL Windows Samples Programs
	@cdit $(EXAMPLES)mgl dmake mkwin
	@k_cp done $(T_MGLWIN_S)

$(T_MGLOS2_S):
	@k_echo =============================================================================
	@k_echo START: Building MGL OS/2 Samples Programs
	@cdit $(EXAMPLES)mgl dmake mkos2
	@k_cp done $(T_MGLOS2_S)

$(T_MGLLNX_S):
	@k_echo =============================================================================
	@k_echo START: Building MGL Linux Samples Programs
	@cdit $(EXAMPLES)mgl dmake mklinux
	@k_cp done $(T_MGLLNX_S)

$(T_MGLQNX_S):
	@k_echo =============================================================================
	@k_echo START: Building MGL QNX Samples Programs
	@cdit $(EXAMPLES)mgl dmake mkqnx
	@k_cp done $(T_MGLQNX_S)

$(T_OGLDOS_S):
	@k_echo =============================================================================
	@k_echo START: Building OpenGL DOS Samples Programs
	@cdit $(EXAMPLES)opengl dmake mkdos
	@k_cp done $(T_OGLDOS_S)

$(T_OGLWIN_S):
	@k_echo =============================================================================
	@k_echo START: Building OpenGL Windows Samples Programs
	@cdit $(EXAMPLES)opengl dmake mkwin
	@k_cp done $(T_OGLWIN_S)

$(T_OGLOS2_S):
	@k_echo =============================================================================
	@k_echo START: Building OpenGL OS/2 Samples Programs
	@cdit $(EXAMPLES)opengl dmake mkos2
	@k_cp done $(T_OGLOS2_S)

$(T_OGLLNX_S):
	@k_echo =============================================================================
	@k_echo START: Building OpenGL Linux Samples Programs
	@cdit $(EXAMPLES)opengl dmake mklinux
	@k_cp done $(T_OGLLNX_S)

$(T_OGLQNX_S):
	@k_echo =============================================================================
	@k_echo START: Building OpenGL QNX Samples Programs
	@cdit $(EXAMPLES)opengl dmake mkqnx
	@k_cp done $(T_OGLQNX_S)

$(T_WISE):
	@k_echo =============================================================================
	@k_echo START: Building Wise install archives
	@runwise $(SCRIPTS)mksetup.wse
	@runwise $(SCRIPTS)mglbase.wse
	@copy $(SCRIPTS)mglbase.exe $(BUILDS)$(MGLBASE)
	@runwise $(SCRIPTS)mglfont.wse
	@copy $(SCRIPTS)mglfont.exe $(BUILDS)$(MGLFONT)
	@runwise $(SCRIPTS)mglsrc.wse
	@copy $(SCRIPTS)mglsrc.exe $(BUILDS)$(MGLSRC)
	@runwise $(SCRIPTS)mgldm.wse
	@copy $(SCRIPTS)mgldm.exe $(BUILDS)$(MGLDEMO)
	@runwise $(SCRIPTS)ogldm.wse
	@copy $(SCRIPTS)ogldm.exe $(BUILDS)$(OGLDEMO)
	@runwise $(SCRIPTS)mgldos-bc4.wse
	@copy $(SCRIPTS)mgldos-bc4.exe $(BUILDS)$(MGLD_BC45)
	@runwise $(SCRIPTS)mgldos-bc5.wse
	@copy $(SCRIPTS)mgldos-bc5.exe $(BUILDS)$(MGLD_BC50)
	@runwise $(SCRIPTS)mgldos-dj2.wse
	@copy $(SCRIPTS)mgldos-dj2.exe $(BUILDS)$(MGLD_DJ20)
	@runwise $(SCRIPTS)mgldos-vc4.wse
	@copy $(SCRIPTS)mgldos-vc4.exe $(BUILDS)$(MGLD_VC42)
	@runwise $(SCRIPTS)mgldos-vc5.wse
	@copy $(SCRIPTS)mgldos-vc5.exe $(BUILDS)$(MGLD_VC50)
	@runwise $(SCRIPTS)mgldos-vc6.wse
	@copy $(SCRIPTS)mgldos-vc6.exe $(BUILDS)$(MGLD_VC60)
	@runwise $(SCRIPTS)mgldos-wc10.wse
	@copy $(SCRIPTS)mgldos-wc10.exe $(BUILDS)$(MGLD_WC10)
	@runwise $(SCRIPTS)mgldos-wc11.wse
	@copy $(SCRIPTS)mgldos-wc11.exe $(BUILDS)$(MGLD_WC11)
	@runwise $(SCRIPTS)mglwin-bc4.wse
	@copy $(SCRIPTS)mglwin-bc4.exe $(BUILDS)$(MGLW_BC45)
	@runwise $(SCRIPTS)mglwin-bc5.wse
	@copy $(SCRIPTS)mglwin-bc5.exe $(BUILDS)$(MGLW_BC50)
	@runwise $(SCRIPTS)mglwin-vc4.wse
	@copy $(SCRIPTS)mglwin-vc4.exe $(BUILDS)$(MGLW_VC42)
	@runwise $(SCRIPTS)mglwin-vc5.wse
	@copy $(SCRIPTS)mglwin-vc5.exe $(BUILDS)$(MGLW_VC50)
	@runwise $(SCRIPTS)mglwin-vc6.wse
	@copy $(SCRIPTS)mglwin-vc6.exe $(BUILDS)$(MGLW_VC60)
	@runwise $(SCRIPTS)mglwin-dp2.wse
	@copy $(SCRIPTS)mglwin-dp2.exe $(BUILDS)$(MGLW_DP20)
	@runwise $(SCRIPTS)mglwin-wc10.wse
	@copy $(SCRIPTS)mglwin-wc10.exe $(BUILDS)$(MGLW_WC10)
	@runwise $(SCRIPTS)mglwin-wc11.wse
	@copy $(SCRIPTS)mglwin-wc11.exe $(BUILDS)$(MGLW_WC11)
	@k_cp done $(T_WISE)

$(T_BUILDINC):
	@k_echo =============================================================================
.IF $(BUILDSERVER)
	@k_echo START: Incrementing build number
	@cdit bldinc build bc45-d16 bldinc.exe -u OPT=1 $(BETA_FLAG)
    @-$(SOURCE)\bldinc\bldinc.exe
.ENDIF
	@k_cp done $(T_BUILDINC)

# Target to remove all target build flags

rmtargets:
	@k_rm -f targets/*
