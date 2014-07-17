@echo off
REM Setup environment variables for Visual C++ 5.0 32 bit edition

if .%CHECKED%==.1 goto checked_build
set LIB=%SCITECH_LIB%\LIB\RELEASE\WIN32\VC5;%VC5_PATH%\VC\LIB;%TNT_PATH%\LIB;.
echo Release build enabled.
goto setvars

:checked_build
set LIB=%SCITECH_LIB%\LIB\DEBUG\WIN32\VC5;%VC5_PATH%\VC\LIB;%TNT_PATH%\LIB;.
echo Checked debug build enabled.
goto setvars

:setvars
set TOOLROOTDIR=%VC5_PATH%\VC
set INCLUDE=.;INCLUDE;%SCITECH%\INCLUDE;%PRIVATE%\INCLUDE;%VC5_PATH%\VC\INCLUDE;%TNT_PATH%\INCLUDE;
set INIT=%VC5_PATH%\VC
SET MAKESTARTUP=%SCITECH%\MAKEDEFS\VC32.MK
SET USE_TNT=
SET USE_WIN16=
SET USE_WIN32=1
SET WIN32_GUI=1
SET USE_RTTARGET=
SET VC_LIBBASE=vc5
PATH %SCITECH_BIN%;%VC5_PATH%\VC\BIN;%VC5_PATH%\SHAREDIDE\BIN;%TNT_PATH%\BIN;%DEFPATH%%VC32_CD_PATH%

echo Visual C++ 5.0 32-bit Windows compilation environment set up
