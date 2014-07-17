// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
//
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//
// $Log:$
//
// DESCRIPTION:
//      DOOM graphics stuff for MGL
//      doesn't contain any system-specific code!
//-----------------------------------------------------------------------------

static const char
rcsid[] = "$Id: i_x.c,v 1.6 1997/02/03 22:45:10 b1 Exp $";

#include <stdlib.h>
#include <ctype.h>

#include <stdarg.h>
#include <sys/time.h>
#include <sys/types.h>
//#include <sys/socket.h>

//#include <netinet/in.h>
#include <signal.h>

#include "doomstat.h"
#include "i_system.h"
#include "v_video.h"
#include "m_argv.h"
#include "d_main.h"

#include "doomdef.h"

#include "mgraph.h"
#include "pmapi.h"

// Blocky mode,
// replace each 320x200 pixel with multiply*multiply pixels.
// According to Dave Taylor, it still is a bonehead thing
// to use ....
static int      multiply=1;

MGLDC           *dc;     // MGL scren device context
MGLDC           *memDC;  // MGL memory device context
int             driver = grDETECT;
ibool           useLinear = true;
ibool           useDirectDraw = true;
ibool           useWinDirect = true;

//
//  Translates the key currently in MGL event
//
int xlatekey(int code)
{
   int rc;

   switch (code) {
      case KB_F1:          rc = KEY_F1;            break;
      case KB_F2:          rc = KEY_F2;            break;
      case KB_F3:          rc = KEY_F3;            break;
      case KB_F4:          rc = KEY_F4;            break;
      case KB_F5:          rc = KEY_F5;            break;
      case KB_F6:          rc = KEY_F6;            break;
      case KB_F7:          rc = KEY_F7;            break;
      case KB_F8:          rc = KEY_F8;            break;
      case KB_F9:          rc = KEY_F9;            break;
      case KB_F10:         rc = KEY_F10;           break;
      case KB_F11:         rc = KEY_F11;           break;
      case KB_F12:         rc = KEY_F12;           break;
      case KB_left:        rc = KEY_LEFTARROW;     break;
      case KB_right:       rc = KEY_RIGHTARROW;    break;
      case KB_down:        rc = KEY_DOWNARROW;     break;
      case KB_up:          rc = KEY_UPARROW;       break;
      case KB_esc:         rc = KEY_ESCAPE;        break;
      case KB_enter:       rc = KEY_ENTER;         break;
      case KB_tab:         rc = KEY_TAB;           break;

      case KB_backspace:
      case KB_delete:      rc = KEY_BACKSPACE;     break;

/*    case KB_pause:       rc = KEY_PAUSE;         break;*/

      case KB_pageUp:      rc = KEY_EQUALS;        break;

      case KB_pageDown:    rc = KEY_MINUS;         break;

      case KB_leftShift:   rc = KEY_RSHIFT;        break;
      case KB_rightShift:  rc = KEY_RSHIFT;        break;

      case KB_leftCtrl:    rc = KEY_RCTRL;         break;
      case KB_rightCtrl:   rc = KEY_RCTRL;         break;

      case KB_leftAlt:     rc = KEY_RALT;          break;
      case KB_rightAlt:    rc = KEY_RALT;          break;

      case KB_space:       rc = ' ';                break;
   }
   return rc;
}

void I_ShutdownGraphics(void)
{
   // Close MGL
   MGL_exit();
}



//
// I_StartFrame
//
void I_StartFrame (void)
{
    // er?

}

static int      lastmousex = 0;
static int      lastmousey = 0;
boolean         mousemoved = false;

void I_GetEvent(void)
{

    event_t_ event; /* DOOM event */
    event_t  mglev; /* MGL event */


    if (EVT_getNext(&mglev,EVT_EVERYEVT)) {
        switch (mglev.what) {
            case EVT_KEYDOWN:
                event.type = ev_keydown;
                if (isalnum(EVT_asciiCode(mglev.message)))
                   event.data1 = EVT_asciiCode(mglev.message);
                else
                   event.data1 = xlatekey(EVT_scanCode(mglev.message));
                D_PostEvent(&event);
                // fprintf(stderr, "kd");
                break;

            case EVT_KEYUP:
                event.type = ev_keyup;
                if (isalnum(EVT_asciiCode(mglev.message)))
                   event.data1 = EVT_asciiCode(mglev.message);
                else
                   event.data1 = xlatekey(EVT_scanCode(mglev.message));
                D_PostEvent(&event);
                // fprintf(stderr, "ku");
                break;

            case EVT_MOUSEDOWN:
               event.type = ev_mouse;
               event.data1 =
                   (mglev.modifiers & EVT_LEFTBUT ? 1 : 0)
                   | (mglev.modifiers & EVT_RIGHTBUT ? 2 : 0)
                   | (mglev.modifiers & EVT_MIDDLEBUT ? 4 : 0);
               event.data2 = event.data3 = 0;
               D_PostEvent(&event);
               // fprintf(stderr, "b");
               break;

            case EVT_MOUSEUP:
               event.type = ev_mouse;
               event.data1 =
                   (mglev.modifiers & EVT_LEFTBUT ? 1 : 0)
                   | (mglev.modifiers & EVT_RIGHTBUT ? 2 : 0)
                   | (mglev.modifiers & EVT_MIDDLEBUT ? 4 : 0);
               event.data2 = event.data3 = 0;
               D_PostEvent(&event);
               // fprintf(stderr, "b");
               break;

            case EVT_MOUSEMOVE:
               event.type = ev_mouse;
               event.data1 =
                   (mglev.modifiers & EVT_LEFTBUT ? 1 : 0)
                   | (mglev.modifiers & EVT_RIGHTBUT ? 2 : 0)
                   | (mglev.modifiers & EVT_MIDDLEBUT ? 4 : 0);
               event.data2 = mglev.relative_x << 2;
               event.data3 = -mglev.relative_y << 2;

               D_PostEvent(&event);
               // fprintf(stderr, "m");
               mousemoved = false;
               break;
        }
    }
}

