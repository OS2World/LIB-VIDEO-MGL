@echo off
rem Batch file to create the initial setup for using the SciTech MGL
rem Graphics Library, using the selected default compiler.

rem Setup the default path environment variable

set DEFPATH=E:\WIN95\COMMAND;C:\BIN\MSDOS;C:\BIN\COMMON
PATH %DEFPATH%

rem Initialise the default compile time environment

call C:\SCITECH\bin\set-vars.bat
call C:\SCITECH\bin\WC10-D32.BAT

rem Setup the location to the MGL data files

set MGL_ROOT=C:\SCITECH

rem Change to the directory containing all the MGL sample code

cd C:\SCITECH\EXAMPLES\MGL

