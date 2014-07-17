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
*				device driver Hardware Abstraction Layer.
*
*				Note that the Nucleus Graphics Architecture provides
*				support for multiple independant controllers and full plug
*				and play support. This allows for multiple drivers to
*				be present in a single system, and the proper driver will be
*				matched to the card's found in the system given the PCI
*				configuration ID's for the cards.
*
*				ISA and VLB devices are only supported in single controller
*				mode.
*
****************************************************************************/

#ifndef __NUCLEUS_GRAPHICS_H
#define __NUCLEUS_GRAPHICS_H

#include "nucleus/monitor.h"
#include "drvlib/peloader.h"
#include "ztimerc.h"

/*---------------------- Macros and type definitions ----------------------*/

#pragma pack(1)

/* Signature for the graphics driver file */

#define	GA_SIGNATURE			"GRAPHICS"

/* Define the interface version */

#define	GA_VERSION				0x100

/* Define the maximum number of devices supported. Internally we can handle
 * any number, so if you need more than this many devices let us know and
 * we can build custom drivers that support more devices.
 */

#define	GA_MAX_DEVICES			4

/* Define the maximum number of chained filter drivers */

#define	GA_MAX_FILTERS			8

/* Define the maximum number of virtual displays */

#define	GA_MAX_VIRTUAL_DISPLAYS	16

/* Define the flag to enable mixed mode for GA_setActiveDevice */

#define	GA_DEVICE_MIXED_MODE	0x80

/****************************************************************************
REMARKS:
Fundamental type definition for an array element of an 8x8 monochrome
pattern. Each line in the pattern is represented as a single byte, with 8
bytes in total for the entire pattern.

HEADER:
nucleus/graphics.h

MEMBERS:
p	- 8 bytes of pattern data
****************************************************************************/
typedef struct {
	N_uint8		p[8];
	} GA_pattern;

/****************************************************************************
REMARKS:
Fundamental type definition for an array element of an 8x8 color bitmap
pattern data for 8bpp modes.

HEADER:
nucleus/graphics.h

MEMBERS:
p	- 8x8 bytes of pattern data
****************************************************************************/
typedef struct {
	N_uint8		p[8][8];
	} GA_colorPattern_8;

/****************************************************************************
REMARKS:
Fundamental type definition for an array element of an 8x8 color bitmap
pattern data for 16bpp modes.

HEADER:
nucleus/graphics.h

MEMBERS:
p	- 8x8 words of pattern data
****************************************************************************/
typedef struct {
	N_uint16	p[8][8];
	} GA_colorPattern_16;

/****************************************************************************
REMARKS:
Fundamental type definition for an array element of an 8x8 color bitmap
pattern data for 24bpp modes.

HEADER:
nucleus/graphics.h

MEMBERS:
p	- 8x8x3 bytes of pattern data
****************************************************************************/
typedef struct {
	N_uint8		p[8][8][3];
	} GA_colorPattern_24;

/****************************************************************************
REMARKS:
Fundamental type definition for an array element of an 8x8 color bitmap
pattern data for 32pp modes.

HEADER:
nucleus/graphics.h

MEMBERS:
p	- 8x8 dwords of pattern data
****************************************************************************/
typedef struct {
	N_uint32	p[8][8];
	} GA_colorPattern_32;

/****************************************************************************
REMARKS:
Fundamental type definition for an array element of an 8x8 color bitmap
pattern. Each line in the pattern is represented as an array of packed
pixel data. In 8bpp modes there is 8 bytes per line, for 16bpp modes
there are 16bytes per line, for 24bpp modes there are 24bytes per line
and for 32bpp modes there are 32 bytes per line. Hence the size of the
pattern data is different depending on the color depth currently active.

HEADER:
nucleus/graphics.h
****************************************************************************/
typedef union {
	GA_colorPattern_8	b8;
	GA_colorPattern_16	b16;
	GA_colorPattern_24	b24;
	GA_colorPattern_32	b32;
	} GA_colorPattern;

/****************************************************************************
REMARKS:
Fundamental type definition for a 16-bit line stipple pattern. Note that
we define it as a 32-bit value so it will be passed as a 32-bit argument
on the stack correctly when calling 32-bit code from a 16-bit segment.

HEADER:
nucleus/graphics.h
****************************************************************************/
typedef N_uint32 GA_stipple;

/****************************************************************************
REMARKS:
Fundamental type definition for a 32-bit color value. The color value is
intepreted differently depending on what graphics mode the system is in,
and in 15-bit and above modes will have the color values packed according
to the pixel format information stored in the GA_modeInfo structure.

HEADER:
nucleus/graphics.h
****************************************************************************/
typedef N_uint32 GA_color;

/****************************************************************************
REMARKS:
This enumeration defines the identifiers used to obtain the device context
function group pointer structures. As new features and capabilities are
added to the future versions of the specification, new identifiers will
be added to extract new function pointers from the drivers.

The GA_GET_FIRST_OEM defines the first identifier for OEM extensions. OEM's
are free to added their own private functional extensions to the drivers
as desired. Note that OEM's must verify the presence of their OEM drivers
via the the OemVendorName string before attempting to use OEM extension
functions.

HEADER:
nucleus/graphics.h

MEMBERS:
GA_GET_WIN16FUNCS		- Get GA_Win16Funcs structure (Intel x86 only)
GA_GET_INITFUNCS		- Get GA_initFuncs structure
GA_GET_DRIVERFUNCS		- Get GA_driverFuncs structure
GA_GET_CURSORFUNCS     	- Get GA_cursorFuncs structure
GA_GET_VIDEOFUNCS		- Get GA_videoFuncs strucure
GA_GET_DPMSFUNCS		- Get GA_DPMSFuncs structure
GA_GET_SCIFUNCS			- Get GA_SCIFuncs structure
GA_GET_2DSTATEFUNCS		- Get GA_2DStateFuncs structure
GA_GET_2DRENDERFUNCS	- Get GA_2DRenderFuncs structure
GA_GET_3DSETUPFUNCS		- Get GA_3DSetupFuncs structure
GA_GET_3DSTATEFUNCS		- Get GA_3DStateFuncs structure
GA_GET_3DRENDERFUNCS	- Get GA_3DRenderFuncs structure
GA_GET_D3DRENDERFUNCS	- Get GA_D3DRenderFuncs structure
GA_GET_VBEFUNCS			- Get GA_VBEFuncs structure
GA_GET_FIRST_OEM		- ID of first OEM extension function
****************************************************************************/
typedef enum {
	GA_GET_WIN16FUNCS,
	GA_GET_INITFUNCS,
	GA_GET_DRIVERFUNCS,
	GA_GET_CURSORFUNCS,
	GA_GET_VIDEOFUNCS,
	GA_GET_DPMSFUNCS,
	GA_GET_SCIFUNCS,
	GA_GET_2DSTATEFUNCS,
	GA_GET_2DRENDERFUNCS,
	GA_GET_3DSETUPFUNCS,
	GA_GET_3DSTATEFUNCS,
	GA_GET_3DRENDERFUNCS,
	GA_GET_D3DRENDERFUNCS,
	GA_GET_VBEFUNCS,
	GA_GET_FIRST_OEM				= 0x00010000,
	} GA_funcGroupsType;

/****************************************************************************
REMARKS:
This enumeration defines the flags for combining with graphics mode numbers
to be passed to the SetVideoMode function.

HEADER:
nucleus/graphics.h

MEMBERS:
gaDontClear			- Don't clear display memory
gaLinearBuffer		- Enable linear framebuffer mode
ga6BitDAC			- Set the mode with a 6-bit RAMDAC instead of 8
gaRefreshCtrl		- Enable refresh rate control
gaInitOnly			- Initialise the driver internals, but don't program hardware
gaModeMask			- Mask to remove flags and extract VBE mode number
****************************************************************************/
typedef enum {
	gaDontClear						= 0x8000,
	gaLinearBuffer					= 0x4000,
	ga6BitDAC						= 0x2000,
	gaRefreshCtrl					= 0x0800,
	gaInitOnly						= 0x0200,
	gaModeMask						= 0x01FF,
	} GA_modeFlagsType;

/****************************************************************************
REMARKS:
Logical mix operation codes for accelerated rendering functions that
support mixes. The set of mix codes is the standard Microsoft Raster
Operation (ROP2) codes between two values. We define our ROP2 codes as being
between the source and destination pixels for blt's, between the foreground
or background color and the destination pixels for solid and mono pattern
fills and between the pattern pixels and the destination pixels for color
pattern fills. It is up to the driver to do any necessary translation
between these generic ROP2 codes and each different type of hardware mix
code internally. Next to each code is the equivalent Microsoft defined ROP3
code between source and destination pixels.

Note:	Some graphics controllers may not support all mix codes, so you must
		use the GetMixTable function to determine the set of mix codes that
		the controller supports. Setting a mix code that is not listed in
		the returned mix table will result in undefined behaviour.

HEADER:
nucleus/graphics.h

MEMBERS:
GA_R2_BLACK        	- 0
GA_R2_NOTMERGESRC  	- DSon
GA_R2_MASKNOTSRC   	- DSna
GA_R2_NOTCOPYSRC   	- Sn
GA_R2_MASKSRCNOT   	- SDna
GA_R2_NOT          	- Dn
GA_R2_XORSRC       	- DSx
GA_R2_NOTMASKSRC   	- DSan
GA_R2_MASKSRC      	- DSa
GA_R2_NOTXORSRC    	- DSxn
GA_R2_NOP			- D
GA_R2_MERGENOTSRC  	- DSno
GA_R2_COPYSRC      	- S
GA_R2_MERGESRCNOT  	- SDno
GA_R2_MERGESRC     	- DSo
GA_R2_WHITE   		- 1
GA_REPLACE_MIX 		- Replace mode
GA_AND_MIX 	   		- AND mode
GA_OR_MIX 	   		- OR mode
GA_XOR_MIX 	   		- XOR mode
GA_NOP_MIX 	   		- Destination pixel unchanged
****************************************************************************/
typedef enum {
	GA_R2_BLACK,
	GA_R2_NOTMERGESRC,
	GA_R2_MASKNOTSRC,
	GA_R2_NOTCOPYSRC,
	GA_R2_MASKSRCNOT,
	GA_R2_NOT,
	GA_R2_XORSRC,
	GA_R2_NOTMASKSRC,
	GA_R2_MASKSRC,
	GA_R2_NOTXORSRC,
	GA_R2_NOP,
	GA_R2_MERGENOTSRC,
	GA_R2_COPYSRC,
	GA_R2_MERGESRCNOT,
	GA_R2_MERGESRC,
	GA_R2_WHITE,
	GA_REPLACE_MIX = GA_R2_COPYSRC,
	GA_AND_MIX 	   = GA_R2_MASKSRC,
	GA_OR_MIX 	   = GA_R2_MERGESRC,
	GA_XOR_MIX 	   = GA_R2_XORSRC,
	GA_NOP_MIX 	   = GA_R2_NOP,
	} GA_mixCodesType;

/****************************************************************************
REMARKS:
Raster Operation codes for accelerated rendering functions that support
ternary operations. The set of mix codes is the standard Microsoft ternary
Raster Operation (ROP3) codes between three values, a source, pattern and
destination. Note that we don't list the codes here for brevity.

Note:	Some graphics controllers may not support all ROP3 codes due to
		hardware bugs, so you must use the GetROP3ExceptionTable function to
		determine the set of ROP3 codes that the hardware does not properly
		handle.

HEADER:
nucleus/graphics.h
****************************************************************************/
typedef enum {
	GA_R3_0,
	GA_R3_DPSoon,
	GA_R3_DPSona,
	GA_R3_PSon,
	GA_R3_SDPona,
	GA_R3_DPon,
	GA_R3_PDSxnon,
	GA_R3_PDSaon,
	GA_R3_SDPnaa,
	GA_R3_PDSxon,
	GA_R3_DPna,
	GA_R3_PSDnaon,
	GA_R3_SPna,
	GA_R3_PDSnaon,
	GA_R3_PDSonon,
	GA_R3_Pn,
	GA_R3_PDSona,
	GA_R3_DSon,
	GA_R3_SDPxnon,
	GA_R3_SDPaon,
	GA_R3_DPSxnon,
	GA_R3_DPSaon,
	GA_R3_PSDPSanaxx,
	GA_R3_SSPxDSxaxn,
	GA_R3_SPxPDxa,
	GA_R3_SDPSanaxn,
	GA_R3_PDSPaox,
	GA_R3_SDPSxaxn,
	GA_R3_PSDPaox,
	GA_R3_DSPDxaxn,
	GA_R3_PDSox,
	GA_R3_PDSoan,
	GA_R3_DPSnaa,
	GA_R3_SDPxon,
	GA_R3_DSna,
	GA_R3_SPDnaon,
	GA_R3_SPxDSxa,
	GA_R3_PDSPanaxn,
	GA_R3_SDPSaox,
	GA_R3_SDPSxnox,
	GA_R3_DPSxa,
	GA_R3_PSDPSaox,
	GA_R3_DPSana,
	GA_R3_SSPxPDxaxn,
	GA_R3_SPDSoax,
	GA_R3_PSDnox,
	GA_R3_PSDPxox,
	GA_R3_PSDnoan,
	GA_R3_PSna,
	GA_R3_SDPnaon,
	GA_R3_SDPSoox,
	GA_R3_Sn,
	GA_R3_SPDSaox,
	GA_R3_SPDSxnox,
	GA_R3_SDPox,
	GA_R3_SDPoan,
	GA_R3_PSDPoax,
	GA_R3_SPDnox,
	GA_R3_SPDSxox,
	GA_R3_SPDnoan,
	GA_R3_PSx,
	GA_R3_SPDSonox,
	GA_R3_SPDSnaox,
	GA_R3_PSan,
	GA_R3_PSDnaa,
	GA_R3_DPSxon,
	GA_R3_SDxPDxa,
	GA_R3_SPDSanaxn,
	GA_R3_SDna,
	GA_R3_DPSnaon,
	GA_R3_DSPDaox,
	GA_R3_PSDPxaxn,
	GA_R3_SDPxa,
	GA_R3_PDSPDaoxxn,
	GA_R3_DPSDoax,
	GA_R3_PDSnox,
	GA_R3_SDPana,
	GA_R3_SSPxDSxoxn,
	GA_R3_PDSPxox,
	GA_R3_PDSnoan,
	GA_R3_PDna,
	GA_R3_DSPnaon,
	GA_R3_DPSDaox,
	GA_R3_SPDSxaxn,
	GA_R3_DPSonon,
	GA_R3_Dn,
	GA_R3_DPSox,
	GA_R3_DPSoan,
	GA_R3_PDSPoax,
	GA_R3_DPSnox,
	GA_R3_DPx,
	GA_R3_DPSDonox,
	GA_R3_DPSDxox,
	GA_R3_DPSnoan,
	GA_R3_DPSDnaox,
	GA_R3_DPan,
	GA_R3_PDSxa,
	GA_R3_DSPDSaoxxn,
	GA_R3_DSPDoax,
	GA_R3_SDPnox,
	GA_R3_SDPSoax,
	GA_R3_DSPnox,
	GA_R3_DSx,
	GA_R3_SDPSonox,
	GA_R3_DSPDSonoxxn,
	GA_R3_PDSxxn,
	GA_R3_DPSax,
	GA_R3_PSDPSoaxxn,
	GA_R3_SDPax,
	GA_R3_PDSPDoaxxn,
	GA_R3_SDPSnoax,
	GA_R3_PDSxnan,
	GA_R3_PDSana,
	GA_R3_SSDxPDxaxn,
	GA_R3_SDPSxox,
	GA_R3_SDPnoan,
	GA_R3_DSPDxox,
	GA_R3_DSPnoan,
	GA_R3_SDPSnaox,
	GA_R3_DSan,
	GA_R3_PDSax,
	GA_R3_DSPDSoaxxn,
	GA_R3_DPSDnoax,
	GA_R3_SDPxnan,
	GA_R3_SPDSnoax,
	GA_R3_DPSxnan,
	GA_R3_SPxDSxo,
	GA_R3_DPSaan,
	GA_R3_DPSaa,
	GA_R3_SPxDSxon,
	GA_R3_DPSxna,
	GA_R3_SPDSnoaxn,
	GA_R3_SDPxna,
	GA_R3_PDSPnoaxn,
	GA_R3_DSPDSoaxx,
	GA_R3_PDSaxn,
	GA_R3_DSa,
	GA_R3_SDPSnaoxn,
	GA_R3_DSPnoa,
	GA_R3_DSPDxoxn,
	GA_R3_SDPnoa,
	GA_R3_SDPSxoxn,
	GA_R3_SSDxPDxax,
	GA_R3_PDSanan,
	GA_R3_PDSxna,
	GA_R3_SDPSnoaxn,
	GA_R3_DPSDPoaxx,
	GA_R3_SPDaxn,
	GA_R3_PSDPSoaxx,
	GA_R3_DPSaxn,
	GA_R3_DPSxx,
	GA_R3_PSDPSonoxx,
	GA_R3_SDPSonoxn,
	GA_R3_DSxn,
	GA_R3_DPSnax,
	GA_R3_SDPSoaxn,
	GA_R3_SPDnax,
	GA_R3_DSPDoaxn,
	GA_R3_DSPDSaoxx,
	GA_R3_PDSxan,
	GA_R3_DPa,
	GA_R3_PDSPnaoxn,
	GA_R3_DPSnoa,
	GA_R3_DPSDxoxn,
	GA_R3_PDSPonoxn,
	GA_R3_PDxn,
	GA_R3_DSPnax,
	GA_R3_PDSPoaxn,
	GA_R3_DPSoa,
	GA_R3_DPSoxn,
	GA_R3_D,
	GA_R3_DPSono,
	GA_R3_SPDSxax,
	GA_R3_DPSDaoxn,
	GA_R3_DSPnao,
	GA_R3_DPno,
	GA_R3_PDSnoa,
	GA_R3_PDSPxoxn,
	GA_R3_SSPxDSxox,
	GA_R3_SDPanan,
	GA_R3_PSDnax,
	GA_R3_DPSDoaxn,
	GA_R3_DPSDPaoxx,
	GA_R3_SDPxan,
	GA_R3_PSDPxax,
	GA_R3_DSPDaoxn,
	GA_R3_DPSnao,
	GA_R3_DSno,
	GA_R3_SPDSanax,
	GA_R3_SDxPDxan,
	GA_R3_DPSxo,
	GA_R3_DPSano,
	GA_R3_PSa,
	GA_R3_SPDSnaoxn,
	GA_R3_SPDSonoxn,
	GA_R3_PSxn,
	GA_R3_SPDnoa,
	GA_R3_SPDSxoxn,
	GA_R3_SDPnax,
	GA_R3_PSDPoaxn,
	GA_R3_SDPoa,
	GA_R3_SPDoxn,
	GA_R3_DPSDxax,
	GA_R3_SPDSaoxn,
	GA_R3_S,
	GA_R3_SDPono,
	GA_R3_SDPnao,
	GA_R3_SPno,
	GA_R3_PSDnoa,
	GA_R3_PSDPxoxn,
	GA_R3_PDSnax,
	GA_R3_SPDSoaxn,
	GA_R3_SSPxPDxax,
	GA_R3_DPSanan,
	GA_R3_PSDPSaoxx,
	GA_R3_DPSxan,
	GA_R3_PDSPxax,
	GA_R3_SDPSaoxn,
	GA_R3_DPSDanax,
	GA_R3_SPxDSxan,
	GA_R3_SPDnao,
	GA_R3_SDno,
	GA_R3_SDPxo,
	GA_R3_SDPano,
	GA_R3_PDSoa,
	GA_R3_PDSoxn,
	GA_R3_DSPDxax,
	GA_R3_PSDPaoxn,
	GA_R3_SDPSxax,
	GA_R3_PDSPaoxn,
	GA_R3_SDPSanax,
	GA_R3_SPxPDxan,
	GA_R3_SSPxDSxax,
	GA_R3_DSPDSanaxxn,
	GA_R3_DPSao,
	GA_R3_DPSxno,
	GA_R3_SDPao,
	GA_R3_SDPxno,
	GA_R3_DSo,
	GA_R3_SDPnoo,
	GA_R3_P,
	GA_R3_PDSono,
	GA_R3_PDSnao,
	GA_R3_PSno,
	GA_R3_PSDnao,
	GA_R3_PDno,
	GA_R3_PDSxo,
	GA_R3_PDSano,
	GA_R3_PDSao,
	GA_R3_PDSxno,
	GA_R3_DPo,
	GA_R3_DPSnoo,
	GA_R3_PSo,
	GA_R3_PSDnoo,
	GA_R3_DPSoo,
	GA_R3_1,
	} GA_rop3CodesType;

