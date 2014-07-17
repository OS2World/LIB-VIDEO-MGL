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
*				This header file contains all the macros, type definitions
*				and function prototypes for the 3D functionality.
*
****************************************************************************/

#ifndef __NUCLEUS_GA3D_H
#define __NUCLEUS_GA3D_H

#ifndef	__NUCLEUS_GRAPHICS_H
#include "nucleus/graphics.h"
#endif

/*---------------------- Macros and type definitions ----------------------*/

#pragma pack(1)

/* {secret} */
struct _GA_tmap;

/****************************************************************************
REMARKS:
Flags for to determine the hardware depth buffer capabilities. If the
hardware supports depth buffering, it will support one or more of the
following formats. If the hardware supports internal depth buffering where
the resuting depth values for each pixel are not stored in memory (ie: for
hardware that does scene based rendering), the ga3DDepthBufferInternal flag
will be set and one of the other flags will define the precision of the
internal depth buffering available.

Note:	These flags are also used to enable a specific depth buffer format
		when depth buffering is enabled. Enabling a depth buffer format
		other than one of the ones defined as being supported will result
		in undefined behaviour.

HEADER:
nucleus/ga3d.h

MEMBERS:
ga3DDepthBuffer16		- 16-bit integer depth buffer
ga3DDepthBuffer24       - 24-bit integer depth buffer
ga3DDepthBuffer32       - 32-bit integer depth buffer
ga3DDepthBufferFlt16	- 16-bit floating point depth buffer
ga3DDepthBufferFlt24	- 24-bit floating point depth buffer
ga3DDepthBufferFlt32	- 32-bit floating point depth buffer
ga3DDepthBufferInternal	- Internal depth buffering, values not stored in mem
****************************************************************************/
typedef enum {
	ga3DDepthBuffer16				= 0x00000001,
	ga3DDepthBuffer24				= 0x00000002,
	ga3DDepthBuffer32				= 0x00000004,
	ga3DDepthBufferFlt16			= 0x00000008,
	ga3DDepthBufferFlt24			= 0x00000010,
	ga3DDepthBufferFlt32			= 0x00000020,
	ga3DDepthBufferInternal			= 0x40000000
	} GA_DepthFormatFlagsType;

/****************************************************************************
REMARKS:
Flags stored in the Attributes member of the GA_3DSurface structure, which
determine the type of the surface.

HEADER:
nucleus/ga3d.h

MEMBERS:
ga3DSurfSysMem		- Surface is located in system memory
ga3DSurfTexture		- Surface is a texture buffer
ga3DSurfColor		- Surface is a color front buffer
ga3DSurfDepth		- Surface is an attached depth buffer
ga3DSurfStencil		- Surface is an attached stencil buffer
ga3DSurfRight		- Surface is an attached right stereo color buffer
ga3DSurfBack		- Surface is an attached back buffer
ga3DSurfBackRight	- Surface is an attached right stereo back buffer
****************************************************************************/
typedef enum {
	ga3DSurfSysMem				= 0x00000001,
	ga3DSurfTexture				= 0x00000002,
	ga3DSurfColor				= 0x00000004,
	ga3DSurfDepth				= 0x00000008,
	ga3DSurfStencil				= 0x00000010,
	ga3DSurfRight				= 0x00000020,
	ga3DSurfBack				= 0x00000040,
	ga3DSurfBackRight			= 0x00000080,
	} GA_3DSurfaceFlagsType;

/****************************************************************************
REMARKS:
Flags for hardware texure map formats supported by the graphics mode returned
in the TextureFormats member of the GA_modeInfo structure. These flags
define the hardware texture map pixel formats of the particular graphics
mode, and are only valid if the gaHaveAccel3D flag is defined in the
Attributes member of the GA_modeInfo structure.

Note:	These flags are also used to specific the texture map format of a
		texture when it is passed to the hardware for rendering. Specifying
		a texture map format other than one of the ones defined as being
		supported will result in undefined behaviour.

HEADER:
nucleus/ga3d.h

MEMBERS:
ga3DMultiTexture	- Multiple simultaenous texture mapping is supported
ga3DTexture0		- Indentifies this as being texture 0
ga3DTexture1		- Indentifies this as being texture 1
ga3DTexture2		- Indentifies this as being texture 2
ga3DMaxTextures		- Maximum number of multiple textures (currently 3)
ga3DTexCI1			- 1-bit color index texture map
ga3DTexCI2			- 2-bit color index texture map
ga3DTexCI4			- 4-bit color index texture map
ga3DTexCI8			- 8-bit color index texture map
ga3DTexAlpha4		- 4-bit alpha only texture map
ga3DTexAlpha8		- 8-bit alpha only texture map
ga3DIntensity4		- 4-bit intensity only texture map
ga3DIntensity8		- 8-bit intensity only texture map
ga3DTexRGB332		- 3:3:2 RGB texture map
ga3DTexRGB444		- 4:4:4 RGB texture map
ga3DTexRGB555		- 5:5:5 RGB texture map
ga3DTexRGB565		- 5:6:5 RGB texture map
ga3DTexRGB888		- 8:8:8 RGB texture map
ga3DTexRGB2222		- 2:2:2:2 ARGB texture map
ga3DTexRGB4444		- 4:4:4:4 ARGB texture map
ga3DTexRGB1555		- 1:5:5:5 ARGB texture map
ga3DTexRGB8888		- 8:8:8:8 ARGB texture map
ga3DTexCompressed	- Compressed texture map (proprietry format)
ga3DSquareTex		- Textures must be square in dimensions
ga3DPwr2Tex			- Textures dimensions must be a power of two
ga3DTexMipMap		- Texture data is part of a mip-map hierarchy
ga3DTexSysMem		- Texture data is in system memory for bus mastering
****************************************************************************/
typedef enum {
	ga3DMultiTexture				= 0x0000000F,
	ga3DTexture0					= 0x00000001,
	ga3DTexture1					= 0x00000002,
	ga3DTexture2					= 0x00000003,
	ga3DMaxTextures					= 0x00000003,
	ga3DTexCI1						= 0x00000004,
	ga3DTexCI2						= 0x00000008,
	ga3DTexCI4						= 0x00000010,
	ga3DTexCI8						= 0x00000020,
	ga3DTexAlpha4					= 0x00000040,
	ga3DTexAlpha8					= 0x00000080,
	ga3DTexIntensity4				= 0x00000100,
	ga3DTexIntensity8				= 0x00000200,
	ga3DTexRGB332					= 0x00000400,
	ga3DTexRGB444					= 0x00000800,
	ga3DTexRGB555					= 0x00001000,
	ga3DTexRGB565					= 0x00002000,
	ga3DTexRGB888					= 0x00004000,
	ga3DTexARGB2222					= 0x00008000,
	ga3DTexARGB4444					= 0x00010000,
	ga3DTexARGB1555					= 0x00020000,
	ga3DTexARGB8888					= 0x00040000,
	ga3DTexCompressed				= 0x00080000,
	ga3DSquareTex					= 0x08000000,
	ga3DPwr2Tex						= 0x10000000,
	ga3DTexMipMap					= 0x20000000,
	ga3DTexSysMem					= 0x40000000
	} GA_TextureFormatFlagsType;

