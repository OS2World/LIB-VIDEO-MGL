# Define MPW MAC specific macros.
# Assumes MPW C, change as needed.
#

A            *:= .lib
S	     *:= .s
V            *:= v
TMPDIR       *:= $(MPW)/tmp

# import library definitions
.IMPORT .IGNORE : CLibraries Libraries

# Set arguments for the SHELL.  Since we can't execute sub-processes,
# these variables are not important, except for some makefiles that check
# for some values to determine the platform.
SHELL       *:= mpw
SHELLFLAGS  *:=
GROUPFLAGS  *:=
SHELLMETAS  *:=

# Define toolkit macros
CC          *:= c
AS          *:= asm
LD          *:= link
AR          *:=
ARFLAGS     *:=
RM          *:= delete
RMFLAGS     *:= 
MV          *:= rename
YTAB        *:= ytab
LEXYY       *:= lex_yy

LDLIBS      *=  "$(CLibraries)CSANELib.o" "$(CLibraries)Math.o" \
                "$(CLibraries)StdCLib.o" "$(Libraries)Runtime.o" \
                "$(Libraries)Interface.o" "$(Libraries)Toollibs.o"

# Disable the print command
PRINT       *=

# Make certain to disable defining how to make executables.
__.EXECS !:=
