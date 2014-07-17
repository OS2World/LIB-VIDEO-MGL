/****************************************************************************
*
*						SciTech MGL Graphics Library
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
* Description:	Internal header file for the SciTech MGL defining the
*				contents of the private MGL device context variables.
*				This information is useful for high performance add on
*				libraries like Mesa so they can directly access the MGL's
*				high performance software rasterisation functions.
*
*				** NOTE ** The internal structures in here are subject
*						   to change from release to release, so any
*						   libraries built on these functions will need to
*						   be re-compiled for each MGL release!
*
****************************************************************************/

#ifndef	__MGL_INT_H
#define	__MGL_INT_H

#define BUILD_MGL
#include "mgraph.h"
#include "nucleus/graphics.h"
#include "nucleus/ref2d.h"

/*---------------------- Macro and type definitions -----------------------*/

#pragma pack(1)

/* Internal device context specific structures */

typedef struct {
	void		*SGICtx;	/* SGI OpenGL context						*/
	void		*SGISurf;	/* SGI OpenGL surface						*/
	MGL_HDC		hdc;		/* Handle to DC for drawing to buffer		*/
	MGL_HGLRC	glrc;		/* OpenGL rendering context					*/
	} gendc_vars;

#if defined(MGLWIN)
typedef struct {
	void		*SGICtx;	/* SGI OpenGL context						*/
	void		*SGISurf;	/* SGI OpenGL surface						*/
	MGL_HDC		hdc;		/* Handle to DC for drawing to buffer		*/
	MGL_HGLRC	glrc;		/* OpenGL rendering context					*/
	} fulldc_vars;

typedef struct {
	void		*SGICtx;	/* SGI OpenGL context						*/
	void		*SGISurf;	/* SGI OpenGL surface						*/
	MGL_HDC		hdc;		/* Handle to DC for drawing to buffer		*/
	MGL_HGLRC	glrc;		/* OpenGL rendering context					*/
	MGL_HBITMAP	hbm;		/* Handle to bitmap for DC					*/
	} memdc_vars;

typedef struct {
	void		*SGICtx;	/* SGI OpenGL context						*/
	void		*SGISurf;	/* SGI OpenGL surface						*/
	MGL_HDC		hdc;		/* Currently active DC for all output		*/
	MGL_HGLRC	glrc;		/* OpenGL rendering context					*/
	MGL_HDC     hdcOwned;	/* Constant DC for window, NULL if none		*/
	MGL_HWND	hwnd;		/* Handle to associated window mgr window	*/
	MGL_HPALETTE hpal;		/* Currently active palette for WinDC		*/
	ibool		palNoStatic;/* True when in SYSPAL_NOSTATIC mode		*/
	ibool		isBackground;/* True when window is in background		*/
	} windc_vars;

typedef union {
	gendc_vars	gendc;		/* Generic DC variables						*/
	fulldc_vars	fulldc;		/* Fullscreen DC specific variables			*/
	windc_vars	windc;		/* Window DC specific variables				*/
	memdc_vars	memdc;		/* Memory DC specific variables				*/
	} wm_vars;

#elif defined(MGLX11)

typedef struct {
	void		*SGICtx;	/* SGI OpenGL context						*/
	void		*SGISurf;	/* SGI OpenGL surface						*/
	MGL_HDC		hdc;		/* Handle to DC for drawing to buffer		*/
	MGL_HGLRC	glrc;		/* OpenGL rendering context					*/
	MGL_HBITMAP	img;		/* Handle to pixmap for DC					*/
	} memdc_vars;

typedef struct {
	void		*SGICtx;	/* SGI OpenGL context						*/
	void		*SGISurf;	/* SGI OpenGL surface						*/
	MGL_HDC		gc;		    /* Currently active DC for all output		*/
	MGL_HGLRC	glrc;		/* OpenGL rendering context					*/
	Display     *dpy;       /* X11 display handle (DGA)                 */
	Screen      *scr;       /* X11 default screen (DGA)                 */
	int         dga_pages;  /* Number of DGA pages 						*/
	//	XDGA_page   *pages;     /* Pages data   							*/
	void        *pages;

	uint        depth;      /* Depth of the display (8, 16, 32)         */
    } fulldc_vars;

