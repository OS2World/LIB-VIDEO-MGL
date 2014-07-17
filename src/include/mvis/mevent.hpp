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
* Description:	Header file for the event collection subsystem. We rely on
*				the SciTech MGL event module to collect the events for us,
*				which we then stick into a more suitable structure for
*				C++.
*
****************************************************************************/

#ifndef	__MVIS_MEVENT_HPP
#define	__MVIS_MEVENT_HPP

#ifndef	__MVIS_MPOINT_HPP
#include "mvis/mpoint.hpp"
#endif

#ifndef	__MGRAPH_H
#include "mgraph.h"
#endif

/*------------------------------ Constants --------------------------------*/

const ulong

// Event codes

	evKeyDown		= EVT_KEYDOWN,		// Key was depressed
	evKeyAuto		= EVT_KEYREPEAT,	// Key auto repeat
	evKeyUp			= EVT_KEYUP,		// Key was released
	evMouseDown		= EVT_MOUSEDOWN,	// Mouse button was depressed
	evMouseAuto		= EVT_MOUSEAUTO,	// Mouse button auto repeat
	evMouseUp		= EVT_MOUSEUP,		// Mouse button was released
	evMouseMove		= EVT_MOUSEMOVE,	// Mouse cursor was moved
	evJoyClick		= EVT_JOYCLICK,		// Joystick button state changed
	evJoyMove		= EVT_JOYMOVE,		// Joytick position changed
	evCommand		= 0x00010000,		// Event is a command message
	evBroadcast		= 0x00020000,		// Event is a broadcast message
	evRepaint		= 0x00040000,		// Event is a repaint message

// Event code masks

	evNothing		= EVT_NULLEVT,
	evMouse			= evMouseDown | evMouseUp | evMouseMove | evMouseAuto,
	evKeyboard		= evKeyDown | evKeyAuto | evKeyUp,
	evJoystick		= evJoyClick | evJoyMove,
	evAll			= 0xFFFFFFFF,		// All events mask

// Mouse button masks

	mbLeftButton	= EVT_LEFTBMASK,
	mbMidButton		= EVT_MIDDLEBMASK,
	mbRightButton	= EVT_RIGHTBMASK,
	mbDblClick		= EVT_DBLCLICK,
	mbBothButtons	= mbLeftButton | mbRightButton,
	mbAllButtons	= mbLeftButton | mbMidButton | mbRightButton,

// Event modifier masks

	mdLeftButton	= EVT_LEFTBUT,
	mdMidButton		= EVT_MIDDLEBUT,
	mdRightButton	= EVT_RIGHTBUT,
	mdLeftShift		= EVT_LEFTSHIFT,
	mdRightShift	= EVT_RIGHTSHIFT,
	mdLeftCtrl		= EVT_LEFTCTRL,
	mdRightCtrl		= EVT_RIGHTCTRL,
	mdLeftAlt		= EVT_LEFTALT,
	mdRightAlt		= EVT_RIGHTALT,
	mdShift			= EVT_SHIFTKEY,
	mdCtrl			= EVT_CTRLSTATE,
	mdAlt			= EVT_ALTSTATE,
	mdMouse			= mdLeftButton | mdMidButton | mdRightButton,
	mdKey			= mdShift | mdCtrl | mdAlt,

//  Event masks

	positionalEvents= evMouse | evJoystick,
	focusedEvents	= evKeyboard | evCommand;

/*-------------------------- Class definitions ----------------------------*/

#pragma pack(1)

struct MVMouseEvent {
	ushort	buttons;				// Button code mask
	short	doubleClick;			// Event was a double click
	ulong	modifiers;				// Modifier flags
	};

struct MVJoystickEvent {
	ulong	buttons;				// Button code mask
	ulong	modifiers;				// Modifier flags
	};

struct MVKeyEvent {
	union {
		ushort		keyCode;		// Combined ASCII and scan codes
		struct {
			uchar	charCode;		// ASCII code
			uchar	scanCode;		// Scan code
			} charScan;
		};
	short	count;					// Repeat count
	ulong	modifiers;				// Modifier flags
	};

struct MVMessageEvent {
	ulong	command;				// Command for this message
	union {
		void*	infoPtr;			// Various message data types
		long	infoLong;
		ushort	infoWord;
		short	infoInt;
		uchar	infoByte;
		char	infoChar;
		};
	};

class MVView;

struct MVEvent {
	MVView	*which;					// Which view to send event to
	ulong	what;					// Event code
	ulong	when;					// Clock ticks when message occurred
	MVPoint	where;					// Where the event occurred
	MVPoint	relative;				// Relative mouse movement
	union {
		MVMouseEvent	mouse;		// Mouse event information
		MVJoystickEvent	joy;		// Joystick event information
		MVKeyEvent		key;		// Key event information
		MVMessageEvent	message;	// Message event information
		};
	int     res[2];					// Reserved
	};

#pragma pack()

//---------------------------------------------------------------------------
// The following class implements the event queue mechanism for the
// class library. It is basically C++ wrapper to the code in the Event
// module of the MGL.
//---------------------------------------------------------------------------

class MVEventQueue {
public:
			// Obtain the next event
			ibool getNext(MVEvent& event,ulong mask = evAll)
			{ return EVT_getNext((event_t*)&event,mask); };

			// Look at the next event without removing it
			ibool peekNext(MVEvent& event,ulong mask = evAll)
			{ return EVT_peekNext((event_t*)&event,mask); };

			// Post an event to the event queue
			ibool post(const MVEvent& event);

			// Flush all events specified by the mask
			void flush(ulong mask = evAll)	{ EVT_flush(mask); };
	};

/*--------------------------- Global Variables ----------------------------*/

extern MVEventQueue	MV_eventQueue;

#endif	// __MVIS_MEVENT_HPP
