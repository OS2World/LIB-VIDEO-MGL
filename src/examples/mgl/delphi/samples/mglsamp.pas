//
// NOTE:   Fonts are searched for relative to the directory
//         where the driver files are found, or in the
//         relative to the directory specified by the
//         MGL_ROOT environment variable.
//
//         If the driver files are linked with the program
//         code, fonts are searched relative to the path
//         specified to MGL_init().
//
unit MGLSamp;

interface

uses
    Classes,
    MGLTypes,
    MGLFx;

{-- Service functions --------------------------------------------------------}

function    WaitEvent: Integer;
function    CheckEvent: Boolean;
function    RandomColor: color_t;
procedure   GPrintF(const Fmt: string; const Args: array of const);
procedure   GPrint(const Str: string);
procedure   DrawBorder;
procedure   StatusLine(const Msg: string);
function    Pause: Boolean;
procedure   DefaultAttributes(dc: PMGLDC);
procedure   MainWindow(dc: PMGLDC; const Heading: string);
procedure   MGLError;
procedure   Swap(var i1, i2: int);

{-- Information functions ----------------------------------------------------}

function    MaxColor    : color_t;
function    MaxColorUns : Double;
function    AspectRatio : double;
function    Aspect      : int;
function    PalSize     : int;
function    DefPal      : ppalette_t;
function    LargeFont   : pfont_t;
function    DefFont     : pfont_t;

{-- Demo management functions ------------------------------------------------}

type
    TDemoProc   = procedure(DC: PMGLDC);

procedure   RegisterDemo(const Title, Descr: string; Proc: TDemoProc);
procedure   GetDemoList(List: TStrings);
function    GetDemoDescr(i: Integer): string;
procedure   RunDemo(i: Integer; DC: PMGLDC);

implementation

uses
    SysUtils;

{-- Internal variables -------------------------------------------------------}

var
    fullView    : rect_t; // Full screen viewport
    titleView   : rect_t; // Title text viewport
    statusView  : rect_t; // Status line viewport
    demoView    : rect_t; // Viewport to draw demo stuff in

    colorMask   : color_t;
    _defFont    : pfont_t;
    _largeFont  : pfont_t;
    snowLevel   : int = -1 ;

    defaultTextSettings : text_settings_t;
    colorMode   : int = MGL_CMAP_MODE;

    _maxColor   : color_t;
    _aspect     : int;
    _aspectRatio: double;

    _palSize    : int;
    _defPal     : ppalette_t;

{-- WaitEvent ----------------------------------------------------------------}

// Waits for an event and returns the ASCII code of the key that was pressed.

function    WaitEvent: Integer;
var
    Evt: event_t;
begin
    EVT_halt(@Evt,EVT_KEYDOWN or EVT_KEYREPEAT or EVT_MOUSEDOWN);
    if (evt.what and EVT_MOUSEDOWN) = 0 then
        Result := EVT_asciiCode(Evt.message)
    else
        Result := 0;
end;

{-- CheckEvent ---------------------------------------------------------------}

// Checks to see if there are any keyboard or mouse events waiting in the
// event queue.

function    CheckEvent: Boolean;
var
    Evt: event_t;
begin
    EVT_peekNext(@Evt,EVT_KEYDOWN or EVT_KEYREPEAT or EVT_MOUSEDOWN);
    Result := Evt.what <> EVT_NULLEVT;
end;

{-- RandomColor --------------------------------------------------------------}

// Computes a random color value and returns it. To do this so it works
// properly in RGB modes with all pixel formats, we need to generate a
// random 32 bit number and then mask it to the current pixel format.
// In some systems the alpha channel information in 16 bit and 32 bit
// pixel modes is active, and hence *must* be set to zero for proper
// operation (the ATI Mach64 is one such card).

function    RandomColor: color_t;
begin
    Result := MGL_randoml($FFFFFFFF) and colorMask;
end;

{-- GPrint -------------------------------------------------------------------}

// Simple writeln style output routine for sending text to the current
// viewport. It begins drawing the string at the current CP location,
// and move the CP to the start of the next logical line.

procedure   GPrint(const Str: string);
var
    CP  : point_t;
begin
    MGL_getCP(@CP);

    MGL_drawStr(PChar(Str));

    Inc(CP.y,MGL_textHeight);
    MGL_moveTo(CP);
end;

{-- GPrintF ------------------------------------------------------------------}

// Simple printf style output routine for sending text to the current
// viewport. It begins drawing the string at the current CP location,
// and move the CP to the start of the next logical line.

procedure   GPrintF(const Fmt: string; const Args: array of const);
begin
    GPrint(Format(Fmt,Args));
