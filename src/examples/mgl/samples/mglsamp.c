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
* Environment:	Any
*
* Description:  Common framework code for fullscreen sample programs.
*				This simple framework takes care of initialising the
*				MGL in any of the available graphics modes, and provides
*				a common front end for all of the MGL sample programs.
*
*				When compiling for MSDOS this framework provides a simple
*				command line driven and menu interface to allow the
*				selection of graphics modes and to change the operation of
*				the MGL.
*
*				When compiling for Windows this framework provides a simple
*				dialog box driven front end for the sample code allowing
*				the use to choose a display mode and to change the operation
*				of the MGL.
*
*				This module also contains code to compile and link with the
*				SciTech UVBELib device support libraries for both DOS and
*				Windows.
*
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include "mglsamp.h"
#include "pmapi.h"
#include "getopt.h"
#ifdef	ISV_LICENSE
#include "nucleus/graphics.h"
#include "isv.c"
#endif

/*----------------------------- Global Variables --------------------------*/

int             mode;    			/* Selected graphics mode           */
int             palsize;            /* Size of video palette            */
int             aspect;             /* Video mode aspect ratio          */
double          aspectRatio;        /* Aspect ratio of pixels           */
int             err;                /* Error code                       */
color_t         maxcolor;          	/* Maximum color value				*/
color_t         colorMask;          /* Mask for random color generation */
palette_t       *defPal;            /* Pointer to palsette on heap      */
text_settings_t defaultTextSettings;/* Default text settings            */
font_t          *defFont;           /* Default 8x8 bitmap font          */
font_t          *largeFont;         /* Large font for labelling         */
ibool			useVBE = true;
ibool			useNucleus = true;
ibool			useDirectDraw = true;
int             snowlevel = -1;

/* Viewport rectangles used throught program    */

rect_t          fullView;           /* Full screen viewport             */
rect_t          titleView;          /* Title text viewport              */
rect_t          statusView;         /* Status line viewport             */
rect_t          demoView;           /* Viewport to draw demo stuff in   */

/*---------------------------- Implementation -----------------------------*/

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
Checks to see if there are any keyboard or mouse events waiting in the
event queue.
****************************************************************************/
ibool checkEvent(void)
{
    event_t evt;
	EVT_peekNext(&evt,(EVT_KEYDOWN | EVT_KEYREPEAT | EVT_MOUSEDOWN));
	return evt.what != EVT_NULLEVT;
}

/****************************************************************************
REMARKS:
Computes a random color value and returns it. To do this so it works
properly in RGB modes with all pixel formats, we need to generate a random
32 bit number and then mask it to the current pixel format. In some systems
the alpha channel information in 16 bit and 32 bit pixel modes is active,
and hence *must* be set to zero for proper operation (the ATI Mach64 is
one such card).
****************************************************************************/
color_t randomColor(void)
{
    return MGL_randoml(0xFFFFFFFFUL) & colorMask;
}

/****************************************************************************
PARAMETERS:
fmt     - Format string
...     - Standard printf style parameters

RETURNS:
Number of items converted successfully.

REMARKS:
Simple printf style output routine for sending text to the current viewport.
It begins drawing the string at the current CP location, and move the CP
to the start of the next logical line.
****************************************************************************/
int gprintf(
	char *fmt,
	...)
{
    va_list argptr;                 /* Argument list pointer            */
    char    buf[255];               /* Buffer to build sting into       */
    int     cnt;                    /* Result of SPRINTF for return     */
    point_t CP;

    va_start(argptr,fmt);

    cnt = vsprintf(buf,fmt,argptr);
    MGL_getCP(&CP);
	MGL_drawStr(buf);           	/* Display the string               */
    CP.y += MGL_textHeight();       /* Advance to next line             */
    MGL_moveTo(CP);

    va_end(argptr);

    return cnt;                     /* Return the conversion count      */
}

/****************************************************************************
REMARKS:
Draws a border around the currently active viewport. The border is drawn
just inside the bounds of the viewport.
****************************************************************************/
void drawBorder(void)
{
    rect_t  view;

    MGL_getViewport(&view);
    MGL_setViewport(fullView);
    MGL_rect(view);
    MGL_setViewport(view);
}

