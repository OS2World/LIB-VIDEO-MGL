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
#               Cygwin b20.1 port of GNU C/C++ to Win32.
#
#############################################################################

# Include standard startup script definitions
.IMPORT: SCITECH
.INCLUDE: "$(SCITECH)\makedefs\startup.mk"

# Override some file suffix definitions
   L            := .a     # Libraries
   O            := .o     # Objects

# Override the file prefix/suffix definitions for library naming.
   LP           := lib		# LP - Library file prefix (name of file on disk)
   LL           := -l		# Library link prefix (name of library on link command line)
   LE           :=			# Library link suffix (extension of library on link command line)

# Import enivornment variables that we use
.IMPORT .IGNORE : CYG_LIBBASE

# We are compiling for a 32 bit envionment
   _32BIT_      := 1

# Default commands for compiling, assembling linking and archiving
   CC           := gcc      # C-compiler and flags
   CFLAGS       := -Wall -I. -I$(SCITECH:s,\,/)/include -I$(PRIVATE:s,\,/)/include -D_POSIX_SOURCE
   AS			:= nasm
   ASFLAGS      := -t -f coff -F null -d__FLAT__ -d__GNUC__ -iINCLUDE -i$(SCITECH)\INCLUDE
   LD           := gcc    	# Loader and flags
.IF $(WIN32_GUI)
   LDFLAGS       = -mwindows -e _mainCRTStartup
.ELSE
   LDFLAGS       =
.ENDIF
   RC           := windres
   RCFLAGS		:= -O coff
   LIB          := ar       # Librarian
   LIBFLAGS     := rs
   USE_NASM		:= 1
   USE_GCC		:= 1

# Optionally turn on debugging information
.IF $(DBG)
   CFLAGS       += -g       # Turn on debugging for C compiler
.END

# Optionally turn on optimisations
.IF $(OPT)
   CFLAGS       += -O2
.ELIF $(OPT_SIZE)
   CFLAGS       += -O1
.END

# Optionally turn on direct i387 FPU instructions

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

   LDFLAGS       = -mwindows -e _mainCRTStartup

# DOS extender dependant flags
   DX_CFLAGS    +=
   DX_ASFLAGS   += -dGYGWIN

# Define the base directory for library files

.IF $(CHECKED)
LIB_BASE_DIR	:= $(SCITECH_LIB)\lib\debug
CFLAGS			+= -DCHECKED=1
.ELSE
LIB_BASE_DIR	:= $(SCITECH_LIB)\lib\release
.ENDIF

# Define where to install library files
   LIB_DEST     := $(LIB_BASE_DIR)\WIN32\$(CYG_LIBBASE)
   LDFLAGS      += -L$(LIB_DEST)

# Place to look for PMODE library files

PMLIB           := -lpm

# Define which file contains our rules

   RULES_MAK	:= cygwin.mk
