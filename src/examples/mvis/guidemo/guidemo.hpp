/****************************************************************************
*
*                   Demo - Demonstration Program for the
*                       MegaGraph Graphics Library
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
*
* Language:     C++ 3.0
* Environment:  any
*
* Description:  Header file for the demo program's main class.
*
*
****************************************************************************/

#ifndef __GUIDEMO_HPP
#define __GUIDEMO_HPP

#include "mvis/mvision.hpp"
#include "mvis/mprogram.hpp"
#include "mvis/msgbox.hpp"
#include "mvis/mfiledlg.hpp"
#include "mvis/mmodesel.hpp"
#include "mvis/mradio.hpp"
#include "mvis/mcheckbx.hpp"

/*------------------------------ Constants --------------------------------*/

const ushort cmAbout 		= cmUser+100;	// Bring up the about box
const ushort cmLoadFile 	= cmUser+101;	// Bring up a load file dialog box
const ushort cmBrowseFile 	= cmUser+102;	// Bring up a load file dialog box
const ushort cmSaveFile 	= cmUser+103;	// Bring up a save file dialog box

const ushort cmMsgNoIcon 	= cmUser+200;	// Display different message boxes
const ushort cmMsgWarning	= cmUser+201;
const ushort cmMsgError		= cmUser+202;
const ushort cmMsgInform	= cmUser+203;
const ushort cmMsgConfirm	= cmUser+204;

const ushort cmDlgRadio		= cmUser+300;
const ushort cmDlgCheckBox	= cmUser+301;
const ushort cmDlgButtons	= cmUser+302;

const ushort cmWinFixed		= cmUser+400;
const ushort cmWinMove		= cmUser+401;
const ushort cmWinResize	= cmUser+402;

const ushort cmVideoMode 	= cmUser+500;	// Change video modes

const ushort cmCloseWindow	= cmUser+1000;	// Close a window on the desktop

/*-------------------------- Class definitions ----------------------------*/

class SimpleWindow : public MVWindow {
public:
			// Constructor
			SimpleWindow(MGLDevCtx& dc,const MVRect& bounds,
				const char *title,ushort flags = wfNormal)
			: MVWindow(dc,bounds,title,flags),
			  MVWindowInit(MVWindow::initFrame,MVWindow::initTitleBar) {};

			// Virtual to handle an event
	virtual	void handleEvent(MVEvent& event,phaseType phase = phFocused);
	};

class GUIDemoApp : public MVProgram {
protected:
	SimpleWindow	*wFixed;
	SimpleWindow	*wMove;
	SimpleWindow	*wResize;

public:

			// Constructor
			GUIDemoApp(MGLDevCtx& dc);

			// Virtual to handle an event
	virtual	void handleEvent(MVEvent& event,phaseType phase = phFocused);

			// Display the file dialog boxes
			void loadFile();
			void browseFile();
			void saveFile();

			// Display message boxes
			void msgNoIcon();
			void msgWarning();
			void msgError();
			void msgInform();
			void msgConfirm();

			// Dialog box functions
			void dlgRadio();
			void dlgCheckBox();
			void dlgButtons();

			// Window functions
			void winFixed();
			void winMove();
			void winResize();

			// Change video modes
			void changeVideoMode();

			// Method to create the menubar
	static	MVMenuBar *initMenuBar(MGLDevCtx& dc,const MVRect& bounds);
	};

#endif  // __GUIDEMO_HPP
