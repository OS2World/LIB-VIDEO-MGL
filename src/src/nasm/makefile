#############################################################################
#
#                   Copyright (C) 1998 SciTech Software.
#                           All rights reserved.
#
# Descripton:   Generic makefile for the Netwide assembler. Requires the
#				SciTech Makefile utitilies. Should work with any compiler
#				supported by the makefile utilities.
#
#############################################################################

# Add DOS extender dependant flags to command line

CFLAGS      += $(DX_CFLAGS) -DTASM_COMPAT
ASFLAGS     += $(DX_ASFLAGS)
NO_PMLIB	:= 1

.IF $(UNIX)
# Linux systems don't provide the stricmp function
CFLAGS	+= -Dstricmp=nasm_stricmp
.ENDIF

# Define the object files for each of the programs

NASM_OBJ 	= nasm$O nasmlib$O float$O insnsa$O assemble$O labels$O 	\
			  parser$O outform$O outbin$O outaout$O outcoff$O outelf$O 	\
              outobj$O outas86$O outrdf$O outrdf2$O outdbg$O preproc$O  \
              listing$O eval$O zoutieee$O

NDISASM_OBJ = ndisasm$O disasm$O sync$O nasmlib$O insnsd$O

# Targets to build the files

all:	nasm$E ndisasm$E

nasm$E: $(NASM_OBJ)
ndisasm$E: $(NDISASM_OBJ)

# Define the list of object files to create dependency information for

DEPEND_OBJ      = $(NASM_OBJ) $(NDISASM_OBJ)

.INCLUDE: "$(SCITECH)/makedefs/common.mk"
# DO NOT DELETE
