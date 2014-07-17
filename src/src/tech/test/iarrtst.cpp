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
* Description:	Test program for the Array classes.
*
*
****************************************************************************/

#include "array.hpp"
#include <malloc.h>
#include <iostream.h>
#include <conio.h>

class MyNode {
	int		_id;

public:
			MyNode(int id) 	{ _id = id; };
	virtual	~MyNode();
			int id(void) const	{ return _id; };
			void setId(int id)	{ _id = id; };
			ibool operator < (const MyNode& n) const
				{ return id() < n.id(); };
			ibool operator > (const MyNode& n) const
				{ return id() > n.id(); };
			ibool operator == (const MyNode& n) const
				{ return id() == n.id(); };
	friend	ostream& operator << (ostream& o,const MyNode& n)
				{ return o << n._id; };
	};

MyNode::~MyNode()
{
	cout << "Destructing " << _id << endl;
}

void dumpArray(IArray<MyNode>& a)
{
	if (!a.isEmpty()) {

		// Dump some details about the array

		cout << "Contents of array (" << a.numberOfItems() << " elements,"
			 << a.size() << " size, " << a.delta() << " delta):";
		cout << endl << "    ";

		for (int i = 0; i < a.numberOfItems(); i++)
			cout << *a[i] << " ";
		if (a.isFull())
			cout << "Array is full!" << endl;
		}
	else
		cout << "Empty array (" << a.numberOfItems() << ")";

	cout << endl << endl;
}

void main(void)
{
	int		i;

	cout << endl << endl;

	cout << "Memory at start: " << coreleft() << " bytes\n";

	IArray<MyNode>	a(10),b(a),c;

	// Create an array of 10 elements

	for (i = 0; i < 10; i++) {
		a.add(new MyNode(i));
		}

	cout << "Memory after creating array: " << coreleft() << " bytes\n";

	dumpArray(a);
	dumpArray(b);
	cout << "a == b > " << ((a == b) ? "TRUE" : "FALSE") << endl;
	c = a;
	dumpArray(c);
	cout << "c == a > " << ((c == a) ? "TRUE" : "FALSE") << endl;

	// Insert a number of elements into the middle of the array

	a.setDelta(10);
	for (i = 100; i < 120; i++) {
		a.insert(new MyNode(i),5);
		}
	dumpArray(a);
	getch();

	// Insert an element at the start and at the end

	a.insert(new MyNode(-1),0);
	a.insert(new MyNode(-2),a.numberOfItems());
	dumpArray(a);
	getch();

	// Now replace some elements in the array

	a.replace(new MyNode(-3),4);
	a.replace(new MyNode(-3),5);
	delete a[6];	a[6] = new MyNode(-4);
	delete a[7];	a[7] = new MyNode(-5);
	dumpArray(a);
	getch();

	// Now remove some elements from the array

	for (i = 0; i < 7; i++)
		a.destroy(4);
	dumpArray(a);

	a.destroy(10);
	dumpArray(a);
	getch();

	// Display the array using iterators.

	IArrayIterator<MyNode> it1;

	for (it1 = a; it1; it1++)
		cout << *it1.node() << " ";
	cout << endl;

	for (it1.restart(); it1;)
		cout << *it1++ << " ";
	cout << endl;

	for (it1.restart(10,20); it1;)
		cout << *++it1 << " ";
	cout << endl;
	getch();

	a.empty();
	dumpArray(a);

	cout << "Memory at end: " << coreleft() << " bytes\n\n";
}
