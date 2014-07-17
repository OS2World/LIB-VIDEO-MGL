#############################################################################
#
#                   Copyright (C) 1996 SciTech Software.
#                           All rights reserved.
#
# Descripton:   Generic makefile for the PM library. Builds the library
#               file and all test programs.
#
#############################################################################

#----------------------------------------------------------------------------
# Add DOS extender dependant flags to command line
#----------------------------------------------------------------------------

CFLAGS          += $(DX_CFLAGS)
ASFLAGS         += $(DX_ASFLAGS)
#NO_PMLIB        := 1

#----------------------------------------------------------------------------
# Include definitions specific for the target system
#----------------------------------------------------------------------------

.IF $(USE_VXD)

# Building for Win32 VxD (minimal PM library implementation)

LIBNAME			= pm
OBJECTS			= pm$O vflat$O ztimer$O cpuinfo$O mtrr$O fileio$O pcilib$O \
				  vgastate$O _pm$O _mtrr$O _cpuinfo$O _pcilib$O
DEPEND_SRC   	:= vxd;common;codepage;tests
.SOURCE:		   vxd common codepage tests

.ELIF $(USE_WIN32)

# Building for Win32

LIBNAME			= pm
OBJECTS			= pm$O vflat$O event$O ddraw$O ztimer$O cpuinfo$O pcilib$O \
				  vgastate$O _pmwin32$O _joy$O _cpuinfo$O _pcilib$O
DEPEND_SRC   	:= win32;common;codepage;tests
.SOURCE:		   win32 common codepage tests

.ELIF $(USE_OS232)

# Building for OS/2

.IF $(USE_OS2GUI)
LIBNAME			= pm_pm
.ELSE
LIBNAME			= pm
.ENDIF
OBJECTS			= pm$O vflat$O event$O ztimer$O cpuinfo$O mtrr$O pcilib$O \
				  vgastate$O _pmos2$O _joy$O _cpuinfo$O _pcilib$O
DEPEND_SRC   	:= os2;common;codepage;tests
.SOURCE:		   os2 common codepage tests

.ELIF $(USE_QNX)

# Building for QNX

USE_BIOS		:= 1
.IF $(USE_PHOTON)
LIBNAME			= pm_ph
.ELIF $(USE_X11)
LIBNAME			= pm_x11
.ELSE
LIBNAME			= pm
.ENDIF
OBJECTS			= pm$O vflat$O event$O ztimer$O cpuinfo$O mtrr$O pcilib$O \
				  mtrrqnx$O unixio$O vgastate$O _joy$O _mtrrqnx$O _cpuinfo$O _pcilib$O
DEPEND_SRC   	:= qnx;common;codepage;tests
.SOURCE:		   qnx common codepage tests
.IF $(USE_QNX4)
LDFLAGS			+= -T1
.ENDIF

.ELIF $(USE_LINUX)

# Building for Linux

CFLAGS			+= -DENABLE_MTRR -DUSE_OS_JOYSTICK
.IF $(USE_X11)
LIBNAME			= pm_x11
.ELSE
LIBNAME			= pm
.ENDIF
OBJECTS			= pm$O vflat$O event$O ztimer$O cpuinfo$O pcilib$O \
				  unixio$O vgastate$O _cpuinfo$O _pcilib$O
DEPEND_SRC   	:= linux;common;codepage;tests;x11
.SOURCE:		   linux common codepage tests x11

.ELIF $(USE_BEOS)

# Building for BeOS GUI

LIBNAME			= pm
OBJECTS			= pm$O vflat$O event$O ztimer$O cpuinfo$O pcilib$O \
				  vgastate$O _joy$O _cpuinfo$O _pcilib$O
DEPEND_SRC   	:= beos;common;codepage;tests
.SOURCE:		   beos common codepage tests

.ELIF $(USE_SMX32)

# Building for SMX

LIBNAME			= pm
OBJECTS			= pm$O pmsmx$O vflat$O event$O ztimer$O cpuinfo$O mtrr$O pcilib$O \
				  vgastate$O _pm$O _pmsmx$O _mtrr$O _event$O _joy$O _cpuinfo$O _pcilib$O \
				  _lztimer$O
DEPEND_SRC   	:= smx;common;codepage;tests
.SOURCE:		   smx common codepage tests

.ELIF $(USE_RTTARGET)

# Building for RTTarget-32

LIBNAME			= pm
OBJECTS			= pm$O vflat$O event$O ztimer$O cpuinfo$O mtrr$O pcilib$O \
				  vgastate$O _mtrr$O _joy$O _cpuinfo$O _pcilib$O