typedef struct {
	void		*SGICtx;	/* SGI OpenGL context						*/
	void		*SGISurf;	/* SGI OpenGL surface						*/
	MGL_HDC		gc;		    /* Currently active DC for all output		*/
	MGL_HGLRC	glrc;		/* OpenGL rendering context					*/
	Display     *dpy;       /* X11 display handle                       */
	MGL_HWND	wnd;		/* Handle to associated window mgr window	*/
	MGL_HPALETTE hpal;		/* Currently active palette for WinDC		*/
	Screen      *scr;       /* X11 default screen                       */
	uint        depth;      /* Depth of the display (8, 16, 32)         */
	ibool		palNoStatic;/* True when in SYSPAL_NOSTATIC mode		*/
	ibool		isBackground;/* True when window is in background		*/
	} xwindc_vars;

typedef union {
	gendc_vars	gendc;		/* Generic DC variables						*/
	fulldc_vars	fulldc;		/* Fullscreen DC specific variables			*/
	xwindc_vars	xwindc;		/* X-Window DC specific variables			*/
	memdc_vars	memdc;		/* Memory DC specific variables				*/
	} wm_vars;

#elif defined(__PHOTON__)
typedef union {
	void		*SGICtx;	/* SGI OpenGL context						*/
	void		*SGISurf;	/* SGI OpenGL surface						*/
	void		*window;	/* Ptr to window widget */
	unsigned	bitfmt;
	} windc_vars;

typedef union {
	gendc_vars	gendc;		/* Generic DC variables						*/
	windc_vars	windc;		/* Photon DC specific variables				*/
	} wm_vars;
#else
typedef union {
	gendc_vars	gendc;		/* Generic DC variables						*/
	} wm_vars;
#endif

/* Structure for 32 bit fixed point rectangles */

typedef struct {
	fix32_t     left,top,right,bottom;
	} rectFX;

/* Vector font vector structure. The first bit is the opcode, and the
 * last 7 bits are the signed movement commands. We put the movement
 * commands in the last 7 bits, so that we can quickly convert it to
 * a signed integer by doing a simple signed shift right of 9 bits :-)
 */

typedef	ushort	vector;

/* Macros to access field of a vector font structure */

#define VEC_OP1(v)				((v) & 0x0001)
#define	VEC_DX(v)				(int)(((signed char)(v)) >> 1)
#define VEC_OP2(v)				((v) & 0x0100)
#define	VEC_DY(v)				(int)(((signed short)(v)) >> 9)
#define REPLACE_VEC(v,dx,dy)	(((v) & 0x0101) | (((dx) & 0x7F) << 1) | (((dy) & 0x7F) << 9))

/* Flags stored in the isStereo member */

#define	_MGL_NO_STEREO			0x00	/* No stereo mode enabled				*/
#define	_MGL_SOFT_STEREO		0x01	/* Software stereo mode					*/
#define	_MGL_HW_STEREO			0x02	/* Hardware stereo page flipping		*/
#define	_MGL_HW_STEREO_DUAL		0x04	/* Hardware stereo with dual disp start */
#define	_MGL_HW_STEREO_SYNC		0x08	/* Hardware stereo sync					*/
#define	_MGL_STEREO_BLUECODE	0x10	/* Using blue code system				*/
#define	_MGL_STEREO_H3D			0x20	/* Using H3D compatible sync frames		*/

/* Structure holding the display device specific vectors and the display
 * device internal state. The internal state block is contained in extra
 * memory allocated at the end of this block when the device driver
 * creates it.
 */

