############################################################################
#
#                   Copyright (C) 1995 SciTech Software.
#                           All rights reserved.
#
# Descripton:   Common makefile definitions for the MegaVision samples.
#               Requires the SciTech Software makefile definitions package
#               to be installed.
#
#############################################################################

# Link with common MegaVision C++ libraries

.IF $(STKCALL)
BASELIBS         += $(LL)smvis$(LE) $(LL)smglcpp$(LE) $(LL)stech$(LE)
.ELSE
BASELIBS         += $(LL)mvis$(LE) $(LL)mglcpp$(LE) $(LL)tech$(LE)
.ENDIF

# Link with standard MGL libraries

.INCLUDE: "../../mgl.mak"