/****************************************************************************
REMARKS:
Flags for 2D alpha blending functions supported by the nucleus drivers.
The values in here define the the alpha blending functions passed to
SetSrcBlendFunc and SetDstBlendFunc. Essentially the blend function defines
how to combine the source and destination pixel color together to get the
resulting destination color during rendering. The formula used for this is
defined as:

	DstColor = SrcColor * SrcFunc + DstColor * DstFunc;

If the source alpha blending function is set to gaBlendConstantAlpha, the
/SrcFunc/ above becomes:

	SrcFunc = ConstAlpha

If the destination alpha blending function is set to gaBlendOneMinusDstAlpha
then /DstFunc/ above becomes:

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

Note:	The constant color value set by a call to SetForeColor, and the
		constant alpha value set by a call to SetAlphaValue.

Note:	Setting a blending function that uses the destination alpha
		components is only supported if the framebuffer currently supports
		destination alpha. Likewise setting a blending function that
		uses source alpha components is only supported if the framebuffer
		or incoming bitmap data contains an alpha channel. The results
		are undefined if these conditiions are not met.

Note:	Enabling source or destination alpha blending overrides the
		setting of the current mix mode. Logical mix modes and blending
		cannot be used at the same time.

HEADER:
nucleus/graphics.h

MEMBERS:
gaBlendNone						- No alpha blending
gaBlendZero						- Blend factor is always zero
gaBlendOne                  	- Blend factor is always one
gaBlendSrcColor					- Blend factor is source color
gaBlendOneMinusSrcColor			- Blend factor is 1-source color
gaBlendSrcAlpha					- Blend factor is source alpha
gaBlendOneMinusSrcAlpha			- Blend factor is 1-source alpha
gaBlendDstAlpha					- Blend factor is destination alpha
gaBlendOneMinusDstAlpha			- Blend factor is 1-destination alpha
gaBlendDstColor					- Blend factor is destination color
gaBlendOneMinusDstColor			- Blend factor is 1-destination color
gaBlendSrcAlphaSaturate			- Blend factor is src alpha saturation
gaBlendConstantColor			- Blend factor is a constant color
gaBlendOneMinusConstantColor	- Blend factor is 1-constant color
gaBlendConstantAlpha			- Blend factor is constant alpha
gaBlendOneMinusConstantAlpha	- Blend factor is 1-constant alpha
****************************************************************************/
typedef enum {
	gaBlendNone,
	gaBlendZero,
	gaBlendOne,
	gaBlendSrcColor,
	gaBlendOneMinusSrcColor,
	gaBlendSrcAlpha,
	gaBlendOneMinusSrcAlpha,
	gaBlendDstAlpha,
	gaBlendOneMinusDstAlpha,
	gaBlendDstColor,
	gaBlendOneMinusDstColor,
	gaBlendSrcAlphaSaturate,
	gaBlendConstantColor,
	gaBlendOneMinusConstantColor,
	gaBlendConstantAlpha,
	gaBlendOneMinusConstantAlpha,
	} GA_blendFuncType;

/****************************************************************************
REMARKS:
Flags for the Attributes member of the GA_modeInfo structure and in the
Attributes member of the main GA_devCtx device context block structure. These
flags define the hardware capabilities of the particular device or graphics
mode.

The gaHaveDisplayStart flag is used to determine whether the graphics mode
supports changing the CRTC display start address. This is used to implement
hardware virtual scrolliong and multi-buffering for flicker free animation.
If this bit is 0, then the application cannot change the display start
address after initialising a display mode.

The gaHaveBankedBuffer flag is used to determine if the graphics mode
supports the banked framebuffer access modes. If this bit is 0, then the
application cannot use the banked framebuffer style access. Some controllers
may not support a banked framebuffer mode in some modes. In this case a
linear framebuffer mode will be provided (either a banked buffer or linear
buffer must be available for the mode to be valid).

The gaHaveLinearBuffer flag is used to determine if the graphics mode
supports the linear framebuffer access modes. If this bit is 0, then the
application cannot start the linear framebuffer graphics mode.

The gaHaveAccel2D flag is used to determine if the graphics mode supports 2D
accelerator functions. If this bit is 0, then the application can only use
direct framebuffer access in this video mode, and the 2D acceleration
functions are not available. The cases where this might crop up are more
common than you might think. This bit may be 0 for very low resolution
graphics modes on some controllers, and on older controllers for the 24 bit
and above graphics modes.

The gaHaveHWCursor flag is used to determine if the controller supports a
hardware cursor for the specified graphics mode. You must check this flag
for each graphics mode before attempting to use the hardware cursor functions
as some graphics modes will not be able to support the hardware cursor (but
may still support 2D acceleration).

The gaHave8BitDAC flag is used to determine if the controller will be using
the 8 bit wide palette DAC modes when runing in 256 color index modes. The 8
bit DAC modes allow the palette to be selected from a range of 16.7 million
colors rather than the usual 256k colors available in 6 bit DAC mode. The 8
bit DAC mode allows the 256 color modes to display a full range of 256
grayscales, while the 6 bit mode only allows a selection of 64 grayscales.
Note that the 8 bit DAC mode is not selectable. If the hardware supports an
8 bit DAC, it will always be used by default.

The gaHaveNonVGAMode flag is used to determine if the mode is a VGA compatible
mode or a NonVGA mode. If this flag is set, the application software must
ensure that no attempts are made to directly program any of the standard VGA
compatible registers such as the RAMDAC control registers and inpus status
registers while the NonVGA graphics mode is used. Attempting to use these
registers in NonVGA modes generally results in the application program
hanging the system.

The gaHaveDoubleScan flag is used to determine if the mode requires double
scanning. If this bit is set, the double scan bit must be set for the
graphics mode if it is initialised with generic refresh control turned on.

The gaHaveTripleScan flag is used to determine if the mode requires triple
scanning. If this bit is set, the triple scan bit must be set for the
graphics mode if it is initialised with generic refresh control turned on.

The gaHaveInterlaced flag is used to determine if the mode supports
interlaced operation or not. If this bit is set, the mode may be initialized
for interlaced operation when using the refresh rate control to initialise
the mode.

The gaHaveTripleBuffer flag is used to determine if the mode supports
hardware triple buffering.

The gaHaveStereo flag is used to determine if the mode supports hardware
support for stereo LC shutter glasses.

The gaHaveHWStereoSync flag is used to determine if the controller supports
the hardware stereo LC shutter glasses sync signal via the VESA EVC Enhanced
Video Connector. The gaHaveEVCStereoSync flag is used to determine if the
controller supports the hardware stereo LC shutter glasses sync signal via
the VESA mini-DIN3 stereo connector. If either of these values are set, the
application can disable all software stero sync mechanisms and rely on the
the hardware stereo sync for maximum performance.

The gaHaveAccelVideo flag is used to determine if the mode supports
hardware video acceleration. If this bit is not 0, then the application
can use the hardware video functions for video overlay windows.

The gaHaveAccel3D flag is used to determine if the mode supports hardware
3D acceleration. If this bit is not 0, then the application can use the
hardware 3D accleration functions for high performance 3D graphics.

The gaHave8bppRGBCursor flag is used to determine if the color values for
the hardware cursor in 8bpp modes are defined as a color index or as
a TrueColor RGB tuple. Most cards require a color index in 8bpp modes,
but some new hardware uses a TrueColor cursor in 8bpp display modes and
this flag will be set if this is the case.

The gaHaveAccelIOPL flag indicates that the accelerated drawing functions
require IOPL access to be enabled. If this flag is not set, then the 2D
and 3D drawing functions use only memory mapped registers and hence can
be executed entirely in ring-3 without needing IOPL to be enabled. Note
that this does not include hardware cursor functions or hardware video
overlay functions, only 2D and 3D drawing functions. It is assumed that
all initialisation and driver functions require IOPL to be enabled.

The gaIsVirtualMode flag indicates that the mode is a special
multi-controller virtual display mode that spans multiple display devices.
This is an informational flag so that any high level OS drivers can know
when one of these modes is in use.

The gaHaveLCDOutput flag is used to determine if a mode can be displayed
on an LCD flat panel. This flag is generally only available for laptop
chipsets, and indicates display modes that can support output to the LCD
panel as well as simulatenous output to both displays at the same time.

The gaHaveTVOutput flag is used to determine if a mode can be displayed
via the TVOut connector for the graphics card. If the graphics card does
not support TVOut capabilities this flag will never be set. Otherwise it
will be set for those display modes that can be displayed on the TV. Note
that both PAL and NTSC output may be supported, or only one or the other
depending on the underlying hardware.

The gaIsTextMode flag is used to determine if the mode is a graphics mode
or a text mode. If this flag is set to 1, then the mode is a hardware text
mode and not a graphics mode.

HEADER:
nucleus/graphics.h

MEMBERS:
gaHaveDisplayStart   	- Mode supports changing the display start address
gaHaveBankedBuffer		- Mode supports banked framebuffer access
gaHaveLinearBuffer		- Mode supports linear framebuffer access
gaHaveAccel2D			- Mode supports 2D acceleration
gaHaveHWCursor			- Mode supports a hardware cursor
gaHave8BitDAC			- Mode uses an 8 bit palette DAC
gaHaveNonVGAMode		- Mode is a NonVGA mode
gaHaveDoubleScan		- Mode is double scanned
gaHaveTripleScan		- Mode is triple scanned
gaHaveInterlaced		- Mode supports interlacing
gaHaveTripleBuffer		- Mode supports triple buffering
gaHaveStereo			- Mode supports stereo LCD glasses
gaHaveHWStereoSync		- Mode supports stereo signalling
gaHaveEVCStereoSync		- Mode supports stereo sync via EVC connector
gaHaveAccelVideo    	- Mode supports video playback acceleration
gaHaveAccel3D			- Mode supports 3D acceleration
gaHave8bppRGBCursor		- Mode requires RGB colors for 8bpp hardware cursor
gaHaveHWColorCursor		- Mode supports a 16-color hardware color cursor
gaHaveAccelIOPL			- Mode needs IOPL for drawing functions
gaHaveHWColorCursor256	- Mode supports a 256-color hardware color cursor
gaHaveHWColorCursorRGBA	- Mode supports a 24-bit RGBA hardware color cursor
gaIsVirtualMode			- Mode is a multi-controller virtual mode
gaHaveLCDOutput			- Mode supports output to LCD display
gaHaveTVOutput			- Mode supports output to TV connector
gaIsTextMode			- Mode is a text mode rather than a graphics mode
****************************************************************************/
typedef enum {
	gaHaveDisplayStart   			= 0x00000001,
	gaHaveBankedBuffer				= 0x00000002,
	gaHaveLinearBuffer				= 0x00000004,
	gaHaveAccel2D					= 0x00000008,
	gaHaveHWCursor					= 0x00000010,
	gaHave8BitDAC					= 0x00000020,
	gaHaveNonVGAMode				= 0x00000040,
	gaHaveDoubleScan				= 0x00000080,
	gaHaveTripleScan				= 0x00000100,
	gaHaveInterlaced				= 0x00000200,
	gaHaveTripleBuffer				= 0x00000400,
	gaHaveStereo					= 0x00000800,
	gaHaveHWStereoSync				= 0x00001000,
	gaHaveEVCStereoSync				= 0x00002000,
	gaHaveAccelVideo    			= 0x00004000,
	gaHaveAccel3D					= 0x00008000,
	gaHave8bppRGBCursor				= 0x00010000,
	gaHaveHWColorCursor				= 0x00020000,
	gaHaveAccelIOPL					= 0x00040000,
	gaHaveHWColorCursor256			= 0x00080000,
	gaHaveHWColorCursorRGBA			= 0x00100000,
	gaIsVirtualMode					= 0x08000000,
	gaHaveLCDOutput					= 0x10000000,
	gaHaveTVOutput					= 0x20000000,
	gaIsTextMode					= 0x40000000,
	} GA_AttributeFlagsType;

/****************************************************************************
REMARKS:
Flags for the WorkArounds member of the GA_devCtx structure. These flags
define conditions for uncommon hardware bugs that can't easily be handled
via the generic Nucleus information reporting mechanism. Any code that
calls the Nucleus hardware drivers directly must be aware of these
workarounds and how to deal with them. However the Nucleus Software Reference
Rasteriser knows how to deal with all currently known bugs, so application
developers should use the reference rasteriser at all times for maximum
compatibility with new hardware drivers.

The gaSlowBltSys flag indicates that the hardware BitBltSys function is
faster than a direct linear framebuffer software blit. Most modern hardware
can do a software blit as fast or faster than using the hardware, but some
hardware can be faster than a pure software blit. This is only true when the
mix mode is GA_REPLACE_MIX, since software reads from the framebuffer over
the PCI bus are terribly slow.

The gaHWCursor32x32 flag indicates that the hardware only supports a 32x32
hardware cursor, while the Nucleus specification implements an interface
for 64x64 hardware cursors. Nucleus drivers will still implement hardware
cursor support for hardware that only supports a 32x32 cursor, however this
flag will be set. If the high level operating system drivers require a
cursor larger than 32x32, then this flag should be checked and a software
cursor used in it's place when this is the case.

The gaSlow24bpp flag indicates that although the 24bpp display modes are
accelerated, they are only partically accelerated. Hence if there is an
equiavlent 32bpp display mode, that most should be used in preference to
the 24bpp display mode if possible.

The gaSlow32bpp flag indicates that although the 32bpp display modes are
accelerated, they are only partically accelerated. Hence if there is an
equiavlent 24bpp display mode, that most should be used in preference to
the 32bpp display mode if possible.

The gaBrokenLines flag indicates that the hardware line drawing produces
slightly different pixels than the software reference rasterizer and
cannot be made to produce correct pixels. For this reason, conformance
testing for line drawing will be skipped on this hardware.

The gaNoDDCBIOS flag is an internal flag to indicate that the card does
not have DDC BIOS support, and hence we should not attempt to use the
DDC BIOS functions to read the EDID for legacy devices.

The gaNoWriteCombine flag is an internal flag to indicate that the graphics
chipset does not work properly when write combining is enabled for
later processors. If write combining is used, it may cause the system
to lock or hang.

The gaNoInterlacedCursor flag is an internal flag to indicate that the
graphics chipset does not properly handle hardware cursors in interlaced
display modes. Hence a software cursor should be used instead for these
modes.

HEADER:
nucleus/graphics.h

MEMBERS:
gaSlowBltSys				- Software is slower than hardware for GA_REPLACE_MODE
gaHWCursor32x32				- The hardware cursor is only 32x32 in size
gaHWCursorBlackBackground	- The hardware cursor requires that the
							  background color always be black (0's)
gaSlow24bpp					- The 24bpp modes are only partially accelerated
gaSlow32bpp					- The 32bpp modes are only partially accelerated
gaBrokenLines				- The hardware line drawing is not conformant
gaNoDDCBIOS					- Card does not have DDC BIOS support
gaNoWriteCombine			- Card does not support write combining
gaNoInterlacedCursor		- HW cursor in interlaced modes is broken
gaHWCursorBlackAndWhite8bpp	- The hardware cursor in 8bpp is always black
							  and white and cannot be changed.
****************************************************************************/
typedef enum {
	gaSlowBltSys					= 0x00000001,
	gaHWCursor32x32					= 0x00000002,
	gaHWCursorBlackBackground		= 0x00000004,
	gaSlow24bpp						= 0x00000008,
	gaSlow32bpp						= 0x00000010,
	gaBrokenLines					= 0x00000020,
	gaNoDDCBIOS						= 0x00000040,
	gaNoWriteCombine				= 0x00000080,
	gaNoInterlacedCursor			= 0x00000100,
	gaHWCursorBlackAndWhite8bpp		= 0x00000200,
	} GA_WorkAroundsFlagsType;

/****************************************************************************
REMARKS:
Flags for the different output displays supported by the driver. These
flags are passed to the SetDisplayOutput function to change the currently
active display device and the GetDisplayDevice function to determine what
devices are currenlty being used to display output.

NOTE:	The color format for TV modes may be specified, or it may not. In
		the case where the color format is not specified, the currently
		active color format will be used. In some cases the color format
		is set in hardware and cannot be changed.

HEADER:
nucleus/graphics.h

MEMBERS:
gaOUTPUT_CRT			- Indicates output is sent to CRT display
gaOUTPUT_LCD			- Indicates output is sent to LCD panel
gaOUTPUT_TV				- Indicates output is sent to TV connector
gaOUTPUT_TVNTSC			- Set TVOut connector color format to NTSC
gaOUTPUT_TVNTSC_J		- Set TVOut connector color format to NTSC-J
gaOUTPUT_TVPAL			- Set TVOut connector color format to PAL
gaOUTPUT_TVPAL_M		- Set TVOut connector color format to PAL-M
gaOUTPUT_TVPAL_60		- Set TVOut connector color format to PAL-60
gaOUTPUT_TVPAL_CN		- Set TVOut connector color format to PAL-CN
gaOUTPUT_TVSCART_PAL	- Set TVOut connector color format to SCART-PAL
gaOUTPUT_TVUNDERSCAN	- Indicates TV output should be underscanned
gaOUTPUT_TVOVERSCAN		- Indicates TV output should be overscanned
gaOUTPUT_TVCOLORMASK	- Mask to mask out TV color format
****************************************************************************/
typedef enum {
	gaOUTPUT_CRT			= 0x0001,
	gaOUTPUT_LCD			= 0x0002,
	gaOUTPUT_TV				= 0x0004,
	gaOUTPUT_TVNTSC			= 0x0008,
	gaOUTPUT_TVNTSC_J		= 0x0010,
	gaOUTPUT_TVPAL			= 0x0020,
	gaOUTPUT_TVPAL_M		= 0x0040,
	gaOUTPUT_TVPAL_60		= 0x0080,
	gaOUTPUT_TVPAL_CN       = 0x0100,
	gaOUTPUT_TVSCART_PAL    = 0x0200,
	gaOUTPUT_TVUNDERSCAN	= 0x0000,
	gaOUTPUT_TVOVERSCAN		= 0x8000,
	gaOUTPUT_TVCOLORMASK	= 0x03F8,
	} GA_OutputFlagsType;

