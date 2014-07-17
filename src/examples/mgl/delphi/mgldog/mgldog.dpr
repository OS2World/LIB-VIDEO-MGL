program MGLDog;

uses
  Forms,
  fmMain in 'fmMain.pas' {MGLDogForm},
  fmVideo in '..\SHARED\fmvideo.pas' {VideoModeForm};

{$R *.RES}

begin
  Application.Initialize;
  Application.CreateForm(TMGLDogForm, MGLDogForm);
  Application.CreateForm(TVideoModeForm, VideoModeForm);
  Application.Run;
end.
