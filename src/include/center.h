/****************************************************************************
*
*				   Display Doctor Windows Interface Code
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
* Environment:	IBM PC (MS DOS)
*
* Description:	Header file for centering windows.
*
****************************************************************************/

#ifndef	__CENTER_H
#define	__CENTER_H

#define	STRICT
#define	WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "scitech.h"

/*------------------------- Function Prototypes ---------------------------*/

#ifdef	__cplusplus
extern "C" {			/* Use "C" linkage when in C++ mode	*/
#endif

void _EXPORT CenterWindow(HWND hWndCenter, HWND parent, BOOL repaint);
void _EXPORT CenterLogo(HWND hWndLogo, HWND hWndParent, int CenterY);

#ifdef	__cplusplus
}						/* End of "C" linkage for C++	*/
#endif

#pragma pack()

#endif	/* __CENTER_H */

