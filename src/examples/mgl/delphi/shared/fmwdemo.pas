unit fmWDemo;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Menus, MGLFX, MGLTypes;

type
  TWinDemoForm = class(TForm)
    MainMenu1: TMainMenu;
    File1: TMenuItem;
    About1: TMenuItem;
    Exit1: TMenuItem;
    procedure Exit1Click(Sender: TObject);
    procedure About1Click(Sender: TObject);
    procedure FormResize(Sender: TObject);
    procedure FormPaint(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure FormHide(Sender: TObject);
  private
    { Private declarations }
    FWinDC: PMGLDC;
  protected
    procedure   GetMemoryBitmapDepth(dc: PMGLDC; var bits: int; var pf: pixel_format_t);
    function    LoadBitmapIntoMemDC(dc: PMGLDC; const bitmapName: string): PMGLDC;
    procedure   LoadBitmapPalette(WinDC, MemDC: PMGLDC);
    procedure   InitDrawing;

    procedure   InitDemo; virtual;
    procedure   DoneDemo; virtual;
    procedure   CreateContexts; virtual;
    procedure   DestroyContexts; virtual;
    function    UpdatePalette: Integer;
    procedure   WMPaletteChanged(var Msg: TWMPaletteChanged); message WM_PALETTECHANGED;
    procedure   WMQueryNewPalette(var Msg: TWMQueryNewPalette); message WM_QUERYNEWPALETTE;
  public
    { Public declarations }
    property    WinDC: PMGLDC read FWinDC;
  end;

var
  WinDemoForm: TWinDemoForm;

implementation

uses fmAbout;

{$R *.DFM}

{-- TWinDemoForm -------------------------------------------------------------}

// Set same format as the current Windows display mode
// NOTE: We do not support Win3.x, so we should not limit us to 8 bits

procedure   TWinDemoForm.GetMemoryBitmapDepth(dc: PMGLDC; var bits: int; var pf: pixel_format_t);
begin
    bits := MGL_getBitsPerPixel(dc);
    MGL_getPixelFormat(dc,@pf);
end;

{-- TWinDemoForm -------------------------------------------------------------}

// Loads the specified bitmap into a memory DC with the
// same dimensions as the bitmap on disk, but with the same
// pixel depth and format used by the display DC (for maximum
// blt performance). The MGL automatically handles pixel
// format conversion for us when we load the bitmap into our
// memory DC, and the bitmap's palette is loaded into the
// memory DC's palette.

function    TWinDemoForm.LoadBitmapIntoMemDC(dc: PMGLDC; const bitmapName: string): PMGLDC;
var
    width,height,bits   : int;
    pf                  : pixel_format_t;
    memDC               : PMGLDC;
    tempDC              : PMGLDC;
begin
    // Get dimensions of bitmap image on disk
    if not MGL_getBitmapSize(PChar(bitmapName),@width,@height,@bits,@pf) then
        raise Exception.Create('Cant locate bitmap file!');

    tempDC := nil ;
    Result := nil ;
    if dc = nil then
    begin
        tempDC  := MGL_createWindowedDC(GetDesktopWindow);
        dc      := tempDC;
    end;
    try
        // Get bitmap depth and pixel format for memory DC
        GetMemoryBitmapDepth(dc,bits,pf);

        // Create the memory DC and load the bitmap file into it
        memDC := MGL_createMemoryDC(width,height,bits,@pf);
        if memDC = nil then
            raise Exception.Create('Not enough memory to load bitmap!');
        try
            if not MGL_loadBitmapIntoDC(memDC,PChar(bitmapName),0,0,true) then
                raise Exception.Create('Can''t locate bitmap file!');
        except
            MGL_destroyDC(memDC);
            raise;
        end;
        Result := memDC;
    finally
        if tempDC <> nil then
            MGL_destroyDC(tempDC);
    end;
end;

{-- TWinDemoForm -------------------------------------------------------------}

// Load the palette from the memory DC bitmap into the Window DC
// and realize it to make it the hardware palette. Note that
// we also realize the palette for the loaded bitmap in the
// memory DC to re-build the DIB's color table when we switch
// to and from SYSPAL_NOSTATIC mode.

procedure   TWinDemoForm.LoadBitmapPalette(WinDC, MemDC: PMGLDC);
var
    pal : array[0..255] of palette_t;
begin
    MGL_getPalette(MemDC,@pal[0],MGL_getPaletteSize(MemDC),0);
    MGL_setPalette(WinDC,@pal[0],MGL_getPaletteSize(WinDC),0);
    MGL_realizePalette(MemDC,MGL_getPaletteSize(MemDC),0,0);
    MGL_realizePalette(WinDC,MGL_getPaletteSize(WinDC),0,0);
end;

{-- TWinDemoForm -------------------------------------------------------------}

// Initialise the MGL and load all bitmap resources required by
// the application. This is where the application should allocate
// space for all MGL memory DC's used for storing sprites etc
// and load the sprites from disk.

procedure   TWinDemoForm.InitDemo;
begin
    // Register the MGL packed pixel drivers we want to use.
    MGL_registerDriver(MGL_PACKED1NAME,PACKED1_driver);
    MGL_registerDriver(MGL_PACKED4NAME,PACKED4_driver);
    MGL_registerDriver(MGL_PACKED8NAME,PACKED8_driver);
    MGL_registerDriver(MGL_PACKED16NAME,PACKED16_driver);
    MGL_registerDriver(MGL_PACKED24NAME,PACKED24_driver);
    MGL_registerDriver(MGL_PACKED32NAME,PACKED32_driver);

    // Initialise the MGL for Windowed output only
    MGL_initWindowed('');
end;

{-- TWinDemoForm -------------------------------------------------------------}

// Sets up the MGL for output to the client region of the
// specified window. This function will destroy any previously
// existing MGL device contexts, and will re-build all the
// DC's for the Window. Hence this routine is always called
// from the Windows WM_SIZE message handler, which will always
// be called before the first WM_PAINT message is received,
// allowing the MGL device contexts to be created with the
// apropriate size information.

procedure   TWinDemoForm.CreateContexts;
begin
    // Destroy the previous DC's if they existed
    DestroyContexts;

    // Create the main MGL Windowed DC for interfacing our main window
    FWinDC := MGL_createWindowedDC(Handle);
    if FWinDC = nil then
        raise Exception.Create('Unable to create Windowed DC!');
end;

{-- TWinDemoForm -------------------------------------------------------------}

procedure   TWinDemoForm.DestroyContexts;
begin
    if FWinDC <> nil then
    begin
        MGL_destroyDC(FWinDC);
        FWinDC := nil;
    end;
end;

{-- TWinDemoForm -------------------------------------------------------------}

procedure TWinDemoForm.Exit1Click(Sender: TObject);
begin
    Close;
end;

{-- TWinDemoForm -------------------------------------------------------------}

procedure TWinDemoForm.About1Click(Sender: TObject);
begin
    AboutForm.ShowModal;
end;

{-- TWinDemoForm -------------------------------------------------------------}

procedure TWinDemoForm.FormResize(Sender: TObject);
begin
    if Visible then
        CreateContexts;
end;

{-- TWinDemoForm -------------------------------------------------------------}

function    TWinDemoForm.UpdatePalette: Integer;
begin
    Result := 0;
    if (FWinDC <> nil) and MGL_activatePalette(FWinDC,False) then
    begin
        InvalidateRect(Handle,nil,TRUE);
        Result := 1;
    end;
end;

{-- TWinDemoForm -------------------------------------------------------------}

procedure   TWinDemoForm.WMPaletteChanged(var Msg: TWMPaletteChanged);
begin
    if Msg.PalChg <> Handle then
        Msg.Result := UpdatePalette;
end;

{-- TWinDemoForm -------------------------------------------------------------}

procedure   TWinDemoForm.WMQueryNewPalette(var Msg: TWMQueryNewPalette);
begin
    Msg.Result := UpdatePalette;
end;

{-- TWinDemoForm -------------------------------------------------------------}

procedure TWinDemoForm.FormPaint(Sender: TObject);
begin
    InitDrawing;
end;

{-- TWinDemoForm -------------------------------------------------------------}

procedure   TWinDemoForm.InitDrawing;
begin
    MGL_setWinDC(FWinDC,Canvas.Handle);
end;

{-- TWinDemoForm -------------------------------------------------------------}

procedure TWinDemoForm.FormShow(Sender: TObject);
begin
    Caption := Application.Title;
    InitDemo;
end;

{-- TWinDemoForm -------------------------------------------------------------}

procedure TWinDemoForm.FormHide(Sender: TObject);
begin
    DoneDemo;
end;

{-- TWinDemoForm -------------------------------------------------------------}

procedure   TWinDemoForm.DoneDemo;
begin
    MGL_exit;
end;

end.
