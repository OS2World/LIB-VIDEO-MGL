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
* Description:	Test program for the SimpleList linked list class.
*
*
****************************************************************************/

#include "tcl\list.hpp"
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

TCList<MyNode>	l;

void dumpList(TCList<MyNode>& l,ibool checkSorted = false)
{
	MyNode	*n;

	if (!l.isEmpty()) {

		// Dump some details about the list

		cout << "Head Node: " << l.peekHead()->id() << endl;

		cout << "Contents of list (" << l.numberOfItems() << " items):";
		cout << endl << "    ";

		for (n = l.peekHead(); n; n = l.next(n)) {
			cout << n->id() << " ";
			if (checkSorted && l.next(n) && (n->id() > l.next(n)->id())) {
				cerr << "List is not in sorted order!!\n";
				exit(1);
				}
			}
		}
	else
		cout << "Empty list (" << l.numberOfItems() << ")";

	cout << endl << endl;
}

int myCmp(MyNode *n1,MyNode *n2)
{
	return n1->id() - n2->id();
}

void main(void)
{
	int		i,count;
	MyNode	*n1,*middle,*beforeMiddle,*last;

	cout << "Memory at start: " << coreleft() << " bytes\n";

	// Create a linked list of 10 nodes adding at the head of the list.

	for (i = 0; i < 10; i++) {
		n1 = new MyNode(i);
		l.addToHead(n1);
		if (i == 0)
			last = n1;
		if (i == 5)
			beforeMiddle = n1;
		if (i == 4)
			middle = n1;
		}

	cout << "Memory after creating list: " << coreleft() << " bytes\n";

	dumpList(l);

	// Add a new node after the head of the list and after a middle node

	n1 = new MyNode(20);
	l.addAfter(n1,l.peekHead());
	n1 = new MyNode(21);
	l.addAfter(n1,last);
	n1 = new MyNode(22);
	l.addAfter(n1,middle);

	dumpList(l);

	// Remove the middle object from the list using beforeMiddle as the
	// previous node in the list.

	delete l.removeNext(beforeMiddle);
	dumpList(l);

	// Remove all remaining nodes with removeFromHead().

	count = l.numberOfItems();
	for (i = 0; i < count+5; i++)
		delete l.removeFromHead();

	dumpList(l);

	cout << "Memory: " << coreleft() << " bytes\n";

	for (i = 0; i < 10; i++) {
		n1 = new MyNode(i);
		l.addToHead(n1);
		}

	// Display the list using iterators to move through the list.

	TCListIterator<MyNode> it1;

	for (it1 = l; it1; it1++)
		cout << it1.node()->id() << " ";
	cout << endl;

	for (it1.restart(); it1;)
		cout << it1++->id() << " ";
	cout << endl;

	l.empty();

	l.empty();

	// Create a list of 100 random numbers, and sort them

#if 0
	srand(time(NULL));
	for (i = 0; i < 100; i++) {
		n1 = new MyNode(rand());
		l.addToHead(n1);
		}
#endif

	l.addToHead(new MyNode(3));
	l.addToHead(new MyNode(1));
	l.addToHead(new MyNode(2));

	dumpList(l);

	l.sort(myCmp);

	dumpList(l,true);

	l.empty();

	cout << "Memory at end: " << coreleft() << " bytes\n\n";
}
