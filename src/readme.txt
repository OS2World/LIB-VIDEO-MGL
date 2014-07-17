
                SciTech Multi-platform Graphics Library
                            Version 4.5
                              BETA 5

                       Getting Started Guide


What is this document for?
--------------------------

This text document is intended to help developers get up and running
with the MGL 4.5 beta products, and to show the steps necessary to
compile and link the libraries and sample programs from for your
OS and compiler configuration.


DJGPP 2.02 and SciTech MGL
--------------------------

The MGL 4.5 is designed to work with DJGPP 2.02 and GCC 2.9.5. If you
don't have all the correct files or you are having problems, we recommend
that you download and install DJGPP 2.02 using the Windows based install
archive we have created. The name of the file is:

 djgpp202.exe         - Windows hosted DJGPP 2.02 32-bit DOS compiler


Installing the files
--------------------

To install the files you will need a copy of the source archive, the base
utilities archive for your OS and the font and bitmap resource archive.
Uncompress all files in the archive into the directoy on your system
where you want the files to live (normally c:\scitech or ~/scitech). The
names of the archives you will need are the following for the BETA 4
release:

 mgl45b5-src.zip      - Source archive in ZIP format
 mgl45b5-font.zip     - Base font and bitmap archive in ZIP format
 mgl45b5-ttf1.zip     - Free TrueType Font archive 1 in ZIP format
 mgl45b5-ttf2.zip     - Free TrueType Font archive 2 in ZIP format
 mgl45b5-dos.zip      - DOS hosted base utilities
 mgl45b5-os2.zip      - OS/2 hosted base utilities

 mgl45b5-src.tar.gz   - Source archive in gzip'ed tar format
 mgl45b5-font.tar.gz  - Base font and bitmap archive in gzip'ed tar format
 mgl45b5-ttf1.tar.gz  - Free TrueType Font archive 1 in gzip'ed tar format
 mgl45b5-ttf2.tar.gz  - Free TrueType Font archive 2 in gzip'ed tar format
 mgl45b5-linux.tar.gz - Linux hosted base utilities
 mgl45b5-qnx.tar.gz   - QNX hosted base utilities

If you have already downloaded and installed the font and bitmap files
from a previous beta, they have not changed so you do not need to download
those files again.


Makefile utilities configuration
--------------------------------

Once you have installed the files, you need to configure the makefile
utilties and tools to allow you to compile the libraries. If you are
compiling under DOS a Windows DOS box, you will need to set the environment
for your DOS box to at least 2048 bytes as we use a lot of environment
variables.

The first step to set up the compile environment is always to edit the
set-vars script files specific to your platform. This file contains two
very important variables that you need to ensure are set correctly. These
are:

  SCITECH     - Points to the location where you installed the files
  SCITECH_LIB - Should be the same as the above

The SCITECH_LIB variable is where files get copied to when you do a
'dmake install', and is usually the same as the SCITECH variable. However
it can be different if you wish the libraries to be installed to a
different path such as on a shared file server for release builds.

