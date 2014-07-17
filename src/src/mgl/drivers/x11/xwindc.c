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
* Description:	Main windowed X11 device driver file. This file
*				will be linked in when a call to MGL_registerDriver() is
*				made to register this device driver for use.
*
****************************************************************************/

#include "mgldd.h"
#include "drivers/unix/xwindc.h"
#include <stdio.h>

/*---------------------------- Driver Tables -----------------------------*/

drivertype	_VARAPI XWINDC_driver = {
    IDENT_FLAG,
    MGL_XWINDCNAME,
    MGL_XWINDCNAME,
    "\r\n\r\nVersion " DRIVER_VERSION_STR " - " __DATE__ "\r\n"
    "Copyright (C) 1998 SciTech Software\r\n",
    DRIVER_MAJOR_VERSION,
    DRIVER_MINOR_VERSION,
    grXWIN,
	XWINDC_createInstance,
    NULL,
    XWINDC_initDriver,
	XWINDC_destroyInstance,
};

PRIVATE drv_vec	g_state;	/* Global state buffer pointer	*/


/*------------------------------ Implementation ------------------------------*/

PRIVATE void MGLAPI destroyDC(MGLDC *dc)
/****************************************************************************
*
* Function:		destroyDC
* Parameters:	dc	- Device context
*
* Description:	Destroys all internal memory allocated by the device
*				context.
*
****************************************************************************/
{
	if (--dc->v->d.refCount == 0) {
		if (dc->v != &g_state)
			MGL_fatalError("Internal error: dc->v != g_state!\n");
		//		MGL_free(dc->v);
		//		g_state = NULL;
		}
	/* If we have a palette associated with this DC, then destroy it */
	if (dc->wm.xwindc.hpal)
		XFreeColormap(dc->wm.xwindc.dpy, dc->wm.xwindc.hpal);
}

void * MGLAPI  XWINDC_createInstance(void)
{
	return MGL_malloc(sizeof(XWINDC_data));
}


void MGLAPI XWINDC_destroyInstance(void *data)
/****************************************************************************
*
* Function:		XWIN8_destroyInstance
*
* Description:	Frees the internal data structures allocated during the
*				detection process
*
****************************************************************************/
{
	if(data){
		//		XDestroyWindow(dpy, ((XWIN8_data*)data)->w);
		MGL_free(data);
	}
}


ibool 	MGLAPI XWINDC_initDriver(void *data,MGLDC *dc,int driverId,int modeId,ulong hwnd,
								 int virtualX,int virtualY,int numBuffers,ibool stereo,int refreshRate)
/****************************************************************************
*
* Function:		XWINDC_initDriver
* Parameters:	dc	- Device context.
* Returns:		True if the device was correctly initialised.
*
* Description:	Initialises the device driver, and starts the specified
*				graphics mode. This is also where we fill in all of the
*				vectors in the device context to initialise our device
*				context properly.
*
****************************************************************************/
{
	Screen *scr;
	XSetWindowAttributes xswa;
	Display *dpy = dc->wm.xwindc.dpy;
	XVisualInfo *visinfo, vis;
	int nvis, npfv;
	Window root;
	int x,y;
	unsigned int w,h, bw, d;
	XPixmapFormatValues *pfv;

	_MGL_initInternal();
	dc->deviceType = MGL_WINDOWED_DEVICE;
	dc->xInch = dc->yInch = 0;

	g_state.d.hardwareCursor = false;

	dc->v = &g_state;
	dc->v->m.refCount++;
	dc->wm.xwindc.scr = scr = DefaultScreenOfDisplay(dpy);
	dc->wm.xwindc.gc  = DefaultGCOfScreen(scr);

	xswa.background_pixel = BlackPixel(dpy,XScreenNumberOfScreen(scr));
	xswa.backing_store = Always;

	dc->wm.xwindc.wnd = hwnd;
	XGetGeometry(dpy, hwnd, &root, &x, &y, &w, &h, &bw, &d);

	dc->mi.xRes = w;
	dc->mi.yRes = h;
	dc->mi.maxPage = 0;
	dc->mi.bytesPerLine = 0;
	dc->mi.pageSize = 0;
	dc->surface = dc->surfaceStart = NULL;
	dc->mi.bitsPerPixel = dc->wm.xwindc.depth = d;

	vis.visualid = XVisualIDFromVisual(DefaultVisual(dpy, XScreenNumberOfScreen(scr)));
	visinfo = XGetVisualInfo(dpy, VisualIDMask, &vis, &nvis);

	pfv = XListPixmapFormats(dpy, &npfv);
	switch (d) {
		case 8:	dc->mi.maxColor = 0xFFUL;		break;
		case 15:	dc->mi.maxColor = 0x7FFFUL;		dc->pf= _MGL_pixelFormats[pfRGB555]; break;
		case 16:	dc->mi.maxColor = 0xFFFFUL;		dc->pf= _MGL_pixelFormats[visinfo->green_mask==0x7e0 ? pfRGB565 : pfRGB555]; break;
	    case 24: {
			int i;
			dc->mi.maxColor = 0xFFFFFFUL;
			for(i=0; i<npfv; i++)
				if(pfv[i].depth == 24) break;
			if(pfv[i].bits_per_pixel==32){
				dc->mi.bitsPerPixel = 32;
				dc->pf= _MGL_pixelFormats[pfARGB32];
			}else
				dc->pf= _MGL_pixelFormats[pfRGB24]; 
			break;
		    }
		case 32:	dc->mi.maxColor = 0xFFFFFFFFUL;	dc->pf= _MGL_pixelFormats[pfARGB32]; break;
		}

	XFree(visinfo);

	if(d == 8){
		// Set up the private colormap if necesary
		dc->wm.xwindc.hpal = xswa.colormap = XCreateColormap(dpy, hwnd, DefaultVisualOfScreen(scr), AllocAll);
		XChangeWindowAttributes(dpy, hwnd, CWColormap, &xswa);
	    }

	XMapRaised(dpy, hwnd);
	XClearWindow(dpy, hwnd);

	XWIN_initInternal(dc);
	dc->v->w.destroy = destroyDC;
	dc->r.realizePalette = XWIN_realizePalette;
	dc->r.getDefaultPalette = XWIN_getDefaultPalette;
   	dc->r.putImage = XWIN_putImage;
	return true;
}
