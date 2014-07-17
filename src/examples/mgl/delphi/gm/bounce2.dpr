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

program Bounce2;

uses
  MGLTypes,
  MGLFx,
  MGLGM,
  SysUtils,
  Windows;

// Note that we need to include the Game Framework dialog box resources
// in here, so that we can properly display the mode browsing dialog box.
{$R GMDLG.RES}

type
    TBall   = record
        x, y, r, dx, dy: integer;
        c: integer;
    end;

const
    MAX_BALLS = 100;
    START_BALLS = 5;

var
    Balls           : array[0..MAX_BALLS-1] of TBall;
    N               : Integer = 0;
    defFnt          : pfont_t = nil;
    StartTime       : Integer;
    Frames          : Integer;
    gm              : PGMDC;
    driverOpt       : GM_driverOptions;
    startWindowed   : bool = true;
    info            : GM_modeInfo;

procedure   ResetTimer;
begin
    Frames := 0;
    StartTime := GetCurrentTime;
end;

procedure   AddBall;
begin
    if N = MAX_BALLS then
        Exit;

    with Balls[N] do
    begin
        x   := Random(MGL_maxx);
        y   := Random(MGL_maxy);
        r   := Random(100);
        dx  := Random(5) - 2;
        dy  := Random(5) - 2;
        c   := Random(100);
    end;
    Inc(N);
    ResetTimer;
end;

procedure   RemoveBall;
begin
    if N > 0 then
        Dec(N);
    ResetTimer;
end;

procedure   MoveBall(i: Integer);
var
    newx, newy: integer;
begin
    with Balls[i] do
    begin
        newx := x + dx;
        newy := y + dy;
        if (newx + r > MGL_maxx) then
            dx := -Abs(dx)
        else if (newx - r < 0) then
            dx := Abs(dx) ;
        if (newy + r > MGL_maxy) then
            dy := -Abs(dy)
        else if (newy - r < 0) then
            dy := Abs(dy) ;
        Inc(x,dx);
        Inc(y,dy);
    end;
end;

procedure   DrawBall(i: Integer);
begin
    with Balls[i] do
    begin
        MGL_setColorCI(c);
        MGL_fillEllipseCoord(x,y,r,r);
    end;
end;

function    RandomX: int;
begin
    Result := Random(MGL_maxx);
end;

function    RandomY: int;
begin
    Result := Random(MGL_maxy);
end;

procedure   DrawProc; cdecl;
var
    i: Integer;
    Delta: Integer;
    FC: Integer;
begin
    MGL_clearDevice;
    for i := 0 to N - 1 do
        DrawBall(i);
    Delta := (GetCurrentTime - StartTime) div 1000;
    if Delta <> 0 then
        FC := Frames div Delta
    else
        FC := 0;
    MGL_drawStrXY(0,100,PChar(Format('Balls: %d, FrameRate: %d/sec, Time: %d',[N,FC,Delta])));
    GM_SwapBuffers(MGL_waitVRT);
    Inc(Frames);
end;

procedure   KeyDownProc(evt: pevent_t); cdecl;
begin
    if EVT_asciiCode(evt^.message) = $1B then
        GM_exit
    else if EVT_asciiCode(evt^.message) = Ord('+') then
        AddBall
    else if EVT_asciiCode(evt^.message) = Ord('-') then
        RemoveBall
    else if EVT_asciiCode(evt^.message) = Ord('*') then
        if N < MAX_BALLS then
            while N < MAX_BALLS do
                AddBall
        else
            while N > 0 do
                RemoveBall;
end;

procedure   KeyRepeatProc(evt: pevent_t); cdecl;
begin
    KeyDownProc(evt);
end;

procedure   KeyUpProc(evt: pevent_t); cdecl;
begin
end;

procedure   MouseDownProc(evt: pevent_t); cdecl;
begin
end;

procedure   MouseUpProc(evt: pevent_t); cdecl;
begin
end;

procedure   MouseMoveProc(evt: pevent_t); cdecl;
begin
end;

procedure   EventProc(evt: pevent_t); cdecl;
begin
end;

procedure   GameLogicProc; cdecl;
var
    i: Integer;
begin
    for i := 0 to N - 1 do
        MoveBall(i);
end;

procedure   ExitProc; cdecl;
begin
end;

procedure   InitPalette;
var
    Pal: array[0..99] of palette_t;
    i: Integer;
begin
    for i := Low(Pal) to High(Pal) do
        with Pal[i] do
        begin
            blue := Random(255);
            red  := Random(255);
            green:= Random(255);
            alpha:= 0;
        end;
    GM_setPalette(@Pal[0],100,0);
    GM_realizePalette(100,0,true);

    if defFnt <> nil then
    begin
        MGL_unloadFont(defFnt);
        defFnt := nil;
    end;
    defFnt := MGL_loadFont('pc8x8.fnt');
    MGL_useFont(defFnt);
    Randomize;
    StartTime := GetCurrentTime;
    Frames    := 0;
end;

procedure SwitchModes(var mode: GM_modeInfo; windowed: bool); cdecl;
begin
    InitPalette;
end;

procedure   AppActivateProc(Active: bool); cdecl;
begin
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
        else if CompareText(ParamStr(1),'-nolinear') = 0 then
            driverOpt.useLinear := false
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
    GM_setDrawFunc(DrawProc);
    GM_setGameLogicFunc(GameLogicProc);
    GM_setDrawFunc(DrawProc);
    GM_setKeyDownFunc(KeyDownProc);
    GM_setKeyRepeatFunc(KeyRepeatProc);
    GM_setKeyUpFunc(KeyUpProc);
    GM_setMouseDownFunc(MouseDownProc);
    GM_setMouseUpFunc(MouseUpProc);
    GM_setMouseMoveFunc(MouseMoveProc);
    GM_setEventFunc(EventProc);
    GM_setModeSwitchFunc(SwitchModes);
    GM_setExitFunc(ExitProc);
    GM_setAppActivate(AppActivateProc);

    {Loop around choosing a mode and runing demo}
    while GM_chooseMode(info,startWindowed) do
    begin
        if not GM_setMode(info,startWindowed,3,true) then
           MGLError;
        InitPalette;
        N := 0;
        while N < START_BALLS do
            AddBall;
        GM_mainLoop;
    end;
end.

