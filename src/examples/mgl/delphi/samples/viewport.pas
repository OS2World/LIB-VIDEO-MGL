// Simple demo showing how to use the viewport functions to
// move the output of a simple drawing to a different
// location on the screen.
unit ViewPort;

interface

implementation

uses
    MGLFx,
    MGLSamp;

{-- Constants ----------------------------------------------------------------}

const
    IMAGE_NAME  = 'data\frog.bmp';

{-- Demo ---------------------------------------------------------------------}

procedure   Demo(dc: PMGLDC);
var
    bitmap  : pbitmap_t;
    rect    : rect_t;
begin
    // Display title message at the top of the window.
    MainWindow(dc,'Viewport Demo');

    // Load a bitmap and the palette from the image file.
    bitmap := MGL_loadBitmap(IMAGE_NAME,TRUE);
    if bitmap = nil then
        MGLError;

    if MGL_getBitsPerPixel(dc) <= 8 then
    begin
        // Set the display device context with the palette from the
        // memory device context.
        MGL_setPalette(dc,bitmap^.pal,MGL_getPaletteSize(dc),0);
        MGL_realizePalette(dc,MGL_getPaletteSize(dc),0,1);
    end;

    // Put the image onto the display device context.
    MGL_putBitmapTransparent(dc,0,0,bitmap,0,true);

    // change the dimensions of the current active viewport
    with rect do
    begin
        left    := bitmap^.width div 3;
        top     := bitmap^.height div 3;
        right   := rect.left + bitmap^.width;
        bottom  := rect.top + bitmap^.height;
    end;
    MGL_setViewportDC(dc,rect);

    // Put the image onto the display device context.
    MGL_putBitmapTransparent(dc,0,0,bitmap,0,true);

    // Pause until the user hits a key or presses a mouse button.
    Pause;
end;

initialization
    RegisterDemo('ViewPort',
                 'Simple demo showing how to use the viewport functions to '+
                 'move the output of a simple drawing to a different '+
                 'location on the screen.',
                 Demo);
end.
