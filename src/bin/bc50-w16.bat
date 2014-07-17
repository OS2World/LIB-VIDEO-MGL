 @echo off
REM Setup for compiling with Borland C++ 5.0 in 16 bit Windows mode.

if .%CHECKED%==.1 goto checked_build
SET LIB=%SCITECH_LIB%\LIB\RELEASE\WIN16\BC5;%BC5_PATH%\LIB;.
%SCITECH%\bin-dos\k_cp %SCITECH%\BIN\release\tlink5.win %BC5_PATH%\BIN\tlink.cfg
%SCITECH%\bin-dos\k_cp %SCITECH%\BIN\release\turboc5.win %BC5_PATH%\BIN\turboc.cfg
echo Release build enabled.
goto setvars

:checked_build
SET LIB=%SCITECH_LIB%\LIB\DEBUG\WIN16\BC5;%BC5_PATH%\LIB;.
%SCITECH%\bin-dos\k_cp %SCITECH%\BIN\debug\tlink5.win %BC5_PATH%\BIN\tlink.cfg
%SCITECH%\bin-dos\k_cp %SCITECH%\BIN\debug\turboc5.win %BC5_PATH%\BIN\turboc.cfg
echo Checked debug build enabled.
goto setvars

:setvars
SET INCLUDE=INCLUDE;%SCITECH%\INCLUDE;%PRIVATE%\INCLUDE;%BC5_PATH%\INCLUDE;
SET MAKESTARTUP=%SCITECH%\MAKEDEFS\BC16.MK
SET USE_DPMI16=
SET USE_WIN16=1
SET USE_WIN32=
SET USE_VXD=
SET USE_BC5=1
SET USE_SMX32=
SET USE_SMX16=
SET WIN32_GUI=
SET BC_LIBBASE=BC5
PATH %SCITECH_BIN%;%BC5_PATH%\BIN;%DEFPATH%%BC5_CD_PATH%

echo Borland C++ 5.0 16 bit Windows compilation configuration set up.
