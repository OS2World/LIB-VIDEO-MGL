// Simple demo showing how to turn on 8bpp RGB dithering and
// to draw a bunch of primitives (a few lines, circles,
// rectangles and polygons) with dithering turned on.
unit Dither;

interface

implementation

uses
    MGLSamp,
    MGLTypes,
    MGLFx;

{-- Global variables ---------------------------------------------------------}
var
    maxx, maxy: int;

{-- DitherLines --------------------------------------------------------------}
    
// Description:  Randomly places lines onto the screen.
function    DitherLines(dc: PMGLDC): Boolean;
var
    p1,p2: point_t;
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
        MGL_setColorMapMode(MGL_DITHER_RGB_MODE);
        MGL_line(p1,p2);
    end;

    defaultAttributes(dc);
    Result := Pause;
end;

{-- DitherPolygons -----------------------------------------------------------}

// Display a random pattern of convex triangular polygons
// in replace mode at full speed.
function    DitherPolygons(dc: PMGLDC): Boolean;
var
    poly: array[0..2] of fxpoint_t;
begin
    MainWindow(dc,'MGL_fillPolygonFast Demonstration');
    StatusLine('Press any key to continue, ESC to Abort');

    while not checkEvent do
    begin
        // Define a random polygon
        poly[0].x := MGL_randoml(MGL_TOFIX(maxx));
        poly[0].y := MGL_randoml(MGL_TOFIX(maxy));
        poly[1].x := MGL_randoml(MGL_TOFIX(maxx));
        poly[1].y := MGL_randoml(MGL_TOFIX(maxy));
        poly[2].x := MGL_randoml(MGL_TOFIX(maxx));
        poly[2].y := MGL_randoml(MGL_TOFIX(maxy));

        MGL_setColor(randomColor);
        MGL_setColorMapMode(MGL_DITHER_RGB_MODE);
        MGL_fillPolygonFX(3,@poly[0],sizeof(fxpoint_t),0,0);
    end;
    defaultAttributes(dc);
    Result := Pause;
end;

{-- DitherRectangles ---------------------------------------------------------}

// Display a number of random rectangles
function    DitherRectangles(dc: PMGLDC): Boolean;
var
    r: rect_t;
begin
    // Display title message at the top of the window.
    MainWindow(dc,'Rectangle Demonstration');
    StatusLine('Press any key to continue, ESC to Abort');

    while not checkEvent do
    begin
        r.left  := MGL_random(maxx);
        r.right := MGL_random(maxx);
        r.top   := MGL_random(maxy);
        r.bottom:= MGL_random(maxy);

        // Fix the rectangle so it is not empty
        if (r.right < r.left) then
            Swap(r.left,r.right);
        if (r.bottom < r.top) then
            Swap(r.top,r.bottom);

        MGL_setColor(randomColor);
        MGL_setBackColor(randomColor);
        MGL_setColorMapMode(MGL_DITHER_RGB_MODE);
        MGL_fillRect(r);
    end;

    defaultAttributes(dc);
    Result := Pause;
end;

{-- DitherEllipses -----------------------------------------------------------}

// Description:  Randomly place ellipses on the screen.
function    DitherEllipses(dc: PMGLDC): Boolean;
var
    r: rect_t;
begin
    MainWindow(dc,'Random filled ellipses Demonstration');
    StatusLine('Press any key to continue, ESC to Abort');

    while not checkEvent do
    begin
        r.left  := MGL_random(maxx-100);
        r.top   := MGL_random(maxy-100);
        r.right := r.left + MGL_random(100);
        r.bottom:= r.top + MGL_random(100);

        MGL_setColor(randomColor);
        MGL_setBackColor(randomColor);
        MGL_setColorMapMode(MGL_DITHER_RGB_MODE);
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

    // Display title message at the top of the window.
    MainWindow(dc,'Dither Demo');

    // MGL_beginDrawing must be called before calling any of the
    // special high performance drawing routines, such as the
    // MGL_line and associated routines.
    if(not ditherLines(dc)) then Exit;
    if(not ditherPolygons(dc)) then Exit;
    if(not ditherRectangles(dc)) then Exit;
    if(not ditherEllipses(dc)) then Exit;
end;

initialization
    RegisterDemo('Dither demo',
                 'Simple demo showing how to turn on 8bpp RGB dithering and '+
                 'to draw a bunch of primitives (a few lines, circles '+
                 'rectangles and polygons) with dithering turned on.',
                 Demo);
end.
