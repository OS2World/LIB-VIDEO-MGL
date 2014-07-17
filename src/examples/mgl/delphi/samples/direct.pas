// Sample program showing how to render directly to a device
// context surface. There are two methods shown here:
//
// 1.  We create any MGL device context and use the
//     mode_t information block to directly draw on
//     the device surface.
//
// 2.  We create our own system memory device context
//     as a block of memory for the surface and fill
//     in a bitmap_t header. We can then draw directly
//     on the bitmap surface and the blt it to the
//     display.
unit Direct;

interface

implementation

uses
    SysUtils,
    MGLSamp,
    MGLTypes,
    MGLFx;

{-- Global variables ---------------------------------------------------------}
var
    maxx,maxy       : int;      // Maximum coordinates for direct surface
    bytesPerLine    : long;     // Scanline width for surface
    surface         : PChar;    // Pointer to the surface to draw to

{-- putPixel -----------------------------------------------------------------}

// Plots a pixel to an 8 bit device surface.
procedure   putPixel(x, y, color: int);
var
    Offset: long;
begin
    Offset := y * bytesPerLine + x;
    Surface[Offset] := Char(color);
end;

{-- line ---------------------------------------------------------------------}

// Scan convert a line segment using the MidPoint Digital
// Differential Analyser algorithm.
procedure   line(x1, y1, x2, y2, color: int);
var
    d               : int ; // Decision variable
    dx,dy           : int ; // Dx and Dy values for the line
    Eincr,NEincr    : int ; // Decision variable increments
    yincr           : int ; // Increment for y values
    t               : int ; // Counters etc.
begin
    dx := Abs(x2 - x1);
    dy := Abs(y2 - y1);

    if (dy <= dx) then
    begin

        // We have a line with a slope between -1 and 1
        //
        // Ensure that we are always scan converting the line from left to
        // right to ensure that we produce the same line from P1 to P0 as the
        // line from P0 to P1.

        if (x2 < x1) then
        begin
            t := x2; x2 := x1; x1 := t;    // Swap X coordinates
            t := y2; y2 := y1; y1 := t;    // Swap Y coordinates
        end;

        if (y2 > y1) then
            yincr := 1
        else
            yincr := -1;

        d       := 2*dy - dx;       // Initial decision variable value  
        Eincr   := 2*dy;            // Increment to move to E pixel
        NEincr  := 2*(dy - dx);     // Increment to move to NE pixel

        putPixel(x1,y1,color);      // Draw the first point at (x1,y1)  

        // Incrementally determine the positions of the remaining pixels
        Inc(x1);
        while x1 <= x2 do
        begin
            if (d < 0) then
                Inc(d,Eincr)        // Choose the Eastern Pixel
            else
            begin
                Inc(d,NEincr);      // Choose the North Eastern Pixel
                Inc(y1,yincr);      // (or SE pixel for dx/dy < 0!)     
            end;
            putPixel(x1,y1,color);      // Draw the point
            Inc(x1);
        end;
    end
    else
    begin

        // We have a line with a slope between -1 and 1 (ie: includes
        // vertical lines). We must swap our x and y coordinates for this.
        //
        // Ensure that we are always scan converting the line from left to
        // right to ensure that we produce the same line from P1 to P0 as the
        // line from P0 to P1.

        if (y2 < y1) then
        begin
            t := x2; x2 := x1; x1 := t;    // Swap X coordinates
            t := y2; y2 := y1; y1 := t;    // Swap Y coordinates
        end;

        if (x2 > x1) then
            yincr := 1
        else
            yincr := -1;

        d := 2*dx - dy;             // Initial decision variable value
        Eincr := 2*dx;              // Increment to move to E pixel
        NEincr := 2*(dx - dy);      // Increment to move to NE pixel

        putPixel(x1,y1,color);      // Draw the first point at (x1,y1)

        // Incrementally determine the positions of the remaining pixels
        Inc(y1);
        while y1 <= y2 do
        begin
            if (d < 0) then
                Inc(d,Eincr)        // Choose the Eastern Pixel
            else
            begin
                Inc(d, NEincr);     // Choose the North Eastern Pixel
                Inc(x1, yincr);     // (or SE pixel for dx/dy < 0!)
            end;
            putPixel(x1,y1,color);  // Draw the point
            Inc(y1);
        end;
    end;
end;

{-- Moire --------------------------------------------------------------------}

// Draws a simple moire pattern on the current surface.
procedure   moire;
var
    i: int;
