/****************************************************************************
*
*           		SciTech Nucleus Graphics Architecture
*
*               Copyright (C) 1991-1998 SciTech Software, Inc.
*                            All rights reserved.
*
*  ======================================================================
*  |REMOVAL OR MODIFICATION OF THIS HEADER IS STRICTLY PROHIBITED BY LAW|
*  |                                                                    |
*  |This copyrighted computer code contains proprietary technology      |
*  |owned by SciTech Software, Inc., located at 505 Wall Street,        |
*  |Chico, CA 95928 USA (http://www.scitechsoft.com).                   |
*  |                                                                    |
*  |The contents of this file are subject to the SciTech Nucleus        |
*  |License; you may *not* use this file or related software except in  |
*  |compliance with the License. You may obtain a copy of the License   |
*  |at http://www.scitechsoft.com/nucleus-license.txt                   |
*  |                                                                    |
*  |Software distributed under the License is distributed on an         |
*  |"AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or      |
*  |implied. See the License for the specific language governing        |
*  |rights and limitations under the License.                           |
*  |                                                                    |
*  |REMOVAL OR MODIFICATION OF THIS HEADER IS STRICTLY PROHIBITED BY LAW|
*  ======================================================================
*
* Language:     ANSI C
* Environment:  Any 32-bit protected mode environment
*
* Description:  Header file for the SciTech Nucleus Graphics Architecture
*				device driver Hardware Abstraction Layer, stereo specific
*				information.
*
****************************************************************************/

#ifndef __NUCLEUS_STEREO_H
#define __NUCLEUS_STEREO_H

/*---------------------- Macros and type definitions ----------------------*/

/****************************************************************************
REMARKS:
This enumeration defines the types of known LC shutter glasses supported
by Nucleus for software stereo support. The values for the COM or LPT port
that the glasses are connected to, are only included when specific glasses
types are selected that are known to be connected to a COM or LPT port. If
the user selects a custom I/O port configuration with gaGlassesIOPort,
it is expected the correct I/O port information is manually configured
in the GA_options structure.

HEADER:
nucleus/stereo.h

MEMBERS:
gaGlassesBlueCode		- Generic LC glasses that use the blue code system
gaGlassesLPTPort		- Generic LC glasses toggled by parallel port
gaGlassesCOMPort		- Generic LC glasses toggled by serial port
gaGlassesIOPort			- Generic LC glasses toggled by custom I/O port
gaGlassesVSync			- Generic LC glasses toggled by vSync modification
gaGlassesH3D			- H3D compatible LC glasses with special key frames
gaGlassesLineBlanking	- Generic LC glasses using external line blanking
gaGlassesSyncDouble		- Generic LC glasses using external sync doubler
gaGlassesFreeRunning	- Generic LC glasses are free running
gaGlassesHardwareSync	- Generic LC glasses connected to VESA stereo connector
gaGlassesMask			- Mask to find the glasses type
gaGlassesPortMask		- Mask to find the port the glasses are attached to
gaGlassesCOM1			- Indicates glasses are on serial port COM1
gaGlassesCOM2          	- Indicates glasses are on serial port COM2
gaGlassesCOM3          	- Indicates glasses are on serial port COM3
gaGlassesCOM4          	- Indicates glasses are on serial port COM4
gaGlassesLPT1			- Indicates glasses are on parallel port LPT1
gaGlassesLPT2          	- Indicates glasses are on parallel port LPT2
gaGlassesLPT3         	- Indicates glasses are on parallel port LPT3
****************************************************************************/
typedef enum {
	gaGlassesBlueCode,
	gaGlassesLPTPort,
	gaGlassesCOMPort,
	gaGlassesIOPort,
	gaGlassesVSync,
	gaGlassesH3D,
	gaGlassesLineBlanking,
	gaGlassesSyncDouble,
	gaGlassesFreeRunning,
	gaGlassesHardwareSync,
	gaGlassesMask			= 0x0FFF,
	gaGlassesPortMask		= 0xF000,
	gaGlassesCOM1			= 0x1000,
	gaGlassesCOM2			= 0x2000,
	gaGlassesCOM3			= 0x3000,
	gaGlassesCOM4			= 0x4000,
	gaGlassesLPT1			= 0x5000,
	gaGlassesLPT2			= 0x6000,
	gaGlassesLPT3			= 0x7000,
	} GA_glassesTypeFlags;