//
// I_StartTic
//
void I_StartTic (void)
{

    I_GetEvent();
    mousemoved = false;

}


//
// I_UpdateNoBlit
//
void I_UpdateNoBlit (void)
{
    // what is this?
}

//
// I_FinishUpdate
//
void I_FinishUpdate (void)
{

    static int  lasttic;
    int         tics;
    int         i;
    // UNUSED static unsigned char *bigscreen=0;

    // draws little dots on the bottom of the screen
    if (devparm)
    {

        i = I_GetTime();
        tics = i - lasttic;
        lasttic = i;
        if (tics > 20) tics = 20;

        for (i=0 ; i<tics*2 ; i+=2)
            screens[0][ (SCREENHEIGHT-1)*SCREENWIDTH + i] = 0xff;
        for ( ; i<20*2 ; i+=2)
            screens[0][ (SCREENHEIGHT-1)*SCREENWIDTH + i] = 0x0;

    }

    {
        // draw the image through MGL
        memcpy(memDC->surface, screens[0], SCREENWIDTH*SCREENHEIGHT);
        MGL_bitBltCoord(dc,memDC,0,0,SCREENWIDTH,SCREENHEIGHT,0,0,
                        MGL_REPLACE_MODE);
    }

}


//
// I_ReadScreen
//
void I_ReadScreen (byte* scr)
{
    memcpy (scr, screens[0], SCREENWIDTH*SCREENHEIGHT);
}


//
// Palette stuff.
//
palette_t    pal[256];

void UploadNewPalette(byte *palette)
{

    register int        i;
    register unsigned   c;
    static boolean      firstcall = true;

    // initialize the palette
    if (firstcall)
    {
        firstcall = false;
        for (i = 0; i < 256; i++)
        {
            pal[i].alpha = 0;
        }
    }
    // set the MGL palette entries
    for (i=0 ; i<256 ; i++)
    {
        c = gammatable[usegamma][*palette++];
        pal[i].red   = c;
        c = gammatable[usegamma][*palette++];
        pal[i].green = c;
        c = gammatable[usegamma][*palette++];
        pal[i].blue  = c;
    }

    // store the colors to the current colormap
    // Program palette using MGL
    if (MGL_getBitsPerPixel(dc) == 8) {
        MGL_setPalette(dc,pal,256,0);
        MGL_realizePalette(dc,256,0,true);
    }
}

//
// I_SetPalette
//
void I_SetPalette (byte* palette)
{
    UploadNewPalette(palette);
}

extern void initFatalError(void)
{
    char    buf[80];
    sprintf(buf,"Graphics error: %s\n",MGL_errorMsg(MGL_result()));
    MGL_fatalError(buf);
}

//
// I_InitGraphics
//
void I_InitGraphics(void)
{

    char*               displayname;
    char*               d;
    int                 n;
    int                 pnum;
    int                 x=0;
    int                 y=0;

    int                 oktodraw;
    static int          firsttime=1;
    pixel_format_t      pf;
    MGLDC               *bitDC;
    int                 width,height,bitsPerPixel;

    if (!firsttime)
        return;
    firsttime = 0;

    signal(SIGINT, (void (*)(int)) I_Quit);

    /* Create a MGL display device context */
    if ((dc = MGL_createDisplayDC(1)) == NULL)
        initFatalError();
        MGL_makeCurrentDC(dc);

    /* Register our suspend application callback */
//    MGL_setSuspendAppCallback(doSuspendApp);

    /* Turn off identity palette checking for maximum speed */
    MGL_checkIdentityPalette(false);

    // allocate MGL image buffer ?
    if ((memDC = MGL_createMemoryDC(SCREENWIDTH, SCREENHEIGHT, 8, &pf)) == NULL)
            MGL_fatalError("Can't allocate offscreen buffer!\n");

    MGL_makeCurrentDC(dc);
}
