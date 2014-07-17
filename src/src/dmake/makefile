# Default makefile for the various versions of dmake that we
# have available.  This is a bootstrap version and uses /bin/sh to
# execute a script which compiles dmake.
#
# Note the DOS commands actually invoke command.com to run the .bat file
# to make the script.

all :
	@-echo ""; clear; exit 0
	@echo "INDEX:  You must specify 'make target' where target is one of:"
	@echo "-------------"
	@echo "   make BeOS          - BeOS using GCC compiler"
	@echo "   make bsd43         - Generic BSD 4.3 System"
	@echo "   make bsd43uw       - Generic BSD 4.3 at U of Waterloo"
	@echo "   make bsd43vf       - Generic BSD 4.3 that needs vfprintf"
	@echo "   make sysvr4        - Generic SysV R4 UNIX System"
	@echo "   make sysvr3        - Generic SysV R3 UNIX System"
	@echo "   make sysvr3pwd     - Generic SysV R3 UNIX System, our PWD"
	@echo "   make sysvr1        - Generic SysV R1 UNIX System"
	@echo "   make dynix         - Sequent DYNIX System"
	@echo "   make freebsd       - FreeBSD System"
	@echo "   make linux         - Linux using GCC compiler"
	@echo "   make ultrix        - Ultrix 3.0 System"
	@echo "   make mips          - Any MIPS System"
	@echo "   make irix          - Any IRIX System"
	@echo "   make osf1          - OSF/1 UNIX System"
	@echo "   make coherent40    - Coherent Version 4.0"
	@echo "   make coherent42    - Coherent Version 4.2 or greater"
	@echo "   make hpux          - HP Unix"
	@echo "   make dgux          - Data General UNIX"
	@echo "   make 386ix         - 386/ix (SysV R3) System"
	@echo "   make xenix         - 386 Xenix System"
	@echo "   make xenixpwd      - 386 Xenix System, our PWD"
	@echo "   make aix           - IBM RS6000/AIX System"
	@echo "   make Solaris       - SUN Solaris 1.0 to 2.0"
	@echo "   make Solaris2.1    - SUN Solaris 2.1 or greater"
	@echo "   make gccSolaris2.1 - SUN Solaris 2.1 or greater with gcc"
	@echo "   make qnxwcc        - QNX 4.2 or later with Watcom-C"
	@echo "   make os2-ibm       - OS/2 using IBM ICC compiler"
	@echo "   make tos           - Atari-ST TOS using GCC as compiler"

	@echo "   make tcc20swp      - Turbo-C 2.0 Swapping DOS Version"
	@echo "   make bcc30         - Borland C++ 3.0 DOS Version"
	@echo "   make bcc30swp      - Borland C++ 3.0 Swapping DOS Version"
	@echo "   make bcc40swp      - Borland C++ 4.0 Swapping DOS Version"
	@echo "   make bcc45swp      - Borland C++ 4.5 Swapping DOS Version"
	@echo "   make bcc50swp      - Borland C++ 5.0 Swapping DOS Version"
	@echo "   make msc51         - Microsoft-C 5.1 DOS Verson"
	@echo "   make msc51swp      - Microsoft-C 5.1 Swapping DOS Version"
	@echo "   make msc60         - Microsoft-C 6.0 DOS Verson"
	@echo "   make msc60swp      - Microsoft-C 6.0 Swapping DOS Version"

	@echo "   make win95-bcc50   - Borland C++ 5.0 32-bit build for Win-95"
	@echo "   make win95-vpp40   - Microsoft VC++ 4.0 32-bit build for Win-95"

	@echo "   make winnt-bcc50   - Borland C++ 5.0 32-bit build for Win-nt"
	@echo "   make winnt-vpp40   - Microsoft VC++ 4.0 32-bit build for Win-nt"

	@echo "   make mpwmac        - Macintosh under MPW"

ultrix aix      : sysvr3;
Solaris         : bsd43;
hpux	        : sysvr3pwd;
irix osf1 dgux  : sysvr4;

sysvr1 sysvr3 sysvr4 bsd43 386ix :; /bin/sh -x < unix/$@/make.sh

BeOS             :; /bin/sh -x < beos/gnu/make.sh
bsd43uw 	     :; /bin/sh -x < unix/bsd43/uw/make.sh
bsd43vf dynix mips   :; /bin/sh -x < unix/bsd43/vf/make.sh
coherent40 	     :; /bin/sh -x < unix/coherent/ver40/make.sh
coherent42 	     :; /bin/sh -x < unix/coherent/ver42/make.sh
xenix 		     :; /bin/sh -x < unix/xenix/make.sh
xenixpwd 	     :; /bin/sh -x < unix/xenix/pwd/make.sh
sysvr3pwd 	     :; /bin/sh -x < unix/sysvr3/pwd/make.sh
linux	 	     :; /bin/sh -x < unix/linux/gnu/make.sh
freebsd	 	     :; /bin/sh -x < unix/freebsd/gnu/make.sh
Solaris2.1	     :; /bin/sh -x < unix/solaris/make.sh
gccSolaris2.1	     :; /bin/sh -x < unix/solaris/gnu/make.sh
tos                  :; sh -x tos/make.sh
mpwmac               :; :mac:make.sh

# Greg Yahchuks make ... sigh
qnxwcc :
	/bin/sh -x < qssl/make.sh

# Various OS/2 targets.
OS2_VER = os2-ibm
$(OS2_VER) :
	cmd.exe /c "make.cmd $@"

# DOS with some form of make and sh
# Note if you do not have a 'make and/or sh' program under MSDOS then
# typing 'make' in the dmake distribution directory will invoke the make.bat
# batch file which will issue the appropriate instructions to do a build.
DOS_VER = tccswp     bcc30        bcc30swp         bcc40swp \
          bcc45swp   bcc50swp     msc51            msc51swp \
	  msc60      msc60swp

# Win-95 with some form of make and sh
# Note if you do not have a 'make and/or sh' program under Win-95 then
# typing 'make' in the dmake distribution directory will invoke the make.bat
# batch file which will issue the appropriate instructions to do a build.
WIN95_VER = win95-bcc50 win95-vpp40

$(DOS_VER) $(WIN95_VER) :; command.com /c "make.bat $@"

# Win-nt with some form of make and sh
# Note if you do not have a 'make and/or sh' program under Win-95 then
# typing 'make' in the dmake distribution directory will invoke the make.bat
# batch file which will issue the appropriate instructions to do a build.
WINNT_VER = winnt-bcc50 winnt-vpp40

$(WINNT_VER) :; cmd.exe /c "make.bat $@"
