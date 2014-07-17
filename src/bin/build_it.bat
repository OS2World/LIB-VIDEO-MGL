@echo off
rem Generic batch file to build a version of the library. This batch file
rem assumes that the correct batch files exist to setup the appropriate
rem compilation environments, and that the DMAKE.EXE program is available
rem somewhere on the path.
rem
rem Builds as release or debug depending on the value of the CHECKED
rem environment variable.

rem Unset all environment variables that change the compile process
set DBG=
set OPT=
set OPT_SIZE=
set BUILD_DLL=
set IMPORT_DLL=
set FPU=
set CHECKS=
set BETA=

if %1==bc31-d16 goto bc31-d16
if %1==bc45-d16 goto bc45-d16
if %1==bc45-d32 goto bc45-d32
if %1==bc45-tnt goto bc45-tnt
if %1==bc45-w16 goto bc45-w16
if %1==bc45-w32 goto bc45-w32
if %1==bc45-c32 goto bc45-c32
if %1==bc45-vxd goto bc45-vxd
if %1==bc50-d16 goto bc50-d16
if %1==bc50-d32 goto bc50-d32
if %1==bc50-tnt goto bc50-tnt
if %1==bc50-w16 goto bc50-w16
if %1==bc50-w32 goto bc50-w32
if %1==bc50-c32 goto bc50-c32
if %1==bc50-vxd goto bc50-vxd
if %1==dj20-d32 goto dj20-d32
if %1==sc70-d16 goto sc70-d16
if %1==sc70-w16 goto sc70-w16
if %1==sc70-tnt goto sc70-tnt
if %1==sc70-w32 goto sc70-w32
if %1==vc40-d16 goto vc40-d16
if %1==vc40-tnt goto vc40-tnt
if %1==vc40-w16 goto vc40-w16
if %1==vc40-w32 goto vc40-w32
if %1==vc40-c32 goto vc40-c32
if %1==vc40-drv goto vc40-drv
if %1==vc40-rtt goto vc40-rtt
if %1==vc50-d16 goto vc50-d16
if %1==vc50-tnt goto vc50-tnt
if %1==vc50-w16 goto vc50-w16
if %1==vc50-w32 goto vc50-w32
if %1==vc50-c32 goto vc50-c32
if %1==vc50-drv goto vc50-drv
if %1==vc50-rtt goto vc50-rtt
if %1==vc60-d16 goto vc60-d16
if %1==vc60-tnt goto vc60-tnt
if %1==vc60-w16 goto vc60-w16
if %1==vc60-w32 goto vc60-w32
if %1==vc60-c32 goto vc60-c32
if %1==vc60-drv goto vc60-drv
if %1==vc60-rtt goto vc60-rtt
if %1==wc10-d16 goto wc10-d16
if %1==wc10-d32 goto wc10-d32
if %1==wc10-tnt goto wc10-tnt
if %1==wc10-w16 goto wc10-w16
if %1==wc10-w32 goto wc10-w32
if %1==wc10-c32 goto wc10-c32
if %1==wc10-o32 goto wc10-o32
if %1==wc10-p32 goto wc10-p32
if %1==wc11-d16 goto wc11-d16
if %1==wc11-d32 goto wc11-d32
if %1==wc11-tnt goto wc11-tnt
if %1==wc11-w16 goto wc11-w16
if %1==wc11-w32 goto wc11-w32
if %1==wc11-c32 goto wc11-c32
if %1==wc11-o32 goto wc11-o32
if %1==wc11-p32 goto wc11-p32

echo Usage: BUILD compiler_name [DMAKE commands]
echo.
echo Current compilers:                                    Extenders:
echo    bc45-d16, bc45-w16 - Borland C++ 4.5x 16 bit
echo    bc45-d32, bc45-w32 - Borland C++ 4.5x 32 bit       (DPMI32)
echo    bc50-d16, bc50-w16 - Borland C++ 5.x 16 bit
echo    bc50-d32, bc50-w32 - Borland C++ 5.x 32 bit        (DPMI32)
echo    bc45-tnt, bc50-tnt - Borland C++ 4.x/5.x 32 bit    (Phar Lap TNT)
echo    vc40-d16, vc40-w16 - Visual C++ 4.x 16 bit
echo    vc40-tnt, vc40-w32 - Visual C++ 4.x 32 bit         (Phar Lap TNT)
echo    vc50-d16, vc50-w16 - Visual C++ 5.x 16 bit
echo    vc50-tnt, vc50-w32 - Visual C++ 5.x 32 bit         (Phar Lap TNT)
echo    wc10-d16, wc10-w16 - Watcom C++ 10.6 16 bit
echo    wc10-d32, wc10-w32 - Watcom C++ 10.6 32 bit        (DOS4GW)
echo    wc11-d16, wc11-w16 - Watcom C++ 11.0 16 bit
echo    wc11-d32, wc11-w32 - Watcom C++ 11.0 32 bit        (DOS4GW)
echo    wc10-tnt, wc11-tnt - Watcom C++ 10.6/11.0 32 bit   (Phar Lap TNT)
goto end

