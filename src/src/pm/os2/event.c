/****************************************************************************
*
*					SciTech OS Portability Manager Library
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
* Environment:  IBM PC (OS/2)
*
* Description:	OS/2 implementation for the SciTech cross platform
*				event library.
*
****************************************************************************/

/*---------------------------- Global Variables ---------------------------*/

/* Define generous keyboard monitor circular buffer size to minimize
 * the danger of losing keystrokes
 */
#define KEYBUFSIZE  (EVENTQSIZE + 10)

static int		oldMouseState;			/* Old mouse state				 */
static ulong	oldKeyMessage;			/* Old keyboard state			 */
static ushort	keyUpMsg[256] = {0};	/* Table of key up messages		 */
static int		rangeX,rangeY;			/* Range of mouse coordinates	 */
static ulong	frequency;				/* Timer frequency				 */
HMOU			_EVT_hMouse;			/* Handle to the mouse driver	 */
HMONITOR        _EVT_hKbdMon;       	/* Handle to the keyboard driver */
TID             kbdMonTID = 0;      	/* Keyboard monitor thread ID    */
HEV             hevStart;           	/* Start event semaphore handle  */
BOOL            bMonRunning;        	/* Flag set if monitor thread OK */
HMTX            hmtxKeyBuf;         	/* Mutex protecting key buffer   */
KEYPACKET       keyMonPkts[KEYBUFSIZE]; /* Array of monitor key packets  */
int             kpHead = 0;         	/* Key packet buffer head        */
int             kpTail = 0;        		/* Key packet buffer tail        */

/*---------------------------- Implementation -----------------------------*/

/* These are not used under OS/2 */
#define	_EVT_disableInt()		1
#define	_EVT_restoreInt(flags)

/****************************************************************************
PARAMETERS:
scanCode    - Scan code to test

REMARKS:
This macro determines if a specified key is currently down at the
time that the call is made.
****************************************************************************/
#define _EVT_isKeyDown(scanCode)	(keyUpMsg[scanCode] != 0)

/****************************************************************************
REMARKS:
This function is used to return the number of ticks since system
startup in milliseconds. This should be the same value that is placed into
the time stamp fields of events, and is used to implement auto mouse down
events.
****************************************************************************/
ulong _EVT_getTicks(void)
{
	QWORD count;
	DosTmrQueryTime(&count);
	return (count.ulLo * 1000.0 / frequency);
}

/****************************************************************************
REMARKS:
Converts a mickey movement value to a pixel adjustment value.
****************************************************************************/
static int MickeyToPixel(
	int mickey)
{
	// TODO: We can add some code in here to handle 'acceleration' for
	//		 the mouse cursor. For now just use the mickeys.
	return mickey;
}

/* Some useful defines any typedefs used in the keyboard handling */
#define KEY_RELEASE             0x40

