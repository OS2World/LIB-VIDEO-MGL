program WShowBmp;

uses
  Forms,
  fmWDemo in '..\Shared\fmWDemo.pas' {WinDemoForm},
  fmAbout in '..\Shared\fmAbout.pas' {AboutForm},
  fmMain in 'fmMain.pas' {MainForm};

{$R *.RES}

begin
  Application.Initialize;
  Application.Title := 'SHOWBMP: MGL Bitmap Loading Demo';
  Application.CreateForm(TMainForm, MainForm);
  Application.CreateForm(TAboutForm, AboutForm);
  Application.Run;
end.
