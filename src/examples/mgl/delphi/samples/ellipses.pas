// Simple demo showing how to draw a bunch of simple ellipses
// on the screen with the MGL.
unit Ellipses;

interface

implementation

uses
    Patterns,
    MGLSamp,
    MGLTypes,
    MGLFx;

{-- Constants ----------------------------------------------------------------}
const
    MIN_RADIUS = 5;

{-- Global variables ---------------------------------------------------------}
var
    maxx,maxy: int;

{-- EllipsesDemo -------------------------------------------------------------}

// This demo fills the entire screen with an ellipses. The
// MGL_fillEllipse function uses a bounding rectangle defining
// the ellipses. The size, color and pen style are randomly
// changing as well.
function    EllipsesDemo(dc: PMGLDC): Boolean;
var
    val : int;
    r   : rect_t;
begin
    // Display title message at the top of the window.
    MainWindow(dc,'Filled ellipses Demonstration');
    StatusLine('Press any key to start random test, ESC to Abort');

    // The pen style MGL_BITMAP_TRANSPARENT uses the foreground
    // color to fill in all pixels in the bitmap. Where the pixels in the
    // bitmap pattern are a 0, the original background color is retained.
    // In the MGL_BITMAP_OPAQUE mode, the background color is used to
    // fill in the pixels in the bitmap that are set to a 0.

    val := 0;
    while not checkEvent do
    begin
        MGL_setColor(randomColor);
        MGL_setBackColor(randomColor);

        if (val = 0) then
        begin
            MGL_setPenStyle(MGL_BITMAP_TRANSPARENT);
            MGL_setPenBitmapPattern(@bitpat[MGL_random(NUMPATS)+1]);
        end
        else if (val = 1) then
        begin
            MGL_setPenStyle(MGL_BITMAP_OPAQUE);
            MGL_setPenBitmapPattern(@bitpat[MGL_random(NUMPATS)+1]);
        end
        else
        begin
            MGL_setPenStyle(MGL_BITMAP_SOLID);
            val := 0;
        end;

        Inc(val);
        r.top       := maxy div 2;
        r.left      := maxx div 2;
        r.right     := maxx div 2;
        r.bottom    := maxy div 2;

        while((r.top > 0) or (r.right<maxx)) and not checkEvent do
        begin
            if(r.top>0) then
                Dec(r.top);
            if(r.left>0) then
                Dec(r.left);
            if(r.right<maxx) then
                Inc(r.right);
            if(r.bottom<maxy) then
                Inc(r.bottom);
            MGL_fillEllipse(r);
        end;
    end;
    defaultAttributes(dc);
    Result := Pause;
end;

{-- RandomEllipsesDemo -------------------------------------------------------}

// Demo randomly places ellipses on the screen.
function    RandomEllipsesDemo(dc: PMGLDC): Boolean;
var
    val : int;
    r   : rect_t;
begin
    MainWindow(dc,'Random filled ellipses Demonstration');
    StatusLine('Press any key to continue, ESC to Abort');

    while not checkEvent do
    begin
        r.left      := MGL_random(maxx-100);
        r.top       := MGL_random(maxy-100);
        r.right     := r.left + MGL_random(100);
        r.bottom    := r.top + MGL_random(100);

        MGL_setColor(randomColor);
        MGL_setBackColor(randomColor);

        val := MGL_random(5);
        if val = 0 then
        begin
            MGL_setPenStyle(MGL_BITMAP_TRANSPARENT);
            MGL_setPenBitmapPattern(@bitpat[MGL_random(NUMPATS)+1]);
        end
        else if val = 1 then
        begin
            MGL_setPenStyle(MGL_BITMAP_OPAQUE);
            MGL_setPenBitmapPattern(@bitpat[MGL_random(NUMPATS)+1]);
        end
        else
            MGL_setPenStyle(MGL_BITMAP_SOLID);

        MGL_fillEllipse(r);
    end;

    defaultAttributes(dc);
    Result := Pause;
end;

{-- Demo proc ----------------------------------------------------------------}

procedure   Demo(dc: PMGLDC);
begin
    // Get the screen size and store them.
    maxx    := MGL_maxx;
    maxy    := MGL_maxy;

    if not ellipsesDemo(dc) then
        Exit;
    if not randomEllipsesDemo(dc) then
        Exit;
end;

initialization
    RegisterDemo('Ellipses',
                 'Simple demo showing how to draw a bunch of simple ellipses '+
                 'on the screen with the MGL.',
                 Demo);
end.