/****************************************************************************
REMARKS:
Pumps all messages in the message queue from OS/2 into our event queue.
****************************************************************************/
static void _EVT_pumpMessages(void)
{
	static KBDINFO		keyInfo;		/* Must not cross a 64K boundary */
	static KBDKEYINFO	key;			/* Must not cross a 64K boundary */
	static MOUQUEINFO	mqueue;			/* Must not cross a 64K boundary */
	static MOUEVENTINFO	mouse;			/* Must not cross a 64K boundary */
	static ushort		mWait;			/* Must not cross a 64K boundary */
	static KEYPACKET    kp;				/* Must not cross a 64K boundary */
	event_t				evt;
	int                 scan;

	/* Pump all keyboard messages from our circular buffer */
	for (;;) {
		/* Check that the monitor thread is still running */
		if (!bMonRunning)
			PM_fatalError("Keyboard monitor thread died!");

		/* Protect keypacket buffer with mutex */
		DosRequestMutexSem(hmtxKeyBuf, SEM_INDEFINITE_WAIT);
		if (kpHead == kpTail) {
			DosReleaseMutexSem(hmtxKeyBuf);
			break;
			}

		/* Read packet from circular buffer and remove it */
		memcpy(&kp, &keyMonPkts[kpTail], sizeof(KEYPACKET));
		if (++kpTail == KEYBUFSIZE)
			kpTail = 0;
		DosReleaseMutexSem(hmtxKeyBuf);

		/* Determine type of keyboard event */
		memset(&evt,0,sizeof(evt));
		if (kp.KbdDDFlagWord & KEY_RELEASE)
			evt.what = EVT_KEYUP;
		else
			evt.what = EVT_KEYDOWN;

		/* Convert keyboard codes */
		scan = kp.MonFlagWord >> 8;
		if (evt.what == EVT_KEYUP) {
			/* Get message for keyup code from table of cached down values */
			evt.message = keyUpMsg[scan];
			keyUpMsg[scan] = 0;
			oldKeyMessage = -1;
			}
		else {
			evt.message = ((ulong)scan << 8) | kp.XlatedChar;
			if (evt.message == keyUpMsg[scan]) {
				evt.what = EVT_KEYREPEAT;
				evt.message |= 0x10000;
				}
			oldKeyMessage = evt.message & 0x0FFFF;
			keyUpMsg[scan] = (ushort)evt.message;
			}

		 /* Convert shift state modifiers */
		 if (kp.ShiftState & 0x0001)
			 evt.modifiers |= EVT_RIGHTSHIFT;
		 if (kp.ShiftState & 0x0002)
			 evt.modifiers |= EVT_LEFTSHIFT;
		 if (kp.ShiftState & 0x0100)
			 evt.modifiers |= EVT_LEFTCTRL;
		 if (kp.ShiftState & 0x0200)
			 evt.modifiers |= EVT_LEFTALT;
		 if (kp.ShiftState & 0x0400)
			 evt.modifiers |= EVT_RIGHTCTRL;
		 if (kp.ShiftState & 0x0800)
			 evt.modifiers |= EVT_RIGHTALT;
		 oldMove = -1;

		 /* Add time stamp and add the event to the queue */
		 evt.when = key.time;
		 if (count < EVENTQSIZE)
			 addEvent(&evt);
		 }

	/* Don't just flush because that terminally confuses the monitor */
	do {
		KbdCharIn(&key, IO_NOWAIT, 0);
		} while (key.fbStatus & KBDTRF_FINAL_CHAR_IN);

	/* Pump all mouse messages */
	KbdGetStatus(&keyInfo,0);
	MouGetNumQueEl(&mqueue,_EVT_hMouse);
	while (mqueue.cEvents) {
		while (mqueue.cEvents--) {
			memset(&evt,0,sizeof(evt));
			mWait = MOU_NOWAIT;
			MouReadEventQue(&mouse,&mWait,_EVT_hMouse);

			/* Update the mouse position. We get the mouse coordinates
			 * in mickeys so we have to translate these into pixels and
			 * move our mouse position. If we don't do this, OS/2 gives
			 * us the coordinates in character positions since it still
			 * thinks we are in text mode!
			 */
			_EVT_mx += MickeyToPixel(mouse.col);
			_EVT_my += MickeyToPixel(mouse.row);
			if (_EVT_mx < 0) _EVT_mx = 0;
			if (_EVT_my < 0) _EVT_my = 0;
			if (_EVT_mx > rangeX)	_EVT_mx = rangeX;
			if (_EVT_my > rangeY)	_EVT_my = rangeY;
			evt.where_x = _EVT_mx;
			evt.where_y = _EVT_my;
			evt.relative_x = mouse.col;
			evt.relative_y = mouse.row;
			evt.when = key.time;
			if (mouse.fs & (MOUSE_BN1_DOWN | MOUSE_MOTION_WITH_BN1_DOWN))
				evt.modifiers |= EVT_LEFTBUT;
			if (mouse.fs & (MOUSE_BN2_DOWN | MOUSE_MOTION_WITH_BN2_DOWN))
				evt.modifiers |= EVT_RIGHTBUT;
			if (mouse.fs & (MOUSE_BN3_DOWN | MOUSE_MOTION_WITH_BN3_DOWN))
				evt.modifiers |= EVT_MIDDLEBUT;
			if (keyInfo.fsState & 0x0001)
				evt.modifiers |= EVT_RIGHTSHIFT;
			if (keyInfo.fsState & 0x0002)
				evt.modifiers |= EVT_LEFTSHIFT;
			if (keyInfo.fsState & 0x0100)
				evt.modifiers |= EVT_LEFTCTRL;
			if (keyInfo.fsState & 0x0200)
				evt.modifiers |= EVT_LEFTALT;
			if (keyInfo.fsState & 0x0400)
				evt.modifiers |= EVT_RIGHTCTRL;
			if (keyInfo.fsState & 0x0800)
				evt.modifiers |= EVT_RIGHTALT;

			/* Check for left mouse click events */
			/* 0x06 == (MOUSE_BN1_DOWN | MOUSE_MOTION_WITH_BN1_DOWN) */
			if (((mouse.fs & 0x0006) && !(oldMouseState & 0x0006))
					|| (!(mouse.fs & 0x0006) && (oldMouseState & 0x0006))) {
				if (mouse.fs & 0x0006)
					evt.what = EVT_MOUSEDOWN;
				else
					evt.what = EVT_MOUSEUP;
				evt.message = EVT_LEFTBMASK;
				oldMove = -1;
				if (count < EVENTQSIZE)
					addEvent(&evt);
				}

			/* Check for right mouse click events */
			/* 0x0018 == (MOUSE_BN2_DOWN | MOUSE_MOTION_WITH_BN2_DOWN) */
			if (((mouse.fs & 0x0018) && !(oldMouseState & 0x0018))
					|| (!(mouse.fs & 0x0018) && (oldMouseState & 0x0018))) {
				if (mouse.fs & 0x0018)
					evt.what = EVT_MOUSEDOWN;
				else
					evt.what = EVT_MOUSEUP;
				evt.message = EVT_RIGHTBMASK;
				oldMove = -1;
				if (count < EVENTQSIZE)
					addEvent(&evt);
				}

			/* Check for middle mouse click events */
			/* 0x0060 == (MOUSE_BN3_DOWN | MOUSE_MOTION_WITH_BN3_DOWN) */
			if (((mouse.fs & 0x0060) && !(oldMouseState & 0x0060))
					|| (!(mouse.fs & 0x0060) && (oldMouseState & 0x0060))) {
				if (mouse.fs & 0x0060)
					evt.what = EVT_MOUSEDOWN;
				else
					evt.what = EVT_MOUSEUP;
				evt.message = EVT_MIDDLEBMASK;
				oldMove = -1;
				if (count < EVENTQSIZE)
					addEvent(&evt);
				}

			/* Check for mouse movement event */
			if (mouse.fs & 0x002B) {
				evt.what = EVT_MOUSEMOVE;
				if (oldMove != -1) {
					evtq[oldMove].where_x = evt.where_x;/* Modify existing one  */
					evtq[oldMove].where_y = evt.where_y;
					}
				else {
					oldMove = freeHead;	/* Save id of this move event	*/
					if (count < EVENTQSIZE)
						addEvent(&evt);
					}
				}

			/* Save current mouse state */
			oldMouseState = mouse.fs;
			}
		MouGetNumQueEl(&mqueue,_EVT_hMouse);
		}
}

