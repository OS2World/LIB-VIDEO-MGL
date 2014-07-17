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
* Language:     ANSI C
* Environment:  Win32
*
* Description:  Windows version of MGLDOG sample app that runs in a window
*               on the Windows desktop. This shows how you can build a
*               normal windows application and use the MGL for high
*               performance rendering to a window.
*
****************************************************************************/

#define	WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "mgraph.h"
#include "wmgldog.rh"

/*--------------------------- Global Variables ----------------------------*/

char        szAppName[] = "WMGLDOG: MGL Sprite Demo";
HINSTANCE   hInstApp;
HWND        hwndApp;
color_t     transparent;
int         sizex,sizey,width,height;
MGLDC       *winDC = NULL,*dogDC = NULL,*memDC = NULL;

/*------------------------------ Implementation ---------------------------*/

/****************************************************************************
PARAMETERS:
dc          - Display dc
bitmapName  - Name of bitmap file to load

RETURNS:
Pointer to valid memory DC with bitmap loaded

REMARKS:
Loads the specified bitmap into a memory DC with the same dimensions as the
bitmap on disk, but with the same pixel depth and format used by the display
DC (for maximum blt performance). The MGL automatically handles pixel format
conversion for us when we load the bitmap into our memory DC, and the
bitmap's palette is loaded into the memory DC's palette.
****************************************************************************/
MGLDC *LoadBitmapIntoMemDC(
	MGLDC *dc,
	char *bitmapName)
{
    int             width,height,bits;
    pixel_format_t  pf;
    MGLDC           *memDC;

    /* Get dimensions of bitmap image on disk */
    if (!MGL_getBitmapSize(bitmapName,&width,&height,&bits,&pf))
        MGL_fatalError("Cant locate bitmap file!");

    /* Get bitmap depth and pixel format for memory DC */ 
	bits = MGL_getBitsPerPixel(dc);
	MGL_getPixelFormat(dc,&pf);

    /* Create the memory DC and load the bitmap file into it */
    if ((memDC = MGL_createMemoryDC(width,height,bits,&pf)) == NULL)
        MGL_fatalError("Not enough memory to load bitmap!");
    if (!MGL_loadBitmapIntoDC(memDC,bitmapName,0,0,true))
        MGL_fatalError("Cant locate bitmap file!");
    return memDC;
}

/****************************************************************************
PARAMETERS:
dc	- Windowed DC to create memory back buffer for

REMARKS:
Creates a memory back buffer DC for a Windowed DC of the correct dimensions.
****************************************************************************/
MGLDC *CreateBackBufferDC(
	MGLDC *dc)
{
	int             bits;
    pixel_format_t  pf;
	palette_t       pal[256];
	MGLDC			*memDC;

    /* Create the main MGL Windowed DC for interfacing our main window */
	bits = MGL_getBitsPerPixel(dc);
	MGL_getPixelFormat(dc,&pf);
	sizex = MGL_sizex(dc)+1;
	sizey = MGL_sizey(dc)+1;

    /* Create the main backbuffer DC with same pixel format as Window DC */
    if ((memDC = MGL_createMemoryDC(sizex,sizey,bits,&pf)) == NULL)
		MGL_fatalError("Unable to create Memory DC!");
	MGL_makeCurrentDC(memDC);

	/* Copy the palette from the sprite and store in the offscreen bitmap
	 * and the Window DC. The MGL automatically takes care of creating
	 * identity palettes when we realize the palette for the memory DC and
	 * the Window DC, but we must make sure that both of them get realized.
	 */
	MGL_getPalette(dogDC,pal,MGL_getPaletteSize(dogDC),0);
	MGL_setPalette(memDC,pal,MGL_getPaletteSize(memDC),0);
	MGL_realizePalette(memDC,MGL_getPaletteSize(memDC),0,false);
	MGL_setPalette(dc,pal,MGL_getPaletteSize(dc),0);
	MGL_realizePalette(dc,MGL_getPaletteSize(dc),0,false);
	return memDC;
}

