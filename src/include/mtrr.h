/****************************************************************************
*
*                         SciTech Display Doctor
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
* Language:		ANSI C
* Environment:	Any
*
* Description:  Include file defining the external ring 0 helper functions
*				needed by the MTRR module. These functions may be included
*				directly for native ring 0 device drivers, or they may
*				be calls down to a ring 0 helper device driver where
*				appropriate (or the entire MTRR module may be located in
*				the device driver if the device driver is 32-bit).
*
****************************************************************************/

#ifndef	__MTRR_H
#define	__MTRR_H

#include "scitech.h"

/*--------------------------- Function Prototypes -------------------------*/

#ifdef	__cplusplus
extern "C" {			/* Use "C" linkage when in C++ mode	*/
#endif

/* Internal functions (requires ring 0 access or helper functions!) */

void 	MTRR_init(void);
int 	MTRR_enableWriteCombine(ulong base,ulong size,uint type);

/* External assembler helper functions */

ibool	_ASMAPI _MTRR_isRing0(void);
ulong	_ASMAPI _MTRR_disableInt(void);
void	_ASMAPI _MTRR_restoreInt(ulong flags);
void	_ASMAPI _MTRR_flushTLB(void);
ulong	_ASMAPI _MTRR_saveCR4(void);
void	_ASMAPI _MTRR_restoreCR4(ulong cr4Val);
uchar	_ASMAPI _MTRR_getCx86(uchar reg);
void	_ASMAPI	_MTRR_setCx86(uchar reg,uchar data);
#ifdef	__16BIT__
void	_ASMAPI _MTRR_readMSR(ulong reg, ulong far *eax, ulong far *edx);
#else
void	_ASMAPI _MTRR_readMSR(ulong reg, ulong *eax, ulong *edx);
#endif
void 	_ASMAPI _MTRR_writeMSR(ulong reg, ulong eax, ulong edx);

#ifdef	__cplusplus
}						/* End of "C" linkage for C++	*/
#endif

#endif  /* __MTRR_H */
