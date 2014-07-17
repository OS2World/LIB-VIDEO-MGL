/****************************************************************************
*
*					SciTech Multi-platform Graphics Library
*
*  ========================================================================
*
*    The contents of this file are subject to the SciTech MGL Public
*    License Version 1.0 (the "License"); you may not use this file
*    except in compliance with the License. You may obtain a copy of
*    the License at http://www.scitechsoft.com/mgl-license.txt
*
*    Software distributed under the License is distributed on an
*    "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
*    implied. See the License for the specific language governing
*    rights and limitations under the License.
*
*    The Original Code is Copyright (C) 1991-1998 SciTech Software, Inc.
*
*    The Initial Developer of the Original Code is SciTech Software, Inc.
*    All Rights Reserved.
*
*  ========================================================================
*
* Language:		ANSI C
* Environment:	Any
*
* Description:	Header file for the SciTech MGL.
*
****************************************************************************/

#ifndef	__MGRAPH_H
#define	__MGRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include "scitech.h"
#include "mgl/mglver.h"
#include "event.h"

/*---------------------- Macros and type definitions ----------------------*/

#pragma pack(4)

/* Define the version number for the MGL release */

#define MGL_VERSION_STR	\
	MGL_RELEASE_MAJOR_STR "." MGL_RELEASE_MINOR_STR

/* Define the calling conventions for all public MGL functions */

#define	MGLAPI	_ASMAPI			/* 'C' calling conventions for all 		*/
#define	MGLAPIP	_ASMAPIP		/* 'C' calling conventions for all		*/

/* Maximum number of display devices supported. This is a compile time
 * constant and can be changed. The MGL supports as many devices as you
 * can stuff into your machine, but it takes up global data space when
 * you include support for more devices. Hence we set it to a realistic
 * value here, but if you need more you can change this and recompile the
 * libraries.
 */

#define	MAX_DISPLAY_DEVICES		4

/* Enable mixed mode for multi-controller support */

#define	MM_MODE_MIXED			0x80

/****************************************************************************
REMARKS:
Defines the error codes returned by MGL_result. If a function fails for
any reason, you can check the error code return by MGL_result to determine
the cause of the failure, or display an error message to the user with
MGL_errorMsg.

HEADER:
mgraph.h

MEMBERS:
grOK            - No error
grNoInit        - Graphics driver has not been installed
grNotDetected   - Graphics hardware was not detected
grDriverNotFound- Graphics driver file not found
grBadDriver     - File loaded was not a graphics driver
grLoadMem       - Not enough memory to load graphics driver
grInvalidMode   - Invalid graphics mode for selected driver
grError         - General graphics error
grInvalidName   - Invalid driver name
grNoMem         - Not enough memory to perform operation
grNoModeSupport - Select video mode not supported by hard.
grInvalidFont   - Invalid font data
grBadFontFile   - File loaded was not a font file
grFontNotFound  - Font file was not found
grOldDriver     - Driver file is an old version
grInvalidDevice - Invalid device for selected operation
grInvalidDC     - Invalid device context
grInvalidCursor - Invalid cursor file
grCursorNotFound- Cursor file was not found
grInvalidIcon   - Invalid icon file
grIconNotFound  - Icon file was not found
grInvalidBitmap - Invalid bitmap file
grBitmapNotFound- Bitmap file was not found
grNewFontFile   - Only Windows 2.x font files supported
grNoDoubleBuff  - Double buffering is not available
grNoHardwareBlt - No hardware bitBlt for OffscreenDC
grNoOffscreenMem- No available offscreen memory
grInvalidPF     - Invalid pixel format for memory DC
grInvalidBuffer - Invalid offscreem buffer
grNoDisplayDC	- Display DC has not been created
grFailLoadRef2d	- 2D reference rasteriser driver failed to load
grLastError     - Last error number in list
****************************************************************************/
typedef enum {
    grOK            = 0,
    grNoInit        = -1,
    grNotDetected   = -2,
    grDriverNotFound= -3,
    grBadDriver     = -4,
    grLoadMem       = -5,
    grInvalidMode   = -6,
    grError         = -8,
    grInvalidName   = -9,
    grNoMem         = -10,
    grNoModeSupport = -11,
    grInvalidFont   = -12,
    grBadFontFile   = -13,
    grFontNotFound  = -14,
    grOldDriver     = -15,
    grInvalidDevice = -16,
    grInvalidDC     = -17,
    grInvalidCursor = -18,
    grCursorNotFound= -19,
    grInvalidIcon   = -20,
    grIconNotFound  = -21,
    grInvalidBitmap = -22,
    grBitmapNotFound= -23,
	grNewFontFile   = -25,
    grNoDoubleBuff  = -26,
    grNoHardwareBlt = -28,
    grNoOffscreenMem= -29,
	grInvalidPF     = -30,
	grInvalidBuffer	= -31,
	grNoDisplayDC	= -32,
	grFailLoadRef2d	= -33,
	grLastError     = -34
	} MGL_errorType;

/****************************************************************************
REMARKS:
Defines the background mode for monochrome bitmap and font drawing. By
default the MGL starts up in transparent mode, however you can change the
background mode to draw monochrome bitmaps with all 0 bits draw in the
background color instead of being transparent. This affects all monochrome
bitmap functions, including text rendering.

HEADER:
mgraph.h

MEMBERS:
MGL_OPAQUE_BACKGROUND		- Background for monochrome bitmaps is opaque,
							  and drawn in the current background color.
MGL_TRANSPARENT_BACKGROUND	- Background for monochrome bitmaps is transparent
****************************************************************************/
typedef enum {
	MGL_OPAQUE_BACKGROUND,
	MGL_TRANSPARENT_BACKGROUND
	} MGL_backModes;

/****************************************************************************
REMARKS:
Defines the MGL dithering modes, used when blitting RGB images to 8, 15
and 16 bits per pixel device context. If dithering is enabled, the blit
operation will dither the resulting image to produce the best quality. When
dithering is disabled, the blit operation uses the closest color which
has less quality but is faster.

Note:	The closest color method is fastest when the destination device
		context is a 15 or 16 bits per pixel bitmap. However when the
		destination is an 8 bits per pixel device context, the dithering
		mode will usually be faster.

Note:	Dithering is /on/ by default in the MGL.

HEADER:
mgraph.h
****************************************************************************/
typedef enum {
	MGL_DITHER_OFF,
	MGL_DITHER_ON
	} MGL_ditherModes;

/****************************************************************************
REMARKS:
Defines the MGL standard colors. This is the standard set of colors for the
IBM PC in DOS graphics modes. The default palette will have been programmed
to contain these values by the MGL when a graphics mode is started. If the
palette has been changed, the colors on the screen will not correspond to
the names defined here. Under a Windowing system (like Windows, OS/2 PM or
X Windows) these colors will not correspond to the default colors. For
Windows see the MGL_WIN_COLORS enumeration which defines the default colors
in a windowed mode.

HEADER:
mgraph.h
****************************************************************************/
typedef enum {
	MGL_BLACK,						/* dark colors	*/
	MGL_BLUE,
	MGL_GREEN,
	MGL_CYAN,
	MGL_RED,
	MGL_MAGENTA,
	MGL_BROWN,
	MGL_LIGHTGRAY,
	MGL_DARKGRAY,					/* light colors	*/
	MGL_LIGHTBLUE,
	MGL_LIGHTGREEN,
	MGL_LIGHTCYAN,
	MGL_LIGHTRED,
	MGL_LIGHTMAGENTA,
	MGL_YELLOW,
	MGL_WHITE
    } MGL_COLORS;

/****************************************************************************
REMARKS:
Defines the Windows standard colors for 256 color graohics modes when in
a window. 8,9,246,247 are reserved and you should not count on these colors
always being the same. For 16 color bitmaps, colors 248-255 are mapped to
colors 8-15.

HEADER:
mgraph.h
****************************************************************************/
typedef enum {
	MGL_WIN_BLACK 			= 0,
	MGL_WIN_DARKRED			= 1,
	MGL_WIN_DARKGREEN		= 2,
	MGL_WIN_DARKYELLOW		= 3,
	MGL_WIN_DARKBLUE		= 4,
	MGL_WIN_DARKMAGENTA		= 5,
	MGL_WIN_DARKCYAN		= 6,
	MGL_WIN_LIGHTGRAY		= 7,
	MGL_WIN_TURQUOISE		= 8,		/* Reserved; dont count on this	*/
	MGL_WIN_SKYBLUE			= 9,		/* Reserved; dont count on this	*/
	MGL_WIN_CREAM			= 246,		/* Reserved; dont count on this	*/
	MGL_WIN_MEDIUMGRAY		= 247,		/* Reserved; dont count on this	*/
	MGL_WIN_DARKGRAY		= 248,
	MGL_WIN_LIGHTRED		= 249,
	MGL_WIN_LIGHTGREEN		= 250,
	MGL_WIN_LIGHTYELLOW		= 251,
	MGL_WIN_LIGHTBLUE		= 252,
	MGL_WIN_LIGHTMAGENTA	= 253,
	MGL_WIN_LIGHTCYAN		= 254,
	MGL_WIN_WHITE 			= 255
	} MGL_WIN_COLORS;

/****************************************************************************
REMARKS:
Defines the logical write mode operation codes for all drawing functions.
The set of mix codes is the standard Microsoft Raster Operation (ROP2) codes
between two values. We define the MGL ROP2 codes as being between the source
and destination pixels for blt's, between the foreground or background color
and the destination pixels for solid and mono pattern fills and between the
pattern pixels and the destination pixels for color pattern fills.

HEADER:
mgraph.h

MEMBERS:
MGL_R2_BLACK        	- 0
MGL_R2_NOTMERGESRC  	- DSon
MGL_R2_MASKNOTSRC   	- DSna
MGL_R2_NOTCOPYSRC   	- Sn
MGL_R2_MASKSRCNOT   	- SDna
MGL_R2_NOT          	- Dn
MGL_R2_XORSRC       	- DSx
MGL_R2_NOTMASKSRC   	- DSan
MGL_R2_MASKSRC      	- DSa
MGL_R2_NOTXORSRC    	- DSxn
MGL_R2_NOP				- D
MGL_R2_MERGENOTSRC  	- DSno
MGL_R2_COPYSRC      	- S
MGL_R2_MERGESRCNOT  	- SDno
MGL_R2_MERGESRC     	- DSo
MGL_R2_WHITE   			- 1
MGL_REPLACE_MODE		- Replace mode
MGL_AND_MODE			- AND mode
MGL_OR_MODE				- OR mode
MGL_XOR_MODE			- XOR mode
****************************************************************************/
typedef enum {
	MGL_R2_BLACK,
	MGL_R2_NOTMERGESRC,
	MGL_R2_MASKNOTSRC,
	MGL_R2_NOTCOPYSRC,
	MGL_R2_MASKSRCNOT,
	MGL_R2_NOT,
	MGL_R2_XORSRC,
	MGL_R2_NOTMASKSRC,
	MGL_R2_MASKSRC,
	MGL_R2_NOTXORSRC,
	MGL_R2_NOP,
	MGL_R2_MERGENOTSRC,
	MGL_R2_COPYSRC,
	MGL_R2_MERGESRCNOT,
	MGL_R2_MERGESRC,
	MGL_R2_WHITE,
	MGL_REPLACE_MODE 	= MGL_R2_COPYSRC,
	MGL_AND_MODE		= MGL_R2_MASKSRC,
	MGL_OR_MODE			= MGL_R2_MERGESRC,
	MGL_XOR_MODE		= MGL_R2_XORSRC
	} MGL_writeModeType;

/****************************************************************************
REMARKS:
Defines the pen styles passed to MGL_setPenStyle. These styles define the
different fill styles that can be used when the filling the interior of
filled primitives and also the outline of non-filled primitives.

HEADER:
mgraph.h

MEMBERS
MGL_BITMAP_SOLID        - Fill with a solid color
MGL_BITMAP_OPAQUE       - Fill with an opaque bitmap pattern. Where bits in
                          the pattern are a 1, the foreground color is used.
                          Where bits in the pattern are a 0, the background
                          color is used. The pattern itself is defined as an
                          8x8 monochrome bitmap.
MGL_BITMAP_TRANSPARENT  - Fill with a transparent bitmap pattern. Where bits
                          in the pattern are a 1, the foreground color is
                          used. Where bits in the pattern are a 0, the pixel
                          is left unmodified on the screen. The pattern itself
                          is defined as an 8x8 monochrome bitmap.
MGL_PIXMAP              - Fill with a color pixmap pattern. The pixmap pattern
                          is defined as an 8x8 array of color_t values,
                          where each entry corresponds to the color values
                          packed for the appropriate color mode (ie: a color
						  index in color index modes and a packed RGB value in
                          HiColor and TrueColor modes).
MGL_PIXMAP_TRANSPARENT  - Fill with a color pixmap pattern, but with one of
						  the colors to be transparent. The transparent color
						  is set with a call to MGL_setPenPixmapTransparent.
****************************************************************************/
typedef enum {
	MGL_BITMAP_SOLID,
	MGL_BITMAP_OPAQUE,
	MGL_BITMAP_TRANSPARENT,
	MGL_PIXMAP,
	MGL_PIXMAP_TRANSPARENT
	} MGL_penStyleType;

/****************************************************************************
REMARKS:
Defines the line styles passed to MGL_setLineStyle.

HEADER:
mgraph.h

MEMBERS:
MGL_LINE_PENSTYLE   - Line drawn in current pen style
MGL_LINE_STIPPLE    - Line drawn with current line stipple pattern. The
					  line stipple pattern is a 16x1 pattern that defines
					  which pixels should be drawn in the line. Where a bit
					  is a 1 in the pattern, a pixel will be drawn and where
					  a bit is a 0 in the pattern, the pixel will be left
					  untouched on the screen.
****************************************************************************/
typedef enum {
	MGL_LINE_PENSTYLE,
	MGL_LINE_STIPPLE
	} MGL_lineStyleType;

/****************************************************************************
REMARKS:
Defines the polygon types passed to MGL_setPolygonType.

HEADER:
mgraph.h

MEMBERS:
MGL_CONVEX_POLYGON  - Monotone vertical polygon (includes convex polygons).
                      A monotone vertical polygon is one whereby there will
                      never be a horizontal line that can intersect the
                      polygon at more than two edges at a time. Note that
                      if you set the polygon type to this value and you
                      pass it a non-monotone vertical polygon, the output
                      results are undefined.
MGL_COMPLEX_POLYGON - Non-Simple polygons. When set to this mode the MGL
                      will correctly rasterise all polygon types that you
                      pass to it, however the drawing will be slower.
MGL_AUTO_POLYGON    - Auto detect the polygon type. In this mode the MGL
                      will examine the polygon vertices passed in and will
					  automatically draw it with the faster routines if it
                      is monotone vertical. Note that this does incur an
                      overhead for the checking code, so if you know all
                      your polygons are monotone vertical or convex, then
                      you should set the type to MGL_CONVEX_POLYGON.
****************************************************************************/
typedef enum {
    MGL_CONVEX_POLYGON,
    MGL_COMPLEX_POLYGON,
	MGL_AUTO_POLYGON
	} MGL_polygonType;

/****************************************************************************
REMARKS:
Defines the text justification styles passed to MGL_setTextJustify.

HEADER:
mgraph.h

MEMBERS:
MGL_LEFT_TEXT       - Justify from left
MGL_TOP_TEXT        - Justify from top
MGL_CENTER_TEXT     - Center the text
MGL_RIGHT_TEXT      - Justify from right
MGL_BOTTOM_TEXT     - Justify from bottom
MGL_BASELINE_TEXT   - Justify from the baseline
****************************************************************************/
typedef enum {
	MGL_LEFT_TEXT       =   0,
	MGL_TOP_TEXT        =   0,
	MGL_CENTER_TEXT     =   1,
	MGL_RIGHT_TEXT      =   2,
	MGL_BOTTOM_TEXT     =   2,
	MGL_BASELINE_TEXT   =   3
	} MGL_textJustType;

/****************************************************************************
REMARKS:
Defines the text direction styles passed to MGL_setTextDirection

HEADER:
mgraph.h

MEMBERS:
MGL_LEFT_DIR        - Text goes to left
MGL_UP_DIR          - Text goes up
MGL_RIGHT_DIR       - Text goes right
MGL_DOWN_DIR        - Text goes down
****************************************************************************/
typedef enum {
    MGL_LEFT_DIR        =   0,
    MGL_UP_DIR          =   1,
    MGL_RIGHT_DIR       =   2,
	MGL_DOWN_DIR        =   3
    } MGL_textDirType;

/****************************************************************************
REMARKS:
Defines the different font types

HEADER:
mgraph.h

MEMBERS:
MGL_VECTORFONT  	- Vector font
MGL_FIXEDFONT   	- Fixed width bitmap font
MGL_PROPFONT    	- Proportional width bitmap font
MGL_ANTIALIASEDFONT	- Flags that a font is anti-aliased
MGL_FONTTYPEMASK	- Mask for the font type flags
****************************************************************************/
typedef enum {
	MGL_VECTORFONT 		= 1,
	MGL_FIXEDFONT,
	MGL_PROPFONT,
	MGL_ANTIALIASEDFONT	= 0x80,
	MGL_FONTTYPEMASK	= 0x3F
	} MGL_fontType;

/****************************************************************************
REMARKS:
Defines the different font library types

HEADER:
mgraph.h

MEMBERS:
MGL_BITMAPFONT_LIB		- Bitmap font library (Windows 2.x style)
MGL_TRUETYPEFONT_LIB	- TrueType scalable font library
MGL_TYPE1FONT_LIB		- Adobe Type 1 scalable font library
****************************************************************************/
typedef enum {
	MGL_BITMAPFONT_LIB,
	MGL_TRUETYPEFONT_LIB,
	MGL_TYPE1FONT_LIB
	} MGL_fontLibType;

