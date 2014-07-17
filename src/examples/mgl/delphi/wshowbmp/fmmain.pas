// Windows version of SHOWBMP sample app that runs in a window
// on the Windows desktop. Will load a bitmap of any format
// into a memory DC with the same pixel depths as the main
// Windows video mode. Hence if you load a 24 bit image while
// in a HiColor or TrueColor video mode, the image will display
// with maximum color resolution. In 8 bit modes the image will
// be dithered if necessary.
unit fmMain;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  fmWDemo, Menus, MGLFx;

type
  TMainForm = class(TWinDemoForm)
    N1: TMenuItem;
    Open1: TMenuItem;
    Options1: TMenuItem;
    StretchItem: TMenuItem;
    IncludeItem: TMenuItem;
    AnimateItem: TMenuItem;
    OpenDialog1: TOpenDialog;
    procedure FormPaint(Sender: TObject);
    procedure Open1Click(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure StretchItemClick(Sender: TObject);
    procedure IncludeItemClick(Sender: TObject);
    procedure AnimateItemClick(Sender: TObject);
    procedure FormResize(Sender: TObject);
  private
    { Private declarations }
    FMemDC      : PMGLDC;
    FAppActive  : Boolean;

    procedure   DoneMemDC;
    procedure   UpdateEnabled;
    procedure   LoadBitmap(const FileName: string);
    procedure   WMActivateApp(var Msg: TWMActivateApp); message WM_ACTIVATEAPP;
    procedure   AppActivate(Active: Boolean);
    procedure   IdleAnimate(Sender: TObject; var Done: Boolean);
  protected
    procedure   InitDemo; override;
    procedure   DoneDemo; override;
    procedure   CreateContexts; override;
    procedure   DestroyContexts; override;
  public
    { Public declarations }
  end;

var
  MainForm: TMainForm;

implementation

{$R *.DFM}

uses
    MGLTypes;

{-- Constants ----------------------------------------------------------------}

const
    // Globals for setting up SYSPAL_NOSTATIC mode
    NumSysColors    = 19;
    rgbBlack        = $000000 ; // RGB(0,0,0)
    rgbWhite        = $FFFFFF ; // RGB(255,255,255)

    // These are the GetSysColor display element identifiers
    SysPalIndex : array[0..NumSysColors-1] of int = (
        COLOR_ACTIVEBORDER,
        COLOR_ACTIVECAPTION,
        COLOR_APPWORKSPACE,
        COLOR_BACKGROUND,
        COLOR_BTNFACE,
        COLOR_BTNSHADOW,
        COLOR_BTNTEXT,
        COLOR_CAPTIONTEXT,
        COLOR_GRAYTEXT,
        COLOR_HIGHLIGHT,
        COLOR_HIGHLIGHTTEXT,
        COLOR_INACTIVEBORDER,
        COLOR_INACTIVECAPTION,
        COLOR_MENU,
        COLOR_MENUTEXT,
        COLOR_SCROLLBAR,
        COLOR_WINDOW,
        COLOR_WINDOWFRAME,
        COLOR_WINDOWTEXT
    );

    // This array translates the display elements to black and white
    MonoColors : array[0..NumSysColors-1] of TColorRef = (
        rgbBlack,
        rgbWhite,
        rgbWhite,
        rgbWhite,
        rgbWhite,
        rgbBlack,
        rgbBlack,
        rgbBlack,
        rgbBlack,
        rgbBlack,
        rgbWhite,
        rgbWhite,
        rgbWhite,
        rgbWhite,
        rgbBlack,
        rgbWhite,
        rgbWhite,
        rgbBlack,
        rgbBlack
    );

{-- Variables ----------------------------------------------------------------}

var
    // This array holds the old color mapping so we can restore them
    OldColors   : array[0..NumSysColors-1] of TColorRef;
    OldPresent  : Boolean = False;

{-- TMainForm ----------------------------------------------------------------}

procedure   TMainForm.InitDemo;
begin
    inherited InitDemo;
end;

{-- TMainForm ----------------------------------------------------------------}

procedure   TMainForm.DoneMemDC;
begin
    if FMemDC <> nil then
    begin
        MGL_destroyDC(FMemDC);
        FMemDC := nil;
    end;
end;

{-- TMainForm ----------------------------------------------------------------}

procedure   TMainForm.DoneDemo;
begin
    DoneMemDC;
    AppActivate(False);
    inherited DoneDemo;
end;

{-- TMainForm ----------------------------------------------------------------}

procedure   TMainForm.CreateContexts;
begin
    inherited CreateContexts;
    if FMemDC <> nil then
        LoadBitmapPalette(WinDC,FMemDC);
    UpdateEnabled;
end;

{-- TMainForm ----------------------------------------------------------------}

procedure   TMainForm.DestroyContexts;
begin
    inherited DestroyContexts;
end;

{-- TMainForm ----------------------------------------------------------------}

procedure   TMainForm.UpdateEnabled;
begin
    IncludeItem.Enabled := MGL_getBitsPerPixel(WinDC) = 8;
    AnimateItem.Enabled := IncludeItem.Enabled;
end;

{-- TMainForm ----------------------------------------------------------------}

procedure   TMainForm.LoadBitmap(const FileName: string);
begin
    DoneMemDC;
    FMemDC := LoadBitmapIntoMemDC(WinDC,FileName);
    LoadBitmapPalette(WinDC,FMemDC);
    Invalidate;
end;

{-- TMainForm ----------------------------------------------------------------}

procedure TMainForm.FormPaint(Sender: TObject);
begin
  inherited;
  PatBlt(Canvas.Handle,0,0,MGL_sizex(WinDC)+1,MGL_sizey(WinDC)+1,WHITENESS);
  if FMemDC <> nil then
  begin
    InitDrawing;
    if StretchItem.Checked then
        MGL_stretchBltCoord(WinDC,FMemDC,
                        0,0,MGL_sizex(FMemDC)+1,MGL_sizey(FMemDC)+1,
                        0,0,MGL_sizex(WinDC)+1,MGL_sizey(WinDC)+1)
    else
        MGL_bitBltCoord(WinDC,FMemDC,
                        0,0,MGL_sizex(FMemDC)+1,MGL_sizey(FMemDC)+1,
                        0,0,MGL_REPLACE_MODE);
  end;
end;

{-- TMainForm ----------------------------------------------------------------}

procedure TMainForm.Open1Click(Sender: TObject);
begin
  inherited;
  if OpenDialog1.Execute then
    LoadBitmap(OpenDialog1.FileName);
end;

{-- TMainForm ----------------------------------------------------------------}

procedure TMainForm.FormShow(Sender: TObject);
begin
  inherited;
  Application.OnIdle := IdleAnimate;
  FAppActive         := True;
end;

{-- TMainForm ----------------------------------------------------------------}

procedure   TMainForm.WMActivateApp(var Msg: TWMActivateApp);
begin
    FAppActive := Msg.Active;
    if not IncludeItem.Checked then
        AppActivate(Msg.Active);
    MGL_appActivate(WinDC,Msg.Active);
end;

{-- TMainForm ----------------------------------------------------------------}

// If the application is activating, then swap the system
// into SYSPAL_NOSTATIC mode so that our palettes will display
// correctly.

procedure   TMainForm.AppActivate(Active: Boolean);
var
    DC  : HDC;
    i   : int;
begin
    DC := GetDC(0);
    try
        if (Active and (GetSystemPaletteUse(DC) = SYSPAL_STATIC)) then
        begin
            // Save the current mapping so we can restore it
            for i := 0 to NumSysColors - 1 do
                OldColors[i] := GetSysColor(SysPalIndex[i]);

            OldPresent := True;

            // Switch to SYSPAL_NOSTATIC and remap the colors
            SetSystemPaletteUse(DC, SYSPAL_NOSTATIC);
            SetSysColors(NumSysColors, SysPalIndex[0], MonoColors);
        end
        else if not Active and OldPresent then
        begin
            // Always switch back to SYSPAL_STATIC and the old mapping
            SetSystemPaletteUse(DC, SYSPAL_STATIC);
            SetSysColors(NumSysColors, SysPalIndex[0], OldColors);
        end;
    finally
        ReleaseDC(0,DC);
    end;
end;

{-- TMainForm ----------------------------------------------------------------}

procedure TMainForm.StretchItemClick(Sender: TObject);
begin
  inherited;
  StretchItem.Checked := not StretchItem.Checked ;
  Invalidate;
end;

{-- TMainForm ----------------------------------------------------------------}

procedure TMainForm.IncludeItemClick(Sender: TObject);
begin
  inherited;
  // Enable use of colors or remap colors to normal
  AppActivate(IncludeItem.Checked);
  IncludeItem.Checked := not IncludeItem.Checked;
  // Rebuild contexts to rebuild palette for the loaded bitmap
  CreateContexts;
end;

{-- TMainForm ----------------------------------------------------------------}

procedure TMainForm.AnimateItemClick(Sender: TObject);
begin
  inherited;
  AnimateItem.Checked := not AnimateItem.Checked;
end;

{-- TMainForm ----------------------------------------------------------------}

procedure   TMainForm.IdleAnimate(Sender: TObject; var Done: Boolean);
var
    palsize : int;
begin
    if AnimateItem.Checked and FAppActive and Visible then
    begin
        // newTime = timeGetTime();
        // if ((newTime - lastTime) > 100) {
        // Rotate the hardware palette
        palsize := MGL_getPaletteSize(WinDC);
        MGL_rotatePalette(WinDC,palsize-2,1,MGL_ROTATE_UP);
        MGL_realizePalette(WinDC,palsize,0,1);
        Done := False;
        // lastTime = newTime;
    end else
        Done := True;
end;

{-- TMainForm ----------------------------------------------------------------}

procedure TMainForm.FormResize(Sender: TObject);
begin
  inherited;
  Invalidate;
end;

end.