/****************************************************************************
REMARKS:
Flags for hardware stencil buffer formats supported by the graphics mode
returned in the StencilFormats member of the GA_modeInfo structure. These
flags define the hardware stencil buffer formats of the particular graphics
mode, and are only valid if the gaHaveAccel3D flag is defined in the
Attributes member of the GA_modeInfo structure.

Note:	These flags are also used to enable a specific stencil buffer format
		when stencil buffering is enabled. Enabling a stencil buffer format
		other than one of the ones defined as being supported will result
		in undefined behaviour.

HEADER:
nucleus/ga3d.h

MEMBERS:
ga3DStencilBuffer1	- 1-bit stencil buffer
ga3DStencilBuffer2	- 2-bit stencil buffer
ga3DStencilBuffer4	- 4-bit stencil buffer
ga3DStencilBuffer8	- 8-bit stencil buffer
****************************************************************************/
typedef enum {
	ga3DStencilBuffer1				= 0x00000001,
	ga3DStencilBuffer2  			= 0x00000002,
	ga3DStencilBuffer4  			= 0x00000004,
	ga3DStencilBuffer8				= 0x00000008
	} GA_StencilFormatFlagsType;

/****************************************************************************
REMARKS:
Flags for polygon modes supported by the graphics mode and returned
in the PolyMode member of the GA_modeInfo's HW3DCaps structure. These are
also the flags used to test what modes are supported concurrently via
calling the Test3DState function.

Note:	These flags are also used to enable a specific polygon rendering mode
		when changing the 3D rendering state. Enabling a polygon mode
		other than one of the ones defined as being supported will result
		in undefined behaviour.

HEADER:
nucleus/ga3d.h

MEMBERS:
ga3DPolyModePoint	- Point rendering mode
ga3DPolyModeLine	- Line rendering mode
ga3DPolyModeFill	- Fill rendering mode
****************************************************************************/
typedef enum {
	ga3DPolyModePoint				= 0x0001,
	ga3DPolyModeLine    			= 0x0002,
	ga3DPolyModeFill    			= 0x0004
	} GA_PolyModeType;

/****************************************************************************
REMARKS:
Flags for perspective correction supported by the graphics mode and returned
in the PersectiveMode member of the GA_modeInfo's HW3DCaps structure. These
are also the flags used to test what modes are supported concurrently via
calling the Test3DState function. Fast perspective correction if supported
provides for higher performance at the expensive of a minor loss in visual
quality. In many cases fast perspective correction looks as good as full
per pixel perspective correction (depending on the hardware of course).

Note:	These flags are also used to enable a specific perspective correction
		mode when changing the 3D rendering state. Enabling a perspective
		correction mode other than one of the ones defined as being
		supported will result in undefined behaviour.

HEADER:
nucleus/ga3d.h

MEMBERS:
ga3DPerspectiveNone		- No perspective correction
ga3DPerspectiveFast		- Fast perspective correction
ga3DPerspectiveCorrect	- Full per pixel perspective correction
****************************************************************************/
typedef enum {
	ga3DPerspectiveNone				= 0x0000,
	ga3DPerspectiveFast				= 0x0001,
	ga3DPerspectiveCorrect			= 0x0002
	} GA_PersectiveModeType;

/****************************************************************************
REMARKS:
Flags for shading modes supported by the graphics mode and returned
in the ShadeMode member of the GA_modeInfo's HW3DCaps structure. These are
also the flags used to test what modes are supported concurrently via
calling the Test3DState function.

Note:	These flags are also used to enable a specific shading mode
		when changing the 3D rendering state. Enabling a shading mode
		other than one of the ones defined as being supported will result
		in undefined behaviour.

HEADER:
nucleus/ga3d.h

MEMBERS:
ga3DFlatShade	- Flat shading
ga3DSmoothShade	- Smooth shading
****************************************************************************/
typedef enum {
	ga3DFlatShade					= 0x0000,
	ga3DSmoothShade					= 0x0001
	} GA_ShadeModeType;

/****************************************************************************
REMARKS:
Flags for backface culling supported by the graphics mode and returned
in the BackfaceCull member of the GA_modeInfo's HW3DCaps structure.
These are also the flags used to test what modes are supported concurrently
via calling the Test3DState function. For clockwise backface culling,
vertices are determines to be backfacing if the cross product of the two
vectors constructed from the first three vertices of the polygon in a
clockwise direction results in a vector that points into the screen.
Likewise counter clockwise backface culling reverses this sense.

Note:	These flags are also used to enable a specific backface culling mode
		when changing the 3D rendering state. Enabling a backface culling
		mode other than one of the ones defined as being supported will
		result in undefined behaviour.

HEADER:
nucleus/ga3d.h

MEMBERS:
ga3DCullNone	- No backface culling
ga3DCullCW		- Clockwise backface culling
ga3DCullCCW		- Counter Clockwise backface culling
****************************************************************************/
typedef enum {
	ga3DCullNone					= 0x0000,
	ga3DCullCW						= 0x0001,
	ga3DCullCCW						= 0x0002
	} GA_BackfaceCullType;

/****************************************************************************
REMARKS:
Flags for dithering supported by the graphics mode and returned
in the DitherMode member of the GA_modeInfo's HW3DCaps structure. These are
also the flags used to test what modes are supported concurrently via
calling the Test3DState function.

Note:	These flags are also used to enable a specific dithering mode
		when changing the 3D rendering state. Enabling a dithering mode
		other than one of the ones defined as being supported will result
		in undefined behaviour.

HEADER:
nucleus/ga3d.h

MEMBERS:
ga3DDitherOff	- No dithering
ga3DDitherOn	- Dithering enabled
****************************************************************************/
typedef enum {
	ga3DDitherOff					= 0x0000,
	ga3DDitherOn					= 0x0001
	} GA_DitherModeType;

