// Flood fill region fill routines. This code shows how to
// build upon the low level MegaGraph library routines to
// build fast line adjacency flood fill routines. Two distinct
// routines are implemented here - a boundary flood fill,
// and an interior flood fill. Note that since the low
// level scanline routines do not do any clipping or view
// port mapping, we must do that within the routines.
// To mimise the amount of stack space that is used, only the
// variable that absolutely MUST be local are stored as local
// or passed as parameters, while the rest are stored globally
// to the recursive routines.
//
// NOTE:These routines use a lot of stack space since they
//      are recursive. If you wish to use these in real
//      programs, you should use a large stack of at least 20k.
unit FFill;

interface

uses
    MGLTypes,
    MGLFx;

{-- Prototypes of filling routines defined in this unit ----------------------}
procedure   MGL_interiorFill(x: int; y: int);
procedure   MGL_boundaryFill(x: int; y: int; bdr: color_t);

implementation

uses
    SysUtils;

{-- Constants ----------------------------------------------------------------}
const
    UP      = -1;
    DOWN    = 1;

{-- Variables ----------------------------------------------------------------}
var
    border  : color_t;  // Color of border pixels
    fill    : color_t;  // Color of current fill color
    interior: color_t;  // Color of interior pixels
    back    : color_t;  // Background color
    viewPort: rect_t;   // The current viewport
    clipRect: rect_t;   // The current clipping rectangle
    v       : color_t;  // Global variable

{-- BorderAdjFill ------------------------------------------------------------}

// Recursively border fills an area starting at the seed
// point (seedX,seedY) and travelling in the direction D.
//
// A border fill is terminated when it comes across a border
// pixel of the specified border color.

function    BorderAdjFill(x, y, D, prevXL, prevXR: int): int;
var
    xl, xr: int;
begin
    // Initialise to seed coordinates
    xl := x;
    xr := x;

    // Scan left in video buffer from the current seed coordinate looking
    // for border pixels.

    Dec(xl);
    if xl >= clipRect.left then
    begin
        xl := MGL_scanLeftForColor(xl+viewPort.left,y+viewPort.top,border);
        Dec(xl,viewPort.left);
        if xl < clipRect.left then
            xl := clipRect.left-1;
        Inc(xl);
    end
    else
        xl := clipRect.left;

    // Scan right in video buffer from the current seed coordinate looking
    // for border pixels.

    Inc(xr);
    if xr < clipRect.right then
    begin
        xr := MGL_scanRightForColor(xr+viewPort.left,y+viewPort.top,border);
        Dec(xr,viewPort.left);
        if xr >= clipRect.right then
            xr := clipRect.right;
        Dec(xr);
    end
    else
        xr := clipRect.right-1;

    // Fill in the current scan line
    MGL_scanLine(y+viewPort.top,xl+viewPort.left,xr+viewPort.left+1);

    // Find and fill adjacent line segments in same direction
    if (y+D >= clipRect.top) and (y+D < clipRect.bottom) then
    begin
        x := xl ;
        while x <= xr do
        begin
            v := MGL_getPixelCoord(x,y+D);
            if (v <> border) and (v <> fill) and (v <> back) then
                x := BorderAdjFill(x,y+D,D,xl,xr);
            Inc(x); 
        end;
    end;

    // Find and fill adjacent line segments in opposite direction
    if (y-D >= clipRect.top) and (y-D < clipRect.bottom) then
    begin
        Dec(prevXL,2);
        x := xl;
        while x <= prevXL - 1 do
        begin
            v := MGL_getPixelCoord(x,y-D);
            if (v <> border) and (v <> fill) and (v <> back) then
                x := BorderAdjFill(x,y-D,-D,xl,xr);
            Inc(x);
        end;

        x := prevXR+2;
        while x <= xr - 1 do
        begin
            v := MGL_getPixelCoord(x,y-D);
            if (v <> border) and (v <> fill) and (v <> back) then
                x := BorderAdjFill(x,y-D,-D,xl,xr);
            Inc(x);
        end;
    end;

    Result := xr;
end;

{-- MGL_boundaryFill ---------------------------------------------------------}

// Does a boundary fill starting at the point (x,y) and
// filling all the area bounded by pixels of the color bdr.
//
// The area is filled in with the current drawing color and
// the current pattern. Note however that you can only use
// MGL_BITMAP_OPAQUE pattern's, or solid fills.

procedure   MGL_boundaryFill(x, y: int; bdr: color_t);
var
    penstyle    : int;
    writemode   : int;
