// Simple hello program showing how to get up and running
// using the MGL.
unit Hello;

interface

implementation

uses
    MGLFx,
    MGLTypes,
    MGLSamp;

{-- Demo ---------------------------------------------------------------------}
procedure   Demo(dc: PMGLDC);
var
    xres, yres  : int;
    i           : int;

    function    FixColor(C: int): color_t;
    begin
        if MaxColor < 0 then { This occured then too many colors :) }
            Result := C
        else
            Result := C mod MaxColor;
    end;
begin
    // Draw a simple pattern of lines on the display
    xres := MGL_maxx+1;
    yres := MGL_maxy+1;

    i := 0;
    while i < xres do
    begin
        MGL_setColorCI(FixColor(i));
        MGL_lineCoord(xres div 2,yres div 2,i,0);
        MGL_setColorCI(FixColor(i+1));
        MGL_lineCoord(xres div 2,yres div 2,i,yres);
        Inc(i,5);
    end;

    i := 0;
    while i < yres do
    begin
        MGL_setColorCI(FixColor(i+2));
        MGL_lineCoord(xres div 2,yres div 2,0,i);
        MGL_setColorCI(FixColor(i+3));
        MGL_lineCoord(xres div 2,yres div 2,xres,i);
        Inc(i,5);
    end;

    MGL_setColorCI(MGL_WHITE);
    MGL_lineCoord(0,0,xres-1,0);
    MGL_lineCoord(0,0,0,yres-1);
    MGL_lineCoord(xres-1,0,xres-1,yres-1);
    MGL_lineCoord(0,yres-1,xres-1,yres-1);

    Pause;
end;

initialization
    RegisterDemo('Hello',
                 'Simple hello program showing how to get up and running '+
                 'using the MGL.',
                 Demo);
end.
