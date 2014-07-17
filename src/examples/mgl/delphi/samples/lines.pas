// Simple demo showing how to draw a bunch of lines on
// the screen with the MGL.
unit Lines;

interface

implementation

uses
    Patterns,
    MGLSamp,
    MGLTypes,
    MGLFx;

{-- Constants ----------------------------------------------------------------}
const
    MAX_LINES   = 35;

{-- Types --------------------------------------------------------------------}
type
    pvector = ^vector;
    vector  = record
        p1, p2  : point_t;
        d1, d2  : point_t;
        color   : int;
    end;
    vectors = array[0..0] of vector;
    pvectors= ^vectors;

{-- Variables ----------------------------------------------------------------}
var
    maxx,maxy: int;

{-- LineTest -----------------------------------------------------------------}

// Draw solid lines and how andchange thier color.

function    LineTest(dc: PMGLDC): Boolean;
var
    i,lines : int;
    v       : pvectors;
begin
    lines   := Trunc((MAX_LINES/maxy)*maxy);

    v       := pvectors(MGL_malloc(sizeof(vector)*lines));
    try
        for i := 0 to lines - 1 do
            with v^[i] do
            begin
                p1.x    := Trunc(i*(maxx/MAX_LINES));
                p1.y    := Trunc(i*(maxy/MAX_LINES));
                p2.x    := Trunc(maxx - i*(maxx/MAX_LINES));
                p2.y    := Trunc(i*(maxy/MAX_LINES));
                d1.x    := 1;
                d1.y    := -1;
                d2.x    := -1;
                d2.y    := 1;
                color   := randomColor;
            end;

        // Display title message at the top of the window.
        MainWindow(dc,'Line Demonstration');
        StatusLine('Press any key to continue, ESC to Abort');

        MGL_setPenStyle(MGL_BITMAP_SOLID);
        while not checkEvent do
            for i := 0 to lines - 1 do
                with v[i] do
                begin
                    // change the position of both vertices
                    Inc(p1.x,d1.x);
                    Inc(p1.y,d1.y);
                    Inc(p2.x,d2.x);
                    Inc(p2.y,d2.y);

                    // change the direction of the first vertice
                    if p1.x>maxx then
                        d1.x := -1;
                    if p1.x<=0 then
                        d1.x := 1;
                    if p1.y>maxy then
                        d1.y := -1;
                    if p1.y<=0 then
                        d1.y := 1;

                    // change the direction of the second vertice
                    if p2.x>maxx then
                        d2.x := -1;
                    if p2.x<=0 then
                        d2.x := 1;
                    if p2.y>maxy then
                        d2.y := -1;
                    if p2.y<=0 then
                        d2.y := 1;

                    MGL_setColor(color);
                    MGL_line(p1,p2);
                end;
    finally
        MGL_free(v);
    end;
    defaultAttributes(dc);
    Result := Pause;
end;

{-- RandomLineTest -----------------------------------------------------------}

// This demo randomly places lines onto the screen. The size,
// color and pen style are randomly changing as well. The pen
// style MGL_BITMAP_TRANSPARENT uses the foreground color to
// fill in all pixels in the bitmap. Where the pixels in the
// bitmap pattern are a 0, the original background color is
// retained. In the MGL_BITMAP_OPAQUE mode, the background
// color is used to fill in the pixels in the bitmap that are
// set to a 0.

function    RandomLineTest(dc: PMGLDC): Boolean;
var
    val     : int;
    p1,p2   : point_t;
begin
    // Display new title message at the top of the window.
    MainWindow(dc,'Random line Demonstration');
    StatusLine('Press any key to continue, ESC to Abort');

    while not checkEvent do
    begin
        p1.x := MGL_random(maxx);
        p1.y := MGL_random(maxy);
        p2.x := MGL_random(maxx);
        p2.y := MGL_random(maxy);

        MGL_setColor(randomColor);
        MGL_setBackColor(randomColor);
        MGL_setPenSize(MGL_random(5)+1,MGL_random(5)+1);

        val := MGL_random(5);
        if val = 0 then
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
            MGL_setPenStyle(MGL_BITMAP_SOLID);

        MGL_line(p1,p2);
    end;
    defaultAttributes(dc);
    Result := Pause;
end;

{-- Demo ---------------------------------------------------------------------}

procedure   Demo(dc: PMGLDC);
begin
    // Get the screen size and store them.
    maxx := MGL_maxx;
    maxy := MGL_maxy;
    if not lineTest(dc) then Exit;
    if not randomLineTest(dc) then Exit;
end;

initialization
    RegisterDemo('Lines',
                 'Simple demo showing how to draw a bunch of lines on '+
                 'the screen with the MGL.',
                 Demo);
end.

