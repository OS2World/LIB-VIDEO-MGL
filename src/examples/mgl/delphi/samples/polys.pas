// Simple demo showing how to draw complex and convex polygons
// on the screen.
unit Polys;

interface

implementation

uses
    Patterns,
    MGLFx,
    MGLTypes,
    MGLSamp;

{-- Constants ----------------------------------------------------------------}

const
    // Maximum # of pts in polygon
    MAXPTS = 6;

{-- FastPolyDemo -------------------------------------------------------------}

// Description:  Display a random pattern of convex triangular polygons
//               in replace mode at full speed.
function    FastPolyDemo(dc: PMGLDC): Boolean;
var
    maxx,maxy   : int;
    // Space to hold polygon data
    poly        : array[0..3] of fxpoint_t;
begin
    MainWindow(dc,'MGL_fillPolygonFast Demonstration');
    StatusLine('Press any key to continue, ESC to Abort');

    maxx := MGL_maxx;
    maxy := MGL_maxy;

    while not CheckEvent do
    begin
        // Define a random polygon
        poly[0].x := MGL_randoml(MGL_TOFIX(maxx));
        poly[0].y := MGL_randoml(MGL_TOFIX(maxy));
        poly[1].x := MGL_randoml(MGL_TOFIX(maxx));
        poly[1].y := MGL_randoml(MGL_TOFIX(maxy));
        poly[2].x := MGL_randoml(MGL_TOFIX(maxx));
        poly[2].y := MGL_randoml(MGL_TOFIX(maxy));

        MGL_setColor(randomColor);
        MGL_fillPolygonFX(3,@poly[0],sizeof(fxpoint_t),0,0);
    end;

    defaultAttributes(dc);
    Result := Pause;
end;

{-- PolyDemo -----------------------------------------------------------------}

// Display a random pattern of polygons on the screen with
// random fill styles.
function    PolyDemo(dc: PMGLDC): Boolean;
var
    i,maxx,maxy,val : int;
    // Space to hold polygon data
    poly            : array[0..MAXPTS-1] of fxpoint_t;
begin
    MainWindow(dc,'MGL_fillPolygon Demonstration');
    StatusLine('Press any key to continue, ESC to Abort');

    maxx := MGL_maxx;
    maxy := MGL_maxy;

    while not CheckEvent do
    begin
        // Define a random polygon
        for i := 0 to MAXPTS - 1 do
        begin
            poly[i].x := MGL_randoml(MGL_TOFIX(maxx));
            poly[i].y := MGL_randoml(MGL_TOFIX(maxy));
        end;

        MGL_setColor(randomColor);
        MGL_setBackColor(randomColor);

        val := MGL_random(3);
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

        MGL_fillPolygonFX(MAXPTS,@poly[0],sizeof(fxpoint_t),0,0);
    end;

    defaultAttributes(dc);
    Result := Pause;
end;

{-- Demo ---------------------------------------------------------------------}

procedure   Demo(dc: PMGLDC);
begin
    // Display title message at the top of the window.
    MainWindow(dc,'Polygon Demo');

    if not FastPolyDemo(dc) then Exit;
    if not PolyDemo(dc) then Exit;
end;

initialization
    RegisterDemo('Polys',
                 'Simple demo showing how to draw complex and convex polygons '+
                 'on the screen.',
                 Demo);
end.
