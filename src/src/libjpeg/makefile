#############################################################################
#
#                   Copyright (C) 1996 SciTech Software
#                           All rights reserved.
#
# Descripton:   Generic makefile for the MGL version of the JPEG image
#               library.
#
#############################################################################

#----------------------------------------------------------------------------
# Define the lists of object files
#----------------------------------------------------------------------------

# Objects for building Binary Portable DLL
DLL_OBJS		= dllstart$O jpeginit$O

# library object files common to compression and decompression
COMOBJECTS      = jcomapi$O jutils$O jerror$O jmemmgr$O jmemmgl$O

# compression library object files
CLIBOBJECTS		= jcapimin$O jcapistd$O jctrans$O jcparam$O jdatadst$O 		\
				  jcinit$O jcmaster$O jcmarker$O jcmainct$O jcprepct$O 		\
				  jccoefct$O jccolor$O jcsample$O jchuff$O jcphuff$O 		\
				  jcdctmgr$O jfdctfst$O jfdctflt$O jfdctint$O

# decompression library object files
DLIBOBJECTS		= jdapimin$O jdapistd$O jdtrans$O jdatasrc$O 				\
				  jdmaster$O jdinput$O jdmarker$O jdhuff$O jdphuff$O 		\
				  jdmainct$O jdcoefct$O jdpostct$O jddctmgr$O jidctfst$O 	\
				  jidctflt$O jidctint$O jidctred$O jdsample$O jdcolor$O		\
				  jquant1$O jquant2$O jdmerge$O

# These objectfiles are included in libjpeg.lib
OBJECTS			= $(CLIBOBJECTS) $(DLIBOBJECTS) $(COMOBJECTS)

.IF $(BUILD_DLL)
CFLAGS      	+= -I$(PRIVATE)\include\drvlib -I$(SCITECH)\include\drvlib -D__DRIVER__
ASFLAGS   		+= -d__DRIVER__
EXELIBS			= drvlib$L
.ELSE
LIBFILE         = $(LP)jpeg$L
LIBCLEAN  		= *.dll *.lib *.a
.ENDIF

#----------------------------------------------------------------------------
# Target to build the Binary Portable DLL target
#----------------------------------------------------------------------------

jpeg.dll: $(DLL_OBJS) $(OBJECTS)

#----------------------------------------------------------------------------
# Target to build all Intel binary drivers
#----------------------------------------------------------------------------

.PHONY mksdd:
	@build wc11-w32 jpeg.dll -u BUILD_DLL=1 NO_RUNTIME=1 OPT=1
	@k_cp jpeg.dll $(SCITECH)\drivers\jpeg.bpd
	@dmake cleanexe

.PHONY db:
	@build wc11-w32 jpeg.dll BUILD_DLL=1 NO_RUNTIME=1 OPT=1
	@k_cp jpeg.dll $(SCITECH)\drivers\jpeg.bpd

#----------------------------------------------------------------------------
# Define the list of object files to create dependency information for
#----------------------------------------------------------------------------

DEPEND_OBJ      = $(OBJECTS)

.INCLUDE: "$(SCITECH)/makedefs/common.mk"
