#############################################################################
#
#                   Copyright (C) 1996 SciTech Software
#                           All rights reserved.
#
# Descripton:   Generic makefile for the MegaVision Application Frameworks.
#               Requires the SciTech Software makefile definitions package
#               to be installed.
#
#               Specifying CHECKS on the command line will enable a
#               debugging version of the MVis that does extra runtime
#               checking.
#
#############################################################################

# Comment the following if you dont use pre-compiled headers

.IF $(NO_PCH)
.ELSE
CFLAGS          += -DPC_HEADERS
.ENDIF

# Include definitions for the target system

.IMPORT .IGNORE : MGLDOS MGLWIN MGLPM MGLX

.IF $(USE_WIN32)
CFLAGS          += -DMGLWIN
.ENDIF

# Name of library

.IF $(STKCALL)
LIBFILE         = $(LP)smvis$L
.ELSE
LIBFILE         = $(LP)mvis$L
.ENDIF

LIBCLEAN 		= *.dll *.lib *.a

# All the object modules in the library

OBJECTS =   mvision$O                                                   \
            hotkey$O mbackgnd$O mbitbtn$O mbutton$O mcheckbx$O          \
			mdesktop$O mdialog$O message$O mevent$O mfiledlg$O 			\
			mfontmgr$O mframe$O mgroup$O minputl$O misc$O mlabel$O 		\
			mlist$O mmenu$O mmenubar$O mmenuite$O mmltext$O mmodesel$O	\
			mprogram$O mradio$O mrendera$O mscrollb$O msgbox$O 			\
			mstatic$O msttext$O mtbbtn$O mtitleb$O mview$O mvtext$O		\
			mwindow$O safetyp$O validvw$O viewstk$O

# Define the list of object files to create dependency information for

DEPEND_OBJ      = $(OBJECTS)

.INCLUDE: "$(SCITECH)/makedefs/common.mk"

# Target to build all DOS 32-bit libraries

.PHONY mkdos32:
	@build wc10-d32 install -u OPT=1 NO_PCH=1
	@build wc11-d32 install -u OPT=1 NO_PCH=1
	@build bc45-d32 install -u OPT=1
	@build bc50-d32 install -u OPT=1
	@build vc40-tnt install -u OPT=1
	@build vc50-tnt install -u OPT=1
	@build dj20-d32 install -u OPT=1

# Target to build all DOS libraries

.PHONY mkdos: mkdos32 cleanexe

# Target to build all win 32-bit libraries

.PHONY mkwin32:
	@build wc10-w32 install -u OPT=1 NO_PCH=1
	@build wc11-w32 install -u OPT=1 NO_PCH=1
	@build bc45-w32 install -u OPT=1
	@build bc50-w32 install -u OPT=1
	@build vc40-w32 install -u OPT=1
	@build vc50-w32 install -u OPT=1

# Target to build all Unix libraries
.PHONY mkunix:
	@build $(UNIX_ENV) install -u OPT_SIZE=1

# Target to build all Windows libraries

.PHONY mkwin: mkwin32 cleanexe

# Target to build all libraries

.IF $(UNIX)
mkall: mkunix
.ELSE
.PHONY mkall: mkdos32 mkwin32 cleanexe
.ENDIF

# Target to build all libraries for SDD

.PHONY mksdd:
	@build_db wc10-d32 install -u DBG=1 NO_PCH=1
	@build    wc10-d32 install -u OPT=1 NO_PCH=1
	@build_db wc11-d32 install -u DBG=1 NO_PCH=1
	@build    wc11-d32 install -u OPT=1 NO_PCH=1
