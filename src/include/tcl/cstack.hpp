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
* Description:	Header file for a creating typed stack classes, with the
*				objects in a contiguous array.
*
*
****************************************************************************/

#ifndef	__TCL_CSTACK_HPP
#define	__TCL_CSTACK_HPP

#ifndef	__SCITECH_H
#include "scitech.h"
#endif

#ifndef	__TCL_ERROR_HPP
#include "tcl/error.hpp"
#endif

/*--------------------------- Class Definition ----------------------------*/

//---------------------------------------------------------------------------
// Template class to create a downward growing stack class that maintains
// elements of a particular type (and only this type) in a contiguous array.
// The following operations are supported on the stack:
//
//	StackName(int size)	- Construct a stack of a given size
//	top()				- Examine the top item on the stack
//	element(int)		- Examine the nth item from the top of the stack
//	push(elementType&)	- Push an element onto the stack
//	pushq(elementType&)	- Push an element with no range checking
//	pop()				- Pop the top element from the stack
//	popq()				- Pop the top element with no range checking
//	popn(int)			- Pop n items from the stack, returning top element
//	popqn(int)			- Pop n items from stack with no range checking
//	empty()				- Empty the stack of all elements
//	numberOfItems()		- Return the number of items on the stack
//	isEmpty()			- Returns true if stack is empty
//	isFull()			- Returns true if stack is full
//
//---------------------------------------------------------------------------

template <class T> class TCCStack {
protected:
	int			size;
	T			*stack;
	T			*p_stack;

public:
			TCCStack(int size)
			{
				stack = new T[size];
				TCCStack<T>::size = size;
				if (valid()) empty();
			};
	inline	~TCCStack()				{ delete [] stack; };
	inline	T& top() const			{ return *p_stack; };
	inline	T& element(int offset)	{ return *(p_stack + offset - 1); };
	inline	void push(T& node)
				{ isFull() ? TCL_errorHandler(STK_OVERFLOW) : pushq(node); };
	inline	void pushq(T& node)		{ *--p_stack = node; };
	inline	T& pop()
			{
				if (isEmpty())
					TCL_errorHandler(STK_UNDERFLOW);
				return popq();
			};
	inline	T& popq()				{ return *p_stack++; };
	inline	T& popn(int n)
			{
				if (numberOfItems() < n)
					TCL_errorHandler(STK_UNDERFLOW);
				return popqn(n);
			};
	inline	T& popqn(int n)			{ return (p_stack += n)[-n]; };
	inline	void empty()			{ p_stack = stack + size; };
	inline	int numberOfItems() const
				{ return size - (p_stack - stack); };
	inline	ibool isEmpty() const
				{ return p_stack >= (stack + size); };
	inline	ibool isFull() const		{ return p_stack <= stack; };
	inline	ibool valid() const		{ return stack != NULL; };
	inline	int getSize() const		{ return size; };
	};

//---------------------------------------------------------------------------
// Another template class to create a downward growing stack class with
// identical semantics to the above class. This version however has the
// size of the stack specified at compile time and the memory for the
// stack is allocated as part of the actual class (does not use new and
// delete).
//---------------------------------------------------------------------------

template <class T,int size> class TCStaticCStack {
protected:
	T			stack[size];
	T			*p_stack;

public:
			TCStaticCStack()		{ empty(); };
	inline	T& top() const			{ return *p_stack; };
	inline	T& element(int offset)	{ return *(p_stack + offset - 1); };
	inline	void push(T& node)
				{ isFull() ? TCL_errorHandler(STK_OVERFLOW) : pushq(node); };
	inline	void pushq(T& node)		{ *--p_stack = node; };
	inline	T& pop()
			{
				if (isEmpty())
					TCL_errorHandler(STK_UNDERFLOW);
				return popq();
			};
	inline	T& popq()				{ return *p_stack++; };
	inline	T& popn(int n)
			{
				if (numberOfItems() < n)
					TCL_errorHandler(STK_UNDERFLOW);
				return popqn(n);
			};
	inline	T& popqn(int n)			{ return (p_stack += n)[-n]; };
	inline	void empty()			{ p_stack = stack + size; };
	inline	int numberOfItems() const
				{ return size - (p_stack - (T*)stack); };
	inline	ibool isEmpty() const
				{ return p_stack >= (stack + size); };
	inline	ibool isFull() const		{ return p_stack <= (T*)stack; };
	inline	int getSize() const		{ return size; };
	};

#endif	// __TCL_CSTACK_HPP
