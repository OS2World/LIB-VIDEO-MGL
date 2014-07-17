@echo off
REM Setup for compiling with Borland C++ 4.5 in 16 bit mode.

if .%CHECKED%==.1 goto checked_build
SET LIB=%SCITECH_LIB%\LIB\RELEASE\DOS16\BC4;%BC4_PATH%\LIB;.
%SCITECH%\bin-dos\k_cp %SCITECH%\BIN\release\tlink.dos %BC4_PATH%\BIN\tlink.cfg
%SCITECH%\bin-dos\k_cp %SCITECH%\BIN\release\turboc.dos %BC4_PATH%\BIN\turboc.cfg
echo Release build enabled.
goto setvars

:checked_build
SET LIB=%SCITECH_LIB%\LIB\DEBUG\DOS16\BC4;%BC4_PATH%\LIB;.
%SCITECH%\bin-dos\k_cp %SCITECH%\BIN\debug\tlink.dos %BC4_PATH%\BIN\tlink.cfg
%SCITECH%\bin-dos\k_cp %SCITECH%\BIN\debug\turboc.dos %BC4_PATH%\BIN\turboc.cfg
echo Checked debug build enabled.
goto setvars

:setvars
SET INCLUDE=INCLUDE;%SCITECH%\INCLUDE;%PRIVATE%\INCLUDE;%BC4_PATH%\INCLUDE
SET MAKESTARTUP=%SCITECH%\MAKEDEFS\BC16.MK
SET USE_DPMI16=
SET USE_WIN16=
SET USE_WIN32=
SET USE_VXD=
SET USE_BC5=
SET WIN32_GUI=
SET BC_LIBBASE=BC4
PATH %SCITECH_BIN%;%BC4_PATH%\BIN;%DEFPATH%%BC_CD_PATH%

echo Borland C++ 4.5 16 bit DOS compilation configuration set up.