typedef struct {
	void	(MGLAPIP destroy)(MGLDC *dc);
	int		refCount;		/* Reference count of drivers using this	*/
	ulong	hwnd;			/* Handle to associated window mgr window	*/
	int		device;			/* Device index for this display device		*/
	int		maxProgram;		/* Number of palette entries to program		*/
	int		widePalette;	/* Do we have an 8 bit wide palette?		*/
	int		hardwareCursor;	/* True if device has hardware cursor		*/
	int		isStereo;		/* Stereo enabled flag						*/
	int		stereoRunning;	/* True when stereo is running				*/
	int		refreshRate;	/* Refresh rate selected for display mode	*/
	ibool	useLinearBlits;	/* True if allowing linear blits			*/
	int		saveMode;		/* Mode number used to save display memory	*/
	char	saveMemName[256];/* Name of file used to memory				*/
	ibool	inited;			/* Flag if internal structures inited		*/

	void    (MGLAPIP restoreTextMode)(MGLDC *dc);
	ibool	(MGLAPIP restoreGraphMode)(MGLDC *dc);
	void    (MGLAPIP setActivePage)(MGLDC *dc,int page);
	void    (MGLAPIP setVisualPage)(MGLDC *dc,int page,int waitVRT);
	void    (MGLAPIP vSync)(MGLDC *dc);
	int		(MGLAPIP isVSync)(MGLDC *dc);
	void	(MGLAPIP setDisplayStart)(MGLDC *dc,int x,int y,ibool waitVRT);
	void	(MGLAPIP startStereo)(MGLDC *dc);
	void	(MGLAPIP stopStereo)(MGLDC *dc);
	int		(MGLAPIP getCurrentScanLine)(MGLDC *dc);
	void    (MGLAPIP setCursor)(MGLDC *dc,cursor_t *curs);
	void	(MGLAPIP setCursorColor)(MGLDC *dc,color_t cursorColor);
	void    (MGLAPIP setColorCursor)(MGLDC *dc,cursor_t *curs);
	void    (MGLAPIP setCursorPos)(MGLDC *dc,int x,int y);
	void    (MGLAPIP showCursor)(MGLDC *dc,ibool visible);
	void	(MGLAPIP beginAccess)(MGLDC *dc,N_int32 left,N_int32 top,N_int32 right,N_int32 bottom);
	void	(MGLAPIP endAccess)(MGLDC *dc);
	ibool	(MGLAPIP setActiveDevice)(N_int32 device);
	ibool	(MGLAPIP createOffscreenBuffer)(MGLBUF *buf);
	void	(MGLAPIP destroyOffscreenBuffer)(MGLBUF *buf);
	} display_vec;

/* Structure holding the windowed device specific vectors and internal
 * state.
 */

typedef struct {
	void	(MGLAPIP destroy)(MGLDC *dc);
	int		refCount;
	ibool	(MGLAPIP createOffscreenBuffer)(MGLBUF *buf);
	void	(MGLAPIP destroyOffscreenBuffer)(MGLBUF *buf);
	} windowed_vec;

/* Structure holding the memory device specific vectors and state 		*/

typedef struct {
	void			(MGLAPIP destroy)(MGLDC *dc);
	int				refCount;
	REF2D_driver	*ref2d;
	PE_MODULE 		*hModRef2d;
	ulong			ref2dSize;
	} mem_vec;

/* Structure holding the overlay device specific vectors and internal
 * state.
 */

typedef struct {
	void	(MGLAPIP destroy)(MGLDC *dc);
	int		refCount;
	} overlay_vec;

/* Union for holding all of the device specific vectors for the device
 * driver. This will be created and filled in by the device driver when
 * it is initialised (the deviceType flag is used to determine which
 * block to access).
 */

typedef union {
	display_vec		d;		/* Display device state & vectors			*/
	windowed_vec	w;		/* Windowed device state & vectors			*/
	mem_vec			m;		/* Memory device state & vectors			*/
	overlay_vec		o;		/* Overlay device state & vectors			*/
	} drv_vec;

/* Structure for all 2D rendering functions. We will have separate sets
 * of functions depending on the type of rendering operation, and we
 * can quickly switch between modes by copying these structures.
 */

