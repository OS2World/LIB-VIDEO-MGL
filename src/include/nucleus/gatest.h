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
*  |This copyrighted computer code is a proprietary trade secret of     |
*  |SciTech Software, Inc., located at 505 Wall Street, Chico, CA 95928 |
*  |USA (www.scitechsoft.com).  ANY UNAUTHORIZED POSSESSION, USE,       |
*  |VIEWING, COPYING, MODIFICATION OR DISSEMINATION OF THIS CODE IS     |
*  |STRICTLY PROHIBITED BY LAW.  Unless you have current, express       |
*  |written authorization from SciTech to possess or use this code, you |
*  |may be subject to civil and/or criminal penalties.                  |
*  |                                                                    |
*  |If you received this code in error or you would like to report      |
*  |improper use, please immediately contact SciTech Software, Inc. at  |
*  |530-894-8400.                                                       |
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

#ifndef	__GATEST_H
#define	__GATEST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "nucleus/gasdk.h"
#include "gtf.h"
#include "ztimer.h"
#include "event.h"
#include "console.h"

/*---------------------------- Global Variables ---------------------------*/

#ifdef  __cplusplus
extern "C" {            /* Use "C" linkage when in C++ mode */
#endif

/* Global variables in gatest.c */

extern int 				cntDevice,numDevices;
extern ibool 			softwareEmulate,useGTF,fullSpeed,breakPoint;
extern char				accelTestName[];
extern GA_initFuncs		init;
extern GA_driverFuncs	driver;

typedef ibool 	(*doTestType)(GA_devCtx *dc,int xRes,int yRes,int bitsPerPixel,N_uint32 flags,int refreshRate,GA_CRTCInfo *crtc,N_uint32 planeMask);

/*------------------------- Function Prototypes ---------------------------*/

/* General utility functions in util.c */

int 	SetGraphicsMode(GA_devCtx *dc);
void 	RestoreMode(GA_devCtx *dc,int oldMode);
int 	GetModeName(char *buf,GA_modeInfo *mi);
void 	InitMouse(int xRes,int yRes);
void 	GetMousePos(int *x,int *y);
int 	EVT_kbhit(void);
int 	EVT_getch(void);
int 	GetString(char *str,int maxLen);

/* DPMS tests in dpmstest.c */

void 	DPMSTests(GA_devCtx *dc);

/* DDC tests in ddctest.c */

void 	DDCTests(GA_devCtx *dc);

/* DDC/CI tests in ddccitst.c */

void 	DDCCITests(GA_devCtx *dc);

/* Text mode tests in txttest.c */

ibool 	doTextTest(GA_devCtx *dc,int xRes,int yRes,int bitsPerPixel,N_uint32 flags,int refreshRate,GA_CRTCInfo *crtc,N_uint32 planeMask);

/* Centering and refresh control in center.c */

ibool 	doCenter(GA_devCtx *dc,int xRes,int yRes,int bitsPerPixel,N_uint32 flags,int refreshRate,GA_CRTCInfo *crtc,N_uint32 planeMask);

/* TV centering and refresh control in tv.c */

ibool 	CenterTVMode(GA_devCtx *_dc,N_uint32 mode);

/* Visual tests in tests.c */

void	moire(GA_color defcolor);
int 	moireTest(void);
int 	pageFlipTest(void);
int 	paletteTest(void);
int 	wideDACTest(void);
ibool 	doTest(GA_devCtx *dc,int xRes,int yRes,int bitsPerPixel,N_uint32 flags,int refreshRate,GA_CRTCInfo *crtc,N_uint32 planeMask);
ibool 	doScrollTest(GA_devCtx *dc,int xRes,int yRes,int bitsPerPixel,N_uint32 flags,int refreshRate,GA_CRTCInfo *crtc,N_uint32 planeMask);

/* Acceleration tests in conform/2dtests.c */

int 	doAccelTests(const char *testName,ibool useSmallBuffer);
void 	doCursorInit(void);
int 	doCursorTest(const char *testName);

/* Stereo display mode test in tests.c */

ibool 	doStereoTest(GA_devCtx *dc,int xRes,int yRes,int bitsPerPixel,N_uint32 flags,int refreshRate,GA_CRTCInfo *crtc,N_uint32 planeMask);

/* Win16 thunks for 32-bit DOS test harness */

void	NAPI GA_InitThunks(GA_devCtx *dc,GA_win16Funcs *win16);
void	NAPI GA_WaitTillIdleW16(void);
void	NAPI GA_EnableDirectAccessW16(void);
void	NAPI GA_DisableDirectAccessW16(void);
void	NAPI GA_SetCursorPosW16(N_int32 x,N_int32 y);
void	NAPI GA_ExitThunks(void);

/* Function to build the Win16 functions for the driver */

void 	GA_MakeWin16(GA_devCtx *dc);

#ifdef  __cplusplus
}                       /* End of "C" linkage for C++   */
#endif

#endif	/* __GATEST_H */
