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
* Description:  Module to implement the code to test the Nucleus Graphics
*				Architecture.
*
****************************************************************************/

#ifndef	__NUCLEUS_GASDK_H
#define	__NUCLEUS_GASDK_H

#include "nucleus/graphics.h"
#include "nucleus/ref2d.h"

/*---------------------- Macros and type definitions ----------------------*/

/* Values stored in the isStereo global */

#define	gaNoStereo		0		/* Not running in stereo mode			*/
#define	gaSoftStereo	1		/* Running in software stereo mode		*/
#define	gaHWStereo		2		/* Running in hardware stereo mode		*/

/* Defines for left and right buffers for SetActivePage() */

#define	gaLeftBuffer	0x0000	/* Default is for left buffer			*/
#define	gaRightBuffer	0x8000	/* Signal to make right buffer active	*/

/* Defines for waitVRT flag for SetVisualPage() */

#define	gaTripleBuffer  0		/* Triple buffer						*/
#define	gaWaitVRT		1		/* Wait for vertical retrace			*/
#define gaDontWait		2		/* Don't wait for retrace				*/

/*---------------------------- Global Variables ---------------------------*/

#ifdef  __cplusplus
extern "C" {            /* Use "C" linkage when in C++ mode */
#endif

/* Global variables in gasdk.c */

extern N_uint16			cntMode;
extern N_int32			x,y,maxX,maxY,maxPage,activePage,visualPage,maxMem;
extern N_int32 			pageSize,virtualX,virtualY,bytesPerLine,bytesPerPixel;
extern N_int32			isStereo,stereoRunning,useBlueCode,blueCodeIndex;
extern GA_color			defcolor,maxcolor;
extern GA_modeInfo		modeInfo;
extern REF2D_driver		*ref2d;
extern GA_devCtx		*dc;
extern GA_driverFuncs	driver;
extern GA_2DStateFuncs	drv_state2d,state2d;
extern GA_2DRenderFuncs	drv_draw2d,draw2d;
extern GA_palette 		VGA8_defPal[256];
extern GA_palette 		VGA4_defPal[64];
extern ibool 			softwareOnly;
extern GA_buffer		smallBuf;

/*------------------------- Function Prototypes ---------------------------*/

/* Rendering functions in gasdk.c */

GA_color rgbColor(uchar r,uchar g,uchar b);
GA_color realColor(int index);
void 	SetForeColor(GA_color color);
void 	SetBackColor(GA_color color);
void 	SetMix(N_int32 mix);
void 	Set8x8MonoPattern(N_int32 index,GA_pattern *pattern);
void 	Use8x8MonoPattern(N_int32 index);
void 	Use8x8TransMonoPattern(N_int32 index);
void 	Set8x8ColorPattern(N_int32 index,GA_colorPattern *pattern);
void 	Use8x8ColorPattern(N_int32 index);
void 	Use8x8TransColorPattern(N_int32 index,GA_color transparent);
void 	SetLineStipple(GA_stipple stipple);
void 	SetLineStippleCount(N_uint32 count);
void	SetPlaneMask(N_uint32 planeMask);
void 	SetActivePage(int page);
void 	SetVisualPage(int page,int waitVRT);
ibool	StartStereo(void);
void	StopStereo(void);
void 	ClearPage(GA_color color);
void 	WriteText(int x,int y,char *s,GA_color color);
void 	gmoveto(int _x,int _y);
void 	gnewline(void);
int 	ggetx(void);
int 	ggety(void);
int 	gprintf(char *fmt, ... );
void 	InitSoftwareRasterizer(int deviceIndex,int reservePages);
void 	ExitSoftwareRasterizer(void);
void	ForceSoftwareOnly(void);
void	RevertSoftwareOnly(void);
void	_ASMAPI SetBankDL(void);

#ifdef  __cplusplus
}                       /* End of "C" linkage for C++   */
#endif

#endif	/* __NUCLEUS_GASDK_H */
