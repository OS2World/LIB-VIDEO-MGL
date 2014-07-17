program Samples;

uses
  Forms,
  MGLSamp in 'MGLSamp.pas',
  fmMain in 'fmMain.pas' {SamplesForm},
  MGLDemo in 'MGLDemo.pas',
  FFill in 'FFill.pas',
  BitBlt in 'BitBlt.pas',
  Bitmap in 'Bitmap.pas',
  Direct in 'Direct.pas',
  Dither in 'Dither.pas',
  Patterns in 'Patterns.pas',
  Ellipses in 'Ellipses.pas',
  Lines in 'Lines.pas',
  Mouse in 'Mouse.pas',
  MouseDB in 'MouseDB.pas',
  PageFlip in 'PageFlip.pas',
  pcx in 'pcx.pas',
  Palette in 'Palette.pas',
  PixelFmt in 'PixelFmt.pas',
  Polys in 'Polys.pas',
  rects in 'rects.pas',
  Regions in 'Regions.pas',
  Sprites in 'Sprites.pas',
  Stretch in 'Stretch.pas',
  TextDemo in 'TextDemo.pas',
  ViewPort in 'ViewPort.pas',
  Hello in 'Hello.pas',
  fmVideo in '..\SHARED\fmvideo.pas' {VideoModeForm};

{$R *.RES}

begin
  Application.Initialize;
  Application.CreateForm(TSamplesForm, SamplesForm);
  Application.CreateForm(TVideoModeForm, VideoModeForm);
  Application.Run;
end.