/****************************************************************************
REMARKS:
Flags for hardware video input formats defined in the VideoInputFormats
member of the GA_videoInf structure. These flags define the hardware
video capabilities of the particular video overlay window, and are only valid
if the gaHaveAccelVideo flag is defined in the Attributes member of the
GA_modeInfo structure.

The gaVideoRGB332 flag is used to determine whether the video overlay window can
support hardware video playback of frames stored in the RGB 3:3:2 format
(8 bits per pixel).

The gaVideoRGB555 flag is used to determine whether the video overlay window can
support hardware video playback of frames stored in the RGB 5:5:5 format
(16 bits per pixel, 1 ignored).

The gaVideoRGB565 flag is used to determine whether the video overlay window can
support hardware video playback of frames stored in the RGB 5:6:5 format
(16 bits per pixel).

The gaVideoRGB888 flag is used to determine whether the video overlay window can
support hardware video playback of frames stored in the RGB 8:8:8 format
(24 bits per pixel). Only one RGB format is supported, and the Blue byte is
always stored first in memory (ie: B:G:R).

The gaVideoRGB8888 flag is used to determine whether the video overlay window can
support hardware video playback of frames stored in the RGB 8:8:8:8 format
(32 bits per pixel). Only one RGB format is supported, and the Blue byte is
always stored first in memory (ie: B:G:R:A).

The gaVideoYUV9 flag is used to determine whether the video overlay window can
support hardware video playback of frames stored in the YUV9 format. For
more information on different YUV formats and how they are actually stored
in the framebuffer, see the section titled 'Overview of YUV pixels'.

The gaVideoYUV12 flag is used to determine whether the video overlay window can
support hardware video playback of frames stored in the YUV12 format. For
more information on different YUV formats and how they are actually stored
in the framebuffer, see the section titled 'Overview of YUV pixels'.

The gaVideoYUV411 flag is used to determine whether the video overlay window can
support hardware video playback of frames stored in the YUV 4:1:1 format.
The YUV 4:1:1 data can be stored with the YUV values in varying formats, and
you should check the gaVideoYUYV etc flags to determine which formats are
supported by this controller. For more information on different YUV formats
and how they are actually stored in the framebuffer, see the section titled
'Overview of YUV pixels'.

The gaVideoYUV422 flag is used to determine whether the video overlay window can
support hardware video playback of frames stored in the YUV 4:2:2 format.
The YUV 4:2:2 data can be stored with the YUV values in varying formats, and
you should check the gaVideoYUYV etc flags to determine which formats are
supported by this controller. For more information on different YUV formats
and how they are actually stored in the framebuffer, see the section titled
'Overview of YUV pixels'.

The gaVideoYUV444 flag is used to determine whether the video overlay window can
support hardware video playback of frames stored in the YUV 4:4:4 format.
The YUV 4:4:4 data can be stored with the YUV values in varying formats, and
you should check the gaVideoYUYV etc flags to determine which formats are
supported by this controller. For more information on different YUV formats
and how they are actually stored in the framebuffer, see the section titled
'Overview of YUV pixels'.

The gaVideoYCrCb422 flag is used to determine whether the video overlay window can
support hardware video playback of frames stored in the YCrCb 4:2:2 format.
The YCrCb 4:2:2 data can be stored with the YUV values in varying formats,
and you should check the gaVideoYUYV etc flags to determine which formats are
supported by this controller. For more information on different YUV formats
and how they are actually stored in the framebuffer, see the section titled
'Overview of YUV pixels'.

The gaVideoYUYV, gaVideoYVYU, gaVideoUYVY and gaVideoVYUY flags are used to
determine what YUV pixel layouts is supported for the above suppored YUV
pixel formats.

Note:	These flags are also passed to the SetVideoInput function to
		determine the video input data type being displayed for the
		video window.

Note: 	The gaVideoYUYV and related flags define the YUV pixel layouts that
		are supported by the hardware for the YUV input format it supports.
		For instance the hardware may report gaVideoYUV422 and the
		gaVideoYUYV flags, which means it supports the YUV422 format with
		the format 4:2:4:2 (Y:U:Y:V) in video memory. See the section titled
		'Overview of YUV Pixels' for more information.

HEADER:
nucleus/graphics.h

MEMBERS:
gaVideoRGB332				- Supports RGB 3:3:2 input format
gaVideoRGB555				- Supports RGB 5:5:5 input format
gaVideoRGB565				- Supports RGB 5:6:5 input format
gaVideoRGB888				- Supports RGB 8:8:8 input format
gaVideoRGB8888				- Supports RGB 8:8:8:8 input format
gaVideoYUV9					- Supports YUV9 input format
gaVideoYUV12				- Supports YUV12 input format
gaVideoYUV411				- Supports YUV411 input format
gaVideoYUV422				- Supports YUV422 input format
gaVideoYUV444				- Supports YUV444 input format
gaVideoYCrCb422				- Supports YCrCb422 input format
gaVideoYUYV					- Supports the YUYV pixel layout (for the above YUV formats)
gaVideoYVYU					- Supports the YVYU pixel layout (for the above YUV formats)
gaVideoUYVY					- Supports the UYVY pixel layout (for the above YUV formats)
gaVideoVYUY					- Supports the VYUY pixel layout (for the above YUV formats)
****************************************************************************/
typedef enum {
	gaVideoRGB332					= 0x00000001,
	gaVideoRGB555					= 0x00000002,
	gaVideoRGB565					= 0x00000004,
	gaVideoRGB888					= 0x00000008,
	gaVideoRGB8888					= 0x00000010,
	gaVideoYUV9						= 0x00000020,
	gaVideoYUV12					= 0x00000040,
	gaVideoYUV411					= 0x00000080,
	gaVideoYUV422					= 0x00000100,
	gaVideoYUV444					= 0x00000200,
	gaVideoYCrCb422					= 0x00000400,
	gaVideoYUYV						= 0x08000000,
	gaVideoYVYU						= 0x10000000,
	gaVideoUYVY						= 0x20000000,
	gaVideoVYUY						= 0x40000000,
	} GA_VideoInputFormatsType;

/****************************************************************************
REMARKS:
Flags for hardware video output capabilities defined in the VideoOutputFlags
member of the GA_videoInf structure. These flags define the hardware
video capabilities of the particular graphics mode, and are only valid
if the gaHaveAccelVideo flag is defined in the Attributes member of the
GA_modeInfo structure.

The gaVideoXInterp flag is used to determine whether the video overlay
window can support hardware interpolation or filtering in the X axis when
scaling the input image to the display. If this bit is 1, then the hardware
can support filtering of values in the X direction resulting in better
looking images when scaled from a smaller input frame.

The gaVideoYInterp flag is used to determine whether the video overlay
window can support hardware interpolation or filtering in the Y axis when
scaling the input image to the display. If this bit is 1, then the hardware
can support filtering of values in the Y direction resulting in better
looking images when scaled from a smaller input frame.

The gaVideoColorKeySrcSingle flag is used to determine whether the video
overlay window can support hardware source color keying with a single source
color key value. When hardware source color keying is enabled, any pixel
data in the incoming source video that matches the currently set video
color key will be ignored and not displayed on the screen, essentially
allowing the display data under the video overlay window to show through.

The gaVideoColorKeySrcRange flag is used to determine whether the video
overlay window can support hardware source color keying with a range of
color key values. This is the same as single source color keying, but
the color key values may be allows to fall within a range of available
colors (useful if data has been filtered causing the colors to change).

The gaVideoColorKeyDstSingle flag is used to determine whether the video
overlay window can support hardware destination color keying with a single
destination color key value. When hardware detination color keying is
enabled (sometimes called blue-screening), any destination pixels that the
overlay window overlaps that match the currently set video color key,
will cause the source input pixels to be ignored, essentially
allowing the display data under the video overlay window to show through.

The gaVideoColorKeyDstRange flag is used to determine whether the video
overlay window can support hardware destination color keying with a range of
color key values. This is the same as single destination color keying, but
the color key values may be allows to fall within a range of available
colors.

Note:	These flags are also passed to the SetVideoOutput function to
		determine what features are enabled for the output window.

HEADER:
nucleus/graphics.h

MEMBERS:
gaVideoXInterp				- Supports X interpolation
gaVideoYInterp				- Supports Y interpolation
gaVideoColorKeySrcSingle	- Supports source color keying, single color
gaVideoColorKeySrcRange		- Supports source color keying, range of colors
gaVideoColorKeyDstSingle	- Support destination color keying, single color
gaVideoColorKeyDstRange		- Support destination color keying, range of colors
****************************************************************************/
typedef enum {
	gaVideoXInterp					= 0x00000001,
	gaVideoYInterp					= 0x00000002,
	gaVideoColorKeySrcSingle  		= 0x00000004,
	gaVideoColorKeySrcRange   		= 0x00000008,
	gaVideoColorKeyDstSingle  		= 0x00000010,
	gaVideoColorKeyDstRange   		= 0x00000020,
	} GA_VideoOutputFlagsType;

/****************************************************************************
REMARKS:
Flags for hardware line drawing using the bresenham engine line draw
function.

HEADER:
nucleus/graphics.h

MEMBERS:
gaLineXMajor		- Line is X major (ie: longer in the X direction)
gaLineXPositive		- Direction of line is positive in X
gaLineYPositive		- Direction of line is positive in Y
gaLineDoLastPel		- Draw the last pixel in the line
****************************************************************************/
typedef enum {
	gaLineXMajor					= 0x00000001,
	gaLineXPositive					= 0x00000002,
	gaLineYPositive					= 0x00000004,
	gaLineDoLastPel					= 0x00000008,
	gaLineDoFirstPel				= 0x00000010,
	} GA_BresenhamLineFlagsType;

/* Applications can adjust the bias for the bresenham line drawing
 * functions by applying a small bias to the initial error terms. The bias
 * parameter is an OR'ing of the appropriate octant constants defined
 * below to indicate which octants to bias a line to prefer an axial step
 * when the Bresenham error term is exactly zero. By default Nucleus uses
 * a line bias of 0 for all octants, so you may need to adjust as necesary
 * to compensate for different rasterisation rules. The octants are mapped
 * as follows:
 *
 *   \    |    /
 *    \ 3 | 2 /
 *     \  |  /
 *    4 \ | / 1
 *       \|/
 *   -----------
 *       /|\
 *    5 / | \ 8
 *     /  |  \
 *    / 6 | 7 \
 *   /    |    \
 *
 * For more information, see "Ambiguities in Incremental Line Rastering,"
 * Jack E. Bresenham, IEEE CG&A, May 1987.
 */

#define gaLineOctant1	(1 << (gaLineXPositive | gaLineXMajor))
#define gaLineOctant2 	(1 << (gaLineXPositive))
#define gaLineOctant3 	(1 << (0))
#define gaLineOctant4	(1 << (gaLineXMajor))
#define gaLineOctant5 	(1 << (gaLineYPositive | gaLineXMajor))
#define gaLineOctant6 	(1 << (gaLineYPositive))
#define gaLineOctant7 	(1 << (gaLineXPositive | gaLineYPositive))
#define gaLineOctant8 	(1 << (gaLineXPositive | gaLineYPositive | gaLineXMajor))

#define	gaLineOctantMask \
	(gaLineXMajor | gaLineXPositive | gaLineYPositive)

/****************************************************************************
REMARKS:
Flags for hardware blitting with special effects, passed to the BltBltFx
family of functions. This family of functions exposes a wide variety of
special effects blitting if the hardware is capable of these functions.
You can determine what special effects are supported by the hardware by
examining the BitBltCaps member of the GA_modeInfo structure. However to
check whether a set of combined effects are supported, set the desired
effects flags in the GA_bltFx structure and call the BitBltFxTest function.
The driver will examine the passed in flags and return true if the
combination is supported, and false if not. Calling a BltBltFx function with
a combination of flags not supported by the hardware will produce undefined
results.

The gaBltMixEnable flag determines if the graphics mode supports arbitrary
mix modes for extended BitBlt functions.

The gaBltStretchNearest flag determines if the graphics mode supports
hardware stretch blitting, with nearest pixel filtering.

The gaBltStretchXFilter flag determines if the graphics mode supports
hardware stretch blitting, with linear interpolated filtering in the
X direction.

The gaBltStretchYFilter flag determines if the graphics mode supports
hardware stretch blitting, with linear interpolated filtering in the
Y direction.

The gaBltColorKeySrcSingle flag determines whether the graphics mode
supports hardware source transparent blitting with single source color key.
When hardware source color keying is enabled, any pixel data in the incoming
bitmap that matches the currently set color key will be ignored and not
displayed on the screen, essentially making those source pixels transparent.

The gaBltColorKeySrcRange flag determines whether the graphics mode
supports hardware source transparent blitting with a range of color keys.
This is the same as single source color keying, but the color key values may
be allows to fall within a range of available colors (useful if data has
been filtered causing the colors to shift slightly).

The gaBltColorKeyDstSingle flag determines whether the graphics mode
supports hardware destination transparent blitting with single destination color key.
When hardware detination color keying is enabled (sometimes called
blue-screening), any destination pixels in the framebuffer that match the
currently set color key, will cause the source input pixels to be ignored.

The gaBltColorKeyDstRange flag determines whether the graphics mode
supports hardware destination transparent blitting with a range of color keys.
This is the same as single destination color keying, but the color key values
may be allows to fall within a range of available colors (useful if data has
been filtered causing the colors to shift slightly).

The gaBltFlipX flag determines whether the graphics mode supports hardware
blitting with data flipped in the X axis. This is useful for 2D sprite
based games and animation where the same sprite data can be resused for
characters going left or right on the screen by flipping the data during
the blit operation.

The gaBltFlipY flag determines whether the graphics mode supports hardware
blitting with data flipped in the Y axis. This is useful for 2D sprite
based games and animation where the same sprite data can be resused for
characters going up or down on the screen by flipping the data during
the blit operation.

The gaBltBlend flag determines whether the hardware can support alpha
blended blit operations.

The gaBltConvert flag determines whether the hardware can support pixel
format conversion.

The gaBltClip flag determines whether the hardware can support clipping
while blitting is in effect. This is normally only used to implement proper
clipping for stretching operations, where software clipping can get
complicated.

The gaBltDither flag determines whether the closest color is selected, or
if dithering is used when blitting an RGB bitmap where the destination is
an 8bpp, 15bpp or 16bpp device context. Dithering slows things down somewhat
for 15/16bpp modes, but produces better quality. Dithering in 8bpp looks
best if a halftone palette is used, and in fact is a lot faster than
using the closest color method. Dithering in 8bpp will however map to any
palette, but the quality is best if a halftone palette is used.

Note:	These flags are also passed to the BitBltFx family of functions to
		define the type of extended Blt that should be performed, as well
		as reporting the available capabilities via the GA_bltFx structure
		stored in the GA_modeInfo structure.

Note: 	Availabiliy of some features may be mututally exclusive on other
		features. Hence you /must/ call BitBltFxTest first to find out if
		the set of features that you require are all supported at the same
		time before attempting to perform an extended BitBlt operation.

Note: 	In many cases stretching with X filtering is relatively cheap,
		while Y filtering is more expensive. Hence it may be faster on
		some hardware to enable only X filtering and not Y filteringto get
		improved performance.

HEADER:
nucleus/graphics.h

MEMBERS:
gaBltMixEnable			- Mix code enabled, defined in GA_bltFx structure
gaBltStretchNearest		- Enable stretching, nearest pixel
gaBltStretchXFilter		- Enable X axis filtering for stretch blit
gaBltStretchYFilter		- Enable Y axis filtering for stretch blit
gaBltColorKeySrcSingle	- Source color keying enabled, single color
gaBltColorKeySrcRange	- Source color keying enabled, range of colors
gaBltColorKeyDstSingle	- Destination color keying enabled, single color
gaBltColorKeyDstRange	- Destination color keying enabled, range of colors
gaBltFlipX				- Enable flip in X axis
gaBltFlipY				- Enable flip in Y axis
gaBltBlend				- Enable alpha blending
gaBltConvert			- Enable pixel format/palette conversion
gaBltClip				- Clip to destination clip rectangle
gaBltDither				- Dither if an 8/15/16bpp destination
****************************************************************************/
typedef enum {
	gaBltMixEnable					= 0x00000001,
	gaBltStretchNearest				= 0x00000002,
	gaBltStretchXInterp				= 0x00000004,
	gaBltStretchYInterp				= 0x00000008,
	gaBltColorKeySrcSingle  		= 0x00000010,
	gaBltColorKeySrcRange   		= 0x00000020,
	gaBltColorKeyDstSingle  		= 0x00000040,
	gaBltColorKeyDstRange   		= 0x00000080,
	gaBltFlipX              		= 0x00000100,
	gaBltFlipY              		= 0x00000200,
	gaBltBlend						= 0x00000400,
	gaBltConvert					= 0x00000800,
	gaBltClip						= 0x00001000,
	gaBltDither						= 0x00002000,
	} GA_BitBltFxFlagsType;

/* Convenience macros for BitBltFx flags */

#define	gaAnyColorKey	(gaBltColorKeySrcSingle | gaBltColorKeySrcRange | gaBltColorKeyDstSingle | gaBltColorKeyDstRange)

/****************************************************************************
REMARKS:
Definitions for flags member of the GA_CRTCInfo structure. These flags
define the different flags required to complete a mode set with refresh
rate control enabled.

The gaInterlaced flag is used to determine whether the mode programmed into
the hardware is interlaced or non-interlaced. The CRTC timings passed in
will be identical for both interlaced and non-interlaced modes, and it is up
to the graphics driver to perform any necessary scaling between the hardware
values and the normalized CRTC values in interlaced modes. Note that you must
check the gaHaveInterlaced bit in the GA_modeInfo structure to determine if
interlaced mode is supported before attempting to initialise an interlaced
mode.

The gaDoubleScan flag is used to determine whether the mode programmed into
the hardware is double scanned or not. When double scanning is specified,
the vertical CRTC values passed in will be double what the real vertical
resolution will be. Double scanning is used to implement the 200, 240 and
300 line graphics modes on modern controllers. Note that you must check the
gaHaveDoubleScan bit in the GA_modeInfo structure to determine if double scan
mode is supported by the hardware in that display mode. Note also that all
modes with vertical resolutions below 300 scanline modes require the double
scanning to be enabled, and modes between 300 and 400 scanlines can usually
look better if it is enabled.

The gaHSyncNeg flag is used to determine if the horizontal sync polority
should be set to a negative sync (gaHSyncNeg is set) or positive sync
(gaHSyncNeg is not set).

The gaVSyncNeg flag is used to determine if the vertical sync polority
should be set to a negative sync (gaVSyncNeg is set) or positive sync
(gaVSyncNeg is not set).

HEADER:
nucleus/graphics.h

MEMBERS:
gaInterlaced	- Enable interlaced mode
gaDoubleScan	- Enable double scanned mode
gaTripleScan	- Enable triple scanned mode
gaHSyncNeg		- Horizontal sync is negative
gaVSyncNeg		- Vertical sync is negative
****************************************************************************/
typedef enum {
	gaInterlaced	= 0x01,
	gaDoubleScan	= 0x02,
	gaTripleScan	= 0x04,
	gaHSyncNeg		= 0x08,
	gaVSyncNeg		= 0x10,
	} GA_CRTCInfoFlagsType;

/* Flags for common sync polarity combinations */

#define	ga350Line		gaSyncVNeg
#define	ga400Line		gaSyncHNeg
#define	ga480Line		(gaSyncHNeg | gaSyncVNeg)

/****************************************************************************
REMARKS:
Hardware video window information. There is a single structure for each
available hardware video window, and defines the capabilities of that
hardware video window. The VideoMinXScale and VideoMinYScale members defines
the inverse of minimum scale ratio supported by the hardware in the each
direction. For instance if the value is 4, the hardware can only scale video
down to a window that is 1/4 the size of the source input video.
The VideoMaxXScale and VideoMaxYScale define the maxium scale ratio supported
by the hardware. For instance if the value is 4, the hardware can only scale
video data up to a window that 4 times the size of the source input video.

HEADER:
nucleus/graphics.h

MEMBERS:
dwSize				- Set to size of structure in bytes
VideoInputFormats	- Hardware video input format flags
VideoOutputFlags	- Hardware video output format capabilities
VideoMinXScale		- Minimum X scale factor (1/value)
VideoMinYScale		- Minimum Y scale factor (1/value)
VideoMaxXScale		- Maximum X scale factor
VideoMaxYScale		- Maximum Y scale factor
****************************************************************************/
typedef struct {
	N_uint32	dwSize;
	N_uint32	VideoInputFormats;
	N_uint32	VideoOutputFlags;
	N_uint16	VideoMinXScale;
	N_uint16	VideoMinYScale;
	N_uint16	VideoMaxXScale;
	N_uint16	VideoMaxYScale;
	} GA_videoInf;

/****************************************************************************
REMARKS:
Palette entry structure, which defines a single entry in the hardware
color lookup table or gamma correction table.

HEADER:
nucleus/graphics.h

MEMBERS:
blue    - Blue component of palette entry, range [0-255]
green   - Green component of palette entry, range [0-255]
red     - Blue component of palette entry, range [0-255]
alpha	- Alpha or alignment byte
****************************************************************************/
typedef struct {
	N_uint8    	Blue;
	N_uint8    	Green;
	N_uint8    	Red;
	N_uint8    	Alpha;
	} GA_palette;

/****************************************************************************
REMARKS:
Extended palette entry structure, which defines a single entry in the
hardware color lookup table or gamma correction table.

HEADER:
nucleus/graphics.h

MEMBERS:
blue    - Blue component of palette entry, range [0-65535]
green   - Green component of palette entry, range [0-65535]
red     - Blue component of palette entry, range [0-65535]
****************************************************************************/
typedef struct {
	N_uint16	Blue;
	N_uint16    Green;
	N_uint16    Red;
	} GA_paletteExt;

