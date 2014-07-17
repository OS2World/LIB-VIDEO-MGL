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
* Language:     C++ 3.0
* Environment:  any
*
* Description:  Main program code for the demo program. This _must_ not be
*               compiled with anything but normal 8086 instructions, since
*               it must be able to detect an incorrect processor
*               configuration!
*
****************************************************************************/

#include "demo.hpp"
#pragma hdrstop
#ifdef	ISV_LICENSE
#include "nucleus/graphics.h"
#include "isv.c"
#endif

/*---------------------------- Global Variables ---------------------------*/

int     mode = -1,snowlevel = -1;
ibool 	useVBE = true,useNucleus = true,useDirectDraw = true;

/*------------------------------ Implementation ---------------------------*/

/****************************************************************************
REMARKS:
Handles fatal errors
****************************************************************************/
void initFatalError(void)
{
	MGL_fatalError(MGL_errorMsg(MGL_result()));
}

/****************************************************************************
REMARKS:
Waits for an event and returns the ASCII code of the key that was pressed.
****************************************************************************/
int waitEvent(void)
{
    event_t evt;
    EVT_halt(&evt,EVT_KEYDOWN | EVT_KEYREPEAT | EVT_MOUSEDOWN);
    if (!(evt.what & EVT_MOUSEDOWN))
        return EVT_asciiCode(evt.message);
    return 0;
}

/****************************************************************************
REMARKS:
This function disables any of the display drivers as necessary to
enable and disable DirectDraw or Nucleus.
****************************************************************************/
static void disableDrivers(void)
{
	MGL_enableAllDrivers();
	if (!useVBE)
		MGL_disableDriver(MGL_VBENAME);
	if (!useNucleus)
		MGL_disableDriver(MGL_NUCLEUSNAME);
	if (!useDirectDraw)
		MGL_disableDriver(MGL_DDRAWNAME);
}

void initGraphics(int xRes,int yRes,int bits)
/****************************************************************************
*
* Function:     initGraphics
* Returns:      Pointer to the MGL device context to use for the application
*
* Description:  Initialises the MGL and creates an appropriate display
*               device context to be used by the GUI. This creates and
*               apropriate device context depending on the system being
*               compile for, and should be the only place where system
*               specific code is required.
*
****************************************************************************/
{
	// Initialise the MGL and attempt to set up 640x480x256 graphics as
	// the default video mode.
	if (MGL_init("..\\..\\..\\",NULL)== 0)
		initFatalError();
	disableDrivers();
	if ((mode = MGL_findMode(xRes,yRes,bits)) == -1)
		initFatalError();
}

MGLDC *createDisplayDC(void)
/****************************************************************************
*
* Function:     initGraphics
* Returns:      Pointer to the MGL device context to use for the application
*
* Description:  Initialises the MGL and creates an appropriate display
*               device context to be used by the GUI. This creates and
*               apropriate device context depending on the system being
*               compile for, and should be the only place where system
*               specific code is required.
*
****************************************************************************/
{
    MGLDC   *dc;

	// Start the specified video mode and create the display DC
	if ((dc = MGL_createDisplayDC(mode,MGL_availablePages(mode),MGL_DEFAULT_REFRESH)) == NULL)
        initFatalError();
    MGL_makeCurrentDC(dc);
    if (snowlevel != -1)
		MGL_setPaletteSnowLevel(dc,snowlevel);
	return dc;
}

void runDemo(int xRes,int yRes,int bits)
{
	initGraphics(xRes,yRes,bits);

	// Keep re-starting the application while we recieve the cmRestart
	// command code. This allows the application to change video modes
	// on the fly.
	uint endState = cmRestart;
	while (endState == cmRestart) {
		MGLDevCtx dc(createDisplayDC());
		Demo *demo = new Demo(dc);
		endState = demo->run();
		delete demo;
		}

	MGL_exit();
}

#ifdef __CONSOLE__

#include <ctype.h>
#include "getopt.h"

