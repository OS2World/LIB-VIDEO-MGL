(****************************************************************************
*
*                       MegaGraph Graphics Library
*
*               Copyright (C) 1991-1997 SciTech Software, Inc.
*                            All rights reserved.
*
*  ======================================================================
*       This library is free software; you can use it and/or
*       modify it under the terms of the SciTech MGL Software License.
*
*       This library is distributed in the hope that it will be useful,
*       but WITHOUT ANY WARRANTY; without even the implied warranty of
*       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*       SciTech MGL Software License for more details.
*  ======================================================================
*
* Filename:     $Workfile:   bounce.c  $
* Version:      $Revision:   1.25  $
*
* Language:     ANSI C
* Environment:  MSDOS/Win32 full screen and windowed
*
* Description:  Simple game framework demonstration program.
*
* $Date:   01 Oct 1997 05:50:54  $ $Author:   KendallB  $
*
****************************************************************************)

program Bounce;

uses
    Windows,
    SysUtils,
    MGLFx,
    MGLTypes,
    MGLGm;

// Note that we need to include the Game Framework dialog box resources
// in here, so that we can properly display the mode browsing dialog box.
{$R GMDLG.RES}

const
    CSIZE   = 10;   // Circle Size

var
    pixelx,
    pixely          : int;
    xres,
    yres            : int;
    incx,
    incy            : int;
    crunchx,
    crunchy         : int;

    prevDirtyRect   : rect_t;
    prevDirty       : pregion_t;

    gm              : PGMDC;

    driverOpt       : GM_driverOptions;
    startWindowed   : bool = true;
    info            : GM_modeInfo;

const
    CL_ELLIPSEFILL  = 13;
    CL_ELLIPSE      = 12;

// Initialisation function that sets up the internal's of the bounce
// demo after a mode set.
procedure   MyInit;
var
    pal : array[0..255] of palette_t;
begin
    FillChar(pal,SizeOf(pal),0);
    pal[CL_ELLIPSEFILL].blue:= 255;
    pal[CL_ELLIPSE].red     := 255;
    pal[CL_ELLIPSE].green   := 255;
    pal[CL_ELLIPSE].blue    := 255;

    xres    := MGL_sizex(gm^.dc)+1-CSIZE;
    yres    := MGL_sizey(gm^.dc)+1-CSIZE;
    pixelx  := xres div 2;
    pixely  := yres div 2;
    incx    := 1;
    incy    := -1;
    crunchx := CSIZE;
    crunchy := CSIZE;
    GM_setPalette(@pal,256,0);
    GM_realizePalette(256,0,true);
    with prevDirtyRect do
    begin
        left    := 0;
    top     := 0;
    right   := MGL_sizex(gm^.dc)+1;
    bottom  := MGL_sizey(gm^.dc)+1;
    end;
    prevDirty := MGL_rgnSolidRect(prevDirtyRect);
end;

// Main draw callback handler that draws the current frame to the device
// context and then swaps the display buffers. When the game is
// minimized on the task bar in fullscreen modes this function will not
// be called to avoid drawing to memory that we do not own.
procedure   DrawFrame; cdecl;
var
    dirtyRect           : rect_t;
    cntDirty,fullDirty  : pregion_t;
begin
    // Draw the ball at the current location
    MGL_clearDevice;
    MGL_setColorCI(13);
    MGL_fillEllipseCoord(pixelx,pixely,CSIZE+crunchx,CSIZE+crunchy);
    MGL_setColorCI(12);
    MGL_ellipseCoord(pixelx,pixely,CSIZE+crunchx,CSIZE+crunchy);
    with dirtyRect do
    begin
        left    := pixelx - (CSIZE+crunchx);
        right   := pixelx + CSIZE+crunchx;
        top     := pixely - (CSIZE+crunchy);
        bottom  := pixely + CSIZE+crunchy;
    end;

    // Swap display buffers with dirty rectangles
    cntDirty := MGL_rgnSolidRect(dirtyRect);
    MGL_unionRegionRect(cntDirty,@prevDirtyRect);
    if gm^.dispdc^.mi.maxPage > 0 then
    begin
        (* Note that if we have more than 1 display page and we are doing
         * double buffering, we have to union in the dirty region from the
         * previous buffer as the dirty regions from that frame are also
         * dirty in this buffer (as they two are distinct memory buffers in
         * video memory). This works for double buffering, and if you wish
         * to use more than 2 buffers you will need to use a chain of dirty
         * regions to find all the dirty portions to update for the current
         * buffer (ie: for triple buffering the region of the last 2 frames
         * instead of the last 1).
         *)
        fullDirty := MGL_copyRegion(cntDirty);
        MGL_unionRegion(fullDirty,prevDirty);
        MGL_optimizeRegion(fullDirty);
        GM_swapDirtyBuffers(fullDirty,MGL_waitVRT);
        MGL_freeRegion(fullDirty);
        MGL_freeRegion(prevDirty);
        prevDirty := cntDirty;
        prevDirtyRect := dirtyRect;
    end
    else begin
         MGL_optimizeRegion(cntDirty);
     GM_swapDirtyBuffers(cntDirty,MGL_waitVRT);
     MGL_freeRegion(cntDirty);
     prevDirtyRect := dirtyRect;
    end;
