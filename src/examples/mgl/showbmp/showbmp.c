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
* Environment:  Full screen environment
*
* Description:  Program to demonstrate loading bitmaps of any size and
*               color depth directly into the display DC. Automatic color
*               conversion is done on the bitmap to convert it to the
*               current display DC's pixel format.
*
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "mgraph.h"
#include "pmapi.h"
#ifndef	_MAX_PATH
#define	_MAX_PATH	255
#endif

#define PROG_NAME   "showbmp"

/*---------------------------- Global Variables ---------------------------*/

static int    	driver = grDETECT;
static ibool  	useLinear = true;
static ibool	useDirectDraw = true;
static ibool	useWinDirect = true;

/*------------------------------ Implementation ---------------------------*/

static void initFatalError(void)
{
    char    buf[80];
    sprintf(buf,"Graphics error: %s\n",MGL_errorMsg(MGL_result()));
    MGL_fatalError(buf);
}

MGLDC *initGraphics(int modeNum)
/****************************************************************************
*
* Function:     initGraphics
* Parameters:   modeNum - Video mode number to start
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

    /* Start the MGL and create a display device context */
	MGL_unregisterAllDrivers();
	MGL_registerAllDispDrivers(useLinear,useDirectDraw,useWinDirect);
    MGL_registerAllMemDrivers();
    if (!MGL_init(&driver,&modeNum,"..\\..\\..\\"))
        initFatalError();
    if ((dc = MGL_createDisplayDC(1)) == NULL)
        initFatalError();
    MGL_makeCurrentDC(dc);
    return dc;
}

void waitEvent(void)
/****************************************************************************
*
* Function:     waitEvent
*
* Description:  Waits for either a keyboard or mouse event before returning.
*
****************************************************************************/
{
    event_t evt;
    EVT_halt(&evt,EVT_KEYDOWN | EVT_KEYREPEAT | EVT_MOUSEDOWN);
}

void showBitmap(int mode,char *bitmapName)
{
    MGLDC       *dc;

    /* Initialise the graphics mode */
    dc = initGraphics(mode);

    /* Load the bitmap into the display DC. In 8 bit modes the palette is
     * automatically handled by this routine for display DC's.
     */
	if (!MGL_loadBitmapIntoDC(dc,bitmapName,0,0,true))
		initFatalError();
	waitEvent();

	MGL_exit();
}

#ifdef __CONSOLE__

#include "getopt.h"

void help(char *progName)
/****************************************************************************
*
* Function:     help
* Parameters:   progName    - Name of demo program
*
* Description:  Provide usage information about the program.
*
****************************************************************************/
{
    int     i,mode = grDETECT;
    uchar   *modeList;

	MGL_unregisterAllDrivers();
	MGL_registerAllDispDrivers(useLinear,useDirectDraw,useWinDirect);
	MGL_detectGraph(&driver, &mode);
	printf("Usage: %s [-VGA -VESA -bh] <video mode>\n", progName);
	printf("  -VGA    Use standard VGA driver\n");
	printf("  -VBE    Use unaccelerated VESA VBE driver\n");
	printf("  -b      Use only banked video modes (disable linear framebuffer)\n");
	printf("  -h      Provide this usage information.\n\n");
	printf("Press a key for list of video modes.");
	PM_getch();
	printf("\n\nAvailable modes are:\n");
	for (modeList = MGL_availableModes(), i = 0; modeList[i] != 0xFF; i++) {
		printf("    %2d - %-26s %2d page (%s)\n",
			modeList[i],MGL_modeName(modeList[i]),
			MGL_availablePages(modeList[i]),
			MGL_modeDriverName(modeList[i]));
		}
	printf("\nVideo Card: %s\n",MGL_driverName(driver));
	exit(1);
}

void parseArguments(int argc,char *argv[],char *progName)
/****************************************************************************
*
* Function:     parseArguments
* Parameters:   argc,argv   - Command line arguments from main()
*
* Description:  Parse the initial command line before starting the demo.
*
****************************************************************************/
{
    int     option;
    char    *argument;

    /* Parse command line options */
    do {
		option = getopt(argc,argv,"V:bpl",&argument);
		switch (option) {
			case 'V':
				if (tolower(argument[0]) == 'g')
					driver = grVGA;
				else if (tolower(argument[0]) == 'b')
					driver = grVESA;
				break;
            case 'b':
                useLinear = false;
                break;
			case INVALID:
				help(progName);
			}
		} while (option != ALLDONE && option != PARAMETER);
}

int main(int argc,char *argv[])
{
	char  	bitmapName[_MAX_PATH];

	/* Parse command line */
	parseArguments(argc,argv,PROG_NAME);

	/* Initialise desired graphics mode */
	if (nextargv >= argc)
		help(PROG_NAME);
	printf("Bitmap to load: ");
	gets(bitmapName);
	showBitmap(atoi(argv[nextargv]),bitmapName);
	return 0;
}

#elif defined(__OS2_PM__)

// TODO: Add code to support OS/2 Presentation Manager!

#elif   defined(__WINDOWS__)
#undef	WINGDIAPI
#undef	APIENTRY
#undef	STRICT
#define	WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commdlg.h>
#include "showbmp.rh"

static short   modeNums[MAX_MODES];    /* List of modes to select from     */
static char    szAppFilter[] = "Bitmaps\0*.bmp;*.dib\0";
static char    bitmapName[_MAX_PATH] = "";

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

ibool OpenBitmapFile(HWND hwnd)
/****************************************************************************
*
* Function:     OpenBitmapFile
* Parameters:   hwnd    - Handle to parent window
* Returns:      True for valid bitmap name, false otherwise
*
* Description:  Open the bitmap file and load it into the memory DC.
*
****************************************************************************/
{
    OPENFILENAME    ofn;

    bitmapName[0] = 0;

    /* Prompt user for file to open */
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.hInstance = NULL;
    ofn.lpstrFilter = szAppFilter;
    ofn.lpstrCustomFilter = NULL;
    ofn.nMaxCustFilter = 0;
    ofn.nFilterIndex = 0;
    ofn.lpstrFile = bitmapName;
    ofn.nMaxFile = sizeof(bitmapName);
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = NULL;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
    ofn.nFileOffset = 0;
    ofn.nFileExtension = 0;
    ofn.lpstrDefExt = NULL;
    ofn.lCustData = 0;
    ofn.lpfnHook = NULL;
    ofn.lpTemplateName = NULL;
    return GetOpenFileName(&ofn);
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
                case IDC_SELECT_BITMAP:
                    OpenBitmapFile(hwnd);
                    break;
                case IDC_MODELIST:
					if (HIWORD(wParam) != LBN_DBLCLK)
                        break;
                case IDC_OK:
                    hwndLst = GetDlgItem(hwnd,IDC_MODELIST);
                    if (SendMessage(hwndLst,LB_GETCOUNT,0,0)) {
                        if (bitmapName[0] == 0 && !OpenBitmapFile(hwnd))
                            break;
                        mode = modeNums[SendMessage(hwndLst,LB_GETCURSEL,0,0)];
                        MGL_setMainWindow(hwnd);
                        showBitmap(mode,bitmapName);
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
	MGL_setAppInstance(hInst);
	DialogBox(hInst,MAKEINTRESOURCE(IDD_MAINDLG),NULL,(DLGPROC)MainDlgProc);
	hPrev = hPrev;
    szCmdLine = szCmdLine;
    sw = sw;
    return 0;
}

#endif
