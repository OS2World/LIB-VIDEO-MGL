@echo off
REM Setup for compiling with Borland C++ 5.0 in 32 bit mode.

if .%CHECKED%==.1 goto checked_build
SET LIB=%SCITECH_LIB%\LIB\RELEASE\DOS32\BC5;%BC5_PATH%\LIB;.
%SCITECH%\bin-dos\k_cp %SCITECH%\BIN\release\tlink52.dos %BC5_PATH%\BIN\tlink32.cfg
%SCITECH%\bin-dos\k_cp %SCITECH%\BIN\release\bcc52.dos %BC5_PATH%\BIN\bcc32.cfg
echo Release build enabled.
goto setvars

:checked_build
SET LIB=%SCITECH_LIB%\LIB\DEBUG\DOS32\BC5;%BC5_PATH%\LIB;.
%SCITECH%\bin-dos\k_cp %SCITECH%\BIN\debug\tlink52.dos %BC5_PATH%\BIN\tlink32.cfg
%SCITECH%\bin-dos\k_cp %SCITECH%\BIN\debug\bcc52.dos %BC5_PATH%\BIN\bcc32.cfg
echo Checked debug build enabled.
goto setvars

:setvars
SET INCLUDE=INCLUDE;%SCITECH%\INCLUDE;%PRIVATE%\INCLUDE;%BC5_PATH%\INCLUDE;
SET MAKESTARTUP=%SCITECH%\MAKEDEFS\BC32.MK
SET USE_DPMI16=
SET USE_WIN16=
SET USE_WIN32=
SET USE_VXD=
SET USE_TNT=
SET USE_SMX32=
SET USE_SMX16=
SET USE_BC5=1
SET WIN32_GUI=
SET BC_LIBBASE=BC5
PATH %SCITECH_BIN%;%BC5_PATH%\BIN;%DEFPATH%%BC5_CD_PATH%

echo Borland C++ 5.0 32 bit DOS compilation configuration set up (DPMI32).
