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
* Description:	Header file for a TCHashTable class.
*
*
****************************************************************************/

#ifndef	__TCL_HASHTAB_HPP
#define	__TCL_HASHTAB_HPP

#ifndef	__SCITECH_H
#include "scitech.h"
#endif

#ifndef	__IOSTREAM_H
#include <iostream.h>
#endif

/*--------------------------- Class Definition ----------------------------*/

//---------------------------------------------------------------------------
// The TCHashTableNode class is a simple class used to link the objects in
// a single hash table bucket together. To put anything useful into the
// table, you must derive the object place in the table from TCHashTableNode.
//---------------------------------------------------------------------------

class TCHashTableNode {
protected:
	TCHashTableNode	*next;
	TCHashTableNode	**prev;

	friend class TCGenHashTable;
	friend	ostream& operator << (ostream& o,TCGenHashTable& h);
public:
			// Constructor to satisfy some compilers :-(
			TCHashTableNode() {};

			// Virtual destructor to delete a hash table node
	virtual	~TCHashTableNode();

			// Virtual member to compute the hash value of a symbol
	virtual	uint hash() const = 0;

			// Virtual equality operator for a hash table node and a key
	virtual ibool operator == (const TCHashTableNode& key) const = 0;

			// Virtual member to display a hash table node
	virtual void printOn(ostream& o) const = 0;

			// Method to display a hash table node
	friend	ostream& operator << (ostream& o,TCHashTableNode& n)
			{
				n.printOn(o);
				return o;
			};
	};

//---------------------------------------------------------------------------
// The TCHashTable class is a class designed to hold a number of unordered
// objects together in a hash table for fast access. In the simple form,
// TCHashTableNode objects contain nothing special. To add an arbitrary class
// to the table, you must derive the class from TCHashTableNode (either
// through single or multiple inheritance).
//---------------------------------------------------------------------------

class TCGenHashTable {
protected:
	uint			size;		// Size of hash table
	ulong			count;		// Number of objects in table
	TCHashTableNode	**table;	// Table of hash table node pointers

public:
			// Constructor
			TCGenHashTable(uint tabsize = 0);

			// Destructor
			~TCGenHashTable();

			// Member function to add a symbol to the table
			void add(TCHashTableNode* node);

			// Member function to remove a symbol from the table
			TCHashTableNode* remove(TCHashTableNode* node);

			// Member function to find a symbol in the table
			TCHashTableNode* find(TCHashTableNode* key) const;

			// Member function to find a symbol in the table (cached)
			TCHashTableNode* findCached(TCHashTableNode* key);

			// Member function to find the next symbol in chain
			TCHashTableNode* next(TCHashTableNode* last) const;

			// Empties the entire table by destroying all nodes.
			void empty();

			// Returns the number of items in the table
			ulong numberOfItems() const	{ return count; };

			// Returns true if the table is empty
			ibool isEmpty() const		{ return count == 0; };

			// Returns the load factor for the table
			ulong loadFactor() const	{ return (size * 1000L) / count; };

			// Friend to display the contents of the hash table
	friend	ostream& operator << (ostream& o,TCGenHashTable& h);
	};

//---------------------------------------------------------------------------
// Template wrapper class for declaring Type Safe Hash Tables.
//---------------------------------------------------------------------------

template <class T> class TCHashTable : public TCGenHashTable {
public:
			TCHashTable(uint tabsize = 0)
				: TCGenHashTable(tabsize){};
			T* remove(T* node)
				{ return (T*)TCGenHashTable::remove(node); };
			T* find(T* key) const
				{ return (T*)TCGenHashTable::find(key); };
			T* findCached(T* key)
				{ return (T*)TCGenHashTable::findCached(key); };
			T* next(T* last) const
				{ return (T*)TCGenHashTable::next(last); };
	};

/*-------------------------- Function Prototypes --------------------------*/

// Pre-defined hash routines for C style strings

uint	TCL_hashAdd(uchar *name);		// Simple and fast
uint	TCL_hashPJW(uchar *name);		// Better distribution of symbols
uint	TCL_hashSA(uchar *name);		// From Sedgewick's Algorithms
uint	TCL_hashGE(uchar *name);		// From Gosling's Emac's
uchar	TCL_hash8(uchar *name);			// Randomised 8 bit hash value
ushort	TCL_hash16(uchar *name);		// Randomised 16 bit hash value
ulong	TCL_hash32(uchar *name);		// Randomised 32 bit hash value

#endif	// __TCL_HASHTAB_HPP
