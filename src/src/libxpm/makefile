#############################################################################
#
#                   Copyright (C) 1999 SciTech Software
#                           All rights reserved.
#
# Descripton:   Generic makefile for libxpm library.
#
#############################################################################

# Force compile for Win32

.IF $(USE_WIN32)
CFLAGS	+= -DWIN32 -DFOR_MSW
.ENDIF

# Name of library and object files required to build it

.IF $(BUILD_DLL)
DLLFILE         = libxpm$D
LIBFILE         = libxpm$L
.ELSE
.IF $(STKCALL)
LIBFILE         = $(LP)slibxpm$L
.ELSE
LIBFILE         = $(LP)libxpm$L
.ENDIF
.ENDIF
LIBCLEAN        = *.lib *.a

# define library sources

OBJECTS			= crbuffri$O crdatfri$O create$O crifrbuf$O crifrdat$O \
				  data$O hashtab$O misc$O parse$O rdftodat$O rdftoi$O \
				  rgb$O scan$O simx$O wrffrdat$O wrffrp$O wrffri$O

# Target to build all libraries for SciTech Display Doctor

.PHONY mksdd:
	@build bc50-w32 install -u OPT_SIZE=1 CHECKED=1 DBG=1
	@build bc50-w32 install -u OPT_SIZE=1
	@dmake cleanexe

# Define the list of object files to create dependency information for

DEPEND_OBJ      = $(OBJECTS)

.INCLUDE: "$(SCITECH)/makedefs/common.mk"