end;

// Main game logic callback handler that moves the bouncing ball around
// on the screen. This function is called once per main loop by the
// Game Framework, and you should do all non-drawing related game logic
// type stuff in here. This is where you would do all your sound processing,
// network processing etc before drawing the next frame.
procedure   GameLogic; cdecl;
begin
    // Move the ball to new location
    if pixelx >= xres-CSIZE then
        crunchx := xres-pixelx-1;
    if pixely >= yres-CSIZE then
        crunchy := yres-pixely-1;
    if pixelx <= CSIZE*2 then
        crunchx := pixelx-CSIZE-1;
    if pixely <= CSIZE*2 then
        crunchy := pixely-CSIZE-1;
    Inc(pixelx,incx);
    Inc(pixely,incy);
    if (pixelx >= xres) or (pixelx <= CSIZE) then
        incx := -incx;
    if (pixely >= yres) or (pixely <= CSIZE) then
        incy := -incy;
end;

// Main key down event handler to allow you to process key down events. The
// Game Framework allows you to register key repeat and key up event handlers
// as well (you can also use the same handler if you wish).
procedure   KeyDown(evt: pevent_t); cdecl;
begin
    case EVT_asciiCode(evt^.message) of
        $1B     : GM_exit;
        Ord('+'): begin
                    incx := incx * 2;
                    incy := incy * 2;
                  end;
        Ord('-'): if abs(incx) > 1 then
                  begin
                    incx := incx div 2;
                    incy := incy div 2;
                  end;
    end;
end;

// Main mouse down event handler to allow you to process mouse down events. The
// Game Framework allows you to register mouse up and mouse movement event
// handlers as well (you can also use the same handler if you wish).
procedure   MouseDown(evt: pevent_t); cdecl;
begin
    // Simply re-start the animation when we get a mouse down
    MyInit;
end;

// Main callback to switch between fullscreen and windowed graphics modes. This
// function is called by the game framework when the user requests to switch to
// fullscreen modes either by typing 'Alt-Enter' or by clicking on the maximum
// button when in windowed modes.
procedure SwitchModes(var mode: GM_modeInfo; windowed: bool); cdecl;
begin
     MyInit();
end;

// This function gets called when our application is activated and
// de-activated. If you have other components in your game that need
// to be disabled while your are not active (such as sound or CD-ROM
// Audio) you should do it here.
procedure   Activate(acitve: bool); cdecl;
begin
end;

// This function is where you would put your own code to handle
// switching to and from fullscreen modes, such as pausing
// CD-Audio and other stuff. The main game loop will continue
// to run as we return MGL_NO_SUSPEND_APP (but no drawFrame will
// not be called, only gameLogic), and you can optionally return
// MGL_NO_DEACTIVATE to forcibly ignore the switch.
function    suspendApp(dc: PMGLDC; flags: int): int; cdecl;
begin
    if flags = MGL_DEACTIVATE then
    begin
        // We are switching back to GDI mode, so put code in here to disable
        // stuff when switched away from fullscreen mode. Note that this
        // function may get once to determine if the switch should occur,
        // and again when the switch actually happens.
    end
    else if flags = MGL_REACTIVATE then
    begin
        // We are now back in fullscreen mode, so put code in here to renable
        // stuff for fullscreen modes.
    end;
    Result := MGL_NO_SUSPEND_APP;
end;

procedure   MGLError;
begin
    raise Exception.CreateFmt('MGL error %s',[MGL_errorMsg(MGL_result)]);
end;

begin
    {Fill in default driver options}
    driverOpt.useWinDirect := true;
    driverOpt.useDirectDraw := true;
    driverOpt.useVGA := true;
    driverOpt.useVGAX := true;
    driverOpt.useVBE := true;
    driverOpt.useLinear := true;
    driverOpt.useVBEAF := true;
    driverOpt.useFullscreenDIB := true;
    driverOpt.useHWOpenGL := false;
    driverOpt.modeFlags := GM_MODE_ALLBPP;

    {Parse command line}
    if ParamCount > 0 then
    begin
        if CompareText(ParamStr(1),'-nowindirect') = 0 then
            driverOpt.useWinDirect := false
        else if CompareText(ParamStr(1),'-novbe') = 0 then
            driverOpt.useVBE := false
        else if CompareText(ParamStr(1),'-novbeaf') = 0 then
            driverOpt.useVBEAF := false
        else if CompareText(ParamStr(1),'-nodirectdraw') = 0 then
            driverOpt.useDirectDraw := false
    end;

    {Initialize Game Framework}
    GM_setDriverOptions(driverOpt);
    gm := GM_init('Bounce');
    if gm = nil then
        MGLError;
    GM_setDrawFunc(DrawFrame);
    GM_setGameLogicFunc(GameLogic);
    GM_setKeyDownFunc(KeyDown);
    GM_setMouseDownFunc(MouseDown);
    GM_setModeSwitchFunc(SwitchModes);
    GM_setAppActivate(Activate);
    GM_setSuspendAppCallback(suspendApp);

    while GM_chooseMode(info,startWindowed) do
    begin
        if not GM_setMode(info,startWindowed,2,true) then
           MGLError;
        myInit;
        GM_mainLoop;
    end;
end.

