// Page flipping sample program.
unit PageFlip;

interface

implementation

uses
    MGLFx,
    MGLTypes,
    MGLSamp;

{-- Variables ----------------------------------------------------------------}

var
    maxx,maxy,stepx,stepy,secAngle,minAngle : int;
    extent : rect_t;

{-- InitAnimation ------------------------------------------------------------}

procedure   InitAnimation;
begin
    maxx            := MGL_maxx;
    maxy            := MGL_maxy;
    extent.top      := 0;
    extent.left     := 0;
    extent.right    := maxx div 5;
    extent.bottom   := Trunc((LongInt(extent.right) * 1000)/MGL_getAspectRatio);
    stepx           := 1;
    stepy           := 1;
    secAngle        := 90;
    minAngle        := 90;
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

{-- FixAngle -----------------------------------------------------------------}

function    FixAngle(angle: int): int;
begin
    while angle < 0 do Inc(angle,360);
    while angle >= 360 do Dec(angle,360);
    Result := angle;
end;

{-- MoveClock ----------------------------------------------------------------}

procedure   MoveClock;
begin
    // Bounce the clock off the walls
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
var
    numpages,aPage,vPage    : int;
    sizex,sizey,bits        : int;
    fullView,oldView        : rect_t;
    pf                      : pixel_format_t;
    pal                     : array[0..255] of palette_t;
    memdc                   : PMGLDC;
begin
    // The first way to do simple page flipping is using two display
    // pages for 'double' buffering. The MGL has functions to set the
    // system into double buffered modes and flip pages for you, so
    // this method is very easy. We use MGL_doubleBuffer() to change
    // to double buffered mode and MGL_swapBuffers() to change display
    // pages.
    if not MGL_doubleBuffer(dc) then
        MGLError;
    MainWindow(dc,'Page Flip Demo');
    StatusLine('Method 1: Double buffering with two pages');
    MGL_swapBuffers(dc,MGL_waitVRT);

    // Draw to the hidden page
    MainWindow(dc,'Page Flip Demo');
    StatusLine('Method 1: Double buffering with two pages');

    InitAnimation;
    repeat
        // Clear the entire display device before drawing the next frame
        MGL_clearViewport;

        // Draw the clock at the current location and move it
        DrawClock;
        MoveClock;

        // Swap the display buffers
        MGL_swapBuffers(dc,MGL_waitVRT);
    until CheckEvent;
    WaitEvent;

    // Return to single buffered mode
    MGL_singleBuffer(dc);

    // The second way to use page flipping is with multiple display
    // pages (3+) to do multi-buffering. Because we are using multiple
    // display pages, we can turn off the wait for retrace flag when we
    // swap visual pages. In order for triple buffering to work properly,
    // we need to be doing a complete re-paint of the screen each frame
    // without doing a screen clear (otherwise you will see flicker).
    // Games like Quake and Duke3D repaint the entire frame from top to
    // bottom without any overdraw, which allows them to use triple
    // buffering techniques like this. We simulate this here by rendering
    // to a system buffer and blitting to the screen constantly to achieve
    // a similar effect for this sample program.
    //
    // Note that VBE/Core 3.0 and VBE/AF 2.0 include full support for
    // hardware triple buffering that can be used to eliminate this problem.
    // When these standards are ratified, the MGL will be updated to support
    // this and you will be able to do 'real' triple buffering without
    // any flicker on the screen.
    numpages := MGL_maxPage(dc)+1;
    if numpages < 3 then
        Exit;

    // Get the display device size, color depth and pixel format so that we
    // can create a compatible memory device context. Note that we also
    // need to copy the palette from the display DC to the memory DC.
    sizex := MGL_sizex(dc);
    sizey := MGL_sizey(dc);
    bits  := MGL_getBitsPerPixel(dc);
    MGL_getPixelFormat(dc,@pf);
    memdc := MGL_createMemoryDC(sizex+1,sizey+1,bits,@pf);
    if memdc = nil then
        MGLError;
    if bits = 8 then
    begin
        MGL_getPalette(dc,@pal[0],256,0);
        MGL_setPalette(memdc,@pal[0],256,0);
        MGL_realizePalette(memdc,256,0,1);
    end;
    MGL_makeCurrentDC(memdc);

    // Make the fullViewport for the display DC the full screen, as the
    // above code has changed it to a smaller value.
    fullView.left   := 0;
    fullView.top    := 0;
    fullView.right  := sizex+1;
    fullView.bottom := sizey+1;
    MGL_setViewportDC(dc,fullView);

    // Draw the main window display on the memory buffer
    MainWindow(memdc,'Page Flip Demo');
    StatusLine('Method 2: Multi-buffering with 3+ pages');
    InitAnimation();
    aPage := 1;
    MGL_setActivePage(dc,aPage);
    vPage := 0;
    MGL_setVisualPage(dc,vPage,MGL_dontWait);
    MGL_getViewport(@oldView);
    repeat
        // Clear the fullViewport before drawing the next frame
        MGL_clearViewport;

        // Draw the clock at the current location and move it
        DrawClock;
        MoveClock;

        // Copy the memory device to the display device
        MGL_setViewport(fullView);
        MGL_bitBlt(dc,memdc,fullView,0,0,MGL_REPLACE_MODE);
        MGL_setViewport(oldView);

        // Swap the hardware display buffers
        aPage := (aPage+1) mod numpages;
        vPage := (vPage+1) mod numpages;
        MGL_setActivePage(dc,aPage);
        MGL_setVisualPage(dc,vPage,MGL_waitVRT);
    until CheckEvent;
    WaitEvent;
end;

initialization
    RegisterDemo('PageFlip',
                 'Page flipping sample program.',
                 Demo);
end.