void help(void)
/****************************************************************************
*
* Function:     help
*
* Description:  Provide usage information about the program.
*
****************************************************************************/
{
	int	mode,xRes,yRes,bits;

	disableDrivers();
	if (MGL_init("..\\..\\..\\",NULL) == 0)
		initFatalError();
	printf("Usage: demo [-novbe -nonuc -ph] [<xRes> <yRes> <bitsPerPixel>]\n\n");
	printf("    -novbe  Don't use VESA VBE driver.\n");
	printf("    -nonuc  Don't use SciTech Nucleus driver.\n");
	printf("    -p<arg> Set the palette snow level factor (defaults to 256).\n");
	printf("    -h      Provide this usage information.\n");
	printf("Press a key for list of video modes.");
	fflush(stdout);
	if (waitEvent() != ASCII_esc) {
		printf("\n\nAvailable modes are:\n");
		for (mode = 0; MGL_modeResolution(mode,&xRes,&yRes,&bits); mode++) {
			printf("  %4d x %4d x %2d %3d page (%s)\n",
				xRes,yRes,bits,
				MGL_availablePages(mode),
				MGL_modeDriverName(mode));
			if (mode != 0 && ((mode % 20) == 0)) {
				printf("Press a key to continue ... ");
				fflush(stdout);
				if (waitEvent() == ASCII_esc)
					break;
				printf("\n");
				}
			}
		}
	MGL_exit();
	exit(1);
}

/****************************************************************************
PARAMETERS:
argc    - Number of command line arguments
argv    - Array of command line arguments

REMARKS:
Parses the command line.
****************************************************************************/
void parseArguments(
	int argc,
	char *argv[])
{
    int     option;
    char    *argument;

    /* Parse command line options */
	do {
		option = getopt(argc,argv,"N:n:P:p:Hh",&argument);
        if (isascii(option))
            option = tolower(option);
        switch (option) {
			case 'n':
				if (stricmp(argument,"ovbe") == 0)
					useVBE = false;
				if (stricmp(argument,"onuc") == 0)
					useNucleus = false;
				break;
            case 'p':
                snowlevel = atoi(argument);
                break;
			case ALLDONE:
			case PARAMETER:
                break;
            case 'h':
            case INVALID:
            default:
                help();
            }
        } while (option != ALLDONE && option != PARAMETER);
}

int main(int argc,char *argv[])
{
	/* Register the ISV license file if desired */
#ifdef	ISV_LICENSE
	GA_registerLicense(OemLicense,false);
#endif
	parseArguments(argc,argv);
	if ((argc - nextargv) >= 3) {
		int xRes = atoi(argv[nextargv]);
		int yRes = atoi(argv[nextargv+1]);
		int bits = atoi(argv[nextargv+2]);
		runDemo(xRes,yRes,bits);
		}
	else
		runDemo(640,480,8);
	return 0;
}

#elif defined(__OS2_PM__)

// TODO: Add code to support OS/2 Presentation Manager apps

#elif defined(__WINDOWS__)
#undef	WINGDIAPI
#undef	APIENTRY
#undef	STRICT
#define	WIN32_LEAN_AND_MEAN
#include <windows.h>
#ifdef  USE_CTL3D
#include <ctl3d.h>
#endif
#include "demo.rh"

int	modeNums[MAX_MODES];    /* List of modes to select from	*/

