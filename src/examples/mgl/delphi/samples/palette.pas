// Simple demo showing how to use the palette functions and
// to do palette animation and fades using the mgl.
unit Palette;

interface

implementation

uses
    MGLFx,
    MGLTypes,
    MGLSamp;

{-- ColorDemo ----------------------------------------------------------------}

// Display the current color palette on the screen.
function    ColorDemo(dc: PMGLDC): Boolean;
var
    color,width,height,x,y,i,j,top,bottom,start : int;
    pal : array[0..255] of palette_t;
begin
    MainWindow(dc,'Color Demonstration');

    if MaxColorUns <= 15 then
    begin
        // Simple color demonstration for 16 color displays
        width   := 2 * ((MGL_maxx+1) div 16);
        height  := 2 * ((MGL_maxy-10) div 10);

        x       := width div 2;
        y       := height div 2;
        color   := 1;
        for j := 0 to 2 do
        begin
            for i := 0 to 4 do
            begin
                MGL_setColor(color);
                Inc(color);
                MGL_fillRectCoord(x,y,x+width,y+height);
                Inc(x,(width div 2) * 3);
            end;
            Inc(y,(height div 2) * 3);
            x := width div 2;
        end;
    end
    else if MaxColorUns = 255 then
    begin
        // Color demonstration for large numbers of colors
        width   := 2 * ((MGL_maxx+1) div 46);
        height  := 2 * ((MGL_maxy-10) div 47);

        x       := width div 2;
        y       := height div 2;
        color   := 1;
        for j := 0 to 15 do
        begin
            for i := 0 to 15 do
            begin
                MGL_setColor(color);
                Inc(color);
                MGL_fillRectCoord(x,y,x+width,y+height);
                Inc(x,(width div 2) * 3);
            end;
            Inc(y,(height div 2) * 3);
            x := width div 2;
        end;

        //MGL_setColor(MGL_defaultColor());
        StatusLine('Press a key to rotate palette');
        WaitEvent;
        StatusLine('Press a key to rotate other direction');

        // Palette rotations
        while not checkEvent do
        begin
            // Copy image from image in memory to the screen.
            MGL_rotatePalette(dc,palsize-1,1,MGL_ROTATE_UP);
            MGL_realizePalette(dc,palsize-1,1,1);
        end;
        WaitEvent;
        StatusLine('Press a key to fade palette');

        while not checkEvent do
        begin
            MGL_rotatePalette(dc,palsize-1,1,MGL_ROTATE_DOWN);
            MGL_realizePalette(dc,palsize-1,1,1);
        end;
        WaitEvent;

        // Palette fade out
        MGL_getPalette(dc,@pal[0],palsize,0);
        for i := 63 downto 0 do
        begin
            MGL_fadePalette(dc,@pal[1],palsize-1,1,i*4);
            MGL_realizePalette(dc,palsize-1,1,1);
        end;

        // Palette fade in
        for i := 0 to 63 do
        begin
            MGL_fadePalette(dc,@pal[1],palsize-1,1,i*4);
            MGL_realizePalette(dc,palsize-1,1,1);
        end;
    end
    else
    begin
        // Color demonstration for Dithered/HiColor/TrueColor modes
        start   := MGL_maxx div 8;
        width   := MGL_maxx - start*2;

        top     := MGL_maxy div 8;
        bottom  := MGL_maxy div 8 + MGL_maxy div 5;

        for x := 0 to width - 1 do
        begin
            MGL_setColor(MGL_packColor(@dc^.pf,(LongInt(x) * 255) div width,0,0));
            MGL_lineCoord(start+x,top,start+x,bottom);
        end;

        Inc(top,MGL_maxy div 5);
        Inc(bottom,MGL_maxy div 5);
        for x := 0 to width - 1 do
        begin
            MGL_setColor(MGL_packColor(@dc^.pf,0,(LongInt(x) * 255) div width,0));
            MGL_lineCoord(start+x,top,start+x,bottom);
        end;

        Inc(top,MGL_maxy div 5);
        Inc(bottom,MGL_maxy div 5);
        for x := 0 to width - 1 do
        begin
            MGL_setColor(MGL_packColor(@dc^.pf,0,0,(LongInt(x) * 255) div width));
            MGL_lineCoord(start+x,top,start+x,bottom);
        end;

        Inc(top,MGL_maxy div 5);
        Inc(bottom,MGL_maxy div 5);
        for x := 0 to width div 2 - 1 do
        begin
            MGL_setColor(MGL_packColor(@dc^.pf,
                                       (LongInt((width div 2)-x) * 255) div (width div 2),
                                       (LongInt(x) * 255) div (width div 2),
                                       0));
            MGL_lineCoord(start+x,top,start+x,bottom);
        end;
        for x := width div 2 to width - 1 do
        begin
            MGL_setColor(MGL_packColor(@dc^.pf,
                                       0,
                                       (LongInt((width div 2) - (x - (width div 2))) * 255) div (width div 2),
                                       ((x-width div 2) * 255) div (width div 2)));
            MGL_lineCoord(start+x,top,start+x,bottom);
        end;
    end;

    MGL_setColor(MGL_defaultColor);
    Result := Pause;
    MGL_setPalette(dc,defPal,palsize,0);
    MGL_realizePalette(dc,MGL_getPaletteSize(dc),0,1);
end;

{-- Demo ---------------------------------------------------------------------}

procedure   Demo(dc: PMGLDC);
begin
    MainWindow(dc,'Palette Demonstration');
    StatusLine('Press any key to continue, ESC to Abort');

    ColorDemo(dc);
end;

initialization
    RegisterDemo('Palette',
                 'Simple demo showing how to use the palette functions and '+
                 'to do palette animation and fades using the mgl.',
                 Demo);
end.
