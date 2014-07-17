#############################################################################
#
#                   Copyright (C) 1998 SciTech Software.
#                           All rights reserved.
#
# Descripton:   Global makefile to build SciTech MGL sample programs.
#
#############################################################################

# defines for where the demo programs end up

MGLDOS_DIR  = c:\scitech\demos\mgldos
MGLWIN_DIR  = c:\scitech\demos\mglwin

# Targets to build DOS sample programs

.PHONY mkdos:
	@cdit demo dmake cleanexe
	@cdit demo build wc11-d32 -u OPT=1 USE_CAUSEWAY=1
	@k_cp demo\demo.exe $(MGLDOS_DIR)
	@cdit demo3d dmake cleanexe
	@cdit demo3d build wc11-d32 -u OPT=1 USE_CAUSEWAY=1
	@k_cp demo3d\demo3d.exe $(MGLDOS_DIR)
	@cdit foxbear dmake cleanexe
	@cdit foxbear build wc11-d32 -u OPT=1 USE_CAUSEWAY=1
	@k_cp foxbear\foxbear.exe $(MGLDOS_DIR)
	@cdit smacker dmake cleanexe
	@cdit smacker build wc11-d32 -u OPT=1 USE_CAUSEWAY=1
	@k_cp smacker\play.exe $(MGLDOS_DIR)
	@cdit mgldog dmake cleanexe
	@cdit mgldog build wc11-d32 -u OPT=1 USE_CAUSEWAY=1
	@k_cp mgldog\mgldog.exe $(MGLDOS_DIR)
	@cdit showbmp dmake cleanexe
	@cdit showbmp build wc11-d32 -u OPT=1 USE_CAUSEWAY=1
	@k_cp showbmp\showbmp.exe $(MGLDOS_DIR)
	@cdit samples dmake cleanexe
	@cdit samples build wc11-d32 -u OPT=1 USE_CAUSEWAY=1
	@k_cp samples\*.exe $(MGLDOS_DIR)
	@cdit gm dmake cleanexe
	@cdit gm build wc11-d32 -u OPT=1 USE_CAUSEWAY=1
	@k_cp gm\*.exe $(MGLDOS_DIR)

# Targets to build Windows sample programs

.PHONY mkwin:
	@cdit demo dmake cleanexe
	@cdit demo build bc50-w32 -u OPT=1
	@k_cp demo\demo.exe $(MGLWIN_DIR)
	@cdit demo3d dmake cleanexe
	@cdit demo3d build bc50-w32 -u OPT=1
	@k_cp demo3d\demo3d.exe $(MGLWIN_DIR)
	@cdit foxbear dmake cleanexe
	@cdit foxbear build bc50-w32 -u OPT=1
	@k_cp foxbear\foxbear.exe $(MGLWIN_DIR)
	@cdit smacker dmake cleanexe
	@cdit smacker build bc50-w32 -u OPT=1
	@k_cp smacker\play.exe $(MGLWIN_DIR)
	@cdit mgldog dmake cleanexe
	@cdit mgldog build bc50-w32 -u OPT=1
	@k_cp mgldog\mgldog.exe $(MGLWIN_DIR)
	@cdit showbmp dmake cleanexe
	@cdit showbmp build bc50-w32 -u OPT=1
	@k_cp showbmp\showbmp.exe $(MGLWIN_DIR)
	@cdit wmgldog dmake cleanexe
	@cdit wmgldog build bc50-w32 -u OPT=1
	@k_cp wmgldog\wmgldog.exe $(MGLWIN_DIR)
	@cdit wshowbmp dmake cleanexe
	@cdit wshowbmp build bc50-w32 -u OPT=1
	@k_cp wshowbmp\wshowbmp.exe $(MGLWIN_DIR)
	@cdit samples dmake cleanexe
	@cdit samples build bc50-w32 -u OPT=1
	@k_cp samples\*.exe $(MGLWIN_DIR)
	@cdit gm dmake cleanexe
	@cdit gm build bc50-w32 -u OPT=1
	@k_cp gm\*.exe $(MGLWIN_DIR)