/****************************************************************************
REMARKS:
This macro/function is used to converts the scan codes reported by the
keyboard to our event libraries normalised format. We only have one scan
code for the 'A' key, and use shift modifiers to determine if it is a
Ctrl-F1, Alt-F1 etc. The raw scan codes from the keyboard work this way,
but the OS gives us 'cooked' scan codes, we have to translate them back
to the raw format.
****************************************************************************/
#define _EVT_maskKeyCode(evt)

/****************************************************************************
REMARKS:
Keyboard monitor thread. Needed to catch both keyup and keydown events.
****************************************************************************/
static void _kbdMonThread(
	void *params)
{
	APIRET  			rc;
	static KEYPACKET 	kp;
	static USHORT    	count = sizeof(KEYPACKET);
	static MONBUF    	monInbuf;
	static MONBUF    	monOutbuf;
	int					kpNew;

	/* Raise thread priority for higher responsiveness */
	DosSetPriority(PRTYS_THREAD, PRTYC_TIMECRITICAL, 0, 0);
	monInbuf.cb  = sizeof(monInbuf) - sizeof(monInbuf.cb);
	monOutbuf.cb = sizeof(monOutbuf) - sizeof(monOutbuf.cb);
	bMonRunning = FALSE;

	/* Register the buffers to be used for monitoring for current session */
	if (DosMonReg(_EVT_hKbdMon, &monInbuf, (ULONG*)&monOutbuf,MONITOR_END, -1)) {
		DosPostEventSem(hevStart);  /* unblock the main thread */
		return;
		}

	/* Unblock the main thread and tell it we're OK*/
	bMonRunning = TRUE;
	DosPostEventSem(hevStart);
	while (bMonRunning) {  /* Start an endless loop */
		/* Read data from keyboard driver */
		rc = DosMonRead((PBYTE)&monInbuf, IO_WAIT, (PBYTE)&kp, (PUSHORT)&count);
		if (rc) {
#ifdef CHECKED
			if (bMonRunning)
				printf("Error in DosMonRead, rc = %d\n", rc);
#endif
			bMonRunning = FALSE;
			return;
			}

		/* Pass FLUSH packets immediately */
		if (kp.MonFlagWord & 4) {
#ifdef CHECKED
			printf("Flush packet!\n");
#endif
			DosMonWrite((PBYTE)&monOutbuf, (PBYTE)&kp, count);
			continue;
			}

		//TODO: to be removed
		/* Skip extended scancodes & some others */
		if (((kp.MonFlagWord >> 8) == 0xE0) || ((kp.KbdDDFlagWord & 0x0F) == 0x0F)) {
			DosMonWrite((PBYTE)&monOutbuf, (PBYTE)&kp, count);
			continue;
			}

//  	printf("RawScan = %X, XlatedScan = %X, fbStatus = %X, KbdDDFlags = %X\n",
//      	kp.MonFlagWord >> 8, kp.XlatedScan, kp.ShiftState, kp.KbdDDFlagWord);

		/* Protect access to buffer with mutex semaphore */
		rc = DosRequestMutexSem(hmtxKeyBuf, 1000);
		if (rc) {
#ifdef CHECKED
			printf("Can't get access to mutex, rc = %d\n", rc);
#endif
			bMonRunning = FALSE;
			return;
			}

		/* Store packet in circular buffer, drop it if it's full */
		kpNew = kpHead + 1;
		if (kpNew == KEYBUFSIZE)
			kpNew = 0;
		if (kpNew != kpTail) {
			memcpy(&keyMonPkts[kpHead], &kp, sizeof(KEYPACKET));
			// TODO: fix this!
			/* Convert break to make code */
			keyMonPkts[kpHead].MonFlagWord &= 0x7FFF;
			kpHead = kpNew;
			}
		DosReleaseMutexSem(hmtxKeyBuf);

		/* Finally write the packet */
		rc = DosMonWrite((PBYTE)&monOutbuf, (PBYTE)&kp, count);
		if (rc) {
#ifdef CHECKED
			if (bMonRunning)
				printf("Error in DosMonWrite, rc = %d\n", rc);
#endif
			bMonRunning = FALSE;
			return;
			}
		}
	(void)params;
}

