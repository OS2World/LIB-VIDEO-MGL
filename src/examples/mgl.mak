############################################################################
#
#                   Copyright (C) 1995 SciTech Software.
#                           All rights reserved.
#
# Descripton:   Common makefile definitions for the MGL programs.
#               Requires the SciTech Software makefile definitions package
#               to be installed.
#
#############################################################################

# DOS Extender dependant flags

CFLAGS          += $(DX_CFLAGS)

# Flag to set privity level for executables under QNX.

.IF $(USE_QNX4)
LDFLAGS		+= -T1
.ENDIF

# Include definitions for the target system

.IMPORT .IGNORE: MGL_DLL USE_DWSTK ISV_LICENSE TEST_HARNESS DEBUG_MGL
.IMPORT .IGNORE: DEBUG_JPEG DEBUG_PNG DEBUG_FREETYPE DEBUG_OPENGL

# Compile with ISV license if specified

.IF $(ISV_LICENSE)
CFLAGS		+= -DISV_LICENSE
.ENDIF

# Include definitions for the target system

.IF $(USE_WIN32)
.IF $(MGL_DLL)

# Link with Windows dynamic link libraries

CFLAGS          += -DMGL_DLL
BASELIBS    	+= $(LL)mgli$(LE)

.ELSE

# Link with Windows static link libraries

.IF $(STKCALL)
BASELIBS        += $(LL)smgl$(LE)
.ELSE
BASELIBS       	+= $(LL)mgl$(LE)
.ENDIF

.ENDIF

.ELIF $(USE_QNX)

# Link with QNX versions of MGL

BASELIBS		= $(LL)mgl$(LE)

.ELIF $(USE_LINUX)

# Link with Linux static link libraries

.IF $(USE_X11)
BASELIBS       	+= $(LL)mglx11$(LE)
BASELIBS     	+= -L/usr/X11R6/lib -lX11 -lXext
.ELSE
BASELIBS       	+= $(LL)mgl$(LE)
.ENDIF

.ELIF $(USE_OS232)
.IF $(USE_OS2GUI)

# Link with OS/2 Presentation Manager libraries

.IF $(STKCALL)
BASELIBS        += $(LL)smglpm$(LE)
.ELSE
BASELIBS       	+= $(LL)mglpm$(LE)
.ENDIF

.ELSE

# Link with OS/2 fullscreen console libraries

.IF $(STKCALL)
BASELIBS        += $(LL)smgl$(LE)
.ELSE
BASELIBS       	+= $(LL)mgl$(LE)
.ENDIF

.ENDIF
.ELSE

# MS-DOS based libraries

.IF $(STKCALL)
BASELIBS        += $(LL)smgl$(LE)
.ELSE
BASELIBS        += $(LL)mgl$(LE)
.ENDIF

.ENDIF

# Link with static libraries rather than BPD's in DEBUG mode

.IF $(DEBUG)
BASELIBS		+= $(LL)jpeg$(LE) $(LL)png$(LE) $(LL)freetype$(LE) $(LL)mesagl$(LE)
.ENDIF

# Compile in stubs to BPD libraries when not building debug libraries

.IF $(DEBUG_JPEG)
BASELIBS 		+= $(LL)jpeg$(LE)
.ENDIF

.IF $(DEBUG_PNG)
BASELIBS 		+= $(LL)png$(LE)
.ENDIF

.IF $(DEBUG_FREETYPE)
BASELIBS 		+= $(LL)freetype$(LE)
.ENDIF

.IF $(DEBUG_OPENGL)
CFLAGS			+= -DDEBUG_OPENGL -DNO_GL_MACROS -DNO_GLU_MACROS -DNO_MGL_IMPORTS
BASELIBS 		+= $(LL)mesagl$(LE) $(LL)mesaglu$(LE)
.ENDIF

# Link with Nucleus test harness if desired

.IF $(TEST_HARNESS)
BASELIBS 		+= $(LL)n_ref2d$(LE) $(LL)n_driver$(LE)
.ENDIF
