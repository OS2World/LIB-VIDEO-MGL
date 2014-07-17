unit fmMain;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  fmVideo, StdCtrls, ExtCtrls, MGLFx, MGLTypes;

type
  TMGLDogForm = class(TVideoModeForm)
    Button1: TButton;
    OpenDialog1: TOpenDialog;
    procedure Button1Click(Sender: TObject);
    procedure FormShow(Sender: TObject);
  private
    { Private declarations }
    FBitmap: string;
  protected
    procedure   InitDemo(Mode: int); override;
    procedure   RunDemo; override;
    function    GetDescription: string; override;
    function    HandleSuspendApp(DC: PMGLDC; Flags: int): int; override;
  public
    { Public declarations }
  end;

var
  MGLDogForm: TMGLDogForm;

implementation

{$R *.DFM}

{-- TMGLDogForm --------------------------------------------------------------}
procedure   TMGLDogForm.RunDemo;
var
    MemDC: PMGLDC;
    Trans: color_t;
    x, y : int;
    w, h : int;
    Done : bool;
    Evt  : event_t;
begin
    // Load bitmap file into memory DC with same pixel depth as display
    MemDC   := LoadBitmapIntoMemDC(DisplayDC,FBitmap);
    try

        // Get transparent color from pixel (0,0)
        MGL_makeCurrentDC(MemDC);
        Trans   := MGL_getPixelCoord(0,0);
        MGL_makeCurrentDC(DisplayDC);

        // Draw the bitmap initially in the middle of the display
        w := MGL_sizex(MemDC)+1;
        h := MGL_sizey(MemDC)+1;
        MGL_transBltCoord(DisplayDC,MemDC,
                          0,0,w,h,
                          (MGL_sizex(DisplayDC) - w) div 2,
                          (MGL_sizey(DisplayDC) - h) div 2,Trans,True);

        // Show the mouse cursor
        if MGL_getBitsPerPixel(DisplayDC) = 8 then
            MS_setCursorColor($FF);
        MS_show;

        Done := False;
        // Continue drawing doggies while the mouse buttons are pressed
        while not Done do
        begin
            if EVT_getNext(@Evt,EVT_EVERYEVT) then
            begin
                case Evt.what of
                    EVT_KEYDOWN:
                        if EVT_asciiCode(evt.message) = $1B then
                            Done := True;
                    EVT_MOUSEMOVE:
                        begin
                            x := Evt.where_x - w div 2;
                            y := Evt.where_y - h div 2;
                            MS_obscure;
                            try
                                if (Evt.modifiers and EVT_LEFTBUT) <> 0 then
                                    MGL_transBltCoord(DisplayDC,MemDC,
                                                      0,0,w,h,
                                                      x,y,
                                                      Trans,true)
                                else
                                if (Evt.modifiers and EVT_RIGHTBUT) <> 0 then
                                begin
                                    MGL_bitBltCoord(DisplayDC,MemDC,
                                                    0,0,w,h,
                                                    x,y,
                                                    MGL_REPLACE_MODE);
                                end;
                            finally
                                MS_show;
                            end;
                    end;
                end;
            end;
        end;
    finally
        MGL_destroyDC(MemDC);
    end;
end;

{-- TMGLDogForm --------------------------------------------------------------}
function    TMGLDogForm.GetDescription: string;
begin
    Result := 'Full Screen MGL Sprite Demo';
end;

{-- TMGLDogForm --------------------------------------------------------------}
procedure TMGLDogForm.Button1Click(Sender: TObject);
begin
  inherited;
  if OpenDialog1.Execute then
    FBitmap := OpenDialog1.FileName;
end;

{-- TMGLDogForm --------------------------------------------------------------}
procedure TMGLDogForm.FormShow(Sender: TObject);
begin
  inherited;
  FBitmap := 'doggie2.bmp';
end;

{-- TMGLDogForm --------------------------------------------------------------}
function    TMGLDogForm.HandleSuspendApp(DC: PMGLDC; Flags: int): int;
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

{-- TMGLDogForm --------------------------------------------------------------}
procedure   TMGLDogForm.InitDemo(Mode: int);
begin
    inherited InitDemo(Mode) ;
    // Turn off identity palette checking for maximum speed
    MGL_checkIdentityPalette(False);
end;

end.
