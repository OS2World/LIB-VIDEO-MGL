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

# Implicit generation rules for making object files
.IF $(WC_LIBBASE) == WC10A
%$O: %.c ; $(CC) $(CFLAGS) $(<:s,/,\)
%$O: %$P ; $(CP) $(CFLAGS) $(<:s,/,\)
.ELSE
%$O: %.c ; $(CC) @$(mktmp $(CFLAGS:s/\/\\)) $(<:s,/,\)
%$O: %$P ; $(CP) @$(mktmp $(CFLAGS:s/\/\\)) $(<:s,/,\)
.ENDIF
.IF $(USE_NASM)
%$O: %$A ; $(AS) @$(mktmp -o $@ $(ASFLAGS:s/\/\\)) $(<:s,/,\)
.ELSE
%$O: %$A ; $(AS) @$(mktmp $(ASFLAGS:s/\/\\)) $(<:s,/,\)
.ENDIF

# Implit rule to compile .S assyntax.h assembler files with NASM

%$O: %$S ;
	@gcpp -DNASM_ASSEMBLER -D__WATCOMC__ -EP $(<:s,/,\) > $(*:s,/,\).asm
	nasm @$(mktmp -f obj -o $@) $(*:s,/,\).asm
	@k_rm -S $(mktmp $(*:s,/,\).asm)

# Special target to build dllstart.asm using Borland TASM
dllstart.obj: dllstart.asm
.IF $(DLL_START_TASM)
    tasmx @$(mktmp /t /mx /m /D__FLAT__ /i$(SCITECH)\INCLUDE /q) $(PRIVATE)\src\common\dllstart.asm
.ELSE
	tasm32 @$(mktmp /t /mx /m /D__FLAT__ /i$(SCITECH)\INCLUDE /q) $(PRIVATE)\src\common\dllstart.asm
.ENDIF

# Implicit rule for building resource files
%$R: %.rc ; $(RC) $(RCFLAGS) -r $<

# Implicit rule for building a DLL using a response file
.IF $(IMPORT_DLL)
.ELSE
.IF $(USE_OS232)
%$D: ;
	@trimlib $(mktmp $(LDFLAGS) OP quiet SYS os2v2 dll\nN $@\nF $(&:t",\n":s/\/\\)\nLIBR $(EXELIBS:t",")) $*.lnk
	rclink $(LD) $(RC) $@ $*.lnk
	@k_rm -S $(mktmp *.lnk)
.ELIF $(USE_WIN32)
.IF $(NO_RUNTIME)
%$D: ;
	@trimlib $(mktmp $(LDFLAGS) OP nodefaultlibs OP quiet SYS nt_dll\nN $@\nF $(&:t",\n":s/\/\\)\nLIBR $(EXELIBS:t",")) $*.lnk
	rclink $(LD) $(RC) $@ $*.lnk
	@k_rm -S $(mktmp *.lnk)
.ELSE
%$D: ;
	@trimlib $(mktmp $(LDFLAGS) OP quiet SYS nt_dll\nN $@\nF $(&:t",\n":s/\/\\)\nLIBR $(EXELIBS:t",")) $*.lnk
	rclink $(LD) $(RC) $@ $*.lnk
	@k_rm -S $(mktmp *.lnk)
.ENDIF
.ELSE
%$D: ;
	@trimlib $(mktmp $(LDFLAGS) OP quiet SYS win386\nN $*.rex\nF $(&:t",\n":s/\/\\)\nLIBR $(EXELIBS:t",")) $*.lnk
	rclink $(LD) $(RC) $@ $*.lnk
	wbind $* -d -q -n
	@k_rm -S $(mktmp *.lnk)
.ENDIF
.ENDIF

# Implicit rule for building a library file using response file
.IF $(BUILD_DLL)
%$L: ; $(ILIB) $(ILIBFLAGS) $@ +$?
.ELIF $(IMPORT_DLL)
%$L: ; $(ILIB) $(ILIBFLAGS) $@ +$?
.ELSE
%$L: ; $(LIB) $(LIBFLAGS) $@ @$(mktmp,$*.rsp -+$(?:t"\n-+":s/\/\\)\n)
.ENDIF

# Implicit rule for building an executable file using response file
.IF $(USE_X32)
%$E: ;
	@trimlib $(mktmp OP quiet\nN $@\nF $(&:t",":s/\/\\)\nLIBR $(PMLIB)$(EXELIBS:t",")) $*.lnk
	$(LD) $(LDFLAGS) @$*.lnk
	x32fix $@
	@k_rm -S $(mktmp *.lnk)
.ELIF $(USE_OS232)
.IF $(USE_OS2GUI)
%$E: ;
	@trimlib $(mktmp $(LDFLAGS) OP quiet SYS os2v2_pm\nN $@\nF $(&:t",":s/\/\\)\nLIBR $(PMLIB)$(EXELIBS:t",")) $*.lnk
	rclink $(LD) $(RC) $@ $*.lnk
	@k_rm -S $(mktmp *.lnk)
.IF $(LXLITE)
 	lxlite $@
.ENDIF
.ELSE
%$E: ;
	@trimlib $(mktmp $(LDFLAGS) OP quiet SYS os2v2\nN $@\nF $(&:t",":s/\/\\)\nLIBR $(PMLIB)$(EXELIBS:t",")) $*.lnk
	rclink $(LD) $(RC) $@ $*.lnk
	@k_rm -S $(mktmp *.lnk)
.IF $(LXLITE)
 	lxlite $@
.ENDIF
.ENDIF
.ELIF $(USE_WIN32)
.IF $(WIN32_GUI)
%$E: ;
	@trimlib $(mktmp $(LDFLAGS) OP quiet SYS win95\nN $@\nF $(&:t",":s/\/\\)\nLIBR $(PMLIB)$(EXELIBS:t",")) $*.lnk
	rclink $(LD) $(RC) $@ $*.lnk
	@k_rm -S $(mktmp *.lnk)
.ELSE
%$E: ;
	@trimlib $(mktmp $(LDFLAGS) OP quiet SYS nt\nN $@\nF $(&:t",":s/\/\\)\nLIBR $(PMLIB)$(EXELIBS:t",")) $*.lnk
	rclink $(LD) $(RC) $@ $*.lnk
	@k_rm -S $(mktmp *.lnk)
.ENDIF
.ELIF $(USE_WIN386)
%$E: ;
	@trimlib $(mktmp $(LDFLAGS) OP quiet SYS win386\nN $*.rex\nF $(&:t",":s/\/\\)\nLIBR $(EXELIBS:t",")) $*.lnk
	rclink $(LD) wbind $*.rex $*.lnk
	@k_rm -S $(mktmp *.lnk)
.ELIF $(USE_TNT)
%$E: ;
	@trimlib $(mktmp OP quiet\nN $@\nF $(&:t",":s/\/\\)\nLIBR dosx32.lib,tntapi.lib,$(PMLIB)$(EXELIBS:t",")) $*.lnk
	$(LD) $(LDFLAGS) @$*.lnk
	@k_rm -S $(mktmp *.lnk)
.IF $(DOSSTYLE)
	@markphar $@
.ENDIF
.ELSE
%$E: ;
	@trimlib $(mktmp OP quiet\nN $@\nF $(&:t",":s/\/\\)\nLIBR $(PMLIB)$(EXELIBS:t",")) $*.lnk
	$(LD) $(LDFLAGS) @$*.lnk
	@k_rm -S $(mktmp *.lnk)
.ENDIF

# TODO: Change batch files to use a special Win32 utility to speed this up!!
