// Demo program for the MegaGraph graphics library. This
// program is very similar to the graphics demo program
// that comes with Borland's range of compiler, but has
// been extended to fully test the MGL library.
unit MGLDemo;

interface

implementation

uses
    SysUtils,
    Patterns,
    FFill,
    MGLTypes,
    MGLFx,
    MGLSamp;

{-- Constants ----------------------------------------------------------------}
const
    markerStyle : array[MGL_MARKER_SQUARE..MGL_MARKER_X] of string =
        (
            'MGL_MARKER_SQUARE',
            'MGL_MARKER_CIRCLE',
            'MGL_MARKER_X'
        );

    writeMode   : array[MGL_REPLACE_MODE..MGL_XOR_MODE] of string =
        (
            'MGL_REPLACE_MODE',
            'MGL_AND_MODE',
            'MGL_OR_MODE',
            'MGL_XOR_MODE'
        );

    fillStyle   : array[MGL_BITMAP_SOLID..MGL_PIXMAP] of string =
        (
            'MGL_BITMAP_SOLID',
            'MGL_BITMAP_OPAQUE',
            'MGL_BITMAP_TRANSPARENT',
            'MGL_PIXMAP'
        );

    polyStyle   : array[MGL_CONVEX_POLYGON..MGL_AUTO_POLYGON] of string =
        (
            'MGL_CONVEX_POLYGON',
            'MGL_COMPLEX_POLYGON',
            'MGL_AUTO_POLYGON'
        );

    textDirection: array[MGL_LEFT_DIR..MGL_DOWN_DIR] of string =
        (
            'MGL_LEFT_DIR',
            'MGL_UP_DIR',
            'MGL_RIGHT_DIR',
            'MGL_DOWN_DIR'
        );

    horizJust   : array[MGL_LEFT_TEXT..MGL_RIGHT_TEXT] of string =
        (
            'MGL_LEFT_TEXT',
            'MGL_CENTER_TEXT',
            'MGL_RIGHT_TEXT'
        );

    vertJust    : array[MGL_TOP_TEXT..MGL_BOTTOM_TEXT] of string =
        (
            'MGL_TOP_TEXT',
            'MGL_CENTER_TEXT',
            'MGL_BOTTOM_TEXT'
        );

    // Names of all vector fonts on disk

    NumFonts    = 24;

    fontFilename: array[0..NumFonts-1] of string =
        (
            'astrolog.fnt',
            'cyrillc.fnt',
            'gotheng.fnt',
            'gothger.fnt',
            'gothita.fnt',
            'greekc.fnt',
            'greekcs.fnt',
            'greeks.fnt',
            'italicc.fnt',
            'italiccs.fnt',
            'italict.fnt',
            'japanese.fnt',
            'little.fnt',
            'mathbig.fnt',
            'mathsmal.fnt',
            'musical.fnt',
            'romanc.fnt',
            'romancs.fnt',
            'romand.fnt',
            'romans.fnt',
            'romant.fnt',
            'scriptc.fnt',
            'scripts.fnt',
            'symbol.fnt'
        );

// Implementation

{-- ReportStatus -------------------------------------------------------------}

// Report the current graphics system configuration after
// graphics has been initialised.

function    ReportStatus(dc: PMGLDC): Boolean;
var
    Driver  : int;
    Mode    : int;
    Attr    : attributes_t;
begin
    Driver  := MGL_getDriver(dc);
    Mode    := MGL_getMode(dc);

    MainWindow(dc,'Status report after MGL_init');

    MGL_moveToCoord(10,4);

    // Get required information
    MGL_getAttributes(@attr);

    gprintf('Graphics device    : %s', [MGL_driverName(driver)]);
    gprintf('Graphics mode      : %s (%s)', [MGL_modeName(mode), MGL_modeDriverName(mode)]);
    gprintf('Screen resolution  : ( 0, 0, %d, %d )',[MGL_sizex(dc),MGL_sizey(dc)]);
    gprintf('View port          : ( %d, %d, %d, %d )',
        [attr.viewPort.left,attr.viewPort.top,
        attr.viewPort.right,attr.viewPort.bottom]);
    gprintf('Clipping rectangle : ( %d, %d, %d, %d )',
        [attr.clipRect.left,attr.clipRect.top,
        attr.clipRect.right,attr.clipRect.bottom]);
    if attr.clip <> 0 then
        gprint('Clipping           : ON')
    else
        gprint('Clipping           : OFF');
    gprintf('Current position   : ( %d, %d )', [MGL_getX,MGL_getY]);
    gprintf('Maximum color      : %g', [MaxColorUns]);
    gprintf('Color              : %d', [attr.color]);
    gprintf('Background color   : %d', [attr.backColor]);
    gprintf('Border dark color  : %d', [attr.bdrDark]);
    gprintf('Border bright color: %d', [attr.bdrBright]);
    gprintf('Marker color       : %d', [attr.markerColor]);
    gprintf('Marker size        : %d', [attr.markerSize]);
    gprintf('Marker style       : %s', [markerStyle[attr.markerStyle]]);
    gprintf('Write mode         : %s', [writeMode[attr.writeMode]]);
    gprintf('Pen height         : %d', [attr.penHeight]);
    gprintf('Pen width          : %d', [attr.penWidth]);
    gprintf('Fill style         : %s', [fillStyle[attr.penStyle]]);
    gprintf('Polygon style      : %s', [polyStyle[attr.polyType]]);
    gprintf('Maximum Page Index : %d', [MGL_maxPage(dc)]);

    gprint('');
    gprint('Text settings');
    gprint('');

    gprintf('Current font       : %s', [attr.ts.font^.name]);
    gprintf('Text direction     : %s', [textDirection[attr.ts.dir]]);
    gprintf('Horizontal justify : %s', [horizJust[attr.ts.horizJust]]);
    gprintf('Vertical justify   : %s', [vertJust[attr.ts.vertJust]]);
    gprintf('Horizontal size    : ( %d / %d )', [attr.ts.szNumerx,attr.ts.szDenomx]);
    gprintf('Vertical size      : ( %d / %d )', [attr.ts.szNumery,attr.ts.szDenomy]);
    gprintf('Space extra        : %d', [attr.ts.spaceExtra]);

    Result := Pause;
end;

