program ShowBMP;

uses
  Forms,
  fmVideo in '..\Shared\fmVideo.pas' {VideoModeForm},
  fmMain in 'fmMain.pas' {ShowBMPForm};

{$R *.RES}

begin
  Application.Initialize;
  Application.CreateForm(TShowBMPForm, ShowBMPForm);
  Application.Run;
end.
