unit CmdLine;

interface

procedure   InitGLUT;

implementation

uses
    MGLTypes,
    MGLGlut;

type
    PArgArray   = ^TArgArray;
    TArgArray   = array[0..0] of PChar;

procedure   InitGLUT;
var
    argc    : int;
    argv    : PArgArray;
    i       : Integer;
begin
    argc    := ParamCount + 1;
    GetMem(argv,SizeOf(PChar)*(argc+1));
    try
        for i := 0 to ParamCount do
            argv[i] := PChar(ParamStr(i));
        argv[argc] := nil;
        glutInit(@argc,@argv);
    finally
        FreeMem(argv);
    end;
end;

end.