typedef struct {
	/* SciTech Nucleus compatible low level functions */
	void	(MGLAPIP DrawScanList)(N_int32 y,N_int32 length,N_int16 *scans);
	void	(MGLAPIP DrawEllipseList)(N_int32 y,N_int32 length,N_int32 height,N_int16 *scans);
	void	(MGLAPIP DrawFatEllipseList)(N_int32 y,N_int32 length,N_int32 height,N_int16 *scans);
	void	(MGLAPIP DrawRect)(N_int32 left,N_int32 top,N_int32 width,N_int32 height);
	void	(MGLAPIP DrawTrap)(GA_trap *trap);
	} rVecs;

/* Structure containing all rendering vectors */

typedef struct {
	/* Hardware gamma correction functions */
	void	(MGLAPIP setGammaRamp)(MGLDC *dc,palette_ext_t *pal,int num,int index,int waitVRT);
	void	(MGLAPIP getGammaRamp)(MGLDC *dc,palette_ext_t *pal,int num,int index);

	/* 2D state management vectors */
	void	(MGLAPIP makeCurrent)(MGLDC *dc,ibool partial);
	ulong	(MGLAPIP getWinDC)(MGLDC *dc);
	void *	(MGLAPIP getDefaultPalette)(MGLDC *dc);
	void    (MGLAPIP realizePalette)(MGLDC *dc,palette_t *pal,int num,int index,int waitVRT);
	void    (MGLAPIP realizePaletteExt)(MGLDC *dc,palette_ext_t *pal,int num,int index,int waitVRT);
	void    (MGLAPIP setClipRect)(MGLDC *dc,int left,int top,int right,int bottom);

	/* Nucleus compatible 2D state management vectors */
	void	(MGLAPIP SetDrawSurface)(void *surface,N_int32 xRes,N_int32 yRes,N_int32 bytesPerLine,N_int32 bitsPerPixel,GA_pixelFormat *pf);
	N_int32 (MGLAPIP SetDrawBuffer)(GA_buffer *drawBuf);
	N_int32	(MGLAPIP SetMix)(N_int32 mix);
	void	(MGLAPIP SetForeColor)(GA_color color);
	void	(MGLAPIP SetBackColor)(GA_color color);
	void	(MGLAPIP Set8x8MonoPattern)(N_int32 index,GA_pattern *pattern);
	void	(MGLAPIP Use8x8MonoPattern)(N_int32 index);
	void	(MGLAPIP Use8x8TransMonoPattern)(N_int32 index);
	void	(MGLAPIP Set8x8ColorPattern)(N_int32 index,GA_colorPattern *pattern);
	void	(MGLAPIP Use8x8ColorPattern)(N_int32 index);
	void	(MGLAPIP Use8x8TransColorPattern)(N_int32 index,GA_color transparent);
	void	(MGLAPIP SetLineStipple)(GA_stipple stipple);
	void	(MGLAPIP SetLineStippleCount)(N_uint32 count);
	void	(MGLAPIP SetPlaneMask)(N_uint32 mask);
	void	(MGLAPIP SetSrcBlendFunc)(N_int32 blendFunc);
	void	(MGLAPIP SetDstBlendFunc)(N_int32 blendFunc);
	void	(MGLAPIP SetAlphaValue)(N_uint8 alpha);

	/* Internal rendering vectors */
	void	(MGLAPIP lockBuffer)(MGLBUF *buf);
	void	(MGLAPIP restoreBuffer)(MGLBUF *buf);
	void	(MGLAPIP unlockBuffer)(MGLBUF *buf);
	void    (MGLAPIP beginDirectAccess)(void);
	void    (MGLAPIP endDirectAccess)(void);
	void	(MGLAPIP BitBltDC)(MGLDC *src,MGLDC *dst,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 mix);
	void	(MGLAPIP SrcTransBltDC)(MGLDC *src,MGLDC *dst,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 mix,GA_color transparent);
	void	(MGLAPIP DstTransBltDC)(MGLDC *src,MGLDC *dst,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 mix,GA_color transparent);
	void	(MGLAPIP StretchBltDC)(MGLDC *src,MGLDC *dst,N_int32 srcLeft,N_int32 srcTop,N_int32 srcWidth,N_int32 srcHeight,N_int32 dstLeft,N_int32 dstTop,N_int32 dstWidth,N_int32 dstHeight,N_int32 doClip,N_int32 clipLeft,N_int32 clipTop,N_int32 clipRight,N_int32 clipBottom,N_int32 mix);
	void	(MGLAPIP BltBuffer)(MGLBUF *src,MGLDC *dst,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 mix);
	void	(MGLAPIP SrcTransBltBuffer)(MGLBUF *src,MGLDC *dst,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 mix,GA_color transparent);
	void	(MGLAPIP DstTransBltBuffer)(MGLBUF *src,MGLDC *dst,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 mix,GA_color transparent);
	void	(MGLAPIP StretchBuffer)(MGLBUF *src,MGLDC *dst,N_int32 srcLeft,N_int32 srcTop,N_int32 srcWidth,N_int32 srcHeight,N_int32 dstLeft,N_int32 dstTop,N_int32 dstWidth,N_int32 dstHeight,N_int32 doClip,N_int32 clipLeft,N_int32 clipTop,N_int32 clipRight,N_int32 clipBottom,N_int32 mix);

	/* SciTech Nucleus compatible rendering vectors */
	GA_color (MGLAPIP GetPixel)(N_int32 x,N_int32 y);
	void    (MGLAPIP PutPixel)(N_int32 x,N_int32 y);
	void	(MGLAPIP DrawRectLin)(N_int32 dstOfs,N_int32 dstPitch,N_int32 left,N_int32 top,N_int32 width,N_int32 height);
	void	(MGLAPIP DrawLineInt)(N_int32 x1,N_int32 y1,N_int32 x2,N_int32 y2,N_int32 drawLast);
	void	(MGLAPIP DrawBresenhamLine)(N_int32 x1,N_int32 y1,N_int32 initialError,N_int32 majorInc,N_int32 diagInc,N_int32 count,N_int32 flags);
	void	(MGLAPIP DrawStippleLineInt)(N_int32 x1,N_int32 y1,N_int32 x2,N_int32 y2,N_int32 drawLast,N_int32 transparent);
	void	(MGLAPIP DrawBresenhamStippleLine)(N_int32 x1,N_int32 y1,N_int32 initialError,N_int32 majorInc,N_int32 diagInc,N_int32 count,N_int32 flags,N_int32 transparent);
	void	(MGLAPIP DrawEllipse)(N_int32 left,N_int32 top,N_int32 A,N_int32 B);
	void	(MGLAPIP ClipEllipse)(N_int32 left,N_int32 top,N_int32 A,N_int32 B,N_int32 clipLeft,N_int32 clipTop,N_int32 clipRight,N_int32 clipBottom);
	void	(MGLAPIP PutMonoImage)(N_int32 x,N_int32 y,N_int32 width,N_int32 height,N_int32 byteWidth,N_uint8 *image,N_int32 transparent);
	void	(MGLAPIP ClipMonoImage)(N_int32 x,N_int32 y,N_int32 width,N_int32 height,N_int32 byteWidth,N_uint8 *image,N_int32 transparent,N_int32 clipLeft,N_int32 clipTop,N_int32 clipRight,N_int32 clipBottom);
	void	(MGLAPIP BitBlt)(N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 mix);
	void	(MGLAPIP BitBltLin)(N_int32 srcOfs,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 mix);
	void	(MGLAPIP BitBltSys)(void *srcAddr,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 mix,N_int32 flipY);
	void	(MGLAPIP SrcTransBlt)(N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 mix,GA_color transparent);
	void	(MGLAPIP SrcTransBltLin)(N_int32 srcOfs,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 mix,GA_color transparent);
	void	(MGLAPIP SrcTransBltSys)(void *srcAddr,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 mix,GA_color transparent,N_int32 flipY);
	void	(MGLAPIP DstTransBlt)(N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 mix,GA_color transparent);
	void	(MGLAPIP DstTransBltLin)(N_int32 srcOfs,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 mix,GA_color transparent);
	void	(MGLAPIP DstTransBltSys)(void *srcAddr,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 mix,GA_color transparent,N_int32 flipY);
	void	(MGLAPIP StretchBlt)(N_int32 srcLeft,N_int32 srcTop,N_int32 srcWidth,N_int32 srcHeight,N_int32 dstLeft,N_int32 dstTop,N_int32 dstWidth,N_int32 dstHeight,N_int32 doClip,N_int32 clipLeft,N_int32 clipTop,N_int32 clipRight,N_int32 clipBottom,N_int32 mix);
	void	(MGLAPIP StretchBltLin)(N_int32 srcOfs,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 srcWidth,N_int32 srcHeight,N_int32 dstLeft,N_int32 dstTop,N_int32 dstWidth,N_int32 dstHeight,N_int32 doClip,N_int32 clipLeft,N_int32 clipTop,N_int32 clipRight,N_int32 clipBottom,N_int32 mix);
	void	(MGLAPIP StretchBltSys)(void *srcAddr,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 srcWidth,N_int32 srcHeight,N_int32 dstLeft,N_int32 dstTop,N_int32 dstWidth,N_int32 dstHeight,N_int32 doClip,N_int32 clipLeft,N_int32 clipTop,N_int32 clipRight,N_int32 clipBottom,N_int32 mix,N_int32 flipY);
	N_int32 (MGLAPIP ConvertBltSys)(void *srcAddr,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 bitsPerPixel,GA_pixelFormat *pixelFormat,GA_palette *dstPal,GA_palette *srcPal,N_int32 dither,N_int32 mix,N_int32 flipY);
	N_int32 (MGLAPIP StretchConvertBltSys)(void *srcAddr,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 srcWidth,N_int32 srcHeight,N_int32 dstLeft,N_int32 dstTop,N_int32 dstWidth,N_int32 dstHeight,N_int32 doClip,N_int32 clipLeft,N_int32 clipTop,N_int32 clipRight,N_int32 clipBottom,N_int32 bitsPerPixel,GA_pixelFormat *pixelFormat,GA_palette *dstPal,GA_palette *srcPal,N_int32 dither,N_int32 mix,N_int32 flipY);
	N_int32	(MGLAPIP BitBltFxTest)(GA_bltFx *fx);
	N_int32 (MGLAPIP BitBltFx)(N_int32 srcLeft,N_int32 srcTop,N_int32 srcWidth,N_int32 srcHeight,N_int32 dstLeft,N_int32 dstTop,N_int32 dstWidth,N_int32 dstHeight,GA_bltFx *fx);
	N_int32 (MGLAPIP BitBltFxLin)(N_int32 srcOfs,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 srcWidth,N_int32 srcHeight,N_int32 dstLeft,N_int32 dstTop,N_int32 dstWidth,N_int32 dstHeight,GA_bltFx *fx);
	N_int32 (MGLAPIP BitBltFxSys)(void *srcAddr,N_int32 srcPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 srcWidth,N_int32 srcHeight,N_int32 dstLeft,N_int32 dstTop,N_int32 dstWidth,N_int32 dstHeight,GA_bltFx *fx);
	void	(MGLAPIP GetBitmapSys)(void *dstAddr,N_int32 dstPitch,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop);

	rVecs	cur;			/* Current rendering vectors 				*/
	rVecs   solid;			/* Solid 1x1 pen vectors in replace ROP		*/
	rVecs   patt;			/* Pattern filled vectors (all ROP's)		*/
	rVecs	colorPatt;		/* Color pattern filled vectors (all ROP's)	*/
	} vecs;

