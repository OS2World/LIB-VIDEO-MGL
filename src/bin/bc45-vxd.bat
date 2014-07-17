@echo off
REM Setup for compiling with Borland C++ 4.5 in 32 bit Windows VxD mode.

if .%CHECKED%==.1 goto checked_build
SET LIB=%SCITECH_LIB%\LIB\RELEASE\VXD\BC4;%BC4_PATH%\LIB;.
%SCITECH%\bin-dos\k_cp %SCITECH%\BIN\release\tlink32.vxd %BC4_PATH%\BIN\tlink32.cfg
%SCITECH%\bin-dos\k_cp %SCITECH%\BIN\release\bcc32.vxd %BC4_PATH%\BIN\bcc32.cfg
echo Release build enabled.
goto setvars

:checked_build
SET LIB=%SCITECH_LIB%\LIB\DEBUG\VXD\BC4;%BC4_PATH%\LIB;.
%SCITECH%\bin-dos\k_cp %SCITECH%\BIN\debug\tlink32.vxd %BC4_PATH%\BIN\tlink32.cfg
%SCITECH%\bin-dos\k_cp %SCITECH%\BIN\debug\bcc32.vxd %BC4_PATH%\BIN\bcc32.cfg
echo Checked debug build enabled.
goto setvars

:setvars
SET INCLUDE=INCLUDE;%SCITECH%\INCLUDE;%PRIVATE%\INCLUDE;%BC4_PATH%\INCLUDE;
SET MAKESTARTUP=%SCITECH%\MAKEDEFS\BC32.MK
SET USE_WIN16=
SET USE_WIN32=
SET USE_VXD=1
SET USE_TNT=
SET USE_BC5=
SET WIN32_GUI=
SET BC_LIBBASE=BC4
PATH %SCITECH_BIN%;%BC4_PATH%\BIN;%DEFPATH%%BC_CD_PATH%

echo Borland C++ 4.5 32-bit VxD compilation configuration set up.
