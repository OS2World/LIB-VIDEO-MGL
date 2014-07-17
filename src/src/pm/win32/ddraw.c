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
* Environment:	Win32
*
* Description:	Win32 implementation for the SciTech cross platform
*				event library.
*
****************************************************************************/

#include "event.h"
#include "pmapi.h"
#include <stdio.h>
#include "win32/oshdr.h"

/*---------------------------- Global Variables ---------------------------*/

#if 0

/* Publicly accessible variables */

int				_PM_deskX,_PM_deskY;/* Desktop dimentions				*/
HWND			_PM_hwndConsole;	/* Window handle for console		*/
HWND			_PM_hwndUser;		/* User window handle				*/
HINSTANCE		_PM_hInstDD = NULL;	/* Handle to DirectDraw DLL 		*/
LPDIRECTDRAW	_PM_lpDD1 = NULL;	/* DirectDraw object				*/
LPDIRECTDRAW2 	_PM_lpDD = NULL;	/* DirectDraw2 object				*/

/* Private internal variables */

static HINSTANCE	hInstApp = NULL;/* Application instance handle		*/
#if 0
static LONG			oldWndStyle;	/* Info about old user window		*/
static LONG			oldExWndStyle;	/* Info about old user window		*/
static int			oldWinPosX;		/* Old window position X coordinate	*/
static int			oldWinPosY;		/* Old window pisition Y coordinate	*/
static int			oldWinSizeX;	/* Old window size X				*/
static int			oldWinSizeY;	/* Old window size Y				*/
static WNDPROC		oldWndProc = NULL;
#endif

/* Internal strings */

static char *szWinClassName		= "SciTechDirectDrawWindow";
static char *szAutoPlayKey 		= "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer";
static char *szAutoPlayValue	= "NoDriveTypeAutoRun";

/* Dynalinks to DirectDraw functions */

HRESULT (WINAPI *pDirectDrawCreate)(GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter);

/* Declare all GUID's as static variables within this module */

