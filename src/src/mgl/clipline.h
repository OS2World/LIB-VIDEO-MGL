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
* 		     		Portions Copyright (c) 1987 X Consortium
* 			Portions Copyright 1987 by Digital Equipment Corporation,
*							Maynard, Massachusetts.
*
* Language:		ANSI C
* Environment:	Any
*
* Description:	Header file for internal definitions for line clipping.
*
****************************************************************************/

#ifndef	__CLIPLINE_H
#define	__CLIPLINE_H

#define OUT_LEFT  	0x08
#define OUT_RIGHT 	0x04
#define OUT_ABOVE 	0x02
#define OUT_BELOW 	0x01

/* Macro to set the clipping outcodes, given inclusive clip rectangle */

#define MIOUTCODES(_result, _x, _y, _xmin, _ymin, _xmax, _ymax)	\
{																\
	if      ( (_x) < (_xmin))	(_result) |= OUT_LEFT; 			\
	else if ( (_x) > (_xmax))  	(_result) |= OUT_RIGHT; 		\
	if      ( (_y) < (_ymin)) 	(_result) |= OUT_ABOVE; 		\
	else if ( (_y) > (_ymax)) 	(_result) |= OUT_BELOW;			\
}

/* Macro to set the clipping outcodes, given exclusive clip rectangle */

#define OUTCODES(_result, _x, _y)								\
{																\
	if      ( (_x) <  (clipLeft))  	(_result) |= OUT_LEFT; 		\
	else if ( (_x) >= (clipRight))  (_result) |= OUT_RIGHT; 	\
	if      ( (_y) <  (clipTop))  	(_result) |= OUT_ABOVE; 	\
	else if ( (_y) >= (clipBottom)) (_result) |= OUT_BELOW;		\
}

/* Macros to swap variables values */

#define SWAPINT(i, j) \
{  register int _t = i;  i = j;  j = _t; }

#define SWAPINT_PAIR(x1, y1, x2, y2)	\
{   int t = x1;  x1 = x2;  x2 = t;		\
		t = y1;  y1 = y2;  y2 = t;		\
}

/* Macros to test which octant a line is in */

#define IsXMajorOctant(_octant)         ((_octant) & gaLineXMajor)
#define IsYMajorOctant(_octant)         (!((_octant) & gaLineXMajor))
#define IsXDecreasingOctant(_octant)    (!((_octant) & gaLineXPositive))
#define IsYDecreasingOctant(_octant)    (!((_octant) & gaLineYPositive))

/* Bit codes for the terms of the 16 clipping equations defined below. */

#define T_2NDX          (1 << 0)
#define T_2MDY          (0)                             /* implicit term */
#define T_DXNOTY        (1 << 1)
#define T_DYNOTX        (0)                             /* implicit term */
#define T_SUBDXORY      (1 << 2)
#define T_ADDDX         (T_DXNOTY)                      /* composite term */
#define T_SUBDX         (T_DXNOTY | T_SUBDXORY)         /* composite term */
#define T_ADDDY         (T_DYNOTX)                      /* composite term */
#define T_SUBDY         (T_DYNOTX | T_SUBDXORY)         /* composite term */
#define T_BIASSUBONE    (1 << 3)
#define T_SUBBIAS       (0)                             /* implicit term */
#define T_DIV2DX        (1 << 4)
#define T_DIV2DY        (0)                             /* implicit term */
#define T_ADDONE        (1 << 5)

/* Bit masks defining the 16 equations used in miZeroClipLine. */

#define EQN1    (T_2MDY | T_ADDDX | T_SUBBIAS    | T_DIV2DX)
#define EQN1B   (T_2MDY | T_ADDDX | T_SUBBIAS    | T_DIV2DX)
#define EQN2    (T_2MDY | T_ADDDX | T_BIASSUBONE | T_DIV2DX)
#define EQN2B   (T_2MDY | T_ADDDX | T_BIASSUBONE | T_DIV2DX)

#define EQN3    (T_2MDY | T_SUBDY | T_BIASSUBONE | T_DIV2DX | T_ADDONE)
#define EQN3B   (T_2MDY | T_ADDDY | T_BIASSUBONE | T_DIV2DX)
#define EQN4    (T_2MDY | T_SUBDY | T_SUBBIAS    | T_DIV2DX | T_ADDONE)
#define EQN4B   (T_2MDY | T_ADDDY | T_SUBBIAS    | T_DIV2DX)

#define EQN5    (T_2NDX | T_SUBDX | T_BIASSUBONE | T_DIV2DY | T_ADDONE)
#define EQN5B   (T_2NDX | T_ADDDX | T_BIASSUBONE | T_DIV2DY)
#define EQN6    (T_2NDX | T_SUBDX | T_SUBBIAS    | T_DIV2DY | T_ADDONE)
#define EQN6B   (T_2NDX | T_ADDDX | T_SUBBIAS    | T_DIV2DY)

#define EQN7    (T_2NDX | T_ADDDY | T_SUBBIAS    | T_DIV2DY)
#define EQN7B   (T_2NDX | T_ADDDY | T_SUBBIAS    | T_DIV2DY)
#define EQN8    (T_2NDX | T_ADDDY | T_BIASSUBONE | T_DIV2DY)
#define EQN8B   (T_2NDX | T_ADDDY | T_BIASSUBONE | T_DIV2DY)

#endif	/* __CLIPLINE_H */