/****************************************************************************
REMARKS:
Defines the different palette rotation directions

HEADER:
mgraph.h

MEMBERS:
MGL_ROTATE_UP   - Rotate the palette values up
MGL_ROTATE_DOWN - Rotate the palette values down
****************************************************************************/
typedef enum {
    MGL_ROTATE_UP,
	MGL_ROTATE_DOWN
	} MGL_palRotateType;

/****************************************************************************
REMARKS:
Defines for waitVRT flag for MGL_setVisualPage, MGL_swapBuffers and
MGL_glSwapBuffers.

HEADER:
mgraph.h

MEMBERS:
MGL_tripleBuffer    - Triple buffer. This mode enables hardware or software
					  triple buffering if available on the target system.
					  In this case when triple buffering is available the
					  MGL will ensure that there is no flicker when flipping
					  pages, however your frame rate will run at the maximum
					  rate until you get to the physical refresh rate of
					  the screen (ie: 60fps or higher). Note that if there
					  is no hardware or software triple buffering available,
					  this function will work like regular double buffering.
					  Note also that you /must/ have at least 3 pages
					  available for triple buffering to work.
MGL_waitVRT         - Wait for vertical retrace. This mode always waits for
					  the vertical retrace when swapping display pages, and
					  is required if only have two pages available to avoid
					  flicker during animation.
MGL_dontWait        - Don't wait for retrace. This mode simply programs
					  the display start address change and returns. This may
					  cause flicker on the screen during animation, and is
					  mostly useful for debugging and testing purposes to
					  see what the raw framerate of an animation is. Also
					  if you dont have hardware or software triple available,
					  and you allocate at least 3 pages you can achieve the
					  effect of triple buffering if you know that the frame
					  rate of your animation will not exceed the refresh
					  rate of the screen.
****************************************************************************/
typedef enum {
	MGL_tripleBuffer    = 0,
	MGL_waitVRT         = 1,
	MGL_dontWait        = 2
	} MGL_waitVRTFlagType;

/****************************************************************************
REMARKS:
Defines the flags passed to MGL_setRefreshRate. You can pass the value
of MGL_DEFAULT_REFRESH to set the refresh rate to the adapter default.

HEADER:
mgraph.h

MEMBERS:
MGL_DEFAULT_REFRESH	- Use the default refresh rate for the graphics mode
MGL_INTERLACED_MODE	- Set the mode to be interlaced (not always supported)
****************************************************************************/
typedef enum {
	MGL_DEFAULT_REFRESH	= -1,
	MGL_INTERLACED_MODE	= 0x4000
	} MGL_refreshRateType;

/* Standard MGL device driver names */

#define	MGL_VBENAME			"vbe.drv"		/* SciTech VESA VBE driver 	*/
#define	MGL_NUCLEUSNAME		"nucleus.drv"	/* SciTech Nucleus driver 	*/
#define	MGL_DDRAWNAME		"ddraw.drv"		/* DirectDraw driver 		*/
#define	MGL_OPENGLNAME		"opengl.drv"	/* Hardware OpenGL driver 	*/
#define	MGL_GLDIRECTNAME	"gldirect.drv"	/* GLDirect OpenGL driver 	*/

/* Standard bitmap names */

#define	MGL_EMPTY_FILL	_MGL_getEmptyPat()
#define	MGL_GRAY_FILL	_MGL_getGrayPat()
#define	MGL_SOLID_FILL	_MGL_getSolidPat()

/****************************************************************************
REMARKS:
Defines the suspend application callback flags, passed to the suspend
application callback registered with the MGL. This callback is called
when the user presses one of the system key sequences indicating that
they wish to change the active application. The MGL will catch these
events and if you have registered a callback, will call the callback to
save the state of the application so that it can be properly restored
when the user switches back to your application. The MGL takes care of
all the details about saving and restoring the state of the hardware,
and all your application needs to do is save its own state so that you can
re-draw the application screen upon re-activation.

Note: Your application suspend callback may get called twice with the
      MGL_DEACTIVATE flag in order to test whether the switch should
      occur (under both DirectX and WinDirect fullscreen modes).

Note: When your callback is called with the MGL_DEACTIVATE flag, you
      cannot assume that you have access to the display memory surfaces
      as they may have been lost by the time your callback has been called.

HEADER:
mgraph.h

MEMBERS:
MGL_DEACTIVATE  - Application losing active focus
MGL_REACTIVATE  - Application regaining active focus
****************************************************************************/
typedef enum {
    MGL_DEACTIVATE  = 0x0001,
	MGL_REACTIVATE  = 0x0002
    } MGL_suspendAppFlagsType;

/****************************************************************************
REMARKS:
Defines the return codes that the application can return from the suspend
application callback registered with the MGL. The default value to be
returned is MGL_SUSPEND_APP and this will cause the application execution to
be suspended until the application is re-activated again by the user. During
this time the application will exist on the task bar under Windows 95 and
Windows NT in minimised form.

MGL_NO_DEACTIVATE signals to the MGL that the application does not want
to allow switching to occur, and the switch request will be ignored and
the app will remain in fullscreen mode. This is valid in both WinDirect
and DirectX modes under Windows 3.1 and Windows 95, and allows you to give
power users the option of disabling the ability to switch away from the
application back to the desktop. Note however that under Windows NT this
is not possible, so even if you return this under NT the switch will
still occur.

MGL_NO_SUSPEND_APP can be used to tell the MGL to switch back to the
Windows desktop, but not to suspend the applications execution. This must
be used with care as the suspend application callback is then responsible
for setting a flag in the application that will /stop/ the application from
doing any rendering directly to the framebuffer while the application is
minimised on the task bar (since the application no lonter owns the screen!).
This return value is most useful for networked games that need to maintain
network connectivity while the user has temporarily switched back to the
Windows desktop. Hence you can ensure that you main loop continues to run,
including networking and AI code, but no drawing occurs to the screen.

Note: The MGL ensures that your application will /never/ be switched away
      from outside of a message processing loop. Hence as long as you do
      not process messages inside your drawing loops, you will never lose
      the active focus (and your surfaces) while drawing, but only during
      event processing.

HEADER:
mgraph.h

MEMBERS:
MGL_NO_DEACTIVATE   - Don't allow app to be deactivated
MGL_SUSPEND_APP     - Suspend application execution until restored
MGL_NO_SUSPEND_APP  - Don't suspend execution, but allow switch
****************************************************************************/
typedef enum {
	MGL_NO_DEACTIVATE   = 0,
	MGL_SUSPEND_APP     = 1,
	MGL_NO_SUSPEND_APP  = 2
	} MGL_suspendAppCodesType;

/****************************************************************************
REMARKS:
Determines the type of blending used when drawing anti-aliased fonts.
Currently only two types, which are blended and non Blended. Used by
the MGL_setFontBlendMode function, and only affects TrueType or Adobe
Type 1 fonts loaded with anti-aliasing enabled.

HEADER:
mgraph.h

MEMBERS:
MGL_AA_NORMAL     - Four level antialiasing, between foreground and background.
MGL_AA_RGBBLEND   - Blends antialiasing with contents of screen. RGB only.
****************************************************************************/
typedef enum {
	MGL_AA_NORMAL,
	MGL_AA_RGBBLEND
	} MGL_fontBlendType;

/****************************************************************************
REMARKS:
Flags for 2D alpha blending functions supported by the SciTech MGL.
The values in here define the the alpha blending functions passed to
the MGL_setSrcBlendFunc and MGL_setDstBlendFunc functions. Essentially the
blend function defines how to combine the source and destination pixel
colors together to get the resulting destination color during rendering.
The formula used for this is defined as:

	DstColor = SrcColor * SrcFunc + DstColor * DstFunc;

If the source alpha blending function is set to MGL_BLEND_CONSTANTALPHA, the
/SrcFunc/ above becomes:

	SrcFunc = ConstAlpha

If the destination alpha blending function is set to
MGL_BLEND_ONEMINUSDSTALPHA then /DstFunc/ above becomes:

	DstFunc = (1-DstAlpha)

and the final equation becomes (note that each color channel is multiplied
individually):

	DstColor = SrcColor * ConstAlpha + DstColor * (1-DstAlpha)

Although the above is a completely contrived example, it does illustrate
how the functions defined below combine to allow you to build complex and
interesting blending functions. For simple source alpha transparency, the
following formula is usually used:

	DstColor = SrcColor * SrcAlpha + DstColor * (1-SrcAlpha)

For simple constant alpha transparency, the following formula is usually
used:

	DstColor = SrcColor * ConstantAlpha + DstColor * (1-ConstantAlpha)

Note:	All the above equations assume the color values and alpha values
		are in the range of 0 through 1 in floating point. In reality all
		blending is done with integer color and alpha components in the
		range of 0 to 255, when a value of 255 corresponds to a value of 1.0
		in the above equations.

Note:	The constant color value set by a call to MGL_setColor, and the
		constant alpha value set by a call to MGL_setAlphaValue.

Note:	Setting a blending function that uses the destination alpha
		components is only supported if the framebuffer currently supports
		destination alpha. Likewise setting a blending function that
		uses source alpha components is only supported if the framebuffer
		or incoming bitmap data contains an alpha channel. The results
		are undefined if these conditiions are not met.

Note:	Enabling source or destination alpha blending overrides the
		setting of the current write mode. Logical write modes and blending
		cannot be used at the same time.

HEADER:
mgraph.h

MEMBERS:
MGL_BLEND_NONE                  - No alpha blending
MGL_BLEND_ZERO                  - Blend factor is always zero
MGL_BLEND_ONE                   - Blend factor is always one
MGL_BLEND_SRCCOLOR              - Blend factor is source color
MGL_BLEND_ONEMINUSSRCCOLOR      - Blend factor is 1-source color
MGL_BLEND_SRCALPHA              - Blend factor is source alpha
MGL_BLEND_ONEMINUSSRCALPHA      - Blend factor is 1-source alpha
MGL_BLEND_DSTALPHA              - Blend factor is destination alpha
MGL_BLEND_ONEMINUSDSTALPHA      - Blend factor is 1-destination alpha
MGL_BLEND_DSTCOLOR              - Blend factor is destination color
MGL_BLEND_ONEMINUSDSTCOLOR      - Blend factor is 1-destination color
MGL_BLEND_SRCALPHASATURATE      - Blend factor is src alpha saturation
MGL_BLEND_CONSTANTCOLOR         - Blend factor is a constant color
MGL_BLEND_ONEMINUSCONSTANTCOLOR - Blend factor is 1-constant color
MGL_BLEND_CONSTANTALPHA			- Blend factor is constant alpha
MGL_BLEND_ONEMINUSCONSTANTALPHA	- Blend factor is 1-constant alpha
****************************************************************************/
typedef enum {
	MGL_BLEND_NONE,
	MGL_BLEND_ZERO,
	MGL_BLEND_ONE,
	MGL_BLEND_SRCCOLOR,
	MGL_BLEND_ONEMINUSSRCCOLOR,
	MGL_BLEND_SRCALPHA,
	MGL_BLEND_ONEMINUSSRCALPHA,
	MGL_BLEND_DSTALPHA,
	MGL_BLEND_ONEMINUSDSTALPHA,
	MGL_BLEND_DSTCOLOR,
	MGL_BLEND_ONEMINUSDSTCOLOR,
	MGL_BLEND_SRCALPHASATURATE,
	MGL_BLEND_CONSTANTCOLOR,
	MGL_BLEND_ONEMINUSCONSTANTCOLOR,
	MGL_BLEND_CONSTANTALPHA,
	MGL_BLEND_ONEMINUSCONSTANTALPHA
	} MGL_blendFuncType;

/****************************************************************************
REMARKS:
Defines the flags for the types of direct surface access provided.

HEADER:
mgraph.h

MEMBERS:
MGL_NO_ACCESS       - Surface cannot be accessed
MGL_VIRTUAL_ACCESS  - Surface is virtualised
MGL_LINEAR_ACCESS   - Surface can be linearly accessed
MGL_STEREO_ACCESS	- Surface supports stereo rendering
MGL_SHADOW_BUFFER	- Display is using a system memory shadow buffer
****************************************************************************/
typedef enum {
    MGL_NO_ACCESS       = 0x0,
    MGL_VIRTUAL_ACCESS  = 0x1,
    MGL_LINEAR_ACCESS   = 0x2,
	MGL_SURFACE_FLAGS   = 0x3,
	MGL_STEREO_ACCESS	= 0x4,
	MGL_SHADOW_BUFFER	= 0x8
    } MGL_surfaceAccessFlagsType;

/****************************************************************************
REMARKS:
Defines the flags returned by the MGL_modeFlags functions. This function
allows you to enumerate and detect support for different types of hardware
features for a specific graphics mode after calling MGL_detectGraph, but
before you actually initialise the desired mode. This will allow your
application to search for fullscreen graphics modes that have the features
that you desire (such as 2D or 3D acceleration).

HEADER:
mgraph.h

MEMBERS:
MGL_HAVE_LINEAR			- Graphics mode supports a hardware linear
						  framebuffer.
MGL_HAVE_REFRESH_CTRL	- Graphics mode supports refresh rate control,
						  allowing you to increase the refresh rate to
						  a desired value (such as high refresh rates for
						  stereo LC shutter glasses support).
MGL_HAVE_INTERLACED		- Graphics mode supports interlaced operation, and
						  you can request and interlaced mode via the
                          refresh rate control mechanism in the MGL.
MGL_HAVE_DOUBLE_SCAN	- Graphics mode supports double scan operation.
MGL_HAVE_TRIPLEBUFFER	- Graphics mode supports hardware triple buffering,
						  allowing your application to use true triple
						  buffering without any visible flickering.
MGL_HAVE_STEREO			- Graphics mode supports hardware stereo page
						  flipping, providing hardware support for stereo LC
						  shutter glasses.
MGL_HAVE_STEREO_DUAL	- Graphics mode supports hardware stereo page
						  flipping, with dual display start addresses.
MGL_HAVE_STEREO_HWSYNC	- Graphics mode provides hardware stereo sync support
						  via an external connector for stereo LC shutter
						  glasses.
MGL_HAVE_STEREO_EVCSYNC	- Graphics mode provides support for the EVC stereo
						  connector. If this bit is set, the above bit will
						  also be set.
MGL_HAVE_HWCURSOR		- Graphics mode supports a hardware cursor.
MGL_HAVE_ACCEL_2D		- Graphics mode supports 2D hardware acceleration.
						  2D acceleration may be provided either by WinDirect
                          and a VESA VBE/AF driver, or via DirectDraw.
MGL_HAVE_ACCEL_3D		- Graphics mode supports 3D hardware acceleration.
						  Hardware 3D acceleration is always provided in the
                          form of an OpenGL hardware driver of some form.
MGL_HAVE_ACCEL_VIDEO	- Graphics mode supports hardware video acceleration,
						  either via WinDirect and a VESA VBE/AF driver, or
						  via DirectDraw.
MGL_HAVE_VIDEO_XINTERP	- Graphics mode supports hardware video with
						  interpolation along the X axis.
MGL_HAVE_VIDEO_YINTERP	- Graphics mode supports hardware video with
						  interpolation along the Y axis.
****************************************************************************/
typedef enum {
	MGL_HAVE_LINEAR			= 0x00000001,
	MGL_HAVE_REFRESH_CTRL	= 0x00000002,
	MGL_HAVE_INTERLACED		= 0x00000004,
	MGL_HAVE_DOUBLE_SCAN	= 0x00000008,
	MGL_HAVE_TRIPLEBUFFER	= 0x00000010,
	MGL_HAVE_STEREO			= 0x00000020,
	MGL_HAVE_STEREO_DUAL	= 0x00000040,
	MGL_HAVE_STEREO_HWSYNC	= 0x00000080,
	MGL_HAVE_STEREO_EVCSYNC	= 0x00000100,
	MGL_HAVE_HWCURSOR		= 0x00000200,
	MGL_HAVE_ACCEL_2D		= 0x00000400,
	MGL_HAVE_ACCEL_3D		= 0x00000800,
	MGL_HAVE_ACCEL_VIDEO	= 0x00001000,
	MGL_HAVE_VIDEO_XINTERP	= 0x00002000,
	MGL_HAVE_VIDEO_YINTERP	= 0x00004000
	} MGL_modeFlagsType;

/****************************************************************************
REMARKS:
Defines the flags for the types of hardware acceleration supported by
the device context. This will allow the application to tailor the use of
MGL functions depending upon whether specific hardware support is
available. Hence applications can use specialised software rendering
support if the desired hardware support is not available on the end
user system.

Note: If the hardware flags are not MGL_HW_NONE, you /must/ call
	  the MGL_beginDirectAccess and MGL_endDirectAccess functions
	  before and after any custom code that does direct framebuffer
	  rendering!! This is not necessary for non-accelerated device
	  contexts, so you might want to optimise these calls out if
	  there is no hardware acceleration support.

HEADER:
mgraph.h

MEMBERS:
MGL_HW_NONE         	- No hardware acceleration
MGL_HW_LINE         	- Hardware line drawing
MGL_HW_STIPPLE_LINE 	- Hardware stippled line drawing
MGL_HW_POLY         	- Hardware polygon filling
MGL_HW_RECT         	- Hardware rectangle fill
MGL_HW_PATT_RECT    	- Hardware pattern rectangle fill
MGL_HW_CLRPATT_RECT 	- Hardware color pattern fill
MGL_HW_SYS_BLT      	- Hardware system->screen bitBlt
MGL_HW_SCR_BLT      	- Hardware screen->screen bitBlt
MGL_HW_SRCTRANS_BLT 	- Hardware source transparent blt
MGL_HW_DSTTRANS_BLT 	- Hardware dest. transparent blt
MGL_HW_SRCTRANS_SYS_BLT	- Hardware system->screen source transparent blt
MGL_HW_DSTTRANS_SYS_BLT	- Hardware system->screen destination transparent blt
MGL_HW_STRETCH_BLT 		- Hardware stretch blt
MGL_HW_STRETCH_SYS_BLT 	- Hardware system->screen stretch blt
MGL_HW_MONO_BLT     	- Hardware monochrome blt
****************************************************************************/
typedef enum {
	MGL_HW_NONE         	= 0x00000000,
	MGL_HW_LINE         	= 0x00000010,
	MGL_HW_STIPPLE_LINE 	= 0x00000020,
	MGL_HW_POLY         	= 0x00000040,
	MGL_HW_RECT         	= 0x00000080,
	MGL_HW_PATT_RECT    	= 0x00000100,
	MGL_HW_CLRPATT_RECT 	= 0x00000200,
	MGL_HW_SYS_BLT      	= 0x00000400,
	MGL_HW_SCR_BLT      	= 0x00000800,
	MGL_HW_SRCTRANS_BLT 	= 0x00001000,
	MGL_HW_DSTTRANS_BLT 	= 0x00002000,
	MGL_HW_SRCTRANS_SYS_BLT = 0x00004000,
	MGL_HW_DSTTRANS_SYS_BLT = 0x00008000,
	MGL_HW_STRETCH_BLT 		= 0x00010000,
	MGL_HW_STRETCH_SYS_BLT 	= 0x00020000,
	MGL_HW_MONO_BLT     	= 0x00040000,
	MGL_HW_FLAGS        	= 0x0007FFF0
	} MGL_hardwareFlagsType;