/****************************************************************************
REMARKS:
Structure representing the format of an RGB pixel. This structure is used
to describe the RGB pixel format for Nucleus graphics modes, as well as the
pixel format for system memory buffers converted on the fly by Nucleus to
the destination pixel format. RGB pixel formats are required for pixel
depths greater than or equal to 15-bits per pixel. The pixel formats for 15
and 16-bit modes are constant and never change, however there are 2 possible
pixel formats for 24 bit RGB modes and 4 possible formats for 32 bit RGB
modes that are supported by the MGL. The possible modes for 24-bits per
pixel are:

24-bit	- Description
RGB		- Values are packed with Red in byte 2, Green in byte 1 and Blue
		  in byte 0. This is the standard format used by all 24 bit
		  Windows BMP files, and the native display format for most
		  graphics hardware on the PC.
BGR		- Values are packed with Blue in byte 2, Green in byte 1 and Red
		  in byte 0. This format is the native display format for some
		  graphics hardware on the PC.

The possible modes for 32-bits per pixel are:

32-bit	- Description
ARGB	- Values are packed with Red in byte 2, Green in byte 1 and Blue
		  in byte 0 and alpha in byte 3.
ABGR	- Values are packed with Blue in byte 2, Green in byte 1 and Red
		  in byte 0 and alpha in byte 3.
RGBA	- Values are packed with Red in byte 3, Green in byte 2 and Blue
		  in byte 1 and alpha in byte 0.
BGRA	- Values are packed with Blue in byte 3, Green in byte 2 and Red
		  in byte 1 and alpha in byte 0.

If you intend to write your own direct rendering code for RGB graphics modes,
you will need to write your code so that it will adapt to the underlying
pixel format used by the hardware to display the correct colors on the
screen. Nucleus has the ability to perform pixel format translation on the
fly using the ConvertBlt family of functions, but this can be time consuming
so directly rendering in the native pixel format can be more efficient.
The formula for packing the pixel data into the proper positions given three
8-bit RGB values is as follows:

	color = ((GA_color)((R >> RedAdjust) & RedMask)
			<< RedPosition)
		  | ((GA_color)((G >> GreenAdjust) & GreenMask)
			<< GreenPosition)
		  | ((GA_color)((B >> BlueAdjust) & BlueMask)
			<< BluePosition);

Alternatively you can unpack the color values from the framebuffer with the
following code (note that you lose precision when unpacking values from the
framebuffer since the bottom bits always get set to 0):

	R = (((color) >> RedPosition) & RedMask)
		<< RedAdjust;
	G = (((color) >> GreenPosition) & GreenMask)
		<< GreenAdjust;
	B = (((color) >> BluePosition) & BlueMask)
		<< BlueAdjust;

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

HEADER:
nucleus/graphics.h

MEMBERS:
RedMask     	- Unshifted 8-bit mask for the red color channel
RedPosition		- Bit position for bit 0 of the red color channel information
RedAdjust		- Number of bits to shift the 8-bit red value right
GreenMask		- Unshifted 8-bit mask for the green color channel
GreenPosition	- Bit position for bit 0 of the green color channel information
GreenAdjust		- Number of bits to shift the 8-bit green value right
BlueMask		- Unshifted 8-bit mask for the blue color channel
BluePosition	- Bit position for bit 0 of the blue color channel information
BlueAdjust		- Number of bits to shift the 8-bit blue value right
RsvdMask		- Unshifted 8-bit mask for the reserved or alpha channel
RsvdPosition	- Bit position for bit 0 of the reserved color channel information
RsvdAdjust		- Number of bits to shift the 8-bit reserved value right
****************************************************************************/
typedef struct {
	N_uint8	RedMask;
	N_uint8	RedPosition;
	N_uint8	RedAdjust;
	N_uint8	GreenMask;
	N_uint8	GreenPosition;
	N_uint8	GreenAdjust;
	N_uint8	BlueMask;
	N_uint8	BluePosition;
	N_uint8	BlueAdjust;
	N_uint8	RsvdMask;
	N_uint8	RsvdPosition;
	N_uint8	RsvdAdjust;
	} GA_pixelFormat;

/****************************************************************************
REMARKS:
Hardware 2D BitBltFx information structure. This structure defines the type
of BitBlt operation tha is performed by the BitBltFx family of functions.
The Flags member defines the type of BitBlt operation to be performed, and
can be any combination of the supported flags (be sure to call BitBltFxTest
first to determine if that combination of effects is supported).

If mixes are enabled, the Mix member is used to determine the mix operation
to apply. If mixes are not enabled, GA_REPLACE_MIX is assumed (some hardware
may not support mix operations for effects blits).

The ColorKeyLo and ColorKeyHi members define the color key ranges if range
based color keying is selected. If only a single color key is enabled,
the ColorKeyLo value is the value used as the color key.

If blending is enabled, the SrcBlendFunc, DstBlendFunc and Alpha values
are used to implement the blending operation.

If clipping is enabled, the destination clip rectangle is passed in the
ClipLeft, ClipTop, ClipRight and ClipBottom members. Clipping is most
useful for stretching operations, where clipping in software is problematic.

Note:	The dwSize member is intended for future compatibility, and
		should be set to the size of the structure as defined in this
		header file. Future drivers will be compatible with older software
		by examiming this value.

HEADER:
nucleus/graphics.h

MEMBERS:
dwSize			- Set to size of structure in bytes
Flags			- BitBltFx flags to define the type of BitBlt operation
Mix				- Logical mix operation (if mixes enabled)
ColorKeyLo		- Color key low value of range (if color keying enabled)
ColorKeyHi		- Color key high value of range (if color keying enabled)
SrcBlendFunc	- Src blend function (GA_blendFuncType)
DstBlendFunc	- Dst blend function (GA_blendFuncType)
Alpha			- Constant alpha blend factor (0-255 if blending enabled)
BitsPerPixel	- Color depth for the source bitmap
PixelFormat		- Pixel format for the source bitmap
DstPalette		- Color index palette for destination (if destination BitsPerPixel = 8)
SrcPalette		- Color index palette for source bitmap (if source BitsPerPixel = 8)
ClipLeft		- Left coordinate for destination clip rectangle
ClipTop			- Top coordinate for destination clip rectangle
ClipRight		- Right coordinate for destination clip rectangle
ClipBottom		- Bottom coordinate for destination clip rectangle
****************************************************************************/
typedef struct {
	N_uint32		dwSize;
	N_uint32		Flags;
	N_int32			Mix;
	GA_color		ColorKeyLo;
	GA_color		ColorKeyHi;
	N_int32         SrcBlendFunc;
	N_int32         DstBlendFunc;
	N_uint32		Alpha;
	N_int32 		BitsPerPixel;
	GA_pixelFormat	PixelFormat;
	GA_palette		*DstPalette;
	GA_palette		*SrcPalette;
	N_int32			ClipLeft;
	N_int32			ClipTop;
	N_int32			ClipRight;
	N_int32			ClipBottom;
	} GA_bltFx;

/* {secret} */
struct _GA_3DState;

/****************************************************************************
REMARKS:
Graphics mode information block. This structure contains detailed information
about the capabilities and layout of a specific graphic mode.

The Attributes member contains a number of flags that describes certain
important characteristics of the graphics mode, and the values this member
contains are defined in the GA_AttributeFlagsType.

The XResolution and YResolution specify the width and height in pixel
elements for this display mode.

The BytesPerScanLine member specifies how many full bytes are in each logical
scanline. The logical scanline could be equal to or larger than the displayed
scanline, and can be changed when the display mode is first initialized.

The MaxBytesPerScanLine and MaxScanLineWidth members define the maximum
virtual framebuffer coordinates that can be initialised for the mode, in
both bytes and pixels respectively. If an attempt is made to initialize a
graphics mode with values larger than these values, the mode set will fail.

The MaxScanLines member holds the total number of scanlines available in
that graphics mode when initialised with the default scanline width. This
field combined with BytesPerScanLine can be used to determine the maximum
addressable display memory for drawing operations. This can also be used to
determine how large a virtual screen image can be before initialising a
graphics mode. This field also determines the addressable limit for
X/Y based drawing functions in offscreen video memory.

The LinearHeapStart member determines the start of the linear only heap,
if one is available. Some hardware has restrictions on the addressable memory
for the (x,y) coordinates passed to the 2D drawing functions. If the
hardware supports DrawRectLin and the BitBltLin family of functions, the
memory past the (x,y) coordinate restriction can be accessed using those
functions. Hence this member determines the start of this linear only
heap as a byte offset from the beginning of display memory. Memory in the
linear only heap can only be accessed directly via the linear framebuffer,
or using DrawRectLin or the BitBitLin family of blitting functions. None of
the X/Y based drawing functions can be used to draw to the linear only heap.

The MaxLinearOffset member hold the maximum addressable display memory
offset for linear drawing functions (DrawRectLin, BitBltLin etc). If the
hardware has restrictions on the addressable memory for the X/Y drawing
functions, the linear only heap resides between LinearHeapStart and
MaxLinearOffset. If the maximum addressable scanline value for a display
mode is past the end of display memory (common for high resolution modes),
then LinearHeapStart = MaxLinearOffset which indicates that there is no
linear only heap for that display mode.

NOTE:	There may well be some memory used by the graphics hardware
		for caching the hardware cursor, patterns and other data between
		the end of (MaxScanLines * BytesPerScanLine) and LinearHeapStart.
		Hence the application software must /never/ write to the memory
		between (MaxScanLines * BytesPerScanLine) and LinearHeapStart.

The BitsPerPixel member specifies the number of bits per pixel for this
display mode. For 5:5:5 format RGB modes this should contain a value of 15,
and for 5:6:5 format RGB modes this should contain a value of 16. For 8:8:8
bit RGB modes this should contain a value of 24 and for 8:8:8:8 RGBA modes
this should contain a value of 32. For 24 and 32bpp modes, the application
should look at the pixel format mask values (see below) to determine the
actual format of the pixels within the display buffer.

The MaxBuffers member specified the maximum number of display buffers that
can be allocated in video memory for page flipping. This value is a
convenience function only, and can be computed manually from the value
of MaxScanLines / YResolution.

The MaxPixelClock member specifies the maximum possible pixel dot clock that
can be selected in this display mode when a refresh controlled mode is
selected. Any attempt to select a higher pixel clock will cause the mode set
to fail. This member can be used to determine what the maximum available
refresh rate for the display mode will be.

The RedMaskSize, GreenMaskSize, BlueMaskSize and RsvdMaskSize members define
the size, in bits, of the red, green, and blue components of an RGB pixel
respectively. A bit mask can be constructed from the MaskSize members using
simple shift arithmetic. For example, the MaskSize values for an RGB 5:6:5
mode would be 5, 6, 5, and 0, for the red, green, blue, and reserved members
respectively.

The RedFieldPosition, GreenFieldPosition, BlueFieldPosition and
RsvdFieldPosition members define the bit position within the RGB pixel of
the least significant bit of the respective color component. A color value
can be aligned with its pixel member by shifting the value left by the
FieldPosition. For example, the FieldPosition values for an RGB 5:6:5 mode
would be 11, 5, 0, and 0, for the red, green, blue, and reserved members
respectively.

The BitmapStartAlign member defines the alignment requirements in bytes for
offscreen memory bitmaps for this graphics mode. If the value in here is
set to 8 for instance, then the start for all offscreen bitmaps must be
aligned to an 8 byte boundary in order to be used for offscreen bitmap
blitting. Note that the BitmapStartAlign member also defines the alignment
requirements for all buffers passed to the SetDrawBuffer function.

The BitmapStridePad member defines the alignment requirements in bytes for
the stride of offscreen memory bitmaps (the number of bytes to move from
one line of the bitmap to the next). If the value in here is set to 8 for
instance, then the number of bytes for each scanline in the offscreen
bitmap must be padded out to a multiple of 8 (inserting zeros if necessary
when downloading a source bitmap to offscreen memory). Note that the
BitmapStridePad member also defines the padding requirements for all
buffers passed to the SetDrawBuffer function.

The MonoBitmapStartAlign member defines the alignment requirements in bytes
for monochrome offscreen memory bitmaps for this graphics mode, which are
used by the PutMonoImageMSBLin and PutMonoImageLSBLin functions. If the
value in here is set to 8 for instance, then the start for all monochrome
offscreen bitmaps must be aligned to an 8 byte boundary in order to be used
by the PutMonoImageMSBLin and PutMonoImageLSBLin functions.

The MonoBitmapStridePad member defines the alignment requirements in bytes
for the stride of monochrome offscreen memory bitmaps (the number of bytes
to move from one line of the bitmap to the next). If the value in here is
set to 8 for instance, then the number of bytes for each scanline in the
monochrome offscreen bitmap must be padded out to a multiple of 8 (inserting
zeros if necessary when downloading a source bitmap to offscreen memory).

The RefreshRateList member contains a list of all valid refresh rates
supported by the display mode which can be passed to the SetVideoMode
function. Interlaced modes are indicated by a negative refresh rate
(ie: 48Hz Interlaced is -48). The current default refresh rate is stored
in the DefaultRefreshRate member, and except for special circumstances the
default refresh rate set by the user should be used rather than overriding
the refresh rate.

The BitBltCaps member defines the extended hardware BitBlt capabilities
for the graphics mode, and points to a static GA_bltFx structure. Refer
to the documentation of GA_bltFx to determine what this structure contains.

The VideoWindows member defines the hardware video capabilities for each of
up to a maximum number of hardware video overlay windows. The list of
hardware video overlay window capabilities is terminated with a NULL pointer.
For instance if only 2 hardware video windwos are supported, the first two
entries in this array would point to valid GA_videoInf structures, while the
third would contain a NULL terminating the list. Refer to the documentation
of GA_videoInf to determine what these structures contains.

The HW3DCaps member defined the hardware 3D capabilities for the graphics
mode, and points to a static GA_3DState structure. Refer to the
documentation of GA_3DState to determine what this structure contains.

The LinearSize member is the 32-bit length of the linear frame buffer memory
in bytes. In can be any length up to the size of the available video memory.
The LinearBasePtr member is the 32-bit physical address of the start of frame
buffer memory when the controller is in linear frame buffer memory mode for
this particular graphics mode. If the linear framebuffer is not available,
then this member will be zero.

Note:	The LinearSize and LinearBasePtr members are duplicated in the mode
		information block because they may possibly change locations in
		memory depending on the display mode. Normally applications will
		always use the value stored in the the GA_devCtx LinearMem pointer
		to directly access the framebuffer (which is automatically adjusted
		for you), however if the information about the framebuffer starting
		address needed to be reported to other applications directly, the
		values stored in this mode information block should be used.

Note:	The memory pointed to by the RefreshRateList, BitBltCaps,
		VideoWindows and HW3DCaps fields will be reused the next time
		GetVideoModeInfo is called, so do not rely on the information in
		these fields to remain the same across calls to this function.

Note:	The dwSize member is intended for future compatibility, and
		should be set to the size of the structure as defined in this
		header file. Future drivers will be compatible with older software
		by examiming this value.

HEADER:
nucleus/graphics.h

MEMBERS:
dwSize				- Set to size of structure in bytes
Attributes			- Mode attributes
XResolution			- Horizontal resolution in pixels
YResolution			- Vertical resolution in pixels
XCharSize			- Character cell X dimension for text modes
YCharSize			- Character cell Y dimension for text modes
BytesPerScanLine	- Bytes per horizontal scan line
MaxBytesPerScanLine	- Maximum bytes per scan line
MaxScanLineWidth	- Maximum pixels per scan line
MaxScanLines		- Maximum number of scanlines for default scanline width
LinearHeapStart		- Start of linear only heap (if any)
MaxLinearOffset		- Maximum display memory offset for linear drawing
BitsPerPixel		- Bits per pixel
PixelFormat			- Pixel format for the display mode
MaxBuffers			- Maximum number of display buffers
MaxPixelClock		- Maximum pixel clock for mode
DefaultRefreshRate	- Currently active default refresh rate
RefreshRateList		- List of all valid refresh rates terminated with -1.
BitmapStartAlign	- Linear bitmap start alignment in bytes
BitmapStridePad		- Linear bitmap stride pad in bytes
MonoBitmapStartAlign - Linear bitmap start alignment in bytes
MonoBitmapStridePad	- Linear bitmap stride pad in bytes
BitBltCaps			- Hardware 2D BitBltFx capabilities
VideoWindows		- Up to 8 hardware video overlays
HW3DCaps			- Hardware 3D capabilities
MaxOOZ				- Maximum ooz coordinate value in floating point
MaxOOW				- Maximum oow coordinate value in floating point
MaxOOS				- Maximum one over s coordinate value in floating point
MaxOOT				- Maximum one over t coordinate value in floating point
DepthFormats		- Depth buffer formats flags
DepthStartAlign		- Depth buffer start alignment in bytes
DepthStridePad		- Depth buffer stride pad in bytes
TextureFormats		- Texture formats flags
TextureStartAlign	- Texture start alignment in bytes
TextureStridePad	- Texture stride pad in bytes
TextureMaxX			- Maximum texture X dimension
TextureMaxY			- Maximum texture Y dimension
TextureMaxAspect	- Maximum texture aspect ratio (1:x)
StencilFormats		- Stencil buffer formats flags
StencilStartAlign	- Stencil buffer start alignment in bytes
StencilStridePad	- Stencil buffer stride pad in bytes
LinearSize         	- Linear buffer size in bytes
LinearBasePtr      	- Physical addr of linear buffer
****************************************************************************/
typedef struct {
	N_uint32    	dwSize;
	N_uint32   		Attributes;
	N_uint16   		XResolution;
	N_uint16   		YResolution;
	N_uint8			XCharSize;
	N_uint8			YCharSize;
	N_uint32   		BytesPerScanLine;
	N_uint32   		MaxBytesPerScanLine;
	N_uint32   		MaxScanLineWidth;
	N_uint32		MaxScanLines;
	N_uint32		LinearHeapStart;
	N_uint32		MaxLinearOffset;
	N_uint16   		BitsPerPixel;
	GA_pixelFormat  PixelFormat;
	N_uint16   		MaxBuffers;
	N_uint32   		MaxPixelClock;
	N_int32			DefaultRefreshRate;
	N_int32			_FAR_ *RefreshRateList;
	N_uint32   		BitmapStartAlign;
	N_uint32   		BitmapStridePad;
	N_uint32   		MonoBitmapStartAlign;
	N_uint32   		MonoBitmapStridePad;
	GA_bltFx    	_FAR_ *BitBltCaps;
	GA_videoInf 	_FAR_ * _FAR_ *VideoWindows;
	struct _GA_3DState	_FAR_ *HW3DCaps;
	N_flt32			MaxOOZ;
	N_flt32			MaxOOW;
	N_flt32			MaxOOS;
	N_flt32			MaxOOT;
	N_uint32   		DepthFormats;
	N_uint32   		DepthStartAlign;
	N_uint32   		DepthStridePad;
	N_uint32   		TextureFormats;
	N_uint32   		TextureStartAlign;
	N_uint32   		TextureStridePad;
	N_uint32   		TextureMaxX;
	N_uint32   		TextureMaxY;
	N_uint16   		TextureMaxAspect;
	N_uint32   		StencilFormats;
	N_uint32   		StencilStartAlign;
	N_uint32   		StencilStridePad;
	N_uint32   		LinearSize;
	N_uint32   		LinearBasePtr;
	} GA_modeInfo;

/****************************************************************************
REMARKS:
CRTC information block for refresh rate control, passed in to the
SetVideoMode function.

The HorizontalTotal, HorizontalSyncStart, HorizontalSyncEnd, VerticalTotal,
VerticalSyncStart and VerticalSyncEnd members define the default normalized
CRTC values that will be programmed if the gaRefreshCtl flag is passed to
SetVideoMode. The CRTC values for a particular resolution will always be the
same regardless of color depth. Note also that the CRTC table does not
contain any information about the horizontal and vertical blank timing
positions. It is up the the driver implementation to determine the correct
blank timings to use for the mode when it is initialized depending on the
constraints of the underlying hardware (some hardware does not require this
information, and most VGA compatible hardware can be very picky about the
values programmed for the blank timings).

The Flags member defines the flags that modify the operation of the mode,
and the values for this member are defined in the GA_CRTCInfoFlagsType
enumeration.

The PixelClock member defines the normalized pixel clock that will be
programmed into the hardware. This value is represented in a 32 bit unsigned
integer in units of Hz. For example to represent a pixel clock of 25.18Mhz
one would code a value of 25,180,000. From the pixel clock and the horizontal
and vertical totals, you can calculate the refresh rate for the specific
graphics mode using the following formula:

	refresh rate = (PixelClock * 10,000) /
				   (HorizontalTotal * VerticalTotal)

For example a 1024x768 mode with a HTotal of 1360, VTotal of 802, a pixel
clock of 130Mhz might be computed as follows:

	refresh rate = (130 * 10,000) / (1360 * 802)
				 = 59.59 Hz

The RefreshRate field defines the refresh rate that the CRTC information
values define. This value may not actually be used by the driver but must be
calculated by the application program using the above formulas before
initializing the mode. This entry may be used by the driver to identify any
special cases that may need to be handled when setting the mode for specific
refresh rates.  The value in this field should be represented in units if
0.01 Hz (ie: a value 7200 represents a refresh rate of 72.00Hz).

Note:	The dwSize member is intended for future compatibility, and
		should be set to the size of the structure as defined in this
		header file. Future drivers will be compatible with older software
		by examiming this value.

HEADER:
nucleus/graphics.h

MEMBERS:
HorizontalTotal    	- Horizontal total (pixels)
HorizontalSyncStart	- Horizontal sync start position
HorizontalSyncEnd  	- Horizontal sync end position
VerticalTotal      	- Vertical Total (lines)
VerticalSyncStart  	- Vertical sync start position
VerticalSyncEnd    	- Vertical sync end position
PixelClock         	- Pixel clock in units of Hz
RefreshRate        	- Expected refresh rate in .01Hz
Flags              	- Initialisation flags for mode
****************************************************************************/
typedef struct {
	N_uint16   	HorizontalTotal;
	N_uint16   	HorizontalSyncStart;
	N_uint16   	HorizontalSyncEnd;
	N_uint16   	VerticalTotal;
	N_uint16   	VerticalSyncStart;
	N_uint16   	VerticalSyncEnd;
	N_uint32   	PixelClock;
	N_uint16  	RefreshRate;
	N_uint8    	Flags;
	} GA_CRTCInfo;

