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
* Language:		ANSI C
* Environment:	Any
*
* Description:	Block memory management routines. These functions are used
*				to manage blocks of objects of fixed sizes for efficient
*				memory management using a free list.
*
****************************************************************************/

#include "mgl.h"

/*------------------------- Implementation --------------------------------*/

/****************************************************************************
PARAMETERS:
l			- Block list to allocate
max			- Maximum number of objects in each block
objectSize	- Size of each object in bytes

REMARKS:
Creates the initial block list by allocating and building the block
list free list pool. Block lists are arbirtrary blocks of objects, are
form the building block for memory management for all the region functions.
Whenever the block list needs to be resized, we always resize it in
increments of 'max' objects.
{secret}
****************************************************************************/
void _MGL_createBlockList(
	blockList_t *l,
	int max,
	int objectSize)
{
	l->max = max;
	l->objectSize = objectSize;
	if ((l->curBlock = MGL_malloc(max * objectSize + sizeof(block_t))) == NULL)
		MGL_fatalError("Not enough memory to create memory pool!");
	l->blockCount = 1;
}

/****************************************************************************
PARAMETERS:
l	- Block list to resize

REMARKS:
Resizes the block list if full by allocating another pool block and linking
it onto the list.
{secret}
****************************************************************************/
void _MGL_resizeBlockList(
	blockList_t *l)
{
	block_t		*temp;

	if ((temp = MGL_malloc(l->max * l->objectSize + sizeof(block_t))) == NULL)
		MGL_fatalError("Not enough memory to resize memory pool!");
	temp->next = l->curBlock;
	l->curBlock = temp;
	l->blockCount++;
}

/****************************************************************************
PARAMETERS:
l	- Block list to free

REMARKS:
Frees all of the pool blocks in the list
{secret}
****************************************************************************/
void _MGL_freeBlockList(
	blockList_t *l)
{
	while (l->blockCount) {
		block_t *temp = l->curBlock;
		l->curBlock = temp->next;
		MGL_free(temp);
		l->blockCount--;
		}
}

/****************************************************************************
PARAMETERS:
l	- Block list to build free list for

REMARKS:
Builds the free list of object in the first block in the list, assuming
the size of the blocks specified in the blockList.
{secret}
****************************************************************************/
void *_MGL_buildFreeList(
	blockList_t *l)
{
	int		i,max = l->max,size = l->objectSize;
	void	*freeList;
	char	*p,*next;

	p = freeList = (l->curBlock+1);
	for (i = 0; i < max; i++,p = next)
		FREELIST_NEXT(p) = next = p+size;
	FREELIST_NEXT(p-size) = NULL;

	return freeList;
}