/****************************************************************************
REMARKS:
Flags for depth buffer comparision functions supported by the graphics mode and
returned in the DepthCompFunc member of the GA_modeInfo's HW3DCaps
structure. These are also the flags used to test what modes are supported
concurrently via calling the Test3DState function. When depth buffering is
enabled, the depth comparision function determines if the destation pixel
will be written or remain unchanged. If the depth comparsion /passes/, the
pixel is written. If the depth comparison /fails/ nothing is written to the
framebuffer. The depth comparision is between the depth value for the pixel
currently being rendered, and the depth value of the current destination pixel
stored in the framebuffer.

Note:	These flags are also used to enable a specific depth buffer
		comparison mode when changing the 3D rendering state. Enabling a
		depth buffer comparision mode other than one of the ones defined as
		being supported will result in undefined behaviour.

HEADER:
nucleus/ga3d.h

MEMBERS:
ga3DDepthCompNone		- No depth buffering
ga3DDepthCompNever		- Depth compare always fails
ga3DDepthCompLess		- Pass if new value is less
ga3DDepthCompEqual      - Pass if new value is equal
ga3DDepthCompLEqual     - Pass if new value is less than or equal
ga3DDepthCompGreater    - Pass if new value is greater
ga3DDepthCompNotEqual   - Pass if new value is not equal
ga3DDepthCompGEqual     - Pass if new value is greater than or equal
ga3DDepthCompAlways     - Depth compare always passes
****************************************************************************/
typedef enum {
	ga3DDepthCompNone				= 0x0000,
	ga3DDepthCompNever				= 0x0001,
	ga3DDepthCompLess               = 0x0002,
	ga3DDepthCompEqual              = 0x0004,
	ga3DDepthCompLEqual             = 0x0008,
	ga3DDepthCompGreater            = 0x0010,
	ga3DDepthCompNotEqual           = 0x0020,
	ga3DDepthCompGEqual             = 0x0040,
	ga3DDepthCompAlways             = 0x0080
	} GA_DepthCompFuncType;

/****************************************************************************
REMARKS:
Flags for alpha buffer comparision functions supported by the graphics mode and
returned in the AlphaCompFunc member of the GA_modeInfo's HW3DCaps
structure. These are also the flags used to test what modes are supported
concurrently via calling the Test3DState function. When alpha buffering is
enabled (this requires destination alpha to be supported by the current
display mode, where the hardware stores the current alpha values for each
pixel in the framebuffer), the alpha comparision function determines if the
destation pixel will be written or remain unchanged. If the alpha comparsion
/passes/, the pixel is written. If the alpha comparison /fails/ nothing is
written to the framebuffer. The alpha comparision is between the alpha value
for the source pixel currently being rendered, and the alpha value of the
current destination pixel stored in the framebuffer.

Note:	These flags are also used to enable a specific alpha buffer
		comparison mode when changing the 3D rendering state. Enabling a
		alpha buffer comparision mode other than one of the ones defined as
		being supported will result in undefined behaviour.

HEADER:
nucleus/ga3d.h

MEMBERS:
ga3DAlphaCompNone		- No alpha compare
ga3DAlphaCompNever      - Alpha compare always fails
ga3DAlphaCompLess       - Pass if new value is less
ga3DAlphaCompEqual      - Pass if new value is equal
ga3DAlphaCompLEqual     - Pass if new value is less than or equal
ga3DAlphaCompGreater    - Pass if new value is greater
ga3DAlphaCompNotEqual   - Pass if new value is not equal
ga3DAlphaCompGEqual     - Pass if new value is greater than or equal
ga3DAlphaCompAlways     - Alpha compare always passes
****************************************************************************/
typedef enum {
	ga3DAlphaCompNone              	= 0x0000,
	ga3DAlphaCompNever              = 0x0001,
	ga3DAlphaCompLess               = 0x0002,
	ga3DAlphaCompEqual              = 0x0004,
	ga3DAlphaCompLEqual             = 0x0008,
	ga3DAlphaCompGreater            = 0x0010,
	ga3DAlphaCompNotEqual           = 0x0020,
	ga3DAlphaCompGEqual             = 0x0040,
	ga3DAlphaCompAlways             = 0x0080
	} GA_AlphaCompFuncType;

/****************************************************************************
REMARKS:
Flags for alpha blending functions supported by the graphics mode and returned
in the AlphaBlendFunc member of the GA_modeInfo's HW3DCaps structure. These
are also the flags used to test what modes are supported concurrently via
calling the Test3DState function. The values in here define the the alpha
blending functions passed to Set3DAlphaBlendSrcFunc and
Set3DAlphaBlendDstFunc. Essentially the blend function defines how to
combine the source and destination pixel color together to get the resulting
destination color during rendering. The formula used for this is defined
as:

	DstColor = SrcColor * SrcFunc + DstColor * DstFunc;

If the source alpha blending function is set to ga3DConstantAlpha, the
/SrcFunc/ above becomes:

	SrcFunc = ConstAlpha

If the destination alpha blending function is set to ga3DOneMinusDstAlpha
then /DstFunc/ above becomes:

	DstFunc = (1-DstAlpha)

and the final equation becomes (note that each color channel is multiplied
individually):

	DstColor = SrcColor * ConstAlpha + DstColor * (1-DstAlpha)

Although the above is a completely contrived example, it does illulstrate
how the functions defined below combine to allow you to build complex and
interesting blending functions. For simple source alpha transparency, the
following formula is usually used:

	DstColor = SrcColor * SrcAlpha + DstColor * (1-SrcAlpha)

Note:	The constant color value set by a call to Set3DColor, and the
		constant alpha value set by a call to Set3DAlphaValue.

Note:	Setting a blending function that uses the destination alpha
		components is only supported if the framebuffer currently supports
		destination alpha.

Note:	These flags are also used to enable a specific alpha blending
		function when changing the 3D rendering state. Enabling an alpha
		blending function other than one of the ones defined as being
		supported will result in undefined behaviour.

HEADER:
nucleus/ga3d.h

MEMBERS:
ga3DBlendNone					- No alpha blending
ga3DBlendZero					- Blend factor is always zero
ga3DBlendOne                    - Blend factor is always one
ga3DBlendSrcColor				- Blend factor is source color
ga3DBlendOneMinusSrcColor		- Blend factor is 1-source color
ga3DBlendSrcAlpha				- Blend factor is source alpha
ga3DBlendOneMinusSrcAlpha		- Blend factor is 1-source alpha
ga3DBlendDstAlpha				- Blend factor is destination alpha
ga3DBlendOneMinusDstAlpha		- Blend factor is 1-destination alpha
ga3DBlendDstColor				- Blend factor is destination color
ga3DBlendOneMinusDstColor		- Blend factor is 1-destination color
ga3DBlendSrcAlphaSaturate		- Blend factor is src alpha saturation
ga3DBlendConstantColor			- Blend factor is a constant color
ga3DBlendOneMinusConstantColor	- Blend factor is 1-constant color
ga3DBlendConstantAlpha			- Blend factor is constant alpha
ga3DBlendOneMinusConstantAlpha	- Blend factor is 1-constant alpha
****************************************************************************/
typedef enum {
	ga3DBlendNone					= 0x0000,
	ga3DBlendZero                   = 0x0001,
	ga3DBlendOne                    = 0x0002,
	ga3DBlendSrcColor               = 0x0004,
	ga3DBlendOneMinusSrcColor       = 0x0008,
	ga3DBlendSrcAlpha               = 0x0010,
	ga3DBlendOneMinusSrcAlpha       = 0x0020,
	ga3DBlendDstAlpha               = 0x0040,
	ga3DBlendOneMinusDstAlpha       = 0x0080,
	ga3DBlendDstColor               = 0x0100,
	ga3DBlendOneMinusDstColor       = 0x0200,
	ga3DBlendSrcAlphaSaturate       = 0x0400,
	ga3DBlendConstantColor          = 0x0800,
	ga3DBlendOneMinusConstantColor  = 0x1000,
	ga3DBlendConstantAlpha          = 0x2000,
	ga3DBlendOneMinusConstantAlpha  = 0x4000
	} GA_AlphaBlendFuncType;

