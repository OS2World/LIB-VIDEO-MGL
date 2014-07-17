// Demo showing the use of the stretchBlt functions to stretch
// a bitmap from memory dc to the screen and stretchBitmap to
// stretch a custom bitmap image from memory to the screen.
unit Stretch;

interface

implementation

uses
    MGLFx,
    MGLTypes,
    MGLSamp;

{-- Constants ----------------------------------------------------------------}

const
    IMAGE_NAME = 'data\frog.bmp';

{-- Variables ----------------------------------------------------------------}

var
    maxx,maxy: int;

{-- BitmapStretch ------------------------------------------------------------}

// Demo showing how to use stretchBitmap to stretch a custom
// bitmap image from memory to the screen.

function    BitmapStretch(dc: PMGLDC): Boolean;
var
    x, y    : int;
    bitmap  : pbitmap_t;
begin
    x := maxx div 2;
    y := maxy div 2;
    
    mainWindow(dc,'Bitmap Stretch Demo');
    statusLine('Press any key to continue, ESC to Abort');

    // Load a bitmap and the palette from the image file.
    bitmap := MGL_loadBitmap(IMAGE_NAME,TRUE);
    if bitmap = nil then
        MGLError;

    // Set the display device context with the palette from the bitmap
    MGL_setPalette(dc,bitmap^.pal,MGL_getPaletteSize(dc),0);
    MGL_realizePalette(dc,MGL_getPaletteSize(dc),0,1);

    while (x<maxx) and (y<maxy) and not checkEvent do
    begin
        MGL_stretchBitmap(dc,maxx-x,maxy-y,x,y,bitmap);
        Inc(x);
        Inc(y);
    end;

    // Remove all bitmaps from memory.
    MGL_unloadBitmap(bitmap);

    Result := Pause;
end;

{-- MemoryDCStretch ----------------------------------------------------------}

// Demo showing the use of the stretchBlt functions to stretch
// a bitmap from memory dc to the screen.

function    MemoryDCStretch(dc: PMGLDC): Boolean;
var
    memdc               : PMGLDC;
    width,height,depth  : int;
    pf                  : pixel_format_t;
    pal                 : array[0..255] of palette_t;
    x, y                : int;
begin
    x := maxx div 2;
    y := maxy div 2;

    MainWindow(dc,'Memory DC Stretch Demo');
    StatusLine('Press any key to continue, ESC to Abort');

    // Get image information
    MGL_getBitmapSize(IMAGE_NAME,@width,@height,@depth,@pf);

    // Create a memory device context.
    memdc := MGL_createMemoryDC(width,height,depth,@pf);
    if memdc = nil then
        MGLError;

    // Load a bitmap into the memory device context. The palette gets
    // loaded into the memory device context as well.
    if not MGL_loadBitmapIntoDC(memdc,IMAGE_NAME,0,0,TRUE) then
        MGLError;

    // Set the display device context with the palette from the
    // memory device context.
    MGL_getPalette(memdc,@pal[0],MGL_getPaletteSize(memdc),0);
    MGL_setPalette(dc,@pal[0],MGL_getPaletteSize(memdc),0);
    MGL_realizePalette(dc,MGL_getPaletteSize(memdc),0,1);

    // Put the image onto the display device context.
    while (x<maxx) and (y<maxy) and not checkEvent do
    begin
        MGL_stretchBltCoord(dc,memdc,0,0,width,height,maxx-x,maxy-y,x,y);
        Inc(x);
        Inc(y);
    end;

    // Remove all device contexts from memory.
    MGL_destroyDC(memdc);
    Result := Pause;
end;

{-- Demo ---------------------------------------------------------------------}

procedure   Demo(dc: PMGLDC);
begin
    maxx := MGL_maxx;
    maxy := MGL_maxy;

    if not BitmapStretch(dc) then
        Exit;
    if not MemoryDCStretch(dc) then
        Exit;
end;

initialization
    RegisterDemo('Stretch',
                 'Demo showing the use of the stretchBlt functions to stretch '+
                 'a bitmap from memory dc to the screen and stretchBitmap to '+
                 'stretch a custom bitmap image from memory to the screen.',
                 Demo);
end.