{-- ColorDemo ----------------------------------------------------------------}

// Display the current color palette on the screen.

function    ColorDemo(dc: PMGLDC): Boolean;
var
    color,width,height,x,y,i,j,top,bottom,start: int;
    pal: array[0..255] of palette_t;
begin
    MainWindow(dc,'Color Demonstration');

    if (MaxColorUns <= 15) then
    begin
        // Simple color demonstration for 16 color displays 

        width := 2 * ((MGL_maxx()+1) div 16);
        height := 2 * ((MGL_maxy()-10) div 10);

        x := width div 2;
        y := height div 2;
        color := 1;
        for j := 0 to 2 do
        begin
            for i := 0 to 4 do
            begin
                MGL_setColor(color);
                Inc(color);
                MGL_fillRectCoord(x,y,x+width,y+height);
                Inc(x, (width div 2) * 3);
            end;
            Inc(y, (height div 2) * 3);
            x := width div 2;
        end;
    end
    else if MaxColorUns = 255 then
    begin
        // Color demonstration for large numbers of colors
        width   := 2 * ((MGL_maxx+1) div 46);
        height  := 2 * ((MGL_maxy-10) div 47);

        x := width div 2;
        y := height div 2;
        color := 1;

        for j := 0 to 15 do
        begin
            for i := 0 to 15 do
            begin
                MGL_setColor(color);
                Inc(color);
                MGL_fillRectCoord(x,y,x+width,y+height);
                Inc(x,(width div 2) * 3);
            end;
            Inc(y, (height div 2) * 3);
            x := width div 2;
        end;

        MGL_setColor(MGL_defaultColor);
        statusLine('Press a key to rotate palette');
        waitEvent;

        statusLine('Press a key to rotate other direction');

        // Palette rotations
        while not checkEvent do
        begin
            MGL_rotatePalette(dc,palsize-1,1,MGL_ROTATE_UP);
            MGL_realizePalette(dc,palsize-1,1,1);
        end;
        waitEvent;

        statusLine('Press a key to fade palette');

        while not checkEvent do
        begin
            MGL_rotatePalette(dc,palsize-1,1,MGL_ROTATE_DOWN);
            MGL_realizePalette(dc,palsize-1,1,1);
        end;
        waitEvent;

        // Palette fade out
        MGL_getPalette(dc,@pal,palsize,0);
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
            MGL_setColor(MGL_packColor(@dc.pf,(long(x) * 255) div width,0,0));
            MGL_lineCoord(start+x,top,start+x,bottom);
        end;

        Inc(top, MGL_maxy div 5);
        Inc(bottom, MGL_maxy div 5);

        for x := 0 to width - 1 do
        begin
            MGL_setColor(MGL_packColor(@dc.pf,0,(long(x) * 255) div width,0));
            MGL_lineCoord(start+x,top,start+x,bottom);
        end;

        Inc(top, MGL_maxy div 5);
        Inc(bottom, MGL_maxy div 5);

        for x := 0 to width - 1 do
        begin
            MGL_setColor(MGL_packColor(@dc.pf,0,0,(long(x) * 255) div width));
            MGL_lineCoord(start+x,top,start+x,bottom);
        end;

        Inc(top, MGL_maxy div 5);
        Inc(bottom, MGL_maxy div 5);

        for x := 0 to width div 2 - 1 do
        begin
            MGL_setColor(MGL_packColor(@dc.pf,
                                       (long((width div 2)-x) * 255) div (width div 2),
                                       (long(x) * 255) div (width div 2),
                                       0
                                      ));
            MGL_lineCoord(start+x,top,start+x,bottom);
        end;

        for x := width div 2 to width - 1 do
        begin
            MGL_setColor(MGL_packColor(@dc.pf,0,
                (long((width div 2) - (x - (width div 2))) * 255) div (width div 2),
                (long(x-width div 2) * 255) div (width div 2)));
            MGL_lineCoord(start+x,top,start+x,bottom);
        end;
    end;

    MGL_setColor(MGL_defaultColor);
    MGL_setPalette(dc,defPal,palsize,0);
    MGL_realizePalette(dc,MGL_getPaletteSize(dc),0,1);

    Result := Pause;
end;

{-- PaletteDemo --------------------------------------------------------------}

// Change the palette of the screen until the user has had enough.

function    PaletteDemo(dc: PMGLDC): Boolean;
var
    i,j,x,y,color,height,width: int;
begin
    if MaxColorUns > 255 then
    begin
        Result := True;
        Exit;
    end;

    MainWindow(dc,'Palette Demonstration');
    StatusLine('Press any key to continue, ESC to Abort');

    width   := (MGL_maxx+1) div 15; // Compute width of boxes
    height  := (MGL_maxy+1) div 10; // Compute height of boxes

    x       := 0;
    y       := 0;                   // Start in upper corner
    color   := 1;                   // Begin width 1st color        

    for j := 0 to 9 do
    begin
        // For 10 rows of boxes
        for i := 0 to 14 do
        begin
            // For 15 columns of boxes
            MGL_setColor(color);
            Inc(color);
            MGL_fillRectCoord(x,y,x+width+1,y+height+1);
            Inc(x, width + 1);  // Advance to next col          

            // Choose color out of order
            color := 1 + (color mod 14);
        end;

        x := 0;
        Inc(y, height + 1);
    end;

    while not checkEvent do
    begin
        // Until user hits a key...
        MGL_setPaletteEntry(dc,1+MGL_random(14),
            byte(MGL_random(PALMAX+1)),
            byte(MGL_random(PALMAX+1)),
            byte(MGL_random(PALMAX+1)));
        MGL_realizePalette(dc,MGL_getPaletteSize(dc),0,1);
    end;

    MGL_setColor(MGL_defaultColor);
    MGL_setPalette(dc,defPal,palsize,0);
    MGL_realizePalette(dc,MGL_getPaletteSize(dc),0,1);

    Result := Pause;
end;

{-- PixelDemo ----------------------------------------------------------------}

// Display a pattern of random dots on the screen and pick them back
// up again.

function    PixelDemo(dc: PMGLDC): Boolean;
var
    seed: int;
    i,x,y,maxx,maxy: int;
    color: color_t;
