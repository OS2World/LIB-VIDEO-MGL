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
#               OS/2 version for EMX/GNU C/C++.
#
#############################################################################

# Include standard startup script definitions
.IMPORT: SCITECH
.INCLUDE: "$(SCITECH)\makedefs\startup.mk"

# Disable warnings for macros redefined here that were given
# on the command line.
__.SILENT       := $(.SILENT)
.SILENT         := yes

# Import enivornment variables that we use common to all compilers
.IMPORT .IGNORE : TEMP SHELL INCLUDE LIB SCITECH PRIVATE SCITECH_LIB
.IMPORT .IGNORE : DBG OPT OPT_SIZE SHW BETA CHECKED USE_X11 USE_LINUX
.IMPORT .IGNORE : USE_EGCS USE_PGCC STATIC_LIBS LIBC
   TMPDIR       := $(TEMP)

# Standard file suffix definitions
   L            := .lib   	# Libraries
   E            := .exe   	# Executables for libc5
   O            := .obj   	# Objects
   A            := .asm   	# Assembler sources
   S            := .s           # GNU assembler sources
   P            := .cpp   	# C++ sources

# File prefix/suffix definitions. The following prefixes are defined, and are
# used primarily to abstract between the Unix style libXX.a naming convention
# and the DOS/Windows/OS2 naming convention of XX.lib.
   LP           :=      	# LP - Library file prefix (name of file on disk)
   LL           := -l		# Library link prefix (name of library on link command line)
   LE           :=      	# Library link suffix (extension of library on link command line)

# Import enivornment variables that we use
.IMPORT .IGNORE : EMX_LIBBASE USE_OS232 USE_OS2GUI

# We are compiling for a 32 bit envionment
   _32BIT_      := 1

# DMAKE uses this recipe to remove intermediate targets
.REMOVE :; $(RM) -f $<

# Turn warnings back to previous setting.
.SILENT := $(__.SILENT)

# We dont use TABS in our makefiles
.NOTABS         := yes

# Default commands for compiling, assembling linking and archiving.
   CC           := gcc
   CFLAGS       := -Zomf -Wall -I. -I$(INCLUDE)
   CXX          := gcc -x c++
.IF $(USE_NASM)
   AS           := nasm
   ASFLAGS      := -t -f obj -F null -d__FLAT__ -d__NOU__ -iINCLUDE -i$(SCITECH)\INCLUDE
#  ASFLAGS      := -t -f obj -F null -d__FLAT__ -d__NOU_VAR__ -d__NOU__ -iINCLUDE -i$(SCITECH)\INCLUDE
#  ASFLAGS      := -t -f aout -d__FLAT__ -iinclude -i$(SCITECH)/include -d__NOU__
.ELSE
   AS           := tasm     # Assembler and flags
   ASFLAGS      := /t /mx /m /D__FLAT__ /D__NOU__ /iINCLUDE /i$(SCITECH)\INCLUDE /DDJGPP
.ENDIF
   LD           := gcc
   LDXX		:= gcc -x c++
   LDFLAGS      := -L. -Zomf
   LIB          := emxomfar
   LIBFLAGS     := -p32 rcv

   YACC		:= bison -y
   LEX		:= flex
   SED		:= sed
   RM		:= k_rm -f

# Link to static libraries if requested
.IF $(STATIC_LIBS)
   LDFLAGS      += -static
.ENDIF

# Optionally turn on debugging information
.IF $(DBG)
   CFLAGS       += -g
.ELSE
# NASM does not support debugging information yet
   ASFLAGS      +=
.ENDIF

# Optionally turn on optimisations
.IF $(OPT_MAX)
   CFLAGS       += -O6
.ELIF $(OPT)
   CFLAGS       += -O2
.ELIF $(OPT_SIZE)
   CFLAGS       += -O1
.ENDIF

# Optionally turn on direct i387 FPU instructions
.IF $(FPU)
   CFLAGS       += -DFPU387
   ASFLAGS      += -dFPU387
.END

# Optionally compile a beta release version of a product
.IF $(BETA)
   CFLAGS       += -DBETA
   ASFLAGS      += -dBETA
.ENDIF

# Disable standard C runtime library

.IF $(NO_RUNTIME)
CFLAGS			+= -fno-builtin -nostdinc
.ENDIF

# Target environment dependant flags
   CFLAGS    	+= -D__OS2_32__
   CFLAGS    	+= -D__OS2__
   ASFLAGS   	+= -d__OS2__

# Define the base directory for library files

.IF $(CHECKED)
LIB_BASE_DIR	:= $(SCITECH_LIB)/lib/debug
CFLAGS		+= -DCHECKED=1
.ELSE
LIB_BASE_DIR	:= $(SCITECH_LIB)/lib/release
.ENDIF

# Define where to install library files
   LIB_DEST     := $(LIB_BASE_DIR)\OS232\$(EMX_LIBBASE)
   LDFLAGS      += -L$(LIB_DEST)

# Build 32-bit OS/2 apps
.IF $(BUILD_DLL)
   CFLAGS       += -Zdll -DBUILD_DLL
   LDFLAGS      += -Zdll
   ASFLAGS      += -dBUILD_DLL
.ELSE
.IF $(USE_OS2GUI)
   CFLAGS	+= -D__OS2_PM__
   LDFLAGS      += -Zlinker /PMTYPE:PM
.ELSE
   CFLAGS	+= -D__OS2_CONSOLE__
   LDFLAGS      += -Zlinker /PMTYPE:VIO
.ENDIF
.ENDIF

# Place to look for PMODE library files

PMLIB           := -lpm

# Define which file contains our rules

   RULES_MAK	:= emx_os2.mk
