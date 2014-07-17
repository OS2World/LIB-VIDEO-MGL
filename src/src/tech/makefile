#############################################################################
#
#                   Copyright (C) 1996 SciTech Software
#                           All rights reserved.
#
# Descripton:   Generic makefile for the Techniques Class Library. Requires
#               the SciTech Software makefile definitions package to be
#               installed.
#
# $Date:   25 Jun 1996 23:53:10  $ $Author:   KendallB  $
#
#############################################################################

# Name of library and object files required to build it

.IF $(STKCALL)
LIBFILE         = $(LP)stech$L
.ELSE
LIBFILE         = $(LP)tech$L
.ENDIF

LIBCLEAN        = *.lib *.a

OBJECTS         = dlist$O list$O hashtab$O error$O options$O str$O          \
				  dynstr$O substr$O memmgr$O hash_c$O slist$O

# Sample test programs

hashtst$E: hashtst$O $(LIBFILE)
tststr$E: tststr$O $(LIBFILE)

# Target to build all DOS 32-bit libraries

.PHONY mkdos32:
	@build wc10-d32 install -u OPT=1 NO_PCH=1
	@build wc11-d32 install -u OPT=1 NO_PCH=1
	@build bc45-d32 install -u OPT=1
	@build bc50-tnt install -u OPT=1
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
	@build $(UNIX_ENV) install -u OPT=1

# Target to build all Windows libraries

.PHONY mkwin: mkwin32 cleanexe

# Target to build all libraries

.PHONY mkall: mkdos32 mkwin32 cleanexe

# Target to build all libraries for SciTech Display Doctor

.PHONY mksdd:
	@build_db wc10-d32 install -u DBG=1
	@build    wc10-d32 install -u OPT=1
	@build_db wc11-d32 install -u DBG=1
	@build    wc11-d32 install -u OPT=1
	@build_db bc50-w32 install -u DBG=1
	@build    bc50-w32 install -u OPT=1
	@dmake cleanexe

# Define the list of object files to create dependency information for

DEPEND_OBJ      = $(OBJECTS)
.SOURCE: test

.INCLUDE: "$(SCITECH)/makedefs/common.mk"