begin
    i := 0;
    while i < maxx do
    begin
        line(maxx div 2,maxy div 2,i,0,i mod 255);
        line(maxx div 2,maxy div 2,i,maxy,(i+1) mod 255);
        Inc(i,4);
    end;

    i := 0;
    while i < maxy do
    begin
        line(maxx div 2,maxy div 2,0,i,(i+2) mod 255);
        line(maxx div 2,maxy div 2,maxx,i,(i+3) mod 255);
        Inc(i,4);
    end;

    line(0,0,maxx,0,15);
    line(0,0,0,maxy,15);
    line(maxx,0,maxx,maxy,15);
    line(0,maxy,maxx,maxy,15);
end;

{-- Demo proc ----------------------------------------------------------------}

procedure   Demo(DC: PMGLDC);
var
    MemDC           : PMGLDC;
    pf              : pixel_format_t;
    pal             : array[0..255] of palette_t;
{$IFNDEF MGLLITE}
    bmpHdr          : bitmap_t;
    bmpSize         : Integer;
{$ENDIF}
begin
    // Start the MGL with only the 8 bit linear buffer drivers active
    if MGL_getBitsPerPixel(dc) <> 8 then
        raise Exception.Create('This demo requires 8 bit driver');

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
    if MGL_surfaceAccessType(dc) = MGL_NO_ACCESS then
        raise Exception.Create('This demo requires a linear access surface');
    maxx            := MGL_maxx;
    maxy            := MGL_maxy;
    bytesPerLine    := dc^.mi.bytesPerLine;
    surface         := dc^.surface;

    // If we are drawing to a hardware accelerated surface, we must call
    // MGL_beginDirectAccess() and MGL_endDirectAccess() before and after
    // doing any direct surface access
    if (MGL_getHardwareFlags(dc) <> MGL_HW_NONE) then
        MGL_beginDirectAccess;

    try
        // Now draw directly to display surface
        moire;

    finally
        if (MGL_getHardwareFlags(dc) <> MGL_HW_NONE) then
            MGL_endDirectAccess;
    end;

    // Wait for a keypress
    Pause;

{$IFNDEF MGLLITE}
    // Now we allocate a system memory block for a 100x100 memory
    // surface manually and attach it to our bitmap header. Then we
    // will draw directly to it and then display it on our main device
    // context. Note that we allocate it without a palette and with
    // a pixel format info block.
    bmpHdr.width        := 320;
    bmpHdr.height       := 240;
    bmpHdr.bitsPerPixel := 8;
    bmpHdr.bytesPerLine := bmpHdr.width;
    bmpHdr.pal          := nil;
    bmpHdr.pf           := nil;
    bmpSize             := bmpHdr.bytesPerLine * bmpHdr.height;
    GetMem(bmpHdr.surface,bmpSize);
    try
        maxx                := bmpHdr.width-1;
        maxy                := bmpHdr.height-1;
        bytesPerLine        := bmpHdr.bytesPerLine;
        surface             := PChar(bmpHdr.surface);
        FillChar(surface^,bytesPerLine * (maxy+1),0);

        // Now draw directly to bitmap surface and display it
        moire;
        MGL_putBitmap(dc,160,120,@bmpHdr,MGL_REPLACE_MODE);

        // Wait for a keypress
        Pause;
    finally
        FreeMem(bmpHdr.surface,bmpSize);
    end;
{$ENDIF}

    // Now we allocate a system memory device context and draw directly
    // onto this surface, then blit it to the display. It is similar to
    // the above code but does not use a lightweight bitmap as the drawing
    // surface but a full memory device context (which you can also render
    // to using the MGL functions).
    memdc := MGL_createMemoryDC(160,120,8,@pf);
    if (memDC = nil) then
        MGLError;
    try
        maxx            := memdc^.mi.xRes;
        maxy            := memdc^.mi.yRes;
        bytesPerLine    := memdc^.mi.bytesPerLine;
        surface         := memdc^.surface;
        FillChar(surface^,bytesPerLine * (maxy+1),0);

        // Now set the palette for the memory device to the same as the display
        // DC so that we avoid palette remapping (you could use a different
        // palette and it would be automatically mapped to the screen when you
        // do a blit, but this would be slower).
        MGL_getPalette(dc,@pal[0],256,0);
        MGL_setPalette(memdc,@pal[0],256,0);
        MGL_realizePalette(memdc,256,0,0);

        // Now draw directly to bitmap surface and display it
        moire;
        MGL_bitBltCoord(dc,memdc,0,0,160,120,240,180,MGL_REPLACE_MODE);

        // Wait for a keypress
        Pause;
    finally
        MGL_destroyDC(memDC);
    end;
end;

initialization
    RegisterDemo('Direct',
                 'Sample program showing how to render directly to a device '+
                 'context surface. Requires 8-bit devices and linear access',
                 Demo);
end.