/****************************************************************************
REMARKS:
MGL_glSetOpenGL flags to select the OpenGL implementation.
In the AUTO mode we automatically determine which version of OpenGL to
use depending on the target runtime system. Unless there is hardware
acceleration available via the OS default OpenGL, we choose Silicon
Graphics OpenGL. If the Silicon Graphics OpenGL is not available, we fall
back on the Mesa software rendering libraries.

HEADER:
mgraph.h

MEMBERS:
MGL_GL_AUTO			- Automatically choose OpenGL implementation
MGL_GL_DEFAULT		- Force OS default OpenGL implementation (usually hardware)
MGL_GL_SGI			- Force SGI software OpenGL implementation
MGL_GL_MESA			- Force Mesa software OpenGL implementation
MGL_GL_GLDIRECT		- Force SciTech GLDirect OpenGL game driver implementation
MGL_GL_GLDIRECTCAD	- Force SciTech GLDirect OpenGL CAD driver implementation
****************************************************************************/
typedef enum {
	MGL_GL_AUTO,
	MGL_GL_DEFAULT,
	MGL_GL_SGI,
	MGL_GL_MESA,
	MGL_GL_GLDIRECT,
	MGL_GL_GLDIRECTCAD
	} MGL_glOpenGLType;

/****************************************************************************
REMARKS:
MGL_glCreateContext flags to initialize the pixel format used by the OpenGL
rendering context. If you pass in MGL_GL_VISUAL, the visual used will be
the one currently selected by the previous call to MGL_glSetVisual, and
provides the application programmer with complete control over the pixel
formats used.

You can pass in a combination of any of the other flags (ie:
MGL_GL_RGB | MGL_GL_DOUBLE | MGL_GL_DEPTH) to let the MGL know what you
want and to have it automatically select an appropriate visual for you.
This provides a quick and simple way to get application code up and
running.

HEADER:
mgraph.h

MEMBERS:
MGL_GL_VISUAL	- Use curently assigned visual from call to MGL_glSetVisual
MGL_GL_FORCEMEM	- Force system memory back buffer for all rendering
MGL_GL_RGB		- Select RGB rendering mode (/default/)
MGL_GL_INDEX	- Select color index display mode
MGL_GL_SINGLE	- Select single buffered display mode (/default/)
MGL_GL_DOUBLE	- Select double buffered display mode
MGL_GL_ACCUM	- Enable accumulation buffer (16 bits)
MGL_GL_ALPHA	- Enable alpha buffer (8 bit)
MGL_GL_DEPTH	- Enable depth buffer (16 bits)
MGL_GL_STENCIL	- Enable stencil buffer (8 bits)
MGL_GL_STEREO	- Enable stereo mode
****************************************************************************/
typedef enum {
	MGL_GL_VISUAL				= 0x8000,
	MGL_GL_FORCEMEM				= 0x4000,
	MGL_GL_RGB					= 0x0000,
	MGL_GL_INDEX				= 0x0001,
	MGL_GL_SINGLE				= 0x0000,
	MGL_GL_DOUBLE				= 0x0002,
	MGL_GL_ACCUM				= 0x0004,
	MGL_GL_ALPHA				= 0x0008,
	MGL_GL_DEPTH				= 0x0010,
	MGL_GL_STENCIL				= 0x0020,
	MGL_GL_STEREO              	= 0x0040
	} MGL_glContextFlagsType;

/****************************************************************************
REMARKS:
Defines the flags passed to MGL_setActivePage to let the MGL know which
buffer you wish to draw to when running in stereo mode (ie: after a
display device context created with MGL_createStereoDisplayDC). This
value is logical 'or'ed with the page parameter to MGL_setActivePage.

HEADER:
mgraph.h

MEMBERS:
MGL_LEFT_BUFFER		- Draw to the left buffer in stereo modes
MGL_RIGHT_BUFFER	- Draw to the right buffer in stereo modes
****************************************************************************/
typedef enum {
	MGL_LEFT_BUFFER				= 0x0000,
	MGL_RIGHT_BUFFER			= 0x8000
	} MGL_stereoBufType;

/****************************************************************************
REMARKS:
Structure describing an integer point passed to the MGL.

HEADER:
mgraph.h

MEMBERS:
x   - X coordinate for the point
y   - Y coordinate for the point
****************************************************************************/
typedef struct {
	int	x,y;
	} point_t;

/****************************************************************************
REMARKS:
Structure describing an integer rectangle. Note that MGL defines and uses
rectangles such that the bottom and right coordinates are not actually
included in the pixels that define a raster coordinate rectangle. This
allows for correct handling of overlapping rectangles without drawing any
pixels twice.

HEADER:
mgraph.h

MEMBERS:
left    - Left coordinate of the rectangle
top     - Top coordinate of the rectangle
right	- Right coordinate of the rectangle
bottom  - Bottom coordinate of the rectangle
****************************************************************************/
typedef struct {
	int	left;
	int	top;
	int	right;
	int	bottom;
	} rect_t;

/****************************************************************************
REMARKS:
Type definition for all color values used in MGL. All color values are 32
bits wide, and can be either a 4 or 8 bit color index, or a packed RGB tuple
depending on the pixel format for the display mode. For packed RGB display
modes, the colors may contain 15, 16, 24 or 32 bits of color information, and
the format of the RGB colors is stored in the pixel_format_t structure. You
should use the MGL_packColor family of functions to encode color values in
RGB modes, and use the MGL_unpackColor family of functions to extract color
values in RGB modes.

HEADER:
mgraph.h
****************************************************************************/
typedef ulong       color_t;

#ifndef __FX_FIXED_H
/****************************************************************************
REMARKS:
Type definition for all standard 32-bit fixed point values used in MGL.
Standard fixed point values are 32-bits wide, and represented in 16.16 fixed
point format (16 bits of integer, 16 bits of fraction). These numbers can
represent signed numbers from +32767.9 to -32768.9.

Note: If you are doing fixed point arithmetic for screen coordinate
	  calculations, be very careful of overflow conditions when doing
	  multiplication operations.

HEADER:
mgraph.h
****************************************************************************/
typedef long            fix32_t;
#else
#define fix32_t         FXFixed
#endif

/****************************************************************************
REMARKS:
Structure describing a 16.16 fixed point coordinate.

HEADER:
mgraph.h

MEMBERS:
x   - Fixed point x coordinate
y   - Fixed point y coordinate
****************************************************************************/
typedef struct {
	fix32_t x,y;
	} fxpoint_t;

/* Macros to convert between integer and 32 bit fixed point format */

#define	MGL_FIX_1			0x10000L
#define	MGL_FIX_2			0x20000L
#define	MGL_FIX_HALF		0x08000L
#define	MGL_TOFIX(i)		((long)(i) << 16)
#define MGL_FIXTOINT(f)		((int)((f) >> 16))
#define	MGL_FIXROUND(f) 	((int)(((f) + MGL_FIX_HALF) >> 16))

/****************************************************************************
REMARKS:
Structure representing a segment within a span that forms a complex region.
The segments define the X coordinates of the segments that make up the
span. Segments are always in groups of two (start and end segment).

HEADER:
mgraph.h

MEMBERS:
next	- Next segment in span
x		- X coordinates of this segment
****************************************************************************/
typedef struct segment_t {
	struct segment_t	*next;
	int					x;
	} segment_t;

/****************************************************************************
REMARKS:
Structure representing a span within a complex region. A span is represented
as a list of segments that are included in the span.

HEADER:
mgraph.h

MEMBERS:
next	- Next span in region
seg		- Index of first segment in span
y		- Y coordinate of this span
****************************************************************************/
typedef struct span_t {
	struct span_t	*next;
	segment_t		*seg;
	int				y;
	} span_t;

/****************************************************************************
REMARKS:
Structure representing a complex region. Complex regions are used to
represent non-rectangular areas as unions of smaller rectangles (the
smallest being a single pixel). You can use complex regions to build
complex clipping regions for user interface library development (such as
the SciTech MegaVision Library which makes extensive use of the MGL's
region management functions).

HEADER:
mgraph.h

MEMBERS:
rect    - Bounding rectangle for the region. If the spans field below is NULL,
          then the region is a simple region and is composed of only a single
          rectangle. Note however that you can have a simple region that
          consists of only single rectangle in the span structure (usually
          after complex region arithmetic). You can use the
          MGL_isSimpleRegion function to determine if the region contains
          only a single rectangle.
spans   - Pointer to the internal region span structure.
****************************************************************************/
typedef struct {
    rect_t      rect;
    span_t      *spans;
	} region_t;

/****************************************************************************
REMARKS:
Structure representing a single color palette entry. Color palette entries
are used to build the color lookup tables for all the device contexts used
in the MGL, which are used to define the final color values for colors in
color index modes (8-bits per pixel and below). Color palette information is
always stored in 8-bits per primary format (ie: 8-bits of red, green and blue
information), and will be converted by MGL to the appropriate color format
used by the underlying hardware when the hardware palette is programmed.
Hence in standard VGA modes (which only use 6-bits per primary) the bottom
two bits of color information will be lost when the palette is programmed.

HEADER:
mgraph.h

MEMBERS:
blue    - Blue channel color (0 - 255)
green   - Green channel color (0 - 255)
red     - Red channel color (0 - 255)
alpha   - Alignment value (not used and should always be 0)
****************************************************************************/
#pragma pack(1)
typedef struct {
    uchar   blue;
    uchar   green;
    uchar   red;
    uchar   alpha;
	} palette_t;

/****************************************************************************
REMARKS:
Structure representing a single extended color palette entry. Extended color
palette entries are similar to regular color palette entries, however
the extended color palette supports 16 bits per primary, allowing for higher
color resolution that a regular 8 bit primary color palette. The extended
color palette is used to build the color lookup tables for all the extended
color index device contexts used in the MGL.

HEADER:
mgraph.h

MEMBERS:
blue    - Blue channel color (0 - 65535)
green   - Green channel color (0 - 65535)
red     - Red channel color (0 - 65535)
****************************************************************************/
typedef struct {
	ushort	blue;
	ushort	green;
	ushort	red;
	} palette_ext_t;
#pragma pack(4)

/****************************************************************************
REMARKS:
Structure representing the format of an RGB pixel. This structure is used
to describe the current RGB pixel format used by all MGL device contexts
with pixel depths greater than or equal to 15-bits per pixel. The pixel
formats for 15 and 16-bit modes are constant and never change, however there
are 2 possible pixel formats for 24 bit RGB modes and 4 possible formats for
32 bit RGB modes that are supported by the MGL. The possible modes for
24-bits per pixel are:

24-bit		- Description
RGB			- Values are packed with Red in byte 2, Green in byte 1 and Blue
			  in byte 0. This is the standard format used by all 24 bit
			  Windows BMP files, and the native display format for most
			  graphics hardware on the PC.
BGR			- Values are packed with Blue in byte 2, Green in byte 1 and Red
			  in byte 0. This format is the native display format for some
			  graphics hardware on the PC.

The possible modes for 32-bits per pixel are:

32-bit		- Description
ARGB		- Values are packed with Red in byte 2, Green in byte 1 and Blue
			  in byte 0 and alpha in byte 3.
ABGR		- Values are packed with Blue in byte 2, Green in byte 1 and Red
			  in byte 0 and alpha in byte 3.
RGBA		- Values are packed with Red in byte 3, Green in byte 2 and Blue
			  in byte 1 and alpha in byte 0.
BGRA		- Values are packed with Blue in byte 3, Green in byte 2 and Red
              in byte 1 and alpha in byte 0.

If you intend to write your own direct rendering code for 15-bits per pixel
and higher graphics modes, you will need to write your code so that it will
adapt to the underlying pixel format used by the hardware to display the
correct colors on the screen. The MGL will perform pixel format translation
on the fly for MGL_bitBlt operations, but this can be time consuming. The
formula for packing the pixel data into the proper positions given three
8-bit RGB values is as follows:

	color = ((color_t)((R >> redAdjust) & redMask)
			<< redPos)
		  | ((color_t)((G >> greenAdjust) & greenMask)
			<< greenPos)
		  | ((color_t)((B >> blueAdjust) & blueMask)
		    << bluePos);

Alternatively you can unpack the color values with the following code:

	R = (((color) >> redPos) & redMask)
		<< redAdjust;
	G = (((color) >> greenPos) & greenMask)
		<< greenAdjust;
	B = (((color) >> bluePos) & blueMask)
	    << blueAdjust;

If you wish to create your own pixel formats (such as to create memory
custom bitmaps), the following list defines all the pixel formats that the
Nucleus knows how to deal with:

	{0x1F,0x0A,3, 0x1F,0x05,3, 0x1F,0x00,3, 0x01,0x0F,7}, // 555 15bpp
	{0x1F,0x0B,3, 0x3F,0x05,2, 0x1F,0x00,3, 0x00,0x00,0}, // 565 16bpp
	{0xFF,0x10,0, 0xFF,0x08,0, 0xFF,0x00,0, 0x00,0x00,0}, // RGB 24bpp
	{0xFF,0x00,0, 0xFF,0x08,0, 0xFF,0x10,0, 0x00,0x00,0}, // BGR 24bpp
	{0xFF,0x10,0, 0xFF,0x08,0, 0xFF,0x00,0, 0xFF,0x18,0}, // ARGB 32bpp
	{0xFF,0x00,0, 0xFF,0x08,0, 0xFF,0x10,0, 0xFF,0x18,0}, // ABGR 32bpp
	{0xFF,0x18,0, 0xFF,0x10,0, 0xFF,0x08,0, 0xFF,0x00,0}, // RGBA 32bpp
	{0xFF,0x08,0, 0xFF,0x10,0, 0xFF,0x18,0, 0xFF,0x00,0}, // BGRA 32bpp

Note: For 32-bit modes, the alpha channel information is unused, but should
      always be set to zero. Some hardware devices interpret the alpha
      channel information so unless you use a value of zero, you will get
      some strange looking results on the screen.

HEADER:
mgraph.h

MEMBERS:
redMask     - Unshifted 8-bit mask for the red color channel, and will be
              5-bits wide for a 5-bit color channel or 8-bits wide for an
              8-bit color channel.
redPos      - Bit position for bit 0 of the red color channel information.
redAdjust	- Number of bits to shift the 8 bit red value right
greenMask	- Unshifted 8-bit mask for the green color channel.
greenPos	- Bit position for bit 0 of the green color channel information.
greenAdjust	- Number of bits to shift the 8 bit green value right
blueMask	- Unshifted 8-bit mask for the blue color channel.
bluePos     - Bit position for bit 0 of the blue color channel information.
blueAdjust	- Number of bits to shift the 8 bit blue value right
rsvdMask	- Unshifted 8-bit mask for the reserved or alpha channel.
rsvdPost	- Bit position for bit 0 of the reserved channel information
rsvdAdjust  - Number of bits to shift the 32 bit reserved value right
****************************************************************************/
#pragma pack(1)
typedef struct {
	uchar	redMask;
	uchar	redPos;
	uchar	redAdjust;
	uchar	greenMask;
	uchar	greenPos;
	uchar	greenAdjust;
	uchar	blueMask;
	uchar	bluePos;
	uchar	blueAdjust;
	uchar	rsvdMask;
	uchar	rsvdPos;
	uchar	rsvdAdjust;
	} pixel_format_t;
#pragma pack(4)

/****************************************************************************
REMARKS:
Structure used to return elliptical arc starting and ending coordinates.
This structure is used to obtain the exact center, starting and ending
coordinates after an elliptical arc has been rasterized, so that you can
properly turn the arc into a pie slice for example.

HEADER:
mgraph.h

MEMBERS:
x       - x coordinate of the center of the elliptical arc
y       - y coordinate of the center of the elliptical arc
startX	- x coordinate of the starting pixel on the elliptical arc
startY	- y coordinate of the starting pixel on the elliptical arc
endX	- x coordinate of the ending pixel on the elliptical arc
endY    - y coordinate of the ending pixel on the elliptical arc
****************************************************************************/
typedef struct {
    int     x,y;
    int     startX,startY;
    int     endX,endY;
	} arc_coords_t;