/****************************************************************************
REMARKS:
Flags for stencil compare functions supported by the graphics mode and returned
in the StencilCompFunc member of the GA_modeInfo's HW3DCaps structure. These
are also the flags used to test what modes are supported concurrently via
calling the Test3DState function. When stencil buffering is enabled, the
stencil comparision function determines if the destation pixel will be
written or remain unchanged. If the stencil comparsion /passes/, the pixel
is written. If the stencil comparison /fails/ nothing is written to the
framebuffer.

TODO: Define where the values for the stencil compare operation come from??

Note:	These flags are also used to enable a specific stencil compare
		function when changing the 3D rendering state. Enabling a stencil
		compare function other than one of the ones defined as being
		supported will result in undefined behaviour.

HEADER:
nucleus/ga3d.h

MEMBERS:
ga3DStencilCompNone     - No stencil buffering
ga3DStencilCompNever    - Stencil compare always fails
ga3DStencilCompLess     - Pass if new value is less
ga3DStencilCompEqual    - Pass if new value is equal
ga3DStencilCompLEqual   - Pass if new value is less than or equal
ga3DStencilCompGreater  - Pass if new value is greater
ga3DStencilCompNotEqual - Pass if new value is not equal
ga3DStencilCompGEqual   - Pass if new value is greater than or equal
ga3DStencilCompAlways   - Stencil compare always passes
****************************************************************************/
typedef enum {
	ga3DStencilCompNone             = 0x0000,
	ga3DStencilCompNever            = 0x0001,
	ga3DStencilCompLess             = 0x0002,
	ga3DStencilCompEqual            = 0x0004,
	ga3DStencilCompLEqual           = 0x0008,
	ga3DStencilCompGreater          = 0x0010,
	ga3DStencilCompNotEqual         = 0x0020,
	ga3DStencilCompGEqual           = 0x0040,
	ga3DStencilCompAlways           = 0x0080
	} GA_StencilCompFuncType;

/****************************************************************************
REMARKS:
Flags for stencil functions supported by the graphics mode and returned
in the StencilFunc member of the GA_modeInfo's HW3DCaps structure. These
are also the flags used to test what modes are supported concurrently via
calling the Test3DState function.

TODO: Need to define how this fits into the stenciling operation???

Note:	These flags are also used to enable a specific stencil function
		when changing the 3D rendering state. Enabling a stencil function
		other than one of the ones defined as being supported will result
		in undefined behaviour.

HEADER:
nucleus/ga3d.h

MEMBERS:
ga3DStencilZero		- Stencil value set to zero
ga3DStencilKeep		- Stencil value is unchanged
ga3DStencilReplace	- Stencil value is replaced
ga3DStencilIncr		- Stencil value is incremented
ga3DStencilDecr		- Stencil value is decremented
ga3DStencilInvert	- Stencil value is inverted
****************************************************************************/
typedef enum {
	ga3DStencilZero                 = 0x0001,
	ga3DStencilKeep                 = 0x0002,
	ga3DStencilReplace              = 0x0004,
	ga3DStencilIncr                 = 0x0008,
	ga3DStencilDecr                 = 0x0010,
	ga3DStencilInvert               = 0x0020
	} GA_StencilFuncType;

/****************************************************************************
REMARKS:
Flags for fog modes supported by the graphics mode and returned in the
FogMode member of the GA_modeInfo's HW3DCaps structure. These are
also the flags used to test what modes are supported concurrently via
calling the Test3DState function.

Note:	These flags are also used to enable a specific fog mode
		when changing the 3D rendering state. Enabling a fog mode
		other than one of the ones defined as being supported will result
		in undefined behaviour.

HEADER:
nucleus/ga3d.h

MEMBERS:
ga3DFogNone			- No fog
ga3DFogPerVertex	- Per vertex fog
ga3DFogPerPixel		- Per pixel fog
****************************************************************************/
typedef enum {
	ga3DFogNone						= 0x0000,
	ga3DFogPerVertex                = 0x0001,
	ga3DFogPerPixel                 = 0x0002
	} GA_FogModeType;

/****************************************************************************
REMARKS:
Flags for fog functions supported by the graphics mode and returned
in the FogFunc member of the GA_modeInfo's HW3DCaps structure. These are
also the flags used to test what modes are supported concurrently via
calling the Test3DState function. The fog function determines how the fog
values are determines from the distance the pixel is away from the viewer.
The further the pixel is away from the viewer, the more of the fog color
is blended into the main color making far way objects slowly take on the
color of the fog.

TODO: Define how the fogging function is applied to the pixels in the
	  framebuffer.

TODO: Define the formulas for each of the fog types

Note:	These flags are also used to enable a specific **blah**
		when changing the 3D rendering state. Enabling a **blah**
		other than one of the ones defined as being supported will result
		in undefined behaviour.

HEADER:
nucleus/ga3d.h

MEMBERS:
ga3DFogLinear		- Linear fog equation
ga3DFogExp			- Exponential fog equation
ga3DFogExp2			- Exponential squared fog equation
ga3DFogTableBased	- Table based fog
****************************************************************************/
typedef enum {
	ga3DFogLinear					= 0x0001,
	ga3DFogExp                      = 0x0002,
	ga3DFogExp2                     = 0x0004,
	ga3DFogTableBased               = 0x0008
	} GA_FogFuncType;

