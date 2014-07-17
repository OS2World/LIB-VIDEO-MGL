#############################################################################
#
#                   Copyright (C) 1998 SciTech Software.
#                           All rights reserved.
#
# Descripton:   Global makefile to build SciTech MGL OpenGL sample programs.
#
#############################################################################

# defines for where the atlantis programs end up

OGLDOS_DIR  = c:\scitech\demos\ogldos
OGLWIN_DIR  = c:\scitech\demos\oglwin

# Targets to build DOS sample programs

.PHONY mkdos:
	@cdit atlantis dmake cleanexe
	@cdit atlantis build wc11-d32 -u OPT=1 USE_PMODEW=1
	@k_cp atlantis\atlantis.exe $(OGLDOS_DIR)
	@cdit demos dmake cleanexe
	@cdit demos build wc11-d32 -u OPT=1 USE_PMODEW=1
	@k_cp demos\*.exe $(OGLDOS_DIR)
	@cdit ideas dmake cleanexe
	@cdit ideas build wc11-d32 -u OPT=1 USE_PMODEW=1
	@k_cp ideas\ideas.exe $(OGLDOS_DIR)

# Targets to build Windows sample programs

.PHONY mkwin:
	@cdit atlantis dmake cleanexe
	@cdit atlantis build bc50-w32 -u OPT=1
	@k_cp atlantis\atlantis.exe $(OGLWIN_DIR)
	@cdit demos dmake cleanexe
	@cdit demos build bc50-w32 -u OPT=1
	@k_cp demos\*.exe $(OGLWIN_DIR)
	@cdit ideas dmake cleanexe
	@cdit ideas build bc50-w32 -u OPT=1
	@k_cp ideas\ideas.exe $(OGLWIN_DIR)
	@cdit skyfly dmake cleanexe
	@cdit skyfly build bc50-w32 -u OPT=1
	@k_cp skyfly\skyfly.exe $(OGLWIN_DIR)