end;

{-- DrawBorder ---------------------------------------------------------------}

// Draws a border around the currently active viewport. The border is drawn
// just inside the bounds of the viewport.

procedure   DrawBorder;
var
    view: rect_t;
begin
    MGL_getViewport(@view);
    MGL_setViewport(fullView);
    MGL_rect(view);
    MGL_setViewport(view);
end;

{-- StatusLine ---------------------------------------------------------------}

// Displays a message in the status bar. The status bar is cleared before
// the message is displayed.

procedure   StatusLine(const Msg: string);
var
    tset    : text_settings_t;
    oldView : rect_t;
begin
    MGL_getViewport(@oldView);
    MGL_insetRect(statusView,1,1);
    MGL_setViewport(statusView);
    MGL_clearViewport;

    MGL_getTextSettings(@tset);
    MGL_setTextSettings(@defaultTextSettings);
    MGL_setTextJustify(MGL_CENTER_TEXT,MGL_CENTER_TEXT);
    MGL_drawStrXY(MGL_maxx() div 2, MGL_maxy() div 2, PChar(Msg));
    MGL_setTextSettings(@tset);

    MGL_insetRect(statusView,-1,-1);
    MGL_setViewport(oldView);
end;

{-- Pause --------------------------------------------------------------------}

// Pause until a key is pressed. If the key is the ESC key, the exit the
// program

function    Pause: Boolean;
const
    Msg: string = 'Esc aborts or press a key...';
begin
    statusLine(Msg);
    Result := WaitEvent <> $1B;
end;

{-- DefaultAttributes --------------------------------------------------------}

// Fills in the DC with the default attributes, and sets the
// font to the default font that we have loaded (the MGL does
// not provide a default font).

procedure   DefaultAttributes(dc: PMGLDC);
begin
    MGL_defaultAttributes(dc);
    MGL_useFont(_defFont);
    MGL_setColorMapMode(colorMode);
end;

{-- MainWindow ---------------------------------------------------------------}

// Displays a heading for the demo, sets the viewport for
// the demo code and draws a border around the viewport. We
// also restore the default attributes.

procedure   MainWindow(dc: PMGLDC; const Heading: string);
var
    tset    : text_settings_t;
    r       : rect_t;
begin
    MS_hide;
    defaultAttributes(dc);
    MGL_clearDevice;

    MGL_setViewport(titleView);

    MGL_getTextSettings(@tset);
    MGL_setTextSettings(@defaultTextSettings);
    MGL_setTextJustify(MGL_CENTER_TEXT,MGL_CENTER_TEXT);
    MGL_drawStrXY(MGL_maxx div 2, MGL_maxy div 2, PChar(Heading));
    MGL_setTextSettings(@tset);

    MGL_setViewport(statusView);
    drawBorder();
    MGL_setViewport(demoView);
    drawBorder();

    r := demoView;
    MGL_insetRect(r,1,1);
    MGL_setViewport(r);
    MS_show;
end;

{-- MaxColor -----------------------------------------------------------------}

function    MaxColor: color_t;
begin
    Result := _maxcolor;
end;

{-- MaxColorUns --------------------------------------------------------------}
function    MaxColorUns : Double;
begin
    if _maxcolor < 0 then
        Result := -((1 shl 31)*2.0)+_maxcolor*1.0
    else
        Result := _maxcolor;
end;

{-- AspectRatio --------------------------------------------------------------}

function    AspectRatio: double;
begin
    Result := _aspectRatio;
end;

{-- Aspect -------------------------------------------------------------------}

function    Aspect: int;
begin
    Result := _aspect;
end;

{-- PalSize ------------------------------------------------------------------}

function    PalSize: int;
begin
    Result := _palSize;
end;

{-- DefPal -------------------------------------------------------------------}

function    DefPal: ppalette_t;
begin
    Result := _defPal;
end;

{-- LargeFont ----------------------------------------------------------------}

function    LargeFont   : pfont_t;
begin
    Result := _largeFont;
end;

{-- DefFont ------------------------------------------------------------------}

function    DefFont   : pfont_t;
begin
    Result := _defFont;
end;

{-- Swap ---------------------------------------------------------------------}

procedure   Swap(var i1, i2: int);
var
    t: int;
begin
    t   := i2;
    i2  := i1;
    i1  := t;
end;

{-- MGLError -----------------------------------------------------------------}

procedure   MGLError;
var
    Res: int;
begin
    Res := MGL_result;
    raise Exception.CreateFmt('MGL error: %s (%d)', [MGL_errorMsg(Res),Res]);
end;

