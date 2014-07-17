unit fmSelect;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, DemoList, ComCtrls;

type
  TDemoSelectForm = class(TForm)
    RunBtn: TButton;
    Button2: TButton;
    Label1: TLabel;
    DescrLabel: TLabel;
    TreeView1: TTreeView;
    procedure Button2Click(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure ListBox1DblClick(Sender: TObject);
    procedure RunBtnClick(Sender: TObject);
    procedure TreeView1Change(Sender: TObject; Node: TTreeNode);
  private
    { Private declarations }
    procedure   InitDemoTree;
    procedure   SelChanged;
    procedure   RunDemo;
  public
    { Public declarations }
  end;

var
  DemoSelectForm: TDemoSelectForm;
  SelectedDemo  : TDemoProc = nil;

implementation

{$R *.DFM}

procedure TDemoSelectForm.Button2Click(Sender: TObject);
begin
    Close;
end;

procedure TDemoSelectForm.FormShow(Sender: TObject);
begin
    InitDemoTree;
    Caption := Application.Title;
end;

procedure   TDemoSelectForm.InitDemoTree;
var
    Groups  : TStringList;
    i, j    : Integer;
    Node    : TTreeNode;
    List    : TList;
begin
    TreeView1.Items.Clear;
    Groups  := TStringList.Create;
    try
        List := TList.Create;
        try
            CollectGroups(Groups);
            for i := 0 to Groups.Count - 1 do
            begin
                Node := TreeView1.Items.AddObject(nil,Groups[i],Pointer(-1));
                CollectDemos(Groups[i],List);
                for j := 0 to List.Count - 1 do
                    TreeView1.Items.AddChildObject(Node,GetDemoTitle(Integer(List[j])),List[j]);
            end;
            TreeView1.FullExpand;            
            if TreeView1.Items.Count <> 0 then
            begin
                TreeView1.Selected  := TreeView1.Items[0];
                TreeView1.TopItem   := TreeView1.Items[0];
            end;
        finally
            List.Free;
        end;
    finally
        Groups.Free;
    end;
    TreeView1.SetFocus;
    SelChanged;
end;

procedure   TDemoSelectForm.SelChanged;
begin
    with TreeView1 do
        if (Selected <> nil) and (Selected.Data <> Pointer(-1)) then
        begin
            DescrLabel.Caption  := GetDemoDescr(Integer(Selected.Data));
            RunBtn.Enabled      := True;
        end
        else
        begin
            DescrLabel.Caption  := '';
            RunBtn.Enabled      := False;
        end;
end;

procedure TDemoSelectForm.ListBox1DblClick(Sender: TObject);
begin
    RunDemo;
end;

procedure TDemoSelectForm.RunBtnClick(Sender: TObject);
begin
    RunDemo;
end;

procedure   TDemoSelectForm.RunDemo;
var
    Ind : Integer;
begin
    Ind := Integer(TreeView1.Selected.Data);
    if Ind <> -1 then
    begin
        SelectedDemo := GetDemoProc(Ind);
        Close;
    end;
end;

procedure TDemoSelectForm.TreeView1Change(Sender: TObject;
  Node: TTreeNode);
begin
    SelChanged;
end;

end.
