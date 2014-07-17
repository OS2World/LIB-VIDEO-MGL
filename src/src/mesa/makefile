#############################################################################
#
#                Copyright (C) 1996-2000 SciTech Software
#                           All rights reserved.
#
# Descripton:   Generic makefile for the SciTech Nucleus version of Mesa.
#
#############################################################################

#----------------------------------------------------------------------------
# Extra C flags we pass to the compiler
#----------------------------------------------------------------------------

# Turned off pre compiled headers for the moment to get this version to compile
CFLAGS          += -DBUILD_MESA -DUSE_X86_ASM -DUSE_MMX_ASM -DFAST_MATH -DUSE_3DNOW_ASM

#-DPC_HEADER

#----------------------------------------------------------------------------
# Define the list of object files
#----------------------------------------------------------------------------

DLL_OBJS	= dllstart$O

CORE_OBJS   = accum$O alpha$O alphabuf$O attrib$O bbox$O bitmap$O blend$O \
			  clip$O config$O colortab$O context$O copypix$O cva$O \
			  debug_xform$O depth$O dispatch$O dlist$O drawpix$O \
			  enable$O enums$O eval$O extensions$O feedback$O fog$O get$O \
			  glapi$O glapinoop$O glmisc$O hash$O highpc$O image$O imaging$O \
			  light$O lines$O logic$O lowpc$O masking$O matrix$O mmath$O mthreads$O \
			  pb$O pipeline$O pixel$O points$O polygon$O quads$O rastpos$O \
			  readpix$O rect$O scissor$O shade$O span$O stages$O stencil$O \
			  teximage$O texobj$O texstate$O texture$O translate$O triangle$O \
			  varray$O vb$O vbcull$O vbfill$O vbindirect$O vbrender$O \
              vbxform$O vector$O vertices$O winpos$O xform$O zoom$O

DRIVER_OBJS = glinit$O nmesa$O nmesa8$O nmesa16$O nmesa24$O nmesa32$O halftone$O

ASM_OBJS    = 3dnow_norm_raw$O 3dnow_xform_masked1$O 3dnow_xform_masked2$O \
			  3dnow_xform_masked3$O 3dnow_xform_masked4$O 3dnow_xform_raw1$O \
			  3dnow_xform_raw2$O 3dnow_xform_raw3$O 3dnow_xform_raw4$O \
			  common_x86asm$O mmx_blend$O vertex$O vertex_3dnow$O x86a$O

AMD_MMX_OBJS = 3dnow$O x86$O common_x86$O

OBJECTS     = $(DRIVER_OBJS) $(CORE_OBJS) $(AMD_MMX_OBJS) $(ASM_OBJS)

.IF $(BUILD_DLL)
CFLAGS      	+= -I$(PRIVATE)\include\drvlib -I$(SCITECH)\include\drvlib -D__DRIVER__
ASFLAGS   		+= -d__DRIVER__
EXELIBS			= drvlib$L
.ELSE
LIBFILE         = $(LP)mesagl$L
LIBCLEAN  		= *.dll *.lib *.a
.ENDIF

#----------------------------------------------------------------------------
# Target to build the Binary Portable DLL target
#----------------------------------------------------------------------------

gl.dll: $(DLL_OBJS) $(OBJECTS)

#----------------------------------------------------------------------------
# Target to build all Intel binary drivers
#----------------------------------------------------------------------------

.PHONY mkbpd:
	@build wc11-w32 gl.dll -u BUILD_DLL=1 NO_RUNTIME=1 OPT=1 USE_NASM=1
	@k_cp gl.dll $(SCITECH)\drivers\gl.bpd
	@dmake cleanexe

.PHONY db:
	@build wc11-w32 gl.dll BUILD_DLL=1 NO_RUNTIME=1 OPT=1 USE_NASM=1
	@k_cp gl.dll $(SCITECH)\drivers\gl.bpd

.PHONY db_dbg:
	@build wc11-w32 gl.dll BUILD_DLL=1 NO_RUNTIME=1 DBG=1 USE_NASM=1
	@k_cp gl.dll $(SCITECH)\drivers\gl.bpd

#----------------------------------------------------------------------------
# Define the list of object files to create dependency information for
#----------------------------------------------------------------------------

DEPEND_OBJ      = $(DLL_OBJS) $(OBJECTS) $(ASM_OBJS) $(AMD_MMX_OBJS)
DEPEND_SRC      = gl3d;x86;$(PRIVATE)/src/common
.SOURCE:          gl3d x86 $(PRIVATE)/src/common

.INCLUDE: "$(SCITECH)/makedefs/common.mk"
