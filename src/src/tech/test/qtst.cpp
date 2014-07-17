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
* Description:	Test program for the queue class.
*
*
****************************************************************************/

#include "tcl\queue.hpp"
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

TCQueue<MyNode>	q;

void dumpQueue(TCQueue<MyNode>& q)
{
	TCListIterator<MyNode>	i;

	if (!q.isEmpty()) {

		// Dump some details about the queue

		cout << "First Item: " << q.peek()->id() << endl;

		cout << "Contents of queue (" << q.numberOfItems() << " items):";
		cout << endl << "    ";

		for (i = (TCList<MyNode>&)q; i; i++)
			cout << i.node()->id() << " ";
		}
	else
		cout << "Empty queue (" << q.numberOfItems() << ")";

	cout << endl << endl;
}

void main(void)
{
	int		i;
	MyNode	*n;

	cout << "Memory at start: " << coreleft() << " bytes\n";

	// Create a queue of 10 nodes.

	for (i = 0; i < 10; i++) {
		n = new MyNode(i);
		q.put(n);
		}

	cout << "Memory after creating queue: " << coreleft() << " bytes\n";

	dumpQueue(q);

	// Kill the queue and display it

	q.empty();
	dumpQueue(q);

	// Create a queue of 10 nodes.

	for (i = 0; i < 10; i++) {
		n = new MyNode(i);
		q.put(n);
		}

	dumpQueue(q);

	// Remove all the nodes from the queue with get()

	while ((n = q.get()) != NULL) {
		cout << "Removed " << n->id() << endl;
		delete n;
		}

	dumpQueue(q);

	cout << "Memory at end: " << coreleft() << " bytes\n";
}