DEPEND_SRC   	:= rttarget;common;codepage;tests
.SOURCE:		   rttarget common codepage tests

.ELSE

# Building for MSDOS

LIBNAME			= pm
OBJECTS			= pm$O pmdos$O vflat$O event$O ztimer$O cpuinfo$O mtrr$O \
				  pcilib$O vgastate$O \
				  _pm$O _pmdos$O _mtrr$O _vflat$O _event$O _joy$O _pcilib$O \
				  _cpuinfo$O _lztimer$O _dma$O
DEPEND_SRC   	:= dos;common;codepage;tests
.SOURCE:		   dos common codepage tests

.ENDIF

# Object modules for keyboard code pages

OBJECTS			+= us_eng$O

# Common object modules

OBJECTS			+= common$O
.IF $(CHECKED)
OBJECTS			+= debug$O
.ENDIF

#----------------------------------------------------------------------------
# Name of library and generic object files required to build it
#----------------------------------------------------------------------------

.IF $(STKCALL)
LIBFILE         = s$(LP)$(LIBNAME)$L
.ELSE
LIBFILE         = $(LP)$(LIBNAME)$L
.ENDIF
LIBCLEAN        = *.lib *.a

#----------------------------------------------------------------------------
# Change destination for library file depending the extender being used. This
# is only necessary for DOS extender since the file go into a subdirectory
# in the normal library directory, one for each supported extender. Other
# OS'es put the file into the regular library directory, since there is
# only one per OS in this case.
#----------------------------------------------------------------------------

MK_PMODE        = 1

.IF $(USE_TNT)
LIB_DEST        := $(LIB_BASE)\tnt
.ELIF $(USE_DOS4GW)
LIB_DEST        := $(LIB_BASE)\dos4gw
.ELIF $(USE_X32)
LIB_DEST        := $(LIB_BASE)\x32
.ELIF $(USE_DPMI16)
LIB_DEST        := $(LIB_BASE)\dpmi16
.ELIF $(USE_DPMI32)
LIB_DEST        := $(LIB_BASE)\dpmi32
.ELIF $(USE_DOSX)
LIB_DEST        := $(LIB_BASE)\dosx
.END

#----------------------------------------------------------------------------
# Names of all executable files built
#----------------------------------------------------------------------------

.IF $(USE_REALDOS)
EXEFILES        = memtest$E biosptr$E video$E isvesa$E callreal$E		\
                  mouse$E tick$E key$E key15$E brk$E altbrk$E           \
				  critical$E altcrit$E vftest$E rtc$E getch$E   		\
				  cpu$E timerc$E timercpp$E showpci$E uswc$E
.ELSE
EXEFILES        = memtest$E video$E isvesa$E callreal$E vftest$E getch$E \
				  cpu$E timerc$E timercpp$E showpci$E uswc$E
.ENDIF

all: $(EXEFILES)

$(EXEFILES): $(LIBFILE)

memtest$E:  memtest$O
biosptr$E:  biosptr$O
video$E:    video$O
isvesa$E:   isvesa$O
mouse$E:    mouse$O
tick$E:     tick$O
key$E:      key$O
key15$E:    key15$O
brk$E:    	brk$O
altbrk$E:   altbrk$O
critical$E: critical$O
altcrit$E:  altcrit$O
callreal$E: callreal$O
vftest$E:   vftest$O
rtc$E:   	rtc$O
getch$E:   	getch$O
cpu$E:   	cpu$O
timerc$E:   timerc$O
showpci$E:  showpci$O
uswc$E:     uswc$O

timercpp$E: timercpp$O
.IF $(USE_CXX_LINKER)
	$(LDXX) $(LDFLAGS) -o $@ $& $(EXELIBS) $(PMLIB) -lm
.ENDIF

#----------------------------------------------------------------------------
# Target to build all DOS 16-bit libraries
#----------------------------------------------------------------------------

.PHONY mkdos16:
	@build bc45-d16 install -u OPT_SIZE=1
	@build bc50-d16 install -u OPT_SIZE=1
	@build wc10-d16 install -u OPT_SIZE=1
	@build wc11-d16 install -u OPT_SIZE=1
	@build wc11-d16 install -u OPT_SIZE=1
	@dmake cleanexe

#----------------------------------------------------------------------------
# Target to build all DOS 32-bit libraries
#----------------------------------------------------------------------------

