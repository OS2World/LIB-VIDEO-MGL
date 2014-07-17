@echo off
REM Setup environment variables for Visual C++ 6.0 32 bit edition

if .%CHECKED%==.1 goto checked_build
set LIB=%SCITECH_LIB%\LIB\RELEASE\WIN32\VC6;%VC6_PATH%\VC98\LIB;%TNT_PATH%\LIB;.
echo Release build enabled.
goto setvars

:checked_build
set LIB=%SCITECH_LIB%\LIB\DEBUG\WIN32\VC6;%VC6_PATH%\VC98\LIB;%TNT_PATH%\LIB;.
echo Checked debug build enabled.
goto setvars

:setvars
set TOOLROOTDIR=%VC6_PATH%\VC98
set INCLUDE=.;INCLUDE;%SCITECH%\INCLUDE;%PRIVATE%\INCLUDE;%VC6_PATH%\VC98\INCLUDE;%TNT_PATH%\INCLUDE;
set INIT=%VC6_PATH%\VC98
SET MAKESTARTUP=%SCITECH%\MAKEDEFS\VC32.MK
SET USE_TNT=
SET USE_WIN16=
SET USE_WIN32=1
SET WIN32_GUI=
SET USE_RTTARGET=
SET VC_LIBBASE=vc6
PATH %SCITECH_BIN%;%VC6_PATH%\VC98\BIN;%VC6_PATH%\COMMON\MSDEV98\BIN;%TNT_PATH%\BIN;%DEFPATH%%VC32_CD_PATH%

echo Visual C++ 6.0 32-bit Windows console compilation environment set up