/****************************************************************************
REMARKS:
Structure representing a monochrome or 2-color mouse cursor. The cursor is
defined as a 64x64 image with an AND and XOR mask. The definition of the AND
mask, XOR mask and the pixels that will appear on the screen is as follows
(same as the Microsoft Windows cursor format):

	AND XOR	Result
	0   0	Transparent (color from screen memory)
	0   1 	Invert (complement of color from screen memory)
	1   0 	Cursor background color
	1   1 	Cursor foreground color

The xHotSpot and yHotSpot members define the /hot-spot/ for the cursor,
which is the location where the logical mouse pointer is located in the
cursor image. When you click the mouse, the pixel under the hot-spot is the
pixel selected.

Note that Microsoft Windows cursors stored on disk are generally 32x32
in size, and the MGL will convert them during loading to the 64x64 internal
format. The MGL does however support loading 64x64 cursors, but you may need
to manually create these yourself as Windows resource editors appear to be
hard coded to use 32x32 cursors.

HEADER:
mgraph.h

MEMBERS:
colors		- Indicates the number of colors in the cursor (2 in this case)
xorMask     - 64x64 bit XOR pixel mask
andMask     - 64x64 bit AND pixel mask (see note above)
xHotSpot    - x coordinate of the mouse hotspot location. The mouse hotspot
			  location is used to properly align the mouse cursor image to
			  the actual mouse cursor position on the screen
yHotSpot    - y coordinate of the mouse hotspot location
****************************************************************************/
typedef struct {
	ulong		colors;
	uchar		xorMask[512];
	uchar		andMask[512];
	ulong		xHotSpot;
	ulong		yHotSpot;
	} mono_cursor_t;

/****************************************************************************
REMARKS:
Structure representing a 16-color color mouse cursor. The cursor is
defined as a 64x64 image with an AND and XOR mask. The cursor is defined as
a 64x64 image with an AND mask and color data. The definition of the AND
mask, cursor data and the pixels that will appear on the screen is as
follows:

	AND Color	Result
	0   00		Transparent (color from screen memory)
	0   0F 		Invert (complement of color from screen memory)
	1   xx 		Cursor color data

Hence if the AND mask is a zero the color data should be either 00 or 0F to
either make the pixel transparent or the inversion of the screen pixel. Any
other value will produce an undefined result.

The xHotSpot and yHotSpot members define the /hot-spot/ for the cursor,
which is the location where the logical mouse pointer is located in the
cursor image. When you click the mouse, the pixel under the hot-spot is the
pixel selected.

Note that Microsoft Windows cursors stored on disk are generally 32x32
in size, and the MGL will convert them during loading to the 64x64 internal
format. The MGL does however support loading 64x64 cursors, but you may need
to manually create these yourself as Windows resource editors appear to be
hard coded to use 32x32 cursors.

HEADER:
mgraph.h

MEMBERS:
colors		- Indicates the number of colors in the cursor (16 in this case)
colorData	- Cursor color data as a 64x64 array of packed 4-bit pixels
andMask		- Cursor AND mask
palette		- 16-color palette for cursor image
xHotSpot    - x coordinate of the mouse hotspot location. The mouse hotspot
			  location is used to properly align the mouse cursor image to
			  the actual mouse cursor position on the screen
yHotSpot    - y coordinate of the mouse hotspot location
****************************************************************************/
typedef struct {
	ulong		colors;
	uchar		colorData[2048];
	uchar		andMask[512];
	palette_t	palette[16];
	ulong		xHotSpot;
	ulong		yHotSpot;
	} color16_cursor_t;

/****************************************************************************
REMARKS:
Structure representing a 256-color color mouse cursor. The cursor is
defined as a 64x64 image with an AND and XOR mask. The cursor is defined as
a 64x64 image with an AND mask and color data. The definition of the AND
mask, cursor data and the pixels that will appear on the screen is as
follows:

	AND Color	Result
	0   00		Transparent (color from screen memory)
	0   FF 		Invert (complement of color from screen memory)
	1   xx 		Cursor color data

Hence if the AND mask is a zero the color data should be either 00 or FF to
either make the pixel transparent or the inversion of the screen pixel. Any
other value will produce an undefined result.

The xHotSpot and yHotSpot members define the /hot-spot/ for the cursor,
which is the location where the logical mouse pointer is located in the
cursor image. When you click the mouse, the pixel under the hot-spot is the
pixel selected.

Note that Microsoft Windows cursors stored on disk are generally 32x32
in size, and the MGL will convert them during loading to the 64x64 internal
format. The MGL does however support loading 64x64 cursors, but you may need
to manually create these yourself as Windows resource editors appear to be
hard coded to use 32x32 cursors.

HEADER:
mgraph.h

MEMBERS:
colors		- Indicates the number of colors in the cursor (256 in this case)
colorData	- Cursor color data as a 64x64 array of packed 8-bit pixels
andMask		- Cursor AND mask
palette		- 256-color palette for cursor image
xHotSpot    - x coordinate of the mouse hotspot location. The mouse hotspot
              location is used to properly align the mouse cursor image to
              the actual mouse cursor position on the screen
yHotSpot    - y coordinate of the mouse hotspot location
****************************************************************************/
typedef struct {
	ulong		colors;
	uchar		colorData[4096];
	uchar		andMask[512];
	palette_t	palette[256];
	ulong		xHotSpot;
	ulong		yHotSpot;
	} color256_cursor_t;

/****************************************************************************
REMARKS:
Hardware 32-bit RGBA alpha blended cursor structure. This structure defines
a color hardware cursor that is downloaded to the hardware. The cursor is
defined as a 64x64 32-bit RGBA image with alpha channel. The alpha channel
data is used to define the transparency level for the bitmap, with 0 being
fully transparent and 255 being full opaque. Since the color bitmap data
is alpha blended, the is no AND mask for the cursor image.

Structure representing a 32-bit RGBA alpha blended color mouse cursor. The
cursor is defined as a 64x64 32-bit RGBA image with alpha channel. The alpha
channel data is used to define the transparency level for the bitmap, with
0 being fully transparent and 255 being full opaque. Since the color bitmap
data is alpha blended, the is no AND mask for the cursor image.

The xHotSpot and yHotSpot members define the /hot-spot/ for the cursor,
which is the location where the logical mouse pointer is located in the
cursor image. When you click the mouse, the pixel under the hot-spot is the
pixel selected.

HEADER:
mgraph.h

MEMBERS:
colors		- Indicates the number of colors in the cursor (16,777,216)
colorData	- Cursor color data as a 64x64 array of packed 32-bit pixels
xHotSpot    - x coordinate of the mouse hotspot location. The mouse hotspot
              location is used to properly align the mouse cursor image to
              the actual mouse cursor position on the screen
yHotSpot    - y coordinate of the mouse hotspot location
****************************************************************************/
typedef struct {
	ulong		colors;
	ulong		bitsPerPixel;
	uchar		colorData[16384];
	ulong		xHotSpot;
	ulong		yHotSpot;
	} colorRGBA_cursor_t;

/****************************************************************************
REMARKS:
Structure representing a loaded mouse cursor. This is the structure of the
mouse cursor data after it has been loaded from disk by the MGL, and is used
to set the mouse cursor shape. You can build your own mouse cursors manually
by filling in this structure.

Note that this structure is actually a union of different cursor structures,
and you should examine the 'colors' member of the 'm' member to determine
the number of colors in the cursor, and then use the approproate union
member to access the cursor image data directly. The size of the cursor
image is dependent on the type of cursor stored in the structure.

HEADER:
mgraph.h

MEMBERS:
m		- Structure for monochrome or 2-color cursors
c16		- Structure for 16-color cursors
c256	- Structure for 256-color cursors
cRGBA	- Structure for RGBA alpha blended cursors
****************************************************************************/
typedef union {
	mono_cursor_t		m;
	color16_cursor_t	c16;
	color256_cursor_t	c256;
	colorRGBA_cursor_t	cRGBA;
	} cursor_t;

/****************************************************************************
REMARKS:
Structure representing a loaded lightweight bitmap image. This is the
structure of Windows .BMP files after they have been loaded from disk with
the MGL_loadBitmap function. Lightweight bitmaps have very little memory
overhead once loaded from disk, since the entire bitmap information is storeds
in a single contiguous block of memory (although this is not necessary;
see below). However the only thing you can do with a lightweight bitmap is
display it to any MGL device context, using either stretching or transparency
(MGL_putBitmap, MGL_stretchBitmap, MGL_putSrcBitmapTransparent). If you need
to be able to draw on the bitmap surface, then you should load the bitmap into
an MGL memory device context where you can call any of the standard MGL
drawing functions and BitBlt operations on the bitmap. The only disadvantage
of doing this is that a memory device context has a lot more memory overhead
involved in maintaining the device context information.

You can build you own lightweight bitmap loading routines by creating the
proper header information and loading the bitmap information into this
structure. Note that although the MGL loads the bitmap files from disk with
the bitmap surface, pixel format information and palette information all
loaded into a single memory block, this is not necessary. If you wish you
can create your own lightweight bitmaps with the bitmap surface allocated
in a separate memory block and then use this bitmap header to blast
information from this memory block to a device context as fast as possible.

HEADER:
mgraph.h

MEMBERS:
width           - Width of the bitmap in pixels
height          - Height of the bitmap in pixels
bitsPerPixel	- Pixel depth of the bitmap
bytesPerLine    - Scanline width for the bitmap. The scanline width must
                  always be aligned to a DWORD boundary, so the minimum
                  scanline width is 4 bytes.
surface         - Pointer to the bitmap surface.
pal             - Pointer to the bitmap palette. If this field is NULL, the
                  bitmap does not have an associated palette.
pf              - Pointer to the bitmap pixel format info. This field will be
                  NULL for all bitmaps with 8 or less bits per pixel, but
                  will always be properly filled in for bitmaps with 15 or
                  more bits per pixel.
****************************************************************************/
typedef struct {
    int         width;
    int         height;
	int         bitsPerPixel;
	int         bytesPerLine;
	void		*surface;
    palette_t   *pal;
    pixel_format_t *pf;

	/* ... palette, pixel format and bitmap data are store contiguously */
	} bitmap_t;

/****************************************************************************
REMARKS:
Structure representing a loaded icon. Icons are used by the MGL to display
small, transparent bitmap images that can be of any dimension. The standard
Windows .ICO files can store icons in 32x32 and 64x64 formats, although the
MGL can load icons of any dimensions if you can find an editor that will
allow you to create large icons.

Icons are always drawn by the MGL by first using the icon AND mask to punch
a hole in the background of the display surface, and then the icon bitmap
XOR mask is XOR'ed into the display surface. This method is compatible
with the way that Microsoft Windows displays icons on the screen.

HEADER:
mgraph.h

MEMBERS:
byteWidth   - Width of the monochrome AND mask in bytes. Must be consistent
              with the bitmap width in the xorMask structure.
andMask     - Pointer to the AND mask information, which is stored
              contiguously in memory after the header block. The dimensions
              of the AND mask is defined by the dimensions of the xorMask
              bitmap image.
xorMask     - Bitmap image header block, containing information about the
              mask used to draw the icon image. The actual bitmap surface
              and palette data is stored contiguously in memory after the
              header block.
****************************************************************************/
typedef struct {
    int         byteWidth;
    uchar       *andMask;
    bitmap_t    xorMask;

	/* ... AND mask and bitmap structure are stored contiguously */
	} icon_t;

/* Default cursor name */

#define	MGL_DEF_CURSOR		_MGL_getDefCursor()

/* Size of font name in font structure */

#define	_MGL_FNAMESIZE		58

/****************************************************************************
REMARKS:
Structure representing a loaded MGL font library. MGL font files come in
three flavors, either vector fonts, bitmap fonts or scaleable fonts. Vector
fonts represent the characters in the font as a set of lines that are drawn,
and vector fonts can be scaled and rotated to any desired angle. Vector
fonts however do not look very good when rasterized at high resolutions.
Bitmap fonts represent the characters in the font as small monochrome
bitmaps, and can be either fixed width fonts or proportional fonts. Scaleable
fonts (TrueType and Adobe Type 1) represent the characters as mathematical
outlines that can be scaled to any size. Scaleable fonts are scan converted
into bitmap fonts at runtime when a particular font point size is requested.
Scalable fonts can also be used for anti-aliased text rendering for better
looking fonts at lower resolutions.

The MGL can load both MGL 1.x style font files (vector and bitmap fonts) or
Windows 2.x style bitmap font files. For creating your own bitmap font files,
you should use any standard Windows font file editor and save the fonts in
Windows 2.x format (which is the standard format used by Windows 3.x,
Windows 95 and Windows NT for bitmap fonts).

HEADER:
mgraph.h

MEMBERS:
name        - Name of the font file (i.e. Helvetica Bold)
fontLibType	- Integer representing the type of font library file. Will be
			  one of the values defined by the MGL_fontLibType enumeration.
f			- File handle for font lib
ownHandle	- True if opened by MGL
dwOffset	- Offset within file
****************************************************************************/
typedef struct {
	char            name[_MGL_FNAMESIZE];
	short           fontLibType;
	FILE			*f;
	ibool			ownHandle;
	ulong			dwOffset;
	} font_lib_t;

/****************************************************************************
REMARKS:
Structure representing an MGL font ready for drawing. MGL fonts are loaded
from disk in two flavors, either vector fonts or bitmap fonts. Vector fonts
represent the characters in the font as a set of lines that are drawn, and
vector fonts can be scaled and rotated to any desired angle. Vector fonts
however do not look very good when rasterized at high resolutions. Bitmap
fonts represent the characters in the font as small monochrome bitmaps, and
can be either fixed width fonts or proportional fonts. Bitmap fonts are
also what are generated when you load a TrueType or Adobe Type 1 scalable
font library from disk.

HEADER:
mgraph.h

MEMBERS:
name        - Name of the font file (i.e. Helvetica Bold)
fontType    - Integer representing the type of font file. Will be one of
              the values defined by the MGL_fontType enumeration.
maxWidth	- Maximum character width in pixels
maxKern     - Maximum character kern value in pixels. The kern value for
              the character is the number of pixels it extends back past
              the character origin (such as the tail of the lowercase j
              character for some fonts).
fontWidth   - Font character width. This is the standard character width
              for the font, which may actually be smaller than the maximum
              width.
fontHeight  - Standard height of the font (not including the leading value).
ascent      - Font ascent value. The ascent value is the number of pixels
              that the font extends up from the font's baseline.
descent     - Font descent value. The descent value is the number of pixels
              that the font extends down from the font's baseline.
leading     - Font leading value. The leading value is the number of
              vertical pixels of space that are usually required between
			  two lines of text drawn with this font.
pointSize	- Actual point size for this font instance.
****************************************************************************/
#pragma pack(1)
typedef struct {
	char            name[_MGL_FNAMESIZE];
	short           fontType;
    short           maxWidth;
    short           maxKern;
    short           fontWidth;
    short           fontHeight;
    short           ascent;
    short           descent;
	short           leading;
	short           pointSize;
    } font_t;
#pragma pack(4)

/****************************************************************************
REMARKS:
Structure representing text metrics for a font or a single character, in the
current text attributes. For bitmap fonts you can get all the metric
information from the font_t structure, however for vector fonts, this routine
will provide the proper metrics for the font after being scaled by the
current font character scaling size. This structure is also used to obtain
specified 'tightest fit' metrics information about any character in the
font.

HEADER:
mgraph.h

MEMBERS:
width       - Actual width of the character in pixels
fontWidth   - Font character width, including any extra padding between this
              character and the next character. This value is used to advance
              the current position to the start of the next character, and
              can be larger than the actual character width (in order to put
              space between the characters).
fontHeight  - Standard height of the font (not including the leading value).
ascent      - Font or character ascent value. The ascent value is the number
              of pixels that the font extends up from the font's baseline.
descent     - Font or character descent value. The descent value is the
              number of pixels that the font extends down from the font's
              baseline.
leading     - Font leading value. The leading value is the number of
              vertical pixels of space that are usually required between
              two lines of text drawn with this font.
kern        - Character kern value. The kern value for the character is the
              number of pixels it extends back past the character origin
              (such as the tail of the lowercase j character for some fonts).
****************************************************************************/
typedef struct {
    int             width;
    int             fontWidth;
    int             fontHeight;
    int             ascent;
    int             descent;
    int             leading;
    int             kern;
	} metrics_t;

/****************************************************************************
REMARKS:
Structure representing the current text rasterizing settings. This structure
is used to group all these settings together in the MGL, and allows you to
save and restore the text rendering settings as a single unit.

HEADER:
mgraph.h

MEMBERS:
horizJust   - Horizontal justification value. Will be one of the values
              defined by the MGL_textJustType enumeration.
vertJust    - Vertical justification value. Will be one of the values
              defined by the MGL_textJustType enumeration.
dir         - Current text direction value. Will be one of the values
              defined by the MGL_textDirType enumeration.
szNumerx	- Current text x size numerator value
szNumery	- Current text y size numerator value
szDenomx	- Current text x size denominator value
szDenomy	- Current text y size denominator value
spaceExtra  - Current text space extra value. The space extra value is the
              number of extra pixels to be added to every space character
              when rendering the line of text.
font        - Pointer to current active font loaded in memory.
****************************************************************************/
typedef struct {
    int             horizJust;
    int             vertJust;
    int             dir;
    int             szNumerx;
    int             szNumery;
    int             szDenomx;
    int             szDenomy;
    int             spaceExtra;
    font_t          *font;
	} text_settings_t;

/* Macros to access the (left,top) and (right,bottom) points of a
 * rectangle.
 */

#define	MGL_leftTop(r)		(((point_t *) &(r))[0])
#define	MGL_rightBottom(r)	(((point_t *) &(r))[1])

