############################################################################
#
#                   Copyright (c) 1996 SciTech Software.
#                           All rights reserved.
#
# Descripton:   Makefile for the HELLO program. Requires the SciTech
#               Software makefile definitions package to be installed.
#
# $Date:   23 Oct 1996 16:53:44  $ $Author:   KendallB  $
#
#############################################################################

.INCLUDE: "../common.mak"

EXELIBS         = $(BASELIBS)
USE_CXX_LINKER	:= 1

hello$E: hello$O

DEPEND_OBJ      = hello$O

# Define the list of object files to create dependency information for

.INCLUDE: "$(SCITECH)/makedefs/common.mk"