/****************************************************************************
REMARKS:
Flags for color key functions supported by the graphics mode and returned
in the ColorKeyFunc member of the GA_modeInfo's HW3DCaps structure. These
are also the flags used to test what modes are supported concurrently via
calling the Test3DState function. Color keying can be done on the source
pixel color or the destination pixel color, and the color key may be
defined as a single color or a range of colors. Ranges of colors are
supported so that you can still color key data that has passed through
blending and filtering operations (real alpha blending if available is
much better than this).

Note:	These flags are also used to enable a specific color key modes
		when changing the 3D rendering state. Enabling a color key mode
		other than one of the ones defined as being supported will result
		in undefined behaviour.

HEADER:
nucleus/ga3d.h

MEMBERS:
ga3DColorKeyNone		- No color keying
ga3DColorSrcKeySingle	- Source color keying with a single key color
ga3DColorSrcKeyRange	- Source color keying with a range of colors
ga3DColorDstKeySingle	- Destination color keying with a single key color
ga3DColorDstKeyRange	- Destination color keying with a range of colors
****************************************************************************/
typedef enum {
	ga3DColorKeyNone				= 0x0000,
	ga3DColorSrcKeySingle           = 0x0001,
	ga3DColorSrcKeyRange            = 0x0002,
	ga3DColorDstKeySingle           = 0x0004,
	ga3DColorDstKeyRange            = 0x0080
	} GA_ColorKeyFuncType;

/****************************************************************************
REMARKS:
Flags for texture functions supported by the graphics mode and returned in the
TextureFunc member of the GA_modeInfo's HW3DCaps structure. These are
also the flags used to test what modes are supported concurrently via
calling the Test3DState function.

TODO: This needs to be specified as I am not quite sure how these modulation
	  functions actually work in practice. Also this may need to be fleshed
	  out with more functionality than that which OpenGL provides (especially
	  for OpenGL multi-texture extensions).

Note:	These flags are also used to enable a specific texture function
		when changing the 3D rendering state. Enabling a texture function
		other than one of the ones defined as being supported will result
		in undefined behaviour.

HEADER:
nucleus/ga3d.h

MEMBERS:
ga3DTexModulate	- Texture is modulated with the polygon colors
ga3DTexDecal	- Texture is decaled over the top of the polygon
ga3DTexBlend	- Texture is blended with the polygon colors
ga3DTexReplace	- Texture replaces the polygon colors
ga3DTexAdd		- Texture adds to the polygon colors
****************************************************************************/
typedef enum {
	ga3DTexModulate					= 0x0001,
	ga3DTexDecal                    = 0x0002,
	ga3DTexBlend                    = 0x0004,
	ga3DTexReplace                  = 0x0008,
	ga3DTexAdd                      = 0x0010
	} GA_TextureFuncType;

/****************************************************************************
REMARKS:
Flags for texture magnification function supported by the graphics mode and
returned in the TextureMagFilter member of the GA_modeInfo's HW3DCaps
structure. These are also the flags used to test what modes are supported
concurrently via calling the Test3DState function. The texture magnification
function is applied when a polygon is larger in size than the texture being
applied to it, and the texels from the texture needs to be 'blown up' or
magnified. Nearest pixel mode will produce blocky textures, while linear
mode will do bi-linear filtering between the closest pixels to smooth
the blockiness of the texture.

Note:	These flags are also used to enable a specific texture magnification
		functions when changing the 3D rendering state. Enabling a texture
		magnification function other than one of the ones defined as being
		supported will result in undefined behaviour.

HEADER:
nucleus/ga3d.h

MEMBERS:
ga3DTexMagNearest	- Nearest pixel is chosen
ga3DTexMagLinear	- Bi-linear interpolation used to blend closest pixels
****************************************************************************/
typedef enum {
	ga3DTexMagNearest				= 0x0001,
	ga3DTexMagLinear                = 0x0002
	} GA_TextureMagFilterType;

/****************************************************************************
REMARKS:
Flags for texture minification functions supported by the graphics mode and
returned in the TextureMinFilter member of the GA_modeInfo's HW3DCaps
structure. These are also the flags used to test what modes are supported
concurrently via calling the Test3DState function. The texture minification
function is applied when a polygon is smaller in size than the texture being
applied to it, and the texels from the texture needs to be shrunk or
minified. Nearest pixel mode will produce texture aliasing, while linear
mode will do bi-linear filtering between the closest pixels to smooth
the blockiness of the texture. Mip-mapping is another technique used to
produce higher quality texture mapping, and a mip-map is defined as a
hierarchy of progressively smaller textures.

When mip-mapping is enabled, the polygon being rendered is assigned a
mip-map level that is based on its pixel coverage. The mip-maps in the
hierarchy are also assigned mip-map levels, where the first is at level 1,
the second at level 2, the third at level 4 etc (going up in powers of two).
Then pixels are chosen and blended together from texturs in the mip-map
hierarchy to compute the final pixel color depending on the mip-mapping mode.

When ga3DMinNearestMipMapNearest mode is chosen, the closest mip-map in the
mip-map hierarchy is chosen based the polygons mip-map level. Then the
nearest texel within that texture is chosen for the final pixel color.

When ga3DMinLinearMipMapNearest mode is chosen, the closest mip-map in the
mip-map hierarchy is chosen based the polygons mip-map level. Then the
closest texels within that texture are bi-linearly filtered to produce the
final pixel color.

When ga3DMinNearestMipMapLinear mode is chosen, the two closest mip-maps in
the mip-map hierarchy are chosen based the polygons mip-map level. Then the
nearest texels within each texture are selected and the two texels are
bi-linearly bi-linearly filtered to produce the final pixel color.

When ga3DMinLinearMipMapLinear mode is chosen, the two closest mip-maps in
the mip-map hierarchy are chosen based the polygons mip-map level. Then the
closest texels within each texture are bi-linear filterd to produce the
intermediate texel color for each texture. Then these two texel colors are
are bi-linearly filtered to produce the final pixel color. This operation
is what is commonly known as /tri-linear filtering/.

Note:	These flags are also used to enable a specific texture minification
		function when changing the 3D rendering state. Enabling a texture
		minification function other than one of the ones defined as being
		supported will result in undefined behaviour.

HEADER:
nucleus/ga3d.h

MEMBERS:
ga3DTexMinNearest				- Nearest pixel is chosen
ga3DTexMinLinear				- Bi-linear interpolation between closest pixels
ga3DTexMinNearestMipMapNearest	- Mip-mapping with nearest pixel
ga3DTexMinLinearMipMapNearest	- Mip-mapping with linear/nearest filtering
ga3DTexMinNearestMipMapLinear   - Mip-mapping with nearest/linear filtering
ga3DTexMinLinearMipMapLinear	- Mip-mapping with tri-linear filtering
****************************************************************************/
typedef enum {
	ga3DTexMinNearest				= 0x0000,
	ga3DTexMinLinear                = 0x0001,
	ga3DTexMinNearestMipMapNearest  = 0x0002,
	ga3DTexMinLinearMipMapNearest   = 0x0004,
	ga3DTexMinNearestMipMapLinear   = 0x0008,
	ga3DTexMinLinearMipMapLinear    = 0x0010
	} GA_TextureMinFilterType;

