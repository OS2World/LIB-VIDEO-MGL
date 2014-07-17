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
* Description:	Polyline drawing routines
*
****************************************************************************/

#include "mgl.h"				/* Private declarations					*/

/*------------------------- Implementation --------------------------------*/

/****************************************************************************
DESCRIPTION:
Draws a set of pixels.

HEADER:
mgraph.h

PARAMETERS:
count	- Number of vertices in polyline
vArray	- Array of coordinates to draw the pixels at

REMARKS:
This function draws a set of pixels in the current color at the locations passed in the
vArray parameter.

SEE ALSO:
MGL_polyLine, MGL_polyMarker
****************************************************************************/
void MGLAPI MGL_polyPoint(
	int count,
	point_t *vArray)
{
	MGL_beginPixel();
	while (count--) {
		MGL_pixel(*vArray);
		vArray++;
		}
	MGL_endPixel();
}

/****************************************************************************
DESCRIPTION:
Draws a set of connected lines.

HEADER:
mgraph.h

PARAMETERS:
count	- Number of vertices in polyline
vArray	- Array of vertices in the polyline

REMARKS:
This function draws a set of connected line (a polyline). The coordinates of the
polyline are specified by vArray, and the lines are drawn in the current drawing
attributes.

Note that the polyline is not closed by default, so if you wish to draw the outline of
a polygon, you will need to add the starting point to the end of the vertex array.

SEE ALSO:
MGL_polyMarker, MGL_polyPoint
****************************************************************************/
void MGLAPI MGL_polyLine(
	int count,
	point_t *vArray)
{
	int		i;
	point_t	*p1,*p2;

	p1 = vArray;
	p2 = vArray+1;
	i = count;
	while (--i) {
		MGL_lineExt(*p1,*p2,i == 1);
		p1 = p2++;
		}
}