/****************************************************************************
PARAMETERS:
msg - Message to display in status bar

REMARKS:
Displays a message in the status bar. The status bar is cleared before
the message is displayed.
****************************************************************************/
void statusLine(
	char *msg)
{
    text_settings_t tset;
    rect_t          oldView;

    MGL_getViewport(&oldView);
    MGL_insetRect(statusView,1,1);
    MGL_setViewport(statusView);
    MGL_clearViewport();

    MGL_getTextSettings(&tset);
    MGL_setTextSettings(&defaultTextSettings);
    MGL_setTextJustify(MGL_CENTER_TEXT,MGL_CENTER_TEXT);
    MGL_drawStrXY(MGL_maxx()/2, MGL_maxy()/2, msg);
    MGL_setTextSettings(&tset);

    MGL_insetRect(statusView,-1,-1);
    MGL_setViewport(oldView);
}

/****************************************************************************
REMARKS:
Pause until a key is pressed. If the key is the ESC key, the exit the
program.
****************************************************************************/
ibool pause(void)
{
    static char msg[] = "Esc aborts or press a key...";

    statusLine(msg);        /* Display status message                   */
    if (waitEvent() == ESC)
        return false;
    return true;
}

/****************************************************************************
PARAMETERS:
dc  - Device context

REMARKS:
Fills in the DC with the default attributes, and sets the font to the
default font that we have loaded (the MGL does not provide a default font).
****************************************************************************/
void defaultAttributes(
	MGLDC *dc)
{
    MGL_defaultAttributes(dc);
    MGL_useFont(defFont);
}

/****************************************************************************
PARAMETERS:
dc  - Device context
heading - Text describing the demo

REMARKS:
Displays a heading for the demo, sets the viewport for the demo code and
draws a border around the viewport. We also restore the default attributes.
****************************************************************************/
void mainWindow(
	MGLDC *dc,
	char *heading)
{
    text_settings_t tset;
    rect_t          r;

    MS_hide();
    defaultAttributes(dc);      /* Restore default attributes       */
    MGL_clearDevice();          /* Clear the graphics screen        */

    MGL_setViewport(titleView);

	if (dc->mi.bitsPerPixel == 8)
		MGL_setColor(255);
    MGL_getTextSettings(&tset);
    MGL_setTextSettings(&defaultTextSettings);
    MGL_setTextJustify(MGL_CENTER_TEXT,MGL_CENTER_TEXT);
    MGL_drawStrXY(MGL_maxx()/2, MGL_maxy()/2, heading);
    MGL_setTextSettings(&tset);

    MGL_setViewport(statusView);
    drawBorder();
    MGL_setViewport(demoView);
    drawBorder();

    r = demoView;
    MGL_insetRect(r,1,1);
    MGL_setViewport(r);
    MS_show();
}

