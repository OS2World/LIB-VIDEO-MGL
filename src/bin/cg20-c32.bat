@echo off
REM Setup for compiling with Cygwin GNU C compiler

if .%CHECKED%==.1 goto checked_build
set LIB=%SCITECH_LIB%\LIB\release\win32\cg20
echo Release build enabled.
goto setvars

:checked_build
set LIB=%SCITECH_LIB%\LIB\debug\win32\cg20
echo Checked debug build enabled.
goto setvars

:setvars
set INCLUDE=INCLUDE;%SCITECH%\INCLUDE;%PRIVATE%\INCLUDE
set MAKESTARTUP=%SCITECH%\MAKEDEFS\CYGWIN.MK
set MAKE_MODE=UNIX
set USE_WIN16=
set USE_WIN32=1
set WIN32_GUI=
set CYG_LIBBASE=cg20
PATH %SCITECH_BIN%;%CYGWIN_PATH%\h-i586-cygwin32\BIN;%DEFPATH%

echo Cygwin 20.1 32-bit Win32 console compilation environment set up