/****************************************************************************
REMARKS:
Flags for texture wrapping modes supported by the graphics mode and returned
in the TextureWrapMode member of the GA_modeInfo's HW3DCaps structure. These
are also the flags used to test what modes are supported concurrently via
calling the Test3DState function. Texture wrapping is what happens when the
texture coordinates for the texture go outside of the texture map that is
being applied to the polygon. If clamp mode is selected, the texture
coordinates will be clamped to the maximum coordinates of the current
texture. If wrap mode is enabled, the texture coordinates will wrap back
to the opposite end, effectively enabling a /tiled/ texture mode (used
for backgrounds etc where a small texture constantly repeats).

Note:	These flags are also used to enable a specific texture wrap modes
		when changing the 3D rendering state. Enabling a texture wrap mode
		other than one of the ones defined as being supported will result
		in undefined behaviour.

HEADER:
nucleus/ga3d.h

MEMBERS:
ga3DTexWrapClampS	- Texture values are clamped in the S direction
ga3DTexWrapRepeatS	- Texture values are repeated in the S direction
ga3DTexWrapClampT	- Texture values are clamped in the T direction
ga3DTexWrapRepeatT	- Texture values are repeated in the T direction
****************************************************************************/
typedef enum {
	ga3DTexWrapClampS				= 0x0001,
	ga3DTexWrapRepeatS              = 0x0002,
	ga3DTexWrapClampT               = 0x0004,
	ga3DTexWrapRepeatT              = 0x0008
	} GA_TextureWrapModeType;

/****************************************************************************
REMARKS:
Flags for buffer clearing flags, passed to the Clear3DBuffers functions.
These flags define which buffers should be cleared during the clear function.
High performance hardware usually has a fast buffer clear function, that can
clear multiple buffers at once faster than clearing each buffer individually,
so you should try to clear all the buffers you need cleared in a single
operation.

HEADER:
nucleus/ga3d.h

MEMBERS:
ga3DClearColor		- Clear the color buffer
ga3DClearDepth		- Clear the depth buffer
ga3DClearStencil	- Clear the stencil buffer
****************************************************************************/
typedef enum {
	ga3DClearColor					= 0x0001,
	ga3DClearDepth                  = 0x0002,
	ga3DClearStencil                = 0x0004
	} GA_BufferClearFlagsType;

/****************************************************************************
REMARKS:
3D rendering state parameter block. This structure defines the rendering
state information, and is used to return the 3D rendering capabilities
information to the application via the HW3DCaps member of the GA_modeInfo
structure. It is also used to pass 3D rendering state information to the
Test3DState function, which allows an application to specifically test if
a combination of 3D rendering states is supported by the hardware. State
information is changed via the individual state change functions rather
than a single call using this structure.

Note:	The dwSize member is intended for future compatibility, and
		should be set to the size of the structure as defined in this
		header file. Future drivers will be compatible with older software
		by examiming this value.

HEADER:
nucleus/ga3d.h

MEMBERS:
dwSize				- Set to size of structure in bytes
PolyMode			- Polygon mode type (point, line, fill)
PerspectiveMode		- Perspective correction mode
ShadeMode			- Shading mode (flat, smooth)
CullMode			- Backface culling mode
DitherMode			- Dithering mode
DepthCompFunc		- Depth compare mode
AlphaCompFunc		- Alpha compare mode
AlphaBlendSrcFunc	- Src alpha blend function
AlphaBlendDstFunc	- Dst alpha blend function
StencilCompFunc		- Stencil compare mode
StencilFunc			- Stencil function
FogMode				- Fog mode
FogFunc				- Fog function
ColorKeyMode		- Color key mode
TextureFunc			- Texture map function
TextureMinFilter	- Texture minification filter
TextureMagFilter	- Texture magnification filter
TextureWrapMode		- Texture wrapping mode
****************************************************************************/
typedef struct _GA_3DState {
	N_int32		dwSize;
	N_uint32	PolyMode;
	N_uint32   	PerspectiveMode;
	N_uint32	ShadeMode;
	N_uint32	CullMode;
	N_uint32	DitherMode;
	N_uint32	DepthCompFunc;
	N_uint32   	AlphaCompFunc;
	N_uint32	AlphaBlendSrcFunc;
	N_uint32	AlphaBlendDstFunc;
	N_uint32   	StencilCompFunc;
	N_uint32	StencilFunc;
	N_uint32	FogMode;
	N_uint32	FogFunc;
	N_uint32	ColorKeyMode;
	N_uint32   	TextureFunc;
	N_uint32	TextureMinFilter;
	N_uint32	TextureMagFilter;
	N_uint32   	TextureWrapMode;
	} GA_3DState;

