unit fmMain;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  fmVideo, StdCtrls, ExtCtrls, MGLFx, MGLTypes;

type
  TShowBMPForm = class(TVideoModeForm)
    Button1: TButton;
    OpenDialog1: TOpenDialog;
    procedure Button1Click(Sender: TObject);
  private
    { Private declarations }
    FBitmap: string;
  protected
    procedure   RunDemo; override;
    function    GetDescription: string; override;
    function    HandleSuspendApp(DC: PMGLDC; Flags: int): int; override;
    procedure   Run; override;
  public
    { Public declarations }
  end;

var
  ShowBMPForm: TShowBMPForm;

implementation

{$R *.DFM}

{-- TShowBMPForm -------------------------------------------------------------}
procedure   TShowBMPForm.RunDemo;
begin
    if not MGL_loadBitmapIntoDC(DisplayDC,PChar(FBitmap),0,0,True) then
        MGLError;
    WaitEvent;
end;

{-- TShowBMPForm -------------------------------------------------------------}
function    TShowBMPForm.GetDescription: string;
begin
    Result := 'Full Screen MGL Bitmap Loading Demo';
end;

{-- TShowBMPForm -------------------------------------------------------------}
procedure TShowBMPForm.Button1Click(Sender: TObject);
begin
  inherited;
  if OpenDialog1.Execute then
    FBitmap := OpenDialog1.FileName;
end;

{-- TShowBMPForm -------------------------------------------------------------}
function    TShowBMPForm.HandleSuspendApp(DC: PMGLDC; Flags: int): int;
var
    OldDC: PMGLDC;
begin
    if Flags = MGL_REACTIVATE then
    begin
        OldDC := MGL_makeCurrentDC(DC);
        try
            MGL_clearDevice;
        finally
            MGL_makeCurrentDC(OldDC);
        end;
    end;
    Result := MGL_SUSPEND_APP;
end;

{-- TShowBMPForm -------------------------------------------------------------}
procedure   TShowBMPForm.Run; 
begin
    if FBitmap = '' then
        Button1.Click;
    if FBitmap = '' then
        Exit;
    inherited Run;
end;

end.
