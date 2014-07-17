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
* Environment:	MSDOS, Win16, Win32
*
* Description:	Version number information for SciTech Display Doctor.
*
****************************************************************************/

#ifndef  NO_BUILD
#include "sdd/sddbld.h"
#endif
#include "sdd/sddver.h"

/* Release numbers for this release */

#ifndef	NO_COPYRIGHT
static char *copyright_str =
"Copyright (C) 1993-1999 SciTech Software, Inc. All Rights Reserved.";
#endif

static char *release_major = SDD_RELEASE_MAJOR_STR;
#if defined(BETA)
static char *release_minor = SDD_RELEASE_MINOR_STR " beta";
#else
static char *release_minor = SDD_RELEASE_MINOR_STR;
#endif
#ifndef NO_BUILD
#ifndef	NO_RELEASE_DATE
static char *release_date = __DATE__ " - Build " BUILD_NUMBER;
#ifndef	NO_RELEASE_DATE2
static char *release_date2 = __DATE__;
#endif
#endif
#else
#ifndef	NO_RELEASE_DATE
static char *release_date = __DATE__;
#ifndef	NO_RELEASE_DATE2
static char *release_date2 = __DATE__;
#endif
#endif
#endif