rem -------------------------------------------------------------------------
rem Setup for the specified compiler

:bc31-d16
call bc31-d16.bat
goto compileit

:bc45-d16
call bc45-d16.bat
goto compileit

:bc45-d32
call bc45-d32.bat
goto compileit

:bc45-tnt
call bc45-tnt.bat
goto compileit

:bc45-w16
call bc45-w16.bat
goto compileit

:bc45-w32
call bc45-w32.bat
goto compileit

:bc45-c32
call bc45-c32.bat
goto compileit

:bc45-vxd
call bc45-vxd.bat
goto compileit

:bc50-d16
call bc50-d16.bat
goto compileit

:bc50-d32
call bc50-d32.bat
goto compileit

:bc50-tnt
call bc50-tnt.bat
goto compileit

:bc50-w16
call bc50-w16.bat
goto compileit

:bc50-w32
call bc50-w32.bat
goto compileit

:bc50-c32
call bc50-c32.bat
goto compileit

:bc50-vxd
call bc50-vxd.bat
goto compileit

:dj20-d32
call dj20-d32.bat
goto compileit

:sc70-d16
call sc70-d16.bat
goto compileit

:sc70-w16
call sc70-w16.bat
goto compileit

:sc70-tnt
call sc70-tnt.bat
goto compileit

:sc70-w32
call sc70-w32.bat
goto compileit

:sc70-c32
call sc70-c32.bat
goto compileit

:vc40-d16
call vc40-d16.bat
goto compileit

:vc40-tnt
call vc40-tnt.bat
goto compileit

:vc40-w16
call vc40-w16.bat
goto compileit

:vc40-w32
call vc40-w32.bat
goto compileit

:vc40-c32
call vc40-c32.bat
goto compileit

:vc40-drv
call vc40-drv.bat
goto compileit

:vc40-rtt
call vc40-rtt.bat
goto compileit

:vc50-d16
call vc50-d16.bat
goto compileit

:vc50-tnt
call vc50-tnt.bat
goto compileit

:vc50-w16
call vc50-w16.bat
goto compileit

:vc50-w32
call vc50-w32.bat
goto compileit

:vc50-c32
call vc50-c32.bat
goto compileit

:vc50-drv
call vc50-drv.bat
goto compileit

:vc50-rtt
call vc50-rtt.bat
goto compileit

:vc60-d16
call vc60-d16.bat
goto compileit

:vc60-tnt
call vc60-tnt.bat
goto compileit

:vc60-w16
call vc60-w16.bat
goto compileit

:vc60-w32
call vc60-w32.bat
goto compileit

:vc60-c32
call vc60-c32.bat
goto compileit

:vc60-drv
call vc60-drv.bat
goto compileit

:vc60-rtt
call vc60-rtt.bat
goto compileit

:wc10-d16
call wc10-d16.bat
goto compileit

:wc10-d32
call wc10-d32.bat
goto compileit

:wc10-tnt
call wc10-tnt.bat
goto compileit

:wc10-w16
call wc10-w16.bat
goto compileit

:wc10-w32
call wc10-w32.bat
goto compileit

:wc10-c32
call wc10-c32.bat
goto compileit

:wc10-o32
call wc10-o32.bat
goto compileit

:wc10-p32
call wc10-p32.bat
goto compileit

:wc11-d16
call wc11-d16.bat
goto compileit

:wc11-d32
call wc11-d32.bat
goto compileit

:wc11-tnt
call wc11-tnt.bat
goto compileit

:wc11-w16
call wc11-w16.bat
goto compileit

:wc11-w32
call wc11-w32.bat
goto compileit

:wc11-c32
call wc11-c32.bat
goto compileit

:wc11-o32
call wc11-o32.bat
goto compileit

:wc11-p32
call wc11-p32.bat
goto compileit

:compileit
k_rm -f *.lib *.a
dmake %2 %3 %4 %5 %6 %7 %8 %9
if errorlevel 1 goto errorend
goto end

:errorend
echo *************************************************
echo * An error occurred while building the library. *
echo *************************************************
:end
