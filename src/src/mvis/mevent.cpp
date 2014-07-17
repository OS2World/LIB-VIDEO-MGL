/****************************************************************************
*
*					 MegaVision Application Framework
*
*	   A C++ GUI Toolkit for the SciTech Multi-platform Graphics Library
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
* Language:		C++ 3.0
* Environment:	Any
*
* Description:	Module to implement the mouse and keyboard event collection
*				queue routines for the toolkit.
*
****************************************************************************/

#include "mvis/mvision.hpp"
#include "mvis/mevent.hpp"
#include <string.h>

/*--------------------------- Global Variables ----------------------------*/

MVEventQueue	MV_eventQueue;

/*----------------------------- Implementation ----------------------------*/

ibool MVEventQueue::post(const MVEvent& evt)
/****************************************************************************
*
* Function:		MVEventQueue::post
* Parameters:	evt	- Event to post to the event queue
* Returns:		True if the event was posted, false if queue is full.
*
* Description:	Posts an event to the event queue. If the event is a double
*				click event, we simulate this by duplicating a mouse down
*				event.
*
****************************************************************************/
{
	if (evt.what & evKeyboard)
		return EVT_post(0,evt.what,evt.key.keyCode,evt.key.modifiers);

	if (evt.what & evMouse) {
		if (!EVT_post(0,evt.what,evt.mouse.buttons,evt.mouse.modifiers))
			return false;
		if (evt.mouse.doubleClick)
			return EVT_post(0,evt.what,evt.mouse.buttons,evt.mouse.modifiers);
		}
	return EVT_post((ulong)evt.which,evt.what,evt.message.command,evt.message.infoLong);
}