/****************************************************************************
PARAMETERS:
hwnd    - Handle to Window used for output

REMARKS:
Sets up the MGL for output to the client region of the specified window.
****************************************************************************/
void CreateMGLDeviceContexts(
	HWND hwnd)
{
	/* Create the main MGL Windowed DC for interfacing our main window */
    if ((winDC = MGL_createWindowedDC(hwnd)) == NULL)
        MGL_fatalError("Unable to create Windowed DC!");

	/* Load the sprite bitmap with same pixel format as Window DC */
	dogDC = LoadBitmapIntoMemDC(winDC,"doggie2.bmp");

	/* Get transparent color from pixel (0,0) and dimensions of sprite */
	MGL_makeCurrentDC(dogDC);
	transparent = MGL_getPixelCoord(0,0);
	width = MGL_sizex(dogDC)+1;
	height = MGL_sizey(dogDC)+1;

	/* Create the main backbuffer DC with same pixel format as Window DC */
	memDC = CreateBackBufferDC(winDC);
}

/****************************************************************************
PARAMETERS:
hwnd    - Handle to Window used for output

REMARKS:
Sets up the MGL for output to the client region of the specified window.
This function will destroy any previously existing MGL device contexts, and
will re-build all the DC's for the Window. Hence this routine is always
called from the Windows WM_SIZE message handler, which will always be
called before the first WM_PAINT message is received, allowing the MGL
device contexts to be created with the appropriate size information.
****************************************************************************/
void ResizeMGLDeviceContexts(
	HWND hwnd)
{
	/* Re-size the Windowed DC, and re-create the backbuffer DC with
	 * the new window dimensions.
	 */
	if (winDC) {
		MGL_makeCurrentDC(memDC);
		MGL_resizeWinDC(winDC);
		MGL_destroyDC(memDC);
		memDC = CreateBackBufferDC(winDC);
		}
}

/****************************************************************************
PARAMETERS:
hwnd    - Handle to Window used for output

REMARKS:
Sets up the MGL for output to the client region of the specified window.
This function will destroy any previously existing MGL device contexts, and
will re-build all the DC's for the Window. Hence this routine is always
called from the Windows WM_SIZE message handler, which will always be
called before the first WM_PAINT message is received, allowing the MGL
device contexts to be created with the appropriate size information.
****************************************************************************/
void DestroyMGLDeviceContexts(
	HWND hwnd)
{
	MGL_makeCurrentDC(NULL);
	if (memDC)
        MGL_destroyDC(memDC);
	if (dogDC)
		MGL_destroyDC(dogDC);
	if (winDC)
		MGL_destroyDC(winDC);
}

/****************************************************************************
REMARKS:
Dialog procedure for About dialog box.
****************************************************************************/
BOOL CALLBACK AppAbout(
	HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	char	format[80],buf[80];

    switch (msg) {
        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK)
                EndDialog(hwnd,TRUE);
            break;
        case WM_INITDIALOG:
			GetDlgItemText(hwnd,IDC_VERSION_STR,format,sizeof(format));
			sprintf(buf,format,MGL_VERSION_STR);
			SetDlgItemText(hwnd,IDC_VERSION_STR,buf);
			return TRUE;
        }
    lParam = lParam;
    return FALSE;
}

