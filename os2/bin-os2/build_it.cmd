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

if %1==va30-o32 goto va30-o32
if %1==va30-p32 goto va30-p32
if %1==wc10-o16 goto wc10-o16
if %1==wc10-o32 goto wc10-o32
if %1==wc10-p32 goto wc10-p32
if %1==wc11-o16 goto wc11-o16
if %1==wc11-o32 goto wc11-o32
if %1==wc11-p32 goto wc11-p32
if %1==bc20-o32 goto bc20-o32
if %1==bc20-o32 goto bc20-o32
if %1==emx-o32 goto emx-p32
if %1==emx-p32 goto emx-p32
if %1==cl60-drv goto cl60-drv

echo Usage: BUILD compiler_name [DMAKE commands]
echo.
echo Current compilers:
echo    va30-o32, va32-p32 - IBM VisualAge C++ 3.0 32-bit
echo    wc10-o16           - Watcom C++ 10.6 16-bit
echo    wc10-o32, wc10-p32 - Watcom C++ 10.6 32-bit
echo    wc11-o16           - Watcom C++ 11.0 16-bit
echo    wc11-o32, wc11-p32 - Watcom C++ 11.0 32-bit
echo    bc20-o32, bc20-p32 - Borland C++ 2.0 32-bit
echo    emx-o32, emx-p32   - EMX 0.9c 32-bit
goto end

rem -------------------------------------------------------------------------
rem Setup for the specified compiler

:va30-o32
call va30-o32.cmd
goto compileit

:va30-p32
call va30-p32.cmd
goto compileit

:wc10-o16
call wc10-o16.cmd
goto compileit

:wc10-o32
call wc10-o32.cmd
goto compileit

:wc10-p32
call wc10-p32.cmd
goto compileit

:wc11-o16
call wc11-o16.cmd
goto compileit

:wc11-o32
call wc11-o32.cmd
goto compileit

:wc11-p32
call wc11-p32.cmd
goto compileit

:bc20-o32
call bc20-o32.cmd
goto compileit

:bc20-p32
call bc20-p32.cmd
goto compileit

:emx-o32
call emx-o32.cmd
goto compileit

:emx-p32
call emx-p32.cmd
goto compileit

:cl60-drv
call cl60-drv.cmd
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
