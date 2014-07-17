/****************************************************************************
*
*						  Techniques Class Library
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
* Language:		C++ 3.0
* Environment:	any
*
* Description:	Test program for the TCMemStack class.
*
*
****************************************************************************/

#include "tcl\memmgr.hpp"
#include <malloc.h>
#include <iostream.h>

class MyNode {
	int		_id;
	char	stuff[255];

public:
			MyNode(int id) 	{ _id = id; };
			int id(void)	{ return _id; };
	};

void DoLocal(TCMemStack& mem)
{
	MyNode	*n;

	TCMemMarker marker(mem);

	for (int i = 0; i < 100; i++)
		n = new(mem) MyNode(i);

	cout << "Memory at end of locals: " << coreleft() << " bytes\n";
}

void main(void)
{
	MyNode	*n;

	cout << "Memory at start: " << coreleft() << " bytes\n";

	TCMemStack* mem = new TCMemStack;	// Allocate a memory stack

	cout << "Memory after creating stack: " << coreleft() << " bytes\n";

	for (int i = 0; i < 10; i++)
		n = new(*mem) MyNode(i);

	cout << "Memory after doing allocation: " << coreleft() << " bytes\n";

	DoLocal(*mem);

	cout << "Memory after doing locals: " << coreleft() << " bytes\n";

	for (i = 0; i < 20; i++)
		n = new(*mem) MyNode(i);

	cout << "Memory before end: " << coreleft() << " bytes\n";

	delete mem;

	cout << "Memory at end: " << coreleft() << " bytes\n";
}
