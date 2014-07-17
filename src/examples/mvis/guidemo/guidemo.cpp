/****************************************************************************
*
*						GUI Demonstration Program
*
*			A C++ GUI Toolkit for the MegaGraph Graphics Library
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
* Filename:		$RCSfile$
*
* Language:		C++ 3.0
* Environment:	any
*
* Description:	Implementation for the GUI demonstration program.
*
* $Id$
*
****************************************************************************/

#include "guidemo.hpp"

/*---------------------------- Global Variables ---------------------------*/

int			driver = grDETECT,mode = grDETECT;	// Graphics driver and mode
GUIDemoApp	*app;								// Application object

/*------------------------------ Implementation ---------------------------*/

void SimpleWindow::handleEvent(MVEvent& event,phaseType phase)
/****************************************************************************
*
* Function:		SimpleWindow::handleEvent
* Parameters:	event	- Event to handle
*				phase	- Current phase for the event (pre,focus,post)
*
* Description:	Event handling routine for the SimpleWindow class.
*
****************************************************************************/
{
	MVWindow::handleEvent(event,phase);

	if (event.what == evCommand) {
		switch (event.message.command) {
			case cmClose:
				if (event.which == this)
					MV_message(app,evCommand,cmCloseWindow,this);
				break;
			}
		}
}

// Constructor

GUIDemoApp::GUIDemoApp(MGLDevCtx& dc)
	  : MVProgram(dc, fmHelvetica, ptWindowsPalette),
		MVProgInit(NULL,GUIDemoApp::initMenuBar,MVProgram::initDeskTop)
{
	wFixed = NULL;
	wMove = NULL;
	wResize = NULL;
}

MVMenuBar* GUIDemoApp::initMenuBar(MGLDevCtx& dc,const MVRect& bounds)
/****************************************************************************
*
* Function:		GUIDemoApp::initMenuBar
* Parameters:	bounds	- Bounding box for the entire application
* Returns:		Pointer to the newly created menu bar.
*
* Description:	Creates the menu bar definition for the application.
*
****************************************************************************/
{
	MVMenu *fileMenu = new MVMenu(dc);
	MVMenu *msgMenu = new MVMenu(dc);
	MVMenu *dlgMenu = new MVMenu(dc);
	MVMenu *windowMenu = new MVMenu(dc);
	MVMenu *optionsMenu = new MVMenu(dc);

	*fileMenu
		+ new MVMenuItem("~A~bout this demo...",cmAbout)
		+ new MVMenuItemSeparator()
		+ new MVMenuItem("~L~oad file...",cmLoadFile,
			MVHotKey(kbF3),hcNoContext,"F3")
		+ new MVMenuItem("~B~rowse file...",cmBrowseFile,
			MVHotKey(kbF3,mdAlt),hcNoContext,"Alt-F3")
		+ new MVMenuItem("~S~ave file...",cmSaveFile,
			MVHotKey(kbF2),hcNoContext,"F2")
		+ new MVMenuItemSeparator()
		+ new MVMenuItem("Disabled Item",-1)
		+ new MVMenuItemSeparator()
		+ new MVMenuItem("~Q~uit",cmQuit,MVHotKey(kbX,mdAlt),hcNoContext,"Alt+X");
	fileMenu->doneDefinition();
	fileMenu->disableItem(6);

	*msgMenu
		+ new MVMenuItem("No Icon Message Box...",cmMsgNoIcon)
		+ new MVMenuItem("Warning Message Box...",cmMsgWarning)
		+ new MVMenuItem("Error Message Box...",cmMsgError)
		+ new MVMenuItem("Information Message Box...",cmMsgInform)
		+ new MVMenuItem("Confirm Message Box...",cmMsgConfirm);
	msgMenu->doneDefinition();

	*dlgMenu
		+ new MVMenuItem("Radio buttons...",cmDlgRadio)
		+ new MVMenuItem("Check boxes...",cmDlgCheckBox)
		+ new MVMenuItem("Bitmap buttons...",cmDlgButtons);
	dlgMenu->doneDefinition();

	*windowMenu
		+ new MVMenuItem("Fixed window...",cmWinFixed)
		+ new MVMenuItem("Moveable window...",cmWinMove)
		+ new MVMenuItem("Moveable, resizeable window...",cmWinResize);
	windowMenu->doneDefinition();

	*optionsMenu
		+ new MVMenuItem("~V~ideo mode...",cmVideoMode,
			MVHotKey(kbF9,mdCtrl),hcNoContext,"Ctrl-F9");
    optionsMenu->doneDefinition();

	MVMenuBar *menuBar = new MVMenuBar(dc,bounds);

	*menuBar
		+ new MVSubMenuItem("~F~ile",fileMenu)
		+ new MVSubMenuItem("~M~essages",msgMenu)
		+ new MVSubMenuItem("~D~ialogs",dlgMenu)
		+ new MVSubMenuItem("~W~indows",windowMenu)
		+ new MVSubMenuItem("~O~ptions",optionsMenu);
	menuBar->doneDefinition();

	return (MVMenuBar*)MV_validView(menuBar);
}