/****************************************************************************
REMARKS:
Type definition for an 8x8 monochrome bitmap pattern. This is used to specify
the monochrome bitmap pattern used for filling solid objects when the pen
style is MGL_BITMAP_OPAQUE or MGL_BITMAP_TRANSPARENT.

When the pen style is MGL_BITMAP_OPAQUE, where bits in the pattern are a 1,
the foreground color is used. Where bits in the pattern are a 0, the
background color is used.

When the pen style is MGL_BITMAP_TRANSPARENT, where bits in the pattern are
a 1, the foreground color is used. Where bits in the pattern are a 0, the
pixel is left unmodified on the screen.

HEADER:
mgraph.h

MEMBERS:
p	- 8 bytes of pattern data
****************************************************************************/
typedef struct {
	uchar   p[8];
	} pattern_t;

/****************************************************************************
REMARKS:
Fundamental type definition for an array element of an 8x8 color bitmap
pattern data for 8bpp modes.

HEADER:
mgraph.h

MEMBERS:
p	- 8x8 bytes of pattern data
****************************************************************************/
typedef struct {
	uchar		p[8][8];
	} pixpattern8_t;

/****************************************************************************
REMARKS:
Fundamental type definition for an array element of an 8x8 color bitmap
pattern data for 16bpp modes.

HEADER:
mgraph.h

MEMBERS:
p	- 8x8 words of pattern data
****************************************************************************/
typedef struct {
	ushort		p[8][8];
	} pixpattern16_t;

/****************************************************************************
REMARKS:
Fundamental type definition for an array element of an 8x8 color bitmap
pattern data for 24bpp modes.

HEADER:
mgraph.h

MEMBERS:
p	- 8x8x3 bytes of pattern data
****************************************************************************/
typedef struct {
	uchar		p[8][8][3];
	} pixpattern24_t;

/****************************************************************************
REMARKS:
Fundamental type definition for an array element of an 8x8 color bitmap
pattern data for 32pp modes.

HEADER:
mgraph.h

MEMBERS:
p	- 8x8 dwords of pattern data
****************************************************************************/
typedef struct {
	ulong		p[8][8];
	} pixpattern32_t;

/****************************************************************************
REMARKS:
Type definition for an 8x8 color pixmap pattern. This is used to specify the
color pixmap pattern used for filling solid objects when the pen style is
in MGL_PIXMAP mode. The pixmap pattern is defined as an 8x8 array of
color values. Each line in the pattern is represented as an array of packed
pixel data. In 8bpp modes there is 8 bytes per line, for 16bpp modes
there are 16bytes per line, for 24bpp modes there are 24bytes per line
and for 32bpp modes there are 32 bytes per line. Hence the size of the
pattern data is different depending on the color depth currently active in
the MGL device context that the pattern is loaded into.

HEADER:
mgraph.h
****************************************************************************/
typedef union {
	pixpattern8_t	b8;
	pixpattern16_t	b16;
	pixpattern24_t	b24;
	pixpattern32_t	b32;
	} pixpattern_t;

/****************************************************************************
REMARKS:
Structure representing the current MGL rendering attributes. This structure
groups all of the MGL rendering state variables, and can be used to save
and restore the entire MGL rendering state for any device context as a single
unit.

Note: You should only save and restore the state to the same device context!

HEADER:
mgraph.h

MEMBERS:
color           - Current foreground color
backColor       - Current background color
aaColor      	- Palette for font anti-aliasing. Values corrospond to
				  25/75, 50/50, and 75/25 percent blend of background and
                  foreground colors respectively.
backMode		- Background color mode for monochrome bitmap expansion. Will
				  be either MGL_TRANSPARENT_BACKGROUND or
				  MGL_OPAQUE_BACKGROUND, and determins how background pixels
				  are drawn for monochrome bitmap rendering functions
				  (including text).
CP              - Current Position coordinate
writeMode       - Current write mode. Will be one of the values defined by the
                  MGL_writeModeType enumeration.
penStyle        - Current pen fill style. Will be one of values defined by the
                  MGL_penStyleType enumeration.
penHeight       - Current pen height
penWidth        - Current pen width
penPat          - Array of 8 pen 8x8 monochrome bitmap pattern
penPixPat       - Array of 8 pen 8x8 color pixmap pattern
cntPenPat       - Current pen 8x8 monochrome bitmap pattern index (0-7)
cntPenPixPat    - Current pen 8x8 color pixmap pattern index (0-7)
penPixPatTrans	- Current transparent color for pixmap patterns
lineStyle       - Current line style. Will be one of the values defined by the
                  MGL_lineStyleType enumeration.
lineStipple     - Current 16-bit line stipple mask.
stippleCount    - Current line stipple counter.
viewPort        - Current viewport. All coordinates are offset relative to the
                  current viewport when rendered, and output is always
                  clipped to the viewport boundaries.
viewPortOrg     - Current viewport logical origin. You can change the
				  logical viewport origin independently after the viewport
				  has been set. This effectively changes the logical
				  coordinate of the top left corner of the viewport from the
				  default of (0,0) to a new value (i.e. -10,-10).
clipRect        - Current clip rectangle, stored in local viewport
				  coordinates.
clipRegion      - Current clip region.  If clip region is a simple region
				  this will be NULL.
polyType        - Current polygon rasterizing type. Will be one of the values
				  defined by the MGL_polygonType enumeration.
fontBlendMode   - Type of blending used for anti-aliased fonts. Will be a
				  value from the enumeration MGL_fontBlendType.
srcBlendFunc	- Current src alpha blend function. Will be a value from
				  the enumeration MGL_blendFuncType.
dstBlendFunc	- Current dest alpha blend function. Will be a value from
				  the enumeration MGL_blendFuncType.
alphaValue		- Current constant alpha value between 0 and 255.
planeMask		- Current plane mask to determine which bits get updated.
ditherMode		- Current dither mode for blitting RGB bitmaps
ts              - Current text drawing attributes
****************************************************************************/
typedef struct {
    color_t         color;
	color_t         backColor;
	color_t         aaColor[5];
	int				backMode;
	color_t         bdrBright;
    color_t         bdrDark;
    point_t         CP;
    int             writeMode;
    int             penStyle;
    int             penHeight;
    int             penWidth;
	pattern_t       penPat[8];
	pixpattern_t    penPixPat[8];
	int				cntPenPat;
	int				cntPenPixPat;
	color_t			penPixPatTrans;
	int             lineStyle;
    uint            lineStipple;
    uint            stippleCount;
    rect_t          viewPort;
    point_t         viewPortOrg;
    rect_t          clipRect;
	region_t        *clipRegion;
	int             polyType;
	int             fontBlendMode;
	int				srcBlendFunc;
	int				dstBlendFunc;
	uint            alphaValue;
	ulong			planeMask;
	int				ditherMode;
	text_settings_t ts;
	} attributes_t;

/****************************************************************************
REMARKS:
Structure representing the attributes for a specific video mode. This
structure is also used to store the rendering dimentions for all device
context surfaces in the MGLDC structure.

HEADER:
mgraph.h

MEMBERS:
xRes                - Device x resolution - 1
yRes                - Device y resolution - 1
bitsPerPixel        - Pixel depth
maxColor            - Maximum color for device - 1
maxPage             - Maximum number of hardware display pages - 1
bytesPerLine        - Number of bytes in a single device scanline
aspectRatio         - Device pixel aspect ratio ((horiz/vert) * 1000)
pageSize            - Number of bytes in a hardware display page
scratch1            - Internal scratch value
scratch2            - Internal scratch value
redMaskSize         - Size of RGB red mask
redFieldPosition    - Number of bits in RGB red field
greenMaskSize       - Size of RGB green mask
greenFieldPosition	- Number of bits in RGB green field
blueMaskSize        - Size of RGB blue mask
blueFieldPosition	- Number of bits in RGB blue field
rsvdMaskSize        - Size of RGB reserved mask
rsvdFieldPosition   - Number of bits in RGB reserved field
modeFlags			- Flags for the mode
bitmapStartAlign	- Linear offscreen bitmap start alignment in bytes
bitmapStridePad		- Linear offscreen bitmap stride pad in bytes
****************************************************************************/
typedef struct {
    int     xRes;
    int     yRes;
	int     bitsPerPixel;
	color_t maxColor;
    int     maxPage;
    int     bytesPerLine;
    int     aspectRatio;
    long    pageSize;
    int     scratch1;
    int     scratch2;
    int     redMaskSize;
    int     redFieldPosition;
    int     greenMaskSize;
    int     greenFieldPosition;
    int     blueMaskSize;
    int     blueFieldPosition;
    int     rsvdMaskSize;
	int     rsvdFieldPosition;
	ulong	modeFlags;
	ulong	bitmapStartAlign;
	ulong 	bitmapStridePad;
	} gmode_t;

/****************************************************************************
REMARKS:
Structure representing the information passed to the MGL's OpenGL binding
functions to both choose an appropriate visual that is supported by the
OpenGL implementation and to pass in the information for the visual when
a rendering context is created. Application code will fill in the
structure and call MGL_glChooseVisual to find out a valid set of visual
information that is close to what they requested, then call MGL_glSetVisual
to make that the current visual for a specific MGL device context. The
the next call to MGL_glCreateContext will use the visual information
previously requested with the call to MGL_glSetVisual.

HEADER:
mgraph.h

MEMBERS:
rgb_flag        - True for an RGB mode, false for color index modes
alpha_flag      - True for alpha buffers (8-bits deep)
db_flag         - True for double buffered, false for single buffered
depth_size      - Size of depth buffer in bits
stencil_size    - Size of stencil buffer in bits
accum_size      - Size of accumulation buffer in bits
stereo_flag		- True for a stereo display false
****************************************************************************/
typedef struct {
    ibool       rgb_flag;
    ibool       alpha_flag;
    ibool       db_flag;
    int         depth_size;
    int         stencil_size;
    int         accum_size;
	ibool       stereo_flag;
	} MGLVisual;

/****************************************************************************
REMARKS:
Structure representing the public structure of all MGL device contexts.
You can use the information in this structure to directly access the
device surface for developing your own custom rendering code with the MGL.

HEADER:
mgraph.h

MEMBERS:
a           - Current device attributes
surface     - Pointer to device surface. This pointer will be a pointer to
			  the start of framebuffer memory for the device context, and
			  you can render directly to it as a single block of memory.
mi          - Mode attribute information for the device
pf          - Current pixel format for the device context.
colorTab    - Color lookup table cache for the device context. In color
			  index modes this is an array of 256 palette_t structures that
              represent the color palette for the device context. In
              15-bits per pixel and higher modes, this is an array of 256
              color_t values which contains a lookup table with pre-packed
              color values for the current display mode. This is used by
              the MGL when translating color index bitmaps and drawing
			  them in RGB display modes.
****************************************************************************/
typedef struct {
	attributes_t 	a;
	void    		*surface;
	gmode_t 		mi;
	pixel_format_t	pf;
	color_t 		*colorTab;

	/* Remainder of Device Context structure is private and internal	*/
#if 0
    } MGLDC;
#else
	} publicDevCtx_t;
#endif

#ifndef	BUILD_MGL
/* {secret} */
typedef publicDevCtx_t			MGLDC;
#else
/* {secret} */
struct internalDevCtx_t;
/* {secret} */
typedef struct internalDevCtx_t MGLDC;
#endif

/****************************************************************************
REMARKS:
Structure representing a lightweight bitmap buffer in offscreen display
memory. Offscreen buffers are used to store bitmap and sprite information
in the offscreen memory in the hardware, but are not full offscreen DC's
and hence have much less memory overhead than a full offscreen DC. Buffers
can only be used for storing bitmaps and blitting the around on the screen.
You can copy the contents to a MGL device context using the MGL_putBuffer,
MGL_stretchBuffer and MGL_putBufferTransparent functions. You can also
copy the contents of an MGL device context to a buffer using the
MGL_copyToBuffer function.

If you need to draw on a buffer in offscreen memory, create a full
offscreen device context instead, and then you can call any of the standard
MGL drawing functions and BitBlt operations for the offscreen memory buffer.
The primary disadvantage of doing this is that a full offscreen device
context has a lot more memory overhead involved in maintaining the
device context state information than a simple offscreen buffer.

Note:	The MGL automatically manages offscreen display memory, and if you
		run out of offscreen display memory it will place the buffer
		surfaces in system memory. Hence you should allocate your important
		buffers first, to ensure they end up in offscreen memory.

Note:	The MGL also manages the surface memory for the buffers, so that if
		your offscreen surfaces get lost (ie: on a fullscreen mode switch),
		they will automatically be restored when the application regains the
		active focus. This also allows the MGL to do automatic offscreen
		heap compaction when necessary to free up empty space on the
		heap.

HEADER:
mgraph.h

MEMBERS:
dc					- Display device context buffer is associated with
width           	- Width of the offscreen buffer in pixels
height          	- Height of the offscreen buffer in pixels
bytesPerLine    	- Scanline width for the offscreen buffer
cacheBytesPerLine	- Scanline width for the offscreen buffer cache
surfaceCache		- Pointer to surface cache in system memory (NULL if uncached)
surface         	- Pointer to start of the offscreen buffer surface
startX				- Starting X coordinate in framebuffer for buffer
startY				- Starting Y coordinate in framebuffer for buffer
linearStart			- Linear starting offset for buffer within framebuffer
handle				- OS specific buffer handle if necessary
****************************************************************************/
typedef struct {
	MGLDC		*dc;
	int         width;
    int         height;
	int         bytesPerLine;
	int         cacheBytesPerLine;
	void		*surfaceCache;
	void		*surface;
	int			startX;
	int			startY;
	long		linearStart;
	void		*handle;
	} MGLBUF;

/****************************************************************************
REMARKS:
Structure representing the set of file I/O functions that can be overridden
in the MGL. When you override the file I/O functions in the MGL, you must
provide a compatible function for each of the entries in this structure
that behave identically to the standard C library I/O functions of
similar names.

Note: Once you have overridden the file I/O functions, you can access the
      overriden functions from other libraries and DLL's by calling the
	  MGL_fopen family of functions, which are simply stubs to call the
	  currently overriden function via the function pointers.

HEADER:
mgraph.h

MEMBERS:
fopen   - Standard C fopen function replacement
fclose  - Standard C fclose function replacement
fseek   - Standard C fseek function replacement
ftell   - Standard C ftell function replacement
fread   - Standard C fread function replacement
fwrite  - Standard C fwrite function replacement
****************************************************************************/
typedef struct {
	FILE *	(*fopen)(const char *filename,const char *mode);
	int 	(*fclose)(FILE *f);
	int 	(*fseek)(FILE *f,long offset,int whence);
	long 	(*ftell)(FILE *f);
	size_t	(*fread)(void *ptr,size_t size,size_t n,FILE *f);
	size_t	(*fwrite)(const void *ptr,size_t size,size_t n,FILE *f);
	} fileio_t;

#pragma pack()

#ifdef	DECLARE_HANDLE
/* {secret} */
typedef HWND		MGL_HWND;
/* {secret} */
typedef HBITMAP		MGL_HBITMAP;
#else
#if defined (__QNX__) || !defined (__UNIX__)
/* {secret} */
typedef void		*MGL_HWND;
/* {secret} */
typedef void		*MGL_HBITMAP;
#else
#include "mglunix.h"
#endif
#endif

