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
* Description:	Test program for the hash table class.
*
*
****************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <iostream.h>
#include "tcl/hashtab.hpp"

#ifdef __UNIX__

unsigned long coreleft(void) {
	return 1024;
}

#endif

// Define the nodes to be placed into the hash table

class entry : public TCHashTableNode {
protected:
	char	*name;			// Key of node
	int		other_stuff;
public:
			// Constructor
			entry(char *n,int stuff)
			{
				name = n;
				other_stuff = stuff;
			}

			// Destructor to delete a hash table node
			~entry();

			// Member to compute the hash value of a symbol
			uint hash(void) const;

			// Equality operator for a hash table node and a key
			ibool operator == (const TCHashTableNode& key) const;

			// Member to display a hash table node
			void printOn(ostream& o) const;
	};

entry::~entry()
{
//	cout << "Deleting " << name << endl;
}

uint entry::hash(void) const
{
	return TCL_hashAdd(name);
}

ibool entry::operator == (const TCHashTableNode& key) const
{
	return (strcmp(name,((entry&)key).name) == 0);
}

void entry::printOn(ostream& o) const
{
	o << name << " : " << other_stuff << endl;
}

void main(int argc,char *argv[])
{
	entry	*p,*q,*r;

	cout << "\nMemory at start: " << (unsigned long)coreleft() << endl;

	// Create the hash table

	TCHashTable<entry>* symtab = new TCHashTable<entry>(31);

	// For each element on the command line, put it into the table

	for (++argv, --argc; --argc>=0; argv++)	{
		p = new entry(*argv,argc);
		symtab->add(p);
		}

	// Display the contents of the table

	cout << *symtab << endl;

	// Look up a node in the hash table

	p = new entry("in",0);
	q = symtab->find(p);

	if (q == NULL)
		cout << "Node not found.\n";
	else {
		cout << *q;
		}

	r = q;

	while ((q = symtab->next(q)) != NULL)
		cout << " and " << *q;
	cout << endl;

	// Attempt to remove the first node found

	if (r)
		delete symtab->remove(r);
	cout << *symtab << endl;

	cout << "\nMemory before deleting hash table: " << (unsigned long)coreleft() << endl;

	delete symtab;
	delete p;

	cout << "\nMemory after deleting hash table: " << (unsigned long)coreleft() << endl;
}