{-- PrepareToRun -------------------------------------------------------------}

procedure   PrepareToRun(dc: PMGLDC);
var
    Pal     : array[0..255] of palette_t;
    height  : int;
begin
    // Load the default 8x8 bitmap font into device context
    MGL_makeCurrentDC(dc);
    _defFont := MGL_loadFont('pc8x8.fnt');
    if _defFont = nil then
        raise Exception.Create('Error loading font. Probably you''ve not set MGL_ROOT environment variable.');
    MGL_useFont(_defFont);

    // Change default startup options from command line
    if snowlevel <> -1 then
        MGL_setPaletteSnowLevel(dc,snowlevel);

    // Load a large font for labelling
    _largeFont := MGL_loadFont('romant.fnt');
    if _largeFont = nil then
        raise Exception.Create('Can''t load roman font');

    // Obtain a few useful constant
    _maxcolor       := MGL_maxColor(dc);
    _aspect         := MGL_getAspectRatio(); // Aspect ratio for display mode
    _aspectRatio    := 1000.0 / _aspect;
    MGL_getTextSettings(@defaultTextSettings);

    if (MGL_getBitsPerPixel(dc) <= 8) then
        colorMask := MGL_maxColor(dc)
    else
        colorMask := MGL_packColor(@dc.pf,$FF,$FF,$FF);

    // Change to dithered rendering is requested
    if (dc.mi.bitsPerPixel = 8) and (colorMode = MGL_DITHER_RGB_MODE) then
    begin
        MGL_setColorMapMode(colorMode);
        MGL_getHalfTonePalette(@pal);
        MGL_setPalette(dc,@pal,256,0);
        MGL_realizePalette(dc,256,0,1);
        colorMask := $FFFFFF;
    end;

    // Allocate space for the palette, and read the default palette
    _palSize := MGL_getPaletteSize(dc);
    if _palSize <> -1 then
    begin
        GetMem(_defPal,palsize * sizeof(palette_t));
        MGL_getPalette(dc,_defPal,palsize,0);
    end
    else
        _defPal := nil;

    // Determine the size of the viewports required
    MGL_getViewport(@fullView);
    height              := MGL_textHeight;  // Height of default font
    titleView           := fullView;
    titleView.bottom    := titleView.top + height + 5;
    statusView          := fullView;
    statusView.top      := statusView.bottom - height - 5;
    demoView            := fullView;
    demoView.top        := titleView.bottom;
    demoView.bottom     := statusView.top+1;
end;

{-- FreeAfterRunning ---------------------------------------------------------}

procedure   FreeAfterRunning(dc: PMGLDC);
begin
    MGL_unloadFont(_defFont);
    _defFont := nil;
    MGL_unloadFont(_largeFont);
    _largeFont := nil;
    if _defPal <> nil then
        FreeMem(_defPal,palSize * sizeof(palette_t));
end;

{-- Demo management types and variables --------------------------------------}

type
    TDemoRec    = class
    public
        Title   : string;
        Descr   : string;
        Proc    : TDemoProc;
    end;

var
    Demos: TList;

{-- RegisterDemo -------------------------------------------------------------}

procedure   RegisterDemo(const Title, Descr: string; Proc: TDemoProc);
var
    Rec: TDemoRec;
begin
    Rec := TDemoRec.Create;
    try
        Rec.Title := Title;
        Rec.Proc  := Proc;
        Rec.Descr := Descr;
        Demos.Add(Rec);
    except
        Rec.Free;
        raise;
    end;
end;

{-- GetDemoList --------------------------------------------------------------}

procedure   GetDemoList(List: TStrings);
var
    i: Integer;
begin
    List.Clear;
    for i := 0 to Demos.Count - 1 do
        List.Add(TDemoRec(Demos[i]).Title);
end;

{-- GetDemoDescr -------------------------------------------------------------}

function    GetDemoDescr(i: Integer): string;
begin
    with TDemoRec(Demos[i]) do
        Result := Descr;
end;

{-- RunDemo ------------------------------------------------------------------}

procedure   RunDemo(i: Integer; DC: PMGLDC);
begin
    PrepareToRun(DC);
    try
        TDemoRec(Demos[i]).Proc(DC);
    finally
        FreeAfterRunning(DC);
    end;
end;

{-- FreeDemos ----------------------------------------------------------------}

procedure   FreeDemos;
var
    i: Integer;
begin
    for i := 0 to Demos.Count - 1 do
        TDemoRec(Demos[i]).Free;
end;

initialization
    Demos := TList.Create;
finalization
    if Demos <> nil then
    begin
        FreeDemos;
        Demos.Free;
    end;
end.