begin
    seed    := 1958;
    MainWindow(dc,'MGL_pixel / MGL_getPixel Demonstration');

    maxx    := MGL_maxx;
    maxy    := MGL_maxy;

    MGL_srand(seed);                    // Seed random number gen with known value

    MGL_beginPixel;

    // Put 10000 pixels on screen
    for i := 0 to 9999 do
    begin
        x := 1 + MGL_random(maxx);      // Generate a random location
        y := 1 + MGL_random(maxy);
        MGL_setColor(randomColor);
        MGL_pixelCoordFast(x,y);
    end;

    MGL_srand(seed);                    // Reseed random number gen with same value

    MGL_setColorCI(MGL_BLACK);          // Draw over in MGL_BLACK

    // Take the 10000 pixels off
    for i := 0 to 9999 do
    begin
        x := 1 + MGL_random(maxx);      // Generate a random location
        y := 1 + MGL_random(maxy);
        color := MGL_getPixelCoordFast(x,y);
        if color = randomColor then     // Keep random in sync
            MGL_pixelCoordFast(x,y);
    end;

    MGL_endPixel;
    MGL_setColor(MGL_defaultColor);

    Result := Pause;
end;

{-- RectangleDemo ------------------------------------------------------------}

// Display a number of random rectangles with random attributes.

function    RectangleDemo(dc: PMGLDC): Boolean;
var
    maxx,maxy,val: int;
    r: rect_t;
begin
    MainWindow(dc,'Random rectangles');
    StatusLine('Press any key to continue, ESC to Abort');

    maxx := MGL_maxx;
    maxy := MGL_maxy;

    while not checkEvent do
    begin
        r.left      := MGL_random(maxx);
        r.right     := MGL_random(maxx);
        r.top       := MGL_random(maxy);
        r.bottom    := MGL_random(maxy);

        // Fix the rectangle so it is not empty
        if (r.right < r.left) then
            Swap(r.left,r.right);
        if (r.bottom < r.top) then
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
    Result := Pause;
end;

{-- CircleDemo ---------------------------------------------------------------}

// Display a random pattern of circles on the screen
// until the user hits a key.

function    CircleDemo(dc: PMGLDC): Boolean;
var
    maxx,maxy,val: int;
    r: rect_t;
begin
    MainWindow(dc,'Circle Demonstration');
    StatusLine('Press any key to continue, ESC to Abort');

    maxx := MGL_maxx;
    maxy := MGL_maxy;

    while not checkEvent do
    begin
        r.left      := MGL_random(maxx-100);
        r.top       := MGL_random(maxy-100);
        r.right     := r.left + MGL_random(100);
        r.bottom    := r.top + MGL_random(100);

        MGL_setColor(randomColor);
        MGL_setBackColor(randomColor);

        MGL_setPenSize(MGL_random(5)+1,MGL_random(5)+1);

        val := MGL_random(10);
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

        MGL_ellipse(r);
    end;

    defaultAttributes(dc);
    Result := Pause;
end;

{-- FilledCircleDemo ---------------------------------------------------------}

// Display a random pattern of filled circles on the screen
// until the user hits a key.

function    FilledCircleDemo(dc: PMGLDC): Boolean;
var
    maxx,maxy,val: int;
    r: rect_t;
begin
    MainWindow(dc,'Filled circle Demonstration');
    StatusLine('Press any key to continue, ESC to Abort');

    maxx := MGL_maxx;
    maxy := MGL_maxy;

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

{-- ArcDemo ------------------------------------------------------------------}

// Display a random pattern of elliptical arc's on the screen
// until the user hits a key.

function    ArcDemo(dc: PMGLDC): Boolean;
var
    maxx,maxy,startAngle,endAngle: int;
    r: rect_t;
begin
    MainWindow(dc,'Arc Demonstration');
    StatusLine('Press any key to continue, ESC to Abort');

    maxx := MGL_maxx;
    maxy := MGL_maxy;

    while not checkEvent do
    begin
        r.left      := MGL_random(maxx-100);
        r.top       := MGL_random(maxy-100);
        r.right     := r.left + MGL_random(100);
        r.bottom    := r.top + MGL_random(100);
        startAngle  := MGL_random(360);
        endAngle    := MGL_random(360);

        MGL_setColor(randomColor);
        MGL_ellipseArc(r,startAngle,endAngle);
    end;

    defaultAttributes(dc);
    Result := Pause;
end;

{-- FilledArcDemo ------------------------------------------------------------}

// Display a random pattern of filled arcs on the screen
// until the user hits a key.

function    FilledArcDemo(dc: PMGLDC): Boolean;
var
    maxx,maxy,startAngle,endAngle,val: int;
    r: rect_t;
begin
    MainWindow(dc,'Filled Arc Demonstration');
    StatusLine('Press any key to continue, ESC to Abort');

    maxx := MGL_maxx;
    maxy := MGL_maxy;

    while not checkEvent do
    begin
        r.left      := MGL_random(maxx-100);
        r.top       := MGL_random(maxy-100);
        r.right     := r.left + MGL_random(100);
        r.bottom    := r.top + MGL_random(100);
        startAngle  := MGL_random(360);
        endAngle    := MGL_random(360);

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

        MGL_fillEllipseArc(r,startAngle,endAngle);
    end;

    defaultAttributes(dc);
    Result := Pause;
end;

{-- Adjasp -------------------------------------------------------------------}

function    Adjasp(y: int): int;
begin
    Result := Trunc(AspectRatio * y);
end;

{-- ToRad --------------------------------------------------------------------}

function    ToRad(d: int): double;
begin
    Result := (d * Pi) / 180.0;
end;

{-- PieDemo ------------------------------------------------------------------}

// Description:  Draws a simple pie chart on the screen.

function    PieDemo(dc: PMGLDC): Boolean;
var
    xcenter,ycenter,radius,lradius: int;
    x,y: int;
    radians,piesize: double;
    coords: arc_coords_t;
