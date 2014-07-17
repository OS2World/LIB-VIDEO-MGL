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
* Description:	Member functions for the MVMenuItem class.
*
****************************************************************************/

#include "mvis/mvision.hpp"
#include "mvis/mmenu.hpp"

/*----------------------------- Implementation ----------------------------*/

MVMenuItem::MVMenuItem(const char *name,ulong command,
	const MVHotKey& hotKey,uint helpCtx,const char *hotKeyText)
	: command(command), hotKey(hotKey), helpCtx(helpCtx)
/****************************************************************************
*
* Function:		MVMenuItem::MVMenuItem
* Parameters:	name		- Name to display for the menu item
*				command		- Command to post when activated
*				hotKey		- Altenate hot key value for menu item
*				helpCtx		- Help context number for the item
*               hotKeyText	- Text to display for HotKey value
*
* Description:	Constructor for a MVMenuItem.
*
****************************************************************************/
{
	this->name = MV_newHotStr(name,hotChar,hotIndex);
	this->hotKeyText = MV_newStr(hotKeyText);
	disabled = false;		// Needs to be set on globally disabled commands!
}

MVMenuItem::MVMenuItem(const char *name,MVMenu *subMenu,uint helpCtx)
	: subMenu(subMenu), hotKey(kbNoKey,0), helpCtx(helpCtx)
/****************************************************************************
*
* Function:		MVMenuItem::MVMenuItem
* Parameters:	name	- Name to display for menu item
*				subMenu	- Pointer to the submenu definition for the menu
*               helpCtx	- Help context number for the item
*
* Description:	Constructor for a MVMenuItem. Create a submenu definition.
*
****************************************************************************/
{
	this->name = MV_newHotStr(name,hotChar,hotIndex);
	command = 0;
	disabled = false;
}

MVMenuItem::MVMenuItem(const MVMenuItem& c) 
  :  command(c.command), disabled(c.disabled), hotKey(c.hotKey), helpCtx(c.helpCtx), 
	 hot1(c.hot1), hot2(c.hot2), bounds(c.bounds)
{
	name = MV_newStr(c.name);
	if (c.isSubMenu())
		subMenu = c.subMenu;
	else
		hotKeyText = MV_newStr(c.hotKeyText);
}

MVMenuItem::~MVMenuItem()
/****************************************************************************
*
* Function:		MVMenuItem::~MVMenuItem
*
* Description:	Destructor for the MVMenuItem class.
*
****************************************************************************/
{
  	delete [] name;
	if (isSubMenu())
		delete subMenu;
	else
		delete [] hotKeyText;
}
