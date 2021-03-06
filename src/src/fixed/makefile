#############################################################################
#
#                   Copyright (C) 1996 SciTech Software
#                           All rights reserved.
#
# Descripton:   Generic makefile for the Techniques Class Library. Requires
#               the SciTech Software makefile definitions package to be
#               installed.
#
#               Creates a number of different libraries depending on the
#               options used to create them with the following format:
#
#                   vec<db>_<flt>.lib
#
#               where <db> is either empty or 'd' for the debugging versions
#               of the library compiled with DEBUG=2. You should set DEBUG=1
#               or DEBUG=2 in your own code to add the debugging stuff in
#               the header files. <flt> is the type of real numbers the
#               library is compiled for and will be one of the following:
#
#                   fix - 16.16 fixed point
#                   fl  - IEEE float's
#                   fl7 - IEEE float's for 80387+
#                   db  - IEEE double's
#                   db7 - IEEE double's for 80387+
#
#               The standard fl and db libraries use the default compiler
#               supplied floating point options, so work on all machines
#               with or without and FPU. The fl7 and db7 libraries are
#               compiled with the FPU387=1 flag and contain direct
#               co-processor support for 80387+ FPU's.
#
# $Date:   25 Jun 1996 23:33:00  $ $Author:   KendallB  $
#
#############################################################################

# DOS Extender dependant flags

CFLAGS          += $(DX_CFLAGS)

# Name of library and object files required to build it

.IMPORT .IGNORE : FX_FLOAT FX_DOUBLE FX_FIXED

.IF $(STKCALL)
LIBSTART        = $(LP)svec
.ELSE
LIBSTART        = $(LP)vec
.ENDIF

EXELIBS         = $(LL)ztimer$(LE)
LIBCLEAN        = *.lib *.a

# Set up the type of vectors and matrices being created if any

OBJECTS         = vec3d$O xform2d$O xform3d$O
FIXASMOBJS      = fxatrig$O fxlog$O fxmat2d$O fxmat3d$O fxmisc$O    \
                  fxpow$O fxsqrt$O fxtrig$O fxmap2d$O fxmap3d$O     \
				  tables$O fxzdiv$O
FLASMOBJS       = float$O fllog$O flsqrt$O fltrig$O

# Set up to use 80387 assembler routines if specified

.IF $(FPU)
LIBEND          = 7
.ENDIF

.IF $(NOINLINEASM)
CFLAGS			+= -DNOINLINEASM
.ENDIF

.IF $(FX_DOUBLE)   # Type of real numbers to use
CFLAGS          += -DFX_DOUBLE
ASFLAGS         += -dFX_DOUBLE
LIBFILE         = $(LIBSTART)_db$(LIBEND)$L
OBJECTS         += $(FLASMOBJS)
.ELIF $(FX_FIXED)
CFLAGS          += -DFX_FIXED -DFIXED386
LIBFILE         = $(LIBSTART)_fix$L
OBJECTS         += fixed$O $(FIXASMOBJS)
.ELSE
CFLAGS          += -DFX_FLOAT
ASFLAGS         += -dFX_FLOAT
LIBFILE         = $(LIBSTART)_fl$(LIBEND)$L
OBJECTS         += $(FLASMOBJS)
.ENDIF

# Define the list of object files to create dependency information for

DEPEND_OBJ      = $(OBJECTS) fixedtst$O fixed$O $(FIXASMOBJS) $(FLASMOBJS)  \
                  print$O fltst$O

.IF $(UNIX)
.INCLUDE: "$(SCITECH)/makedefs/common.mk"
.ELSE
.INCLUDE: "$(SCITECH)\makedefs\common.mk"
.ENDIF

tests:  vectst$E mattst$E

fixedtst$E: fixedtst$O fixed$O $(FIXASMOBJS)
fltst$E: fltst$O $(FLASMOBJS)
vectst$E: vectst$O $(LIBFILE)
mattst$E: mattst$O print$O $(LIBFILE)

# Target to build all DOS 32-bit libraries

.PHONY mkdos32:
	@build wc10-d32 install -u OPT=1 NO_PCH=1
	@build wc11-d32 install -u OPT=1 NO_PCH=1
	@build bc45-d32 install -u OPT=1
	@build bc50-d32 install -u OPT=1
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

.IF $(UNIX)
mkall: mkunix
.ELSE
.PHONY mkall: mkdos32 mkwin32 cleanexe
.ENDIF