/****************************************************************************
REMARKS:
Structure to maintain the information about the current display mode. This
structure is used internally by Nucleus drivers, and is only defined here
so that we can manage the shared state structure in shared environments.

HEADER:
nucleus/graphics.h

MEMBERS:
bytesPerLine   	- Bytes per logical scanline
xRes           	- Horizontal pixel resolution
yRes           	- Vertical pixel resolution
bits          	- Color depth per pixel (0 = text mode)
flags        	- Mode flags defining the mode capabilities
{secret}
****************************************************************************/
typedef struct {
	short  		bytesPerLine;
	short   	xRes;
	short   	yRes;
	uchar    	bits;
	ulong    	flags;
	} GA_modeEntry;

/****************************************************************************
REMARKS:
Structure to maintain global state information shared between instances
of Nucleus drivers when running in shared environments. This allows the
separate Nucleus driver instances to keep in sync with each other. This
information is not used externally by Nucleus applications, but only
internally by the Nucleus drivers.

HEADER:
nucleus/graphics.h

MEMBERS:
reloadCRTC	- True if the CRTC tables should be reloaded on the next mode set
cntModeNum	- Currently active Nucleus mode number
cntMode		- Entry containing information about the current display mode
cntModeInfo	- Full GA_modeInfo structure for the display mode
cntCRTC		- Copy of the current CRTC timings in use
{secret}
****************************************************************************/
typedef struct {
	N_int32			reloadCRTC;
	N_int32			cntModeNum;
	GA_modeEntry	cntMode;
	GA_modeInfo		cntModeInfo;
	GA_CRTCInfo		cntCRTC;
	} GA_sharedInfo;

/****************************************************************************
REMARKS:
Generic graphics buffer parameter block. This structure defines a generic
buffer in offscreen video memory, and is passed to the driver to allocate
such buffers for rendering operations. The Offset member is the offset
of the start of the buffer in video memory, local depth buffer or stencil
buffer memory depending on the buffer type (some hardware has separate
buffer regions for framebuffer, depth buffer and texture memory etc). The
Stride member defines the stride of the buffer in bytes, while the Width and
Height members define the dimensions of the buffer in logical pixel units.

Note:	All buffers are in packed pixel format, and the values of the Offset
		and Stride members must adhere to the format restrictions defined in
		the GA_modeInfo structure for the buffer type being enabled.

Note:	The dwSize member is intended for future compatibility, and
		should be set to the size of the structure as defined in this
		header file. Future drivers will be compatible with older software
		by examiming this value.

HEADER:
nucleus/graphics.h

MEMBERS:
dwSize     		- Set to size of structure in bytes
Offset    		- Buffer starting address in bytes
Stride			- Stride of the buffer in bytes (bytes for a line of data)
Width      		- Width of buffer in pixels
Height     		- Height of the buffer in pixels
****************************************************************************/
typedef struct {
	N_uint32   		dwSize;
	N_int32    		Offset;
	N_int32    		Stride;
	N_int32    		Width;
	N_int32    		Height;
	} GA_buffer;

/****************************************************************************
REMARKS:
Hardware monochrome cursor structure. This structure defines a monochrome hardware
cursor that is downloaded to the hardware. The cursor is defined as a 64x64
image with an AND and XOR mask. The definition of the AND mask, XOR mask and
the pixels that will appear on the screen is as follows (same as the
Microsoft Windows cursor format):

	AND XOR	Result
	0   0	Transparent (color from screen memory)
	0   1 	Invert (complement of color from screen memory)
	1   0 	Cursor background color
	1   1 	Cursor foreground color

The HotX and HotY members define the /hot spot/ for the cursor, which is
the location where the logical mouse pointer is located in the cursor
image. When you click the mouse, the pixel under the hot-spot is the pixel
selected.

HEADER:
nucleus/graphics.h

MEMBERS:
XORMask		- Cursor XOR mask
ANDMask		- Cursor AND mask
HotX        - Cursor X coordinate hot spot
HotY       	- Cursor Y coordinate hot spot
****************************************************************************/
typedef struct {
	N_uint8   	XORMask[512];
	N_uint8   	ANDMask[512];
	N_uint32   	HotX;
	N_uint32   	HotY;
	} GA_monoCursor;

/****************************************************************************
REMARKS:
Hardware 16-color cursor structure. This structure defines a color hardware
cursor that is downloaded to the hardware. The cursor is defined as a 64x64
image with an AND mask and color data. The definition of the AND mask,
cursor data and the pixels that will appear on the screen is as follows:

	AND Color	Result
	0   00		Transparent (color from screen memory)
	0   0F 		Invert (complement of color from screen memory)
	1   xx 		Cursor color data

Hence if the AND mask is a zero the color data should be either 00 or 0F to
either make the pixel transparent or the inversion of the screen pixel. Any
other value will produce an undefined result.

The color data is passed down to the driver as 4-bit packed color index
values, along with a 16-color lookup table containing the real 24-bit RGB
color values for the cursor image. It is up to the calling application to
translate and quantise cursor images of higher color depths down to the
format supported by the hardware.

The HotX and HotY members define the /hot spot/ for the cursor, which is
the location where the logical mouse pointer is located in the cursor
image. When you click the mouse, the pixel under the hot-spot is the pixel
selected.

HEADER:
nucleus/graphics.h

MEMBERS:
ColorData	- Cursor color data as a 64x64 array of packed 4-bit pixels
ANDMask		- Cursor AND mask
Palette		- 16-color palette for cursor image
HotX        - Cursor X coordinate hot spot
HotY       	- Cursor Y coordinate hot spot
****************************************************************************/
typedef struct {
	N_uint8		ColorData[2048];
	N_uint8   	ANDMask[512];
	GA_palette	Palette[16];
	N_uint32   	HotX;
	N_uint32   	HotY;
	} GA_colorCursor;

/****************************************************************************
REMARKS:
Hardware 256-color cursor structure. This structure defines a color hardware
cursor that is downloaded to the hardware. The cursor is defined as a 64x64
image with an AND mask and color data. The definition of the AND mask,
cursor data and the pixels that will appear on the screen is as follows:

	AND Color	Result
	0   00		Transparent (color from screen memory)
	0   FF 		Invert (complement of color from screen memory)
	1   xx 		Cursor color data

Hence if the AND mask is a zero the color data should be either 00 or FF to
either make the pixel transparent or the inversion of the screen pixel. Any
other value will produce an undefined result.

The color data is passed down to the driver as 8-bit packed color index
values, along with a 256-color lookup table containing the real 24-bit RGB
color values for the cursor image. It is up to the calling application to
translate and quantise cursor images of higher color depths down to the
format supported by the hardware.

The HotX and HotY members define the /hot spot/ for the cursor, which is
the location where the logical mouse pointer is located in the cursor
image. When you click the mouse, the pixel under the hot-spot is the pixel
selected.

HEADER:
nucleus/graphics.h

MEMBERS:
ColorData	- Cursor color data as a 64x64 array of packed 8-bit pixels
ANDMask		- Cursor AND mask
Palette		- 256-color palette for cursor image
HotX        - Cursor X coordinate hot spot
HotY       	- Cursor Y coordinate hot spot
****************************************************************************/
typedef struct {
	N_uint8		ColorData[4096];
	N_uint8   	ANDMask[512];
	GA_palette	Palette[256];
	N_uint32   	HotX;
	N_uint32   	HotY;
	} GA_colorCursor256;

/****************************************************************************
REMARKS:
Hardware 24-bit RGBA alpha blended cursor structure. This structure defines
a color hardware cursor that is downloaded to the hardware. The cursor is
defined as a 64x64 24-bit RGBA image with alpha channel. The alpha channel
data is used to define the transparency level for the bitmap, with 0 being
fully transparent and 255 being full opaque. Since the color bitmap data
is alpha blended, the is no AND mask for the cursor image.

The HotX and HotY members define the /hot spot/ for the cursor, which is
the location where the logical mouse pointer is located in the cursor
image. When you click the mouse, the pixel under the hot-spot is the pixel
selected.

HEADER:
nucleus/graphics.h

MEMBERS:
ColorData	- Cursor color data as a 64x64 array of packed 24-bit RGBA pixels
HotX        - Cursor X coordinate hot spot
HotY       	- Cursor Y coordinate hot spot
****************************************************************************/
typedef struct {
	N_uint8		ColorData[16384];
	N_uint32   	HotX;
	N_uint32   	HotY;
	} GA_colorCursorRGBA;

/****************************************************************************
REMARKS:
Parameter block for the 2D DrawTrap function. This structure is used to pass
in the information about a trapezoid to be rendered by the hardware to the
driver DrawTrap function.

HEADER:
nucleus/graphics.h

MEMBERS:
y		- Starting Y coordinate
count	- Number of scanlines to draw
x1		- Starting X1 coordinate in 16.16 fixed point
x2		- Starting X2 coordinate in 16.16 fixed point
slope1	- First edge slope in 16.16 fixed point format
slope2	- Second edge slope in 16.16 fixed point format
****************************************************************************/
typedef struct {
	N_uint32	y;
	N_uint32	count;
	N_fix32		x1;
	N_fix32		x2;
	N_fix32		slope1;
	N_fix32		slope2;
	} GA_trap;

/****************************************************************************
REMARKS:
Structure pointed to be the GA_certifyInfo structure, which contains
certification information about the specific chipsets in the device driver.

Note:	The dwSize member is intended for future compatibility, and
		should be set to the size of the structure as defined in this
		header file. Future drivers will be compatible with older software
		by examiming this value.

HEADER:
nucleus/graphics.h

MEMBERS:
dwSize				- Set to size of structure in bytes
ChipsetName			- Name of graphics chipset name
CertifyVersion		- Version of certification program used
CertifiedDate		- Date that the card was certified
****************************************************************************/
typedef struct {
	N_uint32	dwSize;
	char		ChipsetName[30];
	N_uint16    CertifyVersion;
	char		CertifiedDate[20];
	} GA_certifyChipInfo;

/****************************************************************************
REMARKS:
Structure returned by GetCertifyInfo, which contains configuration
information about the certification status of the drivers.

Note:	The dwSize member is intended for future compatibility, and
		should be set to the size of the structure as defined in this
		header file. Future drivers will be compatible with older software
		by examiming this value.

HEADER:
nucleus/graphics.h

MEMBERS:
dwSize				- Set to size of structure in bytes
Signature			- Signature to identify certification information
BuildDate			- String representation of the build date for driver
MaxCertifiedCards	- Maximum number of certified chipsets in driver
NumCertifiedCards	- Number of certified chipsets
CertifiedCards		- List of all certified cards in the driver
****************************************************************************/
typedef struct {
	N_uint32			dwSize;
	char				Signature[20];
	char				BuildDate[30];
	char        		MaxCertifiedChips;
	char        		NumCertifiedChips;
	GA_certifyChipInfo	*CertifiedCards;
	} GA_certifyInfo;

/****************************************************************************
REMARKS:
Structure returned by GetConfigInfo, which contains configuration
information about the installed graphics hardware.

Note:	The dwSize member is intended for future compatibility, and
		should be set to the size of the structure as defined in this
		header file. Future drivers will be compatible with older software
		by examiming this value.

HEADER:
nucleus/graphics.h

MEMBERS:
dwSize				- Set to size of structure in bytes
ManufacturerName	- Name of graphics chipset manufacturer
ChipsetName			- Name of graphics chipset name
DACName				- Name of DAC on graphics card
ClockName			- Name of clock on graphics card
VersionInfo			- String representation of version and build for driver
BuildDate			- String representation of the build date for driver
Certified			- True if the installed device is certified
CertifiedDate		- Date when the device was certified
CertifyVersion		- Version of certification program used
****************************************************************************/
typedef struct {
	N_uint32		dwSize;
	char			ManufacturerName[80];
	char			ChipsetName[80];
	char    		DACName[80];
	char			ClockName[80];
	char			VersionInfo[80];
	char			BuildDate[80];
	char			Certified;
	char			CertifiedDate[20];
	N_uint16    	CertifyVersion;
	} GA_configInfo;

/****************************************************************************
REMARKS:
Structure used to describe the available display modes in the Nucleus
options structure. This allows the end user to add and delete available
display modes from the Nucleus drivers easily using our generic Nucleus
driver interface.

HEADER:
nucleus/graphics.h

MEMBERS:
xRes    - Horizontal pixel resolution
yRes    - Vertical pixel resolution
bits	- Color depth per pixel (0 = text mode)
****************************************************************************/
typedef struct {
	short   	xRes;
	short   	yRes;
	uchar    	bits;
	} GA_mode;

/****************************************************************************
REMARKS:
Structure used to describe the TV parameters specific to a particular TV
output mode. We store these values independently in the options structure
for different TV modes (ie: 640x480, 800x600, PAL, NTSC etc).

HEADER:
nucleus/graphics.h

MEMBERS:
hPos		- Horizontal position value (+-)
vPos		- Vertical position value (+-)
brightness	- Brightness control value
contrast	- Contrast control value
****************************************************************************/
typedef struct {
	N_int16		hPos;
	N_int16		vPos;
	N_uint16	brightness;
	N_uint16	contrast;
	} GA_TVParams;

#define	GA_MAX_RESOLUTIONS	20
#define	GA_MAX_COLORDEPTHS	8

