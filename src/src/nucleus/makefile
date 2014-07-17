############################################################################
#
#                   Copyright (C) 1998 SciTech Software.
#                           All rights reserved.
#
# Descripton:   Makefile script to build all standalone Nucleus test
#				programs. You can build integrated test harness to debug
#				driver code at the source level from within the local driver
#				directory.
#
#############################################################################

#----------------------------------------------------------------------------
# DOS extender dependant flags
#----------------------------------------------------------------------------

CFLAGS      += $(DX_CFLAGS)
ASFLAGS   	+= $(DX_ASFLAGS)

#----------------------------------------------------------------------------
# Define the lists of object files
#----------------------------------------------------------------------------

BASE_OBJS	= peloader$O libcimp$O

.IF $(USE_VXD)
GA_OBJS		= galib$O _ga_imp$O _gatimer$O
.ELSE
GA_OBJS		= galib$O gasdk$O vgapal$O gtfcalc$O _ga_imp$O _gatimer$O
.ENDIF

LINUX_OBJS	= galinux$O

QNX_OBJS	= gaqnx$O

WIN32_OBJS	= gawin32$O

VXD_OBJS	= gavxd$O

OS2_OBJS	= gaos2$O

DOS_OBJS	= gados$O

BEOS_OBJS	= gabeos$O

SMX32_OBJS  = gasmx$O

RTT32_OBJS  = gartt$O

#----------------------------------------------------------------------------
# Find the name of the library file to build
#----------------------------------------------------------------------------

OBJECTS		= $(BASE_OBJS) $(GA_OBJS)
.IF $(STKCALL)
LIBFILE   	= $(LP)sn_ga$L
.ELSE
LIBFILE    	= $(LP)n_ga$L
.ENDIF

LIBCLEAN	= *.lib *.a *.dll

#----------------------------------------------------------------------------
# Define the object files for each supported platform
#----------------------------------------------------------------------------

.IF $(USE_LINUX)

# Linux target platform

OBJECTS         += $(LINUX_OBJS)

.ELIF $(USE_QNX)

# QNX target platform

OBJECTS         += $(QNX_OBJS)

.ELIF $(USE_VXD)

# Win32 VxD target platform

OBJECTS         += $(VXD_OBJS)

.ELIF $(USE_WIN32)

# Win32 target platform

OBJECTS         += $(WIN32_OBJS)

.ELIF $(USE_OS232)

# OS/2 32-bit target platform

OBJECTS         += $(OS2_OBJS)

.ELIF $(USE_BEOS)

OBJECTS			+= $(BEOS_OBJS)

.ELIF $(USE_SMX32)

OBJECTS 		+= $(SMX32_OBJS)

.ELIF $(USE_RTTARGET)

OBJECTS 		+= $(RTT32_OBJS)

.ELSE

# DOS target platform

OBJECTS         += $(DOS_OBJS)

.ENDIF

#----------------------------------------------------------------------------
# Default target to build the library file
#----------------------------------------------------------------------------

all: $(LIBFILE)

#----------------------------------------------------------------------------
# Target to build all DOS 32-bit libraries
#----------------------------------------------------------------------------

.PHONY mkdos32:
	@build wc10-d32 install -u OPT=1
	@build wc10-d32 install -u OPT=1 STKCALL=1
	@build wc11-d32 install -u OPT=1
	@build wc11-d32 install -u OPT=1 STKCALL=1
	@build bc45-d32 install -u OPT=1
	@build bc50-d32 install -u OPT=1
	@build vc40-tnt install -u OPT=1
	@build vc50-tnt install -u OPT=1
	@build dj20-d32 install -u OPT=1

#----------------------------------------------------------------------------
# Target to build all DOS libraries
#----------------------------------------------------------------------------

.PHONY mkdos: mkdos32 cleanexe

#----------------------------------------------------------------------------
# Target to build all win 32-bit libraries
#----------------------------------------------------------------------------

.PHONY mkwin32:
	@build wc10-w32 install -u OPT=1
	@build wc10-w32 install -u OPT=1 STKCALL=1
	@build wc11-w32 install -u OPT=1
	@build wc11-w32 install -u OPT=1 STKCALL=1
	@build bc45-w32 install -u OPT=1
	@build bc50-w32 install -u OPT=1
	@build vc40-w32 install -u OPT=1
	@build vc50-w32 install -u OPT=1
	@build vc60-w32 install -u OPT=1

#----------------------------------------------------------------------------
# Target to build all Windows libraries
#----------------------------------------------------------------------------

.PHONY mkwin: mkwin32 cleanexe

#----------------------------------------------------------------------------
# Target to build Linux libraries
#----------------------------------------------------------------------------

.PHONY mklinux:
	@build gcc-linux install -u OPT=1

#----------------------------------------------------------------------------
# Target to build QNX libraries
#----------------------------------------------------------------------------

.PHONY mkqnx:
	@build qnx4 install -u OPT=1

#----------------------------------------------------------------------------
# Target to build all libraries for target platform
#----------------------------------------------------------------------------

.IF $(USE_LINUX)
.PHONY mkall: mklinux
.ELIF $(USE_QNX)
.PHONY mkall: mkqnx
.ELSE
.PHONY mkall: mkdos32 mkwin32 cleanexe
.ENDIF

#----------------------------------------------------------------------------
# Target to build all libraries for SciTech Display Doctor for DOS/Win32
#----------------------------------------------------------------------------

.PHONY mksdd:
	@build_db wc10-d32 install -u DBG=1
	@build    wc10-d32 install -u OPT=1
	@build_db wc11-d32 install -u DBG=1
	@build	   wc11-d32 install -u OPT=1
	@build_db bc50-w32 install -u DBG=1
	@build    bc50-w32 install -u OPT=1
	@build_db bc50-vxd install -u DBG=1
	@build    bc50-vxd install -u OPT=1
	@dmake cleanexe

.PHONY mksddos2:
	@build_db va30-o32 install -u DBG=1
	@build    va30-o32 install -u OPT=1
	@dmake cleanexe

#----------------------------------------------------------------------------
# Define the list of object files to create dependency information for
#----------------------------------------------------------------------------

DEPEND_OBJ      = $(BASE_OBJS) $(GA_OBJS) $(LINUX_OBJS) \
				  $(QNX_OBS) $(WIN32_OBJS) $(DOS_OBJS) $(SMX32_OBJS) \
				   $(RTT32_OBJS)
DEPEND_INC		= $(SCITECH)\include\nucleus

.INCLUDE: "$(SCITECH)/makedefs/common.mk"