begin
    MainWindow(dc,'Pie Chart Demonstration');

    xcenter := MGL_maxx() div 2;        // Center the Pie horizontally
    ycenter := MGL_maxy() div 2 + 20;   // Center the Pie vertically
    radius  := MGL_maxx() div 5;        // It will cover 2/5ths screen
    piesize := MGL_maxy() / 4.0;        // Optimum height ratio of pie

    while (AspectRatio*radius) < piesize do
        Inc(radius);

    lradius := radius + (radius div 5); // Labels placed 20% farther

    MGL_useFont(largeFont);
    MGL_setTextJustify(MGL_CENTER_TEXT,MGL_TOP_TEXT);
    MGL_drawStrXY(MGL_maxx div 2, 6, 'This is a Pie Chart');

    MGL_setTextSize(2,3,2,3);

    MGL_setColorCI(MGL_RED);
    MGL_fillEllipseArcCoord(xcenter+10,ycenter-Adjasp(10),
                            radius,Adjasp(radius),0,90);
    MGL_setColorCI(MGL_WHITE);
    MGL_setPenStyle(MGL_BITMAP_SOLID);
    MGL_ellipseArcCoord(xcenter+10,ycenter-Adjasp(10),
                        radius,Adjasp(radius),0,90);
    MGL_getArcCoords(@coords);
    MGL_lineCoord(coords.x,coords.y,coords.startX,coords.startY);
    MGL_lineCoord(coords.x,coords.y,coords.endX,coords.endY);
    radians := ToRad(45);
    x := xcenter + Trunc(cos(radians) * lradius);
    y := ycenter - Trunc(sin(radians) * lradius * aspectRatio );
    MGL_setTextJustify(MGL_LEFT_TEXT,MGL_BOTTOM_TEXT);
    MGL_drawStrXY(x,y,'25 %');

    MGL_setColorCI(MGL_GREEN);
    MGL_setPenStyle(MGL_BITMAP_OPAQUE);
    MGL_setPenBitmapPattern(@bitpat[3]);
    MGL_fillEllipseArcCoord(xcenter,ycenter,radius,Adjasp(radius),90,135);
    MGL_setColorCI(MGL_WHITE);
    MGL_setPenStyle(MGL_BITMAP_SOLID);
    MGL_ellipseArcCoord(xcenter,ycenter,radius,Adjasp(radius),90,135);
    MGL_getArcCoords(@coords);
    MGL_lineCoord(coords.x,coords.y,coords.startX,coords.startY);
    MGL_lineCoord(coords.x,coords.y,coords.endX,coords.endY);
    radians := ToRad(113);
    x := xcenter + Trunc(cos(radians) * lradius);
    y := ycenter - Trunc(sin(radians) * lradius * aspectRatio );
    MGL_setTextJustify(MGL_RIGHT_TEXT,MGL_BOTTOM_TEXT);
    MGL_drawStrXY(x,y,'12.5 %');

    MGL_setColorCI(MGL_YELLOW);
    MGL_setPenStyle(MGL_BITMAP_OPAQUE);
    MGL_setPenBitmapPattern(@bitpat[7]);
    MGL_fillEllipseArcCoord(xcenter-10,ycenter,radius,Adjasp(radius),135,225);
    MGL_setColorCI(MGL_WHITE);
    MGL_setPenStyle(MGL_BITMAP_SOLID);
    MGL_ellipseArcCoord(xcenter-10,ycenter,radius,Adjasp(radius),135,225);
    MGL_getArcCoords(@coords);
    MGL_lineCoord(coords.x,coords.y,coords.startX,coords.startY);
    MGL_lineCoord(coords.x,coords.y,coords.endX,coords.endY);
    radians := ToRad(180);
    x := xcenter + Trunc(cos(radians) * lradius);
    y := ycenter - Trunc(sin(radians) * lradius * aspectRatio );
    MGL_setTextJustify(MGL_RIGHT_TEXT,MGL_CENTER_TEXT);
    MGL_drawStrXY(x,y,'25 %');

    MGL_setColorCI(MGL_LIGHTBLUE);
    MGL_setPenStyle(MGL_BITMAP_OPAQUE);
    MGL_setPenBitmapPattern(@bitpat[10]);
    MGL_fillEllipseArcCoord(xcenter,ycenter,radius,Adjasp(radius),225,360);
    MGL_setColorCI(MGL_WHITE);
    MGL_setPenStyle(MGL_BITMAP_SOLID);
    MGL_ellipseArcCoord(xcenter,ycenter,radius,Adjasp(radius),225,360);
    MGL_getArcCoords(@coords);
    MGL_lineCoord(coords.x,coords.y,coords.startX,coords.startY);
    MGL_lineCoord(coords.x,coords.y,coords.endX,coords.endY);
    radians := ToRad(293);
    x := xcenter + Trunc(cos(radians) * lradius );
    y := ycenter - Trunc(sin(radians) * lradius * aspectRatio );
    MGL_setTextJustify(MGL_LEFT_TEXT,MGL_TOP_TEXT);
    MGL_drawStrXY(x,y,'37.5 %');

    defaultAttributes(dc);
    Result := Pause;
end;

{-- LineRelDemo --------------------------------------------------------------}

// Display a pattern using relative move and line commands.

function    LineRelDemo(dc: PMGLDC): Boolean;
var
    h,w,dx,dy,cx,cy: int;
    outs: array[0..6] of point_t;
