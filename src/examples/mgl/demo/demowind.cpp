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
* Description:  Member functions for the class DemoWindow, a class
*               performing each of the graphics library demos in the
*               current window.
*
****************************************************************************/

#include "demo.hpp"

#pragma hdrstop

#include "demowind.hpp"
#include "demoarea.hpp"

/*----------------------------- Implementation ----------------------------*/

DemoWindow::DemoWindow(MGLDevCtx &dc,const MVRect& bounds)
    : MVDialog(dc,bounds,NULL,wfFramed),
      MVWindowInit(MVWindow::initFrame,MVWindow::initTitleBar)
/****************************************************************************
*
* Function:     DemoWindow::DemoWindow
* Parameters:   bounds      - Bounding box for the window
*
* Description:  Constructor for the DemoWindow class. The window is used
*               to render each of the demos currently being executed.
*
****************************************************************************/
{
    options &= ~(ofSelectable | ofFirstClick);

    MVRect r(getClientRect());
    r.offset(-bounds.left(),-bounds.top());
    insert(new DemoArea(dc,r));
}

void DemoWindow::drawBackground(const MVRect&)
/****************************************************************************
*
* Function:     DemoWindow::drawBackground
*
* Description:  Draws the background for the window. Since the window is
*               completely covered by the rendering area, we do not
*               draw the window background, just the frame around the
*               demo window.
*
****************************************************************************/
{
}
