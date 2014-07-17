// Sample code showing how to manually create the different
// pixel formats for memory DC buffers, which can be used to
// create buffers in modes other than the main display DC mode.
unit PixelFmt;

interface

implementation

uses
    MGLFx,
    MGLSamp,
    MGLTypes,
    SysUtils;

{-- Constants ----------------------------------------------------------------}

const
    IMAGE_NAME  = 'data\frog.bmp';
    MAX_FORMATS = 9;

{-- Variables ----------------------------------------------------------------}

var
    width,height,depth: int;

{-- More constants -----------------------------------------------------------}

const
    format_bits : array[0..MAX_FORMATS-1] of int = (8,15,16,24,24,32,32,32,32);
    format_rgb  : array[0..MAX_FORMATS-1] of string =
                        ('(INDEXED)',
                         '(555)',
                         '(565)',
                         '(RGB)',
                         '(BGR)',
                         '(ARGB)',
                         '(ABGR)',
                         '(RGBA)',
                         '(BGRA)'
                        );

    mempf       : array[0..MAX_FORMATS-1] of pixel_format_t = (
        // 8 bit (Ignored)
        (redMask:$FF;greenMask:$FF;blueMask:$FF;
         rsvdMask:$00;redPos:$10;redAdjust:$00;
         greenPos:$08;greenAdjust:$00;bluePos:$00;
         blueAdjust:$00;rsvdPos:$00;rsvdAdjust:$00),
        // 15 bit (555 Format)
        (redMask:$1F;greenMask:$1F;blueMask:$1F;
         rsvdMask:$01;redPos:$0A;redAdjust:$03;
         greenPos:$05;greenAdjust:$03;bluePos:$00;
         blueAdjust:$03;rsvdPos:$0F;rsvdAdjust:$07),
        // 16 bit (565 Format)
        (redMask:$1F;greenMask:$3F;blueMask:$1F;
         rsvdMask:$00;redPos:$0B;redAdjust:$03;
         greenPos:$05;greenAdjust:$02;bluePos:$00;
         blueAdjust:$03;rsvdPos:$00;rsvdAdjust:$00),
        // 24 bit (RGB Format)
        (redMask:$FF;greenMask:$FF;blueMask:$FF;
         rsvdMask:$00;redPos:$10;redAdjust:$00;
         greenPos:$08;greenAdjust:$00;bluePos:$00;
         blueAdjust:$00;rsvdPos:$00;rsvdAdjust:$00),
        // 24 bit (BGR Format)
        (redMask:$FF;greenMask:$FF;blueMask:$FF;
         rsvdMask:$00;redPos:$00;redAdjust:$00;
         greenPos:$08;greenAdjust:$00;bluePos:$10;
         blueAdjust:$00;rsvdPos:$00;rsvdAdjust:$00),
        // 32 bit (ARGB Format)
        (redMask:$FF;greenMask:$FF;blueMask:$FF;
         rsvdMask:$FF;redPos:$10;redAdjust:$00;
         greenPos:$08;greenAdjust:$00;bluePos:$00;
         blueAdjust:$00;rsvdPos:$18;rsvdAdjust:$00),
        // 32 bit (ABGR Format)
        (redMask:$FF;greenMask:$FF;blueMask:$FF;
         rsvdMask:$FF;redPos:$00;redAdjust:$00;
         greenPos:$08;greenAdjust:$00;bluePos:$10;
         blueAdjust:$00;rsvdPos:$18;rsvdAdjust:$00),
        // 32 bit (RGBA Format)
        (redMask:$FF;greenMask:$FF;blueMask:$FF;
         rsvdMask:$FF;redPos:$18;redAdjust:$00;
         greenPos:$10;greenAdjust:$00;bluePos:$08;
         blueAdjust:$00;rsvdPos:$00;rsvdAdjust:$00),
        // 32 bit (BGRA Format)
        (redMask:$FF;greenMask:$FF;blueMask:$FF;
         rsvdMask:$FF;redPos:$08;redAdjust:$00;
         greenPos:$10;greenAdjust:$00;bluePos:$18;
         blueAdjust:$00;rsvdPos:$00;rsvdAdjust:$00));

{-- PackBitmap ---------------------------------------------------------------}

// This function loads and packs a bitmap image. The palette
// gets loaded and realized as well.
function    PackBitmap(dc: PMGLDC; const BitmapFile: string; format: int): PMGLDC;
var
    pf  : pixel_format_t;
    pal : array[0..255] of palette_t;
    mdc : PMGLDC;
begin
    // Get image information
    MGL_getBitmapSize(PChar(bitmapFile),@width,@height,@depth,@pf);

    depth   := format_bits[format];
    pf      := mempf[format];
    if MGL_getBitsPerPixel(dc) = depth then
        MGL_getPixelFormat(dc,@pf);

    // Create a memory device context.
    mdc := MGL_createMemoryDC(width,height,depth,@pf);
    if mdc = nil then
        MGLError;

    // Load a bitmap into the memory device context. The palette gets
    // loaded into the memory device context as well.
    if not MGL_loadBitmapIntoDC(mdc,IMAGE_NAME,0,0,TRUE) then
        MGLError;

    // Set the display device context with the palette from the
    // memory device context.
    if (MGL_getBitsPerPixel(mdc) = 4) or (MGL_getBitsPerPixel(mdc) = 8) then
    begin
        MGL_getPalette(mdc,@pal[0],MGL_getPaletteSize(mdc),0);
        MGL_setPalette(dc,@pal[0],MGL_getPaletteSize(mdc),0);
        MGL_realizePalette(dc,MGL_getPaletteSize(mdc),0,1);
    end
    else if (MGL_getBitsPerPixel(dc) >= 8) then
    begin
        MGL_getHalfTonePalette(@pal[0]);
        MGL_setPalette(dc,@pal[0],MGL_getPaletteSize(dc),0);
        MGL_realizePalette(dc,MGL_getPaletteSize(dc),0,1);
    end;
    Result := mdc;
end;

{-- Demo ---------------------------------------------------------------------}

procedure   Demo(dc: PMGLDC);
var
    posx,posy   : int;
    memdc       : PMGLDC;
    buf         : string;
    format      : int;
begin
    for format := 0 to MAX_FORMATS - 1 do
    begin
        Buf := SysUtils.Format('%s %d Bit Pixel Format Demo',[format_rgb[format],format_bits[format]]);
        MainWindow(dc,buf);

        memdc := PackBitmap(dc,IMAGE_NAME,format);

        try
            // Justify the image that will be put on the screen
            posx := (MGL_sizex(dc)-width) div 2;
            posy := (MGL_sizey(dc)-height) div 2;

            // Copy image from image in memory to the screen.
            MGL_bitBltCoord(dc,memdc,0,0,width,height,posx,posy,MGL_REPLACE_MODE);

            // Pause until the user hits a key or presses a mouse button.
            if not Pause then
                Break;

        finally
            // Remove all device contexts from memory.
            MGL_destroyDC(memdc);
        end;
    end;
end;

initialization
    RegisterDemo('PixelFmt',
                 'Sample code showing how to manually create the different '+
                 'pixel formats for memory DC buffers, which can be used to '+
                 'create buffers in modes other than the main display DC mode.',
                 Demo);
end.