void CenterWindow(HWND hWndCenter, HWND parent, BOOL repaint)
/****************************************************************************
*
* Function:     CenterWindow
* Parameters:   hWndCenter  - Window to center
*               parent      - Handle for parent window
*               repaint     - True if window should be re-painted
*
* Description:  Centers the specified window within the bounds of the
*               specified parent window. If the parent window is NULL, then
*               we center it using the Desktop window.
*
****************************************************************************/
{
    HWND    hWndParent = (parent ? parent : GetDesktopWindow());
    RECT    RectParent;
    RECT    RectCenter;
    int     CenterX,CenterY,Height,Width;

    GetWindowRect(hWndParent, &RectParent);
    GetWindowRect(hWndCenter, &RectCenter);

    Width = (RectCenter.right - RectCenter.left);
    Height = (RectCenter.bottom - RectCenter.top);
    CenterX = ((RectParent.right - RectParent.left) - Width) / 2;
    CenterY = ((RectParent.bottom - RectParent.top) - Height) / 2;

    if ((CenterX < 0) || (CenterY < 0)) {
        /* The Center Window is smaller than the parent window. */
        if (hWndParent != GetDesktopWindow()) {
            /* If the parent window is not the desktop use the desktop size. */
            CenterX = (GetSystemMetrics(SM_CXSCREEN) - Width) / 2;
            CenterY = (GetSystemMetrics(SM_CYSCREEN) - Height) / 2;
            }
        CenterX = (CenterX < 0) ? 0: CenterX;
        CenterY = (CenterY < 0) ? 0: CenterY;
        }
    else {
        CenterX += RectParent.left;
        CenterY += RectParent.top;
        }

    /* Copy the values into RectCenter */
    RectCenter.left = CenterX;
    RectCenter.right = CenterX + Width;
    RectCenter.top = CenterY;
    RectCenter.bottom = CenterY + Height;

    /* Move the window to the new location */
    MoveWindow(hWndCenter, RectCenter.left, RectCenter.top,
            (RectCenter.right - RectCenter.left),
            (RectCenter.bottom - RectCenter.top), repaint);
}

void RefreshModeList(HWND hwnd)
/****************************************************************************
*
* Function:     RefreshModeList
* Parameters:   hwnd    - Handle to dialog box window
*
* Description:  Refreshes the list of available video modes in the video
*               mode list box given the newly selected pixel depth.
*
****************************************************************************/
{
    char    buf[MAX_STR];
    int     i,modes,x,y,bits,selectBits,mode = grDETECT;
    uchar   *modeList;
	HWND    hwndLst = GetDlgItem(hwnd,IDC_MODELIST);
	ibool	haveMode = false;

    /* Find out if we should use both WinDirect and DirectDraw or just one */
	useDirectDraw = IsDlgButtonChecked(hwnd,IDC_USEDDRAW);
	useWinDirect = IsDlgButtonChecked(hwnd,IDC_USEWDIRECT);
	if (IsDlgButtonChecked(hwnd,IDC_USEBOTH))
		useDirectDraw = useWinDirect = true;

	if (IsDlgButtonChecked(hwnd,IDC_4BIT))          selectBits = 4;
    else if (IsDlgButtonChecked(hwnd,IDC_8BIT))     selectBits = 8;
    else if (IsDlgButtonChecked(hwnd,IDC_15BIT))    selectBits = 15;
    else if (IsDlgButtonChecked(hwnd,IDC_16BIT))    selectBits = 16;
    else if (IsDlgButtonChecked(hwnd,IDC_24BIT))    selectBits = 24;
    else if (IsDlgButtonChecked(hwnd,IDC_32BIT))    selectBits = 32;

    driver = grDETECT;
	MGL_unregisterAllDrivers();
	MGL_registerAllDispDrivers(useLinear,useDirectDraw,useWinDirect);
	MGL_detectGraph(&driver, &mode);

    SendMessage(hwndLst,LB_RESETCONTENT,0,0);
    for (modeList = MGL_availableModes(),i = modes = 0; modeList[i] != 0xFF; i++) {
        if (!MGL_modeResolution(mode = modeList[i],&x,&y,&bits))
            continue;
        if (bits != selectBits)
            continue;
        sprintf(buf,"%s %2d page (%s)",
            MGL_modeName(mode),MGL_availablePages(mode),
            MGL_modeDriverName(mode));
        SendMessage(hwndLst,LB_ADDSTRING,0,(LONG)buf);
        modeNums[modes++] = mode;
        haveMode = true;
        }
    SendMessage(hwndLst,LB_SETCURSEL,0,0);

	if (!haveMode) {
		SendMessage(hwndLst,LB_ADDSTRING,0,(LONG)"No available modes");
		EnableWindow(hwndLst,FALSE);
		}
	else {
		EnableWindow(hwndLst,TRUE);
		}
}

