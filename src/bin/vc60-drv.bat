@echo off
REM Setup environment variables for Visual C++ 6.0 32 bit edition

if .%CHECKED%==.1 goto checked_build
set LIB=%VC6_PATH%\VC98\LIB;%SCITECH_LIB%\LIB\RELEASE\WIN32\VC6;.
echo Release build enabled.
goto setvars

:checked_build
set LIB=%VC6_PATH%\VC98\LIB;%SCITECH_LIB%\LIB\DEBUG\WIN32\VC6;.
echo Checked debug build enabled.
goto setvars

:setvars
set TOOLROOTDIR=%VC6_PATH%\VC98
set INCLUDE=.;INCLUDE;%SCITECH%\INCLUDE;%PRIVATE%\INCLUDE;%VC6_PATH%\VC98\INCLUDE
set INIT=%VC6_PATH%\VC98
SET MAKESTARTUP=%SCITECH%\MAKEDEFS\VC32.MK
SET USE_TNT=
SET USE_WIN16=
SET USE_WIN32=1
SET USE_RTTARGET=
SET VC_LIBBASE=vc6
PATH %SCITECH_BIN%;%VC6_PATH%\VC98\BIN;%VC6_PATH%\COMMON\MSDEV98\BIN;%TNT_PATH%\BIN;%DEFPATH%%VC32_CD_PATH%

set MASTER_MAKE=1
set DDKROOT=c:\c\95ddk
set SDKROOT=r:\sdk\mstools
set MASM_ROOT=r:\asm\masm611
set C16_ROOT=c:\c\msvc
set C32_ROOT=c:\c\vc60

echo Visual C++ 6.0 32-bit driver compilation environment set up
