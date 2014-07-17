// Demo showing how to use the text functions to draw text to
// the screen.
unit TextDemo;

interface

implementation

uses
    MGLFx,
    MGLTypes,
    MGLSamp,
    SysUtils;

{-- Constants ----------------------------------------------------------------}

const
    NUM_FONTS    = 24;

    fontFilename : array[0..NUM_FONTS-1] of string = (
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

{-- TextDump -----------------------------------------------------------------}

// Displays all the characters in each of the fonts in
// the distribution.

function    TextDump(dc: PMGLDC): Boolean;
var
    i,j,maxx    : int;
    font        : pfont_t;
    buf         : string;
    err         : int;
begin
    maxx    := MGL_maxx;
    Result  := True;

    // For each available font
    for i := 0 to NUM_FONTS - 1 do
    begin
        // Attempt to load the font file from disk. If it cannot be
        // found, then go onto the next one
        Buf     := FontFilename[i];
        font    := MGL_loadFont(PChar(Buf));
        err     := MGL_result();

        if err = grFontNotFound then
            Continue
        else if err <> grOk then
            MGLError;

        try
            Buf     := Format('Character set (%s)', [font^.name]);
            MainWindow(dc,Buf);

            // Display all characters on the screen
            MGL_useFont(font);
            for j := Ord(' ') to 255 do
            begin
                MGL_drawStr(PChar(string(Chr(j))));
                if MGL_getX + MGL_maxCharWidth > maxx then
                    MGL_moveToCoord(0,MGL_getY + MGL_textHeight);
            end;

        finally
            // Unload the font from memory
            MGL_unloadFont(font);
            MGL_useFont(defFont);
        end;

        Result := Pause;
        if not Result then
            Break;
    end;
    defaultAttributes(dc);
end;

{-- TextStyleDemo ------------------------------------------------------------}

// Show the large font in several different sizes and
// orientations.

function    TextStyleDemo(dc: PMGLDC): Boolean;
var
    i,x,y       : int;
    Buf         : string;
begin
    MainWindow(dc,'Text styles Demonstration');

    MGL_useFont(largeFont);
    x := 5;
    y := 5;
    for i := 5 downto 2 do
    begin
        MGL_setTextSize(2,i,2,i);
        Buf := Format('Size (2/%d)',[i]);
        MGL_drawStrXY(x,y,PChar(buf));
        Inc(y,MGL_textHeight);
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

{-- Demo ---------------------------------------------------------------------}

procedure   Demo(dc: PMGLDC);
begin
    if not TextDump(dc) then Exit;
    if not TextStyleDemo(dc) then Exit;
end;

initialization
    RegisterDemo('TextDemo',
                 'Demo showing how to use the text functions to draw text to '+
                 'the screen.',
                 Demo);
end.
