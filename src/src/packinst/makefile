#############################################################################
#
#                   Copyright (C) 1995 SciTech Software
#                           All rights reserved.
#
# Descripton:   Makefile for the SDD/2 installer components.
#
#############################################################################

#----------------------------------------------------------------------------
# Define the lists of object files
#----------------------------------------------------------------------------

CFLAGS			+= -DSDDINST -I$(SCITECH)\include\packinst
PACKINST_OBJ	= globals$O packinst$O uac_comm$O uac_crc$O \
				  uac_crt$O uac_dcpr$O uac_sys$O unace$O

# Compress with lxLite

LXLITE			:= 1

#----------------------------------------------------------------------------
# Define the targets to build the test programs and analysis tools
#----------------------------------------------------------------------------

all: packinst$E

packinst$E: $(PACKINST_OBJ)

# Define the list of object files to create dependency information for

DEPEND_OBJ      = $(PACKINST_OBJ)

.INCLUDE: "$(SCITECH)\makedefs\common.mk"
