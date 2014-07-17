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
* Description:  Header file for the class ExplanationWindow, a class for
*               displaying the explanation information about the current
*               demo at the bottom of the display. It also contains the
*               speed buttons to move forwards or backwards through the
*               demos.
*
****************************************************************************/

#ifndef __EXPLWIND_HPP
#define __EXPLWIND_HPP

#include "mvis/mdialog.hpp"
#include "mvis/msttext.hpp"

/*-------------------------- Class definitions ----------------------------*/

//---------------------------------------------------------------------------
// The ExplanationWindow class is a special MVDialog class used to display
// information about the current demo, and to handle the speed buttons.
//---------------------------------------------------------------------------

class ExplanationWindow : public MVDialog {
protected:
    MVMultiLineText *explanation;   // Text for the explanation

public:
            // Constructor
            ExplanationWindow(MGLDevCtx& dc,const MVRect& bounds);

            // Method handle events
    virtual void handleEvent(MVEvent& event,phaseType phase = phFocused);
    };

#endif  // __EXPLWIND_HPP
