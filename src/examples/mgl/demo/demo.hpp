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
* Description:  Header file for the demo program's main class.
*
****************************************************************************/

#ifndef __DEMO_HPP
#define __DEMO_HPP

#include "mvis/mvision.hpp"
#include "mvis/mprogram.hpp"
#include "mvis/mmenubar.hpp"

/*------------------------------ Constants --------------------------------*/

// Application command codes

const uint
    cmNextDemo          = 1000,         // Move onto next demo
    cmPrevDemo          = 1001,         // Move back to prev demo
    cmAutoDemo          = 1002,         // Toggle automatic operation
    cmSetExplanation    = 1003,         // Set the explanation window text
    cmSetDemoTitle      = 1004,         // Set the demo title text

// Menu command codes

    cmAbout             = 100,          // Bring up the about box
    cmVideoMode         = 101,          // Change the current video mode
    cmLineDemo          = 102,
    cmEllipseDemo       = 103,
    cmArcDemo           = 104,
    cmPolygonDemo       = 105,
    cmRectangleDemo     = 106,
    cmColorDemo         = 107,
	cmPatternDemo       = 109,
	cmAnimationDemo     = 111,
    cmStatusDemo        = 112;

/*---------------------------- Global Variables ---------------------------*/

extern int  driver,mode,snowlevel;

/*-------------------------- Class definitions ----------------------------*/

class Demo : public MVProgram {
protected:
            // Method to change the current video mode
            void changeVideoMode();

public:
            // Constructor
            Demo(MGLDevCtx& dc);

            // Virtual to handle an event
    virtual void handleEvent(MVEvent& event,phaseType phase = phFocused);

            // Methods to create the menubar, status line and desktop etc
    static  MVMenuBar *initMenuBar(MGLDevCtx& dc,const MVRect& bounds);
    static  MVDeskTop *initDeskTop(MGLDevCtx& dc,const MVRect& bounds);
    };

#endif  // __DEMO_HPP
