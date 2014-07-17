#############################################################################
#
#					SciTech Multi-platform Graphics Library
#
#  ========================================================================
#
#    The contents of this file are subject to the SciTech MGL Public
#    License Version 1.0 (the "License"); you may not use this file
#    except in compliance with the License. You may obtain a copy of
#    the License at http://www.scitechsoft.com/mgl-license.txt
#
#    Software distributed under the License is distributed on an
#    "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
#    implied. See the License for the specific language governing
#    rights and limitations under the License.
#
#    The Original Code is Copyright (C) 1991-1998 SciTech Software, Inc.
#
#    The Initial Developer of the Original Code is SciTech Software, Inc.
#    All Rights Reserved.
#
#  ========================================================================
#
# Descripton:   Generic DMAKE startup makefile definitions file. Assumes
#               that the SCITECH environment variable has been set to point
#               to where all our stuff is installed. You should not need
#               to change anything in this file.
#
#               Watcom C++ 10.x 32 bit version. Supports Rational's DOS4GW
#               DOS Extender, PMODE/W, Causeway, FlashTek's X32-VM,
#               Phar Lap's TNT DOS Extender, 32-bit Windows development and
#				32-bit OS/2 development.
#
#############################################################################

# Include standard startup script definitions
.IMPORT: SCITECH
.INCLUDE: "$(SCITECH)\makedefs\startup.mk"

# Import enivornment variables that we use
.IMPORT .IGNORE : USE_TNT USE_X32 USE_X32VM USE_PMODEW STKCALL USE_CAUSEWAY
.IMPORT .IGNORE : USE_WIN386 USE_OS232 USE_OS2GUI WC_LIBBASE NOOPT DOSSTYLE 
.IMPORT .IGNORE : OS2_SHELL USE_CODEVIEW

# We are compiling for a 32 bit envionment
   _32BIT_      := 1

# Default commands for compiling, assembling linking and archiving
   CC           := wcc386
   CP           := wpp386
   CFLAGS       := -zq-j-s-fpi87
.IF $(USE_NASM)
   AS			:= nasm
   ASFLAGS      := -t -f obj -d__FLAT__ -dSTDCALL_MANGLE -iINCLUDE -i$(SCITECH)\INCLUDE
.ELSE
.IF $(USE_TASM32)
   AS           := tasm32
.ELIF $(USE_TASMX)
   AS           := tasmx
.ELSE
   AS           := tasm
.ENDIF
   ASFLAGS      := /t /mx /m /w-res /w-mcp /D__FLAT__ /DSTDCALL_MANGLE /iINCLUDE /i$(SCITECH)\INCLUDE
.ENDIF
   LD           := wlink
   LDFLAGS       =
.IF $(USE_OS232)
   RC           := rc
.ELSE
   RC           := wrc
.ENDIF
.IF $(USE_WIN32)
   RCFLAGS      := -q /bt=nt
.ELIF $(USE_OS232)
.IF $(USE_OS2GUI)
   CFLAGS		+= -D__OS2_PM__
.ELSE
   CFLAGS		+= -D__OS2_CONSOLE__
.ENDIF
.ELSE
   RCFLAGS      := -q
.ENDIF
   LIB          := wlib
   LIBFLAGS     := -q
   ILIB         := wlib
   ILIBFLAGS    := -c
   INTEL_X86	:= 1

# Set the compiler warning level
.IF $(MAX_WARN)
   CFLAGS       += -w4
.ELSE
   CFLAGS       += -w1
.ENDIF

# Optionally turn on debugging information (Codeview format)
.IF $(DBG)
.IF $(USE_WIN32)
.IF $(USE_CODEVIEW)
   CFLAGS       += -d2 -hc
   LDFLAGS      += D CODEVIEW OPT CVPACK
.ELSE
   CFLAGS       += -d2
   LDFLAGS      += D A
.ENDIF
.ELSE
   CFLAGS       += -d2
   LDFLAGS      += D A
.ENDIF
   LIBFLAGS     += -p=128
.IF $(USE_NASM)
   ASFLAGS      += -F borland -g
.ELSE
.IF $(USE_TASM32)
   ASFLAGS      += /q		# TASM32 fucks up Watcom C++ debug info
.ELIF $(OS2_SHELL)
   ASFLAGS      += /q		# TASM for OS/2 fucks up Watcom C++ debug info
.ELSE
   ASFLAGS      += /zi
.ENDIF
.ENDIF
.ELSE
.IF $(USE_NASM)
   ASFLAGS      += -F null
.ELSE
   ASFLAGS      += /q
.ENDIF
.END

# Optionally turn on optimisations (with or without stack conventions)
.IF $(STKCALL)
.IF $(OPT)
   CFLAGS       += -onatx-5s-fp5
.ELIF $(OPT_SIZE)
   CFLAGS       += -onaslmr-5s-fp5
.ELIF $(NOOPT)
   CFLAGS       += -od-5s
.ELSE
   CFLAGS       += -3s
.END
.ELSE
.IF $(OPT)
   CFLAGS       += -onatx-5r-fp5
.ELIF $(OPT_SIZE)
   CFLAGS       += -onaslmr-5r-fp5
.ELIF $(NOOPT)
   CFLAGS       += -od-5r
.END
.END

# Optionally turn on direct i387 FPU instructions optimised for Pentium
.IF $(FPU)
   CFLAGS       += -DFPU387
   ASFLAGS      += -dFPU387
.END

# Optionally compile a shareware version of a product
.IF $(SHW)
   CFLAGS       += -DSHAREWARE
   ASFLAGS      += -dSHAREWARE
.END

