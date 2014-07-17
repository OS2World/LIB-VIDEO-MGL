@echo off
REM Setup for compiling with Borland C++ 2.0 in 32-bit OS/2 mode.

if .%CHECKED%==.1 goto checked_build
SET LIB=%SCITECH_LIB%\lib\release\os232\bc2;%BC2_PATH%\lib;.
%SCITECH%\bin-os2\cp %SCITECH%\bin-os2\release\tlink.os2 %BC2_PATH%\bin\tlink.cfg
%SCITECH%\bin-os2\cp %SCITECH%\bin-os2\release\turboc.os2 %BC2_PATH%\bin\turboc.cfg
echo Release build enabled.
goto setvars

:checked_build
SET LIB=%SCITECH_LIB%\lib\debug\os232\bc2;%BC2_PATH%\lib;.
%SCITECH%\bin-os2\cp %SCITECH%\bin-os2\debug\tlink.os2 %BC2_PATH%\bin\tlink.cfg
%SCITECH%\bin-os2\cp %SCITECH%\bin-os2\debug\turboc.os2 %BC2_PATH%\bin\turboc.cfg
echo Checked debug build enabled.
goto setvars

:setvars
SET INCLUDE=INCLUDE;%SCITECH%\INCLUDE;%PRIVATE%\INCLUDE;%BC2_PATH%\INCLUDE;
SET MAKESTARTUP=%SCITECH%\MAKEDEFS\BCOS2.MK
SET USE_OS2GUI=1
SET BC_LIBBASE=bc2
PATH %SCITECH%\BIN-OS2;%BC2_PATH%\BIN;%DEFPATH%%BC2_CD_PATH%

echo Borland C++ 2.0 32-bit OS/2 GUI compilation configuration set up.