/****************************************************************************
REMARKS:
Safely abort the event module upon catching a fatal error.
****************************************************************************/
void _EVT_abort(
	int signal)
{
	EVT_exit();
	PM_fatalError("Unhandled exception!");
}

/****************************************************************************
PARAMETERS:
mouseMove	- Callback function to call wheneve the mouse needs to be moved

REMARKS:
Initiliase the event handling module. Here we install our mouse handling ISR
to be called whenever any button's are pressed or released. We also build
the free list of events in the event queue.

We use handler number 2 of the mouse libraries interrupt handlers for our
event handling routines.
****************************************************************************/
void EVTAPI EVT_init(
	_EVT_mouseMoveHandler mouseMove)
{
	ushort	stat;

	/* Initialise the event queue */
	PM_init();
	_mouseMove = mouseMove;
	initEventQueue();
	oldMouseState = 0;
	oldKeyMessage = 0;
	memset(keyUpMsg,0,sizeof(keyUpMsg));

	/* Open the mouse driver, and set it up to report events in mickeys */
	MouOpen(NULL,&_EVT_hMouse);
	stat = 0x7F;
	MouSetEventMask(&stat,_EVT_hMouse);
	stat = (MOU_NODRAW | MOU_MICKEYS) << 8;
	MouSetDevStatus(&stat,_EVT_hMouse);

	/* Open the keyboard monitor  */
	if (DosMonOpen("KBD$", &_EVT_hKbdMon))
		PM_fatalError("Unable to open keyboard monitor!");

	/* Create event semaphore, the monitor will post it when it's initalized */
	if (DosCreateEventSem(NULL, &hevStart, 0, FALSE))
		PM_fatalError("Unable to create event semaphore!");

	/* Create mutex semaphore protecting the keypacket buffer */
	if (DosCreateMutexSem(NULL, &hmtxKeyBuf, 0, FALSE))
		PM_fatalError("Unable to create mutex semaphore!");

	/* Start keyboard monitor thread, use 32K stack */
	kbdMonTID = _beginthread(_kbdMonThread, NULL, 0x8000, NULL);

//extern int _beginthread( register void (*__start_address)(void *),
//	 void *__stack_bottom, unsigned __stack_size, void *__arglist );


	/* Now block until the monitor thread is up and running */
	/* Give the thread one second */
	DosWaitEventSem(hevStart, 1000);
	if (!bMonRunning) {  /* Check the thread is OK */
		DosMonClose(_EVT_hKbdMon);
		PM_fatalError("Keyboard monitor thread didn't initialize!");
		}

	/* Determine the OS/2 timer frequency */
	DosTmrQueryFreq(&frequency);

	/* Catch program termination signals so we can clean up properly */
	signal(SIGABRT, _EVT_abort);
	signal(SIGFPE, _EVT_abort);
	signal(SIGINT, _EVT_abort);
}