/* Mesa 3D internal structures */

#ifndef	TYPES_H
typedef struct gl_visual GLvisual;
#endif
typedef struct MGLMesaContext MGLRC;

/* Device Context structure */

struct internalDevCtx_t {
	/* Public device context structure members */
	attributes_t 	a;				/* Currently active device attributes		*/
	void			*surface;		/* Pointer to device active surface 		*/
	gmode_t			mi;				/* Mode specific information block			*/
	pixel_format_t 	pf;				/* Current pixel format for device context	*/
	color_t			*colorTab;		/* Color lookup table cache					*/

	/* Resources use by the device driver */
	rect_t			size;			/* Restricted dimensions of DC				*/
	long			flags;			/* Flags for the DC							*/
	drv_vec			*v;				/* Driver type specific vectors	and state	*/

	/* The following are device specific for PC based display drivers,
	 * but appear in the main device context for speedy access (rather
	 * than requiring a double indirection if stored in the drv_vec
	 * structure for every graphics operation).
	 */
	char			*surfaceStart;	/* Pointer to start of display surface		*/
	ulong			originOffset;	/* Offset to start of display surface		*/
	int				numBuffers;		/* Number of buffers requested for mode		*/
	int				activePage;		/* Currently active video page				*/
	int				virtualX;		/* Virtual device X coordinate				*/
	int				virtualY;		/* Virtual device Y coordinate				*/
	int				startX;			/* Current display start address			*/
	int				startY;			/* Current display start address			*/
	int				activeStartY;	/* Start of current display page			*/
	int				offscreenStartY;/* Start of offscreen memory 				*/
	int				offscreenY;		/* Current offscreen starting line			*/
	int				visualPage;		/* Current visual page						*/
	int				frontBuffer;	/* Front buffer for OpenGL					*/
	int				backBuffer;		/* Back buffer for OpenGL					*/
	int				glDrawBuffer;	/* Current glDrawBuffer value				*/
	ulong			CRTCBase;		/* CRTC display start address base			*/
	int				currentLock;	/* Count for number of times make current	*/
	wm_vars			wm;				/* Window manager specific variables		*/
	ibool			ownMemory;		/* True if we own the allocated memory		*/
	GLvisual 		*visual;		/* Mesa OpenGL visual						*/
	MGLRC			*rc;			/* Mesa OpenGL rendering context			*/
	MGLVisual 		cntVis;			/* Current MGL OpenGL Visual				*/
	MGLDC			*memdc;			/* Memory DC OpenGL back buffer				*/
	MGLBUF			*offBuf;		/* Offscreen buffer for offscreen DC's		*/

