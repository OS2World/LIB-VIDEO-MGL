@echo off
REM Setup for compiling with Watcom C/C++ 11.0 in 32 bit mode (DOS4GW)

if .%CHECKED%==.1 goto checked_build
SET LIB=%SCITECH_LIB%\LIB\RELEASE\DOS32\WC11;%WC11_PATH%\LIB386;%WC11_PATH%\LIB386\DOS;.
echo Release build enabled.
goto setvars

:checked_build
SET LIB=%SCITECH_LIB%\LIB\DEBUG\DOS32\WC11;%WC11_PATH%\LIB386;%WC11_PATH%\LIB386\DOS;.
echo Checked debug build enabled.
goto setvars

:setvars
SET WD=/s/li#50/tr#rsi
SET EDPATH=%WC11_PATH%\EDDAT
SET INCLUDE=INCLUDE;%SCITECH%\INCLUDE;%PRIVATE%\INCLUDE;%WC11_PATH%\H;
SET WATCOM=%WC11_PATH%
SET MAKESTARTUP=%SCITECH%\MAKEDEFS\WC32.MK
SET USE_TNT=
SET USE_X32=
SET USE_X32VM=
SET USE_WIN16=
SET USE_WIN32=
SET USE_WIN386=
SET USE_OS216=
SET USE_OS232=
SET USE_OS2GUI=
SET WC_LIBBASE=WC11
PATH %SCITECH_BIN%;%WC11_PATH%\BINNT;%WC11_PATH%\BINW;%DEFPATH%%WC_CD_PATH%

echo Watcom C/C++ 11.0 32-bit DOS compilation environment set up (DOS4GW).
