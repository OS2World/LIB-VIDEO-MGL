/****************************************************************************
*
*			Hello World for the MegaVision Application Framework
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
* Description:	Implementation for the hello program.
*
* $Id$
*
****************************************************************************/

#include "mvis/mvision.hpp"
#include "mvis/mprogram.hpp"
#include "mvis/msgbox.hpp"

const ushort cmAbout = 100;			// Bring up the about box

class HelloApp : public MVProgram {
public:
			// Constructor
			HelloApp(MGLDevCtx& dc);

			// Virtual to handle an event
	virtual	void handleEvent(MVEvent& event,phaseType phase = phFocused);

			// Method to create the menubar
	static	MVMenuBar *initMenuBar(MGLDevCtx& dc,const MVRect& bounds);
	};

// Constructor

HelloApp::HelloApp(MGLDevCtx& dc)
	  : MVProgram(dc, fmHelvetica, ptWindowsPalette),
		MVProgInit(NULL,HelloApp::initMenuBar,MVProgram::initDeskTop)
{
}

MVMenuBar* HelloApp::initMenuBar(MGLDevCtx& dc,const MVRect& bounds)
/****************************************************************************
*
* Function:		HelloApp::initMenuBar
* Parameters:	bounds	- Bounding box for the entire application
* Returns:		Pointer to the newly created menu bar.
*
* Description:	Creates the menu bar definition for the application.
*
****************************************************************************/
{
	MVMenu *fileMenu = new MVMenu(dc);

	*fileMenu
		+ new MVMenuItem("~A~bout this demo...",cmAbout)
		+ new MVMenuItemSeparator()
		+ new MVMenuItem("~Q~uit",cmQuit,MVHotKey(kbX,mdAlt),hcNoContext,"Alt+X");
	fileMenu->doneDefinition();

	MVMenuBar *menuBar = new MVMenuBar(dc,bounds);

	*menuBar
		+ new MVSubMenuItem("~F~ile",fileMenu);
	menuBar->doneDefinition();

	return (MVMenuBar*)MV_validView(menuBar);
}

void HelloApp::handleEvent(MVEvent& event,phaseType phase)
/****************************************************************************
*
* Function:		HelloApp::handleEvent
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
				MV_messageBox(dc,
					"MegaVision GUI Application Frameworks\n"
					"Version " MV_VERSION_STR "\n\n"
					"HelloApp\n\n"
					"Copyright (C) 1996 SciTech Software\n"
					"Written by Kendall Bennett",
					mfInformation | mfOKButton | mfOKDefault | mfCenterText);
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

	// Create our application class and run it to completion
	HelloApp *helloApp = new HelloApp(dc);
	helloApp->run();
	delete helloApp;

	// Shut down the MGL
	MGL_exit();
}
