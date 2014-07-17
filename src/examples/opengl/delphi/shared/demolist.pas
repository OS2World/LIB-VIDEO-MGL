unit DemoList;

interface

uses
    Classes;

type
    TDemoProc   = procedure;

procedure   RegisterDemo(const Group, Title, Descr: string; Proc: TDemoProc);

function    GetDemoCount                : Integer;
function    GetDemoGroup(i: Integer)    : string;
function    GetDemoTitle(i: Integer)    : string;
function    GetDemoDescr(i: Integer)    : string;
function    GetDemoProc(i: Integer)     : TDemoProc;
procedure   CollectGroups(Groups: TStrings);
procedure   CollectDemos(const Group: string; DemoList: TList);

implementation

type
    PDemoRec    = ^TDemoRec;
    TDemoRec    = record
        Group   : string;
        Title   : string;
        Descr   : string;
        Proc    : TDemoProc;
    end;

var
    List: TList;

procedure   RegisterDemo(const Group, Title, Descr: string; Proc: TDemoProc);
var
    P : PDemoRec;
begin
    New(P);
    try
        P.Group := Group;
        P.Title := Title;
        P.Descr := Descr;
        P.Proc  := Proc;
        List.Add(P);        
    except
        Dispose(P);
        raise;
    end;
end;

function    GetDemoCount                : Integer;
begin
    Result := List.Count;
end;

function    GetDemoGroup(i: Integer)    : string;
begin
    Result := PDemoRec(List[i]).Group;
end;

function    GetDemoTitle(i: Integer)    : string;
begin
    Result := PDemoRec(List[i]).Title;
end;

function    GetDemoDescr(i: Integer)    : string;
begin
    Result := PDemoRec(List[i]).Descr;
end;

function    GetDemoProc(i: Integer)     : TDemoProc;
begin
    Result := PDemoRec(List[i]).Proc;
end;

procedure   CollectGroups(Groups: TStrings);
var
    i: Integer;
begin
    Groups.Clear;
    for i := 0 to List.Count - 1 do
        with PDemoRec(List[i])^ do
            if Groups.IndexOf(Group) = -1 then
                Groups.Add(Group);
end;

procedure   CollectDemos(const Group: string; DemoList: TList);
var
    i: Integer;
begin
    DemoList.Clear;
    for i := 0 to List.Count - 1 do
        if PDemoRec(List[i]).Group = Group then
            DemoList.Add(Pointer(i));
end;

initialization
    List := TList.Create;
finalization
    while List.Count > 0 do
    begin
        Dispose(PDemoRec(List.Last));
        List.Remove(List.Last);
    end;
    List.Free;
end.