/****************************************************************************
REMARKS:
Structure representing a 3D surface. 3D surfaces can be connected together
into a chain, so that you would have a color buffer surface attached to
an associated depth buffer and stencil buffer for instance. Multiple
surfaces may be attached to other surfaces if desired (ie: depth and stencil
buffers can be shared between front/back buffers for instance).

The Offset member is the offset of the start of the surface in local video
memory, depth buffer memory or texture memory. If the texture surface is
a system memory texture to be rendering using PCI bus mastering or AGP
execute mode (if the ga3DTexSysMem flag is set when the texture is passed
to the driver), the Offset member will be the physical starting address of
the texture map in system memory (note it is a /physical/ memory address, not
a linear memory address). The BytesPerScanLine member defines the stride of
the surface in bytes, while the Width and Height members define the
dimensions of the surface map in pixels or texels.

If the surface is a texture map that is part of a mip-map hierarchy, the
ga3DTexMipMap flag will be set in the TextureAttrbutes member, and the the
next member will point to the child textures that form the remainder of the
mip-map hierarchy. All child textures will have dimensions that are smaller
than the parent texture, and hence the texture you pass in is the first and
largest texture in in the mip-map hierarchy. The last texture in the
hierarchy (which should always have a size of 1x1) will have the next
member set to NULL.

Note:	All surfaces are in packed pixel format, and the values of the
		Offset and BytesPerScanLine members must adhere to the format
		restrictions defined in the GA_modeInfo structure appropriate for
		the type of surface. For instance if the TextureStartAlign member of
		the GA_modeInfo structure is set to 8, then the texture map address
		must start on an 8 byte boundary. Likewise if the TextureStridePad
		member is set to 8, the Stride must be a multiple of 8 bytes (ie:
		if you 8-bit texture is only 5 pixels wide, the stride must be
		rounded up to 8 bytes).

Note:	The dwSize member is intended for future compatibility, and
		should be set to the size of the structure as defined in this
		header file. Future drivers will be compatible with older software
		by examiming this value.

HEADER:
nucleus/opengl.h

MEMBERS:
dwSize				- Set to size of structure in bytes
Attributes			- Surface attributes flags
TextureAttributes	- Extended attributes for texture surfaces
Width  				- Width of texture in pixels/texels
Height 				- Height of texture in pixels/texels
BytesPerScanLine	- Scanline pitch for the surface
BitsPerPixel		- Color depth for the surface
BytesPerPixel		- Number of bytes in a single pixel/texel
PixelFormat			- Pixel format information
Offset  			- Surface start address in hardware memory
Surface				- Linear pointer to surface in hardware memory
Palette				- Optional color palette for surface
Priv				- Implementation specific private pointer
Next				- Next surface in chain (NULL for end of chain)
****************************************************************************/
typedef struct _GA_3DSurface {
	N_uint32				dwSize;
	N_uint32				Attributes;
	N_uint32				TextureAttributes;
	N_int32					Width;
	N_int32					Height;
	N_int32					BytesPerScanLine;
	N_int32					BitsPerPixel;
	N_int32					BytesPerPixel;
	GA_pixelFormat  		PixelFormat;
	N_uint32    			Offset;
	void            		*Surface;
	GA_palette				*Palette;
	void 					*Priv;
	struct _GA_3DSurface	*Next;
	} GA_3DSurface;

/****************************************************************************
REMARKS:
Texture coordinate structure, containing information for the 3D texture
coordinates to be rendered by the graphics hardware. The texture coordinates
determine what portion of the texture map is applied to the polygon.

Note:	The range of values for each coordinate is defined by the MaxOOS,
		MaxOOT and MaxOOW members of the GA_modeInfo structure. These
		values represent the range of values the hardware works with, and
		you must prescale all values to match this range for correct
		results.

HEADER:
nucleus/ga3d.h

MEMBERS:
sow	- s texture coordinate (s over w), range [0-MaxOOS]
tow - t texture coordinate (t over w), range [0-MaxOOT]
oow	- 1/w coordinate, range [0-MaxOOW]
****************************************************************************/
typedef struct {
	N_flt32    	sow;
	N_flt32    	tow;
	N_flt32    	oow;
	} GA_tex;

/****************************************************************************
REMARKS:
Generic vertex containing information for the 3D points to be rendered
by the graphics hardware. The x, y and z coordinates define the location
of the vertex in 3-space (note that you are also responsible for computing
the inverse of some of these values such as ooz is 1/z scaled to the range
of maxOOZ). The r,g,b and a coordinates define the colors (and alpha) of the
vertices when rendering polygons with smooth shading enabled (they are
ignored if smooth shading is not enabled). The tex member contains up to
ga3DMaxTextures of texture coordinates, one for each texture when
multi-texturing is enabled (they are ignored if texture mapping is not
enabled).

Note:	The range of values for each coordinate is defined by the MaxOOZ
		and MaxOOW members of the GA_modeInfo structure. These values
		represent the range of values the hardware works with, and you
		must prescale all values to match this range for correct
		results.

Note:	This structure is the native format used by the graphics drivers,
		and is based on the 3Dfx Interactive Glide Rasterisation Library
		format. All new code should be written to use this vertex format.

HEADER:
nucleus/ga3d.h

MEMBERS:
x   - X coordinate in screen space, range [0-maxX]
y   - Y coordinate in screen space, range [0-maxY]
z   - Z coordinate in screen space (generally ignored)
r   - Red color coordinate, range [0-255.0]
g   - Green color coordinate, range [0-255.0]
b   - Blue color coordinate, range [0-255.0]
ooz	- Inverse of Z coordinate, range [1-maxOOZ]
a 	- Alpha coordinate, range [0-255.0]
oow	- 1/w coordinate, range [0-maxOOW]
tex	- Texture coordinates (one set for each texture)
****************************************************************************/
typedef struct {
	N_flt32    	x;
	N_flt32    	y;
	N_flt32    	z;
	N_flt32    	r;
	N_flt32    	g;
	N_flt32    	b;
	N_flt32    	ooz;
	N_flt32    	a;
	N_flt32    	oow;
	GA_tex     	tex[ga3DMaxTextures];
	} GA_vertex;

/****************************************************************************
REMARKS:
Microsoft Direct3D vertex containing information for the 3D points to be
rendered by the graphics hardware. The format of these structures is
identical to that used in the Microsoft Direct3D libraries for
compatibility with the Direct3D API. Note that as of DirectX 5, there
is no support for multiple textures so only a single texture coordinate
is provided. Hence to support multiple textures use the GA_vertex
structure not the GA_D3Dvertex structure.

Note:	This structure is compatible with the format used by Microsoft
		Direct3D, and is provided for compatibility with Direct3D and to
		allow Direct3D drivers to be written on top of a graphics driver.
		If you use this vertex format, you will have to call the D3D
		specific rendering functions.

HEADER:
nucleus/ga3d.h

MEMBERS:
sx  		- X coordinate in screen space, range [0-maxX]
sy  		- Y coordinate in screen space, range [0-maxY]
sz  		- Z coordinate in screen space (generally ignored)
rhw			- 1/w coordinate, range [0-maxOOW]
b   		- Blue color coordinate, range [0-255]
g   		- Green color coordinate, range [0-255]
r   		- Red color coordinate, range [0-255]
a 			- Alpha coordinate, range [0-255]
specular	- Specular component of vertex (unused - for compatibility)
tu			- Texture coordinate (same as s coordinate), range [0-1.0]
tv      	- Texture coordinate (same as t coordinate), range [0-1.0]
****************************************************************************/
typedef struct {
	N_flt32    	sx;
	N_flt32    	sy;
	N_flt32    	sz;
	N_flt32    	rhw;
	N_uint8    	b;
	N_uint8    	g;
	N_uint8    	r;
	N_uint8    	a;
	GA_color    specular;
	N_flt32    	tu;
	N_flt32    	tv;
	} GA_D3Dvertex;

