unit fmVideo;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls, MGLFx, MGLTypes;

type
  TVideoModeForm = class(TForm)
    Label1: TLabel;
    ModesBox: TListBox;
    Label2: TLabel;
    AppLabel: TLabel;
    ColorBox: TRadioGroup;
    OptionsBox: TRadioGroup;
    RunBtn: TButton;
    Button2: TButton;
    procedure Button2Click(Sender: TObject);
    procedure RunBtnClick(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure ColorBoxClick(Sender: TObject);
    procedure ModesBoxClick(Sender: TObject);
    procedure ModesBoxDblClick(Sender: TObject);
  private
    { Private declarations }
    FValidModes : Boolean;
    FDriver     : int;
    FDC         : PMGLDC;
  protected
    procedure   CreateParams(var Params: TCreateParams); override;
    procedure   WMNCHitTest(var Msg: TWMNCHitTest); message WM_NCHITTEST;

    procedure   InitDemo(Mode: int); virtual;
    procedure   RunDemo; virtual; abstract;
    procedure   DoneDemo; virtual;
    function    GetDescription: string; virtual; abstract;
    function    HandleSuspendApp(DC: PMGLDC; Flags: int): int; virtual;
    function    RunEnabled: Boolean; virtual;
    procedure   UpdateEnabled; virtual;
    function    RequestPageCount(Mode: Integer): Integer; virtual;

    procedure   Demo(Mode: Integer);

    procedure   RefreshList;
    procedure   Run; virtual;

    procedure   MGLError;
{$IFNDEF MGLLITE}
    function    LoadBitmapIntoMemDC(DC: PMGLDC; const BitmapName: string): PMGLDC;
{$ENDIF}    
    procedure   WaitEvent;
  public
    { Public declarations }
    property    DisplayDC: PMGLDC read FDC;
  end;

var
  VideoModeForm: TVideoModeForm;

implementation

{$R *.DFM}

{-- TVideoModeForm -----------------------------------------------------------}
procedure TVideoModeForm.Button2Click(Sender: TObject);
begin
    Close;
end;

{-- TVideoModeForm -----------------------------------------------------------}
procedure TVideoModeForm.RunBtnClick(Sender: TObject);
begin
    Run;
end;

{-----------------------------------------------------------------------------}
var
    CBForm : TVideoModeForm;

function   SuspendAppCallback(DC: PMGLDC; Flags: int): int; cdecl;
begin
    if CBForm <> nil then
        Result := CBForm.HandleSuspendApp(DC,Flags)
    else
        Result := MGL_SUSPEND_APP;
end;

{-- TVideoModeForm -----------------------------------------------------------}
procedure   TVideoModeForm.InitDemo(Mode: int);
begin
    MGL_registerAllMemDrivers;
    if not MGL_init(@FDriver,@Mode,'') then
        MGLError;
    FDC := MGL_createDisplayDC(RequestPageCount(Mode));
    if FDC = nil then
        MGLError;
    MGL_makeCurrentDC(FDC);
    MGL_setSuspendAppCallback(SuspendAppCallback);
end;

{-- TVideoModeForm -----------------------------------------------------------}
procedure   TVideoModeForm.Demo(Mode: Integer);
begin
    InitDemo(Mode);
    try
        RunDemo;
    finally
        DoneDemo;
    end;
end;

{-- TVideoModeForm -----------------------------------------------------------}
procedure   TVideoModeForm.DoneDemo;
begin
    MGL_exit;
end;

{-- TVideoModeForm -----------------------------------------------------------}
procedure TVideoModeForm.FormShow(Sender: TObject);
begin
    AppLabel.Caption := 'Delphi ' + GetDescription ;
    AppLabel.Left    := (ClientWidth - AppLabel.Width) div 2;
    RefreshList;
    MGL_setMainWindow(Handle);
end;

{-- TVideoModeForm -----------------------------------------------------------}
procedure   TVideoModeForm.CreateParams(var Params: TCreateParams);
begin
    inherited CreateParams(Params);
    Params.Style := Params.Style and not WS_CAPTION;
end;

{-----------------------------------------------------------------------------}
const
    cd4bit          = 0;
    cd8bit          = 1;
    cd15bit         = 2;
    cd16bit         = 3;
    cd24bit         = 4;
    cd32bit         = 5;

    opWinDirect     = 0;
    opDirectDraw    = 1;
    opBoth          = 2;

{-- TVideoModeForm -----------------------------------------------------------}
procedure   TVideoModeForm.RefreshList;
const
    Bits: array[cd4Bit..cd32bit] of Integer = (4,8,15,16,24,32);
var
    Linear,
    WinDirect,
    DirectDraw  : Boolean;

    BitCount,
    Driver,
    Mode        : Integer;

    AvailModes  : PUChar;

    MaxX,
    MaxY,
    ModeBits    : int;

    M           : UChar;
begin
    Linear      := True;
    WinDirect   := OptionsBox.ItemIndex in [opWinDirect,opBoth];
    DirectDraw  := OptionsBox.ItemIndex in [opDirectDraw,opBoth];
    BitCount    := Bits[ColorBox.ItemIndex];

    FValidModes := False;
    RunBtn.Enabled := False;

    ModesBox.Clear;
    try
        MGL_unregisterAllDrivers;
        MGL_registerAllDispDrivers(Linear,DirectDraw,WinDirect);

        Driver      := grDETECT;
        MGL_detectGraph(@Driver, @Mode);

        AvailModes  := MGL_availableModes;
        while AvailModes^ <> $FF do
        begin
            M       := AvailModes^;
            MGL_modeResolution(M,@MaxX,@MaxY,@ModeBits);
            if ModeBits = BitCount then
                ModesBox.Items.AddObject(
                    Format('%s %2d page (%s)',[MGL_modeName(M),MGL_availablePages(M),MGL_modeDriverName(M)]),
                    TObject(M)
                                        );
            Inc(AvailModes);
        end;

        if ModesBox.Items.Count = 0 then
            ModesBox.Items.Add('No available modes for this selection')
        else
        begin
            FValidModes := True;
            FDriver     := Driver;
        end;

    except
        ModesBox.Items.Add('Error getting mode list');
        raise;
    end;

    if (ModesBox.Items.Count > 0) and FValidModes and (ModesBox.ItemIndex = -1) then
        SendMessage(ModesBox.Handle,WM_KEYDOWN,VK_DOWN,0);
end;

{-- TVideoModeForm -----------------------------------------------------------}
procedure TVideoModeForm.ColorBoxClick(Sender: TObject);
begin
    RefreshList;
end;

{-- TVideoModeForm -----------------------------------------------------------}
procedure   TVideoModeForm.WMNCHitTest(var Msg: TWMNCHitTest);
begin
    Msg.Result := HTCAPTION;
end;

{-- TVideoModeForm -----------------------------------------------------------}
procedure TVideoModeForm.ModesBoxClick(Sender: TObject);
begin
    UpdateEnabled;
end;

{-- TVideoModeForm -----------------------------------------------------------}
procedure   TVideoModeForm.UpdateEnabled;
begin
    RunBtn.Enabled := RunEnabled;
end;

{-- TVideoModeForm -----------------------------------------------------------}
function    TVideoModeForm.RequestPageCount(Mode: Integer): integer;
begin
    Result := 1;
end;

{-- TVideoModeForm -----------------------------------------------------------}
function  TVideoModeForm.RunEnabled: Boolean;
begin
    Result := FValidModes and (ModesBox.ItemIndex <> -1);
end;

{-- TVideoModeForm -----------------------------------------------------------}
function    TVideoModeForm.HandleSuspendApp(DC: PMGLDC; Flags: int): int;
begin
    Result := MGL_SUSPEND_APP;
end;

{-- TVideoModeForm -----------------------------------------------------------}
function    TVideoModeForm.LoadBitmapIntoMemDC(DC: PMGLDC; const BitmapName: string): PMGLDC;
var
    pal         : array[0..255] of palette_t;
    w, h, bpp   : int;
    pf          : pixel_format_t;
begin
    if not MGL_getBitmapSize(PChar(BitmapName),@w,@h,@bpp,@pf) then
        MGLError;

    // Load into Memory DC with same pixel depth and format as the display
    bpp := MGL_getBitsPerPixel(DC);
    MGL_getPixelFormat(DC,@pf);

    // Create the memory DC and load the bitmap file into it
    Result := MGL_createMemoryDC(w,h,bpp,@pf);
    if Result = nil then
        raise Exception.Create('Not enough memory to load bitmap!');

    if not MGL_loadBitmapIntoDC(Result,PChar(BitmapName),0,0,True) then
        MGLError;

    // If this is an 8 bits per pixel video mode, then set the physical
    // palette to that used by the bitmap we just loaded
    if MGL_getBitsPerPixel(DC) = 8 then
    begin
        MGL_getPalette(Result,@pal[0],MGL_getPaletteSize(Result),0);
        MGL_setPalette(DC,@pal[0],MGL_getPaletteSize(Result),0);
        MGL_realizePalette(DC,MGL_getPaletteSize(Result),0,1);
    end;
end;

{-- TVideoModeForm -----------------------------------------------------------}
procedure TVideoModeForm.ModesBoxDblClick(Sender: TObject);
begin
    Run;
end;

{-- TVideoModeForm -----------------------------------------------------------}
procedure   TVideoModeForm.WaitEvent;
var
    Evt: event_t;
begin
    EVT_halt(@evt,EVT_KEYDOWN or EVT_KEYREPEAT or EVT_MOUSEDOWN);
end;

{-- TVideoModeForm -----------------------------------------------------------}
procedure   TVideoModeForm.Run;
begin
    if RunBtn.Enabled then
        with ModesBox do
            Demo(int(Items.Objects[ItemIndex]));
end;

{-- TVideoModeForm -----------------------------------------------------------}
procedure   TVideoModeForm.MGLError;
var
    Res: int;
begin
    Res := MGL_result;
    raise Exception.CreateFmt('MGL error: %s (%d)',[MGL_errorMsg(Res),Res]);
end;

initialization
    MGL_setAppInstance(hInstance);
end.
