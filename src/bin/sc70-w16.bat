@echo off
REM Setup for compiling with Symantec C++ 7.5 in 16 bit Windows mode.

if .%CHECKED%==.1 goto checked_build
SET LIB=%SCITECH_LIB%\LIB\RELEASE\WIN16\SC7;%SC70_PATH%\LIB;%SC70_PATH%\MFC\LIB;.
echo Release build enabled.
goto setvars

:checked_build
SET LIB=%SCITECH_LIB%\LIB\DEBUG\WIN16\SC7;%SC70_PATH%\LIB;%SC70_PATH%\MFC\LIB;.
echo Checked debug build enabled.
goto setvars

:setvars
SET INCLUDE=.;INCLUDE;%SCITECH%\INCLUDE;%PRIVATE%\INCLUDE;%SC70_PATH%\INCLUDE;%SC70_PATH%\INCLUDE\WIN16;%SC70_PATH%\INCLUDE\WIN32;%SC70_PATH%\MFC\INCLUDE
SET HELP=%SC70_PATH%\HELP
SET MAKESTARTUP=%SCITECH%\MAKEDEFS\SC16.MK
SET USE_WIN16=1
SET USE_WIN32=
SET SC_LIBBASE=sc7
PATH %SCITECH_BIN%;%SC70_PATH%\BIN;%SC70_PATH%\BIN;%DEFPATH%%SC_CD_PATH%

echo Symantec C++ 7.5 16 bit Windows compilation environment set up.