	/* Internal device driver variables */
	GA_trap			tr;				/* Current trapezoid parameters				*/
	rectFX			clipRectFX;		/* Fixed point clipping rectangle			*/
	rectFX			viewPortFX;		/* Fixed point viewport rectangle			*/
	rect_t			intClipRect;	/* Screen space clipping rectangle			*/
	rectFX			intClipRectFX;	/* Fixed point internal clipping rectangle	*/
	region_t 		*intClipRegion;	/* Screen space clip region					*/
	int				doClip;			/* Should clipping be performed?			*/
	arc_coords_t 	ac;				/* Coordinates of last arc operation		*/

	/* Stuff loaded by device driver at initialisation time */
	int				deviceType;		/* Device driver type						*/
	int				deviceIndex;	/* Device index for the device in system	*/
	int				xInch;			/* Device X size in inches*1000				*/
	int				yInch;			/* Device Y size in inches*1000				*/

    /* Place to store the OpenGL rendering state for 2D drawing functions */
	ibool			mglEnabled;
	int 			glViewport[4];
	double			glCntColor[4];
	int 			glSwapBytes;
	int 			glLsbFirst;
	int 			glRowLength;
	int 			glSkipRows;
	int 			glSkipPixels;
	int 			glAlignment;
	ibool			glLighting;
	ibool			glDepthTest;

	vecs			r;				/* Rendering vectors 						*/
	};

/* Define a macro to easily access global device context */

#define	DC			_MGL_dc

/*--------------------------- Global Variables ----------------------------*/

extern MGLDC		_MGL_dc;	/* Global device context block		*/

#pragma pack()

#endif	/* __MGL_INT_H */