#ifdef	__cplusplus
extern "C" {			/* Use "C" linkage when in C++ mode	*/
#endif

/*------------------------- Function Prototypes ---------------------------*/

/*---------------------------------------------------------------------------
 * Routines bound to a specific device context. These routines all take
 * an MGLDC as a parmeter for the context to work with and hence dont work
 * with the current context. If however the context passed is the currently
 * active context, all changes to that context are reflected in the
 * currently active context as well.
 *-------------------------------------------------------------------------*/

/* Environment detection and initialisation */

int		MGLAPI MGL_init(const char *mglpath,const char *server);
MGLDC * MGLAPI MGL_quickInit(int xRes,int yRes,int bitsPerPixel,int numPages);
int		MGLAPI MGL_selectDisplayDevice(int device);
void	MGLAPI MGL_enableAllDrivers(void);
void	MGLAPI MGL_enableOpenGLDrivers(void);
int 	MGLAPI MGL_disableDriver(const char *name);
int		MGLAPI MGL_availablePages(int mode);
ibool	MGLAPI MGL_modeResolution(int mode,int *xRes,int *yRes,int *bitsPerPixel);
ulong	MGLAPI MGL_modeFlags(int mode);
int		MGLAPI MGL_findMode(int xRes,int yRes,int bitsPerPixel);
void 	MGLAPI MGL_exit(void);
void 	MGLAPI MGL_setBufSize(unsigned size);
void	MGLAPI MGL_fatalError(const char *msg, ...);
int 	MGLAPI MGL_result(void);
void	MGLAPI MGL_setResult(int result);
const char * MGLAPI MGL_errorMsg(int err);
const char * MGLAPI MGL_modeDriverName(int mode);

/* Device context management */

MGLDC	* MGLAPI MGL_createDisplayDC(int mode,int numBuffers,int refreshRate);
MGLDC	* MGLAPI MGL_createStereoDisplayDC(int mode,int numBuffers,int refreshRate);
MGLDC	* MGLAPI MGL_createScrollingDC(int mode,int virtualX,int virtualY,int numBuffers,int refreshRate);
MGLDC	* MGLAPI MGL_cloneDisplayDC(MGLDC *dc);
MGLDC	* MGLAPI MGL_createOverlayDC(MGLDC *dc,int format,int numBuffers,int left,int top,int right,int bottom);
MGLDC	* MGLAPI MGL_createOffscreenDC(MGLDC *dc,int width,int height);
MGLDC	* MGLAPI MGL_createWindowedDC(MGL_HWND hwnd);
MGLDC 	* MGLAPI MGL_createMemoryDC(int xSize,int ySize,int bitsPerPixel,pixel_format_t *pf);
MGLDC 	* MGLAPI MGL_createCustomDC(int xSize,int ySize,int bitsPerPixel,pixel_format_t *pf,int bytesPerLine,void *surface,MGL_HBITMAP hbm);
ibool	MGLAPI MGL_destroyDC(MGLDC *dc);
ibool	MGLAPI MGL_isDisplayDC(MGLDC *dc);
ibool	MGLAPI MGL_isOffscreenDC(MGLDC *dc);
ibool	MGLAPI MGL_isOverlayDC(MGLDC *dc);
ibool	MGLAPI MGL_isWindowedDC(MGLDC *dc);
ibool	MGLAPI MGL_isMemoryDC(MGLDC *dc);
ibool	MGLAPI MGL_isStereoDC(MGLDC *dc);
int		MGLAPI MGL_surfaceAccessType(MGLDC *dc);
long	MGLAPI MGL_getHardwareFlags(MGLDC *dc);

/* Lightweight offscreen buffer management */

MGLBUF	* MGLAPI MGL_createBuffer(MGLDC *dc,int width,int height,ibool cacheIt);
void	MGLAPI MGL_lockBuffer(MGLBUF *buf);
void	MGLAPI MGL_unlockBuffer(MGLBUF *buf);
void	MGLAPI MGL_destroyBuffer(MGLBUF *buf);

/* Stereo support */

void	MGLAPI MGL_startStereo(MGLDC *dc);
void	MGLAPI MGL_stopStereo(MGLDC *dc);
void	MGLAPI MGL_setBlueCodeIndex(int index);

/* MGL OpenGL binding functions */

ibool	MGLAPI MGL_glHaveHWOpenGL(void);
void	MGLAPI MGL_glSetOpenGLType(int type);
const char ** MGLAPI MGL_glEnumerateDrivers(void);
ibool	MGLAPI MGL_glSetDriver(const char *name);
void	MGLAPI MGL_glChooseVisual(MGLDC *dc,MGLVisual *visual);
ibool	MGLAPI MGL_glSetVisual(MGLDC *dc,MGLVisual *visual);
void	MGLAPI MGL_glGetVisual(MGLDC *dc,MGLVisual *visual);
ibool 	MGLAPI MGL_glCreateContext(MGLDC *dc,int flags);
void 	MGLAPI MGL_glMakeCurrent(MGLDC *dc);
void 	MGLAPI MGL_glDeleteContext(MGLDC *dc);
void	MGLAPI MGL_glSwapBuffers(MGLDC *dc,int waitVRT);
void	MGLAPI MGL_glResizeBuffers(MGLDC *dc);
void * 	MGLAPI MGL_glGetProcAddress(const char *procName);

/* Enable and disable MGL 2D functions with OpenGL surfaces. These functions
 * save and restore the state of the OpenGL rendering context across 2D MGL
 * functions. If you do a lot of 2D MGL drawing, you can bracket all calls with
 * these two functions which will speed up the drawing significantly since the
 * state will only be saved and restored once.
 */

void	MGLAPI MGL_glEnableMGLFuncs(MGLDC *dc);
void	MGLAPI MGL_glDisableMGLFuncs(MGLDC *dc);

/* OpenGL palette manipulation support. The reason we provide palette
 * manipulation routines specific for OpenGL support is so that when rendering
 * in double buffered modes with a system memory backbuffer, the palette for
 * the backbuffer is kept consistent with the hardware front buffer.
 */

void 	MGLAPI MGL_glSetPalette(MGLDC *dc,palette_t *pal,int numColors,int startIndex);
void 	MGLAPI MGL_glRealizePalette(MGLDC *dc,int numColors,int startIndex,ibool vSync);

/* Viewport and clip rectangle manipulation bound to a specific DC */

void 	MGLAPI MGL_setViewportDC(MGLDC *dc,rect_t view);
void	MGLAPI MGL_setRelViewportDC(MGLDC *dc,rect_t view);
void 	MGLAPI MGL_getViewportDC(MGLDC *dc,rect_t *view);
void 	MGLAPI MGL_setViewportOrgDC(MGLDC *dc,point_t org);
void	MGLAPI MGL_getViewportOrgDC(MGLDC *dc,point_t *org);
void	MGLAPI MGL_globalToLocalDC(MGLDC *dc,point_t *p);
void	MGLAPI MGL_localToGlobalDC(MGLDC *dc,point_t *p);
int 	MGLAPI MGL_maxxDC(MGLDC *dc);
int 	MGLAPI MGL_maxyDC(MGLDC *dc);
void 	MGLAPI MGL_setClipRectDC(MGLDC *dc,rect_t clip);
void 	MGLAPI MGL_getClipRectDC(MGLDC *dc,rect_t *clip);
void 	MGLAPI MGL_setClipRegionDC(MGLDC *dc,region_t *clip);
void	MGLAPI MGL_getClipRegionDC(MGLDC *dc,region_t *clip);

/* Color and palette manipulation */

color_t	MGLAPI MGL_realColor(MGLDC *dc,int color);
color_t	MGLAPI MGL_rgbColor(MGLDC *dc,uint R,uint G,uint B);
void 	MGLAPI MGL_setPaletteEntry(MGLDC *dc,int entry,uchar red,uchar green,uchar blue);
void 	MGLAPI MGL_getPaletteEntry(MGLDC *dc,int entry,uchar *red,uchar *green,uchar *blue);
void 	MGLAPI MGL_setPalette(MGLDC *dc,palette_t *pal,int numColors,int startIndex);
void 	MGLAPI MGL_getPalette(MGLDC *dc,palette_t *pal,int numColors,int startIndex);
void	MGLAPI MGL_rotatePalette(MGLDC *dc,int numColors,int startIndex,int direction);
ibool	MGLAPI MGL_fadePalette(MGLDC *dc,palette_t *fullIntensity,int numColors,int startIndex,uchar intensity);
void	MGLAPI MGL_realizePalette(MGLDC *dc,int numColors,int startIndex,ibool vSync);
int		MGLAPI MGL_getPaletteSize(MGLDC *dc);
void	MGLAPI MGL_getDefaultPalette(MGLDC *dc,palette_t *pal);
void 	MGLAPI MGL_setDefaultPalette(MGLDC *dc);
void	MGLAPI MGL_setPaletteSnowLevel(MGLDC *dc,int level);
int		MGLAPI MGL_getPaletteSnowLevel(MGLDC *dc);
ibool	MGLAPI MGL_checkIdentityPalette(ibool enable);
void	MGLAPI MGL_mapToPalette(MGLDC *dc,palette_t *pal);
ibool	MGLAPI MGL_setGammaRamp(MGLDC *dc,palette_ext_t *gamma,int num,int index,ibool vSync);
ibool	MGLAPI MGL_getGammaRamp(MGLDC *dc,palette_ext_t *gamma,int num,int index);

/* Generic device context information and manipulation */

ibool 	MGLAPI MGL_haveWidePalette(MGLDC *dc);
int		MGLAPI MGL_getBitsPerPixel(MGLDC *dc);
color_t	MGLAPI MGL_maxColor(MGLDC *dc);
int		MGLAPI MGL_maxPage(MGLDC *dc);
int 	MGLAPI MGL_sizex(MGLDC *dc);
int 	MGLAPI MGL_sizey(MGLDC *dc);
void 	MGLAPI MGL_getPixelFormat(MGLDC *dc,pixel_format_t *pf);
void	* MGLAPI MGL_computePixelAddr(MGLDC *dc,int x,int y);

/* Double buffering support */

void	MGLAPI MGL_setActivePage(MGLDC *dc,int page);
int		MGLAPI MGL_getActivePage(MGLDC *dc);
void	MGLAPI MGL_setVisualPage(MGLDC *dc,int page,int waitVRT);
int		MGLAPI MGL_getVisualPage(MGLDC *dc);
void	MGLAPI MGL_setDisplayStart(MGLDC *dc,int x,int y,int waitFlag);
void	MGLAPI MGL_getDisplayStart(MGLDC *dc,int *x,int *y);
ibool	MGLAPI MGL_vSync(MGLDC *dc);
int		MGLAPI MGL_isVSync(MGLDC *dc);
int		MGLAPI MGL_getCurrentScanLine(MGLDC *dc);
ibool	MGLAPI MGL_doubleBuffer(MGLDC *dc);
void	MGLAPI MGL_singleBuffer(MGLDC *dc);
void	MGLAPI MGL_swapBuffers(MGLDC *dc,int waitVRT);

/*---------------------------------------------------------------------------
 * Routines bound to the currently active context. All these routines work
 * with the currently active context and do not reflect any changes made
 * to the global context to the original user supplied context (because it
 * may be cached). The cached DC is automatically flushed back to the
 * original DC when a new context is enabled with MGL_makeCurrentDC().
 *
 * Before destroying a DC that is current, make sure you call
 * MGL_makeCurrentDC(NULL) first!
 *-------------------------------------------------------------------------*/

/* Routines to change the active global device context */

MGLDC *	MGLAPI MGL_makeCurrentDC(MGLDC *dc);
ibool	MGLAPI MGL_isCurrentDC(MGLDC *dc);

/* Current device context information and manipulation */

void 	MGLAPI MGL_defaultAttributes(MGLDC *dc);
int 	MGLAPI MGL_getAspectRatio(void);
void 	MGLAPI MGL_setAspectRatio(int aspectRatio);
void 	MGLAPI MGL_setColor(color_t color);
void	MGLAPI MGL_setColorRGB(uchar R,uchar G,uchar B);
void	MGLAPI MGL_setColorCI(int index);
color_t MGLAPI MGL_getColor(void);
void 	MGLAPI MGL_setBackColor(color_t color);
color_t MGLAPI MGL_getBackColor(void);
color_t	MGLAPI MGL_packColor(pixel_format_t *pf,uchar R,uchar G,uchar B);
color_t	MGLAPI MGL_packColorExt(pixel_format_t *pf,uchar A,uchar R,uchar G,uchar B);
void	MGLAPI MGL_unpackColor(pixel_format_t *pf,color_t color,uchar *R,uchar *G,uchar *B);
void	MGLAPI MGL_unpackColorExt(pixel_format_t *pf,color_t color,uchar *A,uchar *R,uchar *G,uchar *B);
color_t	MGLAPI MGL_defaultColor(void);
void 	MGLAPI MGL_setWriteMode(int mode);
int 	MGLAPI MGL_getWriteMode(void);
void 	MGLAPI MGL_setPenStyle(int style);
int 	MGLAPI MGL_getPenStyle(void);
void 	MGLAPI MGL_setLineStyle(int style);
int 	MGLAPI MGL_getLineStyle(void);
void 	MGLAPI MGL_setLineStipple(ushort stipple);
ushort	MGLAPI MGL_getLineStipple(void);
void 	MGLAPI MGL_setLineStippleCount(uint stippleCount);
uint	MGLAPI MGL_getLineStippleCount(void);
void 	MGLAPI MGL_setPenBitmapPattern(int index,const pattern_t *pat);
void 	MGLAPI MGL_usePenBitmapPattern(int index);
int		MGLAPI MGL_getPenBitmapPattern(int index,pattern_t *pat);
void 	MGLAPI MGL_setPenPixmapPattern(int index,const pixpattern_t *pat);
void 	MGLAPI MGL_usePenPixmapPattern(int index);
int		MGLAPI MGL_getPenPixmapPattern(int index,pixpattern_t *pat);
void 	MGLAPI MGL_setPenPixmapTransparent(color_t color);
color_t	MGLAPI MGL_getPenPixmapTransparent(void);
void	MGLAPI MGL_setPenSize(int height,int width);
void 	MGLAPI MGL_getPenSize(int *height,int *width);
void 	MGLAPI MGL_setBackMode(int mode);
int		MGLAPI MGL_getBackMode(void);
void	MGLAPI MGL_setPolygonType(int type);
int		MGLAPI MGL_getPolygonType(void);
void    MGLAPI MGL_setFontAntiAliasPalette(color_t colorfg, color_t color75, color_t color50, color_t color25, color_t colorbg);
void    MGLAPI MGL_getFontAntiAliasPalette(color_t *colorfg, color_t *color75, color_t *color50, color_t *color25, color_t *colorbg);
void    MGLAPI MGL_setFontBlendMode(int type);
int		MGLAPI MGL_getFontBlendMode(void);
void	MGLAPI MGL_setSrcBlendFunc(int blendFunc);
int 	MGLAPI MGL_getSrcBlendFunc(void);
void	MGLAPI MGL_setDstBlendFunc(int blendFunc);
int		MGLAPI MGL_getDstBlendFunc(void);
void	MGLAPI MGL_setAlphaValue(uchar alpha);
uchar	MGLAPI MGL_getAlphaValue(void);
void	MGLAPI MGL_setPlaneMask(ulong mask);
ulong	MGLAPI MGL_getPlaneMask(void);
void	MGLAPI MGL_setDitherMode(int mode);
int		MGLAPI MGL_getDitherMode(void);
void 	MGLAPI MGL_getAttributes(attributes_t *attr);
void 	MGLAPI MGL_restoreAttributes(attributes_t *attr);

/* Device clearing */

void	MGLAPI MGL_clearDevice(void);
void 	MGLAPI MGL_clearViewport(void);

/* Viewport and clip rectangle manipulation */

void 	MGLAPI MGL_setViewport(rect_t view);
void	MGLAPI MGL_setRelViewport(rect_t view);
void 	MGLAPI MGL_getViewport(rect_t *view);
void 	MGLAPI MGL_setViewportOrg(point_t org);
void	MGLAPI MGL_getViewportOrg(point_t *org);
void	MGLAPI MGL_globalToLocal(point_t *p);
void	MGLAPI MGL_localToGlobal(point_t *p);
int 	MGLAPI MGL_maxx(void);
int 	MGLAPI MGL_maxy(void);
void 	MGLAPI MGL_setClipRect(rect_t clip);
void 	MGLAPI MGL_getClipRect(rect_t *clip);
void 	MGLAPI MGL_setClipRegion(region_t *clip);
void	MGLAPI MGL_getClipRegion(region_t *clip);

/* Pixel plotting */

void 	MGLAPI MGL_pixelCoord(int x,int y);
color_t	MGLAPI MGL_getPixelCoord(int x,int y);
void	MGLAPI MGL_beginPixel(void);
void 	MGLAPI MGL_pixelCoordFast(int x,int y);
color_t	MGLAPI MGL_getPixelCoordFast(int x,int y);
void	MGLAPI MGL_endPixel(void);

/* Line drawing and clipping */

void 	MGLAPI MGL_moveToCoord(int x,int y);
void 	MGLAPI MGL_moveRelCoord(int dx,int dy);
void 	MGLAPI MGL_lineToCoord(int x,int y);
void 	MGLAPI MGL_lineRelCoord(int dx,int dy);
int 	MGLAPI MGL_getX(void);
int 	MGLAPI MGL_getY(void);
void	MGLAPI MGL_getCP(point_t* CP);
void 	MGLAPI MGL_lineCoordExt(int x1,int y1,int x2,int y2,ibool drawLast);
void    MGLAPI MGL_lineEngine(int x1,int y1,int x2,int y2,void (MGLAPIP plotPoint)(long x,long y));
void 	MGLAPI MGL_scanLine(int y,int x1,int x2);

/* Routines to begin/end direct framebuffer access. You must bracket all
 * code that draws directly to a device context surface with these
 * functions.
 */

void 	MGLAPI MGL_beginDirectAccess(void);
void 	MGLAPI MGL_endDirectAccess(void);

/* Function to flush the shadow buffer to the screen for the specified
 * coordinates. This is useful when the application is running with a
 * system memory shadow buffer, and has done direct surface access which
 * now needs to be flushed to the screen. All MGL functions automatically
 * handle this without any application programmer knowledge.
 */

void	MGLAPI MGL_flushShadowBuffer(int left,int top,int right,int bottom);

/* Polygon drawing: Note that the following Cnvx polygon routines
 * only work with convex polygons. The integer coordinate versions are
 * provided for compatibility only, and convert the coordinates to fixed
 * point and call the appropriate fixed point routines below.
 */

void	MGLAPI MGL_fillPolygon(int count,point_t *vArray,int xOffset,int yOffset);
void	MGLAPI MGL_fillPolygonCnvx(int count,point_t *vArray,int xOffset,int yOffset);
void    MGLAPI MGL_fillPolygonFX(int count,fxpoint_t *vArray,int vinc,fix32_t xOffset,fix32_t yOffset);
void    MGLAPI MGL_fillPolygonCnvxFX(int count,fxpoint_t *vArray,int vinc,fix32_t xOffset,fix32_t yOffset);

/* Polyline drawing */

void	MGLAPI MGL_polyPoint(int count,point_t *vArray);
void 	MGLAPI MGL_polyLine(int count,point_t *vArray);

/* Rectangle drawing */

void 	MGLAPI MGL_rectCoord(int left,int top,int right,int bottom);
void 	MGLAPI MGL_fillRectCoord(int left,int top,int right,int bottom);

/* Ellipse drawing */

void	MGLAPI MGL_ellipseArc(rect_t extentRect,int startAngle,int endAngle);
void	MGLAPI MGL_ellipseArcCoord(int x,int y,int xradius,int yradius,int startAngle,int endAngle);
void	MGLAPI MGL_getArcCoords(arc_coords_t *coords);
void	MGLAPI MGL_ellipse(rect_t extentRect);
void	MGLAPI MGL_ellipseCoord(int x,int y,int xradius,int yradius);
void	MGLAPI MGL_fillEllipseArc(rect_t extentRect,int startAngle,int endAngle);
void	MGLAPI MGL_fillEllipseArcCoord(int x,int y,int xradius,int yradius,int startAngle,int endAngle);
void	MGLAPI MGL_fillEllipse(rect_t extentRect);
void	MGLAPI MGL_fillEllipseCoord(int x,int y,int xradius,int yradius);
void	MGLAPI MGL_ellipseEngine(rect_t extentRect,void (MGLAPIP setup)(int topY,int botY,int left,int right),void (MGLAPIP set4pixels)(ibool inc_x,ibool inc_y,ibool region1),void (MGLAPIP finished)(void));
void 	MGLAPI MGL_ellipseArcEngine(rect_t extentRect,int startAngle,int endAngle,arc_coords_t *ac,void (MGLAPIP plotPoint)(long x,long y));

/* Text attribute manipulation */

void 	MGLAPI MGL_setTextJustify(int horiz,int vert);
void 	MGLAPI MGL_getTextJustify(int *horiz,int *vert);
void 	MGLAPI MGL_setTextDirection(int direction);
int   	MGLAPI MGL_getTextDirection(void);
void	MGLAPI MGL_setTextSize(int numerx,int denomx,int numery,int denomy);
void	MGLAPI MGL_getTextSize(int *numerx,int *denomx,int *numery,int *denomy);
void	MGLAPI MGL_setSpaceExtra(int extra);
int		MGLAPI MGL_getSpaceExtra(void);
void	MGLAPI MGL_setTextSettings(text_settings_t *settings);
void	MGLAPI MGL_getTextSettings(text_settings_t *settings);
int 	MGLAPI MGL_textHeight(void);
int 	MGLAPI MGL_textWidth(const char *str);
void	MGLAPI MGL_textBounds(int x,int y,const char *str,rect_t *bounds);
int		MGLAPI MGL_charWidth(char ch);
void	MGLAPI MGL_getFontMetrics(metrics_t *metrics);
void	MGLAPI MGL_getCharMetrics(char ch,metrics_t *metrics);
int		MGLAPI MGL_maxCharWidth(void);
void	MGLAPI MGL_underScoreLocation(int *x,int *y,const char *str);

/* Set and get the currently active font */

ibool	MGLAPI MGL_useFont(font_t *font);
font_t 	* MGLAPI MGL_getFont(void);

/* Text drawing */

void 	MGLAPI MGL_drawStr(const char *str);
void	MGLAPI MGL_drawStrXY(int x,int y,const char *str);
ibool	MGLAPI MGL_vecFontEngine(int x,int y,const char *str,void (MGLAPIP move)(int x,int y),void (MGLAPIP draw)(int x,int y));

/* Unicode Text Drawing */

void 	MGLAPI MGL_drawStr_W(const wchar_t *str);
void 	MGLAPI MGL_drawStrXY_W(int x,int y,const wchar_t *str);
int 	MGLAPI MGL_textWidth_W(const wchar_t *str);
void	MGLAPI MGL_textBounds_W(int x,int y,const wchar_t *str,rect_t *bounds);
int		MGLAPI MGL_charWidth_W(wchar_t ch);
void	MGLAPI MGL_getCharMetrics_W(wchar_t ch,metrics_t *metrics);
void	MGLAPI MGL_underScoreLocation_W(int *x,int *y,const wchar_t *str);

/* BitBlt support */

void 	MGLAPI MGL_bitBltCoord(MGLDC *dst,MGLDC *src,int left,int top,int right,int bottom,int dstLeft,int dstTop,int op);
void 	MGLAPI MGL_srcTransBltCoord(MGLDC *dst,MGLDC *src,int left,int top,int right,int bottom,int dstLeft,int dstTop,color_t transparent,int op);
void 	MGLAPI MGL_dstTransBltCoord(MGLDC *dst,MGLDC *src,int left,int top,int right,int bottom,int dstLeft,int dstTop,color_t transparent,int op);
void 	MGLAPI MGL_stretchBltCoord(MGLDC *dst,MGLDC *src,int left,int top,int right,int bottom,int dstLeft,int dstTop,int dstRight,int dstBottom,int op);
void 	MGLAPI MGL_getDivotCoord(MGLDC *dc,int left,int top,int right,int bottom,void *divot);
void 	MGLAPI MGL_putDivot(MGLDC *dc,void *divot);
long 	MGLAPI MGL_divotSizeCoord(MGLDC *dc,int left,int top,int right,int bottom);
void 	MGLAPI MGL_putMonoImage(MGLDC *dc,int x,int y,int width,int byteWidth,int height,void *image);
void	MGLAPI MGL_putBitmap(MGLDC *dc,int x,int y,const bitmap_t *bitmap,int op);
void	MGLAPI MGL_putBitmapSection(MGLDC *dc,int left,int top,int right,int bottom,int dstLeft,int dstTop,const bitmap_t *bitmap,int op);
void	MGLAPI MGL_putBitmapSrcTrans(MGLDC *dc,int x,int y,const bitmap_t *bitmap,color_t transparent,int op);
void	MGLAPI MGL_putBitmapSrcTransSection(MGLDC *dc,int left,int top,int right,int bottom,int dstLeft,int dstTop,const bitmap_t *bitmap,color_t transparent,int op);
void	MGLAPI MGL_putBitmapDstTrans(MGLDC *dc,int x,int y,const bitmap_t *bitmap,color_t transparent,int op);
void	MGLAPI MGL_putBitmapDstTransSection(MGLDC *dc,int left,int top,int right,int bottom,int dstLeft,int dstTop,const bitmap_t *bitmap,color_t transparent,int op);
void	MGLAPI MGL_putBitmapMask(MGLDC *dc,int x,int y,const bitmap_t *mask,color_t color,int op);
void	MGLAPI MGL_stretchBitmap(MGLDC *dc,int left,int top,int right,int bottom,const bitmap_t *bitmap,int op);
void	MGLAPI MGL_stretchBitmapSection(MGLDC *dc,int left,int top,int right,int bottom,int dstLeft,int dstTop,int dstRight,int dstBottom,const bitmap_t *bitmap,int op);
void	MGLAPI MGL_putIcon(MGLDC *dc,int x,int y,const icon_t *icon);

/* Offscreen buffer blitting support */

void	MGLAPI MGL_copyToBuffer(MGLDC *dc,int left,int top,int right,int bottom,int dstLeft,int dstTop,MGLBUF *buf);
void	MGLAPI MGL_copyBitmapToBuffer(bitmap_t *bitmap,int left,int top,int right,int bottom,int dstLeft,int dstTop,MGLBUF *buf);
void	MGLAPI MGL_putBuffer(MGLDC *dc,int x,int y,MGLBUF *buf,int op);
void	MGLAPI MGL_putBufferSection(MGLDC *dc,int left,int top,int right,int bottom,int dstLeft,int dstTop,MGLBUF *buf,int op);
void	MGLAPI MGL_putBufferSrcTrans(MGLDC *dc,int x,int y,MGLBUF *buf,color_t transparent,int op);
void	MGLAPI MGL_putBufferSrcTransSection(MGLDC *dc,int left,int top,int right,int bottom,int dstLeft,int dstTop,MGLBUF *buf,color_t transparent,int op);
void	MGLAPI MGL_putBufferDstTrans(MGLDC *dc,int x,int y,MGLBUF *buf,color_t transparent,int op);
void	MGLAPI MGL_putBufferDstTransSection(MGLDC *dc,int left,int top,int right,int bottom,int dstLeft,int dstTop,MGLBUF *buf,color_t transparent,int op);
void	MGLAPI MGL_stretchBuffer(MGLDC *dc,int left,int top,int right,int bottom,MGLBUF *buf,int op);
void	MGLAPI MGL_stretchBufferSection(MGLDC *dc,int left,int top,int right,int bottom,int dstLeft,int dstTop,int dstRight,int dstBottom,MGLBUF *buf,int op);

/* Monochrome bitmap manipulation */

void	MGLAPI MGL_drawGlyph(font_t *g,int x,int y,uchar glyph);
int		MGLAPI MGL_getGlyphWidth(font_t *font,uchar glyph);
int		MGLAPI MGL_getGlyphHeight(font_t *font);
void 	MGLAPI MGL_rotateGlyph(uchar *dst,uchar *src,int *byteWidth,int *height,int rotation);
void 	MGLAPI MGL_mirrorGlyph(uchar *dst,uchar *src,int byteWidth,int height);

/* Region management */

region_t * MGLAPI MGL_newRegion(void);
region_t * MGLAPI MGL_copyRegion(const region_t *s);
void	MGLAPI MGL_copyIntoRegion(region_t *d,const region_t *s);
void	MGLAPI MGL_clearRegion(region_t *r);
void 	MGLAPI MGL_freeRegion(region_t *r);
void 	MGLAPI MGL_drawRegion(int x,int y,const region_t *r);

/* Region generation primitives */

region_t * MGLAPI MGL_rgnLineCoord(int x1,int y1,int x2,int y2,const region_t *pen);
/*region_t * MGLAPI MGL_rgnPolygon(int count,point_t *vArray);*/
/*region_t * MGLAPI MGL_rgnPolygonCnvx(int count,point_t *vArray);*/
region_t * MGLAPI MGL_rgnSolidRectCoord(int left,int top,int right,int bottom);
region_t * MGLAPI MGL_rgnEllipse(rect_t extentRect,const region_t *pen);
region_t * MGLAPI MGL_rgnEllipseArc(rect_t extentRect,int startAngle,int endAngle,const region_t *pen);
void	MGLAPI MGL_rgnGetArcCoords(arc_coords_t *coords);
region_t * MGLAPI MGL_rgnSolidEllipse(rect_t extentRect);
region_t * MGLAPI MGL_rgnSolidEllipseArc(rect_t extentRect,int startAngle,int endAngle);

/* Region alegbra */

region_t * MGLAPI MGL_sectRegion(const region_t *r1,const region_t *r2);
region_t * MGLAPI MGL_sectRegionRect(const region_t *r1,const rect_t *r2);
ibool	MGLAPI MGL_unionRegion(region_t *r1,const region_t *r2);
ibool	MGLAPI MGL_unionRegionRect(region_t *r1,const rect_t *r2);
ibool 	MGLAPI MGL_unionRegionOfs(region_t *r1,const region_t *r2,int xOffset,int yOffset);
ibool	MGLAPI MGL_diffRegion(region_t *r1,const region_t *r2);
ibool	MGLAPI MGL_diffRegionRect(region_t *r1,const rect_t *r2);
void	MGLAPI MGL_optimizeRegion(region_t *r);
void 	MGLAPI MGL_offsetRegion(region_t *r,int dx,int dy);
ibool	MGLAPI MGL_emptyRegion(const region_t *r);
ibool	MGLAPI MGL_equalRegion(const region_t *r1,const region_t *r2);
ibool	MGLAPI MGL_ptInRegionCoord(int x,int y,const region_t *r);

/* Region traversal */

/* {partOf:MGL_traverseRegion} */
typedef void (MGLAPIP rgncallback_t)(const rect_t *r);
void 	MGLAPI MGL_traverseRegion(region_t *rgn,rgncallback_t doRect);

/* RGB to 8/15/16 bit halftone dithering routines */

void    MGLAPI MGL_getHalfTonePalette(palette_t *pal);
uchar	MGLAPI MGL_halfTonePixel(int x,int y,uchar R,uchar G,uchar B);
ushort 	MGLAPI MGL_halfTonePixel555(int x,int y,uchar R,uchar G,uchar B);
ushort 	MGLAPI MGL_halfTonePixel565(int x,int y,uchar R,uchar G,uchar B);

/* Unified font loading/unloading functions (all font types) */

void 	MGLAPI MGL_setDotsPerInch(int xDPI,int yDPI);
void 	MGLAPI MGL_setTTFCharacterMap(int platform,int encoding);
char *	MGLAPI MGL_availableFontLib(const char *libname);
font_lib_t * MGLAPI MGL_openFontLib(const char *libname);
font_lib_t * MGLAPI MGL_openFontLibExt(FILE *f,ulong dwOffset,ulong dwSize);
font_t * MGLAPI MGL_loadFontInstance(font_lib_t *lib,float pointSize,float slant,float angle,ibool antialiased);
void	MGLAPI MGL_unloadFontInstance(font_t *font);
void 	MGLAPI MGL_closeFontLib(font_lib_t *lib);

/* Obsolete compatibility bitmap font loading functions */

font_t * MGLAPI MGL_loadFont(const char *fontname);
font_t * MGLAPI MGL_loadFontExt(FILE *f,ulong dwOffset,ulong dwSize);
ibool	MGLAPI MGL_availableFont(const char *fontname);
void	MGLAPI MGL_unloadFont(font_t *font);

/* Mouse cursor resource loading/unloading */

cursor_t * MGLAPI MGL_loadCursor(const char *cursorName);
cursor_t * MGLAPI MGL_loadCursorExt(FILE *f,ulong dwOffset,ulong dwSize);
ibool	MGLAPI MGL_availableCursor(const char *cursorName);
void 	MGLAPI MGL_unloadCursor(cursor_t *cursor);

/* Icon resource loading/unloading */

icon_t * MGLAPI MGL_loadIcon(const char *iconName,ibool loadPalette);
icon_t * MGLAPI MGL_loadIconExt(FILE *f,ulong dwOffset,ulong dwSize,ibool loadPalette);
ibool	MGLAPI MGL_availableIcon(const char *iconName);
void	MGLAPI MGL_unloadIcon(icon_t *icon);

/* Windows BMP bitmap loading/unloading/saving */

bitmap_t * MGLAPI MGL_loadBitmap(const char *bitmapName,ibool loadPalette);
bitmap_t * MGLAPI MGL_loadBitmapExt(FILE *f,ulong dwOffset,ulong dwSize,ibool loadPalette);
ibool	MGLAPI MGL_availableBitmap(const char *bitmapName);
void	MGLAPI MGL_unloadBitmap(bitmap_t *bitmap);
ibool	MGLAPI MGL_getBitmapSize(const char *bitmapName,int *width,int *height,int *bitsPerPixel,pixel_format_t *pf);
ibool	MGLAPI MGL_getBitmapSizeExt(FILE *f,ulong dwOffset,ulong dwSize,int *width,int *height,int *bitsPerPixel,pixel_format_t *pf);
ibool	MGLAPI MGL_loadBitmapIntoDC(MGLDC *dc,const char *bitmapName,int dstLeft,int dstTop,ibool loadPalette);
ibool	MGLAPI MGL_loadBitmapIntoDCExt(MGLDC *dc,FILE *f,ulong dwOffset,ulong dwSize,int dstLeft,int dstTop,ibool loadPalette);
ibool	MGLAPI MGL_saveBitmapFromDC(MGLDC *dc,const char *bitmapName,int left,int top,int right,int bottom);
bitmap_t * MGLAPI MGL_getBitmapFromDC(MGLDC *dc,int left,int top,int right,int bottom,ibool savePalette);
bitmap_t * MGLAPI MGL_buildMonoMask(bitmap_t *bitmap,color_t transparent);

/* PCX bitmap loading/unloading/saving (1/4/8 bpp only) */

bitmap_t * MGLAPI MGL_loadPCX(const char *bitmapName,ibool loadPalette);
bitmap_t * MGLAPI MGL_loadPCXExt(FILE *f,ulong dwOffset,ulong dwSize,ibool loadPalette);
ibool	MGLAPI MGL_availablePCX(const char *bitmapName);
ibool	MGLAPI MGL_getPCXSize(const char *bitmapName,int *width,int *height,int *bitsPerPixel);
ibool	MGLAPI MGL_getPCXSizeExt(FILE *f,ulong dwOffset,ulong dwSize,int *width,int *height,int *bitsPerPixel);
ibool	MGLAPI MGL_loadPCXIntoDC(MGLDC *dc,const char *bitmapName,int dstLeft,int dstTop,ibool loadPalette);
ibool	MGLAPI MGL_loadPCXIntoDCExt(MGLDC *dc,FILE *f,ulong dwOffset,ulong dwSize,int dstLeft,int dstTop,ibool loadPalette);
ibool	MGLAPI MGL_savePCXFromDC(MGLDC *dc,const char *bitmapName,int left,int top,int right,int bottom);

/* JPEG bitmap loading/unloading/saving */

bitmap_t * MGLAPI MGL_loadJPEG(const char *JPEGName,int num8BitColors);
bitmap_t * MGLAPI MGL_loadJPEGExt(FILE *f,ulong dwOffset,ulong dwSize,int num8BitColors);
ibool MGLAPI MGL_availableJPEG(const char *JPEGName);
ibool MGLAPI MGL_getJPEGSize(const char *JPEGName,int *width,int *height,int *bitsPerPixel,pixel_format_t *pf);
ibool MGLAPI MGL_getJPEGSizeExt(FILE *f,ulong dwOffset,ulong dwSize,int *width,int *height,int *bitsPerPixel,pixel_format_t *pf);
ibool MGLAPI MGL_loadJPEGIntoDC(MGLDC *dc,const char *JPEGName,int dstLeft,int dstTop,int num8BitColors);
ibool MGLAPI MGL_loadJPEGIntoDCExt(MGLDC *dc,FILE *f,ulong dwOffset,ulong dwSize,int dstLeft,int dstTop,int num8BitColors);
ibool MGLAPI MGL_saveJPEGFromDC(MGLDC *dc,const char *JPEGName,int left,int top,int right,int bottom,int quality);

/* PNG bitmap loading/unloading/saving */

bitmap_t * MGLAPI MGL_loadPNG(const char *PNGName,ibool loadPalette);
bitmap_t * MGLAPI MGL_loadPNGExt(FILE *f,ulong dwOffset,ulong dwSize,ibool loadPalette);
ibool MGLAPI MGL_availablePNG(const char *PNGName);
ibool MGLAPI MGL_getPNGSize(const char *PNGName,int *width,int *height,int *bitsPerPixel,pixel_format_t *pf);
ibool MGLAPI MGL_getPNGSizeExt(FILE *f,ulong dwOffset,ulong dwSize,int *width,int *height,int *bitsPerPixel,pixel_format_t *pf);
ibool MGLAPI MGL_loadPNGIntoDC(MGLDC *dc,const char *PNGName,int dstLeft,int dstTop,ibool loadPalette);
ibool MGLAPI MGL_loadPNGIntoDCExt(MGLDC *dc,FILE *f,ulong dwOffset,ulong dwSize,int dstLeft,int dstTop,ibool loadPalette);
ibool MGLAPI MGL_savePNGFromDC(MGLDC *dc,const char *PNGName,int left,int top,int right,int bottom);

/* Random number generation routines for shorts and longs with full range */

void    MGLAPI MGL_srand(uint seed);
ushort	MGLAPI MGL_random(ushort max);
ulong	MGLAPI MGL_randoml(ulong max);

/* Mouse support */

void	MGLAPI MS_show(void);
void	MGLAPI MS_hide(void);
void	MGLAPI MS_obscure(void);
void	MGLAPI MS_setCursor(cursor_t *curs);
void	MGLAPI MS_setCursorColor(color_t foreColor);
void	MGLAPI MS_setCursorColorExt(color_t foreColor,color_t backColor);
void 	MGLAPI MS_moveTo(int x,int y);
void	MGLAPI MS_getPos(int *x,int *y);

/* Rectangle and Point manipulation */

rect_t 	MGLAPI MGL_defRect(int left,int top,int right,int bottom);
rect_t 	MGLAPI MGL_defRectPt(point_t leftTop,point_t rightBottom);
ibool 	MGLAPI MGL_sectRect(rect_t s1,rect_t s2,rect_t *d);
ibool 	MGLAPI MGL_sectRectCoord(int left1,int top1,int right1,int bottom1,int left2,int top2,int right2,int bottom2,rect_t *d);
void 	MGLAPI MGL_unionRect(rect_t s1,rect_t s2,rect_t *d);
void 	MGLAPI MGL_unionRectCoord(int left1,int top1,int right1,int bottom1,int left2,int top2,int right2,int bottom2,rect_t *d);

/* Built-in patterns and mouse cursor */

/* {secret} */
pattern_t	* MGLAPI _MGL_getEmptyPat(void);
/* {secret} */
pattern_t   * MGLAPI _MGL_getGrayPat(void);
/* {secret} */
pattern_t   * MGLAPI _MGL_getSolidPat(void);
/* {secret} */
cursor_t    * MGLAPI _MGL_getDefCursor(void);

/* Fixed point multiplication/divide routines */

#if !defined(__WATCOMC__)
fix32_t	MGLAPI MGL_FixMul(fix32_t a,fix32_t b);
fix32_t	MGLAPI MGL_FixDiv(fix32_t a,fix32_t b);
fix32_t	MGLAPI MGL_FixMulDiv(fix32_t a,fix32_t b,fix32_t c);
int		MGLAPI MGL_backfacing(fix32_t dx1,fix32_t dy1,fix32_t dx2,fix32_t dy2);
void 	MGLAPI MGL_memcpy(void *dst,void *src,int n);
#else
/* For Watcom C++ we can use special inline assembler code that is much
 * faster than calling the 386 assembler functions. Currently this is the
 * the only compiler that will allow inline assembler to be expanded
 * directly as inline functions.
 */

fix32_t	MGL_FixMul(fix32_t a,fix32_t b);
#pragma aux MGL_FixMul =			\
	"imul	edx"					\
	"add	eax,8000h"				\
	"adc	edx,0"					\
	"shrd	eax,edx,16"				\
	parm [eax] [edx]				\
	value [eax]						\
	modify exact [eax edx];

fix32_t	MGL_FixDiv(fix32_t a,fix32_t b);
#pragma aux MGL_FixDiv =			\
	"xor	eax,eax"				\
	"shrd	eax,edx,16"				\
	"sar	edx,16"					\
	"idiv	ebx"              		\
	parm [edx] [ebx] 				\
	value [eax]             		\
	modify exact [eax edx];

fix32_t	MGL_FixMulDiv(fix32_t a,fix32_t b,fix32_t c);
#pragma aux MGL_FixMulDiv =			\
	"imul	ebx"					\
	"idiv	ecx"              		\
	parm [eax] [ebx] [ecx] 			\
	value [eax]             		\
	modify exact [eax edx];

int	MGL_backfacing(fix32_t dx1,fix32_t dy1,fix32_t dx2,fix32_t dy2);
#pragma aux MGL_backfacing =		\
	"imul	ebx"					\
	"mov	ebx,eax"				\
	"mov	ecx,edx"				\
	"mov	eax,esi"				\
	"imul	edi"					\
	"sub	eax,ebx"				\
	"mov	eax,1"					\
	"sbb	edx,ecx"				\
	"jns	@@Backfacing"			\
	"xor	eax,eax"				\
	"@@Backfacing:"					\
	parm [eax] [esi] [edi] [ebx]	\
	value [eax]             		\
	modify exact [eax ecx edx];

void MGL_memcpy(void *dst,void *src,int n);
#pragma aux MGL_memcpy =            \
	"mov	eax,ecx"				\
	"shr    ecx,2"                  \
	"rep    movsd"                  \
	"mov	cl,al"					\
	"and	cl,3"					\
	"rep	movsb"					\
	parm [edi] [esi] [ecx]			\
	modify exact [eax ecx esi edi];
#endif

/* The following are special memcpy routines that properly handler reading
 * and writing to virtual linear buffer memory by forcing the proper
 * alignment. Note that the copy is extended to use a DWORD copy of speed.
 */

void 	MGLAPI MGL_memcpyVIRTSRC(void *dst,void *src,int n);
void 	MGLAPI MGL_memcpyVIRTDST(void *dst,void *src,int n);

/* Function to find an MGL system file's full pathname */

/* {secret} */
ibool	MGLAPI _MGL_findFile(char *validpath,const char *dir, const char *filename, const char *mode);

/* Override the internal MGL file I/O functions */

void	MGLAPI MGL_setFileIO(fileio_t *fio);

/* Functions to call the currently overriden file I/O functions */

FILE *	MGLAPI MGL_fopen(const char *filename,const char *mode);
int 	MGLAPI MGL_fclose(FILE *f);
int 	MGLAPI MGL_fseek(FILE *f,long offset,int whence);
long 	MGLAPI MGL_ftell(FILE *f);
size_t	MGLAPI MGL_fread(void *ptr,size_t size,size_t n,FILE *f);
size_t	MGLAPI MGL_fwrite(const void *ptr,size_t size,size_t n,FILE *f);

/*---------------------------------------------------------------------------
 * Memory allocation and utility functions.
 *-------------------------------------------------------------------------*/

void MGL_availableMemory(ulong *physical,ulong *total);
void MGL_useLocalMalloc(void * (*malloc)(size_t size),void (*free)(void *p));
void * MGLAPI MGL_malloc(long size);
void * MGLAPI MGL_calloc(long size,long n);
void MGLAPI MGL_free(void *p);
void MGLAPI MGL_memset(void *s,int c,long n);
void MGLAPI MGL_memsetw(void *s,int c,long n);
void MGLAPI MGL_memsetl(void *s,long c,long n);

/*---------------------------------------------------------------------------
 * Set a fullscreen suspend application callback function. This is used in
 * fullscreen video modes to allow switching back to the normal operating
 * system graphical shell (such as Windows GDI, OS/2 PM etc).
 *-------------------------------------------------------------------------*/

/* {partOf:MGL_setSuspendAppCallback} */
typedef int (MGLAPIP MGL_suspend_cb_t)(MGLDC *dc,int flags);
void	MGLAPI MGL_setSuspendAppCallback(MGL_suspend_cb_t staveState);

/* Internal function to disable linear blits */
void	MGLAPI MGL_useLinearBlit(ibool enable);

/*---------------------- Inline functions as Macros -----------------------*/

#define	MGL_equalPoint(p1,p2)	((p1).x == (p2).x && (p1).y == (p2).y)

#define	MGL_equalRect(r1,r2)	((r1).left == (r2).left &&			\
								 (r1).top == (r2).top &&			\
								 (r1).right == (r2).right &&		\
								 (r1).bottom == (r2).bottom)

