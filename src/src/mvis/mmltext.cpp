/****************************************************************************
*
*					 MegaVision Application Framework
*
*	   A C++ GUI Toolkit for the SciTech Multi-platform Graphics Library
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
* Language:		C++ 3.0
* Environment:	Any
*
* Description:	Member functions for the MVMultiLineText class.
*
****************************************************************************/

#include "mvis/mvision.hpp"
#include "mvis/msttext.hpp"
#include "mvis/mfontmgr.hpp"
#include <string.h>

/*----------------------------- Implementation ----------------------------*/

MVMultiLineText::MVMultiLineText(MGLDevCtx& dc,const MVRect& bounds,
	const char *text,uint flags,int color)
	: MVStaticText(dc,bounds,text,flags,color)
/****************************************************************************
*
* Function:		MVMultiLineText::MVMultiLineText
* Parameters:	bounds	- Bounding box for the text
*				text	- Pointer to the text to display
*				flags	- Static text creation flags
*
* Description:	Constructor for the MVMultiLineText class. Note that the
*				text is _not_ copied onto the heap, but is simply
*				used as a reference. This is more efficient, but also
*				allows the text to be quickly set to another value by
*				pointing it somewhere else.
*
****************************************************************************/
{
	setText(text);
}

const char *MVMultiLineText::setText(const char *t)
/****************************************************************************
*
* Function:		MVMultiLineText::setText
* Parameters:	t	- New string for the text
* Returns:		Pointer to old text.
*
* Description:	Sets the text for the static text item, and also
*				re-computes the position to draw the text at.
*
****************************************************************************/
{
	const char *ret = text;
	text = t;

	// Set the current font and size, and obtain the fonts metrics
	metrics_t m;
	useFont(fmSystemFont);
	dc.getFontMetrics(m);

	// Compute the location to draw the text at. First we need to work out
	// the number of lines of text that will be drawn.
	int	maxTextWidth;
	lines = MV_countLines(dc,t,size.x,maxTextWidth,(flags & sfWrap));

	int hjust = (flags & sfLeftJust ? MGL_LEFT_TEXT :
				(flags & sfRightJust ? MGL_RIGHT_TEXT : MGL_CENTER_TEXT));
	int vjust = (flags & sfTopJust ? MGL_TOP_TEXT :
				(flags & sfBottomJust ? MGL_BOTTOM_TEXT : MGL_CENTER_TEXT));
	tjust.setJustification(hjust,vjust);

	MVRect r(bounds);
	r.moveTo(0,0);

	start.x = 	(hjust == MGL_LEFT_TEXT ? 	 r.left() :
				(hjust == MGL_CENTER_TEXT ? (r.left() + r.right())/2 :
										 r.right()-1));
	start.y =	(vjust == MGL_TOP_TEXT ? 	 r.top() :
				(vjust == MGL_CENTER_TEXT ? ((r.top() + r.bottom()
										 - dc.textHeight()*(lines-1))/2) :
										 r.bottom()-1
										 - dc.textHeight()*(lines-1)));
	repaint();
	return ret;
}

void MVMultiLineText::draw(const MVRect&)
/****************************************************************************
*
* Function:		MVMultiLineText::draw
* Parameters:	clip	- Clipping rectangle for the view
*
* Description:	Draw's the static text item.
*
****************************************************************************/
{
	MVTextJust old;
	old.save(dc);
	tjust.use(dc);

	useFont(fmSystemFont);
	dc.setColor(getColor(scBackground));
	dc.fillRect(bounds);
	dc.setColor(getColor(textColor));

	// Loop around drawing each line of text, wrapping at the right edge
	// if necessary.
	char 	*startText = (char*)text,*p = startText;
	int 	x = start.x + bounds.left(),y = start.y + bounds.top();
	int		width = 0,height = dc.textHeight();
	ibool 	drawIt = false,wrap = (flags & sfWrap);
	char 	oldChar;
	char 	*lastSpace = NULL;

	while (*p) {
		if (*p == '\n') {
			drawIt = true;
			width = 0;
			lastSpace = NULL;
			}
		else {
			if (*p == ' ')
				lastSpace = p;
			width += dc.charWidth(*p);
			if (wrap && lastSpace && width > size.x) {
				p = lastSpace;	// Back up to previous space character

				drawIt = true;
				width = 0;
				lastSpace = NULL;
				}
			}
		if (drawIt) {
			oldChar = *p;
			*p = 0;
			dc.drawStr(x,y,startText);
			y += height;
			*p = oldChar;
			startText = p+1;
			drawIt = false;
			}
		p++;
		}

	if (*startText != 0)
		dc.drawStr(x,y,startText);

	old.use(dc);
}
