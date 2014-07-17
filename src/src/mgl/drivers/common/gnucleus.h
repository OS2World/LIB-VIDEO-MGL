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
* Description:	Common header file for Graphics Accelerator routines used
*				by all SciTech Nucleus accelerated device drivers.
*
****************************************************************************/

#ifndef	__DRIVERS_COMMON_GNUCLEUS_H
#define	__DRIVERS_COMMON_GNUCLEUS_H

#include "drivers/common/common.h"

/*---------------------- Macro and type definitions -----------------------*/

#pragma pack(1)				/* Pack structures to byte granularity		*/

/* The following defines a structure used to cache all necessary mode
 * information after the initial detection for a device, so that
 * it can be re-used efficiently to determine the capabilities of all
 * available display modes for all available display device drivers. The
 * information in here is a subset of that returnd in the GA_modeInfo
 * structure to save space.
 */

typedef struct {
	short			mode;
	short   		XResolution;
	short   		YResolution;
	uchar   		BitsPerPixel;
	uchar			HaveRefresh;
	ulong			Attributes;
	short   		BytesPerScanLine;
	GA_pixelFormat	PixelFormat;
	N_uint16   		MaxBuffers;
	N_uint32   		BitmapStartAlign;
	N_uint32   		BitmapStridePad;
	} GA_mdEntry;

/* The following defines the data structure used to maintain the
 * information about the display device after the Nucleus detection code
 * has been run.
 */

typedef struct {
	short			detected;			/* Driver already detected?			*/
	int				saveMode;			/* Save mode for safe mode			*/
	char			saveMemName[_MAX_PATH];	/* Filename used for safe mode	*/
	ibool			accelerated;		/* True if accelerated				*/
	GA_mdEntry		modeList[MAX_DRIVER_MODES+1];/* List of available modes	*/
	GA_initFuncs	init;				/* Nucleus init functions		*/
	GA_driverFuncs	drv;           		/* Nucleus driver functions		*/
	GA_cursorFuncs	cursor;				/* Nucleus cursor functions		*/
	GA_videoFuncs	video;				/* Nucleus video functions		*/
	void 			(NAPIP WaitTillIdle)(void);
	void 			(NAPIP EnableDirectAccess)(void);
	void 			(NAPIP DisableDirectAccess)(void);
	} NUCL_detectInfo;

#define	SVGA(dc)	(((NUCL_data*)&dc->v->d)->svga)

/* The following defines the internal VGA device driver state buffer. A
 * copy of this is allocated for us in the device context created for
 * the device driver. This state buffer is used to maintain device specific
 * state information that is device context specific.
 */

typedef struct {
	display_vec	v;				/* Display device vectors and info		*/
	N_uint32	oldMode;		/* Old display mode before graphics		*/
	} NUCL_state;

/* The following defines the data structures stored for each Nucleus driver
 * per display device instance data.
 */

typedef struct {
	NUCL_state		state;
	NUCL_detectInfo	svga;
	} NUCL_data;

/* Macro to create a Nucleus draw buffer */

#define	MAKE_GA_BUF(drawBuf,buf)			\
	(drawBuf).dwSize = sizeof((drawBuf));   \
	(drawBuf).Offset = (buf)->linearStart;  \
	(drawBuf).Stride = (buf)->bytesPerLine;	\
	(drawBuf).Width = (buf)->width;         \
	(drawBuf).Height = (buf)->height

#pragma pack()

/*------------------------- Function Prototypes ---------------------------*/

