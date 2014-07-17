unit fmMain;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  fmVideo, StdCtrls, ExtCtrls;

type
  TSamplesForm = class(TVideoModeForm)
    DemosBox: TListBox;
    Label3: TLabel;
    DescrLabel: TLabel;
    procedure DemosBoxClick(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure DemosBoxDblClick(Sender: TObject);
  private
    { Private declarations }
  protected
    procedure   RunDemo; override;
    function    GetDescription: string; override;
    function    RunEnabled: Boolean; override;
    function    RequestPageCount(Mode: Integer): Integer; override;
  public
    { Public declarations }
  end;

var
  SamplesForm: TSamplesForm;

implementation

{$R *.DFM}

uses
    MGLSamp,
    MGLFx;

{-- TSamplesForm -------------------------------------------------------------}
procedure   TSamplesForm.RunDemo;
begin
    MGLSamp.RunDemo(DemosBox.ItemIndex,DisplayDC);
end;

{-- TSamplesForm -------------------------------------------------------------}
function    TSamplesForm.GetDescription: string;
begin
    Result := 'MGL Samples';
end;

{-- TSamplesForm -------------------------------------------------------------}
function    TSamplesForm.RunEnabled: Boolean;
begin
    Result := inherited RunEnabled and (DemosBox.ItemIndex <> -1);
end;

{-- TSamplesForm -------------------------------------------------------------}
procedure TSamplesForm.DemosBoxClick(Sender: TObject);
begin
  inherited;
  UpdateEnabled;
  with DemosBox do
    if ItemIndex <> -1 then
        DescrLabel.Caption := GetDemoDescr(ItemIndex)
    else
        DescrLabel.Caption := '';
end;

{-- TSamplesForm -------------------------------------------------------------}
procedure TSamplesForm.FormShow(Sender: TObject);
begin
    inherited;
    GetDemoList(DemosBox.Items);
    if DemosBox.Items.Count > 0 then
        SendMessage(DemosBox.Handle,WM_KEYDOWN,VK_DOWN,0);
end;

{-- TSamplesForm -------------------------------------------------------------}
function    TSamplesForm.RequestPageCount(Mode: Integer): Integer;
begin
    Result := MGL_availablePages(Mode)
end;

{-- TSamplesForm -------------------------------------------------------------}
procedure TSamplesForm.DemosBoxDblClick(Sender: TObject);
begin
  inherited;
  Run;
end;

end.
