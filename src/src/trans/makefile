#############################################################################
#
#                   Copyright (C) 1996 SciTech Software.
#                           All rights reserved.
#
# Descripton:   Makefile for UniVBE(tm), UniPOWER(tm), UVBELib(tm) and
#               DPMSLib library files. Requires Borland C++ 4.52 to build
#               some components.
#
# $Date:   26 Sep 1997 20:43:08  $ $Author:   KendallB  $
#
#############################################################################

# Add DOS extender dependant flags to command line

CFLAGS          += $(DX_CFLAGS)
ASFLAGS         += $(DX_ASFLAGS)

trans$E: trans$O getopt$O

.INCLUDE: "$(SCITECH)/makedefs/common.mk"

