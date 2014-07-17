unit fmAbout;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls;

type
  TAboutForm = class(TForm)
    Button1: TButton;
    VerLabel: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    procedure FormShow(Sender: TObject);
  private
    { Private declarations }
    procedure   CreateParams(var Params: TCreateParams); override;
  public
    { Public declarations }
  end;

var
  AboutForm: TAboutForm;

implementation

{$R *.DFM}

uses
    MGLFx;

{-- TAboutForm ---------------------------------------------------------------}

procedure   TAboutForm.CreateParams(var Params: TCreateParams);
begin
    inherited CreateParams(Params);
    Params.Style := Params.Style and not WS_CAPTION;
end;

{-- TAboutForm ---------------------------------------------------------------}

procedure TAboutForm.FormShow(Sender: TObject);
begin
    VerLabel.Caption := Format('MGL %s for Windows',[MGL_VERSION_STR]);
end;

end.