#define	MGL_emptyRect(r)		((r).bottom <= (r).top || 			\
								 (r).right <= (r).left)

#define	MGL_disjointRect(r1,r2)	((r1).right <= (r2).left ||			\
								 (r1).left >= (r2).right ||			\
								 (r1).bottom <= (r2).top ||			\
								 (r1).top >= (r2).bottom)

#define	MGL_sectRect(s1,s2,d)										\
   ((d)->left = MAX((s1).left,(s2).left),                           \
	(d)->right = MIN((s1).right,(s2).right),                        \
	(d)->top = MAX((s1).top,(s2).top),                              \
	(d)->bottom = MIN((s1).bottom,(s2).bottom),                     \
	!MGL_emptyRect(*d))

#define	MGL_sectRectFast(s1,s2,d)									\
	(d)->left = MAX((s1).left,(s2).left);                           \
	(d)->right = MIN((s1).right,(s2).right);                        \
	(d)->top = MAX((s1).top,(s2).top);                              \
	(d)->bottom = MIN((s1).bottom,(s2).bottom)

#define	MGL_sectRectCoord(l1,t1,r1,b1,l2,t2,r2,b2,d)				\
   ((d)->left = MAX(l1,l2),											\
	(d)->right = MIN(r1,r2),										\
	(d)->top = MAX(t1,t2),											\
	(d)->bottom = MIN(b1,b2),										\
	!MGL_emptyRect(*d))

