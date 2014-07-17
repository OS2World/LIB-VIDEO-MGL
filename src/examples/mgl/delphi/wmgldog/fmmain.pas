unit fmMain;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  fmWDemo, Menus, MGLFX, MGLTypes;

type
  TMainForm = class(TWinDemoForm)
    procedure FormPaint(Sender: TObject);
    procedure FormMouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
  private
    { Private declarations }
    FDogDC      : PMGLDC;
    FMemDC      : PMGLDC;
    FTransparent: color_t;
    FWidth      : int;
    FHeight     : int;
    FSizeX      : int;
    FSizeY      : int;
  protected
    procedure   InitDemo; override;
    procedure   DoneDemo; override;
    procedure   CreateContexts; override;
    procedure   DestroyContexts; override;
  public
    { Public declarations }
  end;

var
  MainForm: TMainForm;

implementation

{$R *.DFM}

{-- TMainForm ----------------------------------------------------------------}
procedure   TMainForm.InitDemo;
begin
    inherited InitDemo;

    // Load the sprite bitmap into the dogDC
    FDogDC  := LoadBitmapIntoMemDC(WinDC,'doggie2.bmp');

    // Get transparent color from pixel (0,0) and dimensions of sprite
    MGL_makeCurrentDC(FDogDC);
    FTransparent    := MGL_getPixelCoord(0,0);
    FWidth          := MGL_sizex(FDogDC)+1;
    FHeight         := MGL_sizey(FDogDC)+1;
    MGL_makeCurrentDC(nil);
end;

{-- TMainForm ----------------------------------------------------------------}
procedure   TMainForm.DoneDemo;
begin
    if FDogDC <> nil then
        MGL_destroyDC(FDogDC);
    inherited DoneDemo;
end;

{-- TMainForm ----------------------------------------------------------------}
procedure   TMainForm.CreateContexts;
var
    bits    : int;
    pf      : pixel_format_t;
    pal     : array[0..255] of palette_t;
begin
    inherited CreateContexts;

    GetMemoryBitmapDepth(WinDC,bits,pf);
    FSizex  := MGL_sizex(WinDC)+1;
    FSizey  := MGL_sizey(WinDC)+1;

    // Create the main backbuffer DC with same pixel format as Window DC
    FMemDC  := MGL_createMemoryDC(FSizex,FSizey,bits,@pf);
    if FMemDC = nil then
        raise Exception.Create('Unable to create Memory DC!');

    // Copy the palette from the sprite and store in the offscreen bitmap
    // and the Window DC. The MGL automatically takes care of creating
    // identity palettes when we realize the palette for the memory DC and
    // the Window DC, but we must make sure that both of them get realized.
    MGL_getPalette(FDogDC,@pal[0],MGL_getPaletteSize(FDogDC),0);
    MGL_setPalette(FMemDC,@pal[0],MGL_getPaletteSize(FMemDC),0);
    MGL_realizePalette(FMemDC,MGL_getPaletteSize(FMemDC),0,0);
    MGL_setPalette(WinDC,@pal[0],MGL_getPaletteSize(WinDC),0);
    MGL_realizePalette(WinDC,MGL_getPaletteSize(WinDC),0,0);

    // Clear the backbuffer and draw the dog in the middle
    MGL_makeCurrentDC(FMemDC);
    MGL_clearDevice;
    MGL_transBltCoord(FMemDC,FDogDC,0,0,FWidth,FHeight,(FSizex - FWidth) div 2,
                      (FSizey - FHeight) div 2,FTransparent,true);
    MGL_makeCurrentDC(nil);
end;

{-- TMainForm ----------------------------------------------------------------}
procedure   TMainForm.DestroyContexts;
begin
    if FMemDC <> nil then
    begin
        MGL_destroyDC(FMemDC);
        FMemDC := nil;
    end;
    inherited DestroyContexts;
end;

{-- TMainForm ----------------------------------------------------------------}
procedure TMainForm.FormPaint(Sender: TObject);
begin
  inherited;
  MGL_bitBltCoord(WinDC,FMemDC,0,0,FSizeX,FSizey,0,0,MGL_REPLACE_MODE);
end;

{-- TMainForm ----------------------------------------------------------------}
procedure TMainForm.FormMouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
var
    leftDown    : Boolean;
    rightDown   : Boolean;
    DestX, DestY: int;
begin
  inherited;
  leftDown  := ssLeft in Shift;
  rightDown := ssRight in Shift;
  if not leftDown and not rightDown then
    Exit;
  InitDrawing;
  DestX := X - FWidth div 2;
  DestY := Y - FHeight div 2;
  if leftDown then
    MGL_transBltCoord(FMemDC,FDogDC,0,0,FWidth,FHeight,DestX,DestY,FTransparent,true)
  else if rightDown then
    MGL_bitBltCoord(FMemDC,FDogDC,0,0,width,height,DestX,DestY,MGL_REPLACE_MODE);
  MGL_bitBltCoord(WinDC,FMemDC,DestX,DestY,DestX+FWidth,DestY+FHeight,DestX,DestY,MGL_REPLACE_MODE);
end;

end.
