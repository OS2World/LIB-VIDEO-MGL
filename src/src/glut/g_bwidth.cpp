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
* 				 Portions Copyright (c) Mark J. Kilgard, 1994.
*
* Language:   	ANSI C
* Environment:  Any
*
* Description:  SciTech MGL minimal GLUT implementation for fullscreen
*				and windowed operation.
*
****************************************************************************/

#include "glutint.h"

/*------------------------- Implementation --------------------------------*/

int glutBitmapWidth(GLUTbitmapFont font, int c)
{
  BitmapFontPtr fontinfo = (BitmapFontPtr) font;
  BitmapCharPtr ch;

  if (c < fontinfo->first || c >= fontinfo->first + fontinfo->num_chars)
    return 0;
  ch = fontinfo->ch[c - fontinfo->first];
  if (ch)
	return (int)ch->advance;
  else
    return 0;
}

int glutBitmapLength(GLUTbitmapFont font, unsigned char *string)
{
  int c, length;
  BitmapFontPtr fontinfo = (BitmapFontPtr) font;
  BitmapCharPtr ch;

  length = 0;
  for (; *string != '\0'; string++) {
    c = *string;
    if (c < fontinfo->first || c >= fontinfo->first + fontinfo->num_chars) {
      ch = fontinfo->ch[c - fontinfo->first];
      if (ch)
		length += (int)ch->advance;
    }
  }
  return length;
}
