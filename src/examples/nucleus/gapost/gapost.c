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
* Description:	Simple program to cause all secondary adapters to be
*				warm booted and initialised by their respective BIOS'es.
*
*				Under QNX this requires linking with the VBIOS library
*				which is somewhat large under QNX4.  By executing this
*				program at boot time, other apps do not need to be linked
*				with the VBIOS library.
*
****************************************************************************/

#include "nucleus/gatest.h"

/*---------------------------- Global Variables ---------------------------*/

#ifdef ISV_LICENSE
#include "isv.c"
#endif

/*----------------------------- Implementation ----------------------------*/

int main(int argc,char *argv[])
{
	/* Register the ISV license file if desired */
#ifdef	ISV_LICENSE
	GA_registerLicense(OemLicense,false);
#endif

	/* All we need to do is enumerate all the devices which automatically
	 * POST's all the secondary controllers. Loading a device driver for
	 * each device is not necessary.
	 */
	GA_enumerateDevices(false);
	return 0;
}
