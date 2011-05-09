program pm600x_example;

uses
  Forms,
  example in 'example.pas' {Form1},
  pm600x in 'pm600x.pas';

{$R *.RES}

begin
  Application.Initialize;
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
