#############################################################################
#
#					SciTech Multi-platform Graphics Library
#
#  ========================================================================
#
#    The contents of this file are subject to the SciTech MGL Public
#    License Version 1.0 (the "License"); you may not use this file
#    except in compliance with the License. You may obtain a copy of
#    the License at http://www.scitechsoft.com/mgl-license.txt
#
#    Software distributed under the License is distributed on an
#    "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
#    implied. See the License for the specific language governing
#    rights and limitations under the License.
#
#    The Original Code is Copyright (C) 1991-1998 SciTech Software, Inc.
#
#    The Initial Developer of the Original Code is SciTech Software, Inc.
#    All Rights Reserved.
#
#  ========================================================================
#
# Descripton:   Rules makefile definitions, which define the rules used to
#				build targets. We include them here at the end of the
#				makefile so the generic project makefiles can override
#				certain things with macros (such as linking C++ programs
#				differently).
#
#############################################################################

# Take out PMLIB if we don't need to link with it

.IF $(NO_PMLIB)
PMLIB :=
.ENDIF

.IF $(USE_CXX_LINKER)
LD	:= $(LDXX)
.ENDIF

# Implicit generation rules for making object files from source files
%$O: %.c ; @k_cc $(CC) @$(mktmp $(CFLAGS) -c) $<
%$O: %$P ; @k_cc $(CXX) @$(mktmp $(CFLAGS) -c) $<
%$O: %$A ; $(AS) @$(mktmp -o $@ $(ASFLAGS)) $<

# Implicit rule for building a library file
.IF $(DYNAMIC_LIBS)
%$L:     ; @k_cc $(LIB) @$(mktmp $(LIBFLAGS) -Wl,-soname,$(shell basename $@ $(VERSION))$(VERSIONMAJ) -o $@ $& $(LIBS))
.ELSE
%$L:     ; @k_cc $(LIB) @$(mktmp $(LIBFLAGS) $@ $&)
.ENDIF

# Implicit rule for building an executable file
%$E:     ; @k_cc $(LD) @$(mktmp $(LDFLAGS) -o $@ $& $(EXELIBS) $(PMLIB) -lm)

