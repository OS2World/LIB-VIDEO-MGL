// Prototypes of several functions should be defined and bitmaps exist...

// Demo showing how you can create a scrollingDC and scroll around
// inside the scrolling DC's screen image.
unit Scroll;

interface

implementation

uses
    MGLFx,
    MGLSamp,
    SysUtils;

const
    IMAGE_NAME          = 'data\scroll.bmp';
    MAX_BUBBLES         = 70;
    MAX_UNIQUE_BUBBLES  = 3;

type
    image = record
        x,y     : int;
        typ     : int;
        speed   : int;
    end;

var
    // Maximum coordinates for direct surface
    maxx,maxy       : int;
    // Scanline width for surface               
    // bytesPerLine    : long;
    // Pointer to the surface to draw to
    // surface         : PChar;
    mousex, mousey   : int;

procedure   Pan(scrolldc: PMGLDC);
begin
    // turn on the mouse
    MS_setCursor(MGL_DEF_CURSOR);
    MS_show;

    while not CheckEvent do
    begin
        MS_getPos(@mousex,@mousey);
        MGL_setDisplayStart(scrolldc,mousex,0,1);
    end;

    MS_hide;
    WaitEvent;
end;

procedure   DoubleBufferPan(scrolldc: PMGLDC);
var
    bubble      : array[0..MAX_BUBBLES-1] of image;
    Buf         : string;
    bubblebm    : array[0..MAX_UNIQUE_BUBBLES-1] of pbitmap_t;
    i           : int;
begin
    MGL_swapBuffers(scrolldc,true);

    if not MGL_loadBitmapIntoDC(scrolldc,IMAGE_NAME,0,0,TRUE) then
        MGLError;

    for i := 0 to MAX_UNIQUE_BUBBLES - 1 do
    begin
        // Load a bitmap and the palette from the image file.
        Buf := Format('bubble%d.bmp',[i+1]);
        bubblebm[i] := MGL_loadBitmap(PChar(buf),TRUE);
        if bubblebm[i] = nil then
            MGLError;
    end;

    for i := 0 to MAX_BUBBLES - 1 do
    begin
        bubble[i].x     := MGL_random(1260);
        bubble[i].y     := MGL_random(460);
        bubble[i].typ   := MGL_random(MAX_UNIQUE_BUBBLES);
        case bubble[i].typ of
            0: bubble[i].speed := 2;
            1: bubble[i].speed := 3;
            2: bubble[i].speed := 1;
        end;
    end;

    while not checkEvent do
    begin
        for i := 0 to MAX_BUBBLES - 1 do
        begin
            Dec(bubble[i].y,bubble[i].speed);
            MGL_putBitmap(scrolldc,bubble[i].x,bubble[i].y,bubblebm[bubble[i].typ],MGL_REPLACE_MODE);
            if bubble[i].y <= 0 then
                bubble[i].y := 400 + MGL_random(60);
            MS_getPos(@mousex,@mousey);
        end;

        MGL_setDisplayStart(scrolldc,mousex,0,-1);
        MGL_swapBuffers(scrolldc,true);
    end;
end;

procedure   DoScroll;
var
    scrolldc    : PMGLDC;
begin
    scrolldc := MGL_createScrollingDC(1280,480,1);
    if scrolldc = nil then
        MGLError;

    MGL_makeCurrentDC(scrolldc);

    // Load a bitmap into the scrolling device context. The palette gets
    // loaded into the scrolling device context as well.
    if not MGL_loadBitmapIntoDC(scrolldc,IMAGE_NAME,0,0,TRUE) then
        MGLError;

    // Now extract the information about the device surface to our
    // global variables and draw on it. Note that we must check that we
    // have direct access to the surface. If it is a bank switched
    // surface then we cannot directly write to it, and the surface
    // pointer will be NULL.
    //
    // Note that the MGL fully supports virtualised framebuffer surfaces,
    // so in most cases it would be very rare to get a bank switched
    // surface (in fact since we did not register the bank switched
    // drivers, it would be impossible in this sample program!).
    if MGL_surfaceAccessType(scrolldc) = MGL_NO_ACCESS then
        raise Exception.Create('This program requires a linear access surface');

    maxx := scrolldc^.mi.xRes;
    maxy := scrolldc^.mi.yRes;

    // If we are drawing to a hardware accelerated surface, we must call
    // MGL_beginDirectAccess() and MGL_endDirectAccess() before and after
    // doing any direct surface access
    if MGL_getHardwareFlags(scrolldc) <> MGL_HW_NONE then
        MGL_beginDirectAccess;

    if not MS_available then
        MGLError;

    // TODO: What is net function???
    //  _MS_sizeScreen(640-1,480-1);
    MS_moveTo(0,0);

    Pan(scrolldc);
    DoubleBufferPan(scrolldc);

    if MGL_getHardwareFlags(scrolldc) <> MGL_HW_NONE then
        MGL_endDirectAccess;

    MS_hide;
    MGL_exit;
end;

procedure   Demo(dc: PMGLDC);
begin
    DoScroll;
end;

initialization
    RegisterDemo('Scroll',
                 'Demo showing how you can create a scrollingDC and scroll around '+
                 'inside the scrolling DC''s screen image.',
                 Demo);
end.
