@echo off
REM Setup for compiling with DJGPP 2.01

if .%CHECKED%==.1 goto checked_build
set LIB=%SCITECH_LIB%\LIB\release\dos32\dj2
%SCITECH%\bin-dos\k_cp %SCITECH%\BIN\DJGPP.ENV %DJ_PATH%\DJGPP.ENV
echo Release build enabled.
goto setvars

:checked_build
set LIB=%SCITECH_LIB%\LIB\debug\dos32\dj2
%SCITECH%\bin-dos\k_cp %SCITECH%\BIN\DJGPP_DB.ENV %DJ_PATH%\DJGPP.ENV
echo Checked debug build enabled.
goto setvars

:setvars
set DJGPP=%DJ_PATH%\DJGPP.ENV
set INCLUDE=INCLUDE;%SCITECH%\INCLUDE;%PRIVATE%\INCLUDE;%DJ_PATH%\INCLUDE;
set MAKESTARTUP=%SCITECH%\MAKEDEFS\DJ32.MK
set DJ_LIBBASE=dj2
set USE_WIN32=
set USE_WIN16=
PATH %SCITECH_BINDOS%;%DJ_PATH%\BIN;%DEFPATH%

echo DJGPP 2.01 32-bit DOS compilation environment set up (DPMI).
