// Simple demo showing how to load a pcx file from disk and
// display it on the screen.
unit pcx;

interface

implementation

uses
    MGLFx,
    MGLTypes,
    MGLSamp;

{-- Image name ---------------------------------------------------------------}

const
    IMAGE_NAME = 'data\doggie2.pcx';

{-- Demo ---------------------------------------------------------------------}

procedure   Demo(dc: PMGLDC);
var
    posx,posy : int;
    bitmap    : pbitmap_t;
begin
    // Display title message at the top of the window.
    MainWindow(dc,'PCX Demo');

    // Load a pcx and the palette from the image file.
    bitmap := MGL_loadPCX(IMAGE_NAME,TRUE);
    if bitmap = nil then
        MGLError;

    // Set the display device context with the palette from the
    // memory device context.
    MGL_setPalette(dc,bitmap^.pal,MGL_getPaletteSize(dc),0);
    MGL_realizePalette(dc,MGL_getPaletteSize(dc),0,1);

    // Justify the image that will be put on the screen
    posx := (MGL_sizex(dc)-bitmap^.width) div 2;
    posy := (MGL_sizey(dc)-bitmap^.height) div 2;

    // Put the image onto the display device context.
    MGL_putBitmap(dc,posx,posy,bitmap,MGL_REPLACE_MODE);

    // Remove all bitmaps from memory.
    MGL_unloadBitmap(bitmap);

    // Pause until the user hits a key or presses a mouse button.
    Pause;
end;

initialization
    RegisterDemo('PCX',
                 'Simple demo showing how to load a pcx file from disk and '+
                 'display it on the screen.',
                 Demo);
end.