/****************************************************************************
REMARKS:
Handles a fatal error condition during initialisation.
****************************************************************************/
static void initFatalError(void)
{
    char    buf[80];
    sprintf(buf,"Graphics error: %s\n",MGL_errorMsg(MGL_result()));
    MGL_fatalError(buf);
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

/****************************************************************************
PARAMETERS:
xRes	- X resolution for the mode to initialise
yRes    - Y resolution for the mode to initialise
bits    - color depth for the mode to initialise

RETURNS:
Pointer to display device context

REMARKS:
Attempts to initialise the graphics system in the specified manner. Reports
any intialisation errors if they occur.
****************************************************************************/
static MGLDC *startGraphics(
	int xRes,
	int yRes,
	int bits)
{
	int     height,numPages;
	MGLDC   *dc;

	/* Start fullscreen graphics in selected mode */
	if (MGL_init("..\\..\\..\\",NULL) == 0)
		initFatalError();
	disableDrivers();
	if ((mode = MGL_findMode(xRes,yRes,bits)) == -1) {
		/* If we are starting 640x480x8 (the default mode) and it is not
		 * found, then select the first display mode available (always
		 * mode 0).
		 */
		if (xRes == 640 && yRes == 480 && bits == 8)
			mode = 0;
		else
			initFatalError();
		}
	numPages = MIN(MGL_availablePages(mode),2);
	if ((dc = MGL_createDisplayDC(mode,numPages,MGL_DEFAULT_REFRESH)) == NULL)
		initFatalError();
	MGL_makeCurrentDC(dc);

	/* Load the default 8x8 bitmap font into device context */
	if ((defFont = MGL_loadFont("pc8x8.fnt")) == NULL)
		initFatalError();
	MGL_useFont(defFont);

    /* Change default startup options from command line */
    if (snowlevel != -1)
        MGL_setPaletteSnowLevel(dc,snowlevel);

    /* Load a large font for labelling */
    largeFont = MGL_loadFont("romant.fnt");
    if ((err = MGL_result()) != grOK) {
        MGL_exit();
        exit(1);
        }

    /* Obtain a few useful constant */
	maxcolor = MGL_maxColor(dc);
	aspect = MGL_getAspectRatio();/* Aspect ratio for display mode  */
	aspectRatio = 1000.0 / aspect;
	MGL_getTextSettings(&defaultTextSettings);
	if (MGL_getBitsPerPixel(dc) <= 8)
		colorMask = MGL_maxColor(dc);
	else
		colorMask = MGL_packColor(&dc->pf,0xFF,0xFF,0xFF);

	/* Allocate space for the palette, and read the default palette     */
    if ((palsize = MGL_getPaletteSize(dc)) != -1) {
        defPal = malloc(palsize * sizeof(palette_t));
        if (defPal == NULL)
            MGL_fatalError("Out of memory!\n");
        MGL_getPalette(dc,defPal,palsize,0);
        }
	else
		defPal = NULL;

    /* Determine the size of the viewports required */
    MGL_getViewport(&fullView);
    height = MGL_textHeight();  /* Height of default font           */
    titleView = fullView;
    titleView.bottom = titleView.top + height + 5;
    statusView = fullView;
    statusView.top = statusView.bottom - height - 5;
    demoView = fullView;
    demoView.top = titleView.bottom;
    demoView.bottom = statusView.top+1;
	return dc;
}

/****************************************************************************
PARAMETERS:
xRes	- X resolution for the mode to run demo in
yRes    - Y resolution for the mode to run demo in
bits    - color depth for the mode to run demo in

REMARKS:
Initialise the graphics mode, runs the demo and then exits the MGL.
****************************************************************************/
static void runDemo(
	int xRes,
	int yRes,
	int bits)

{
	/* Initialise graphics system */
	MGLDC *dc = startGraphics(xRes,yRes,bits);

	/* Run the demo */
	demo(dc);

	/* Close graphics system and restore original display */
	MGL_exit();
}

#ifdef __CONSOLE__

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
	printf("Usage: %s [-novbe -nonuc -ph] [<xRes> <yRes> <bitsPerPixel>]\n\n", demoName);
	printf("    -novbe  Don't use VESA VBE driver.\n");
	printf("    -nonuc  Don't use SciTech Nucleus driver.\n");
	printf("    -p<arg> Set the palette snow level factor (defaults to 256).\n");
	printf("    -h      Provide this usage information.\n\n");
	printf("Press a key for list of video modes.");
	fflush(stdout);
	if (waitEvent() != ESC) {
		printf("\n\nAvailable modes are:\n");
		for (mode = 0; MGL_modeResolution(mode,&xRes,&yRes,&bits); mode++) {
			printf("  %4d x %4d x %2d %3d page (%s)\n",
				xRes,yRes,bits,
				MGL_availablePages(mode),
				MGL_modeDriverName(mode));
			if (mode != 0 && ((mode % 20) == 0)) {
				printf("Press a key to continue ... ");
				fflush(stdout);
				if (waitEvent() == ESC)
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

/****************************************************************************
PARAMETERS:
argc    - Number of command line arguments
argv    - Array of command line arguments

REMARKS:
Main console mode program entry point.
****************************************************************************/
int main(
	int argc,
	char *argv[])
{
	/* Register the ISV license file if desired */
#ifdef	ISV_LICENSE
	GA_registerLicense(OemLicense,false);
#endif

	/* Parse command line arguments */
    parseArguments(argc,argv);

	/* Run the demo */
	if (nextargv >= argc)
		runDemo(640,480,8);
	else if (nextargv <= argc-3)
		runDemo(atoi(argv[nextargv]),atoi(argv[nextargv+1]),atoi(argv[nextargv+2]));
	printf("Type '%s -h' for a list of available modes\n", demoName);
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
#include "mglsamp.rh"

static short   	modeNums[MAX_MODES];    /* List of modes to select 	*/

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

BOOL WINAPI MainDlgProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
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
			sprintf(buf,format,"Win32",demoName);
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