void GUIDemoApp::loadFile()
/****************************************************************************
*
* Function:     GUIDemoApp::loadFile
*
* Description:  Method to load a new 3D object file from disk. We pop up
*               a standard file dialog box to obtain the file to display.
*
****************************************************************************/
{
	MVFileDialog *d = new MVFileDialog(dc,"*.*","Load File","File to load:",
		fdOpenDialog & ~fdHelpButton);
	if (MVProgram::deskTop->execView(d) == cmFileOpen) {
		char buf[255];
		sprintf(buf,"You selected the file:\n\n    %s",d->getFilename());
		MV_messageBox(dc,buf,mfInformation | mfOKButton | mfOKDefault);
		}
	delete d;
}

void GUIDemoApp::browseFile()
/****************************************************************************
*
* Function:     GUIDemoApp::browseFile
*
* Description:  Method to load a new 3D object file from disk. We pop up
*               a standard file dialog box to obtain the file to display.
*
****************************************************************************/
{
	MVFileDialog *d = new MVFileDialog(dc,"*.*","Browse File","File to load (must exist):",
		fdBrowseDialog & ~fdHelpButton);
	if (MVProgram::deskTop->execView(d) == cmFileOpen) {
		char buf[255];
		sprintf(buf,"You selected the file:\n\n    %s",d->getFilename());
		MV_messageBox(dc,buf,mfInformation | mfOKButton | mfOKDefault);
		}
	delete d;
}

void GUIDemoApp::saveFile()
/****************************************************************************
*
* Function:     GUIDemoApp::saveFile
*
* Description:  Method to load a new 3D object file from disk. We pop up
*               a standard file dialog box to obtain the file to display.
*
****************************************************************************/
{
	MVFileDialog *d = new MVFileDialog(dc,"*.*","Save File","File to save as:",
		fdSaveAsDialog & ~fdHelpButton);
	if (MVProgram::deskTop->execView(d) == cmFileSave) {
		char buf[255];
		sprintf(buf,"You selected the file:\n\n    %s",d->getFilename());
		MV_messageBox(dc,buf,mfInformation | mfOKButton | mfOKDefault);
		}
	delete d;
}

// Display various types of message boxes

void GUIDemoApp::msgNoIcon()
{
	MV_messageBoxEx(dc,
		"This message box has no icon!",
		"No Icon Message Box",
		mfOKCancel);
}

void GUIDemoApp::msgWarning()
{
	MV_messageBoxEx(dc,
		"This is a warning message box!",
		"Warning Message Box",
		mfWarning | mfOKCancel | mfCancelDefault);
}

void GUIDemoApp::msgError()
{
	MV_messageBoxEx(dc,
		"This is an error message box!",
		"Error Message Box",
		mfError | mfYesNoCancel);
}

void GUIDemoApp::msgInform()
{
	MV_messageBoxEx(dc,
		"This is an information message box!",
		"Information Message Box",
		mfInformation | mfOKButton | mfOKDefault);
}

void GUIDemoApp::msgConfirm()
{
	MV_messageBoxEx(dc,
		"This is an information message box!",
		"Information Message Box",
		mfConfirmation | mfYesNo | mfNoDefault);
}

// Display various types of dialog box buttons

