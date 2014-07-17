/****************************************************************************
*
*					Sample Mode Selector Dialog Program
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
* Description:	Implementation for the modes dialog program.
*
* $Id$
*
****************************************************************************/

#include "mvis/mvision.hpp"
#include "mvis/mprogram.hpp"
#include "mvis/msgbox.hpp"
#include "mvis/mmodesel.hpp"

const ushort cmAbout 		= 100;		// Bring up the about box
const ushort cmVideoMode 	= 101;		// Change video modes

class ModeDlgApp : public MVProgram {
public:
			// Constructor
			ModeDlgApp(MGLDevCtx& dc);

			// Virtual to handle an event
	virtual	void handleEvent(MVEvent& event,phaseType phase = phFocused);

			// Change video modes
			void changeVideoMode();

			// Method to create the menubar
	static	MVMenuBar *initMenuBar(MGLDevCtx& dc,const MVRect& bounds);
	};

// Global variables

int	driver = grDETECT,mode = grDETECT;	// Graphics driver and mode

// Constructor

ModeDlgApp::ModeDlgApp(MGLDevCtx& dc)
	  : MVProgram(dc, fmHelvetica, ptWindowsPalette),
		MVProgInit(NULL,ModeDlgApp::initMenuBar,MVProgram::initDeskTop)
{
}

MVMenuBar* ModeDlgApp::initMenuBar(MGLDevCtx& dc,const MVRect& bounds)
/****************************************************************************
*
* Function:		ModeDlgApp::initMenuBar
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
		+ new MVMenuItem("~V~ideo mode...",cmVideoMode)
		+ new MVMenuItemSeparator()
		+ new MVMenuItem("~Q~uit",cmQuit,MVHotKey(kbX,mdAlt),hcNoContext,"Alt+X");
	fileMenu->doneDefinition();

	MVMenuBar *menuBar = new MVMenuBar(dc,bounds);

	*menuBar
		+ new MVSubMenuItem("~F~ile",fileMenu);
	menuBar->doneDefinition();

	return (MVMenuBar*)MV_validView(menuBar);
}

void ModeDlgApp::changeVideoMode()
/****************************************************************************
*
* Function:     ModeDlgApp::changeVideoMode
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

void ModeDlgApp::handleEvent(MVEvent& event,phaseType phase)
/****************************************************************************
*
* Function:		ModeDlgApp::handleEvent
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
					"ModeDlgApp\n\n"
					"Copyright (C) 1996 SciTech Software\n"
					"Written by Kendall Bennett",
					mfInformation | mfOKButton | mfOKDefault | mfCenterText);
				break;
			case cmVideoMode:
                changeVideoMode();
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

	// Register all the 256 color drivers
	MGL_registerDriver(MGL_VGA8NAME,VGA8_driver);
	MGL_registerDriver(MGL_VGAXNAME,VGAX_driver);
	MGL_registerDriver(MGL_SVGA8NAME,SVGA8_driver);
	MGL_registerDriver(MGL_LINEAR8NAME,LINEAR8_driver);
	MGL_registerDriver(MGL_ACCEL8NAME,ACCEL8_driver);
#ifdef	MGL_WIN
	MGL_registerDriver(MGL_DDRAW8NAME,DDRAW8_driver);
#endif

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
		ModeDlgApp *modeDlgApp = new ModeDlgApp(dc);
		endState = modeDlgApp->run();
		delete modeDlgApp;

		// Shut down the MGL
		MGL_exit();
		}
}
