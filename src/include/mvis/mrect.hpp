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
* Description:	Header file to define the TRect class. We simply use the
*				Rect class provided by the MGL and provide a typedef.
*
****************************************************************************/

#ifndef	__MVIS_MRECT_HPP
#define	__MVIS_MRECT_HPP

#ifndef	__MGRAPH_HPP
#include "mgraph.hpp"
#endif

/*----------------------- Macro and type definitions ----------------------*/

typedef	MGLRect		MVRect;
typedef	MGLRegion	MVRegion;

#endif	// __MVIS_MRECT_HPP
