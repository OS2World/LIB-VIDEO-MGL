@echo off
REM Setup environment variables for Microsoft C 6.0 16 bit edition

if .%CHECKED%==.1 goto checked_build
set LIB=C:\DDK\MSC60\LIB;.
echo Release build enabled.
goto setvars

:checked_build
set LIB=C:\DDK\MSC60\LIB;.
echo Checked debug build enabled.
goto setvars

:setvars
set TOOLROOTDIR=%VC6_PATH%\VC98
set INCLUDE=.;INCLUDE;%SCITECH%\INCLUDE;%PRIVATE%\INCLUDE;C:\DDK\MSC60\INCLUDE
SET MAKESTARTUP=%SCITECH%\MAKEDEFS\CL16.MK
SET USE_TNT=
SET USE_OS216=1
SET USE_RTTARGET=
SET HOST_OS=os2
SET HOST_PROC=386
PATH %SCITECH%\bin-os2;C:\DDK\MSC60\BINP;c:\ddk\tools;c:\ddk\masm60\binp;%SCITECH%\bin-os2;%DEFPATH%

set MASTER_MAKE=1
set DDKROOT=c:\ddk
set MASM_ROOT=c:\ddk\masm60
set C16_ROOT=c:\ddk\msc60
set USE_MASM=1

echo Microsoft C 6.0 16-bit driver compilation environment set up
