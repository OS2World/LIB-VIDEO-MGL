// Simple sample program showing how to use the bltBlt
// functions to blt an image from a memory dc to the screen,
// and between areas on the same screen.
unit BitBlt;

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
    width,height,depth  : int;
    pal                 : array[0..255] of palette_t;
    rect                : rect_t;
    pf                  : pixel_format_t;
    memdc               : PMGLDC;
begin
    MainWindow(dc,'Bit Blit Demo');

    // Get image information
    MGL_getBitmapSize(IMAGE_NAME,@width,@height,@depth,@pf);

    // Create a memory device context
    memdc := MGL_createMemoryDC(width,height,depth,@pf);
    if memDC = nil then
        MGLError;

    try
        // Load a bitmap into the memory device context. The palette gets
        // loaded into the memory device context as well.
        if not MGL_loadBitmapIntoDC(memdc,IMAGE_NAME,0,0,True) then
            MGLError;

        // Set the display device context with the palette from the
        // memory device context.
        if (depth <= 8) then
        begin
            MGL_getPalette(memdc,@pal[0],MGL_getPaletteSize(memdc),0);
            MGL_setPalette(dc,@pal[0],MGL_getPaletteSize(memdc),0);
            MGL_realizePalette(dc,MGL_getPaletteSize(memdc),0,1);
        end;

        rect.left   := 0;
        rect.top    := 0;
        rect.right  := width;
        rect.bottom := height;

        // Copy image from image in memory to the screen.
        MGL_bitBlt(dc,memdc,rect,0,0,MGL_REPLACE_MODE);

        // Copy image from screen to screen with an overlapping area.
        MGL_bitBlt(dc,dc,rect,width div 3,height div 3,MGL_REPLACE_MODE);
    finally
        // Remove all device contexts from memory.
        MGL_destroyDC(memdc);
    end;

    // Pause until the user hits a key or presses a mouse button.
    Pause;
end;

initialization
    RegisterDemo('Bit Blit Demo',
                 'Simple sample program showing how to use the bltBlt '+
                 'functions to blt an image from a memory dc to the screen '+
                 'and between areas on the same screen.',
                 Demo);
end.