begin
    MainWindow(dc,'MGL_moveRel / MGL_lineRel Demonstration');

    cx := MGL_maxx div 2;        // Center of the viewport
    cy := MGL_maxy div 2;
    h  := MGL_maxy div 8;
    w  := MGL_maxx div 9;
    dx := 2 * w;
    dy := 2 * h;

    MGL_setBackColor(MGL_realColor(dc,MGL_BLUE));
    MGL_clearViewport;

    outs[0].x := cx -  dx;
    outs[0].y := cy -  dy;
    outs[1].x := cx - (dx-w);
    outs[1].y := cy - (dy+h);
    outs[2].x := cx +  dx;
    outs[2].y := cy - (dy+h);
    outs[3].x := cx +  dx;
    outs[3].y := cy +  dy;
    outs[4].x := cx + (dx-w);
    outs[4].y := cy + (dy+h);
    outs[5].x := cx -  dx;
    outs[5].y := cy + (dy+h);
    outs[6].x := cx -  dx;
    outs[6].y := cy -  dy;

    MGL_setColorCI(MGL_WHITE);
    MGL_fillPolygon(7,@outs[0],0,0);

    outs[0].x := cx - (w div 2);
    outs[0].y := cy + h;
    outs[1].x := cx + (w div 2);
    outs[1].y := cy + h;
    outs[2].x := cx + (w div 2);
    outs[2].y := cy - h;
    outs[3].x := cx - (w div 2);
    outs[3].y := cy - h;
    outs[4].x := cx - (w div 2);
    outs[4].y := cy + h;

    MGL_setColorCI(MGL_BLUE);
    MGL_fillPolygon(5,@outs[0],0,0);

    // Draw a Tesseract object on the screen using the MGL_lineRel and
    // MGL_moveRel drawing commands.

    MGL_moveToCoord(cx-dx,cy-dy);
    MGL_lineRelCoord(w,-h);
    MGL_lineRelCoord(3*w,0);
    MGL_lineRelCoord(0, 5*h);
    MGL_lineRelCoord(-w,h);
    MGL_lineRelCoord(-3*w,0);
    MGL_lineRelCoord(0,-5*h);

    MGL_moveRelCoord(w,-h);
    MGL_lineRelCoord(0,5*h);
    MGL_lineRelCoord(w+(w div 2),0);
    MGL_lineRelCoord(0,-3*h);
    MGL_lineRelCoord(w div 2,-h);
    MGL_lineRelCoord(0,5*h);

    MGL_moveRelCoord(0,-5*h);
    MGL_lineRelCoord(-(w+(w div 2)),0);
    MGL_lineRelCoord(0,3*h);
    MGL_lineRelCoord(-w div 2,h);

    MGL_moveRelCoord(w div 2,-h);
    MGL_lineRelCoord(w,0);

    MGL_moveRelCoord(0,-2*h);
    MGL_lineRelCoord(-w,0);

    defaultAttributes(dc);
    Result := Pause;
end;

{-- LineToDemo ---------------------------------------------------------------}

const
    MAXPTS  = 15;

// Display a pattern using MGL_lineTo and MGL_moveTo commands.

function    LineToDemo(dc: PMGLDC): Boolean;
var
    points: array[0..MAXPTS-1] of point_t;
    i,j,h,w,xcenter,ycenter: int;
    radius,angle,step: int;
    rads: double;
begin
    MainWindow(dc,'MGL_moveTo / MGL_lineTo Demonstration');

    w := MGL_maxx;
    h := MGL_maxy;

    xcenter := w div 2;          // Determine the center of circle
    ycenter := h div 2;
    radius  := Trunc((h - 30) / (aspectRatio * 2));
    step    := 360 div MAXPTS;   // Determine # of increments        

    angle   := 0;
    for i := 0 to MAXPTS - 1 do
    begin
        // Determine circle intercepts
        rads        := angle * Pi / 180.0;  // Convert angle to radians
        points[i].x := xcenter + Trunc(cos(rads) * radius );
        points[i].y := ycenter - Trunc(sin(rads) * radius * AspectRatio);
        Inc(angle,step);                    // Move to next increment
    end;

    // Draw bounding circle
    MGL_ellipseCoord(xcenter,ycenter,radius,Trunc(radius*aspectRatio));

    for i := 0 to MAXPTS - 1 do
    begin
        // Draw the chords to the circle
        for j := i to MAXPTS - 1 do
        begin
            // For each remaining intersect
            MGL_moveTo(points[i]);  // Move to beginning of chord   
            MGL_lineTo(points[j]);  // Draw the chord
        end;
    end;

    Result := Pause;
end;

{-- LineDemo -----------------------------------------------------------------}

// Draw a whole bunch of random lines with random pen sizes
// and pattern modes.

function    LineDemo(dc: PMGLDC): Boolean;
var
    maxx,maxy,val   : int;
    p1,p2           : point_t;
begin
    MainWindow(dc,'Line Demonstration');
    StatusLine('Press any key to continue, ESC to Abort');

    maxx := MGL_maxx;
    maxy := MGL_maxy;

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
        else if val = 1 then
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

{-- TextDump -----------------------------------------------------------------}

// Displays all the characters in each of the fonts in
// the distribution.

function    TextDump(dc: PMGLDC): Boolean;
var
    i,j,maxx: int;
    font    : pfont_t;
    err     : int;
begin
    maxx := MGL_maxx;

    for i := 0 to NumFonts - 1 do
    begin
        // For each available font
        // Attempt to load the font file from disk. If it cannot be
        // found, then go onto the next one
        font := MGL_loadFont(PChar(fontFilename[i]));
        err  := MGL_result;
        if err = grFontNotFound then
            Continue
        else if font = nil then
            raise Exception.Create('Error loading font');

        try
            MainWindow(dc,Format('Character set (%s)', [font^.name]));

            // Display all characters on the screen
            MGL_useFont(font);
            for j := Ord(' ') to 255 do
            begin
                MGL_drawStr(PChar(string(Chr(j))));
                if (MGL_getX + MGL_maxCharWidth > maxx) then
                    MGL_moveToCoord(0,MGL_getY + MGL_textHeight);
            end;
        finally
            // Unload the font from memory
            MGL_unloadFont(font);
        end;

        MGL_useFont(defFont);
        if not Pause then
        begin
            Result := False;
            Exit;
        end;
    end;

    defaultAttributes(dc);
    Result := True;
end;

{-- TextStyleDemo ------------------------------------------------------------}

// Show the large font in several different sizes and
// orientations.

function    TextStyleDemo(dc: PMGLDC): Boolean;
var
    i,x,y: int;
begin
    MainWindow(dc,'Text styles Demonstration');

    MGL_useFont(largeFont);

    x := 5;
    y := 5;
    for i := 5 downto 2 do
    begin
        MGL_setTextSize(2,i,2,i);
        MGL_drawStrXY(x,y,PChar(Format('Size (2/%d)',[i])));
        Inc(y, MGL_textHeight);
    end;

    MGL_setTextSize(2,3,2,3);

    x := MGL_maxx div 2;
    y := MGL_maxy div 2;

    MGL_setTextDirection(MGL_LEFT_DIR);
    MGL_drawStrXY(x,y,'This goes left');

    MGL_setTextDirection(MGL_UP_DIR);
    MGL_drawStrXY(x,y,'This goes up');

    MGL_setTextDirection(MGL_RIGHT_DIR);
    MGL_drawStrXY(x,y,'This goes right');

    MGL_setTextDirection(MGL_DOWN_DIR);
    MGL_drawStrXY(x,y,'This goes down');

    defaultAttributes(dc);
    Result := Pause;
