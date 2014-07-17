/****************************************************************************
*
*						   The Zen Timer Library
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
* Language:		ANSI C, C++ 3.0
* Environment:	Any
*
* Description:	Header file for the Zen Timer library. Provides a number
*				of routines to accurately time segments of code. A long
*				period timer is provided to time code that takes up to
*				one hour to execute, with microsecond precision, and an
*				ultra long period timer for timing code that takes up to
*				24 hours to execute (raytracing etc).
*
*				We also provide a set of convenience C++ classes to
*				manipulate the Zen Timers.
*
****************************************************************************/

#ifndef	__ZTIMER_H
#define	__ZTIMER_H

#include "ztimerc.h"

/*--------------------------- Class Definitions ---------------------------*/

#ifdef	__cplusplus

#include <iostream.h>
#include <stdio.h>

//---------------------------------------------------------------------------
// Long Period Zen Timer class. This can be used to time code that takes up
// to 1 hour to execute between calls to start() and stop() or lap(). The
// aggregate count can be up to 2^32 - 1 microseconds (about 1 hour
// and 10 mins).
//---------------------------------------------------------------------------

class LZTimer {
protected:
	LZTimerObject	tm;
	ulong			_count;
	short			_overflow;

			// Compute the current count
	inline	void computeTime();

public:
			// Constructor
	inline	LZTimer()		{ ZTimerInit(); _count = 0; _overflow = false; };

			// Method to start the timer
	inline	void start()	{ LZTimerOnExt(&tm); };

			// Method to restart the timer
	inline	void restart()	{ reset(); start(); };

			// Method to return the current count without stop timer
	inline	ulong lap()		{ return _count + LZTimerLapExt(&tm); };

			// Method to stop the timer
	inline	void stop()		{ LZTimerOffExt(&tm); computeTime(); };

			// Method to return the current count
	inline	ulong count()	{ return _count; };

			// Method to reset the timer to a zero count
	inline	void reset()	{ _count = 0; _overflow = false; };

			// Method to determine if overflow occurred
	inline	ibool overflow()	{ return _overflow; };

			// Method to return timer resolution (seconds in a count).
	inline	float resolution()	{ return (float)LZTIMER_RES; };

			// Method to display the timed count in seconds
	friend	ostream& operator << (ostream& o,LZTimer& timer);
	};

//---------------------------------------------------------------------------
// Ultra Long Period Zen Timer class. This can be used to time code that
// takes up 24 hours total to execute between calls to start() and stop().
// The resolution of this timer depends on the resolution of the underlying
// OS system timers and will vary from system to system.
//---------------------------------------------------------------------------

class ULZTimer {
protected:
	ulong	_count,_start,_finish;

public:
			// Constructor
	inline	ULZTimer()		{ ZTimerInit(); _count = 0; };

			// Method to start the timer
	inline	void start()	{ _start = ULZReadTime(); };

			// Method to restart the timer
	inline	void restart()	{ reset(); start(); };

			// Method to return the current count without stoping timer
	inline	ulong lap()		{ return ULZElapsedTime(_start,ULZReadTime()); };

			// Method to stop the timer
	inline	void stop();

			// Method to return the current count
	inline	ulong count()	{ return _count; };

			// Method to reset the timer to a zero count
	inline	void reset()	{ _count = 0; };

			// Method to return timer resolution (seconds in a count).
	inline	float resolution()	{ float resolution; ULZTimerResolution(&resolution); return resolution; };

			// Method to display the timed count in seconds
	friend	ostream& operator << (ostream& o,ULZTimer& timer);
	};


/*------------------------- Inline member functions -----------------------*/

inline void LZTimer::computeTime()
{
	// Compute the time elapsed between calls to LZTimerOn() and LZTimerOff()
	// and add it to the current count.
	if (!overflow()) {
		ulong newcount = LZTimerCountExt(&tm);
		if (newcount == 0xFFFFFFFFL)
			_overflow = true;
		else
			_count += newcount;
		}
}

inline void ULZTimer::stop()
{
	// Routine to stop the ultra long period timer.
	_finish = ULZReadTime();
	_count += ULZElapsedTime(_start,_finish);
}

inline ostream& operator << (ostream& o,LZTimer& timer)
{
	char	buf[40];

	if (!timer.overflow()) {
		sprintf(buf, "%.6f", timer.count() * timer.resolution());
		o << buf;
		}
	else
		o << "overflow";
	return o;
}

inline ostream& operator << (ostream& o,ULZTimer& timer)
{
	char	buf[40];

	sprintf(buf, "%.1f", timer.count() * timer.resolution());
	return o << buf;
}

#endif	/* __cplusplus */

#endif	/* __ZTIMER_H */
