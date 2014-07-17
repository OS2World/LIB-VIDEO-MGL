@echo off
REM: Set the place where SciTech Software is installed, and where each
REM: of the supported compilers is installed. These environment variables
REM: are used by the batch files in the SCITECH\BIN-OS2 directory.
REM:
REM: Modify the as appropriate for your compiler configuration (you should
REM: only need to change things in this batch file).
REM:
REM: This version is for a normal OS/2 installation.

REM: The SCITECH variable points to where batch files, makefile startups,
REM: include files and source files will be found when compiling.

SET SCITECH=c:\scitech

REM: The SCITECH_LIB variable points to where the SciTech libraries live
REM: for installation and linking. This allows you to have the source and
REM: include files on local machines for compiling and have the libraries
REM: located on a common network machine (for network builds).

SET SCITECH_LIB=c:\scitech

REM: The PRIVATE variable points to where private source files reside that
REM: do not live in the public source tree

SET PRIVATE=c:\private

REM: Set the following environment variable to use the Netwide Assembler
REM: (NASM) provided with the MGL tools to build all assembler modules.
REM: If you have Turbo Assembler 4.0 and you wish to build for debugging
REM: you can use it by removing the following line.

SET USE_NASM=1

REM: The following define the locations of all the compilers that you may
REM: be using. Change them to reflect where you have installed your
REM: compilers.

SET VA3_PATH=C:\C\IBMC30
SET WC10_PATH=C:\C\WC10
SET WC11_PATH=C:\C\WC11
SET BC2_PATH=C:\C\BCOS2
SET EMX_PATH=C:\C\EMX

REM: Let the makefile utilities know that we are runing in an OS/2 shell

SET OS2_SHELL=1
