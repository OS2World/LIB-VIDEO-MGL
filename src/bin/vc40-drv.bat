@echo off
REM Setup environment variables for Visual C++ 4.2 32 bit edition

if .%CHECKED%==.1 goto checked_build
set LIB=%VC4_PATH%\LIB;%SCITECH_LIB%\LIB\RELEASE\WIN32\VC4;.
echo Release build enabled.
goto setvars

:checked_build
set LIB=%VC4_PATH%\LIB;%SCITECH_LIB%\LIB\DEBUG\WIN32\VC4;.
echo Checked debug build enabled.
goto setvars

:setvars
set TOOLROOTDIR=%VC4_PATH%
set INCLUDE=.;INCLUDE;%SCITECH%\INCLUDE;%PRIVATE%\INCLUDE;%VC4_PATH%\INCLUDE
set INIT=%VC4_PATH%
SET MAKESTARTUP=%SCITECH%\MAKEDEFS\VC32.MK
SET USE_TNT=
SET USE_WIN16=
SET USE_WIN32=1
SET USE_RTTARGET=
SET VC_LIBBASE=VC4
PATH %SCITECH_BIN%;%VC4_PATH%\BIN;%TNT_PATH%\BIN;%DEFPATH%%VC32_CD_PATH%

set MASTER_MAKE=1
set DDKROOT=c:\c\95ddk
set SDKROOT=r:\sdk\mstools
set MASM_ROOT=r:\asm\masm611
set C16_ROOT=c:\c\msvc
set C32_ROOT=c:\c\vc42

echo Visual C++ 4.2 32 bit driver compilation environment set up
