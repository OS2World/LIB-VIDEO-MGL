/****************************************************************************
*
*                           SciTech Sound Doctor
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
* Environment:	MSDOS, Win16, Win32
*
* Description:	Include the correct version number information for
*				the OS we are building for.
*
****************************************************************************/

#if		defined(__LINUX__)
#include "ssd/linux/ver.h"
#elif	defined(__BEOS__)
#include "ssd/beos/ver.h"
#elif	defined(__OS2__)
#include "ssd/os2/ver.h"
#elif	defined(__QNX__)
#include "ssd/qnx/ver.h"
#elif	defined(__WINDOWSNT__)
#include "ssd/winnt/ver.h"
#else
#include "ssd/win9x/ver.h"
#endif