/****************************************************************************
REMARKS:
Structure returned by GetOptions, which contains configuration
information about the options for the installed device driver. All the
boolean configuration options are enabled by default and can be optionally
turned off by the user via the configuration functions.

This structure also contains the configuration information for the software
stereo page flipping support in Nucleus.

If you select the gaGlassesIOPort type, then you need to fill
in the ioPort, ioAndMask, ioLeftOrMask, ioRightOrMask and ioOffOrMask
fields. These fields define the values used to toggle the specified I/O
port when the glasses need to be flipped. First the existing value is read
from the specified I/O port, the AND mask in applied and then the appropriate
OR mask is applied depending on the state of the glasses. This value is
then written back to the desired I/O port.

If you select the gaGlassesGenericVSync type, then you need to fill in the
VSyncWidthLeft and VSyncWidthRight fields, which define the vertical
sync width to program when the desired eye should be active.

The values in the RTCFrequency and RTCAdvanceTicks define the frequency
of the stereo timer interrupt, which can be used to fine tune the overheads
taken by the stereo page flip handler for maximum performance before
stuttering begins (ie: lost frames). The RTCFrequency field can be any
power of 2 frequency between 1024Hz and 8192H, and the RTCAdvanceTicks
should be a value larger than 1. For most systems an RTCFrequency value
of 2048 and an RTCAdvanceTicks of 2 will work well. Nucleus will however
choose good defaults for the target OS if these values are not overridden.

Note:	The dwSize member is intended for future compatibility, and
		should be set to the size of the structure as defined in this
		header file. Future drivers will be compatible with older software
		by examiming this value.

HEADER:
nucleus/graphics.h

MEMBERS:
dwSize					- Set to size of structure in bytes
memoryClock				- Currently configured memory clock
defaultMemoryClock		- Current hardware default memory clock
maxMemoryClock			- Maximum allowable memory clock
gammaRamp				- Default gamma ramp for RGB display modes
outputDevice			- Currently configured output device
TV640PALUnder			- TV parameters for 640x480 PAL underscan modes
TV640NTSCUnder			- TV parameters for 640x480 NTSC underscan modes
TV640PALOver			- TV parameters for 640x480 PAL overscan modes
TV640NTSCOver			- TV parameters for 640x480 NTSC overscan modes
TV800PALUnder			- TV parameters for 800x600 PAL underscan modes
TV800NTSCUnder			- TV parameters for 800x600 NTSC underscan modes
TV800PALOver			- TV parameters for 800x600 PAL overscan modes
TV800NTSCOver			- TV parameters for 800x600 NTSC overscan modes
numHorzDisplay			- Number of horizontal displays
numVertDisplay			- Number of vertical displays
RTCFrequency			- Frequency for real time clock for software stereo
RTCAdvanceTicks			- Number of ticks to advance for software stereo
glassesType				- Type of stereo glasses defined by GA_glassesTypeFlags
ioPort					- Generic I/O port for controlling glasses
ioAndMask				- I/O port AND mask
ioLeftOrMask			- I/O port OR mask when left eye is active
ioRightOrMask			- I/O port OR mask when right eye is active
ioOffOrMask				- I/O port OR mask when glasses are off
vSyncWidthLeft			- Vertical sync width when left eye is active
vSyncWidthRight			- Vertical sync width when right eye is active
text80x43Height			- Character height for 80x43 text mode (8,14 or 16)
text80x50Height         - Character height for 80x50 text mode (8,14 or 16)
text80x60Height         - Character height for 80x60 text mode (8,14 or 16)
text100x43Height		- Character height for 100x43 text mode (8,14 or 16)
text100x50Height        - Character height for 100x50 text mode (8,14 or 16)
text100x60Height        - Character height for 100x60 text mode (8,14 or 16)
text132x43Height		- Character height for 132x43 text mode (8,14 or 16)
text132x50Height        - Character height for 132x50 text mode (8,14 or 16)
text132x60Height        - Character height for 132x60 text mode (8,14 or 16)
tripleScanLowRes		- True to triple scan low res modes
doubleScan512			- True to double scan 512x384 modes
stereoRefresh			- Value to use for stereo mode refresh rate
stereoRefreshInterlaced	- Value to use for stereo mode interlaced refresh rate
stereoMode				- Stereo mode to be used for fullscreen applications
stereoModeWindowed		- Stereo mode to be used for windowed applications
stereoBlankInterval		- Stereo mode blank interval for above below format
stereoDevice			- Stereo device ID defined by stereo control panel
LCDPanelWidth			- Width of attached LCD panel in pixels
LCDPanelHeight			- Height of attached LCD panel in lines
LCDHorzStretch			- Horizontal stretcing: 1 stretch enable, 0 off
LCDVertStretch			- Vertical stretcing: 1 stretch enable, 0 off
bDebugMode				- Enable debug log filter driver (0 is off)
bGenericRefresh			- Enable generic refresh rate control
bDialAMode				- Enable Dial-A-Mode generic mode interface
bVirtualScroll			- Enable virtual scrolling functions
bDoubleBuffer			- Enable double buffering functions
bTripleBuffer			- Enable triple buffering functions
bHardwareStereoSync		- Enable hardware stereo sync flag
bStereo					- Enable stereo display mode support
bMultiDisplay			- Enable multiple display mode support
bPortrait				- Enable portrait display mode
bFlipped				- Enable flipped display mode
bTVOut					- Enable TV Output support
bTVTuner				- Enable TV Tuner support
bMultiCRTC				- Enable Multiple CRTC controller support
bDPMS					- Enable DPMS Display Power Management support
bDDC					- Enable DDC Display Data Channel functions
bDDCCI					- Enable DDC/CI Control Interface functions
bGammaCorrect			- Enable gamma correction
bHardwareCursor			- Enable hardware cursor
bHardwareVideo			- Enable hardware video
bHardwareAccel2D		- Enable hardware 2D acceleration
bHardwareAccel3D		- Enable hardware 2D acceleration
bMonoPattern			- Enable 8x8 mono pattern fills
bTransMonoPattern		- Enable 8x8 mono transparent pattern fills
bColorPattern			- Enable 8x8 color pattern fills
bTransColorPattern		- Enable 8x8 color transparent pattern fills
bSysMem					- Enable system memory blits
bLinear					- Enable linear offscreen memory blits
bBusMaster				- Enable bus mastering functions
bDrawScanList			- Enable DrawScanList family of functions
bDrawEllipseList        - Enable DrawEllipseList family of functions
bDrawFatEllipseList     - Enable DrawFatEllipseList family of functions
bDrawRect               - Enable DrawRect family of functions
bDrawRectLin            - Enable DrawRectLin family of functions
bDrawTrap               - Enable DrawTrap family of functions
bDrawLine				- Enable DrawLine function
bDrawStippleLine		- Enable DrawStippleLine function
bPutMonoImage			- Enable PutMonoImage family of functions
bClipMonoImage			- Enable ClipMonoImage family of functions
bBitBlt                 - Enable BitBlt family of functions
bBitBltPatt             - Enable BitBltPatt family of functions
bBitBltColorPatt        - Enable BitBltColorPatt family of functions
bSrcTransBlt            - Enable SrcTransBlt family of functions
bDstTransBlt            - Enable DstTransBlt family of functions
bStretchBlt             - Enable StretchBlt family of functions
bConvertBlt             - Enable ConvertBlt family of functions
bStretchConvertBlt      - Enable StretchConvertBlt family of functions
bBitBltFx               - Enable BitBltFx family of functions
bGetBitmap              - Enable GetBitmap family of functions
****************************************************************************/
typedef struct {
	N_uint32		dwSize;
	N_fix32			memoryClock;
	N_fix32			defaultMemoryClock;
	N_fix32			maxMemoryClock;
	GA_paletteExt	gammaRamp[256];
	N_int32			outputDevice;
	GA_TVParams		TV640PALUnder;
	GA_TVParams		TV640NTSCUnder;
	GA_TVParams		TV640PALOver;
	GA_TVParams		TV640NTSCOver;
	GA_TVParams		TV800PALUnder;
	GA_TVParams		TV800NTSCUnder;
	GA_TVParams		TV800PALOver;
	GA_TVParams		TV800NTSCOver;
	N_uint8     	bRes1;
	N_uint8     	bRes2;
	N_int32     	RTCFrequency;
	N_int32     	RTCAdvanceTicks;
	N_uint8			glassesType;
	N_uint16		ioPort;
	N_uint8     	ioAndMask;
	N_uint8			ioLeftOrMask;
	N_uint8			ioRightOrMask;
	N_uint8     	ioOffOrMask;
	N_uint8     	vSyncWidthLeft;
	N_uint8			vSyncWidthRight;
	N_uint8			text80x43Height;
	N_uint8			text80x50Height;
	N_uint8			text80x60Height;
	N_uint8			text100x43Height;
	N_uint8			text100x50Height;
	N_uint8			text100x60Height;
	N_uint8			text132x43Height;
	N_uint8			text132x50Height;
	N_uint8			text132x60Height;
	N_uint8         tripleScanLowRes;
	N_uint8         doubleScan512;
	N_uint8         stereoRefresh;
	N_uint8         stereoRefreshInterlaced;
	N_uint8			stereoMode;
	N_uint8			stereoModeWindowed;
	N_uint8         stereoBlankInterval;
	N_uint8         stereoRefreshWindowed;
	N_uint8         stereoRefreshWindowedInterlaced;
	N_uint32        stereoDevice;
	N_uint8			res0[32];
	N_int16         LCDPanelWidth;
	N_int16         LCDPanelHeight;
	N_uint8			LCDHorzStretch;
	N_uint8			LCDVertStretch;
	N_uint8			res1[100];
	N_uint32		resolutions[GA_MAX_RESOLUTIONS];
	N_uint8        	colorDepths[GA_MAX_COLORDEPTHS];
	N_uint8        	maxRefresh;
	N_uint8			res2[160];
	N_uint8			bDebugMode;
	N_uint8			bGenericRefresh;
	N_uint8			bDialAMode;
	N_uint8			bVirtualScroll;
	N_uint8			bDoubleBuffer;
	N_uint8			bTripleBuffer;
	N_uint8			bHardwareStereoSync;
	N_uint8			bStereo;
	N_uint8			bMultiDisplay;
	N_uint8			bPortrait;
	N_uint8			bFlipped;
	N_uint8			bInvertColors;
	N_uint8			bPrefer16bpp;
	N_uint8			bPrefer32bpp;
	N_uint8			bVirtualDisplay;
	N_uint8			res3[157];
	N_uint8			bTVOut;
	N_uint8			bTVTuner;
	N_uint8			bMultiCRTC;
	N_uint8			bDPMS;
	N_uint8			bDDC;
	N_uint8			bDDCCI;
	N_uint8			bGammaCorrect;
	N_uint8			bHardwareCursor;
	N_uint8			bHardwareColorCursor;
	N_uint8			bHardwareVideo;
	N_uint8			bHardwareAccel2D;
	N_uint8			bHardwareAccel3D;
	N_uint8			bMonoPattern;
	N_uint8			bTransMonoPattern;
	N_uint8			bColorPattern;
	N_uint8			bTransColorPattern;
	N_uint8			bSysMem;
	N_uint8			bLinear;
	N_uint8			bBusMaster;
	N_uint8			bDrawScanList;
	N_uint8			bDrawEllipseList;
	N_uint8			bDrawFatEllipseList;
	N_uint8			bDrawRect;
	N_uint8			bDrawRectLin;
	N_uint8			bDrawTrap;
	N_uint8			bDrawLine;
	N_uint8			bDrawStippleLine;
	N_uint8			bPutMonoImage;
	N_uint8			bClipMonoImage;
	N_uint8			bBitBlt;
	N_uint8			bBitBltPatt;
	N_uint8			bBitBltColorPatt;
	N_uint8			bSrcTransBlt;
	N_uint8			bDstTransBlt;
	N_uint8			bStretchBlt;
	N_uint8			bConvertBlt;
	N_uint8			bStretchConvertBlt;
	N_uint8			bBitBltFx;
	N_uint8			bGetBitmap;
	} GA_options;

#define	FIRST_OPTION	bGenericRefresh
#define	LAST_OPTION		bGetBitmap

/****************************************************************************
REMARKS:
Structure used to determine the layout in multi-controller modes. The
layout is defined as adajacent rectangles for each device, and should
be set up using the multi-controller setup program.

HEADER:
nucleus/graphics.h

MEMBERS:
left			- Left coordinate for layout rectangle
top     		- Top coordinate for layout rectangle
right   		- Right coordinate for layout rectangle
bottom  		- Bottom coordinate for layout rectangle
****************************************************************************/
typedef struct {
	N_uint32    left;
	N_uint32	top;
	N_uint32	right;
	N_uint32	bottom;
	} GA_layout;

/****************************************************************************
REMARKS:
Structure returned by GetGlobalOptions, which contains configuration
information about the options effective for all installed display devices.
This structure also contains the layout information used for multi-controller
options in Nucleus (such as what screen is located where).

Note:	The dwSize member is intended for future compatibility, and
		should be set to the size of the structure as defined in this
		header file. Future drivers will be compatible with older software
		by examiming this value.

HEADER:
nucleus/graphics.h

MEMBERS:
dwSize					- Set to size of structure in bytes
bVirtualDisplay			- Enable virtual display mode
bPortrait				- Enable portrait display mode
bFlipped				- Enable flipped display mode
bInvertColors			- Enable invert color mode
bVBEOnly				- Enable VBE/Core fallback driver
bVGAOnly				- Enable VGA fallback driver
bAllowNonCertified		- Allow uncertified drivers to load
wCertifiedVersion		- Version of certify program to allow
bNoWriteCombine			- Disable write combining
virtualSize				- Virtual size for multi-controller displays
resolutions				- Physical resolutions for multi-controller displays
bounds					- Virtual layout for multi-controller displays
****************************************************************************/
typedef struct {
	N_uint32		dwSize;
	N_uint8			bVirtualDisplay;
	N_uint8			bPortrait;
	N_uint8			bFlipped;
	N_uint8			bInvertColors;
	N_uint8         bVBEOnly;
	N_uint8         bVGAOnly;
	N_uint8			bAllowNonCertified;
	N_uint16        wCertifiedVersion;
	N_uint8			bNoWriteCombine;
	N_uint8			res1[154];
	GA_layout       virtualSize;
	GA_layout		resolutions[GA_MAX_VIRTUAL_DISPLAYS];
	GA_layout		bounds[GA_MAX_VIRTUAL_DISPLAYS];
	} GA_globalOptions;

/****************************************************************************
REMARKS:
Structure returned by GetModeProfile, which contains configuration
information about the mode profile for the installed device driver. A
default mode profile is shipped with the graphics device drivers, but a new
mode profile can be downloaded at any time (to implement new display modes
using the new Dial-A-Mode interface). Note that a mode must also have
the associated CRTC tables before it will work.

Note:	The dwSize member is intended for future compatibility, and
		should be set to the size of the structure as defined in this
		header file. Future drivers will be compatible with older software
		by examiming this value.

HEADER:
nucleus/graphics.h

MEMBERS:
dwSize		- Set to size of structure in bytes
numModes	- Count for the number of configured display modes
modeList	- Array of up to 256 configured display modes
****************************************************************************/
typedef struct {
	N_uint32	dwSize;
	N_uint8		numModes;
	GA_mode		modeList[256];
	} GA_modeProfile;

/****************************************************************************
REMARKS:
Main graphics device context structure. This structure consists of a header
block that contains configuration information about the graphic device,
as well as detection information and runtime state information.

The Signature member is filled with the null terminated string 'GRAPHICS\0'
by the driver implementation. This can be used to verify that the file loaded
really is an graphics device driver.

The Version member is a BCD value which specifies what revision level of the
graphics specification is implemented in the driver. The high byte specifies
the major version number and the low byte specifies the minor version number.
For example, the BCD value for version 1.0 is 0x100 and the BCD value for
version 2.2 would be 0x202.

The DriverRev member specifies the driver revision level, and is used by the
driver configuration software to determine which version was used to generate
the driver file.

The OemVendorName member contains the name of the vendor that developed the
device driver implementation, and can be up to 80 characters in length.

The OemCopyright member contains a copyright string for the vendor that
developed the device driver implementation and may be up to 80 characters
in length.

The AvailableModes is an pointer within the loaded driver to a list of mode
numbers for all displaymodes supported by the graphics driver.  Each mode
number occupies one word (16-bits), and is terminated by a -1 (0FFFFh). Any
modes found in this list are guaranteed to be available for the current
configuration.

The TotalMemory member indicates the maximum amount of memory physically
installed and available to the frame buffer in 1Kb units. Note that not all
graphics modes will be able to address all of this memory.

The Attributes member contains a number of flags that describes certain
important characteristics of the graphics controller. The members are
exactly the same as those provided in the GA_modeInfo block for each video
mode, but the meaning is slightly different. For each flag defined in the
GA_AttributeFlagsType enumeration, it represents whether the controller can
support these modes in any available graphics modes. Please see the
GetVideoModeInfo function for a detailed description of each flags meaning.

The TextSize member contains the size of the text mode framebuffer in bytes.
It will generally be 64Kb in length. The TextBasePtr member is a
32-bit physical memory address where the text mode framebuffer memory window
is located in the CPU address space. This will generally be 0xB0000 to cover
the VGA text framebuffer window (both color and monochrome modes).

The BankSize member contains the size of the banked memory buffer in bytes.
It can be either 4Kb or 64Kb in length. The BankedBasePtr member is a
32-bit physical memory address where the banked framebuffer memory window is
located in the CPU address space. If the banked framebuffer mode is not
available, then this member will be zero.

The LinearSize member is the 32-bit length of the linear frame buffer memory
in bytes. In can be any length up to the size of the available video memory.
The LinearBasePtr member is the 32-bit physical address of the start of frame
buffer memory when the controller is in linear frame buffer memory mode. If
the linear framebuffer is not available, then this member will be zero.

The ZBufferSize member is the 32-bit length of the local z-buffer (or depth
buffer) memory in bytes. In can be any length up to the size of the available
local z-buffer memory. The ZBufferBasePtr member is the 32-bit physical
address of the start of local z-buffer memory. Note that if the controller
does not have local z-buffer memory, but shares the z-buffer in the local
framebuffer memory, these two fields will be set to 0.

The TexMemSize member is the 32-bit length of the local texture memory in
bytes. In can be any length up to the size of the available local texture
memory. The TexMemBasePtr member is the 32-bit physical address of the start
of local texture memory. Note that if the controller does not have local
texture memory, but loads textures in the local framebuffer memory, this
field will be set to 0.

The LockedMemSize contains the amount of locked, contiguous memory in bytes
that the graphics driver requires for programming the hardware. If the graphics
accelerator requires DMA transfers for 2D and 3D rendering operations, this
member can be set to the length of the block of memory that is required by
the driver. The driver loader code will attempt to allocate a block of
locked, physically contiguous memory from the operating system and place a
pointer to this allocated memory in the LockedMem member for the driver, and
the physical address of the start of this memory block in LockedMemPhys. Note
that the memory must be locked so it cannot be paged out do disk, and it must
be physically contiguous so that DMA operations will work correctly across
4Kb CPU page boundaries. If the driver does not require DMA memory, this
value should be set to 0.

The MMIOBase member contains the 32-bit physical base addresses pointing
to the start of up to 4 separate memory mapped register areas required by the
controller. The MMIOLen member contains the lengths of each of these
memory mapped IO areas in bytes. When the application maps the memory mapped
IO regions for the driver, the linear address of the mapped memory areas will
then be stored in the corresponding entries in the IOMemMaps array, and will
be used by the driver for accessing the memory mapped registers on the
controller. If any of these regions are not required, the MMIOBase
entries will be NULL and do not need to be mapped by the application.

Note:   The memory regions pointed to by the MMIOBase addresses have
		special meanings for the first two and second two addresses that are
		mapped. If the OS loader is running the driver in user space with a
		safety level of 2, then the only the first two base addresses will
		be mapped into user space, and the second two will be mapped only
		into kernel space (kernel space can also access the user space
		mappings). Please see QueryFunctions for a more detailed overview
		of the safety levels and how this relates to these regions.

The IOMemMaps member contains the mapped linear address of the memory mapped
register regions defined by the MMIOBase and MMIOLen members.

The TextMem member contains the mapped linear address of the text mode
framebuffer, and will be filled in by the application when it has loaded the
device driver. This provides the device driver with direct access to the
video memory on the controller when in text modes.

The BankedMem member contains the mapped linear address of the banked memory
framebuffer, and will be filled in by the application when it has loaded the
device driver. This provides the device driver with direct access to the
video memory on the controller when in the banked framebuffer modes.

The LinearMem member contains the mapped linear address of the linear memory
framebuffer, and will be filled in by the application when it has loaded the
device driver. This provides the device driver with direct access to the
video memory on the controller when in the linear framebuffer modes.

Note:	On some controllers the linear framebuffer address may be different
		for different color depths, so the value in this variable may change
		gater initializing a mode. Applications should always reload the
		address of the linear framebuffer from this variable gater
		initializing a mode set to ensure that the correct value is always
		used.

The ZBufferMem member contains the mapped linear address of the local
z-buffer memory, and will be filled in by the application when it has loaded
the device driver. This provides the device driver with direct access to the
local z-buffer memory on the controller. If the controller does not have
local z-buffer memory, this member will be set to NULL.

The TexBufferMem member contains the mapped linear address of the local
texture memory, and will be filled in by the application when it has loaded
the device driver. This provides the device driver with direct access to the
local texture memory on the controller. If the controller does not have
local texture memory, this member will be set to NULL.

The LockedMem member contains a pointer to the locked DMA memory buffer
allocated for the loaded driver. The graphics driver can use this pointer to
write data directly to the DMA buffer before transferring it to the hardware.
If the driver does not require DMA memory, this value will be set to NULL by
the loader.

The LockedMemPhys member contains the 32-bit physical memory address of the
locked DMA buffer memory allocated for the driver. The graphics driver can use
this physical address to set up DMA transfer operations for memory contained
within the DMA transfer buffer. If the driver does not require DMA memory,
this value will be set to 0 by the loader.

The TextFont8x8, TextFont8x14 and TextFont8x16 members contain pointers to
the 8x8, 8x14 and 8x16 text font bitmaps allocated by the OS loader. This
data is used by the driver for VGA and extended text modes that require the
bitmap font tables.

Note:	None of the function pointers in this structure are listed here
		for brevity. All the functions are documented in the function
		documentation section for each individual function. Please refer
		to this list, or the official nucleus/graphics.h header file for more
		information.

HEADER:
nucleus/graphics.h

MEMBERS:
Signature      		- 'GRAPHICS\0' 20 byte signature
Version            	- Driver Interface Version (2.0)
DriverRev          	- Driver revision number
OemVendorName  		- Vendor Name string
OemCopyright   		- Vendor Copyright string
AvailableModes    	- Offset to supported mode table
DeviceIndex			- Device index for the driver when loaded from disk
TotalMemory        	- Amount of memory in Kb detected
Attributes         	- Driver attributes
WorkArounds			- Hardware WorkArounds flags
TextSize			- Length of the text framebuffer in bytes
TextBasePtr			- Base address of the text framebuffer
BankSize           	- Bank size in bytes (4Kb or 64Kb)
BankedBasePtr      	- Physical addr of banked buffer
LinearSize         	- Linear buffer size in bytes
LinearBasePtr      	- Physical addr of linear buffer
ZBufferSize        	- Z-buffer size in bytes
ZBufferBasePtr     	- Physical addr of Z-buffer
TexBufferSize      	- Texture buffer size in bytes
TexBufferBasePtr   	- Physical addr of texture buffer
LockedMemSize      	- Amount of locked memory for driver in bytes
IOBase				- Base address for I/O mapped registers (relocateable)
MMIOBase    		- Base address of memory mapped I/O regions
MMIOLen     		- Length of memory mapped I/O regions in bytes
DriverStart			- Pointer to the start of the driver in memory
DriverSize			- Size of the entire driver in memory in bytes
IOMemMaps      		- Pointers to mapped I/O memory
BankedMem         	- Ptr to mapped banked video mem
LinearMem         	- Ptr to mapped linear video mem
ZBufferMem        	- Ptr to mapped zbuffer mem
TexBufferMem      	- Ptr to mapped texture buffer mem
LockedMem         	- Ptr to allocated locked memory
LockedMemPhys     	- Physical addr of locked memory
TextFont8x8       	- Ptr to 8x8 text font data
TextFont8x14      	- Ptr to 8x14 text font data
TextFont8x16      	- Ptr to 8x16 text font data
VGAPal4				- Ptr to the default VGA 4bpp palette
VGAPal8				- Ptr to the default VGA 8bpp palette
****************************************************************************/
typedef struct {
	/*------------------------------------------------------------------*/
	/* Device driver header block                                       */
	/*------------------------------------------------------------------*/
	char        	Signature[20];
	N_uint32   		Version;
	N_uint32   		DriverRev;
	char        	OemVendorName[80];
	char        	OemCopyright[80];
	N_uint16    	_FAR_ *AvailableModes;
	N_int32			DeviceIndex;
	N_uint32   		TotalMemory;
	N_uint32   		Attributes;
	N_uint32        WorkArounds;
	N_uint32   		TextSize;
	N_uint32   		TextBasePtr;
	N_uint32   		BankSize;
	N_uint32   		BankedBasePtr;
	N_uint32   		LinearSize;
	N_uint32   		LinearBasePtr;
	N_uint32   		ZBufferSize;
	N_uint32   		ZBufferBasePtr;
	N_uint32   		TexBufferSize;
	N_uint32   		TexBufferBasePtr;
	N_uint32   		LockedMemSize;
	N_uint32		IOBase;
	N_uint32   		MMIOBase[4];
	N_uint32   		MMIOLen[4];
	void			_FAR_ *DriverStart;
	N_uint32		DriverSize;
	N_uint32   		res1[20];

	/*------------------------------------------------------------------*/
	/* Near pointers mapped by loader for driver                   		*/
	/*------------------------------------------------------------------*/
	void       		_FAR_ *IOMemMaps[4];
	void       		_FAR_ *TextMem;
	void       		_FAR_ *BankedMem;
	void       		_FAR_ *LinearMem;
	void       		_FAR_ *ZBufferMem;
	void       		_FAR_ *TexBufferMem;
	void       		_FAR_ *LockedMem;
	N_uint32   		LockedMemPhys;
	void       		_FAR_ *TextFont8x8;
	void       		_FAR_ *TextFont8x14;
	void       		_FAR_ *TextFont8x16;
	GA_palette		_FAR_ *VGAPal4;
	GA_palette		_FAR_ *VGAPal8;
	N_uint32   		res3[20];

	/*------------------------------------------------------------------*/
	/* Driver initialization functions 									*/
	/*------------------------------------------------------------------*/
	N_int32			(NAPIP InitDriver)(void);
	ibool			(NAPIP QueryFunctions)(N_uint32 id,N_int32 safetyLevel,void _FAR_ *funcs);
	void			(NAPIP MapShared)(void);
	} GA_devCtx;