void * 	MGLAPI NUCL_initInstance(void *inst);
ibool	MGLAPI NUCL_detect(NUCL_data *instance,ibool accelerated,int id,int *numModes,modetab availableModes);
ibool 	MGLAPI NUCL_initDriver(NUCL_data *instance,MGLDC *dc,modeent *mode,ulong hwnd,int virtualX,int virtualY,int numBuffers,ibool stereo,int refreshRate,ibool useLinearBlits);
void 	MGLAPI NUCL_restoreTextMode(MGLDC *dc);
ibool 	MGLAPI NUCL_restoreGraphMode(MGLDC *dc);
void 	MGLAPI NUCL_setActivePage(MGLDC *dc,int page);
void 	MGLAPI NUCL_setVisualPage(MGLDC *dc,int page,int waitVRT);
void 	MGLAPI NUCL_vSync(MGLDC *dc);
int 	MGLAPI NUCL_isVSync(MGLDC *dc);
void 	MGLAPI NUCL_setDisplayStart(MGLDC *dc,int x,int y,int waitFlag);
void	MGLAPI NUCL_startStereo(MGLDC *dc);
void	MGLAPI NUCL_stopStereo(MGLDC *dc);
int		MGLAPI NUCL_getCurrentScanLine(MGLDC *dc);
void	MGLAPI NUCL_setCursor(MGLDC *dc,cursor_t *curs);
void    MGLAPI NUCL_setCursorPos(MGLDC *dc,int x,int y);
void    MGLAPI NUCL_showCursor(MGLDC *dc,ibool visible);
void 	MGLAPI NUCL_setCursorColor(MGLDC *dc,color_t cursorColor);
void 	MGLAPI NUCL_makeCurrent(MGLDC *dc,ibool partial);
void	MGLAPI NUCL_setGammaRamp(MGLDC *dc,palette_ext_t *pal,int num,int index,int waitVRT);
void	MGLAPI NUCL_getGammaRamp(MGLDC *dc,palette_ext_t *pal,int num,int index);
void 	MGLAPI NUCL_realizePalette(MGLDC *dc,palette_t *pal,int num,int index,int waitVRT);
void 	MGLAPI NUCL_realizePaletteExt(MGLDC *dc,palette_ext_t *pal,int num,int index,int waitVRT);
ibool	MGLAPI NUCL_createOffscreenBuffer(MGLBUF *buf);
void	MGLAPI NUCL_destroyOffscreenBuffer(MGLBUF *buf);
void	MGLAPI NUCL_restoreBuffer(MGLBUF *buf);
void	MGLAPI NUCL_lockBuffer(MGLBUF *buf);
void	MGLAPI NUCL_unlockBuffer(MGLBUF *buf);
void	MGLAPI NUCL_lockBufferSLOW(MGLBUF *buf);
void	MGLAPI NUCL_unlockBufferSLOW(MGLBUF *buf);

/* Hardware device driver support routines */

void	MGLAPI NUCL_beginDirectAccess(void);
void	MGLAPI NUCL_beginDirectAccessSLOW(void);
void	MGLAPI NUCL_endDirectAccessSLOW(void);
void	MGLAPI NUCL_BitBltDC(MGLDC *src,MGLDC *dst,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 mix);
void	MGLAPI NUCL_SrcTransBltDC(MGLDC *src,MGLDC *dst,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 mix,GA_color transparent);
void	MGLAPI NUCL_DstTransBltDC(MGLDC *src,MGLDC *dst,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 mix,GA_color transparent);
void	MGLAPI NUCL_StretchBltDC(MGLDC *src,MGLDC *dst,N_int32 srcLeft,N_int32 srcTop,N_int32 srcWidth,N_int32 srcHeight,N_int32 dstLeft,N_int32 dstTop,N_int32 dstWidth,N_int32 dstHeight,N_int32 doClip,N_int32 clipLeft,N_int32 clipTop,N_int32 clipRight,N_int32 clipBottom,N_int32 mix);
void	MGLAPI NUCL_BltBuffer(MGLBUF *src,MGLDC *dst,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 mix);
void	MGLAPI NUCL_SrcTransBltBuffer(MGLBUF *src,MGLDC *dst,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 mix,GA_color transparent);
void	MGLAPI NUCL_DstTransBltBuffer(MGLBUF *src,MGLDC *dst,N_int32 srcLeft,N_int32 srcTop,N_int32 width,N_int32 height,N_int32 dstLeft,N_int32 dstTop,N_int32 mix,GA_color transparent);
void	MGLAPI NUCL_StretchBuffer(MGLBUF *src,MGLDC *dst,N_int32 srcLeft,N_int32 srcTop,N_int32 srcWidth,N_int32 srcHeight,N_int32 dstLeft,N_int32 dstTop,N_int32 dstWidth,N_int32 dstHeight,N_int32 doClip,N_int32 clipLeft,N_int32 clipTop,N_int32 clipRight,N_int32 clipBottom,N_int32 mix);

#endif	/* __DRIVERS_COMMON_GNUCLEUS_H */
