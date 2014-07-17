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
* Environment:	Unix/X11
*
* Description:	Header file for the 8-bit X11 driver.
*
****************************************************************************/

#ifndef	__DRIVERS_UNIX_XDGA_H
#define	__DRIVERS_UNIX_XDGA_H

#include "mglunix.h"
#include "mgldd.h"

#ifndef	__DRIVERS_PACKED_PACKED8_H
#include "drivers/packed/packed8.h"
#endif

#include "drivers/common/gunix.h"

#define MAX_DGA_PAGES	16

typedef struct {
	int   x, y;
	char *addr;
	int   bytes_per_line;
} XDGA_page;

typedef struct {
	Display *dpy;
	Screen  scr;
	int     depth;

	int dga_linewidth;  /* Pixel */
	int dga_banksize;   /* Byte  */
	int dga_memsize;    /* Byte (after init) */
	int dga_width;      /* Pixel */
	int dga_height;     /* Pixel */
	char *dga_addr;
	int dga_flags;
	int dga_pages;     /* Number of DGA pages */
	XDGA_page pages[MAX_DGA_PAGES]; /* Pages data */

	XF86VidModeModeInfo oldmode; /* Initial vid mode before we switch */
} XDGA_data;

/*------------------------- Function Prototypes ---------------------------*/

ibool 	MGLAPI XDGA_detect(void *data,int id,int force,int *driver,int *mode,modetab availableModes);
ibool 	MGLAPI XDGA_initDriver(void *data,MGLDC *dc,int driverId,int modeId,ulong hwnd,int virtualX,int virtualY,int numBuffers,ibool stereo,int refreshRate,ibool useLinearBlits);
void   *MGLAPI XDGA_createInstance(void);
void    MGLAPI XDGA_destroyInstance(void *data);

void    MGLAPI XDGA_realizePalette(MGLDC *dc,palette_t *pal,int num,int index,int waitVRT);
void MGLAPI  XDGA_putImage(MGLDC *dc,int left,int top,int right,
	int bottom,int dstLeft,int dstTop,int op,void *surface,
	int bytesPerLine,MGLDC *src);
void  MGLAPI XDGA_setActivePage(MGLDC *dc, int page);
void  MGLAPI XDGA_setVisualPage(MGLDC *dc, int page, int waitVRT);


#endif /* __DRIVERS_UNIX_XWIN8_H */
