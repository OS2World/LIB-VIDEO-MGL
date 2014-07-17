@echo off
REM Setup for compiling with Watcom C/C++ 10.0a in 32-bit OS/2 mode

if .%CHECKED%==.1 goto checked_build
SET LIB=%SCITECH_LIB%\lib\release\os232\wc10;.
echo Release build enabled.
goto setvars

:checked_build
SET LIB=%SCITECH_LIB%\lib\debug\os232\wc10;.
echo Checked debug build enabled.
goto setvars

:setvars
SET WD=/s/li#50
SET EDPATH=%WC10AA_PATH%\eddat
SET INCLUDE=include;%SCITECH%\include;%PRIVATE%\include;%WC10AA_PATH%\h\os2;%WC10AA_PATH%\h
SET WATCOM=%WC10AA_PATH%
SET MAKESTARTUP=%SCITECH%\makedefs\wc32.mk
SET USE_WIN16=
SET USE_WIN32=
SET USE_WIN386=
SET USE_OS216=
SET USE_OS232=1
SET USE_OS2GUI=
SET WC_LIBBASE=wc10
SET EDPATH=%WC10AA_PATH%\EDDAT
PATH %SCITECH_BIN%;%WC10AA_PATH%\BINNT;%WC10AA6_PATH%\BINB;%DEFPATH%%WC_CD_PATH%

echo Watcom C/C++ 10.0a 32-bit OS/2 console compilation environment set up.