end;

{-- PatternDemo --------------------------------------------------------------}

// Display the set of fill patterns defined in this demo
// program.

function    PatternDemo(dc: PMGLDC): Boolean;
var
    i,j,x,y,height,width: int;
begin
    MainWindow(dc,'Pattern Demonstration');

    width  := (MGL_maxx+1) div 20;      // Compute width of boxes
    height := (MGL_maxy+1) div 9;       // Compute height of boxes

    x      := 10;
    y      := 10;                       // Start in upper corner

    MGL_setColorCI(MGL_LIGHTGRAY);
    MGL_setPenStyle(MGL_BITMAP_TRANSPARENT);

    for j := 0 to 6 do
    begin
        // For 7 rows of boxes
        for i := 0 to 14 do
        begin
            // For 15 columns of boxes
            MGL_setPenBitmapPattern(@bitpat[j*15 + i]);
            MGL_fillRectCoord(x,y,x+width+1,y+height+1);
            Inc(x, width + 10);         // Advance to next col
        end;
        x := 10;
        Inc(y, height + 10);
    end;

    MGL_setColor(MGL_defaultColor());
    Result := Pause;
end;

{-- PolyDemo -----------------------------------------------------------------}

// Maximum # of pts in polygon
const
    MaxPolyPts  = 6;

// Display a random pattern of polygons on the screen with
// random fill styles.

function    PolyDemo(dc: PMGLDC): Boolean;
var
    i,maxx,maxy,val: int;
    poly: array[0..MaxPolyPts-1] of fxpoint_t;  // Space to hold polygon data
begin
    MainWindow(dc,'MGL_fillPolygon Demonstration');
    StatusLine('Press any key to continue, ESC to Abort');

    maxx := MGL_maxx;
    maxy := MGL_maxy;

    while not checkEvent do
    begin
        // Define a random polygon
        for i := 0 to MaxPolyPts - 1 do
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

        MGL_fillPolygonFX(MaxPolyPts,@poly[0],sizeof(fxpoint_t),0,0);
    end;

    defaultAttributes(dc);
    Result := Pause;
end;

{-- FastPolyDemo -------------------------------------------------------------}

// Display a random pattern of convex triangular polygons
// in replace mode at full speed.

function    FastPolyDemo(dc: PMGLDC): Boolean;
var
    maxx,maxy   : int;
    // Space to hold polygon data
    poly        : array[0..2] of fxpoint_t;
begin
    MainWindow(dc,'MGL_fillPolygonFast Demonstration');
    StatusLine('Press any key to continue, ESC to Abort');

    maxx := MGL_maxx;
    maxy := MGL_maxy;

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
        MGL_fillPolygonFX(3,@poly[0],sizeof(fxpoint_t),0,0);
    end;

    defaultAttributes(dc);
    Result := Pause;
end;

{-- GouraudPolyDemo ----------------------------------------------------------}

// Display a random pattern of convex gouraud shaded
// triangular polygons at full speed.

function    GouraudPolyDemo(dc: PMGLDC): Boolean;
{$IFDEF MGLFIX3D}
var
    i,maxx,maxy : int;
    gpal        : array[0..255] of palette_t;
    poly        : array[0..2] of fxpointc_t;
    polyrgb     : array[0..2] of fxpointrgb_t;
begin
    // Gouraud shading is not supported for devices with less than 256
    // colors.
    if maxcolorUns < 255 then
    begin
        Result := True;
        Exit;
    end;

    MainWindow(dc,'Shaded polygon Demonstration');
    StatusLine('Press any key to continue, ESC to Abort');

    maxx = MGL_maxx;
    maxy = MGL_maxy;

    // Setup the palette to show smooth shading between bright MGL_RED and
    // bright MGL_BLUE. Note that we leave color 0 as MGL_BLACK.
    if (MGL_getBitsPerPixel(dc) = 8) and
       (MGL_getColorMapMode() <> MGL_DITHER_RGB_MODE) then
    begin
        gpal[0].red     := 0;
        gpal[0].green   := 0;
        gpal[0].blue    := 0;

        for i := 1 to MaxcolorUns do
        begin
            gpal[i].red     = Trunc((PALMAX * i) / MaxColorUns);
            gpal[i].green   = 0;
            gpal[i].blue    = Trunc((PALMAX * (MaxColorUns - i)) / MaxColorUns);
        end;

        MGL_setPalette(dc,gpal,MGL_getPaletteSize(dc),0);
        MGL_realizePalette(dc,MGL_getPaletteSize(dc),0,true);

        MGL_beginShadedDrawing;
        while not checkEvent do
        begin
            for i := 0 to 2 do
            begin
                poly[i].p.x := MGL_randoml(MGL_TOFIX(maxx));
                poly[i].p.y := MGL_randoml(MGL_TOFIX(maxy));
                poly[i].c   := MGL_TOFIX(1 + MGL_randoml(MaxColor-1));
            end;
            MGL_cTriFast(@poly[0],@poly[1],@poly[2],0,0);
        end;
        MGL_endShadedDrawing;
    end
    else
    begin
        MGL_beginDrawing;
        while not checkEvent do
        begin
            for i := 0 to 2 do
            begin
                polyrgb[i].p.x := MGL_randoml(MGL_TOFIX(maxx));
                polyrgb[i].p.y := MGL_randoml(MGL_TOFIX(maxy));
                polyrgb[i].c.r := MGL_randoml(MGL_TOFIX(255));
                polyrgb[i].c.g := MGL_randoml(MGL_TOFIX(255));
                polyrgb[i].c.b := MGL_randoml(MGL_TOFIX(255));
            end;
            MGL_rgbTriFast(@poly[0],@poly[1],@poly[2],0,0);
        end;
        MGL_endDrawing;
    end;

    defaultAttributes(dc);
    MGL_setPalette(dc,defPal,palsize,0);
    MGL_realizePalette(dc,MGL_getPaletteSize(dc),0,true);
    Result := Pause;
{$ELSE}
begin
    Result := True;
{$ENDIF}
end;

{-- FixAngle -----------------------------------------------------------------}

// Returns equivalent angle in the range 0-360 degrees.

function    FixAngle(angle: int): int;
begin
    while angle < 0 do Inc(angle,360);
    while angle >= 360 do Dec(angle,360);
    Result := angle;