.PHONY mkdos32:
	@build bc45-d32 install -u OPT_SIZE=1
	@build bc45-tnt install -u OPT_SIZE=1
	@build bc50-d32 install -u OPT_SIZE=1
	@build bc50-tnt install -u OPT_SIZE=1
	@build wc10-d32 install -u OPT_SIZE=1
	@build wc10-d32 install -u OPT_SIZE=1 STKCALL=1
	@build wc10-tnt install -u OPT_SIZE=1
	@build wc10-tnt install -u OPT_SIZE=1 STKCALL=1
	@build wc11-d32 install -u OPT_SIZE=1
	@build wc11-d32 install -u OPT_SIZE=1 STKCALL=1
	@build wc11-tnt install -u OPT_SIZE=1
	@build wc11-tnt install -u OPT_SIZE=1 STKCALL=1
	@build vc40-tnt install -u OPT_SIZE=1
	@build vc50-tnt install -u OPT_SIZE=1
	@build dj20-d32 install -u OPT_SIZE=1
	@dmake cleanexe

#----------------------------------------------------------------------------
# Target to build all libraries for DOS
#----------------------------------------------------------------------------

.PHONY mkdos: mkdos16 mkdos32 cleanexe

#----------------------------------------------------------------------------
# Target to build all libraries for SMX
#----------------------------------------------------------------------------

.PHONY mksmx32:
	@build bc50-smx install -u OPT_SIZE=1
	@dmake cleanexe

#----------------------------------------------------------------------------
# Target to build all Win32 libraries
#----------------------------------------------------------------------------

.PHONY mkwin32:
	@build_db bc45-w32 install -u DBG=1
	@build    bc45-w32 install -u OPT_SIZE=1
	@build_db bc50-w32 install -u DBG=1
	@build    bc50-w32 install -u OPT_SIZE=1
	@build_db bc50-vxd install -u DBG=1
	@build    bc50-vxd install -u OPT_SIZE=1
#	@build_db cw40-w32 install -u DBG=1
#	@build    cw40-w32 install -u OPT_SIZE=1
#	@build_db cw50-w32 install -u DBG=1
#	@build    cw50-w32 install -u OPT_SIZE=1
#	@build_db va35-w32 install -u DBG=1
#	@build    va35-w32 install -u OPT_SIZE=1
#	@build_db va40-w32 install -u DBG=1
#	@build    va40-w32 install -u OPT_SIZE=1
	@build_db vc40-w32 install -u DBG=1
	@build    vc40-w32 install -u OPT_SIZE=1
	@build_db vc50-w32 install -u DBG=1
	@build    vc50-w32 install -u OPT_SIZE=1
	@build_db vc60-w32 install -u DBG=1
	@build    vc60-w32 install -u OPT_SIZE=1
	@build_db wc10-w32 install -u DBG=1
	@build    wc10-w32 install -u OPT_SIZE=1
	@build_db wc11-w32 install -u DBG=1
	@build    wc11-w32 install -u OPT_SIZE=1
	@dmake cleanexe

#----------------------------------------------------------------------------
# Target to build all libraries for SciTech Display Doctor
#----------------------------------------------------------------------------

.PHONY mksdd:
	@build_db bc50-d16 install -u DBG=1
	@build    bc50-d16 install -u OPT_SIZE=1
	@build_db wc10-d32 install -u DBG=1
	@build    wc10-d32 install -u OPT_SIZE=1
	@build_db wc11-d32 install -u DBG=1
	@build    wc11-d32 install -u OPT_SIZE=1
	@build_db bc50-w32 install -u DBG=1
	@build    bc50-w32 install -u OPT_SIZE=1
	@build_db bc50-vxd install -u DBG=1
	@build    bc50-vxd install -u OPT_SIZE=1
	@dmake cleanexe

.PHONY mksddos2:
	@build_db va30-o32 install -u DBG=1
	@build    va30-o32 install -u OPT_SIZE=1
	@build_db va30-p32 install -u DBG=1
	@build    va30-p32 install -u OPT_SIZE=1
	@dmake cleanexe

.PHONY mkgld:
	@build_db bc50-w32 install -u DBG=1
	@build    bc50-w32 install -u OPT_SIZE=1
	@build_db vc50-w32 install -u DBG=1
	@build    vc50-w32 install -u OPT_SIZE=1
	@build_db bc50-vxd install -u DBG=1
	@build    bc50-vxd install -u OPT_SIZE=1
	@dmake cleanexe

#----------------------------------------------------------------------------
# Define the list of object files to create dependency information for
#----------------------------------------------------------------------------

DEPEND_OBJ      := $(OBJECTS) memtest$O biosptr$O video$O isvesa$O mouse$O	\
				   tick$O key$O key$O brk$O altbrk$O critical$O altcrit$O   \
				   callreal$O vftest$O getch$O

.INCLUDE: "$(SCITECH)/makedefs/common.mk"
