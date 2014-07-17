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
* Description:	Region memory management routines. Our shape algebra is
*				implemented using linked lists of spans and segments with
*				the structures allocated dymanically from large arrays
*				managed with a free list.
*
*               If we run out of local memory in the span and segment pools,
*				we attempt to allocate more memory to the region pools on
*				the fly. This allows us to start off with a reasonably small
*				amount of memory, and expand if the application demand for
*				complex regions is higher than normal.
*
****************************************************************************/

#include "mgl.h"

/*------------------------- Implementation --------------------------------*/

/****************************************************************************
REMARKS:
Creates the initial segment list by allocating and building the segment
list free list pool. This is called by the MGL when it first starts up.
{secret}
****************************************************************************/
void _MGL_createSegmentList(void)
{
	_MGL_createBlockList(&_MGL_segList.bl,DEF_NUM_SEGMENTS,sizeof(segment_t));
	_MGL_segList.freeList = _MGL_buildFreeList(&_MGL_segList.bl);
}

/****************************************************************************
REMARKS:
Resizes the segment list if full by increasing its size by the original
default size and building a new free list in the resized portion.
{secret}
****************************************************************************/
static void resizeSegmentList(void)
{
	_MGL_resizeBlockList(&_MGL_segList.bl);
	_MGL_segList.freeList = _MGL_buildFreeList(&_MGL_segList.bl);
}

/****************************************************************************
REMARKS:
Frees the segment list. This is called by the MGL when it shuts down.
{secret}
****************************************************************************/
void _MGL_freeSegmentList(void)
{ _MGL_freeBlockList(&_MGL_segList.bl); }

/****************************************************************************
RETURNS:
Pointer to new segment

REMARKS:
Allocates space for a new segment in the segment pool from the free list.
If there are no more segments, we attempt to re-size the segment pool
which will bomb out if we run out of memory.
{secret}
****************************************************************************/
segment_t *_MGL_newSegment(void)
{
	segment_t	*p;

	if (_MGL_segList.freeList == NULL)
		resizeSegmentList();
	p = _MGL_segList.freeList;
	_MGL_segList.freeList = FREELIST_NEXT(p);
	return p;
}

/****************************************************************************
PARAMETERS:
s	- Pointer to source segment

RETURNS:
Pointer to copied segment

REMARKS:
Copies the definitions for an entire segment list and returns the pointer
of the first segment in the list. The space for the copied segment is
allocated from the memory pool.
{secret}
****************************************************************************/
segment_t * _MGL_copySegment(
	segment_t *s)
{
	segment_t	*head,*p,**prev;

	for (prev = &head; s; s = s->next) {
		*prev = p = _MGL_newSegment();
		p->x = s->x;
		prev = &p->next;
		}
	*prev = NULL;
	return head;
}

/****************************************************************************
PARAMETERS:
s		- Pointer to source segment
xOffset	- X coordinate offset value

RETURNS:
Pointer to copied segment

REMARKS:
Copies the definitions for an entire segment list and returns the pointer
of the first segment in the list. The space for the copied segment is
allocated from the memory pool. This version offsets all the X coordinates
in the segment list by the specified X offset value.
{secret}
****************************************************************************/
segment_t * _MGL_copySegmentOfs(
	segment_t *s,
	int xOffset)
{
	segment_t	*head,*p,**prev;

	for (prev = &head; s; s = s->next) {
		*prev = p = _MGL_newSegment();
		p->x = xOffset + s->x;
		prev = &p->next;
		}
	*prev = NULL;
	return head;
}