/****************************************************************************
REMARKS:
This enumeration defines the types of known LC shutter glasses by the
Stereo libraries. Note that these values here are not used internally
by Nucleus at all, but are the values stored in the GA_options structure
and used by the stereo control panel applet to configure the Nucleus
stereo parameters as necesary.

HEADER:
nucleus/stereo.h

MEMBERS:
gaGlasses_StereoGraphics_SimulEyes		- StereoGraphics SimulEyes LC glasses
gaGlasses_StereoGraphics_CrystalEyes	- StereoGraphics CrystalEyes LC glasses
gaGlasses_Wooboo_CyberBoy				- Wooboo CyberBoy LC glasses
gaGlasses_Chinon_CyberShades			- Chinon CyberShades LC glasses
gaGlasses_VRJoy							- VRJoy LC glasses 
gaGlasses_IArt_VRWorks					- I-Art VR Works LC glasses
gaGlasses_ISee3D_DVU2000I				- ISee3D DVU2000-I LC glasses
gaGlasses_VRex_VRSurfer					- VRex VRSurface LC glasses
gaGlasses_NuVision_3DSpex				- NuVision 3D-Spex LC glasses
gaGlasses_H3D_Compatible				- H3D compatible LC glasses
gaGlasses_Custom						- User defined custom LC glasses
gaHMD_First_HMD							- First head mounted display ID
gaHMD_Forte_VFX1						- Forte VFX1 Head Mounted Display
gaHMD_Custom							- User defined custom Head Mounted Display
gaProjector_First_Projector				- First stereo projector ID
gaProjector_Custom						- User defined custom stereo projector
gaGlasses_None							- Indicates the device is not configured
****************************************************************************/
typedef enum {
	gaGlasses_StereoGraphics_SimulEyes,
	gaGlasses_StereoGraphics_CrystalEyes,
	gaGlasses_Wooboo_CyberBoy,
	gaGlasses_Chinon_CyberShades,
	gaGlasses_VRJoy,
	gaGlasses_IArt_VRWorks,
    gaGlasses_ISee3D_DVU2000I,
	gaGlasses_VRex_VRSurfer,
	gaGlasses_NuVision_3DSpex,
	gaGlasses_H3D_Compatible,
    gaGlasses_Custom,
    gaHMD_First_HMD				= 0x1000,
	gaHMD_Forte_VFX1,
    gaHMD_Custom,
    gaProjector_First_Projector	= 0x2000,
    gaProjector_Custom,
    gaGlasses_None				= 0x1010101,
	} GA_stereoDeviceType;