/****************************************************************************
REMARKS:
Device driver hardware 2D state management functions.
{secret}
****************************************************************************/
typedef struct {
	N_uint32	dwSize;
	N_int32 	(NAPIP SetDrawBuffer)(GA_buffer *drawBuf);
	N_int32		(NAPIP IsIdle)(void);
	void		(NAPIP WaitTillIdle)(void);
	void		(NAPIP EnableDirectAccess)(void);
	void		(NAPIP DisableDirectAccess)(void);
	N_int32		(NAPIP SetMix)(N_int32 mix);
	void		(NAPIP SetForeColor)(GA_color color);
	void		(NAPIP SetBackColor)(GA_color color);
	void		(NAPIP Set8x8MonoPattern)(N_int32 index,GA_pattern *pattern);
	void		(NAPIP Use8x8MonoPattern)(N_int32 index);
	void		(NAPIP Use8x8TransMonoPattern)(N_int32 index);
	void		(NAPIP Set8x8ColorPattern)(N_int32 index,GA_colorPattern *pattern);
	void		(NAPIP Use8x8ColorPattern)(N_int32 index);
	void		(NAPIP Use8x8TransColorPattern)(N_int32 index,GA_color transparent);
	void		(NAPIP SetLineStipple)(GA_stipple stipple);
	void		(NAPIP SetLineStippleCount)(N_uint32 count);
	void		(NAPIP SetPlaneMask)(N_uint32 mask);
	void		(NAPIP SetSrcBlendFunc)(N_int32 blendFunc);
	void		(NAPIP SetDstBlendFunc)(N_int32 blendFunc);
	void		(NAPIP SetAlphaValue)(N_uint8 alpha);
	void		(NAPIP SetupForWindows)(void);
	void		(NAPIP ResetForWindows)(void);
	} GA_2DStateFuncs;

/****************************************************************************
REMARKS:
Device driver hardware 2D rendering functions.
{secret}
****************************************************************************/
typedef struct {
	N_uint32	dwSize;
	GA_color	(NAPIP GetPixel)(N_int32 x,N_int32 y);
	void		(NAPIP PutPixel)(N_int32 x,N_int32 y);
	void		(NAPIP DrawScanList)(N_int32 y,N_int32 length,N_int16 *scans);
	void		(NAPIP DrawPattScanList)(N_int32 y,N_int32 length,N_int16 *scans);
	void		(NAPIP DrawColorPattScanList)(N_int32 y,N_int32 length,N_int16 *scans);
	void		(NAPIP DrawEllipseList)(N_int32 y,N_int32 length,N_int32 height,N_int16 *scans);
	void		(NAPIP DrawPattEllipseList)(N_int32 y,N_int32 length,N_int32 height,N_int16 *scans);
	void		(NAPIP DrawColorPattEllipseList)(N_int32 y,N_int32 length,N_int32 height,N_int16 *scans);
	void		(NAPIP DrawFatEllipseList)(N_int32 y,N_int32 length,N_int32 height,N_int16 *scans);
	void		(NAPIP DrawPattFatEllipseList)(N_int32 y,N_int32 length,N_int32 height,N_int16 *scans);
	void		(NAPIP DrawColorPattFatEllipseList)(N_int32 y,N_int32 length,N_int32 height,N_int16 *scans);
	void		(NAPIP DrawRect)(N_int32 left,N_int32 top,N_int32 width,N_int32 height);
	void		(NAPIP DrawRectLin)(N_int32 dstOfs,N_int32 dstPitch,N_int32 left,N_int32 top,N_int32 width,N_int32 height);
	void		(NAPIP DrawPattRect)(N_int32 left,N_int32 top,N_int32 width,N_int32 height);
	void		(NAPIP DrawColorPattRect)(N_int32 left,N_int32 top,N_int32 width,N_int32 height);
	void		(NAPIP DrawTrap)(GA_trap *trap);
	void		(NAPIP DrawPattTrap)(GA_trap *trap);
	void		(NAPIP DrawColorPattTrap)(GA_trap *trap);
	void		(NAPIP DrawLineInt)(N_int32 x1,N_int32 y1,N_int32 x2,N_int32 y2,N_int32 drawLast);
	void		(NAPIP DrawBresenhamLine)(N_int32 x1,N_int32 y1,N_int32 initialError,N_int32 majorInc,N_int32 diagInc,N_int32 count,N_int32 flags);
	void		(NAPIP DrawStippleLineInt)(N_int32 x1,N_int32 y1,N_int32 x2,N_int32 y2,N_int32 drawLast,N_int32 transparent);
	void		(NAPIP DrawBresenhamStippleLine)(N_int32 x1,N_int32 y1,N_int32 initialError,N_int32 majorInc,N_int32 diagInc,N_int32 count,N_int32 flags,N_int32 transparent);
	void		(NAPIP DrawEllipse)(N_int32 left,N_int32 top,N_int32 A,N_int32 B);
	void		(NAPIP ClipEllipse)(N_int32 left,N_int32 top,N_int32 A,N_int32 B,N_int32 clipLeft,N_int32 clipTop,N_int32 clipRight,N_int32 clipBottom);
	void		(NAPIP PutMonoImageMSBSys)(N_int32 x,N_int32 y,N_int32 width,N_int32 height,N_int32 byteWidth,N_uint8 *image,N_int32 transparent);
	void		(NAPIP PutMonoImageMSBLin)(N_int32 x,N_int32 y,N_int32 width,N_int32 height,N_int32 byteWidth,N_int32 imageOfs,N_int32 transparent);
	void		(NAPIP PutMonoImageMSBBM)(N_int32 x,N_int32 y,N_int32 width,N_int32 height,N_int32 byteWidth,N_uint8 *image,N_int32 imagePhysAddr,N_int32 transparent);
	void		(NAPIP PutMonoImageLSBSys)(N_int32 x,N_int32 y,N_int32 width,N_int32 height,N_int32 byteWidth,N_uint8 *image,N_int32 transparent);
	void		(NAPIP PutMonoImageLSBLin)(N_int32 x,N_int32 y,N_int32 width,N_int32 height,N_int32 byteWidth,N_int32 imageOfs,N_int32 transparent);
	void		(NAPIP PutMonoImageLSBBM)(N_int32 x,N_int32 y,N_int32 width,N_int32 height,N_int32 byteWidth,N_uint8 *image,N_int32 imagePhysAddr,N_int32 transparent);
	void		(NAPIP ClipMonoImageMSBSys)(N_int32 x,N_int32 y,N_int32 width,N_int32 height,N_int32 byteWidth,N_uint8 *image,N_int32 transparent,N_int32 clipLeft,N_int32 clipTop,N_int32 clipRight,N_int32 clipBottom);
	void		(NAPIP ClipMonoImageMSBLin)(N_int32 x,N_int32 y,N_int32 width,N_int32 height,N_int32 byteWidth,N_int32 imageOfs,N_int32 transparent,N_int32 clipLeft,N_int32 clipTop,N_int32 clipRight,N_int32 clipBottom);
	void		(NAPIP ClipMonoImageMSBBM)(N_int32 x,N_int32 y,N_int32 width,N_int32 height,N_int32 byteWidth,N_uint8 *image,N_int32 imagePhysAddr,N_int32 transparent,N_int32 clipLeft,N_int32 clipTop,N_int32 clipRight,N_int32 clipBottom);
	void		(NAPIP ClipMonoImageLSBSys)(N_int32 x,N_int32 y,N_int32 width,N_int32 height,N_int32 byteWidth,N_uint8 *image,N_int32 transparent,N_int32 clipLeft,N_int32 clipTop,N_int32 clipRight,N_int32 clipBottom);
	void		(NAPIP ClipMonoImageLSBLin)(N_int32 x,N_int32 y,N_int32 width,N_int32 height,N_int32 byteWidth,N_int32 imageOfs,N_int32 transparent,N_int32 clipLeft,N_int32 clipTop,N_int32 clipRight,N_int32 clipBottom);
	void		(NAPIP ClipMonoImageLSBBM)(N_int32 x,N_int32 y,N_int32 width,N_int32 height,N_int32 byteWidth,N_uint8 *image,N_int32 imagePhysAddr,N_int32 transparent,N_int32 clipLeft,N_int32 clipTop,N_int32 clipRight,N_int32 clipBottom);
	void		(NAPIP BitBlt)(N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 mix);
	void		(NAPIP BitBltLin)(N_int32 srcOfs,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 mix);
	void		(NAPIP BitBltSys)(void *srcAddr,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 mix,N_int32 flipY);
	void		(NAPIP BitBltBM)(void *srcAddr,N_int32 srcPhysAddr,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 mix);
	void		(NAPIP BitBltPatt)(N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 rop3);
	void		(NAPIP BitBltPattLin)(N_int32 srcOfs,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 rop3);
	void		(NAPIP BitBltPattSys)(void *srcAddr,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 rop3,N_int32 flipY);
	void		(NAPIP BitBltPattBM)(void *srcAddr,N_int32 srcPhysAddr,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 rop3);
	void		(NAPIP BitBltColorPatt)(N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 rop3);
	void		(NAPIP BitBltColorPattLin)(N_int32 srcOfs,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 rop3);
	void		(NAPIP BitBltColorPattSys)(void *srcAddr,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 rop3,N_int32 flipY);
	void		(NAPIP BitBltColorPattBM)(void *srcAddr,N_int32 srcPhysAddr,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 rop3);
	void		(NAPIP SrcTransBlt)(N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 mix,GA_color transparent);
	void		(NAPIP SrcTransBltLin)(N_int32 srcOfs,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 mix,GA_color transparent);
	void		(NAPIP SrcTransBltSys)(void *srcAddr,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 mix,GA_color transparent,N_int32 flipY);
	void		(NAPIP SrcTransBltBM)(void *srcAddr,N_int32 srcPhysAddr,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 mix,GA_color transparent);
	void		(NAPIP DstTransBlt)(N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 mix,GA_color transparent);
	void		(NAPIP DstTransBltLin)(N_int32 srcOfs,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 mix,GA_color transparent);
	void		(NAPIP DstTransBltSys)(void *srcAddr,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 mix,GA_color transparent,N_int32 flipY);
	void		(NAPIP DstTransBltBM)(void *srcAddr,N_int32 srcPhysAddr,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 mix,GA_color transparent);
	void		(NAPIP StretchBlt)(N_int32 srcLeft,N_int32 srcTop,N_int32 srcWidth,N_int32 srcHeight,N_int32 dstLeft,N_int32 dstTop,N_int32 dstWidth,N_int32 dstHeight,N_int32 doClip,N_int32 clipLeft,N_int32 clipTop,N_int32 clipRight,N_int32 clipBottom,N_int32 mix);
	void		(NAPIP StretchBltLin)(N_int32 srcOfs,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 srcWidth,N_int32 srcHeight,N_int32 dstLeft,N_int32 dstTop,N_int32 dstWidth,N_int32 dstHeight,N_int32 doClip,N_int32 clipLeft,N_int32 clipTop,N_int32 clipRight,N_int32 clipBottom,N_int32 mix);
	void		(NAPIP StretchBltSys)(void *srcAddr,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 srcWidth,N_int32 srcHeight,N_int32 dstLeft,N_int32 dstTop,N_int32 dstWidth,N_int32 dstHeight,N_int32 doClip,N_int32 clipLeft,N_int32 clipTop,N_int32 clipRight,N_int32 clipBottom,N_int32 mix,N_int32 flipY);
	void		(NAPIP StretchBltBM)(void *srcAddr,N_int32 srcPhysAddr,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 srcWidth,N_int32 srcHeight,N_int32 dstLeft,N_int32 dstTop,N_int32 dstWidth,N_int32 dstHeight,N_int32 doClip,N_int32 clipLeft,N_int32 clipTop,N_int32 clipRight,N_int32 clipBottom,N_int32 mix);
	N_int32 	(NAPIP ConvertBltSys)(void *srcAddr,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 bitsPerPixel,GA_pixelFormat *pixelFormat,GA_palette *dstPal,GA_palette *srcPal,N_int32 dither,N_int32 mix,N_int32 flipY);
	N_int32 	(NAPIP ConvertBltBM)(void *srcAddr,N_int32 srcPhysAddr,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 bitsPerPixel,GA_pixelFormat *pixelFormat,GA_palette *dstPal,GA_palette *srcPal,N_int32 dither,N_int32 mix);
	N_int32 	(NAPIP StretchConvertBltSys)(void *srcAddr,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 srcWidth,N_int32 srcHeight,N_int32 dstLeft,N_int32 dstTop,N_int32 dstWidth,N_int32 dstHeight,N_int32 doClip,N_int32 clipLeft,N_int32 clipTop,N_int32 clipRight,N_int32 clipBottom,N_int32 bitsPerPixel,GA_pixelFormat *pixelFormat,GA_palette *dstPal,GA_palette *srcPal,N_int32 dither,N_int32 mix,N_int32 flipY);
	N_int32 	(NAPIP StretchConvertBltBM)(void *srcAddr,N_int32 srcPhysAddr,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 srcWidth,N_int32 srcHeight,N_int32 dstLeft,N_int32 dstTop,N_int32 dstWidth,N_int32 dstHeight,N_int32 doClip,N_int32 clipLeft,N_int32 clipTop,N_int32 clipRight,N_int32 clipBottom,N_int32 bitsPerPixel,GA_pixelFormat *pixelFormat,GA_palette *dstPal,GA_palette *srcPal,N_int32 dither,N_int32 mix);
	N_int32		(NAPIP BitBltFxTest)(GA_bltFx *fx);
	N_int32 	(NAPIP BitBltFx)(N_int32 srcLeft,N_int32 srcTop,N_int32 srcWidth,N_int32 srcHeight,N_int32 dstLeft,N_int32 dstTop,N_int32 dstWidth,N_int32 dstHeight,GA_bltFx *fx);
	N_int32 	(NAPIP BitBltFxLin)(N_int32 srcOfs,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 srcWidth,N_int32 srcHeight,N_int32 dstLeft,N_int32 dstTop,N_int32 dstWidth,N_int32 dstHeight,GA_bltFx *fx);
	N_int32 	(NAPIP BitBltFxSys)(void *srcAddr,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 srcWidth,N_int32 srcHeight,N_int32 dstLeft,N_int32 dstTop,N_int32 dstWidth,N_int32 dstHeight,GA_bltFx *fx);
	N_int32 	(NAPIP BitBltFxBM)(void *srcAddr,N_int32 srcPhysAddr,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 srcWidth,N_int32 srcHeight,N_int32 dstLeft,N_int32 dstTop,N_int32 dstWidth,N_int32 dstHeight,GA_bltFx *fx);
	void		(NAPIP GetBitmapSys)(void *dstAddr,N_int32 dstPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop);
	void		(NAPIP GetBitmapBM)(void *dstAddr,N_int32 dstPhysAddr,N_int32 dstPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop);
	void		(NAPIP UpdateScreen)(N_int32 left,N_int32 top,N_int32 width,N_int32 height);
	N_int32 	(NAPIP DrawClippedLineInt)(N_int32 x1,N_int32 y1,N_int32 x2,N_int32 y2,N_int32 drawLast,N_int32 clipLeft,N_int32 clipTop,N_int32 clipRight,N_int32 clipBottom);
	N_int32 	(NAPIP DrawClippedBresenhamLine)(N_int32 x1,N_int32 y1,N_int32 initialError,N_int32 majorInc,N_int32 diagInc,N_int32 count,N_int32 flags,N_int32 clipLeft,N_int32 clipTop,N_int32 clipRight,N_int32 clipBottom);
	N_int32 	(NAPIP DrawClippedStippleLineInt)(N_int32 x1,N_int32 y1,N_int32 x2,N_int32 y2,N_int32 drawLast,N_int32 transparent,N_int32 clipLeft,N_int32 clipTop,N_int32 clipRight,N_int32 clipBottom);
	N_int32 	(NAPIP DrawClippedBresenhamStippleLine)(N_int32 x1,N_int32 y1,N_int32 initialError,N_int32 majorInc,N_int32 diagInc,N_int32 count,N_int32 flags,N_int32 transparent,N_int32 clipLeft,N_int32 clipTop,N_int32 clipRight,N_int32 clipBottom);
	} GA_2DRenderFuncs;

/****************************************************************************
REMARKS:
Main device driver init functions, including all mode information, setup and
initialisation functions.
{secret}
****************************************************************************/
typedef struct {
	N_uint32	dwSize;
	void		(NAPIP GetConfigInfo)(GA_configInfo *info);
	void		(NAPIP GetModeProfile)(GA_modeProfile *profile);
	void		(NAPIP SetModeProfile)(GA_modeProfile *profile);
	void		(NAPIP GetOptions)(GA_options *options);
	void		(NAPIP SetOptions)(GA_options *options);
	N_int32		(NAPIP GetVideoModeInfo)(N_uint32 mode,GA_modeInfo *modeInfo);
	N_int32		(NAPIP SetVideoMode)(N_uint32 mode,N_int32 *virtualX,N_int32 *virtualY,N_int32 *bytesPerLine,N_int32 *maxMem,N_int32 refreshRate,GA_CRTCInfo *crtc);
	N_uint32 	(NAPIP GetVideoMode)(void);
	N_int32 	(NAPIP GetCustomVideoModeInfo)(N_int32 xRes,N_int32 yRes,N_int32 virtualX,N_int32 virtualY,N_int32 bitsPerPixel,GA_modeInfo *modeInfo);
	N_int32		(NAPIP SetCustomVideoMode)(N_int32 xRes,N_int32 yRes,N_int32 bitsPerPixel,N_uint32 flags,N_int32 *virtualX,N_int32 *virtualY,N_int32 *bytesPerLine,N_int32 *maxMem,GA_CRTCInfo *crtc);
	N_uint32	(NAPIP GetClosestPixelClock)(N_int32 xRes,N_int32 yRes,N_int32 bitsPerPixel,N_uint32 pixelClock);
	void		(NAPIP GetCRTCTimings)(GA_CRTCInfo *crtc);
	void		(NAPIP SetCRTCTimings)(GA_CRTCInfo *crtc);
	void		(NAPIP SetGlobalRefresh)(N_int32 refresh);
	N_int32 	(NAPIP SaveRestoreState)(N_int32 subfunc,void *saveBuf);
	N_int32 	(NAPIP SetDisplayOutput)(N_int32 device);
	N_int32		(NAPIP GetDisplayOutput)(void);
	void		(NAPIP SetSoftwareRenderFuncs)(GA_2DRenderFuncs *softwareFuncs);
	void		(NAPIP GetUniqueFilename)(char *filename,int type);
	void		(NAPIP GetCRTCTable)(void **data,int *size);
	void		(NAPIP GetMonitorInfo)(GA_monitor *monitor);
	void		(NAPIP SetMonitorInfo)(GA_monitor *monitor);
	void		(NAPIP GetCurrentVideoModeInfo)(GA_modeInfo *modeInfo);
	void		(NAPIP GetCertifyInfo)(GA_certifyInfo *info);
	} GA_initFuncs;

