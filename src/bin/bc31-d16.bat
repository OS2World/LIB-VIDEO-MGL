@echo off
REM Setup for compiling with Borland C++ 3.1.

if .%CHECKED%==.1 goto checked_build
SET LIB=%SCITECH_LIB%\LIB\RELEASE\DOS16\BC3;%BC3_PATH%\LIB;.
%SCITECH%\bin-dos\k_cp %SCITECH%\BIN\release\tlink.31 %BC3_PATH%\BIN\tlink.cfg
%SCITECH%\bin-dos\k_cp %SCITECH%\BIN\release\turboc.31 %BC3_PATH%\BIN\turboc.cfg
echo Release build enabled.
goto setvars

:checked_build
SET LIB=%SCITECH_LIB%\LIB\DEBUG\DOS16\BC3;%BC3_PATH%\LIB;.
%SCITECH%\bin-dos\k_cp %SCITECH%\BIN\debug\tlink.31 %BC3_PATH%\BIN\tlink.cfg
%SCITECH%\bin-dos\k_cp %SCITECH%\BIN\debug\turboc.31 %BC3_PATH%\BIN\turboc.cfg
echo Checked debug build enabled.
goto setvars

:setvars
SET INCLUDE=INCLUDE;%SCITECH%\INCLUDE;%PRIVATE%\INCLUDE;%BC3_PATH%\INCLUDE;
SET MAKESTARTUP=%SCITECH%\MAKEDEFS\BC3.MK
SET USE_DPMI16=
SET USE_WIN16=
SET USE_WIN32=
PATH %SCITECH_BIN%;%BC3_PATH%\BIN;%DEFPATH%

echo Borland C++ 3.1 DOS compilation configuration set up.