You should also ensure that you set the MGL_ROOT environment variable,
which points to the location of where you installed the MGL files. This
is a runtime varaiable that is used by the MGL to find all font, bitmap,
icon and cursor resource files.


    DOS/Windows hosted tools
    ------------------------

    The first thing you need to do is edit the bin\set-vars.bat batch
    file to reference the location where you have installed the files,
    and the locations where all your compilers are installed. See the
    comments in set-vars.bat for more information.

    Once you have the startup file configured, you then need to run the
    following each time you start a command shell to enable the SciTech
    makefile utilities (a good idea to put into your startup batch files):

        call c:\scitech\bin\set-vars.bat
        call c:\scitech\bin\wc11-d32.bat

    The second batch file sets up the compiler configuration for your
    default compiler. The line above sets up for Watcom C++ 11.0 32-bit
    DOS compilation. Substitute this for any of the batch files in the
    bin directory for the compiler you are using.

    OS/2 hosted tools
    ------------------

    The first thing you need to do is edit the bin-os2\set-vars.cmd
    script file to reference the location where you have installed the
    files, and the locations where all your compilers are installed. See
    the comments in set-vars.cmd for more information.

    Once you have the startup file configured, you then need to run the
    following each time you start a command shell to enable the SciTech
    makefile utilities (a good idea to put into your startup batch files):

        call c:\scitech\bin-os2\set-vars.cmd
        call c:\scitech\bin-os2\wc11-o32.cmd

    The second batch file sets up the compiler configuration for your
    default compiler. The line above sets up for Watcom C++ 11.0 32-bit
    OS/2 compilation. Substitute this for any of the batch files in the
    bin directory for the compiler you are using.

    NOTE: In order for any of the OS/2 sample programs to run, you must
          have the SDDHELP.SYS device driver loaded. Hence ensure you
          add this device driver to your CONFIG.SYS file and then reboot
          in order to start using the MGL programs.

    Linux hosted tools
    ------------------

    The first thing you need to do is edit the bin\set-vars-linux.sh
    script file to reference the location where you have installed the
    files, and the locations where all your compilers are installed. See
    the comments in set-vars-linux.sh for more information.

    Note that the Linux version of the set-vars uses the MGL_ROOT
    environment variable to set the SCITECH and SCITECH_LIB variables.
    For the most part you can probably just set the MGL_ROOT variable
    and then execute the set-vars-linux.sh shell script from your
    default shell profile (ie: .bash_profile etc).

    Once you have the startup file configured, you then need to run the
    following each time you start a command shell to enable the SciTech
    makefile utilities (a good idea to put into your startup scripts):

        . ~/scitech/bin/set-vars-linux.sh
        . ~/scitech/bin/gcc-linux.sh

    The second script file sets up the compiler configuration for your
    default compiler. The line above sets up for GNU C/C++ for Linux
    (eventually other compilers will come to Linux, such a MetroWerks).

    NOTE: In the above script code, you need to *source* the script
          files to ensure the environment variables are exported to your
          regular shell. Hence make sure the leading '.' is included!

    NOTE: If you are developing on an older libc5 based system (as opposed
          to the newer glibc as used by Red Hat 5.x etc), you will also
          need to set the LIBC=1 environment variable. This will tell
          our script files that you running on an older system and need
          to use the libc5 compiled binaries, and to put your compiled
          libraries into the libc5 directories.

    NOTE: In order to build the MGL libraries, you should have the latest
          Linux 2.x kernel sources installed. The PM library depends upon
          a number of header files from the 2.x kernels (joystick.h and
          mtrr.h) in order to build. You can build for older kernels if you
          wish, but you will need to modify the PM library makefiles to do
          this.

    NOTE: In order to use the mouse and joystick under Linux, you will need
          to set up the MGL mouse and joystick environment variables. See
          the section below on Linux Rutime Setup for more information.

    QNX hosted tools
    ----------------

    The first thing you need to do is edit the bin\set-vars-qnx.sh
    script file to reference the location where you have installed the
    files, and the locations where all your compilers are installed. See
    the comments in set-vars-qnx.sh for more information.

    Note that the QNX version of the set-vars uses the MGL_ROOT
    environment variable to set the SCITECH and SCITECH_LIB variables.
    For the most part you can probably just set the MGL_ROOT variable
    and then execute the set-vars-qnx.sh shell script from your
    default shell profile (ie: .bash_profile etc).

    Once you have the startup file configured, you then need to run the
    following each time you start a command shell to enable the SciTech
    makefile utilities (a good idea to put into your startup scripts):

        . ~/scitech/bin/set-vars-qnx.sh
        . ~/scitech/bin/qnx4.sh

    The second script file sets up the compiler configuration for your
    default compiler. The line above sets up for Watcom C++ 10.6 for QNX.

    NOTE: In the above script code, you need the *source* the script
          files to ensure the environment variables are exported to your
          regular shell. Hence make sure the leading '.' is included!

    NOTE: For QNX development you will need to set the USE_BIOS=1 environment
          variable to enable support for calling the BIOS. You will also need
          to copy the vbios.lib files from the drivers/qnx directory into your
          runtime library directories. The final release will allow you to
          build MGL programs without requiring the BIOS support, but for
          the moment the BIOS support is required to run under QNX, even
          though it is not used if you are running on SciTech Nucleus drivers.

    NOTE: The MGL for QNX uses an updated version of the QNX Input driver
          and a special MglMouse driver for mouse support. You will need to
          copy these two programs from the scitech/drivers/qnx directory
          into your /bin directory. After you have done this, you will also
          need to ensure you have the line 'mousetrap start' in your local
          /etc/config/sysinit.<node> startup script, and to reboot your
          system to run on the new drivers.


