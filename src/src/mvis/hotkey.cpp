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
* Description:	Implementation for the MVHotKey class.
*
****************************************************************************/

#include "mvis/mvision.hpp"
#include "mvis/mkeys.hpp"

ibool MVHotKey::operator == (const MVKeyEvent& event) const
/****************************************************************************
*
* Function:		MVHotKey::operator ==
* Parameters:	event	- Keyboard event to check
* Returns:		True if the hotkey value is present
*
****************************************************************************/
{
	if ((uint)event.charScan.scanCode != scanCode)
		return false;

	// Check that any shift key is down
	if (modifiers & mdShift) {
		if (!(event.modifiers & mdShift))
			return false;
		}

	// Check if any Alt key is down
	if (modifiers & mdAlt) {
		if (!(event.modifiers & mdAlt))
			return false;
		}

	// Check if any Ctrl key is down
	if (modifiers & mdCtrl) {
		if (!(event.modifiers & mdCtrl))
			return false;
		}

	// All conditions are true for this hot key!
	return true;
}
