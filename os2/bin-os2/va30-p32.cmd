@echo off
REM Setup for compiling with IBM VisualAge C++ 3.0 in 32-bit mode

if .%CHECKED%==.1 goto checked_build
SET LIB=%SCITECH_LIB%\lib\release\os232\va3;%VA3_PATH%\LIB;%VA3_PATH%\TOOLKIT\LIB;%VA3_PATH%\DLL
echo Release build enabled.
goto setvars

:checked_build
SET LIB=%SCITECH_LIB%\lib\debug\os232\va3;%VA3_PATH%\LIB;%VA3_PATH%\DLL
echo Checked debug build enabled.
goto setvars

:setvars
SET INCLUDE=include;%SCITECH%\include;%PRIVATE%\include;%VA3_PATH%\INCLUDE;%VA3_PATH%\INCLUDE\OS2;%VA3_PATH%\INC;%VA3_PATH%\INCLUDE\SOM;%VA3_PATH%\TOOLKIT\H
SET MAKESTARTUP=%SCITECH%\makedefs\va32.mk
SET USE_OS232=1
SET USE_OS2GUI=1
SET VA_LIBBASE=va3
PATH %SCITECH%\bin-os2;%VA3_PATH%\BIN;%VA3_PATH%\HELP;%VA3_PATH%\SMARTS\SCRIPTS;%DEFPATH%%VA_CD_PATH%

echo IBM VisualAge C++ 3.0 OS/2 32-bit GUI compilation environment set up.
