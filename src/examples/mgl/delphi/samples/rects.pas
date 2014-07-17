// Display a number of random rectangles with random attributes.
unit rects;

interface

implementation

uses
    Patterns,
    MGLFx,
    MGLTypes,
    MGLSamp;

{-- Demo ---------------------------------------------------------------------}

procedure   Demo(dc: PMGLDC);
var
    maxx,maxy,val   : int;
    r               : rect_t;
begin
    // Display title message at the top of the window.
    MainWindow(dc,'Rectangle Demonstration');
    StatusLine('Press any key to continue, ESC to Abort');

    maxx := MGL_maxx;
    maxy := MGL_maxy;

    while not CheckEvent do
    begin
        r.left      := MGL_random(maxx);
        r.right     := MGL_random(maxx);
        r.top       := MGL_random(maxy);
        r.bottom    := MGL_random(maxy);

        // Fix the rectangle so it is not empty
        if r.right < r.left then
            Swap(r.left,r.right);
        if r.bottom < r.top then
            Swap(r.top,r.bottom);

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

        MGL_fillRect(r);
    end;

    defaultAttributes(dc);
    Pause;
end;

initialization
    RegisterDemo('Rects',
                 'Display a number of random rectangles with random attributes.',
                 Demo);
end.
