/****************************************************************************
*
*                       MegaGraph Graphics Library
*
*               Copyright (C) 1991-1997 SciTech Software, Inc.
*                            All rights reserved.
*
*  ======================================================================
*       This library is free software; you can use it and/or
*       modify it under the terms of the SciTech MGL Software License.
*
*       This library is distributed in the hope that it will be useful,
*       but WITHOUT ANY WARRANTY; without even the implied warranty of
*       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*       SciTech MGL Software License for more details.
*  ======================================================================
*
* Filename:     $Workfile:   wmgldog.c  $
* Version:      $Revision:   1.8  $
*
* Language:     ANSI C
* Environment:  Win32
*
* Description:  QNX version of MGLDOG sample app that runs in a window
*		on the Photon desktop. This shows how you can build a
*               normal Photon application and use the MGL for high
*               performance rendering to a window.
*
* $Date:   01 Oct 1997 05:52:08  $ $Author:   KendallB  $
*
****************************************************************************/

#include <Pt.h>
#include "mgraph.h"

/*--------------------------- Global Variables ----------------------------*/

int		sizex, sizey, width, height;
MGLDC		*winDC = NULL, *dogDC = NULL, *memDC = NULL;
color_t		transparent;
PtWidget_t	*raw;

/*------------------------------ Implementation ---------------------------*/

void GetMemoryBitmapDepth(MGLDC *dc,int *bits,pixel_format_t *pf)
/****************************************************************************
*
* Function:     GetMemoryBitmapDepth
* Parameters:   dc      - Window DC
*               bits    - Place to store pixel depth
*               pf      - Place to store pixel format
* Returns:      Pointer to valid memory DC with bitmap loaded
*
* Description:  
*		Determine the depth and format of the current display mode.
*               We want to create our memory DC with the same depth and
*		format as the current Photon display mode, for performance.
*
****************************************************************************/
{
        *bits = MGL_getBitsPerPixel(dc);
        MGL_getPixelFormat(dc,pf);
}

MGLDC *LoadBitmapIntoMemDC(MGLDC *dc,char *bitmapName)
/****************************************************************************
*
* Function:     LoadBitmapIntoMemDC
* Parameters:   dc          - Display dc
*               bitmapName  - Name of bitmap file to load
* Returns:      Pointer to valid memory DC with bitmap loaded
*
* Description:  Loads the specified bitmap into a memory DC with the
*               same dimensions as the bitmap on disk, but with the same
*               pixel depth and format used by the display DC (for maximum
*               blt performance). The MGL automatically handles pixel
*               format conversion for us when we load the bitmap into our
*               memory DC, and the bitmap's palette is loaded into the
*               memory DC's palette.
*
****************************************************************************/
{
    int             width,height,bits;
    pixel_format_t  pf;
    MGLDC           *memDC;

    /* Get dimensions of bitmap image on disk */
    if (!MGL_getBitmapSize(bitmapName,&width,&height,&bits,&pf))
        MGL_fatalError("Cant locate bitmap file!");

    /* Get bitmap depth and pixel format for memory DC */ 
    GetMemoryBitmapDepth(dc,&bits,&pf);

    /* Create the memory DC and load the bitmap file into it */
    if ((memDC = MGL_createMemoryDC(width,height,bits,&pf)) == NULL)
        MGL_fatalError("Not enough memory to load bitmap!");
    if (!MGL_loadBitmapIntoDC(memDC,bitmapName,0,0,true))
        MGL_fatalError("Cant locate bitmap file!");
    return memDC;
}

