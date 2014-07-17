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
* Description:	Convex polygon drawing routines with clipping. Polygon
*				clipping is done on a scissoring basis, where each scan
*				line is clipped individually, rather than using a routine
*				such as the Sutherland Hodgeman polygon clipping algorithm.
*				This approach has been taken since we don't expect to clip
*				polygons all that often, and those that are clipped will
*				only require a small amount of clipping.
*
* 				This algorithm is a lot faster than the more general complex
*				polygon scan conversion algorithm, since we can take
*				advantage of the fact that every scan line in the polygon
*				will only cross a maximum of two actives edges in the
*				polygon at a time.
*
****************************************************************************/

#include "mgl.h"

/*------------------------- Implementation --------------------------------*/

/****************************************************************************
REMARKS:
Renders a flat shaded trapezoid with clipping.
****************************************************************************/
void MGLAPI _MGL_clipTrap(
	GA_trap *trap)
{
	fix32_t	x1 = trap->x1;
	fix32_t x2 = trap->x2;
	fix32_t	slope1 = trap->slope1;
	fix32_t slope2 = trap->slope2;
	int ix1,ix2,y = trap->y,count = trap->count;

	while (count--) {
		if (y >= DC.intClipRect.top && y < DC.intClipRect.bottom) {
			ix1 = MGL_FIXROUND(x1);
			ix2 = MGL_FIXROUND(x2);
			if (ix2 < ix1)
				SWAP(ix1,ix2);
			ix1 = MAX(ix1,DC.intClipRect.left);
			ix2 = MIN(ix2,DC.intClipRect.right);
			if (ix1 < ix2)
				DC.r.cur.DrawRect(ix1,y,ix2-ix1,1);
			}
		x1 += slope1;
		x2 += slope2;
		y++;
		}
	trap->x1 = x1;
	trap->x2 = x2;
	trap->y = y;
}

/****************************************************************************
PARAMETERS:
count	- Number of vertices to draw
vArray	- Array of vertices
vinc	- Increment to get to next vertex
xOffset	- Offset of x coordinates
yOffset	- Offset of y coordinates

REMARKS:
Scan converts a filled convex polygon. A "convex" polygon is defined as a
polygon such that eveyr horizontal line drawn through the polygon would
cross exactly two active edges (neither horizontal lines nor zero-length
edges count as active edges; both are acceptable anywhere in the polygon).
Right & left edges may cross (polygons may be nonsimple). Attempting to
scan convert a polygon that does non fit this description will produce
unpredictable results.

All vertices are offset by (xOffset,yOffset).
****************************************************************************/
void _MGL_convexPolygon(
	int count,
	fxpoint_t *vArray,
	int vinc,
	fix32_t xOffset,
	fix32_t yOffset)
{
	int			i,minIndex;
	fxpoint_t	*p,*v,*lArray,*rArray;
	fix32_t		y,endy;
	trapFunc	trap;

	/* Get memory in which to store left and right edge arrays			*/
	lArray = _MGL_buf;
	rArray = (fxpoint_t*)((uchar*)_MGL_buf + (count+1) * sizeof(fxpoint_t));

	/* Scan the list of vertices to determine the lowest vertex 		*/
	minIndex = 0;
	y = vArray->y;
	for (i = 1, p = VTX(vArray,vinc,1); i < count; i++, INCVTX(p,vinc)) {
		if (p->y < y) {
			y = p->y;
			minIndex = i;
			}
		}

	/* Copy all vertices from lowest to end into left list 			*/
	v = VTX(vArray,vinc,minIndex);
	for (i = minIndex,p = lArray; i < count; i++,INCVTX(v,vinc))
		*p++ = *v;

	/* Copy all vertices from start to lowest into left list 			*/
	for (i = 0,v = vArray; i < minIndex; i++,INCVTX(v,vinc))
		*p++ = *v;
	p->y = -1;                    	/* Terminate the list				*/

	/* Copy the right list as the left list in reverse order */
	p = rArray;
	*p++ = *lArray;					/* First element the same			*/
	for (i = count-1; i > 0; i--)
		*p++ = lArray[i];
	p->y = -1;						/* Terminate the list				*/

	/* If we are rendering in non-REPLACE or patterned mode, then we
	 * need to use the C version of the trapezoid filling routine that
	 * will correctly handle these cases.
	 */
	if (DC.a.writeMode != MGL_REPLACE_MODE)
		DC.doClip = true;
	trap = DC.doClip ? _MGL_clipTrap : DC.r.cur.DrawTrap;

	/* Setup for rendering the first edge in left list */
	while (_MGL_computeSlope(lArray,lArray+1,&DC.tr.slope1) <= 0) {
		lArray++;
		if ((--count) == 0)
			return;					/* Bail out for zero height polys	*/
		}

	/* Setup for rendering the first edge in right list */
	while (_MGL_computeSlope(rArray,rArray+1,&DC.tr.slope2) <= 0)
		rArray++;

	/* Now render the polygon as a series of trapezoidal slices			*/
	DC.tr.x1 = lArray->x+xOffset;
	DC.tr.x2 = rArray->x+xOffset;
	y = lArray->y;
	DC.tr.y = MGL_FIXROUND(y+yOffset);
	for (;;) {
		endy = lArray[1].y;
		if (endy > rArray[1].y)
			endy = rArray[1].y;

		/* Draw the trapezoid */
		DC.tr.count = MGL_FIXROUND(endy) - MGL_FIXROUND(y);
		trap(&DC.tr);

		/* Advance the left and right edges */
		if (lArray[1].y == endy) {	/* Has left edge ended?				*/
			lArray++;
			while ((count = _MGL_computeSlope(lArray,lArray+1,&DC.tr.slope1)) <= 0) {
				lArray++;
				if (count < 0)
					return;			/* No more left edges				*/
				}
			DC.tr.x1 = lArray->x+xOffset;
			}
		if (rArray[1].y == endy) {	/* Has right edge ended?			*/
			rArray++;
			while ((count = _MGL_computeSlope(rArray,rArray+1,&DC.tr.slope2)) <= 0) {
				rArray++;
				if (count < 0)
					return;			/* No more right edges				*/
				}
			DC.tr.x2 = rArray->x+xOffset;
			}
		y = endy;
		}
}
