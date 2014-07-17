@echo off
REM: Set the place where SciTech Software is installed, and where each
REM: of the supported compilers is installed. These environment variables
REM: are used by the batch files in the SCITECH\BIN directory.
REM:
REM: Modify the as appropriate for your compiler configuration (you should
REM: only need to change things in this batch file).
REM:
REM: This version is for a normal MSDOS installation.

REM: The SCITECH variable points to where batch files, makefile startups,
REM: include files and source files will be found when compiling.

SET SCITECH=C:\SCITECH

REM: The SCITECH_LIB variable points to where the SciTech libraries live
REM: for installation and linking. This allows you to have the source and
REM: include files on local machines for compiling and have the libraries
REM: located on a common network machine (for network builds).

SET SCITECH_LIB=C:\SCITECH

REM: The PRIVATE variable points to where private source files reside that
REM: do not live in the public source tree

SET PRIVATE=C:\PRIVATE

REM: The following sets up the path to the SciTech command line utilities
REM: for the development operating system. This version uses the Win32
REM: hosted tools by default, so you can use long filenames.

SET SCITECH_BIN=%SCITECH%\BIN;%SCITECH%\BIN-WIN32

REM: The following sets up the path to the SciTech command line utilities
REM: for the development operating system. This version uses the DOS hosted
REM: tools, which is necessary for DJGPP. You can use the Win32 hosted
REM: tools, but they run a lot slower.

SET SCITECH_BINDOS=%SCITECH%\BIN;%SCITECH%\BIN-DOS

REM: Set the following environment variable to use the Netwide Assembler
REM: (NASM) provided with the MGL tools to build all assembler modules.
REM: If you have Turbo Assembler 4.0 and you wish to build for debugging
REM: you can use it by removing the following line.

SET USE_NASM=1

REM: The following define the locations of all the compilers that you may
REM: be using. Change them to reflect where you have installed your
REM: compilers.

SET BC3_PATH=C:\C\BC3
SET BC4_PATH=C:\C\BC45
SET BC5_PATH=C:\C\BC50
SET VC_PATH=C:\C\MSVC
SET VC4_PATH=C:\C\VC42
SET VC5_PATH=C:\C\VC50
SET VC6_PATH=C:\C\VC60
SET SC70_PATH=C:\C\SC75
SET WC10A_PATH=C:\C\WC10A
SET WC10_PATH=C:\C\WC10
SET WC11_PATH=C:\C\WC11
SET TNT_PATH=C:\C\TNT
SET DJ_PATH=C:\C\DJGPP
SET CYGWIN_PATH=c:\c\cygwin-b20
SET VTOOLSD=C:\C\VTD95
SET SOFTICE_PATH=C:\C\SIW95

