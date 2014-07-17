// Simple demo showing how to load a bitmap file from disk and
// display it on the screen.
unit Bitmap;

interface

implementation

uses
    MGLSamp,
    MGLTypes,
    MGLFx;

{-- Constants ----------------------------------------------------------------}
const
    IMAGE_NAME = 'data\frog.bmp';

{-- Demo proc ----------------------------------------------------------------}
procedure   Demo(dc: PMGLDC);
var
    bitmap      : pbitmap_t;
    posx,posy   : int;
begin
    // Display title message at the top of the window.
    MainWindow(dc,'Bitmap Demo');

    // Load a bitmap and the palette from the image file.
    bitmap := MGL_loadBitmap(IMAGE_NAME,TRUE);
    if (bitmap = nil) then
        MGLError;

    try
        // Set the display device context with the palette from the
        // memory device context.
        MGL_setPalette(dc,bitmap^.pal,MGL_getPaletteSize(dc),0);
        MGL_realizePalette(dc,MGL_getPaletteSize(dc),0,1);

        // Justify the image that will be put on the screen
        posx := (MGL_sizex(dc)-bitmap^.width) div 2;
        posy := (MGL_sizey(dc)-bitmap^.height) div 2;

        // Put the image onto the display device context.
        MGL_putBitmap(dc,posx,posy,bitmap,MGL_REPLACE_MODE);
    finally
        // Remove all bitmaps from memory.
        MGL_unloadBitmap(bitmap);
    end;

    // Pause until the user hits a key or presses a mouse button.
    Pause;
end;

initialization
    RegisterDemo('Bitmap demo',
                 'Simple demo showing how to load a bitmap file from disk and '+
                 'display it on the screen.',
                 Demo);
end.