/****************************************************************************
REMARKS:
Main device driver runtime functions, including those to set banks, change
the displayed video memory and program the palette.
{secret}
****************************************************************************/
typedef struct {
	N_uint32	dwSize;
	void		(NAPIP SetBank)(N_int32 bank);
	void		(NAPIP SetDisplayStart)(N_int32 offset,N_int32 waitVRT);
	void		(NAPIP SetDisplayStartXY)(N_int32 x,N_int32 y,N_int32 waitVRT);
	void		(NAPIP SetStereoDisplayStart)(N_int32 leftOffset,N_int32 rightOffset,N_int32 waitVRT);
	N_int32		(NAPIP GetDisplayStartStatus)(void);
	void		(NAPIP EnableStereoMode)(N_int32 enable);
	N_int32		(NAPIP IsVSync)(void);
	void		(NAPIP WaitVSync)(void);
	void		(NAPIP SetVSyncWidth)(N_int32 width);
	N_int32		(NAPIP GetVSyncWidth)(void);
	void		(NAPIP SetPaletteData)(GA_palette *pal,N_int32 num,N_int32 index,N_int32 waitVRT);
	void		(NAPIP GetPaletteData)(GA_palette *pal,N_int32 num,N_int32 index);
	void		(NAPIP SetGammaCorrectData)(GA_palette *pal,N_int32 num,N_int32 index,N_int32 waitVRT);
	void		(NAPIP GetGammaCorrectData)(GA_palette *pal,N_int32 num,N_int32 index);
	N_int32		(NAPIP GetCurrentScanLine)(void);
	void		(NAPIP SetPaletteDataExt)(GA_paletteExt *pal,N_int32 num,N_int32 index,N_int32 waitVRT);
	void		(NAPIP GetPaletteDataExt)(GA_paletteExt *pal,N_int32 num,N_int32 index);
	void		(NAPIP SetGammaCorrectDataExt)(GA_paletteExt *pal,N_int32 num,N_int32 index,N_int32 waitVRT);
	void		(NAPIP GetGammaCorrectDataExt)(GA_paletteExt *pal,N_int32 num,N_int32 index);
	} GA_driverFuncs;

/****************************************************************************
REMARKS:
VBE/Core emulation functions. These functions should not be used by
application programs, and they are only intended to be used by the
VBE/Core emulation driver.
{secret}
****************************************************************************/
typedef struct {
	N_uint32	dwSize;
	ibool   	(NAPIP SetBytesPerLine)(int bytesPerLine,int *newBytes);
	ibool		(NAPIP Set8BitDAC)(ibool enable);
	void		(NAPIP SetPaletteData)(GA_palette *pal,N_int32 num,N_int32 index,N_int32 waitVRT);
	void		(NAPIP GetPaletteData)(GA_palette *pal,N_int32 num,N_int32 index);
	} GA_VBEFuncs;

#ifdef	__INTEL__
/****************************************************************************
REMARKS:
Device driver Win16 specific functions. These functions are necessary to
allow Windows 95 and Windows 98 display drivers to utilise Nucleus
internally. There are some functions that simply must be called from Win16
assembler code, and these functions are listed in this structure. Note that
all these functions take parameters in registers and do not use the stack
at all. Refer the the function documentation for information the register
values to pass to these functions.
{secret}
****************************************************************************/
typedef struct {
	N_uint32	dwSize;
	void		(NAPIP WaitTillIdleW16)(void);
	void		(NAPIP EnableDirectAccessW16)(void);
	void		(NAPIP DisableDirectAccessW16)(void);
	N_int32		(NAPIP SetCursorPosW16)(void);
	N_int32		WaitTillIdleW16_Len;
	N_int32		EnableDirectAccessW16_Len;
	N_int32		DisableDirectAccessW16_Len;
	N_int32		SetCursorPosW16_Len;
	void		_FAR_ *CursorState;
	} GA_win16Funcs;
#endif

/****************************************************************************
REMARKS:
Device driver hardware cursor functions.
{secret}
****************************************************************************/
typedef struct {
	N_uint32	dwSize;
	void		(NAPIP SetMonoCursor)(GA_monoCursor *cursor);
	void		(NAPIP SetMonoCursorColor)(GA_palette *foreground,GA_palette *background);
	void		(NAPIP SetColorCursor)(GA_colorCursor *cursor);
	N_int32		(NAPIP SetCursorPos)(N_int32 x,N_int32 y);
	void		(NAPIP ShowCursor)(N_int32 visible);
	void		(NAPIP BeginAccess)(N_int32 left,N_int32 top,N_int32 right,N_int32 bottom);
	void		(NAPIP EndAccess)(void);
	void		(NAPIP SetColorCursor256)(GA_colorCursor256 *cursor);
	void		(NAPIP SetColorCursorRGBA)(GA_colorCursorRGBA *cursor);
	} GA_cursorFuncs;

/****************************************************************************
REMARKS:
Device driver hardware video overlay functions.
{secret}
****************************************************************************/
typedef struct {
	N_uint32	dwSize;
	N_int32		(NAPIP SetVideoInput)(N_int32 window,N_int32 width,N_int32 height,N_int32 format);
	void *		(NAPIP SetVideoOutput)(N_int32 window,N_int32 left,N_int32 top,N_int32 width,N_int32 height,N_int32 flags);
	void		(NAPIP SetVideoColorKey)(N_int32 window,GA_color colorKeyLo,GA_color colorKeyHi);
	void		(NAPIP StartVideoFrame)(N_int32 window);
	void		(NAPIP EndVideoFrame)(N_int32 window);
	} GA_videoFuncs;

#pragma	pack()

#include "nucleus/ddc.h"
#include "nucleus/stereo.h"
#include "nucleus/ref2d.h"

#pragma	pack(1)

#ifndef	__WINDOWS16__
/****************************************************************************
REMARKS:
Structure defining all the Nucleus Graphics API functions as exported from
the Binary Portable DLL.
{secret}
****************************************************************************/
typedef struct {
	ulong			dwSize;
	int				(NAPIP GA_status)(void);
	const char *	(NAPIP GA_errorMsg)(N_int32 status);
	int				(NAPIP GA_getDaysLeft)(N_int32 shared);
	int				(NAPIP GA_registerLicense)(uchar *license,N_int32 shared);
	int				(NAPIP GA_enumerateDevices)(N_int32 shared);
	GA_devCtx * 	(NAPIP GA_loadDriver)(N_int32 deviceIndex,N_int32 shared);
	ibool			(NAPIP GA_setActiveDevice)(N_int32 deviceIndex);
	void			(NAPIP GA_mapShared)(GA_devCtx *dc);
	void			(NAPIP GA_unloadDriver)(GA_devCtx *dc);
	ibool			(NAPIP REF2D_loadDriver)(GA_devCtx *hwCtx,N_int32 bitsPerPixel,N_int32 shared,REF2D_driver _FAR_ *_FAR_ *drv,PE_MODULE _FAR_ *_FAR_ *hModule,ulong _FAR_ *size);
	void			(NAPIP REF2D_unloadDriver)(REF2D_driver *drv,PE_MODULE *hModule);
	ibool			(NAPIP GA_loadRef2d)(GA_devCtx _FAR_ *dc,N_int32 shared,GA_modeInfo _FAR_ *modeInfo,N_int32 transferStart,REF2D_driver _FAR_ *_FAR_ *drv);
	void			(NAPIP GA_unloadRef2d)(GA_devCtx _FAR_ *dc);
	ibool 			(NAPIP GA_softStereoInit)(GA_devCtx *dc);
	void			(NAPIP GA_softStereoOn)(void);
	void			(NAPIP GA_softStereoScheduleFlip)(N_uint32 leftAddr,N_uint32 rightAddr);
	N_int32			(NAPIP GA_softStereoGetFlipStatus)(void);
	void			(NAPIP GA_softStereoWaitTillFlipped)(void);
	void 			(NAPIP GA_softStereoOff)(void);
	void 			(NAPIP GA_softStereoExit)(void);
	void			(NAPIP GA_saveModeProfile)(GA_devCtx *dc,GA_modeProfile *profile);
	void			(NAPIP GA_saveOptions)(GA_devCtx *dc,GA_options *options);
	N_int32			(NAPIP GA_saveCRTCTimings)(GA_devCtx *dc);
	N_int32			(NAPIP GA_restoreCRTCTimings)(GA_devCtx *dc);
	int 			(NAPIP DDC_init)(GA_devCtx *dc);
	ibool 			(NAPIP DDC_readEDID)(N_int32 slaveAddr,uchar *edid,N_int32 length,N_int32 blockNumber,N_int32 monitorPort);
	int 			(NAPIP EDID_parse)(uchar *edid,EDID_record *rec,N_int32 requireDescriptor);
	int 			(NAPIP MCS_begin)(GA_devCtx *dc);
	int				(NAPIP MCS_getCapabilitiesString)(char *data,N_int32 maxlen);
	ibool			(NAPIP MCS_isControlSupported)(uchar controlCode);
	ibool			(NAPIP MCS_enableControl)(uchar controlCode,N_int32 enable);
	ibool			(NAPIP MCS_getControlMax)(uchar controlCode,ushort *max);
	ibool			(NAPIP MCS_getControlValue)(uchar controlCode,ushort *value);
	ibool			(NAPIP MCS_getControlValues)(N_int32 numControls,uchar *controlCodes,ushort *values);
	ibool			(NAPIP MCS_setControlValue)(uchar controlCode,ushort value);
	ibool			(NAPIP MCS_setControlValues)(N_int32 numControls,uchar *controlCodes,ushort *values);
	ibool			(NAPIP MCS_resetControl)(uchar controlCode);
	ibool			(NAPIP MCS_saveCurrentSettings)(void);
	ibool			(NAPIP MCS_getTimingReport)(uchar *flags,ushort *hFreq,ushort *vFreq);
	ibool			(NAPIP MCS_getSelfTestReport)(uchar *flags,uchar *data,uchar *length);
	void			(NAPIP MCS_end)(void);
	ibool			(NAPIP GA_loadInGUI)(N_int32 shared);
	ibool 			(NAPIP DDC_writeEDID)(GA_devCtx *dc,N_int32 slaveAddr,uchar *edid,N_int32 length,N_int32 blockNumber,N_int32 monitorPort);
	ibool 			(NAPIP GA_useDoubleScan)(GA_modeInfo *modeInfo);
	void 			(NAPIP GA_getMaxRefreshRate)(GA_devCtx *dc,GA_modeInfo *modeInfo,N_int32 interlaced,float *maxRefresh);
	ibool 			(NAPIP GA_computeCRTCTimings)(GA_devCtx *dc,GA_modeInfo *modeInfo,N_int32 refreshRate,N_int32 interlaced,GA_CRTCInfo *crtc,N_int32 pureGTF);
	ibool 			(NAPIP GA_addMode)(GA_devCtx *dc,N_int32 xRes,N_int32 yRes,N_int32 bits);
	ibool			(NAPIP GA_addRefresh)(GA_devCtx *dc,N_int32 xRes,N_int32 yRes,N_int32 refresh);
	ibool			(NAPIP GA_delMode)(GA_devCtx *dc,N_int32 xRes,N_int32 yRes,N_int32 bits);
	const char *	(NAPIP GA_getLogName)(void);
	void 			(NAPIP GA_log)(const char *fmt,...);
	int				(NAPIP MDBX_getErrCode)(void);
	char *			(NAPIP MDBX_getErrorMsg)(void);
	ibool			(NAPIP MDBX_open)(const char *filename,ibool shared);
	void			(NAPIP MDBX_close)(void);
	int				(NAPIP MDBX_first)(GA_monitor *rec);
	int				(NAPIP MDBX_last)(GA_monitor *rec);
	int				(NAPIP MDBX_next)(GA_monitor *rec);
	int				(NAPIP MDBX_prev)(GA_monitor *rec);
	int				(NAPIP MDBX_insert)(GA_monitor *rec);
	int 			(NAPIP MDBX_update)(GA_monitor *rec);
	int				(NAPIP MDBX_flush)(void);
	int				(NAPIP MDBX_importINF)(const char *filename,char *mfr);
	void			(NAPIP GA_getGlobalOptions)(GA_globalOptions *options);
	void			(NAPIP GA_setGlobalOptions)(GA_globalOptions *options);
	void			(NAPIP GA_saveGlobalOptions)(GA_globalOptions *options);
	const char *	(NAPIP GA_getInternalName)(N_int32 deviceIndex);
	const char *	(NAPIP GA_getNucleusConfigPath)(void);
	} GA_exports;

/****************************************************************************
REMARKS:
Defines the structure for holding 64-bit integers used for storing the values
returned by the precision timing functions. The precision timing functions
are used internally by the the Nucleus drivers for software stereo
support, however the granularity of the timing functions is variable.
Generally a granularity of around 1us is desired for maximum accuracy.
Where possible these import functions should be implemented using the
Intel Pentium RDTSC instruction or equivalent (with time readings
normalised to 1us granularity to avoid overflow internally).

HEADER:
nucleus/graphics.h

MEMBERS:
low		- Low 32-bits of the 64-bit integer
high	- High 32-bits of the 64-bit integer
****************************************************************************/
typedef struct {
	N_uint32	low;
	N_uint32	high;
	} GA_largeInteger;

/****************************************************************************
REMARKS:
Structure defining all the Nucleus Graphics API functions as imported into
the Binary Portable DLL.
{secret}
****************************************************************************/
typedef struct {
	ulong			dwSize;
	GA_sharedInfo *	(NAPIP GA_getSharedInfo)(int device);
	ibool			(NAPIP TimerInit)(void);
	void			(NAPIP TimerRead)(GA_largeInteger *value);
	N_uint32 		(NAPIP TimerDifference)(GA_largeInteger *a,GA_largeInteger *b);
#ifndef	__INTEL__
	uchar 			(NAPIP outpb)(ulong port,uchar val);
	ushort 			(NAPIP outpw)(ulong port,ushort val);
	ulong 			(NAPIP outpd)(ulong port,ulong val);
	uchar 			(NAPIP inpb)(ulong port);
	ushort 			(NAPIP inpw)(ulong port);
	ulong 			(NAPIP inpd)(ulong port);
#endif
	} GA_imports;

/****************************************************************************
REMARKS:
Function pointer type for the Binary Portable DLL initialisation entry point.
{secret}
****************************************************************************/
typedef GA_exports * (NAPIP GA_initLibrary_t)(ibool shared,const char *path,const char *bpdname,PM_imports *pmImp,N_imports *nImp,GA_imports *gaImp);
#endif

#pragma	pack()

/*---------------------------- Global Variables ---------------------------*/

#ifdef  __cplusplus
extern "C" {            /* Use "C" linkage when in C++ mode */
#endif

/* {secret} Pointer to global exports structure.
 * Should not be used by application programs.
 */
#ifndef	__WINDOWS16__
extern GA_exports	_VARAPI _GA_exports;
#endif

/*-------------------------- Function Prototypes --------------------------*/

#ifndef	__WINDOWS16__

/* Generic error handling functions for all Nucleus drivers */

int				NAPI GA_status(void);
const char *	NAPI GA_errorMsg(N_int32 status);

/* Function to get the number of days left in evaluation period */

int				NAPI GA_getDaysLeft(N_int32 shared);

/* Utility function to register a linkable library license */

int				NAPI GA_registerLicense(uchar *license,N_int32 shared);

/* Utility Functions to load a graphics driver and initialise it */

int				NAPI GA_enumerateDevices(N_int32 shared);
GA_devCtx * 	NAPI GA_loadDriver(N_int32 deviceIndex,N_int32 shared);
ibool			NAPI GA_setActiveDevice(N_int32 deviceIndex);
void			NAPI GA_mapShared(GA_devCtx *dc);
void			NAPI GA_unloadDriver(GA_devCtx *dc);
const char *	NAPI GA_getInternalName(N_int32 deviceIndex);
const char *	NAPI GA_getNucleusConfigPath(void);
ibool			NAPI GA_loadInGUI(N_int32 shared);

/* Utility functions to interface to the 2D reference rasteriser */

ibool			NAPI GA_loadRef2d(GA_devCtx _FAR_ *dc,N_int32 shared,GA_modeInfo _FAR_ *modeInfo,N_int32 transferStart,REF2D_driver _FAR_ *_FAR_ *drv);
void			NAPI GA_unloadRef2d(GA_devCtx _FAR_ *dc);

/* Functions to initialise, enable, disable and clean up the software
 * stereo functions provided by the Nucleus driver. For ring 3 user space
 * code, these functions will generally transition down to the system
 * helper driver (SDDHELP) to implement this as the interrupt handler
 * needs to run in kernel space.
 */

ibool 			NAPI GA_softStereoInit(GA_devCtx *dc);
void			NAPI GA_softStereoOn(void);
void			NAPI GA_softStereoScheduleFlip(N_uint32 leftAddr,N_uint32 rightAddr);
N_int32			NAPI GA_softStereoGetFlipStatus(void);
void			NAPI GA_softStereoWaitTillFlipped(void);
void 			NAPI GA_softStereoOff(void);
void 			NAPI GA_softStereoExit(void);

/* Utility functions to manipulate global options effective for all devices */

void			NAPI GA_getGlobalOptions(GA_globalOptions *options);
void			NAPI GA_setGlobalOptions(GA_globalOptions *options);
void			NAPI GA_saveGlobalOptions(GA_globalOptions *options);

/* Utility functions to save options and configuration information */

void			NAPI GA_saveModeProfile(GA_devCtx *dc,GA_modeProfile *profile);
void			NAPI GA_saveOptions(GA_devCtx *dc,GA_options *options);
N_int32			NAPI GA_saveCRTCTimings(GA_devCtx *dc);
N_int32			NAPI GA_restoreCRTCTimings(GA_devCtx *dc);

/* Utility functions to create GTF display timings */

ibool 			NAPI GA_useDoubleScan(GA_modeInfo *modeInfo);
void 			NAPI GA_getMaxRefreshRate(GA_devCtx *dc,GA_modeInfo *modeInfo,N_int32 interlaced,float *maxRefresh);
ibool 			NAPI GA_computeCRTCTimings(GA_devCtx *dc,GA_modeInfo *modeInfo,N_int32 refreshRate,N_int32 interlaced,GA_CRTCInfo *crtc,N_int32 pureGTF);

/* Utility functions to manipulate the Nucleus Dial-A-Mode features */

ibool 			NAPI GA_addMode(GA_devCtx *dc,N_int32  xRes,N_int32 yRes,N_int32 bits);
ibool			NAPI GA_addRefresh(GA_devCtx *dc,N_int32 xRes,N_int32 yRes,N_int32 refresh);
ibool			NAPI GA_delMode(GA_devCtx *dc,N_int32 xRes,N_int32 yRes,N_int32 bits);

/* Functions to access Nucleus logging from other processes */

const char *	NAPI GA_getLogName(void);
void 			NAPI GA_log(const char *fmt,...);

/* Prototypes for OS provided timer functions */

#ifndef	__DRIVER__
/* {secret} */
ibool			NAPI GA_TimerInit(void);
/* {secret} */
void			NAPI GA_TimerRead(GA_largeInteger *value);
/* {secret} */
N_uint32 		NAPI GA_TimerDifference(GA_largeInteger *a,GA_largeInteger *b);
#else
#define GA_TimerInit()			_GA_imports.TimerInit()
#define GA_TimerRead(value)		_GA_imports.TimerRead(value)
#define GA_TimerDifference(a,b)	_GA_imports.TimerDifference(a,b)
#endif

/* External Intel assembler functions for high precision timing */

#ifdef	__INTEL__
/* {secret} */
ibool			NAPI _GA_haveCPUID(void);
/* {secret} */
uint			NAPI _GA_getCPUIDFeatures(void);
/* {secret} */
void 			NAPI _GA_readTimeStamp(GA_largeInteger *time);
/* {secret} */
#define			CPU_HaveRDTSC	0x00000010
#endif

/* Prototypes for OS provided I/O functions for non x86 systems */

#ifndef	__INTEL__
/* {secret} */
uchar 			NAPI GA_outpb(ulong port,uchar val);
/* {secret} */
ushort 			NAPI GA_outpw(ulong port,ushort val);
/* {secret} */
ulong 			NAPI GA_outpd(ulong port,ulong val);
/* {secret} */
uchar 			NAPI GA_inpb(ulong port);
/* {secret} */
ushort 			NAPI GA_inpw(ulong port);
/* {secret} */
ulong 			NAPI GA_inpd(ulong port);
#endif

/* {secret} Internal function not to be used by applications */
void 			NAPI _GA_postLoadDriver(void);

/* {secret} Function to return the shared info instance for a particular device */
GA_sharedInfo *	NAPI GA_getSharedInfo(int device);

/* {secret} Function to return the system wide shared exports if needed */
ibool			NAPI GA_getSharedExports(GA_exports *gaExp);

#endif	/* !__WINDOWS16__ */

#ifdef  __cplusplus
}                       			/* End of "C" linkage for C++   	*/
#endif

#endif  /* __NUCLEUS_GRAPHICS_H */
