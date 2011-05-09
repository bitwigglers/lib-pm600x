unit test;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  pm600x_dyn, StdCtrls;

type
  TForm1 = class(TForm)
    Button1: TButton;
    Memo1: TMemo;
    procedure Button1Click(Sender: TObject);
  private
    { Private-Deklarationen }
  public
    { Public-Deklarationen }
  end;

var
  Form1: TForm1;

implementation

{$R *.DFM}

procedure TForm1.Button1Click(Sender: TObject);
var
  pm : pm_context;
  list, curdev : ppm_list;
  power : single;
  serial : cardinal;
  idString : array[0..50] of Char;
begin
  if LoadPM600x() then begin
    if pm_init(pm) = 0 then begin
      // read library version
      Memo1.Lines.Add('Library Version: ' + pm_library_version());

      // find all devices on the bus ...
      if pm_find_all(pm, TYPE_PM_ALL, list) = 0 then begin
        if list = nil then begin
          Memo1.Lines.Add('No device found');
        end
        else begin
          // print out all found devices ...
          curdev := list;
          while curdev <> nil do begin
            serial := curdev.serial;
            Memo1.Lines.Add('Found device with serial no. ' + IntToStr(serial));
            curdev := curdev.next;
          end;
          // ... and open the first device
          if pm_open(pm, list.serial) = 0 then begin
            // blink it, just for fun ...
            pm_blink(pm);
            // read out id string
            pm_identify(pm, idString);
            Memo1.Lines.Add(idString);
            // set the frequency to 1 GHz
            pm_set_frequency(pm, 6000000000);
            // set the average count to 1000
            pm_set_averages(pm, 1000); 
            // .. and measure the power
            if pm_measure(pm, power) = 0 then begin
              Memo1.Lines.Add(FloatToStrF(power, ffFixed, 7, 2) + ' dBm');
            end;
            pm_close(pm);
          end;
          // deallocate the device list
          pm_list_free(list);
        end;
      end;
    end;
  end;
end;

end.