/****************************************************************************
REMARKS:
Main window procedure for application.
****************************************************************************/
LONG CALLBACK AppWndProc(
	HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC         hdc;
    BOOL        leftDown,rightDown;
	int         x,y;
	POINT       ofs;

	switch (msg) {
		case WM_CREATE:
			CreateMGLDeviceContexts(hwnd);
			break;
		case WM_SIZE:
			ResizeMGLDeviceContexts(hwnd);
			break;
		case WM_DESTROY:
			DestroyMGLDeviceContexts(hwnd);
            PostQuitMessage(0);
            break;
		case WM_COMMAND:
			switch (wParam) {
				case MENU_ABOUT:
					DialogBox(hInstApp,"AppAbout",hwnd,(DLGPROC)AppAbout);
					break;
				case MENU_EXIT:
					PostMessage(hwnd,WM_CLOSE,0,0L);
					break;
				}
			return 0;
		case WM_MOUSEMOVE:
			leftDown = (GetKeyState(VK_LBUTTON) < 0);
			rightDown = (GetKeyState(VK_RBUTTON) < 0);
			if (!leftDown && !rightDown)
				break;
			hdc = GetDC(hwnd);
			GetViewportOrgEx(hdc,&ofs);
			ReleaseDC(hwnd,hdc);
			x = LOWORD(lParam) - width/2 - ofs.x;
			y = HIWORD(lParam) - height/2 - ofs.y;
			if (leftDown)
				MGL_srcTransBltCoord(memDC,dogDC,0,0,width,height,x,y,transparent,MGL_REPLACE_MODE);
			else if (rightDown)
				MGL_bitBltCoord(memDC,dogDC,0,0,width,height,x,y,MGL_REPLACE_MODE);
			MGL_bitBltCoord(winDC,memDC,x,y,x+width,y+height,x,y,MGL_REPLACE_MODE);
			break;
		case WM_PALETTECHANGED:
			if ((HWND)wParam == hwnd)
				break;
			/* Fall through to WM_QUERYNEWPALETTE */
		case WM_QUERYNEWPALETTE:
			if (winDC && MGL_activatePalette(winDC,false)) {
				InvalidateRect(hwnd,NULL,TRUE);
				return 1;
				}
            return 0;
		case WM_PAINT:
			MGL_beginPaint(winDC,BeginPaint(hwnd,&ps));
			MGL_bitBltCoord(winDC,memDC,0,0,sizex,sizey,0,0,MGL_REPLACE_MODE);
			MGL_endPaint(winDC);
            EndPaint(hwnd,&ps);
            return 0;
        }
    return DefWindowProc(hwnd,msg,wParam,lParam);
}

/****************************************************************************
REMARKS:
Main application entry point.
****************************************************************************/
int PASCAL WinMain(
	HINSTANCE hInst,
	HINSTANCE hPrev,
	LPSTR szCmdLine,
	int sw)
{
	MSG			msg;
	WNDCLASS 	cls;

	/* Save instance handle for dialog boxes */
	hInstApp = hInst;
	szCmdLine = szCmdLine;

	if (!hPrev) {
		/* Register a class for the main application window */
		cls.hCursor        = LoadCursor(NULL,IDC_ARROW);
        cls.hIcon          = LoadIcon(hInst,"AppIcon");
        cls.lpszMenuName   = "AppMenu";
        cls.lpszClassName  = szAppName;
        cls.hbrBackground  = NULL;
        cls.hInstance      = hInst;
        cls.style          = CS_BYTEALIGNCLIENT | CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
		cls.lpfnWndProc    = (LPVOID)AppWndProc;
        cls.cbWndExtra     = 0;
		cls.cbClsExtra     = 0;
        if (!RegisterClass(&cls))
            return FALSE;
        }

    /* Initialise the MGL */
	if (MGL_init(".",NULL) == 0)
		MGL_fatalError(MGL_errorMsg(MGL_result()));

    /* Create the main window and display it */
    hwndApp = CreateWindow(szAppName,szAppName,WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,0,400,400,NULL,NULL,hInst,NULL);
    ShowWindow(hwndApp,sw);

    /* Polling messages from event queue */
    while (1) {
        if (PeekMessage(&msg, NULL, 0, 0,PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                break;
			TranslateMessage(&msg);
            DispatchMessage(&msg);
            }
		else
			WaitMessage();
        }

    /* Clean up the MGL before exit */
    MGL_exit();
    return msg.wParam;
}