end;

{-- PageFlipDemo -------------------------------------------------------------}

// If page flipping is available, this routine will perform
// a reasonably complex animation sequence. We use a smart
// dirty rectangle animation technique to only clear the
// portion of the display page that has changed to get high
// speed animation.

function    PageFlipDemo(dc: PMGLDC): Boolean;
var
    maxx,maxy,stepx,stepy,secAngle,minAngle: int;
    extent,dirtyRect: rect_t;
begin
    if (MGL_doubleBuffer(dc)) then
    begin
        MainWindow(dc,'Page flip animation demo');
        StatusLine('Press any key to continue, ESC to Abort');

        maxx := MGL_maxx;
        maxy := MGL_maxy;

        // Draw title again for hidden page

        MGL_swapBuffers(dc,MGL_waitVRT);

        MainWindow(dc,'Page flip animation demo');
        StatusLine('Press any key to continue, ESC to Abort');

        extent.left     := 0;
        extent.top      := 0;
        extent.right    := maxx div 5;
        extent.bottom   := (extent.right * 1000) div aspect;
        dirtyRect       := extent;

        stepx           := 1;
        stepy           := 1;
        secAngle        := 90;
        minAngle        := 90;

        while not checkEvent do
        begin
            MGL_setColorCI(MGL_BLACK);
            MGL_fillRect(dirtyRect);
            MGL_setColorCI(MGL_RED);
            MGL_fillEllipse(extent);
            MGL_setColorCI(MGL_WHITE);
            MGL_fillEllipseArc(extent,secAngle-5,secAngle);
            MGL_fillEllipseArc(extent,minAngle-5,minAngle);

            // Swap the display buffers
            MGL_swapBuffers(dc,MGL_waitVRT);

            // Bounce the clock off the walls
            dirtyRect := extent;
            MGL_insetRect(dirtyRect,-Abs(stepx),-Abs(stepy));

            if (extent.left + stepx < 0) then stepx := -stepx;
            if (extent.right + stepx > maxx) then stepx := -stepx;

            if (extent.top + stepy < 0) then stepy := -stepy;
            if (extent.bottom + stepy > maxy) then stepy := -stepy;

            MGL_offsetRect(extent,stepx,stepy);

            // Update the hand movement 

            secAngle := fixAngle(secAngle - 5);
            if (secAngle = 90) then minAngle := fixAngle(minAngle - 5);
        end;

        MGL_singleBuffer(dc);
        defaultAttributes(dc);
    end
    else
    begin
        MainWindow(dc,'Page flip animation demo');

        maxx := MGL_maxx;
        maxy := MGL_maxy;

        MGL_setTextJustify(MGL_CENTER_TEXT,MGL_CENTER_TEXT);
        MGL_moveToCoord(maxx div 2,maxy div 2);
        MGL_drawStr('Video mode does not support page flipping');
    end;
    Result := Pause;
end;

{-- MouseCursorDemo ----------------------------------------------------------}

// Display the mouse cursor on the screen, and change it to
// a number of different styles.

function    MouseCursorDemo(dc: PMGLDC): Boolean;
var
    cursor: pcursor_t;
begin
    // First check to ensure that a mouse is actually installed in the
    // system. MS_init() will return 0 if no mouse is installed, or the
    // number of buttons if it is.
    if (MS_available) then
    begin
        MS_show;

        MainWindow(dc,'Mouse Cursor Demonstration');
        StatusLine('Press any key for Hour Glass cursor');
        waitEvent;

        cursor := MGL_loadCursor('wait.cur');
        if cursor = nil then
            raise Exception.Create('Unable to load WAIT.CUR cursor file');

        MS_setCursor(cursor);
        statusLine('Press any key for IBEAM cursor');
        waitEvent;

        MS_setCursor(MGL_DEF_CURSOR);
        MGL_unloadCursor(cursor);

        cursor := MGL_loadCursor('ibeam.cur');
        if cursor = nil then
            raise Exception.Create('Unable to load IBEAM.CUR cursor file');
        MS_setCursor(cursor);
        waitEvent;

        MS_setCursor(MGL_DEF_CURSOR);
        MGL_unloadCursor(cursor);

        if not Pause then
        begin
            Result := False;
            Exit;
        end;
        MS_hide;
    end;
    Result := True;
end;

{-- BoundaryFillDemo ---------------------------------------------------------}

{$IFDEF FLOODFILL_DEMO}
{$MINSTACKSIZE $00100000}
// Demonstrate the boundary flood fill routine.
function    BoundaryFillDemo(dc: PMGLDC): Boolean;
var
    i,j,maxx,maxy   : int;
    color           : color_t;
    poly            : array[0..5] of point_t;
begin
    MainWindow(dc,'MGL_boundaryFill Demonstration');

    maxx := MGL_maxx;
    maxy := MGL_maxy;

    MGL_srand(200);

    // Draw a set of polygons
    for j := 0 to 2 do
    begin
        MGL_random(10); // Keep random in sync
        if j = 0 then
            color := MGL_BLUE
        else if j = 1 then
            color := MGL_RED
        else
            color := MGL_YELLOW;

        for i := 0 to 5 do
        begin
            poly[i].x := MGL_random(maxx);
            poly[i].y := MGL_random(maxy);
        end;

        MGL_setColorCI(color);
        MGL_fillPolygon(6,@poly[0],0,0);
    end;

    MGL_setColor(MGL_defaultColor);
    StatusLine('Press a key for boundary fill');
    WaitEvent;

    MGL_setColorCI(MGL_GREEN);
    MGL_boundaryFill(10,10,MGL_realColor(dc,MGL_RED));

    MGL_setColor(MGL_defaultColor);
    Result := Pause;
end;

{-- InteriorFillDemo ---------------------------------------------------------}

// Demonstrate the interior flood fill routine.
function    InteriorFillDemo(dc: PMGLDC): Boolean;
var
    i,j,maxx,maxy   : int;
    color           : color_t;
    poly            : array[0..5] of point_t;