/****************************************************************************
REMARKS:
Device driver hardware 3D setup and initialisation functions.
{secret}
****************************************************************************/
typedef struct {
	ulong		dwSize;
	N_int32		(NAPIP Set3DDepthBuffer)(N_uint32 flags,GA_buffer *depthBuf);
	N_int32		(NAPIP Set3DStencilBuffer)(N_uint32 flags,GA_buffer *stencilBuf);
	N_int32		(NAPIP Set3DAlphaBuffer)(N_uint32 flags,GA_buffer *alphaBuf);
	} GA_3DSetupFuncs;

/****************************************************************************
REMARKS:
Device driver hardware 3D state management functions.
{secret}
****************************************************************************/
typedef struct {
	ulong		dwSize;
	N_int32		(NAPIP Test3DState)(GA_3DState *state);
	N_int32		(NAPIP Save3DState)(void *state);
	void		(NAPIP Restore3DState)(void *state);
	void		(NAPIP Set3DColor)(GA_color color);
	void		(NAPIP Set3DFogColor)(GA_color color);
	void		(NAPIP Set3DClearColor)(GA_color color);
	void		(NAPIP Set3DColorKey)(GA_color colorKeyLo,GA_color colorKeyHi);
	void		(NAPIP Set3DAlphaValue)(N_uint32 alpha);
	void		(NAPIP Set3DDepthValue)(N_flt32 depth);
	void		(NAPIP Set3DStencilValue)(N_int32 stencil);
	void		(NAPIP Set3DPolyMode)(N_uint32 mode);
	void		(NAPIP Set3DPerspectiveMode)(N_uint32 mode);
	void		(NAPIP Set3DShadeMode)(N_uint32 mode);
	void		(NAPIP Set3DCullMode)(N_uint32 mode);
	void		(NAPIP Set3DDitherMode)(N_uint32 mode);
	void		(NAPIP Set3DDepthCompFunc)(N_uint32 func);
	void		(NAPIP Set3DAlphaCompFunc)(N_uint32 func);
	void		(NAPIP Set3DAlphaBlendSrcFunc)(N_uint32 func);
	void		(NAPIP Set3DAlphaBlendDstFunc)(N_uint32 func);
	void		(NAPIP Set3DStencilCompFunc)(N_uint32 func);
	void		(NAPIP Set3DStencilFunc)(N_uint32 func);
	void		(NAPIP Set3DFogMode)(N_uint32 mode);
	void		(NAPIP Set3DFogFunc)(N_uint32 func);
	void		(NAPIP Set3DColorKeyMode)(N_uint32 mode);
	void		(NAPIP Set3DTextureFunc)(N_uint32 func);
	void		(NAPIP Set3DTextureMinFilter)(N_uint32 filter);
	void		(NAPIP Set3DTextureMagFilter)(N_uint32 filter);
	void		(NAPIP Set3DTextureWrapMode)(N_uint32 mode);
	void		(NAPIP Set3DFogMap)(GA_palette *pal,N_int32 num,N_int32 index);
	void		(NAPIP Set3DTextureMap)(N_uint32 flags,GA_3DSurface *texture,void *data);
	void		(NAPIP Set3DTextureMapBM)(N_uint32 flags,GA_3DSurface *texture,void *data,N_uint32 physAddr);
	void		(NAPIP Set3DTextureColorMap)(GA_palette *pal,N_int32 num,N_int32 index);
	void		(NAPIP Use3DTextureMap)(N_uint32 flags,GA_3DSurface *texture);
	void		(NAPIP Compress3DTextureMap)(N_uint32 flags,GA_3DSurface *texture);
	} GA_3DStateFuncs;

/****************************************************************************
REMARKS:
Device driver hardware 3D native rendering functions.
{secret}
****************************************************************************/
typedef struct {
	ulong		dwSize;
	void		(NAPIP Clear3DBuffers)(N_int32 flags,N_int32 left,N_int32 top,N_int32 right,N_int32 bottom);
	void		(NAPIP Start3DRender)(void);
	void		(NAPIP Draw3DLine)(GA_vertex *v1,GA_vertex *v2);
	void		(NAPIP Draw3DTri)(GA_vertex *v1,GA_vertex *v2,GA_vertex *v3);
	void		(NAPIP Draw3DQuad)(GA_vertex *v1,GA_vertex *v2,GA_vertex *v3,GA_vertex *v4);
	void		(NAPIP Draw3DLineList)(N_int32 index[],GA_vertex vertices[]);
	void		(NAPIP Draw3DTriList)(N_int32 index[],GA_vertex vertices[]);
	void		(NAPIP Draw3DQuadList)(N_int32 index[],GA_vertex vertices[]);
	void		(NAPIP Draw3DTriStrip)(N_int32 index[],GA_vertex vertices[]);
	void		(NAPIP Draw3DQuadStrip)(N_int32 index[],GA_vertex vertices[]);
	void		(NAPIP Draw3DTriFan)(N_int32 index[],GA_vertex vertices[]);
	void		(NAPIP End3DRender)(void);
	void		(NAPIP Flush3DRender)(void);
	} GA_3DRenderFuncs;

/****************************************************************************
REMARKS:
Device driver hardware Direct3D compatible rendering functions.
{secret}
****************************************************************************/
typedef struct {
	ulong		dwSize;
	void		(NAPIP ClearD3DBuffers)(N_int32 flags,N_int32 left,N_int32 top,N_int32 right,N_int32 bottom);
	void		(NAPIP StartD3DRender)(void);
	void		(NAPIP DrawD3DLine)(GA_D3Dvertex *v1,GA_D3Dvertex *v2);
	void		(NAPIP DrawD3DTri)(GA_D3Dvertex *v1,GA_D3Dvertex *v2,GA_D3Dvertex *v3);
	void		(NAPIP DrawD3DQuad)(GA_D3Dvertex *v1,GA_D3Dvertex *v2,GA_D3Dvertex *v3,GA_D3Dvertex *v4);
	void		(NAPIP DrawD3DLineList)(N_int32 index[],GA_D3Dvertex vertices[]);
	void		(NAPIP DrawD3DTriList)(N_int32 index[],GA_D3Dvertex vertices[]);
	void		(NAPIP DrawD3DQuadList)(N_int32 index[],GA_D3Dvertex vertices[]);
	void		(NAPIP DrawD3DTriStrip)(N_int32 index[],GA_D3Dvertex vertices[]);
	void		(NAPIP DrawD3DQuadStrip)(N_int32 index[],GA_D3Dvertex vertices[]);
	void		(NAPIP DrawD3DTriFan)(N_int32 index[],GA_D3Dvertex vertices[]);
	void		(NAPIP EndD3DRender)(void);
	void		(NAPIP FlushD3DRender)(void);
	} GA_D3DRenderFuncs;

#pragma	pack()

#endif  /* __NUCLEUS_GA3D_H */