void InitMGL(PtWidget_t *window)
/****************************************************************************
*
* Function:     InitMGL
*
* Description:  Initialise the MGL and load all bitmap resources required by
*               the application. This is where the application should allocate
*               space for all MGL memory DC's used for storing sprites etc
*               and load the sprites from disk.
*
****************************************************************************/
{
    MGLDC           *winDC;
    int             bits;
    pixel_format_t  pf;

    /* Register the MGL packed pixel drivers we want to use. For this demo
     * we need all color depths >= 8 bits per pixel, since we will be
     * automatically converting our sprite and offscreen bitmap buffer to
     * the same pixel format as the Windows video mode.
     */
    MGL_registerDriver(MGL_PACKED8NAME,PACKED8_driver);
    MGL_registerDriver(MGL_PACKED16NAME,PACKED16_driver);
    MGL_registerDriver(MGL_PACKED24NAME,PACKED24_driver);
    MGL_registerDriver(MGL_PACKED32NAME,PACKED32_driver);

    /* Initialise the MGL for Windowed output only */
    MGL_initWindowed("../../../");

    /* Create an MGL Windowed DC for the Photon Desktop to obtain pixel depth
     * and pixel format of the Windows display driver video mode. We can then
     * check that we are in the correct video mode and also load the bitmaps
     * in the appropriate format.
     */
    if ((winDC = MGL_createWindowedDC((MGL_HWND)window)) == NULL)
	MGL_fatalError("Unable to create Windowed DC!");
    if ((bits = MGL_getBitsPerPixel(winDC)) < 8)
	MGL_fatalError("This program requires 256 or more colors!");
    MGL_getPixelFormat(winDC,&pf);

    /* Load the sprite bitmap into the dogDC */
    dogDC = LoadBitmapIntoMemDC(winDC,"doggie2.bmp");

    /* Get transparent color from pixel (0,0) and dimensions of sprite */
    MGL_makeCurrentDC(dogDC);
    transparent = MGL_getPixelCoord(0,0);
    width = MGL_sizex(dogDC)+1;
    height = MGL_sizey(dogDC)+1;
    MGL_makeCurrentDC(NULL);

    /* Destroy the temporary DC */
    MGL_destroyDC(winDC);
}

void CreateMGLDeviceContexts(PtWidget_t *window)
/****************************************************************************
*
* Function:     CreateMGLDeviceContexts
* Parameters:   window    - Pointer to Window used for output
*
* Description:  Sets up the MGL for output to the client region of the
*               specified window. This function will destroy any previously
*               existing MGL device contexts, and will re-build all the
*               DC's for the Window.
*
****************************************************************************/
{
    int             bits, bytes;
    pixel_format_t  pf;
    palette_t       pal[256];
    void            *surface;

    /* Destroy the previous DC's if they existed */
    if (winDC)
        MGL_destroyDC(winDC);
    if (memDC)
        MGL_destroyDC(memDC);

    /* Create the main MGL Windowed DC for interfacing our main window */
    if ((winDC = MGL_createWindowedDC((MGL_HWND)window)) == NULL)
        MGL_fatalError("Unable to create Windowed DC!");
    GetMemoryBitmapDepth(winDC,&bits,&pf);
    sizex = MGL_sizex(winDC)+1;
    sizey = MGL_sizey(winDC)+1;

    bytes = (bits+7) / 8;
#if 0
    surface = malloc(sizex*sizey*bytes);
#endif
    surface = PgShmemCreate(sizex*sizey*bytes, NULL);

    /* Create the main backbuffer DC with same pixel format as Window DC */
    if ((memDC = MGL_createCustomDC(sizex,sizey,
	bits,&pf,sizex*bytes,surface,NULL)) == NULL)
		MGL_fatalError("Unable to create Memory DC!");

    /* Copy the palette from the sprite and store in the offscreen bitmap
     * and the Window DC. The MGL automatically takes care of creating
     * identity palettes when we realize the palette for the memory DC and
     * the Window DC, but we must make sure that both of them get realized.
     */
    MGL_getPalette(dogDC,pal,MGL_getPaletteSize(dogDC),0);
    MGL_setPalette(memDC,pal,MGL_getPaletteSize(memDC),0);
    MGL_realizePalette(memDC,MGL_getPaletteSize(memDC),0,false);
    MGL_setPalette(winDC,pal,MGL_getPaletteSize(winDC),0);
    MGL_realizePalette(winDC,MGL_getPaletteSize(winDC),0,false);

    /* Clear the backbuffer and draw the dog in the middle */
    MGL_makeCurrentDC(memDC);
    MGL_clearDevice();
    MGL_transBltCoord(memDC,dogDC,0,0,width,height,(sizex - width)/2,
        (sizey - height)/2,transparent,true);
    MGL_makeCurrentDC(NULL);
}