#undef	DEFINE_GUID
#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
		static const GUID name \
				= { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

DEFINE_GUID( IID_IDirectDraw2,                  0xB3A6F3E0,0x2B43,0x11CF,0xA2,0xDE,0x00,0xAA,0x00,0xB9,0x33,0x56 );

#endif

/*---------------------------- Implementation -----------------------------*/

#if 0

/****************************************************************************
REMARKS:
Temporarily disables AutoPlay operation while we are running in fullscreen
graphics modes.
****************************************************************************/
void _PM_disableAutoPlay(void)
{
	DWORD	dwAutoPlay,dwSize = sizeof(dwAutoPlay);
	HKEY	hKey;

	if (RegOpenKeyEx(HKEY_CURRENT_USER,szAutoPlayKey,0,KEY_EXECUTE | KEY_WRITE,&hKey) == ERROR_SUCCESS) {
		RegQueryValueEx(hKey,szAutoPlayValue,NULL,NULL,(void*)&dwAutoPlay,&dwSize);
		dwAutoPlay |= AUTOPLAY_DRIVE_CDROM;
		RegSetValueEx(hKey,szAutoPlayValue,0,REG_DWORD,(void*)&dwAutoPlay,dwSize);
		RegCloseKey(hKey);
		}
}

/****************************************************************************
REMARKS:
Re-enables AutoPlay operation when we return to regular GDI mode.
****************************************************************************/
void _PM_restoreAutoPlay(void)
{
	DWORD	dwAutoPlay,dwSize = sizeof(dwAutoPlay);
	HKEY	hKey;

	if (RegOpenKeyEx(HKEY_CURRENT_USER,szAutoPlayKey,0,KEY_EXECUTE | KEY_WRITE,&hKey) == ERROR_SUCCESS) {
		RegQueryValueEx(hKey,szAutoPlayValue,NULL,NULL,(void*)&dwAutoPlay,&dwSize);
		dwAutoPlay &= ~AUTOPLAY_DRIVE_CDROM;
		RegSetValueEx(hKey,szAutoPlayValue,0,REG_DWORD,(void*)&dwAutoPlay,dwSize);
		RegCloseKey(hKey);
		}
}

static void RestoreFullScreen(void)
/****************************************************************************
*
* Function:     RestoreFullScreen
*
* Description:	Reactivate all the surfaces for DirectDraw and set the
*				system back up for fullscreen rendering.  
*
****************************************************************************/
{
	static	ibool firstTime = true;

    if (firstTime) {
		int 	oldActivePage,oldVisualPage;
        ibool	isActive;
		MGLDC	*dc = _LST_first(DEV.dispDCList);
		MGLDC	*cntDC = _PM_dcPtr;

		/* Clear the message queue while waiting for the surfaces to be
		 * restored.
		 */
		firstTime = false;
		while (1) {
			/* Restore all the surfaces that we have lost */
			_EVT_pumpMessages();
			isActive = (GetActiveWindow() == _PM_hwndFullScreen);
			if (isActive) {
				if (_PM_lpPrimary) {
					if (IDirectDrawSurface_Restore(_PM_lpPrimary) != DD_OK)
						isActive = false;
					else if (_PM_lpOffscreen)
						IDirectDrawSurface_Restore(_PM_lpOffscreen);
					}
				if (_PM_winDirectMode == -1)
					_WIN_restoreFullScreenMode(dc);
				if (_PM_isFSOpenGLDC(dc))
					fglSetFocus(true);
				if (isActive) {
					/* Restore our fullscreen window and the users display */
					if (dc->mi.bitsPerPixel == 8) {
						HDC hdc = GetDC(NULL);
						SetSystemPaletteUse(hdc, SYSPAL_NOSTATIC);
						ReleaseDC(NULL,hdc);
						}
					oldActivePage = MGL_getActivePage(dc);
					oldVisualPage = MGL_getVisualPage(dc);
					MGL_makeCurrentDC(NULL);
					MGL_makeCurrentDC(dc);
					MGL_makeCurrentDC(cntDC);
					MGL_realizePalette(dc,MGL_getPaletteSize(dc),0,false);
					if (_PM_lpPrimary)
						_PM_enumerateSurfaces(dc);
					MGL_setActivePage(dc,oldActivePage);
					MGL_setVisualPage(dc,oldVisualPage,false);
					if (_PM_suspendApp)
						_PM_suspendApp(dc,MGL_REACTIVATE);
					_MS_restoreState();
					_PM_disableAutoPlay();
					_PM_backInGDI = false;
					waitActive = false;
					firstTime = true;
					return;
					}
				}
			Sleep(200);
			}
		}
}

/* {secret} */
void _PM_doSuspendApp(void)
/****************************************************************************
*
* Function:     _PM_doSuspendApp
* Parameters:	flags	- Flags indicating which key started the suspend
*
* Description:  Suspends the application by switching back to the GDI
*				desktop, allowing normal application code to be processed,
*				and then waiting for the application activate command to
*				bring us back to fullscreen mode with our window minimised.
*
*				This version only gets called if we have not captured the
*				screen switch in our activate message loops and will
*				occur if we lose a surface for some reason while rendering.
*				This should not normally happen, but it is included just to
*				be sure (note that this code will always spin loop, and we
*				cannot disable the spin looping from this version).
*
****************************************************************************/
{
	static	ibool firstTime = true;
	MGLDC	*dc = _LST_first(DEV.dispDCList);

	if (firstTime) {
		if (_PM_suspendApp)
			_PM_suspendApp(dc,MGL_DEACTIVATE);
		if (_PM_isFSOpenGLDC(dc))
			fglSetFocus(false);
		_MS_saveState();
		firstTime = false;
		if (dc->mi.bitsPerPixel == 8) {
        	HDC hdc = GetDC(NULL);
			SetSystemPaletteUse(hdc, SYSPAL_STATIC);
            ReleaseDC(NULL,hdc);
			}
		_PM_restoreAutoPlay();
		_PM_backInGDI = true;
		}
	RestoreFullScreen();
    firstTime = true;
}

/* {secret} */
void _PM_deactivate(void)
/****************************************************************************
*
* Function:     _PM_deactivate
* Parameters:	flags	- Flags indicating which key started the suspend
*
* Description:  Suspends the application by switching back to the GDI
*				desktop, allowing normal application code to be processed,
*				and then waiting for the application activate command to
*				bring us back to fullscreen mode with our window minimised.
*
****************************************************************************/
{
	MGLDC	*dc = _LST_first(DEV.dispDCList);
	int		retCode = MGL_SUSPEND_APP;

	if (_PM_backInGDI)
		return;
	if (_PM_suspendApp)
		retCode = _PM_suspendApp(dc,MGL_DEACTIVATE);
	if (_PM_isFSOpenGLDC(dc))
		fglSetFocus(false);
	_MS_saveState();
	if (dc->mi.bitsPerPixel == 8) {
		HDC hdc = GetDC(NULL);
		SetSystemPaletteUse(hdc, SYSPAL_STATIC);
		ReleaseDC(NULL,hdc);
		}
	if (_PM_winDirectMode == -1)
		_WIN_restoreDisplayMode(true);
	_PM_restoreAutoPlay();
	_PM_backInGDI = true;

	/* Now process messages normally until we are re-activated */
	waitActive = true;
	if (retCode != MGL_NO_SUSPEND_APP) {
		while (waitActive) {
			_EVT_pumpMessages();
			Sleep(200);
			}
		}
}

/****************************************************************************
REMARKS:
Main Window proc for the full screen DirectDraw Window that we create while
running in full screen mode. Here we capture all mouse and keyboard events
for the window and plug them into our event queue.
****************************************************************************/
static LONG CALLBACK PM_winProc(
	HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LONG lParam)
{
#if 0
	int	isAlt,doSwitch;
	int	where_x,where_y;

	switch (msg) {
		case WM_SYSCHAR:
			/* Stop Alt-Space from pausing our game */
			return 0;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			if (HIWORD(lParam) & KF_REPEAT) {
				if (msg == WM_SYSKEYDOWN)
                	return 0;
				break;
				}
			/* Fall through for keydown events */
		case WM_KEYUP:
		case WM_SYSKEYUP:
			if (_PM_winDirectMode == -1) {
            	/* Handle Alt-Tabbing for fullscreen DIB modes */
				isAlt = (HIWORD(lParam) & KF_ALTDOWN);
				doSwitch = false;
				if (isAlt && wParam == VK_TAB)
					doSwitch = true;			/* Alt-Tab */
				else if (isAlt && wParam == VK_ESCAPE)
					doSwitch = true;			/* Alt-Esc */
				else if (wParam == VK_ESCAPE) {
					if (GetKeyState(VK_CONTROL) & 0x8000U)
						doSwitch = true;		/* Ctrl-Esc */
					}
				if (doSwitch) {
					MGLDC *dc = _LST_first(DEV.dispDCList);
					/* Check the app wants to block switching and block it */
					if (!_PM_haveWinNT && (_PM_suspendApp && (_PM_suspendApp(dc,MGL_DEACTIVATE) == MGL_NO_DEACTIVATE)))
						return 0;
					ShowWindow(_PM_hwndFullScreen,SW_MINIMIZE);
					}
				}
			if (msg == WM_SYSKEYDOWN || msg == WM_SYSKEYUP) {
				if ((HIWORD(lParam) & KF_ALTDOWN) && wParam == VK_RETURN)
                	break;
				/* We ignore the remainder of the sysyetm keys to stop the
				 * system menu from being activated from the keyboard and pausing
				 * our app while fullscreen (ie: pressing the Alt key).
				 */
				return 0;
				}
			break;
		case WM_SYSCOMMAND:
			switch (wParam & ~0x0F) {
                case SC_SCREENSAVE:
                case SC_MONITORPOWER:
					/* Ignore screensaver requests in fullscreen modes */
					return 0;
				}
			break;
		case WM_MOUSEMOVE:
			where_x = (LOWORD(lParam) * _PM_xRes) / _PM_deskX;
			where_y = (HIWORD(lParam) * _PM_yRes) / _PM_deskY;
			if (_PM_mx != where_x || _PM_my != where_y)
				_MS_moveCursor(_PM_mx = where_x,_PM_my = where_y);
			break;
		case WM_SIZE:
			if (waitActive && _PM_backInGDI && (wParam != SIZE_MINIMIZED)) {
				/* Start the re-activation process */
				PostMessage(hwnd,WM_DO_SUSPEND_APP,WM_PM_RESTORE_FULLSCREEN,0);
				}
			else if (!waitActive && _PM_fullScreen && !_PM_backInGDI && (wParam == SIZE_MINIMIZED)) {
				/* Start the de-activation process */
				PostMessage(hwnd,WM_DO_SUSPEND_APP,WM_PM_DEACTIVATE,0);
				}
			break;
		case WM_DO_SUSPEND_APP:
			switch (wParam) {
				case WM_PM_RESTORE_FULLSCREEN:
					RestoreFullScreen();
					break;
				case WM_PM_DEACTIVATE:
					_PM_deactivate();
					break;
				}
			return 0;
		}
	if (oldWndProc)
		return oldWndProc(hwnd,msg,wParam,lParam);
	else if (_PM_userEventProc)
		return _PM_userEventProc(hwnd,msg,wParam,lParam);
#endif
	return DefWindowProc(hwnd,msg,wParam,lParam);
}

/* {secret} */
LONG WINAPI PM_subWinProc(HWND hwnd, UINT msg, WPARAM wParam, LONG lParam)
/****************************************************************************
*
* Function:		PM_subWinProc
*
* Description:  Main Window proc for the full screen WinDirect Window that
*				we create while running in full screen mode. Here we
*				capture all mouse and keyboard events for the window and
*				plug them into our event queue.
*
****************************************************************************/
{
	int		isAlt,doSwitch;

	switch (msg) {
		case WM_KEYUP:
		case WM_SYSKEYUP:
			/* Filter out the keyup events in DirectDraw that can cause us to
			 * lose our surfaces when a switch occurs.
			 */
			isAlt = (HIWORD(lParam) & KF_ALTDOWN);
			doSwitch = false;
			if (isAlt && wParam == VK_TAB)
				doSwitch = true;			/* Alt-Tab */
			else if (isAlt && wParam == VK_ESCAPE)
				doSwitch = true;			/* Alt-Esc */
			else if (wParam == VK_ESCAPE) {
				if (GetKeyState(VK_CONTROL) & 0x8000U)
					doSwitch = true;		/* Ctrl-Esc */
				}
			if (doSwitch) {
				MGLDC *dc = _LST_first(DEV.dispDCList);
				/* Check the app wants to block switching and block it */
				if (!_PM_haveWinNT && (_PM_suspendApp && (_PM_suspendApp(dc,MGL_DEACTIVATE) == MGL_NO_DEACTIVATE)))
					return 0;
				}
			break;
		}
	return _PM_orgWinProc(hwnd,msg,wParam,lParam);
}

/* {secret} */
void _PM_subClassWindow(HWND hwnd)
/****************************************************************************
*
* Function:		_PM_subClassWindow
* Parameters:	hwnd	- Handle of DirectDraw window to subclass
*
* Description:	Subclasses the DirectDraw window after DirectDraw has
*				gone into exclusive mode so that we can stop DirectDraw
*				from allowing us to Alt-Tab away from the application. Note
*				that this may get called after the window has already been
*				subclassed once.
*
****************************************************************************/
{
	WNDPROC	oldWinProc = (WNDPROC)SetWindowLong(hwnd,
		GWL_WNDPROC, (LPARAM)PM_subWinProc);
	if (!_PM_orgWinProc)
		_PM_orgWinProc = oldWinProc;
}

/* {secret} */
void _PM_unsubClassWindow(HWND hwnd)
/****************************************************************************
*
* Function:		_PM_unsubClassWindow
* Parameters:	hwnd	- Handle of DirectDraw window to subclass
*
* Description:	Subclasses the DirectDraw window after DirectDraw has
*				gone into exclusive mode so that we can stop DirectDraw
*				from allowing us to Alt-Tab away from the application. Note
*				that this may get called after the window has already been
*				subclassed once.
*
****************************************************************************/
{
	SetWindowLong(hwnd,GWL_WNDPROC, (LPARAM)_PM_orgWinProc);
	_PM_orgWinProc = NULL;
}

/* {secret} */
HWND _PM_convertUserWindow(HWND hwnd,int width,int height)
/****************************************************************************
*
* Function:		_PM_convertUserWindow
* Parameters:	hwnd	- User window to convert
*				width	- Window of the fullscreen window
*				height	- Height of the fullscreen window
* Returns:		Handle to converted fullscreen Window.
*
* Description:  This function takes the original user window handle and
*				modifies the size, position and attributes for the window
*				to convert it into a fullscreen window that we can use.
*
****************************************************************************/
{
	RECT	window;

	GetWindowRect(hwnd,&window);
    oldWinPosX = window.left;
	oldWinPosY = window.top;
    oldWinSizeX = window.right - window.left;
    oldWinSizeY = window.bottom - window.top;
	oldWndStyle = SetWindowLong(hwnd,GWL_STYLE,WS_POPUP | WS_SYSMENU);
	oldExWndStyle = SetWindowLong(hwnd,GWL_EXSTYLE,WS_EX_APPWINDOW);
    ShowWindow(hwnd,SW_SHOW);
	MoveWindow(hwnd,0,0,width,height,TRUE);
	SetWindowPos(hwnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	oldWndProc = (WNDPROC)SetWindowLong(hwnd,GWL_WNDPROC, (LPARAM)PM_winProc);
	return hwnd;
}

/* {secret} */
void _PM_restoreUserWindow(HWND hwnd)
/****************************************************************************
*
* Function:		_PM_restoreUserWindow
* Parameters:	hwnd	- User window to convert
*
* Description:  This function restores the original attributes of the
*				user window and put's it back into it's original state
*				before it was converted to a fullscreen window.
*
****************************************************************************/
{
	SetWindowLong(hwnd,GWL_WNDPROC, (LPARAM)oldWndProc);
	SetWindowLong(hwnd,GWL_EXSTYLE,oldExWndStyle);
	SetWindowLong(hwnd,GWL_STYLE,oldWndStyle);
	SetWindowPos(hwnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	ShowWindow(hwnd,SW_SHOW);
	MoveWindow(hwnd,oldWinPosX,oldWinPosY,oldWinSizeX,oldWinSizeY,TRUE);
	oldWndProc = NULL;
}

/****************************************************************************
REMARKS:
Attempts to dynamically load the DirectDraw DLL's and create the DirectDraw
objects that we need.
****************************************************************************/
ibool _PM_loadDirectDraw(void)
{
	GET_DEFAULT_CW();
	if (!_PM_hInstDD) {
		WNDCLASS	cls;
		HDC			hdc;
		int			bits;

		hdc = GetDC(NULL);
		bits = GetDeviceCaps(hdc,BITSPIXEL);
		ReleaseDC(NULL,hdc);
		if (bits < 8)
			return false;
		if ((_PM_hInstDD = LoadLibrary("ddraw.dll")) == NULL)
			return false;
		pDirectDrawCreate = (void*)GetProcAddress(_PM_hInstDD,"DirectDrawCreate");
		if (!pDirectDrawCreate)
			return false;

		/* Create a Window class for the fullscreen window in here, since
		 * we need to register one that will do all our event handling for
		 * us.
		 */
		if (!hInstApp) {
			hInstApp 			= GetModuleHandle(NULL);
			cls.hCursor        	= LoadCursor(NULL,IDC_ARROW);
			cls.hIcon          	= LoadIcon(hInstApp,MAKEINTRESOURCE(1));
			cls.lpszMenuName   	= NULL;
			cls.lpszClassName  	= szWinClassName;
			cls.hbrBackground	= GetStockObject(BLACK_BRUSH);
			cls.hInstance      	= hInstApp;
			cls.style          	= CS_DBLCLKS;
			cls.lpfnWndProc     = PM_winProc;
			cls.cbWndExtra     	= 0;
			cls.cbClsExtra     	= 0;
			if (!RegisterClass(&cls))
				PM_fatalError("Unable to register Window Class!");
			}
		}
	if (!_PM_lpDD) {
		/* Create the DirectDraw and DirectDraw2 objects */
		if (DirectDrawCreate(NULL, &_PM_lpDD1, NULL) != DD_OK) {
			_PM_lpDD1 = NULL;
			return false;
			}
		if (IDirectDraw_QueryInterface(_PM_lpDD1,&IID_IDirectDraw2, (void*)&_PM_lpDD) != DD_OK) {
			_PM_lpDD = NULL;
			IDirectDraw_Release(_PM_lpDD1);
			return false;
			}
		}
	RESET_DEFAULT_CW();
	return true;
}

/****************************************************************************
REMARKS:
Frees any DirectDraw objects and unloads the DirectDraw libraries.
****************************************************************************/
void _PM_unloadDirectDraw(void)
{
	/* Delete the DirectDraw object if we have created it */
	if (_PM_lpDD) {
		IDirectDraw_Release(_PM_lpDD1);
		_PM_lpDD = NULL;
		_PM_lpDD1 = NULL;
		}
	if (_PM_hInstDD) {
		FreeLibrary(_PM_hInstDD);
		_PM_hInstDD = NULL;
		}
}

void _DD_createMainWindow(void)
/****************************************************************************
*
* Function:		_DD_createMainWindow
*
* Description:	Creates the main fullscreen window for DirectDraw and
*				fullscreen DIB modes.
*
****************************************************************************/
{
	/* Create the fullscreen window */
	if (!_MGL_hwndFullScreen) {
		if (!_MGL_hwndUser) {
			char windowTitle[80];
			if (LoadString(_MGL_hInst,1,windowTitle,sizeof(windowTitle)) == 0)
                strcpy(windowTitle,"MGL Fullscreen Application");
			_MGL_hwndFullScreen = CreateWindowEx(WS_EX_APPWINDOW,_DD_szWinClassName,
				windowTitle,WS_POPUP | WS_SYSMENU,0,0,_MGL_deskX,_MGL_deskY,
                NULL,NULL,_MGL_hInst,NULL);
			}
		else {
            _MGL_hwndFullScreen = _DD_convertUserWindow(_MGL_hwndUser,_MGL_deskX,_MGL_deskY);
			}
        ShowCursor(FALSE);
        SetWindowPos(_MGL_hwndFullScreen,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
        ShowWindow(_MGL_hwndFullScreen,SW_SHOW);
		}
    SetFocus(_MGL_hwndFullScreen);
    SetForegroundWindow(_MGL_hwndFullScreen);
}

ibool MGLAPI DDRAW_setMode(MGLDC *dc,int virtualX,int virtualY,int numBuffers,
	ibool stereo,int refreshRate)
/****************************************************************************
*
* Function:		DDRAW_setMode
* Parameters:	dc			- Device context to initialise
*				numBuffers	- Number of buffers to allocate for the mode
* Returns:		TRUE if the mode was successfully set.
*
* Description:	Initialises the video mode by calling DirectDraw to go into
*				fullscreen exclusive mode, set the display mode and create
*				all the primary and secondary surfaces for the display.
*
****************************************************************************/
{
	HRESULT			ret;
	DDSURFACEDESC	ddsd;
    uchar			bits,pf;
	char			msg[256];

	/* We dont support Stereo via DirectDraw yet */
	if (stereo)
		return false; 

	/* Check that there is not a request for a scrolling surface, as DirectDraw
	 * does not support such surfaces.
	 */
	if (numBuffers == 0)
    	return false;
	if (virtualX != -1 && virtualY != -1) {
		if (virtualX != dc->mi.xRes || virtualY != dc->mi.yRes)
			return false;
		}
	_MGL_xRes = _MGL_deskX = dc->mi.xRes+1;
	_MGL_yRes = _MGL_deskY = dc->mi.yRes+1;

	/* Hide the MGL main window if present */
	if (_MGL_hwndMain) {
        ShowWindow(_MGL_hwndMain,SW_HIDE);
        EnableWindow(_MGL_hwndMain,FALSE);
		}

	/* Destroy WinDirect mode if we are currently running in it */
	if (_MGL_winDirectMode || _MGL_fsOpenGLType)
		MGLHW_destroyFullScreenMode();
	_MGL_fsOpenGLType = (numBuffers == -1);

	/* Create the fullscreen window */
	_DD_createMainWindow();

	/* Set the Cooperative level */
	if ((ret = IDirectDraw2_SetCooperativeLevel(_MGL_lpDD,_MGL_hwndFullScreen,DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN)) != DD_OK) {
		sprintf(msg,"SetCooperativeLevel failed (%s)!", DDRAW_errMsg(ret));
		goto Error;
		}

	/* Set the display mode */
	if (refreshRate == MGL_DEFAULT_REFRESH)
		refreshRate = 0;
	if ((ret = IDirectDraw2_SetDisplayMode(_MGL_lpDD,_MGL_xRes,_MGL_yRes,
			(dc->mi.bitsPerPixel == 15) ? 16 : dc->mi.bitsPerPixel,refreshRate,0)) != DD_OK) {
		if ((ret = IDirectDraw2_SetDisplayMode(_MGL_lpDD,_MGL_xRes,_MGL_yRes,
				(dc->mi.bitsPerPixel == 15) ? 16 : dc->mi.bitsPerPixel,0,0)) != DD_OK) {
			sprintf(msg,"SetDisplayMode failed (%s)!", DDRAW_errMsg(ret));
			goto Error;
			}
		}
	_MGL_fullScreen = true;

	if (numBuffers >= 1) {
		/* Create the primary surface and all requested backbuffers */
        ddsd.dwSize = sizeof(ddsd);
		if (numBuffers > 1) {
			ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
			ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX | DDSCAPS_VIDEOMEMORY;
			ddsd.dwBackBufferCount = numBuffers-1;
			dc->mi.maxPage = numBuffers-1;
			}
		else {
			ddsd.dwFlags = DDSD_CAPS;
			ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_VIDEOMEMORY;
			dc->mi.maxPage = 0;
			}

		if ((ret = IDirectDraw2_CreateSurface(_MGL_lpDD,&ddsd,&_MGL_lpPrimary,NULL)) != DD_OK) {
			/* Can't get two surfaces in video memory, so try for a flipping surface without forcing to
			 * video memory.
			 */
			ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
			ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
			ddsd.dwBackBufferCount = numBuffers-1;
			if ((ret = IDirectDraw2_CreateSurface(_MGL_lpDD,&ddsd,&_MGL_lpPrimary,NULL)) != DD_OK) {
                IDirectDraw2_RestoreDisplayMode(_MGL_lpDD);
                sprintf(msg,"CreateSurface failed (%s)!", DDRAW_errMsg(ret));
				goto Error;
				}
			}
		DD_numBuffers = numBuffers;
		if (!_DD_enumerateSurfaces(dc))
			return false;

		/* Read the DirectDraw surface description for our primary surface and
		 * re-build the pixel format and bits per pixel values.
		 * There are some DirectDraw drivers out there that incorrectly return
		 * values for EnumDisplayModes, so we fix them up here if the 16bpp
		 * modes was incorrectly reported.
		 */
        ddsd.dwSize = sizeof (DDSURFACEDESC);
        IDirectDrawSurface_GetSurfaceDesc(_MGL_lpPrimary,&ddsd);
        DD_getPixelFormat(&ddsd,&bits,&pf);
		dc->mi.bitsPerPixel = bits;
		if (dc->mi.bitsPerPixel == 15)
			dc->mi.maxColor = 0x7FFF;
		else if (dc->mi.bitsPerPixel == 16)
			dc->mi.maxColor = 0xFFFF;
        memcpy(&dc->mi.redMaskSize,&pixelFormats[pf],sizeof(pixelFormat));

		/* Create the palette object for 8 bit displays */
		if (dc->mi.bitsPerPixel == 8) {
			PALETTEENTRY pal[256];
            memset(pal,0,sizeof(pal));
            IDirectDraw2_CreatePalette(_MGL_lpDD,DDPCAPS_8BIT | DDPCAPS_ALLOW256,pal,&_MGL_lpPalette,NULL);
            IDirectDrawSurface_SetPalette(_MGL_lpPrimary,_MGL_lpPalette);

			/* TODO: Figure out how to determine the wide palette flag */
			dc->v->d.widePalette = true;
			}
		else
			_MGL_lpPalette = NULL;

		/* Setup the surface access flags */
		if (_MGL_ddCaps.dwCaps & DDCAPS_BANKSWITCHED)
			dc->flags |= MGL_VIRTUAL_ACCESS;
		else
			dc->flags |= MGL_LINEAR_ACCESS;
		}
	else {
		_MGL_lpPrimary = NULL;
        _MGL_lpPalette = NULL;
		}
	dc->v->d.isStereo = _MGL_NO_STEREO;

	/* Initialise the fullscreen event handling routines */
    _EVT_init(_MGL_hwndFullScreen);
    _DD_subClassWindow(_MGL_hwndFullScreen);
	return true;

Error:
    DestroyWindow(_MGL_hwndFullScreen);
    MessageBox(NULL,msg,"MGL DirectDraw failure",MB_ICONEXCLAMATION);
	return false;
}

#endif

PM_HWND PMAPI PM_openConsole(
	PM_HWND hwndUser,
	int device,
	int xRes,
	int yRes,
	int bpp,
	ibool fullScreen)
{
//	if (fullScreen)
//		PM_changeResolution(device,xRes,yRes,bpp);
	(void)hwndUser;
	(void)device;
	(void)xRes;
	(void)yRes;
	(void)bpp;
	(void)fullScreen;
	return 0;
}

int PMAPI PM_getConsoleStateSize(void)
{
	/* Not used in Windows */
	return 1;
}

void PMAPI PM_saveConsoleState(
	void *stateBuf,
	PM_HWND hwndConsole)
{
	/* Not used in Windows */
	(void)stateBuf;
	(void)hwndConsole;
}

void PMAPI PM_changeResolution(
	PM_HWND hwndConsole,
	int xRes,
	int yRes,
	int bpp)
{
	(void)hwndConsole;
	(void)xRes;
	(void)yRes;
	(void)bpp;
}

void PMAPI PM_setSuspendAppCallback(
	int (_ASMAPIP saveState)(
		int flags))
{
	(void)saveState;
}

void PMAPI PM_restoreConsoleState(
	const void *stateBuf,
	PM_HWND hwndConsole)
{
	/* Not used in Windows */
	(void)stateBuf;
	(void)hwndConsole;
}

void PMAPI PM_closeConsole(
	PM_HWND hwndConsole)
{
	(void)hwndConsole;
}

#if 0
/* {secret} */
void MGLHW_destroyFullScreenMode(void)
/****************************************************************************
*
* Function:		MGLHW_destroyFullScreenMode
*
* Description:	Destroys the fullscreen window when switching back to
*				the desktop or creating a new windowed mode.
*
****************************************************************************/
{
	if (_PM_hwndFullScreen) {
		if (_PM_winDirectMode == 1) {
			WD_restoreGDI();
			WD_registerFullScreenWindow(NULL);
			_PM_winDirectMode = 0;
			}
		else {
			if (_PM_winDirectMode == -1) {
				ChangeDisplaySettings(NULL, CDS_FULLSCREEN);
				}
			else {
				_DD_unsubClassWindow(_PM_hwndFullScreen);
				IDirectDraw2_SetCooperativeLevel(_PM_lpDD,_PM_hwndFullScreen,DDSCL_NORMAL);
				IDirectDraw2_RestoreDisplayMode(_PM_lpDD);
				}
			if (_PM_hwndUser)
				_DD_restoreUserWindow(_PM_hwndFullScreen);
			else
				DestroyWindow(_PM_hwndFullScreen);

			/* Show and enable the main application window */
			if (_PM_hwndMain) {
				EnableWindow(_PM_hwndMain,TRUE);
				ShowWindow(_PM_hwndMain,SW_SHOW);
				SetActiveWindow(_PM_hwndMain);
				SetWindowPos(_PM_hwndMain,HWND_TOP,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
				SetForegroundWindow(_PM_hwndMain);
				}

			/* Restore the arrow cursor */
			ShowCursor(TRUE);
			SetCursor(LoadCursor(NULL,IDC_ARROW));
			}
		_PM_restoreAutoPlay();
		}
	RESET_DEFAULT_CW();
	_PM_hwndFullScreen = NULL;
}

#endif
