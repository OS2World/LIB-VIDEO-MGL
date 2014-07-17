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
* Description:	Test program for the SArray class.
*
*
****************************************************************************/

#include "array.hpp"
#include <malloc.h>
#include <iostream.h>

void dumpArray(Array<int>& a)
{
	if (!a.isEmpty()) {

		// Dump some details about the array

		cout << "Contents of array (" << a.numberOfItems() << " elements,"
			 << a.size() << " size, " << a.delta() << " delta):";
		cout << endl << "    ";

		for (int i = 0; i < a.numberOfItems(); i++)
			cout << a[i] << " ";
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

	SArray<int>	a(10),b(a),c;

	// Create an array of 10 elements

	for (i = 10; i > 1; i--) {
		a.add(i);
		}

	cout << "Memory after creating array: " << coreleft() << " bytes\n";

	dumpArray(a);
	dumpArray(b);

	// Sort the array a and copy it to c

	a.sort();
	c = a;
	dumpArray(c);

	// Insert a number of elements in sorted order

	a.setDelta(10);
	for (i = 100; i < 120; i++) {
		a.addSorted(i);
		}

	dumpArray(a);

	// Display the array using iterators.

	ArrayIterator<int> it1;

	for (it1 = a; it1; it1++)
		cout << it1.node() << " ";
	cout << endl;

	for (it1.restart(); it1;)
		cout << it1++ << " ";
	cout << endl;

	for (it1.restart(10,20); it1;)
		cout << ++it1 << " ";
	cout << endl;

	// Search for an item in the array

	cout << "Index of element 110 (linear search): " << a.search(110) << endl;
	cout << "                     (binary search): " << a.binarySearch(110) << endl;

	cout << "Index of element 200 (linear search): " << a.search(200) << endl;

	cout << "                     (binary search): " << a.binarySearch(200) << endl;

	cout << "Memory at end: " << coreleft() << " bytes\n\n";
}