void
MotionButtonCallback(PtWidget_t *window, void *data, PtCallbackInfo_t *cbinfo) 
{
	PhPointerEvent_t	*pev = PhGetData(cbinfo->event);
	short			x, y;

	PtGetAbsPosition(window, &x, &y);
	x = pev->pos.x - width/2 - x;
	y = pev->pos.y - height/2 - y;

	if (pev->buttons & Ph_BUTTON_3)		/* Left button */
		MGL_transBltCoord(memDC, dogDC,
		    0, 0, width, height, x, y, transparent, true);
	else if (pev->buttons & Ph_BUTTON_1)	/* Right button */
		MGL_bitBltCoord(memDC, dogDC,
		    0, 0, width, height, x, y, MGL_REPLACE_MODE);

	PgSetRegion(window->rid);
	MGL_bitBltCoord(winDC, memDC,
	    x, y, x+width, y+height, x, y, MGL_REPLACE_MODE);
}

/* Called when the main window has been resized */
void
ResizeCallback(PtWidget_t *window, void *data, PtCallbackInfo_t *info)
{
	PtArg_t			arg[1];
	PhDim_t			dim;

	/* Resize the canvas to match the new window size */
	PtWidgetDim(window, &dim);
	PtSetArg(&arg[0], Pt_ARG_DIM, &dim, 0);
	PtSetResources(raw, 1, arg);

	/* Re-create the MGL contexts with the new dimensions */
	CreateMGLDeviceContexts(raw);
}

/*
 * Called when contents of raw canvas need to be redrawn.
 * Displays the contents of the memory DC onto the Photon window DC
 */
void
draw(PtWidget_t *window, PhTile_t *damage)
{
	if (!memDC)
		return;
	MGL_bitBltCoord(winDC, memDC,
	    0, 0, memDC->mi.xRes+1, memDC->mi.yRes+1, 0, 0, MGL_REPLACE_MODE);
}

main(int argc, char **argv)
{
	PhDim_t		dim;
	PhPoint_t	pos;
	PtAppContext_t  app;
	PtArg_t         arg[3];
	PtWidget_t      *window;

	/* Create the main window */
	dim.w = 640, dim.h = 480;
	PtSetArg(&arg[0], Pt_ARG_WINDOW_TITLE, argv[0], 0);
	PtSetArg(&arg[1], Pt_ARG_DIM, &dim, 0);
	PtSetArg(&arg[2], Pt_ARG_RESIZE_FLAGS, Pt_FALSE, Pt_RESIZE_XY_AS_REQUIRED);
	window = PtAppInit(&app, &argc, argv, 3, arg);

	/* Create a raw canvas covering the interior of the main window */
	PtWidgetDim(window, &dim);
	pos.x = pos.y = 0; 
	PtSetArg(&arg[0], Pt_ARG_DIM, &dim, 0);
	PtSetArg(&arg[1], Pt_ARG_POS, &pos, 0);
	PtSetArg(&arg[2], Pt_ARG_RAW_DRAW_F, &draw, 1);
	raw = PtCreateWidget(PtRaw, window, 3, arg);

	PtRealizeWidget(window);

	InitMGL(window);
	CreateMGLDeviceContexts(window);

	/* add a resize callback to the main window */
	PtAddCallback(window, Pt_CB_RESIZE, ResizeCallback, NULL); 

	MGL_bitBltCoord(winDC, memDC,
	    0, 0, memDC->mi.xRes+1, memDC->mi.yRes+1, 0, 0, MGL_REPLACE_MODE);

	PtAddEventHandler(window, Ph_EV_PTR_MOTION_BUTTON, 
	    MotionButtonCallback, NULL); 

	PtMainLoop();
}
