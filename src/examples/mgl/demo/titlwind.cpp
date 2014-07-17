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
* Description:  Member functions for the class TitleWindow, a class
*               providing information about the current demo in progress.
*
****************************************************************************/

#include "demo.hpp"

#pragma hdrstop

#include "titlwind.hpp"

/*----------------------------- Implementation ----------------------------*/

TitleWindow::TitleWindow(MGLDevCtx& dc,const MVRect& bounds)
    : MVDialog(dc,bounds,NULL,wfFramed),
      MVWindowInit(MVWindow::initFrame,MVWindow::initTitleBar)
/****************************************************************************
*
* Function:     TitleWindow::TitleWindow
* Parameters:   bounds      - Bounding box for the window
*
* Description:  Constructor for the TitleWindow class. The window will
*               be placed at the top of the bounding box passed to
*               this routine.
*
****************************************************************************/
{
    options &= ~(ofSelectable | ofFirstClick);
    dc.useFont(MV_fontManager.useFont(fmSystemFont));

    // Adjust the bounds for the entire window
    MVRect  r(getClientRect());
    int     borderH = r.left() - bounds.left();
    int     borderV = r.top() - bounds.top();

    r.bottom() = r.top() + dc.textHeight();
    title = new MVStaticText(dc,r,"",0);
    insert(title);

    r.inset(-borderH,-borderV);
    setBounds(r);
}

void TitleWindow::handleEvent(MVEvent& event,phaseType)
/****************************************************************************
*
* Function:     TitleWindow::handleEvent
* Parameters:   event   - Event to handle
*               phase   - Current phase for the event (pre,focus,post)
*
* Description:  Event handling routine for the TitleWindow class. Here
*               we handle events that change the text in the window.
*
****************************************************************************/
{
    if (event.what == evBroadcast) {
        if (event.message.command == cmSetDemoTitle) {
            title->setText((const char *)event.message.infoPtr);
            clearEvent(event);
            }
        }
}