class DlgRadioButtons : public MVDialog {
public:
			// Constructor
			DlgRadioButtons(MGLDevCtx& dc);
	};

DlgRadioButtons::DlgRadioButtons(MGLDevCtx& dc)
	: MVDialog(dc,MVRect(0,0,250,200),"Radio Buttons"),
	  MVWindowInit(MVWindow::initFrame,MVWindow::initTitleBar)
{
	options |= ofCentered;

	MVRect r(getClientRect());
	r.offset(5,5);
	r.right() = r.left() + 90;
	r.bottom() = r.top() + 16;
	insert(new MVRadioButton(dc,r,-1,bfNormal,true));

	r.offset(0,20);
	insert(new MVRadioButton(dc,r,-1,bfNormal));

	r.offset(0,20);
	insert(new MVRadioButton(dc,r,-1,bfNormal));
}

void GUIDemoApp::dlgRadio()
{
	DlgRadioButtons *d = new DlgRadioButtons(dc);
	MVProgram::deskTop->execView(d);
	delete d;
}

class DlgCheckBoxes : public MVDialog {
public:
			// Constructor
			DlgCheckBoxes(MGLDevCtx& dc);
	};

DlgCheckBoxes::DlgCheckBoxes(MGLDevCtx& dc)
	: MVDialog(dc,MVRect(0,0,250,200),"Check Boxes"),
	  MVWindowInit(MVWindow::initFrame,MVWindow::initTitleBar)
{
	options |= ofCentered;

	MVRect r(getClientRect());
	r.offset(5,5);
	r.right() = r.left() + 90;
	r.bottom() = r.top() + 16;
	insert(new MVCheckBox(dc,r,-1,bfNormal,true));

	r.offset(0,20);
	insert(new MVCheckBox(dc,r,-1,bfNormal));

	r.offset(0,20);
	insert(new MVCheckBox(dc,r,-1,bfNormal));
}

void GUIDemoApp::dlgCheckBox()
{
	DlgCheckBoxes *d = new DlgCheckBoxes(dc);
	MVProgram::deskTop->execView(d);
	delete d;
}

void GUIDemoApp::dlgButtons()
{
}

// Display various types of simple windows

void GUIDemoApp::winFixed()
{
	if (wFixed) {
		MVProgram::deskTop->remove(wFixed);
		delete wFixed;
		}
	wFixed = new SimpleWindow(dc,MVRect(20,20,200,150),"Fixed Window",wfFixedWin);
	MVProgram::deskTop->insert(wFixed);
	MVProgram::deskTop->select(wFixed);
}

void GUIDemoApp::winMove()
{
	if (wMove) {
		MVProgram::deskTop->remove(wMove);
		delete wMove;
		}
	wMove = new SimpleWindow(dc,MVRect(20,20,220,150),"Moveable Window",wfMoveableWin);
	MVProgram::deskTop->insert(wMove);
	MVProgram::deskTop->select(wMove);
}

void GUIDemoApp::winResize()
{
	if (wResize) {
		MVProgram::deskTop->remove(wResize);
		delete wResize;
		}
	wResize = new SimpleWindow(dc,MVRect(20,20,250,150),"Resizeable Window",wfNormal);
	MVProgram::deskTop->insert(wResize);
	MVProgram::deskTop->select(wResize);
}

void GUIDemoApp::changeVideoMode()
/****************************************************************************
*
* Function:     GUIDemoApp::changeVideoMode
*
* Description:  Pops up a dialog box to allow the user to change the
*               current video mode, and changes to the new mode if
*               requested.
*
****************************************************************************/
{
    int newMode;

    MVModeSelector *d = new MVModeSelector(dc,"Change Video Mode",driver,
        mode,MGL_availableModes());
    if (deskTop->execView(d) == cmOk) {
        // Change to the newly selected video mode
        //
        // Post a cmRestart event to restart the application with the new
        // video mode.

        if ((newMode = d->getMode()) != mode) {
            MVEvent event;
            mode = newMode;
            event.what = evCommand;
            event.message.command = cmRestart;
            event.message.infoPtr = NULL;
            putEvent(event);
            }
        }
    delete d;
}

