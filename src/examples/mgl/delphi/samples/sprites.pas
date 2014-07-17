// Demo showing how to use offscreen dc's to store sprites and
// to use harware acceleration to blt them from the offscreen
// dc to the display dc. Also shows falling back to a memory
// dc if an offscreen dc is not available.
unit Sprites;

interface

implementation

uses
    MGLFx,
    MGLTypes,
    MGLSamp;

{-- Constants ----------------------------------------------------------------}

const
    IMAGE_NAME = 'data\doggie2.bmp';

{-- Variables ----------------------------------------------------------------}

var
    width,height,depth  : int;
    maxx,maxy           : int;
    pf                  : pixel_format_t;

{-- AccelBlit ----------------------------------------------------------------}

// Demo showing how to use offscreen dc's to store sprites and
// to use harware acceleration to blt them from the offscreen
// dc to the display dc.

function    AccelBlit(dc: PMGLDC; trans: Boolean): Boolean;
var
    offdc       : PMGLDC;
    transparent : color_t;
begin
    // Display title message at the top of the window.
    if trans then
        MainWindow(dc,'Hardware Accelerated Transparent Blit Demo')
    else
        MainWindow(dc,'Hardware Accelerated Blit Demo');
    StatusLine('Press any key to continue, ESC to Abort');

    offdc := MGL_createOffscreenDC;
    if (offdc = nil) or (offdc^.mi.xRes <= width) or (offdc^.mi.yRes <= height) then
        gprint('Hardware accelerated blit not available.')
    else
    begin
        // Load a bitmap into the offscreen device context. The palette gets
        // realized as well.
        if not MGL_loadBitmapIntoDC(offdc,IMAGE_NAME,0,0,TRUE) then
            MGLError;

        // Get transparent color from pixel (0,0)
        MGL_makeCurrentDC(offdc);
        transparent := MGL_getPixelCoord(0,0);
        MGL_makeCurrentDC(dc);

        // Copy image from offscreen video memory to the screen.
        while not checkEvent do
            if trans then
                MGL_transBltCoord(dc,offdc,0,0,width,height,MGL_random(maxx-width),MGL_random(maxy-height),transparent,true)
            else
                MGL_bitBltCoord(dc,offdc,0,0,width,height,MGL_random(maxx-width),MGL_random(maxy-height),MGL_REPLACE_MODE);

        // Remove all device contexts from memory.
        MGL_destroyDC(offdc);
    end;
    Result := Pause;
end;

{-- NonAccelBlit -------------------------------------------------------------}

// Demo showing how to use memory dc's to store sprites and
// to use blit them from the memory dc to the display dc.

function    NonAccelBlit(dc: PMGLDC; trans: Boolean): Boolean;
var
    color       : color_t;
    pal         : array[0..255] of palette_t;
    memdc       : PMGLDC;
begin
    // Display title message at the top of the window.
    if trans then
        MainWindow(dc,'Non-Accelerated Transparent Blit Demo')
    else
        MainWindow(dc,'Non-Accelerated Blit Demo');
    StatusLine('Press any key to continue, ESC to Abort');

    // This inserted to work ok in all modes
    depth := MGL_getBitsPerPixel(dc);
    MGL_getPixelFormat(dc,@pf);
    
    // Create a memory device context.
    memdc := MGL_createMemoryDC(width,height,depth,@pf);
    if memdc = nil then
        MGLError;

    try
        // Load a bitmap into the memory device context. The palette gets
        // loaded into the memory device context as well.
        if not MGL_loadBitmapIntoDC(memdc,IMAGE_NAME,0,0,TRUE) then
            MGLError;

        // Set the display device context with the palette from the
        // memory device context.
        if depth <= 8 then
        begin
            MGL_getPalette(memdc,@pal[0],MGL_getPaletteSize(memdc),0);
            MGL_setPalette(dc,@pal[0],MGL_getPaletteSize(memdc),0);
            MGL_realizePalette(dc,MGL_getPaletteSize(memdc),0,1);
        end;

        // Get transparent color from pixel (0,0)
        MGL_makeCurrentDC(memdc);
        color := MGL_getPixelCoord(0,0);
        MGL_makeCurrentDC(dc);

        // Copy image from image in memory to the screen.
        while not checkEvent do
        begin
            if trans then
                MGL_transBltCoord(dc,memdc,0,0,width,height,MGL_random(maxx-width),MGL_random(maxy-height),color,TRUE)
            else
                MGL_bitBltCoord(dc,memdc,0,0,width,height,MGL_random(maxx-width),MGL_random(maxy-height),MGL_REPLACE_MODE);
        end;
    finally
        // Remove all device contexts from memory.
        MGL_destroyDC(memdc);
    end;
    Result := Pause;
end;

{-- Demo ---------------------------------------------------------------------}

procedure   Demo(dc: PMGLDC);
begin
    // Get the screen size and store them.
    maxx := MGL_maxx;
    maxy := MGL_maxy;

    // Get image information
    MGL_getBitmapSize(IMAGE_NAME,@width,@height,@depth,@pf);

    if not AccelBlit(dc,true) then Exit;
    if not NonAccelBlit(dc,true) then Exit;
    if not AccelBlit(dc,false) then Exit;
    if not NonAccelBlit(dc,false) then Exit;
end;

initialization
    RegisterDemo('Sprites',
                 'Demo showing how to use offscreen dc''s to store sprites and '+
                 'to use harware acceleration to blt them from the offscreen '+
                 'dc to the display dc. Also shows falling back to a memory '+
                 'dc if an offscreen dc is not available.',
                 Demo);
end.