begin
    penstyle    := MGL_getPenStyle();
    writemode   := MGL_getWriteMode();

    if (penstyle = MGL_BITMAP_TRANSPARENT) or (writemode <> MGL_REPLACE_MODE) then
        Exit;

    back := -1;

    if (penstyle = MGL_BITMAP_OPAQUE) then
        back := MGL_getBackColor;

    border  := bdr;
    fill    := MGL_getColor();

    // Set up viewport and clipping rectangles 

    MGL_getViewport(@viewPort);
    MGL_getClipRect(@clipRect);

    if (MGL_ptInRectCoord(x,y,clipRect)) then
        BorderAdjFill(x,y,DOWN,x,x);
end;

{-- InteriorAdjFill ----------------------------------------------------------}

// Recursively fills an area starting at the seed point
// (seedX,seedY) and travelling in the direction D.
//
// An interior fill is terminated when it comes across a
// pixel of different to that of the original seed pixel.

function    InteriorAdjFill(x, y, D, prevXL, prevXR: int): int;
var
    xl,xr: int;
begin
    // Initialise to seed coordinates
    xl := x ;
    xr := x;

    // Scan left in video buffer from the current seed coordinate looking
    // for border pixels.
    Dec(xl);
    if xl >= clipRect.left then
    begin
        xl := MGL_scanLeftWhileColor(xl+viewPort.left,y+viewPort.top,interior);
        Dec(xl,viewPort.left);
        if (xl < clipRect.left) then
            xl := clipRect.left-1;
        Inc(xl);
    end
    else
        xl := clipRect.left;

    // Scan right in video buffer from the current seed coordinate looking
    // for border pixels.
    Inc(xr);
    if xr < clipRect.right then
    begin
        xr := MGL_scanRightWhileColor(xr+viewPort.left,y+viewPort.top,interior);
        Dec(xr,viewPort.left);
        if xr >= clipRect.right then
            xr := clipRect.right;
        Dec(xr);
    end
    else
        xr := clipRect.right-1;

    // Fill in the current scan line 
    MGL_scanLine(y+viewPort.top,xl+viewPort.left,xr+viewPort.left+1);

    // Find and fill adjacent line segments in same direction 
    if (y+D >= clipRect.top) and (y+D < clipRect.bottom) then
    begin
        x := xl;
        while x <= xr do
        begin
            v := MGL_getPixelCoord(x,y+D);
            if (v = interior) and (v <> fill) and (v <> back) then
                x := InteriorAdjFill(x,y+D,D,xl,xr);
            Inc(x);
        end;
    end;

    // Find and fill adjacent line segments in opposite direction 

    if (y-D >= clipRect.top) and (y-D < clipRect.bottom) then
    begin
        Dec(prevXL,2);
        x := xl;
        while x <= prevXL - 1 do
        begin
            v := MGL_getPixelCoord(x,y-D);
            if (v = interior) and (v <> fill) and (v <> back) then
                x := InteriorAdjFill(x,y-D,-D,xl,xr);
            Inc(x);
        end;

        x := prevXR+2;
        while x <= xr - 1 do
        begin
            v := MGL_getPixelCoord(x,y-D);
            if (v = interior) and (v <> fill) and (v <> back) then
                x := InteriorAdjFill(x,y-D,-D,xl,xr);
            Inc(x);
        end;
    end;
    Result := xr;
end;

{-- MGL_interiorFill ---------------------------------------------------------}

// Does a flood fill starting at the point (x,y) and
// filling in all accessible areas of the same color as the
// seed point.
//
// The area is filled in with the current drawing color and
// the current pattern. Note however that you can only use
// MGL_BITMAP_OPAQUE pattern's, or solid fills in replace
// mode.

procedure   MGL_interiorFill(x, y: int);
var
    penstyle    :int;
    writemode   :int;
begin
    penstyle    := MGL_getPenStyle();
    writemode   := MGL_getWriteMode();

    if (penstyle = MGL_BITMAP_TRANSPARENT) or (writemode <> MGL_REPLACE_MODE) then
        Exit;

    back := -1;

    if (penstyle = MGL_BITMAP_OPAQUE) then
        back := MGL_getBackColor;

    interior := MGL_getPixelCoord(x,y);
    if interior = -1 then
        Exit;

    fill := MGL_getColor;

    // Set up viewport and clipping rectangles
    MGL_getViewport(@viewPort);
    MGL_getClipRect(@clipRect);

    InteriorAdjFill(x,y,DOWN,x,x);
end;

end.