# Optionally compile a beta release version of a product
.IF $(BETA)
   CFLAGS       += -DBETA
   ASFLAGS      += -dBETA
.END

# Use a larger stack during linking if requested, or use a default stack
# of 200k. The usual default stack provided by Watcom C++ is *way* to small
# for real 32 bit code development. We also need a *huge* stack for OpenGL
# software rendering also!
.IF $(USE_WIN32)
    # Not necessary for Win32 code.
.ELSE
.IF $(STKSIZE)
    LDFLAGS     += OP STACK=$(STKSIZE)
.ELSE
	LDFLAGS     += OP STACK=204800
.ENDIF
.ENDIF

.IF $(USE_TNT)                  # Use Phar Lap's TNT DOS Extender
	CFLAGS       += -bt=nt -DTNT
	ASFLAGS   	 += -dTNT
	LDFLAGS      += SYS NT OP STUB=GOTNT.EXE
	LIB_OS       = DOS32
.ELIF $(USE_X32VM)              # Use FlashTek X-32VM DOS extender
	CFLAGS       += -bt=dos
	LDFLAGS      += SYS X32RV
    DX_CFLAGS    += -DX32VM
	DX_ASFLAGS   += -dX32VM
    LIB_OS       = DOS32
.ELIF $(USE_X32)                # Use FlashTek X-32 DOS extender
	CFLAGS       += -bt=dos
	LDFLAGS      += SYS X32R
    DX_CFLAGS    += -DX32VM
	DX_ASFLAGS   += -dX32VM
    LIB_OS       = DOS32
.ELIF $(USE_OS232)
.IF $(BUILD_DLL)
	CFLAGS       += -bm-bd-bt=os2-DBUILD_DLL
	ASFLAGS      += -dBUILD_DLL
.ELSE
	CFLAGS       += -bm-bt=os2
.ENDIF
	DX_ASFLAGS   += -d__OS2__
    LIB_OS       = os232
.ELIF $(USE_WIN32)              # Build 32 bit Windows NT app
.IF $(BUILD_DLL)
	CFLAGS       += -bm-bd-bt=nt-DBUILD_DLL-D_WIN32
	ASFLAGS      += -dBUILD_DLL
.ELSE
	CFLAGS       += -bm-bt=nt-D_WIN32
.ENDIF
	DX_ASFLAGS   += -d__WINDOWS32__
    LIB_OS       = WIN32
.ELIF $(USE_WIN386)             # Build 32 bit Win386 extended app
.IF $(BUILD_DLL)
    CFLAGS       += -bd-bt=windows-DBUILD_DLL
	ASFLAGS      += -dBUILD_DLL
.ELSE
    CFLAGS       += -bt=windows
.ENDIF
	DX_ASFLAGS   += -d__WIN386__
    LIB_OS       = WIN386
.ELIF $(USE_PMODEW)             # PMODE/W
	CFLAGS       += -bt=dos
	USE_DOS4GW   := 1
	USE_REALDOS  := 1
	LDFLAGS      += SYS PMODEW
	DX_CFLAGS    += -DDOS4GW
	DX_ASFLAGS   += -dDOS4GW
	LIB_OS       = DOS32
.ELIF $(USE_CAUSEWAY)           # Causeway
	CFLAGS       += -bt=dos
	USE_DOS4GW   := 1
	USE_REALDOS  := 1
	LDFLAGS      += SYS CAUSEWAY
	DX_CFLAGS    += -DDOS4GW
	DX_ASFLAGS   += -dDOS4GW
	LIB_OS       = DOS32
.ELSE                           # Use DOS4GW
	CFLAGS       += -bt=dos
	USE_DOS4GW   := 1
	USE_REALDOS  := 1
	LDFLAGS      += SYS DOS4G
	DX_CFLAGS    += -DDOS4GW
	DX_ASFLAGS   += -dDOS4GW
	LIB_OS       = DOS32
.END

# Place to look for PMODE library files

.IF $(USE_WIN32)
.IF $(STKCALL)
PMLIB           := spm.lib,
.ELSE
PMLIB           := pm.lib,
.ENDIF
.ELIF $(USE_OS232)
.IF $(STKCALL)
.IF $(USE_OS2GUI)
PMLIB           := spm_pm.lib,
.ELSE
PMLIB           := spm.lib,
.ENDIF
.ELSE
.IF $(USE_OS2GUI)
PMLIB           := pm_pm.lib,
.ELSE
PMLIB           := pm.lib,
.ENDIF
.ENDIF
.ELIF $(USE_TNT)
.IF $(STKCALL)
PMLIB           := tnt\spm.lib,
.ELSE
PMLIB           := tnt\pm.lib,
.ENDIF
.ELIF $(USE_X32)
.IF $(STKCALL)
PMLIB           := x32\spm.lib,
.ELSE
PMLIB           := x32\pm.lib,
.ENDIF
.ELSE
.IF $(STKCALL)
PMLIB           := dos4gw\spm.lib,
.ELSE
PMLIB           := dos4gw\pm.lib,
.ENDIF
.END

# Define the base directory for library files

.IF $(CHECKED)
LIB_BASE_DIR	:= $(SCITECH_LIB)\lib\debug
CFLAGS			+= -DCHECKED=1
.ELSE
LIB_BASE_DIR	:= $(SCITECH_LIB)\lib\release
.ENDIF

# Define where to install library files
   LIB_BASE     := $(LIB_BASE_DIR)\$(LIB_OS)\$(WC_LIBBASE)
   LIB_DEST     := $(LIB_BASE)

# Define which file contains our rules

   RULES_MAK	:= wc32.mk
