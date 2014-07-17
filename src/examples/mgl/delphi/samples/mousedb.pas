// Double buffered mouse cursor sample program.
unit MouseDB;

interface

implementation

uses
    MGLFx,
    MGLSamp,
    MGLTypes,
    SysUtils;

{-- Variables ----------------------------------------------------------------}

var
    maxx,maxy,
    stepx,stepy,
    secAngle,minAngle   : int;
    extent,dirtyRect    : rect_t;

{-- InitAnimation ------------------------------------------------------------}

procedure   InitAnimation;
begin
    maxx            := MGL_maxx;
    maxy            := MGL_maxy;
    extent.top      := 0;
    extent.left     := 0;
    extent.right    := maxx div 5;
    extent.bottom   := Trunc((LongInt(extent.right) * 1000) / MGL_getAspectRatio);
    dirtyRect       := extent;
    stepx           := 1;
    stepy           := 1;
    minAngle        := 90;
    secAngle        := 90;
end;

{-- ClearDirtyRect -----------------------------------------------------------}

procedure   ClearDirtyRect;
begin
    MGL_setColorCI(MGL_BLACK);
    MGL_fillRect(dirtyRect);
end;

{-- DrawClock ----------------------------------------------------------------}

procedure   DrawClock;
begin
    MGL_setColorCI(MGL_RED);
    MGL_fillEllipse(extent);
    MGL_setColorCI(MGL_WHITE);
    MGL_fillEllipseArc(extent,secAngle-5,secAngle);
    MGL_fillEllipseArc(extent,minAngle-5,minAngle);
end;

{-- fixAngle -----------------------------------------------------------------}

function    fixAngle(angle: int): int;
begin
    while angle < 0 do Inc(angle,360);
    while angle >= 360 do Dec(angle,360);
    Result := Angle;
end;

{-- MoveClock ----------------------------------------------------------------}

procedure   MoveClock;
begin
    // Bounce the clock off the walls
    dirtyRect := extent;
    MGL_insetRect(dirtyRect,-Abs(stepx),-Abs(stepy));
    if extent.left + stepx < 0 then
        stepx := -stepx;
    if extent.right + stepx > maxx then
        stepx := -stepx;
    if extent.top + stepy < 0 then
        stepy := -stepy;
    if extent.bottom + stepy > maxy then
        stepy := -stepy;
    MGL_offsetRect(extent,stepx,stepy);

    // Update the hand movement
    secAngle := fixAngle(secAngle - 5);
    if secAngle = 90 then
        minAngle := fixAngle(minAngle - 5);
end;

{-- Demo ---------------------------------------------------------------------}

procedure   Demo(dc: PMGLDC);
begin
    // The first way to use a mouse cursor in double buffered modes is
    // useful if you are re-drawing your entire frame for every page
    // flip. In this case you can simply draw your frame, then call
    // MS_drawCursor() to plot the cursor at the current location on the
    // active page before you do a flip. Note that the mouse cursor must
    // be hidden to use this method (it is always hidden by default). As
    // long as you keep the frame rate up, the mouse cursor will appear
    // to move smoothly.
    if not MGL_doubleBuffer(dc) then
        raise Exception.Create('Double buffering not available!');

    MainWindow(dc,'Double buffered mouse cursor');
    StatusLine('Method 1: Re-render entire scene per frame');

    InitAnimation;
    repeat
        // Clear the entire display device before drawing the next frame
        MainWindow(dc,'Double buffered mouse cursor');
        StatusLine('Method 1: Re-render entire scene per frame');

        // Draw the clock at the current location and move it
        DrawClock;
        MoveClock;

        // Draw the mouse cursor on top of the current frame and flip
        MS_drawCursor;
        MGL_swapBuffers(dc,MGL_waitVRT);
    until CheckEvent;

    WaitEvent;

    // The second way to use a mouse cursor in double buffered modes is
    // useful if you may not be re-drawing the entire frame or require
    // the mouse to move in between page flips (ie: if you flip on an
    // intemittant basis such as in an RPG). In this case just turn on the
    // mouse cursor and the MGL will automatically take care of moving it
    // and ensuring it turns up at the right places after a flip. Note
    // that the mouse cursor will *always* be visible on the visible page
    // so you dont need to show and hide the cursor while drawing to the
    // active page in these modes.
    MGL_doubleBuffer(dc);
    MainWindow(dc,'Double buffered mouse cursor');
    StatusLine('Method 2: Re-render partial scene per frame (RPG''s)');
    MGL_swapBuffers(dc,MGL_waitVRT);
    MainWindow(dc,'Double buffered mouse cursor');
    StatusLine('Method 2: Re-render partial scene per frame (RPG''s)');
    MS_show;

    InitAnimation;
    repeat
        // Draw the clock at the current location and move it
        ClearDirtyRect;
        DrawClock;
        MoveClock;

        // Flip buffers
        MGL_swapBuffers(dc,MGL_waitVRT);
    until CheckEvent;
    WaitEvent;
end;

initialization
    RegisterDemo('Double-buffered mouse',
                 'Double buffered mouse cursor sample program.',
                 Demo);
end.