/****************************************************************************
REMARKS
Changes the range of coordinates returned by the mouse functions to the
specified range of values. This is used when changing between graphics
modes set the range of mouse coordinates for the new display mode.
****************************************************************************/
void EVTAPI EVT_setMouseRange(
	int xRes,
	int yRes)
{
	rangeX = xRes;
	rangeY = yRes;
}

/****************************************************************************
REMARKS
Modifes the mouse coordinates as necessary if scaling to OS coordinates,
and sets the OS mouse cursor position.
****************************************************************************/
#define	_EVT_setMousePos(x,y)

/****************************************************************************
REMARKS:
Initiailises the internal event handling modules. The EVT_suspend function
can be called to suspend event handling (such as when shelling out to DOS),
and this function can be used to resume it again later.
****************************************************************************/
void EVT_resume(void)
{
	// Do nothing for OS/2
}

/****************************************************************************
REMARKS
Suspends all of our event handling operations. This is also used to
de-install the event handling code.
****************************************************************************/
void EVT_suspend(void)
{
	// Do nothing for OS/2
}

/****************************************************************************
REMARKS
Exits the event module for program terminatation.
****************************************************************************/
void EVT_exit(void)
{
	APIRET   rc;

	/* Restore signal handlers */
	signal(SIGABRT, SIG_DFL);
	signal(SIGFPE, SIG_DFL);
	signal(SIGINT, SIG_DFL);

	/* Close the mouse driver */
	MouClose(_EVT_hMouse);

	/* Stop the keyboard monitor thread and close the monitor */
	bMonRunning = FALSE;
	rc = DosKillThread(kbdMonTID);
#ifdef CHECKED
	if (rc)
		printf("DosKillThread failed, rc = %d\n", rc);
#endif
	rc = DosMonClose(_EVT_hKbdMon);
#ifdef CHECKED
	if (rc) {
		printf("DosMonClose failed, rc = %d\n", rc);
		}
#endif
	DosCloseEventSem(hevStart);
	DosCloseMutexSem(hmtxKeyBuf);
	KbdFlushBuffer(0);
}