begin
    MainWindow(dc,'MGL_interiorFill Demonstration');

    maxx := MGL_maxx;
    maxy := MGL_maxy;

    MGL_srand(200);

    // Draw a set of polygons
    for j := 0 to 2 do
    begin
        MGL_random(10); // Keep random in sync
        if j = 0 then
            color := MGL_BLUE
        else if j = 1 then
            color := MGL_RED
        else
            color := MGL_YELLOW;

        for i := 0 to 5 do
        begin
            poly[i].x := MGL_random(maxx);
            poly[i].y := MGL_random(maxy);
        end;

        MGL_setColorCI(color);
        MGL_fillPolygon(6,@poly[0],0,0);
    end;

    MGL_setColor(MGL_defaultColor);
    StatusLine('Press a key for interior fill');
    WaitEvent;

    MGL_setColorCI(MGL_GREEN);
    MGL_interiorFill(10,10);

    MGL_setColor(MGL_defaultColor);
    Result := Pause;
end;
{$ENDIF}

{-- MarkerDemo ---------------------------------------------------------------}

const
    NUM_POINTS  = 10;

    data1: array[0..NUM_POINTS-1] of int = (1, 3, 5, 4, 3, 2, 1, 5, 4, 2);
    data2: array[0..NUM_POINTS-1] of int = (4, 6, 10, 2, 6, 4, 8, 10, 6, 2);
    data3: array[0..NUM_POINTS-1] of int = (1, 3, 2, 5, 7, 9, 5, 4, 5, 8);

// Draws a simple graph using markers to show to points on
// the graph.

function    MarkerDemo(dc: PMGLDC): Boolean;
var
    i,stepx,stepy,maxy,maxx: int;
    p: array[0..NUM_POINTS-1] of point_t;
begin
    MainWindow(dc,'Marker Demonstration');

    maxx    := MGL_maxx;
    maxy    := MGL_maxy;
    stepx   := maxx div 12;
    stepy   := maxy div 12;

    // Draw the graph axes
    MGL_lineCoord(stepx,maxy - stepy,stepx*11,maxy-stepy);
    MGL_lineCoord(stepx,maxy - stepy,stepx,maxy - stepy*11);

    i := stepx*11;
    while i >= stepx do
    begin
        MGL_lineCoord(i,maxy - stepy - 3,i,maxy - stepy + 3);
        Dec(i,stepx);
    end;

    i := maxy - stepy;
    while i >= maxy-stepy*11 do
    begin
        MGL_lineCoord(stepx-3,i,stepx+3,i);
        Dec(i,stepy);
    end;

    // Draw the first set of data
    for i := 0 to NUM_POINTS - 1 do
    begin
        p[i].x := stepx + i*stepx;
        p[i].y := maxy - stepy - data1[i]*stepy;
    end;

    MGL_setMarkerSize(4);
    MGL_setColorCI(MGL_LIGHTRED);
    MGL_polyLine(NUM_POINTS,@p[0]);
    MGL_setMarkerColor(MGL_realColor(dc,MGL_MAGENTA));
    MGL_polyMarker(NUM_POINTS,@p[0]);

    // Draw the second set of data
    for i := 0 to NUM_POINTS - 1 do
    begin
        p[i].x := stepx + i*stepx;
        p[i].y := maxy - stepy - data2[i]*stepy;
    end;

    MGL_setColorCI(MGL_LIGHTGREEN);
    MGL_polyLine(NUM_POINTS,@p[0]);
    MGL_setMarkerColor(MGL_realColor(dc,MGL_MAGENTA));
    MGL_setMarkerStyle(MGL_MARKER_CIRCLE);
    MGL_polyMarker(NUM_POINTS,@p[0]);

    // Draw the third set of data

    for i := 0 to NUM_POINTS - 1 do
    begin
        p[i].x := stepx + i*stepx;
        p[i].y := maxy - stepy - data3[i]*stepy;
    end;

    MGL_setColorCI(MGL_LIGHTBLUE);
    MGL_polyLine(NUM_POINTS,@p[0]);
    MGL_setMarkerColor(MGL_realColor(dc,MGL_MAGENTA));
    MGL_setMarkerStyle(MGL_MARKER_X);
    MGL_polyMarker(NUM_POINTS,@p[0]);

    defaultAttributes(dc);
    Result := Pause;
end;

{-- Finale -------------------------------------------------------------------}

// Display a goodbye message before signing off.

procedure   Finale(dc: PMGLDC);
begin
    MainWindow(dc,'Finale');
    MGL_setTextJustify(MGL_CENTER_TEXT,MGL_CENTER_TEXT);

    MGL_useFont(largeFont);
    MGL_drawStrXY(MGL_maxx div 2,MGL_maxy div 2,'Thats all folks!');
    statusLine('Press any key to exit...');
    Pause;
end;

{-- Demo ---------------------------------------------------------------------}

procedure   Demo(DC: PMGLDC);
begin
    if not ReportStatus(dc) then Exit;
    if not ColorDemo(dc) then Exit;
    if not PaletteDemo(dc) then Exit;
    if not PixelDemo(dc) then Exit;
    if not RectangleDemo(dc) then Exit;
    if not CircleDemo(dc) then Exit;
    if not FilledCircleDemo(dc) then Exit;
    if not ArcDemo(dc) then Exit;
    if not FilledArcDemo(dc) then Exit;
    if not PolyDemo(dc) then Exit;
    if not FastPolyDemo(dc) then Exit;
    if not GouraudPolyDemo(dc) then Exit;
    if not PieDemo(dc) then Exit;
    if not LineDemo(dc) then Exit;
    if not LineRelDemo(dc) then Exit;
    if not LineToDemo(dc) then Exit;
    if not PatternDemo(dc) then Exit;
    if not MarkerDemo(dc) then Exit;
    if not PageFlipDemo(dc) then Exit;
{$IFDEF FLOODFILL_DEMO}
    if not BoundaryFillDemo(dc) then Exit;
    if not InteriorFillDemo(dc) then Exit;
{$ENDIF}
    if not MouseCursorDemo(dc) then Exit;
    if not TextDump(dc) then Exit;
    if not TextStyleDemo(dc) then Exit;
    Finale(dc);
end;

initialization
    RegisterDemo('MGLDemo',
                 'Demo program for the MegaGraph graphics library. This '+
                 'program is very similar to the graphics demo program '+
                 'that comes with Borland''s range of compiler, but has '+
                 'been extended to fully test the MGL library.',
                 Demo);
end.
