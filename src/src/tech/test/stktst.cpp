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
* Description:	Test program for the stack class.
*
*
****************************************************************************/

#include "tcl\stack.hpp"
#include <iostream.h>
#include <malloc.h>
#include <stdlib.h>

class MyNode : public TCListNode {
	int		_id;
public:
			MyNode(int id) 	{ _id = id; };
			int id(void)	{ return _id; };
	virtual	~MyNode();
	};

MyNode::~MyNode()
{
//	cout << "Deleting " << _id << endl;
}

TCStack<MyNode>	s;

void dumpStack(TCStack<MyNode>& s)
{
	TCListIterator<MyNode>	i;

	if (!s.isEmpty()) {

		// Dump some details about the stack

		cout << "Top Item: " << s.top()->id() << endl;

		cout << "Contents of stack (" << s.numberOfItems() << " items):";
		cout << endl << "    ";

		for (i = (TCList<MyNode>&)s; i; i++)
			cout << i.node()->id() << " ";
		}
	else
		cout << "Empty stack (" << s.numberOfItems() << ")";

	cout << endl << endl;
}

void main(void)
{
	int		i;
	MyNode	*n;

	cout << "Memory at start: " << coreleft() << " bytes\n";

	// Create a stack of 10 nodes.

	for (i = 0; i < 10; i++) {
		n = new MyNode(i);
		s.push(n);
		}

	cout << "Memory after creating stack: " << coreleft() << " bytes\n";

	dumpStack(s);

	// Kill the stack and display it

	s.empty();
	dumpStack(s);

	// Create a stack of 10 nodes.

	for (i = 0; i < 10; i++) {
		n = new MyNode(i);
		s.push(n);
		}

	dumpStack(s);

	// Remove all the nodes from the stack with pop()

	while ((n = s.pop()) != NULL) {
		cout << "Popped " << n->id() << endl;
		delete n;
		}

	dumpStack(s);

	cout << "Memory at end: " << coreleft() << " bytes\n";
}
