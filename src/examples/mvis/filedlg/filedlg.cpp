/****************************************************************************
*
*					Sample File Browser Dialog Program
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
* Description:	Implementation for the file browser program.
*
* $Id$
*
****************************************************************************/

#include "mvis/mvision.hpp"
#include "mvis/mprogram.hpp"
#include "mvis/msgbox.hpp"
#include "mvis/mfiledlg.hpp"

const ushort cmAbout 		= 100;		// Bring up the about box
const ushort cmLoadFile 	= 101;		// Bring up a load file dialog box
const ushort cmBrowseFile 	= 102;		// Bring up a load file dialog box
const ushort cmSaveFile 	= 103;		// Bring up a save file dialog box

class FileDlgApp : public MVProgram {
public:
			// Constructor
			FileDlgApp(MGLDevCtx& dc);

			// Virtual to handle an event
	virtual	void handleEvent(MVEvent& event,phaseType phase = phFocused);

			// Display the file dialog boxes
			void loadFile();
			void browseFile();
			void saveFile();

			// Method to create the menubar
	static	MVMenuBar *initMenuBar(MGLDevCtx& dc,const MVRect& bounds);
	};

// Constructor

FileDlgApp::FileDlgApp(MGLDevCtx& dc)
	  : MVProgram(dc, fmHelvetica, ptWindowsPalette),
		MVProgInit(NULL,FileDlgApp::initMenuBar,MVProgram::initDeskTop)
{
}

MVMenuBar* FileDlgApp::initMenuBar(MGLDevCtx& dc,const MVRect& bounds)
/****************************************************************************
*
* Function:		FileDlgApp::initMenuBar
* Parameters:	bounds	- Bounding box for the entire FileDlgApp
* Returns:		Pointer to the newly created menu bar.
*
* Description:	Creates the menu bar definition for the FileDlgApp.
*
****************************************************************************/
{
	MVMenu *fileMenu = new MVMenu(dc);

	*fileMenu
		+ new MVMenuItem("~A~bout this demo...",cmAbout)
		+ new MVMenuItem("Load file...",cmLoadFile)
		+ new MVMenuItem("Browse file...",cmBrowseFile)
		+ new MVMenuItem("Save file...",cmSaveFile)
		+ new MVMenuItemSeparator()
		+ new MVMenuItem("~Q~uit",cmQuit,MVHotKey(kbX,mdAlt),hcNoContext,"Alt+X");
	fileMenu->doneDefinition();

	MVMenuBar *menuBar = new MVMenuBar(dc,bounds);

	*menuBar
		+ new MVSubMenuItem("~F~ile",fileMenu);
	menuBar->doneDefinition();

	return (MVMenuBar*)MV_validView(menuBar);
}

void FileDlgApp::loadFile()
/****************************************************************************
*
* Function:     FileDlgApp::loadFile
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

void FileDlgApp::browseFile()
/****************************************************************************
*
* Function:     FileDlgApp::browseFile
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

void FileDlgApp::saveFile()
/****************************************************************************
*
* Function:     FileDlgApp::saveFile
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

void FileDlgApp::handleEvent(MVEvent& event,phaseType phase)
/****************************************************************************
*
* Function:		FileDlgApp::handleEvent
* Parameters:	event	- Event to handle
*				phase	- Phase of focused events
*
* Description:	Main event handling routine for the FileDlgApp. We first
*				let the MVProgram class take care of the events for us, then
*				we check for FileDlgApp specific commands.
*
****************************************************************************/
{
	MVProgram::handleEvent(event,phase);

	// Now check for FileDlgApp commands and dispatch them
	if (event.what == evCommand) {
		switch (event.message.command) {
			case cmAbout:
				MV_messageBox(dc,
					"MegaVision GUI FileDlgApp Frameworks\n"
					"Version " MV_VERSION_STR "\n\n"
					"FileDlgApp\n\n"
					"Copyright (C) 1996 SciTech Software\n"
					"Written by Kendall Bennett",
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
* Returns:		Pointer to the MGL device context to use for the FileDlgApp
*
* Description:	Initialises the MGL and creates an appropriate display
*				device context to be used by the GUI. This creates and
*				apropriate device context depending on the system being
*				compile for, and should be the only place where system
*				specific code is required.
*
****************************************************************************/
{
	int		driver,mode;
	MGLDC	*dc;

	// Register only the SuperVGA 256 color drivers
	MGL_registerDriver(MGL_SVGA8NAME,SVGA8_driver);

	// Start the MGL and create a display device context
	driver = grDETECT;
	mode = grSVGA_640x480x256;
	if (!MGL_init(&driver,&mode,""))
		initFatalError();
	if ((dc = MGL_createDisplayDC(1)) == NULL)
		initFatalError();
	return dc;
}

void main(void)
{
	// Initialize the MGL and create an MGL device context
	MGLDevCtx dc(initGraphics());

	// Create our Application class and run it to completion
	FileDlgApp *fileDlgApp = new FileDlgApp(dc);
	fileDlgApp->run();
	delete fileDlgApp;

	// Shut down the MGL
	MGL_exit();
}
