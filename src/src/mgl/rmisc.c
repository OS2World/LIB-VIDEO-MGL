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
* Description:	Miscellaneous region manipulation routines.
*
****************************************************************************/

#include "mgl.h"				/* Private declarations					*/

/*------------------------- Implementation --------------------------------*/

#undef	MGL_isSimpleRegion

/****************************************************************************
DESCRIPTION:
Offsets a region by the specified amount.

HEADER:
mgraph.h

PARAMETERS:
r	- Region to offset
dx	- Amount to offset x coordinates by
dy	- Amount to offset y coordinates by

REMARKS:
This function offsets the specified region by the dx and dy coordinates,
by modifying all the coordinate locations for every rectangle in the union
of rectangles that constitutes the region by the specified coordinates.

SEE ALSO:
MGL_unionRegion, MGL_diffRegion, MGL_sectRegion
****************************************************************************/
void MGLAPI MGL_offsetRegion(
	region_t *r,
	int dx,
	int dy)
{
	span_t		*s;
	segment_t	*p;

	/* Bail out for empty regions */
	if (MGL_emptyRegion(r))
		return;

	MGL_offsetRect(r->rect,dx,dy);
	for (s = r->spans; s; s = s->next) {
		s->y += dy;
		for (p = s->seg; p; p = p->next)
			p->x += dx;
		}
}

/****************************************************************************
DESCRIPTION:
Determines if two regions are equal.

HEADER:
mgraph.h

PARAMETERS:
r1	- First region to compare
r2	- Second region to compare

RETURNS:
True if the regions are equal, false if not.

REMARKS:
Determines if two regions are equal, by comparing the bounding rectangles
and the definitions for both of the regions.

SEE ALSO:
MGL_emptyRegion, MGL_unionRegion, MGL_diffRegion, MGL_sectRegion,
MGL_offsetRegion, MGL_ptInRegion, MGL_ptInRegionCoord
****************************************************************************/
ibool MGLAPI MGL_equalRegion(
	const region_t *r1,
	const region_t *r2)
{
	span_t		*s1,*s2;
	segment_t	*seg1,*seg2;

	s1 = r1->spans;
	s2 = r2->spans;
	if (!s1 && !s2)
		return MGL_equalRect(r1->rect,r2->rect);
	if (!s1 || !s2)
		return false;

	while (s1 && s2) {
		if (s1->y != s2->y)
			return false;
		seg1 = s1->seg;
		seg2 = s2->seg;
		while (seg1 && seg2) {
			if (seg1->x != seg2->x)
				return false;
			seg1 = seg1->next;
			seg2 = seg2->next;
			}
		if (!seg1 || !seg2)
			return false;
		s1 = s1->next;
		s2 = s2->next;
		}
	return !s1 && !s2;
}

/****************************************************************************
DESCRIPTION:
Determines if a region is empty.

HEADER:
mgraph.h

PARAMETERS:
r	- region to test

RETURNS:
True if region is empty, false if not.

REMARKS:
Determines if a region is empty or not. A region is defined as being
empty if the bounding rectangle's right coordinate is less than or equal
to the left coordinate, or if the bottom coordinate is less than or equal
to the top coordinate.

SEE ALSO:
MGL_equalRegion, MGL_unionRegion, MGL_diffRegion, MGL_sectRegion,
MGL_offsetRegion, MGL_ptInRegion, MGL_ptInRegionCoord
****************************************************************************/
ibool MGLAPI MGL_emptyRegion(
	const region_t *r)
{
	return MGL_emptyRect(r->rect);
}

/***************************************************************************
DESCRIPTION:
Returns true if a region is a simple region, otherwise false.

HEADER:
mgraph.h

PARAMETERS:
r	- Region to test.

REMARKS:
This function determines if the region is simple or not. A simple region is
one that consists of only a single rectangle. This function will not work
properly if the region has been through a number of region algebra routines
with other non-simple regions, even though the end result may be a single
rectangle.

SEE ALSO:
MGL_unionRegion, MGL_diffRegion, MGL_sectRegion
***************************************************************************/
ibool MGL_isSimpleRegion(
	region_t r);
/* Implemented as a macro */
