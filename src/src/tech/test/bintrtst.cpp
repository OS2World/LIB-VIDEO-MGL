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
* Description:	Test program for the BinaryTree class.
*
*
****************************************************************************/

#include "debug.h"
#include "tcl\bintree.hpp"
#include <iostream.h>
#include <malloc.h>
#include <stdlib.h>

class MyNode : public TCBinaryTreeNode {
	int		_id;

public:
			MyNode(int id) 	{ _id = id; };
			int id() const	{ return _id; };
	virtual	~MyNode();
	virtual	int cmp(const TCBinaryTreeNode& node) const;
	};

MyNode::~MyNode()
{
	cout << "Deleting " << _id << endl;
}

MyNode::cmp(const TCBinaryTreeNode *node) const
{
	return (id() - ((MyNode*)node)->id());
}

void main(void)
{
	TCBinaryTree<MyNode>	tree;

}