/****************************************************************************
REMARKS:
Defines the type of fullscreen stereo mode supported by the end user system.
The modes are described below:

The gaStereoPageFlip mode is the preferred mode for fullscreen display modes,
and uses fullscreen page flipping with quad buffering. The system is put
into a high refresh rate non-interlaced display mode. Four display pages
are used, and each vertical retrace the screen flips between the two left
and right buffers while the application draws to the hidden left and right
buffers. In this mode all buffers are created by the application with the
full width and height of the primary surface, and one of those buffers
should be the primary surface. None of the buffers need to be attached in
a DirectDraw flip ring.

The gaStereoAboveBelow mode is an alterate mode for fullscreen display modes,
and uses one or two fullsize buffers for page flipping, and two half height
back buffers for the stereo images. In this mode a full size buffer is
created for the primary surface, which you may also optionally attach a
flipping back buffer to (if you want no tearing). The stereo buffers are
also created by the application but with the full width and half the height
of the primary surface. All rendering to the stereo buffers should be done
using double the aspect ratio of the primary surface. The stereo library will
automatically take care of blitting the stereo back buffers to the primary
surface and flipping the display if an attached back buffer is present.

The gaStereoSideBySide mode is an alterate mode for fullscreen display modes,
and uses one or two fullsize buffers for page flipping, and two half width
back buffers for the stereo images. In this mode a full size buffer is
created for the primary surface, which you may also optionally attach a
flipping back buffer to (if you want no tearing). The stereo buffers are
also created by the application but with half the width and the full height
of the primary surface. All rendering to the stereo buffers should be done
using half the aspect ratio of the primary surface. The stereo library will
automatically take care of blitting the stereo back buffers to the primary
surface and flipping the display if an attached back buffer is present.

The gaStereoInterleaved mode is a mode for fullscreen display modes.
The graphics controller is put into a low refresh rate non-interlaced
display mode, and the scanlines from the left and right buffers are
interleaved together in horizontal scanlines. In this mode two stereo back
buffers are created by the application with the full width and half the
height of the display mode. All rendering to the stereo buffers should be
done using double the aspect ratio of the primary surface. The stereo library
will automatically take care of interleaving the stereo buffers to the
primary surface.

The gaStereoInterlaced mode is a mode for fullscreen display modes.
The graphics controller is put into a high refresh rate interlaced display
mode, and the scanlines from the left and right buffers are interleaved
together in horizontal scanlines. In this mode two stereo back buffers are
created by the application with the full width and half the height of the
display mode. All rendering to the stereo buffers should be done using double
the aspect ratio of the primary surface. The stereo library will
automatically take care of interleaving the stereo buffers to the primary
surface.

The gaStereoDualDisplay mode is a mode that uses two display controllers.
One controller displays the left eye image, and another controller displays
the right eye image.

The gaStereoViewportFlip mode is a variation of the fullscreen page-flipped
display mode where the left and right image components are expected to be
rendered as viewports on the same surface. The graphics controller is put
into a high refresh rate non-interlaced display mode at half the vertical
resolution of the currently set display mode, and the upper and lower halves
of the display surface are automatically page-flipped. In this mode a single
stereo back buffer is created by the application with the full width and
height of the primary surface, and the left and right image components
are rendered into upper and lower viewports. All rendering to the stereo
viewports should be done using double the aspect ratio of the primary surface.

HEADER:
nucleus/stereo.h

MEMBERS:
gaStereoNone			- Stereo is not supported on this system
gaStereoPageFlip		- Fullscreen page flipped stereo
gaStereoAboveBelow		- Fullscreen above below format
gaStereoSideBySide		- Fullscreen side by side format
gaStereoInterleaved		- Fullscreen line interleaved stereo format
gaStereoInterlaced		- Fullscreen interlaced stereo format
gaStereoDualDisplay		- Fullscreen dual display stereo format
gaStereoViewportFlip	- Fullscreen viewport flipped stereo
****************************************************************************/
typedef enum {
	gaStereoNone,
	gaStereoPageFlip,
	gaStereoAboveBelow,
	gaStereoSideBySide,
	gaStereoInterleaved,
	gaStereoInterlaced,
	gaStereoDualDisplay,
	gaStereoViewportFlip,
	} GA_stereoModeType;

/****************************************************************************
REMARKS:
Defines the type of windowed stereo mode supported by the end user system.
The modes are described below:

The gaWinStereoInterlaced mode is a mode for displaying stereo in a window
on the desktop. The graphics controller is put into a high refresh rate
interlaced display mode, and the scanlines from the left and right buffers
are interleaved together in horizontal scanlines. In this mode two stereo
back buffers are created by the application with the full width and half the
height of the application window. All rendering to the stereo buffers should
be done using double the aspect ratio of the primary surface. The stereo
library will automatically take care of interleaving the stereo buffers
onto the application window surface.

The gaWinStereoInterleaved mode is a mode for displaying stereo in a window
on the desktop. The graphics controller is put into a user selectable
refresh rate non-interlaced display mode. The mode is basically the same
as gaWinStereoInterlaced, but interlaced mode is not used.

The gaWinStereoPageFlip mode is a mode for displaying stereo in a window
on the desktop. The graphics controller is put into a high refresh rate
non-interlaced display mode, and the stereo left and right buffers are
automatically composited into the application window every retrace interval.
In this mode four stereo back buffers are created by the application with
the full width and height of the application window. All rendering to the
stereo buffers should be done using the full aspect ratio of the primary
surface. The stereo library automatically takes care of blitting the buffers
to the application window every retrace.

HEADER:
nucleus/stereo.h

MEMBERS:
gaWinStereoNone				- Stereo is not supported on this system
gaWinStereoInterlaced		- Windowed interlaced stereo mode
gaWinStereoInterleaved		- Windowed interleaved stereo mode
gaWinStereoPageFlip			- Windowed page flipped stereo mode
****************************************************************************/
typedef enum {
	gaWinStereoNone,
	gaWinStereoInterlaced,
	gaWinStereoInterleaved,
	gaWinStereoPageFlip,
	} GA_winStereoModeType;

#endif  /* __NUCLEUS_STEREO_H */
