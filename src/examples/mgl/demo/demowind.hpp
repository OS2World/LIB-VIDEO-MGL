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
* Description:  Header file for the class DemoWindow, a class for
*               rendering the current demo in the window.
*
****************************************************************************/

#ifndef __DEMOWIND_HPP
#define __DEMOWIND_HPP

#include "mvis/mdialog.hpp"

/*-------------------------- Class definitions ----------------------------*/

//---------------------------------------------------------------------------
// The DemoWindow class is a special MVDialog class used to display
// the current rendering in a double buffered animation window.
//---------------------------------------------------------------------------

class DemoWindow : public MVDialog {
protected:
            // Method to draw the background
    virtual void drawBackground(const MVRect& clip);

public:
            // Constructor
            DemoWindow(MGLDevCtx& dc,const MVRect& bounds);
    };

#endif  // __DEMOWIND_HPP
