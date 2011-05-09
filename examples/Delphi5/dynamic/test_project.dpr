program test_project;

uses
  Forms,
  test in 'test.pas' {Form1},
  pm600x_dyn in 'pm600x_dyn.pas';

{$R *.RES}

begin
  Application.Initialize;
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