Compiling the libraries
-----------------------

Once you have all the startup scripts configured and executed, you are
ready to begin compiling. Building all the MGL libraries in one fell
swoop is very easy. Simple change into the src directory below where you
have installed all the files and issue a dmake build. Ie:

 cd scitech/src
 dmake build

Using 'dmake build' will force build all the libraries, and will build all
the libraries with your selected compiler and using the currently
configured options. If any errors are encountered during the build, it will
stop and you can fix the errors and then restart the build from the
offending library with a simple 'dmake' (ie: the default target builds
for the selected compiler). You can also build each library from each
directory if you wish as well.

We have also created script files to build all libraries for your
selected compiler in both 'Debug' and 'Release' modes. To build the
debug libraries, go into the src directory and issue run the mkdebug
script files. Ie:

 cd scitech/src
 mkdebug

To build the release libraries, go into the src directory and issue run
the mkrelease script files. Ie:

 cd scitech/src
 mkrelease

NOTE: For DOS environments without long filenames, the command for release
builds is 'mkreleas' without the traling 'e', because 'mkrelease' is longer
than 8 characters. Both mkreleas.bat and mkrelease.bat files are included.

All debug libraries end up under the lib/debug directory tree, and all
release libraries end up under the lib/release directory tree. To link
your own programs with the debug libraries, you set the environment
variable CHECKED=1 (or set it on the dmake command line). This will tell
the dmake makefile scripts to build in CHECKED mode (add extra runtime
checks) as to get the libraries from the lib/debug directories. Otherwise
the libraries are pulled from the lib/release directory.


Compiling the sample programs
-----------------------------

Once you have all the libraries built, you can try to compile some of the
sample programs. To build the example programs, go into the
scitech/examples/mgl/samples directory and run dmake to build all the
example programs.


Linux Runtime Setup
-------------------

In order for the MGL event libraries to support the mouse in fullscreen
console modes, we need to directly interface to the installed mouse
driver. By default the MGL will run without any mouse support if the
mouse environment variables have not been set up. The MGL_MOUSEDRV
environment varaiable should be set to one of the following values,
to inform the MGL what type of mouse you have:

  MGL_MOUSEDRV =
    GPM             - Use the currently installed GPM driver
    Microsoft       - Microsoft and compatible serial mouse
    PS2             - Microsoft and compatible PS/2 mouse
    MouseSystems    - Mouse Systems mouse
    MMSeries        - MMSeries mouse
    Logitech        - Logitech mouse
    BusMouse        - Microsoft bus mouse
    MouseMan        - MouseMan mouse
    IntelliMouse    - Microsoft Intellimouse
    IntelliMousePS2 - Microsoft PS/2 Intellimouse

If you already have the standard Linux GPM driver installed, you can set it
to use this driver by default.

NOTE: If any developers are familiar with mouse programming, we need someone
      to modify the PM event library to auto-detect the installed mouse
      type. This will then eliminate the need for users to configure
      the mouse environment variables for MGL applications.


Using the SciTech Nucleus Preview drivers
-----------------------------------------

SciTech has been working hard on the next generation of SciTech Display
Doctor, which is based on our new binary portable SciTech Nucleus device
driver architecture. SciTech MGL understands how to use SciTech Nucleus
drivers on all supported operating systems, and we have made a preview
of SciTech Nucleus available for MGL developers. You will need to download
the nucleus.zip file from our ftp site, and email KendallB@scitechsoft.com
for the password.

Once you have unzipped the files, move the graphics.bpd file into the
\scitech\drivers directory. Then on your target system, you need to
ensure that MGL_ROOT is set to point to the directory where you have
installed the scitech files (ie: c:\scitech by default). Also make sure
that there is a \scitech\drivers\config directory to ensure that Nucleus
can function properly.


---- END OF README.TXT ----
