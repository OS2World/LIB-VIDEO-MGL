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
* Language:		ANSI C++
* Environment:	Any
*
* Description:	Member functions for the dlist class, a class designed to
*				link a series of objects together into a doubly linked
*				list. All items placed in the list MUST be derived from
*				the class FX_DListNode.
*
****************************************************************************/

#include "gm/sprmgr.hpp"

/*--------------------------- Member functions ----------------------------*/

_FX_GenDListCmp	FX_GenDList::cmp;
FX_DListNode	*FX_GenDList::_z;

FX_GenDList::FX_GenDList()
/****************************************************************************
*
* Function:		FX_GenDList::FX_GenDList
*
* Description:	Constructor for the list class. We set the count of items
*				in the list to 0, and initialise the head and tail pointers
*				for the list. These both point to dummy nodes within the
*				list class, so that the dummy tail node always points to
*				itself ensuring we cannot iterate of the end of the list.
*				This also simplifies maintaining the head and tail pointers.
*
****************************************************************************/
{
	count = 0;
	head = &hz[0];				// Setup head and tail pointers
	z = &hz[1];
	head->next = z->next = z;
	z->prev = head->prev = head;
}

FX_GenDList::~FX_GenDList()
/****************************************************************************
*
* Function:		FX_GenDList::~FX_GenDList
*
* Description:	Destructor for the list class. All we do here is ask the
*				list to empty itself.
*
****************************************************************************/
{
	empty();
}

void FX_GenDList::empty(void)
/****************************************************************************
*
* Function:		FX_GenDList::empty
*
* Description:	Empties the list of all elements. We do this by stepping
*				through the list deleting all the elements as we go.
*
****************************************************************************/
{
	FX_DListNode *temp;

	while (head->next != z) {
		temp = head->next;
		head->next = head->next->next;
		delete temp;
		}
	z->prev = head;
	count = 0;
}

FX_DListNode* FX_GenDList::merge(FX_DListNode *a,FX_DListNode *b,FX_DListNode*& end)
/****************************************************************************
*
* Function:		FX_GenDList::merge
* Parameters:	a,b		- Sublist's to merge
*				end		- Pointer to end of merged list
* Returns:		Pointer to the merged sublists.
*
* Description:	Merges two sorted lists of nodes together into a single
*				sorted list, and sets a pointer to the end of the newly
*				merged lists.
*
****************************************************************************/
{
	FX_DListNode	*c;

	// Go through the lists, merging them together in sorted order

	c = _z;
	while (a != _z && b != _z) {
		if (cmp(a,b) <= 0) {
			c->next = a; c = a; a = a->next;
			}
		else {
			c->next = b; c = b; b = b->next;
			}
		};

	// If one of the lists is not exhausted, then re-attach it to the end
	// of the newly merged list

	if (a != _z) c->next = a;
	if (b != _z) c->next = b;

	// Set end to point to the end of the newly merged list

	while (c->next != _z) c = c->next;
	end = c;

	// Determine the start of the merged lists, and reset _z to point to
	// itself

	c = _z->next; _z->next = _z;
	return c;
}

void FX_GenDList::sort(_FX_GenDListCmp cmp_func)
/****************************************************************************
*
* Function:		FX_GenDList::sort
* Parameters:	cmp	- Function to compare the contents of two ListNode's.
*
* Description:	Mergesort's all the nodes in the list. 'cmp' must point to
*				a comparison function that can compare the contents of
*				two ListNode's. 'cmp' should work the same as strcmp(), in
*				terms of the values it returns.
*
****************************************************************************/
{
	int			i,N;
	FX_DListNode	*a,*b;		// Pointers to sublists to merge
	FX_DListNode	*c;			// Pointer to end of sorted sublists
	FX_DListNode	*todo;		// Pointer to sublists yet to be sorted
	FX_DListNode	*t;			// Temporary

	// Set up globals required by DList::merge().
	FX_DListNode *head = this->head;
	_z = z;
	cmp = cmp_func;

	for (N = 1,a = _z; a != head->next; N = N + N) {
		todo = head->next; c = head;
		while (todo != _z) {

			// Build first sublist to be merged, and splice from main list

			a = t = todo;
			for (i = 1; i < N; i++) t = t->next;
			b = t->next; t->next = _z; t = b;

			// Build second sublist to be merged and splice from main list

			for (i = 1; i < N; i++) t = t->next;
			todo = t->next; t->next = _z;

			// Merge the two sublists created, and set 'c' to point to the
			// end of the newly merged sublists.

			c->next = merge(a,b,t); c = t;
			}
		}

	// After sorting the list, traverse the list from head to tail fixing
	// all the previous pointers as we go.

	a = b = head;
	b = b->next;
	while (1) {
		b->prev = a;
		if (b == _z)
			break;
		a = a->next;
		b = b->next;
		}
}

// Virtual destructor for FX_DListNode's. Does nothing.

FX_DListNode::~FX_DListNode()
{
}


