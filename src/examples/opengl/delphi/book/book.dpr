program Book;

uses
  Forms,
  SysUtils,
  fmSelect in '..\Shared\fmSelect.pas' {DemoSelectForm},
  DemoList in '..\Shared\DemoList.pas',
  Accanti in 'Accanti.pas',
  Jitter in 'Jitter.pas',
  Anti in 'Anti.pas',
  BezMesh in 'BezMesh.pas',
  Checker in 'Checker.pas',
  DepthCue in 'DepthCue.pas',
  Dof in 'Dof.pas',
  Fog in 'Fog.pas',
  FogIndex in 'FogIndex.pas',
  Material in 'Material.pas',
  MipMap in 'MipMap.pas',
  Nurbs in 'Nurbs.pas',
  PickDp in 'PickDp.pas',
  Plane in 'Plane.pas',
  Polys in 'Polys.pas',
  SccLight in 'SccLight.pas',
  Scene in 'Scene.pas',
  SceneBa in 'SceneBa.pas',
  SceneF in 'SceneF.pas',
  Stencil in 'Stencil.pas',
  Stroke in 'Stroke.pas',
  Surface in 'Surface.pas',
  TeaAmb in 'TeaAmb.pas',
  TeaPots in 'TeaPots.pas',
  Texture in 'Texture.pas';

{$R *.RES}

{$R GLUTDLG.RES}

begin
  Application.Initialize;
  Application.Title := 'Book demos';
  Application.CreateForm(TDemoSelectForm, DemoSelectForm);
  Application.Run;

  DemoSelectForm.Hide;
  if @SelectedDemo <> nil then
    while True do
        SelectedDemo;
end.
