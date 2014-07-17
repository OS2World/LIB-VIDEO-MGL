program GLDemos;

uses
  Forms,
  SysUtils,
  fmSelect in 'Shared\fmSelect.pas' {DemoSelectForm},
  Accanti in 'Book\Accanti.pas',
  Atlantis in 'Atlantis\Atlantis.pas',
  Anti in 'Book\Anti.pas',
  BezMesh in 'Book\BezMesh.pas',
  Checker in 'Book\Checker.pas',
  DepthCue in 'Book\DepthCue.pas',
  Dof in 'Book\Dof.pas',
  Fog in 'Book\Fog.pas',
  FogIndex in 'Book\FogIndex.pas',
  Material in 'Book\Material.pas',
  MipMap in 'Book\MipMap.pas',
  Nurbs in 'Book\Nurbs.pas',
  PickDp in 'Book\PickDp.pas',
  Plane in 'Book\Plane.pas',
  Polys in 'Book\Polys.pas',
  SccLight in 'Book\SccLight.pas',
  Scene in 'Book\Scene.pas',
  SceneBa in 'Book\SceneBa.pas',
  SceneF in 'Book\SceneF.pas',
  Stencil in 'Book\Stencil.pas',
  Stroke in 'Book\Stroke.pas',
  Surface in 'Book\Surface.pas',
  TeaAmb in 'Book\TeaAmb.pas',
  TeaPots in 'Book\TeaPots.pas',
  Texture in 'Book\Texture.pas',
  Bounce in 'Demos\Bounce.pas',
  Fractals in 'Demos\Fractals.pas',
  FViewer in 'Demos\FViewer.pas',
  Gears in 'Demos\Gears.pas',
  Gears2 in 'Demos\Gears2.pas',
  IsoSurf in 'Demos\IsoSurf.pas',
  Mech in 'Demos\Mech.pas',
  Morph3D in 'Demos\Morph3D.pas',
  Moth in 'Demos\Moth.pas',
  Rings in 'Demos\Rings.pas',
  Steam in 'Demos\Steam.pas',
  Wave in 'Demos\Wave.pas',
  Objects in 'Ideas\Objects.pas',
  A in 'Ideas\A.pas',
  B in 'Ideas\B.pas',
  D in 'Ideas\D.pas',
  d_holder in 'Ideas\d_holder.pas',
  d_lamp in 'Ideas\d_lamp.pas',
  d_logo in 'Ideas\d_logo.pas',
  d_logol in 'Ideas\d_logol.pas',
  d_logos in 'Ideas\d_logos.pas',
  E in 'Ideas\E.pas',
  F in 'Ideas\F.pas',
  H in 'Ideas\H.pas',
  I in 'Ideas\I.pas',
  M in 'Ideas\M.pas',
  N in 'Ideas\N.pas',
  O in 'Ideas\O.pas',
  P in 'Ideas\P.pas',
  S in 'Ideas\S.pas',
  R in 'Ideas\R.pas',
  T in 'Ideas\T.pas',
  Track in 'Ideas\Track.pas';

{$R *.RES}

{$R GLUTDLG.RES}

begin
  Application.Initialize;
  Application.Title := 'OpenGL demos';
  Application.CreateForm(TDemoSelectForm, DemoSelectForm);
  Application.Run;

  DemoSelectForm.Hide;
  try
      if @SelectedDemo <> nil then
        while True do
            SelectedDemo;
  except
    on E: EAbort do ;
  end;
end.