void GUIDemoApp::handleEvent(MVEvent& event,phaseType phase)
/****************************************************************************
*
* Function:		GUIDemoApp::handleEvent
* Parameters:	event	- Event to handle
*				phase	- Phase of focused events
*
* Description:	Main event handling routine for the application. We first
*				let the MVProgram class take care of the events for us, then
*				we check for application specific commands.
*
****************************************************************************/
{
	MVProgram::handleEvent(event,phase);

	// Now check for application commands and dispatch them
	if (event.what == evCommand) {
		switch (event.message.command) {
			case cmAbout:
				MV_messageBoxEx(dc,
					"MegaVision GUI Application Frameworks\n"
					"Version " MV_VERSION_STR "\n\n"
					"GUI Demonstration\n\n"
					"Copyright (C) 1996 SciTech Software\n"
					"Written by Kendall Bennett",
					"About this demo",
					mfInformation | mfOKButton | mfOKDefault | mfCenterText);
				break;
			case cmLoadFile:
				loadFile();
				break;
			case cmBrowseFile:
				browseFile();
				break;
			case cmSaveFile:
				saveFile();
				break;
			case cmMsgNoIcon:
				msgNoIcon();
				break;
			case cmMsgWarning:
				msgWarning();
				break;
			case cmMsgError:
				msgError();
				break;
			case cmMsgInform:
				msgInform();
				break;
			case cmMsgConfirm:
				msgConfirm();
				break;
			case cmDlgRadio:
				dlgRadio();
				break;
			case cmDlgCheckBox:
				dlgCheckBox();
				break;
			case cmDlgButtons:
				dlgButtons();
				break;
			case cmWinFixed:
				winFixed();
				break;
			case cmWinMove:
				winMove();
				break;
			case cmWinResize:
				winResize();
				break;
			case cmVideoMode:
                changeVideoMode();
				break;
			case cmCloseWindow:
				// Close one of the windows on the desktop
				if ((SimpleWindow*)event.message.infoPtr == wFixed) {
					MVProgram::deskTop->remove(wFixed);
					delete wFixed;
					wFixed = NULL;
					}
				else if ((SimpleWindow*)event.message.infoPtr == wMove) {
					MVProgram::deskTop->remove(wMove);
					delete wMove;
					wMove = NULL;
					}
				else if ((SimpleWindow*)event.message.infoPtr == wResize) {
					MVProgram::deskTop->remove(wResize);
					delete wResize;
					wResize = NULL;
					}
				break;
			default:
				return;					// Don't clear unhandled events
			}
		clearEvent(event);
		}
}

void initFatalError(void)
{
	char msg[80];
	sprintf(msg, "Graphics error: %s", MGL_errorMsg(MGL_result()));
	MGL_fatalError(msg);
}

MGLDC *initGraphics(void)
/****************************************************************************
*
* Function:		initGraphics
* Returns:		Pointer to the MGL device context to use for the application
*
* Description:	Initialises the MGL and creates an appropriate display
*				device context to be used by the GUI. This creates and
*				apropriate device context depending on the system being
*				compile for, and should be the only place where system
*				specific code is required.
*
****************************************************************************/
{
	MGLDC	*dc;

	// Register all display drivers
	MGL_registerAllDispDrivers(true,true,true);

	// Start the MGL and create a display device context
	if (driver == grDETECT) {
        MGL_detectGraph(&driver,&mode);
		uchar *modeList = MGL_availableModes();
		for (int i = 0; modeList[i] != 0xFF; i++) {
            if (modeList[i] == grSVGA_640x480x256) {
                mode = grSVGA_640x480x256;
                break;
                }
            }
        }
	if (!MGL_init(&driver,&mode,""))
		initFatalError();
	if ((dc = MGL_createDisplayDC(1)) == NULL)
		initFatalError();
	return dc;
}

void main(void)
{
	// Keep re-starting the application while we recieve the cmRestart
    // command code. This allows the application to change video modes
    // on the fly.
	ushort endState = cmRestart;

	while (endState == cmRestart) {
		// Initialize the MGL and create an MGL device context
		MGLDevCtx dc(initGraphics());

		// Create our application class and run it to completion
		app = new GUIDemoApp(dc);
		endState = app->run();
		delete app;

		// Shut down the MGL
		MGL_exit();
		}
}