#define	MGL_sectRectFastCoord(l1,t1,r1,b1,l2,t2,r2,b2,d)			\
	(d)->left = MAX(l1,l2);											\
	(d)->right = MIN(r1,r2);										\
	(d)->top = MAX(t1,t2);											\
	(d)->bottom = MIN(b1,b2)

#define	MGL_unionRect(s1,s2,d)										\
	(d)->left = MIN((s1).left,(s2).left);                           \
	(d)->right = MAX((s1).right,(s2).right);                        \
	(d)->top = MIN((s1).top,(s2).top);                              \
	(d)->bottom = MAX((s1).bottom,(s2).bottom)

#define MGL_unionRectCoord(l1,t1,r1,b1,l2,t2,r2,b2,d)				\
	(d)->left = MIN(l1,l2);                                 		\
	(d)->right = MAX(r1,r2);                              			\
	(d)->top = MIN(t1,t2);                                    		\
	(d)->bottom = MAX(b1,b2)

#define	MGL_offsetRect(r,dx,dy)										\
	{	(r).left += dx; (r).right += dx;							\
		(r).top += dy; (r).bottom += dy; }

#define	MGL_insetRect(r,dx,dy)										\
	{	(r).left += dx; (r).right -= dx;							\
		(r).top += dy; (r).bottom -= dy;							\
		if (MGL_emptyRect(r))										\
			(r).left = (r).right = (r).top = (r).bottom = 0; }

#define	MGL_ptInRect(p,r)		((p).x >= (r).left &&				\
								 (p).x < (r).right &&				\
								 (p).y >= (r).top &&				\
								 (p).y < (r).bottom)

#define	MGL_ptInRectCoord(x,y,r)	((x) >= (r).left &&				\
									 (x) < (r).right &&				\
									 (y) >= (r).top &&				\
									 (y) < (r).bottom)

#define	MGL_ptInRegion(p,r)		MGL_ptInRegionCoord((p).x,(p).y,r)

#define	MGL_pixel(p)			MGL_pixelCoord((p).x,(p).y)
#define	MGL_getPixel(p)			MGL_getPixelCoord((p).x,(p).y)
#define	MGL_pixelFast(p)		MGL_pixelCoordFast((p).x,(p).y)
#define	MGL_getPixelFast(p)		MGL_getPixelCoordFast((p).x,(p).y)
#define	MGL_moveTo(p)			MGL_moveToCoord((p).x,(p).y)
#define MGL_moveRel(p)			MGL_moveRelCoord((p).x,(p).y)
#define	MGL_lineCoord(x1,y1,x2,y2)	MGL_lineCoordExt(x1,y1,x2,y2,true)
#define	MGL_line(p1,p2)			MGL_lineCoordExt((p1).x,(p1).y,(p2).x,(p2).y,true)
#define	MGL_lineExt(p1,p2,dl)	MGL_lineCoordExt((p1).x,(p1).y,(p2).x,(p2).y,dl)
#define	MGL_lineTo(p)			MGL_lineToCoord((p).x,(p).y)
#define	MGL_lineRel(p)			MGL_lineRelCoord((p).x,(p).y);
#define	MGL_rectPt(lt,rb)		MGL_rectCoord((lt).x,(lt).y,(rb).x,(rb).y)
#define	MGL_rect(r)				MGL_rectCoord((r).left,(r).top,			\
									(r).right,(r).bottom)
#define	MGL_fillRectPt(lt,rb)	MGL_fillRectCoord((lt).x,(lt).y,	\
									(rb).x,(rb).y)
#define	MGL_fillRect(r)			MGL_fillRectCoord((r).left,(r).top,	\
									(r).right,(r).bottom)
#define	MGL_bitBlt(d,s,r,dl,dt,op) 	MGL_bitBltCoord((d),(s),(r).left,		\
									(r).top,(r).right,(r).bottom,dl,dt,op)
#define	MGL_stretchBlt(d,s,sr,dr,op) 	MGL_stretchBltCoord((d),(s),(sr).left,		\
									(sr).top,(sr).right,(sr).bottom,	\
									(dr).left,(dr).top,(dr).right,(dr).bottom,op)
#define	MGL_srcTransBlt(d,s,r,dl,dt,c,op)	MGL_srcTransBltCoord((d),(s),(r).left,		\
									(r).top,(r).right,(r).bottom,dl,dt,c,op)
#define	MGL_dstTransBlt(d,s,r,dl,dt,c,op)	MGL_dstTransBltCoord((d),(s),(r).left,		\
									(r).top,(r).right,(r).bottom,dl,dt,c,op)
#define	MGL_getDivot(dc,r,divot) MGL_getDivotCoord(dc,(r).left,(r).top,	\
									(r).right,(r).bottom,divot)
#define	MGL_divotSize(dc,r)		MGL_divotSizeCoord(dc,(r).left,(r).top,\
									(r).right,(r).bottom)
#define	MGL_isSimpleRegion(r)	(((region_t*)(r))->spans == NULL)
#define	MGL_rgnLine(p1,p2,p)	MGL_rgnLineCoord((p1).x,(p1).y,(p2).x,(p2).y,p)
#define	MGL_rgnSolidRectPt(lt,rb)	MGL_rgnSolidRectCoord((lt).x,(lt).y,	\
									(rb).x,(rb).y)
#define	MGL_rgnSolidRect(r)			MGL_rgnSolidRectCoord((r).left,(r).top,	\
									(r).right,(r).bottom)

/* Fast color packing/unpacking routines implemented as macros */

#define	MGL_packColorFast(pf,R,G,B)													\
 ((ulong)(((ulong)(R) >> (pf)->redAdjust) & (pf)->redMask) << (pf)->redPos)			\
 | ((ulong)(((ulong)(G) >> (pf)->greenAdjust) & (pf)->greenMask) << (pf)->greenPos)	\
 | ((ulong)(((ulong)(B) >> (pf)->blueAdjust) & (pf)->blueMask) << (pf)->bluePos)

#define	MGL_packColorFastExt(pf,A,R,G,B)											\
 ((ulong)(((ulong)(A) >> (pf)->rsvdAdjust) & (pf)->rsvdMask) << (pf)->rsvdPos)		\
 | ((ulong)(((ulong)(R) >> (pf)->redAdjust) & (pf)->redMask) << (pf)->redPos)		\
 | ((ulong)(((ulong)(G) >> (pf)->greenAdjust) & (pf)->greenMask) << (pf)->greenPos)	\
 | ((ulong)(((ulong)(B) >> (pf)->blueAdjust) & (pf)->blueMask) << (pf)->bluePos)

#define	MGL_unpackColorFast(pf,c,R,G,B)													\
{																						\
 (R) = (uchar)((((ulong)(c) >> (pf)->redPos) & (pf)->redMask) << (pf)->redAdjust);		\
 (G) = (uchar)((((ulong)(c) >> (pf)->greenPos) & (pf)->greenMask) << (pf)->greenAdjust);\
 (B) = (uchar)((((ulong)(c) >> (pf)->bluePos) & (pf)->blueMask) << (pf)->blueAdjust);	\
}

#define	MGL_unpackColorFastExt(pf,c,A,R,G,B)											\
{																						\
 (A) = (uchar)((((ulong)(c) >> (pf)->rsvdPos) & (pf)->rsvdMask) << (pf)->rsvdAdjust);	\
 (R) = (uchar)((((ulong)(c) >> (pf)->redPos) & (pf)->redMask) << (pf)->redAdjust);		\
 (G) = (uchar)((((ulong)(c) >> (pf)->greenPos) & (pf)->greenMask) << (pf)->greenAdjust);\
 (B) = (uchar)((((ulong)(c) >> (pf)->bluePos) & (pf)->blueMask) << (pf)->blueAdjust);	\
}

#ifdef	__cplusplus
}						/* End of "C" linkage for C++	*/

#include "mglrect.hpp"	/* Include C++ point/rectangle classes			*/

#endif	/* __cplusplus */

/* Include appropriate platform specific bindings */

#if		defined(__DRIVER__)
/* Platform independent defines for binary portable DLL builds */
/* {secret} */
typedef	void		*MGL_HDC;
/* {secret} */
typedef	void		*MGL_HINSTANCE;
/* {secret} */
typedef	void		*MGL_HPALETTE;
/* {secret} */
typedef	void		*MGL_HGLRC;
#elif 	defined(__SMX32__)
#include "mglsmx.h"
#elif 	defined(__WINDOWS__)
#include "mglwin.h"
#elif 	defined(__RTTARGET__)
#include "mglrtt.h"
#elif 	defined(__QNX__)
#include "mglqnx.h"
#elif 	defined(__OS2__)
#include "mglos2.h"
#elif 	defined(__UNIX__)
/*#include "mglx.h"*/
#include "mglunix.h"
#elif	defined(__REALDOS__)
#include "mgldos.h"
#else
#error	MGL not ported to this platform yet!
#endif

#endif	/* __MGRAPH_H */