BOOL CALLBACK MainDlgProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
/****************************************************************************
*
* Function:     MainDlgProc
*
* Description:  Dialog procedure for front end dialog box.
*
****************************************************************************/
{
    HWND    hwndLst;
    int     mode;
	char    buf[MAX_STR];
    char    format[MAX_STR];

    switch (msg) {
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDC_CANCEL:
                    EndDialog(hwnd,IDC_CANCEL);
                    break;
                case IDC_MODELIST:
					if (HIWORD(wParam) != LBN_DBLCLK)
                        break;
                case IDC_OK:
                    hwndLst = GetDlgItem(hwnd,IDC_MODELIST);
                    if (SendMessage(hwndLst,LB_GETCOUNT,0,0)) {
                        mode = modeNums[SendMessage(hwndLst,LB_GETCURSEL,0,0)];
                        MGL_setMainWindow(hwnd);
                   		runDemo(mode);
                        }
                    break;
                case IDC_4BIT:
                case IDC_8BIT:
                case IDC_15BIT:
                case IDC_16BIT:
                case IDC_24BIT:
				case IDC_32BIT:
					CheckDlgButton(hwnd,IDC_4BIT,LOWORD(wParam) == IDC_4BIT);
					CheckDlgButton(hwnd,IDC_8BIT,LOWORD(wParam) == IDC_8BIT);
					CheckDlgButton(hwnd,IDC_15BIT,LOWORD(wParam) == IDC_15BIT);
					CheckDlgButton(hwnd,IDC_16BIT,LOWORD(wParam) == IDC_16BIT);
					CheckDlgButton(hwnd,IDC_24BIT,LOWORD(wParam) == IDC_24BIT);
					CheckDlgButton(hwnd,IDC_32BIT,LOWORD(wParam) == IDC_32BIT);
					RefreshModeList(hwnd);
					break;
				case IDC_USEDDRAW:
				case IDC_USEWDIRECT:
				case IDC_USEBOTH:
					CheckDlgButton(hwnd,IDC_USEDDRAW,LOWORD(wParam) == IDC_USEDDRAW);
					CheckDlgButton(hwnd,IDC_USEWDIRECT,LOWORD(wParam) == IDC_USEWDIRECT);
					CheckDlgButton(hwnd,IDC_USEBOTH,LOWORD(wParam) == IDC_USEBOTH);
                    RefreshModeList(hwnd);
					break;
				}
            break;
        case WM_INITDIALOG:
            CenterWindow(hwnd,NULL,FALSE);
			GetDlgItemText(hwnd,IDC_VERSIONSTR,format,sizeof(format));
			sprintf(buf,format,"Win32");
            SetDlgItemText(hwnd,IDC_VERSIONSTR,buf);
			EnableWindow(GetDlgItem(hwnd,IDC_4BIT),FALSE);
            CheckDlgButton(hwnd,IDC_8BIT,TRUE);
			CheckDlgButton(hwnd,IDC_USEBOTH,TRUE);
            RefreshModeList(hwnd);
            return TRUE;
        }
    lParam = lParam;
    return FALSE;
}

ibool HaveWin95(void)
{
    int verMajor = GetVersion() & 0xFF;
    return (verMajor >= 4);
}

int PASCAL WinMain(HINSTANCE hInst,HINSTANCE hPrev,LPSTR szCmdLine,int sw)
{
	/* Register the ISV license file if desired */
#ifdef	ISV_LICENSE
	GA_registerLicense(OemLicense,false);
#endif
	MGL_setAppInstance(hInst);
	DialogBox(hInst,MAKEINTRESOURCE(IDD_MAINDLG),NULL,(DLGPROC)MainDlgProc);
    hPrev = hPrev;
    szCmdLine = szCmdLine;
    sw = sw;
    return 0;
}

#endif
