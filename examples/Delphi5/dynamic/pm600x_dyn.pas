unit pm600x_dyn;
interface

const
TYPE_PM_6006 = $1f180400;
TYPE_PM_6003 = $1f180401;
TYPE_PM_ALL  = $0;

type
    pm_context = record
    handle : Pointer;
    devtype : Cardinal;
    frequency : Int64;
    averages : Word;
    error_str : Pointer;
end;

Ppm_context = ^pm_context;

ppm_list = ^pm_list;

pm_list = record
    next : ppm_list;
    serial : Cardinal;
    devtype : Cardinal;
end;

function pm_init             (var pm:pm_context):Integer;
function pm_find_all         (var pm:pm_context; devtype:Cardinal; var list:ppm_list):Integer;
procedure pm_list_free       (var list:ppm_list);
function pm_open             (var pm:pm_context; serial:LongWord):Integer;
function pm_close            (var pm:pm_context):Integer;
function pm_reset            (var pm:pm_context):Integer;
function pm_identify         (var pm:pm_context; buffer:PChar):Integer;
function pm_set_frequency    (var pm:pm_context; freq:Int64):Integer;
function pm_set_averages     (var pm:pm_context; avg:Word):Integer;
function pm_measure          (var pm:pm_context; var power:single):Integer;
function pm_blink            (var pm:pm_context):Integer;
function pm_get_error_string (var pm:pm_context):PChar;
function pm_library_version ():PChar;

function LoadPM600x():boolean;
function FreePM600x():boolean;

implementation
uses Windows, SysUtils;
{***************************************************************************}

type

{function prototypes as Type to use with dynamically link}
Tp_pm_init  = function(var pm:pm_context):Integer; stdcall;
Tp_pm_find_all = function(var pm:pm_context; devtype:Cardinal; var list:ppm_list):Integer; stdcall;
Tp_pm_list_free = procedure(var list:ppm_list); stdcall;
Tp_pm_open  = function(var pm:pm_context; serial:LongWord):Integer; stdcall;
Tp_pm_close = function(var pm:pm_context):Integer; stdcall;
Tp_pm_reset = function(var pm:pm_context):Integer; stdcall;
Tp_pm_identify = function (var pm:pm_context; buffer:PChar):Integer; stdcall;
Tp_pm_set_frequency = function(var pm:pm_context; freq:Int64):Integer; stdcall;
Tp_pm_set_averages = function(var pm:pm_context; avg:Word):Integer; stdcall;
Tp_pm_measure = function(var pm:pm_context; var power:single):Integer; stdcall;
Tp_pm_blink = function(var pm:pm_context):Integer; stdcall;
Tp_pm_get_error_string = function(var pm:pm_context):PChar; stdcall;
Tp_pm_library_version = function():PChar; stdcall;

var

{function variables}
_pm_init : Tp_pm_init;
_pm_find_all : Tp_pm_find_all;
_pm_list_free : Tp_pm_list_free;
_pm_open : Tp_pm_open;
_pm_close : Tp_pm_close;
_pm_reset : Tp_pm_reset;
_pm_identify : Tp_pm_identify;
_pm_set_frequency : Tp_pm_set_frequency;
_pm_set_averages : Tp_pm_set_averages;
_pm_measure : Tp_pm_measure;
_pm_blink : Tp_pm_blink;
_pm_get_error_string : Tp_pm_get_error_string;
_pm_library_version : Tp_pm_library_version;

Handle: THandle;

function LoadPM600x():boolean;
begin
Handle := LoadLibrary('libpm600x.dll');
if Handle<>0 then
  begin
  @_pm_init := GetProcAddress(Handle, 'pm_init');
  @_pm_find_all := GetProcAddress(Handle, 'pm_find_all');
  @_pm_list_free := GetProcAddress(Handle, 'pm_list_free');
  @_pm_open := GetProcAddress(Handle, 'pm_open');
  @_pm_close := GetProcAddress(Handle, 'pm_close');
  @_pm_reset := GetProcAddress(Handle, 'pm_reset');
  @_pm_identify := GetProcAddress(Handle, 'pm_identify');
  @_pm_set_frequency := GetProcAddress(Handle, 'pm_set_frequency');
  @_pm_set_averages := GetProcAddress(Handle, 'pm_set_averages');
  @_pm_measure:= GetProcAddress(Handle, 'pm_measure');
  @_pm_blink:= GetProcAddress(Handle, 'pm_blink');
  @_pm_get_error_string := GetProcAddress(Handle, 'pm_get_error_string');
  @_pm_library_version := GetProcAddress(Handle, 'pm_library_version');
  end;
Result:=(Handle<>0);
end;

function FreePM600x():boolean;
begin
if Handle<>0 then
  begin
  @_pm_init := nil;
  @_pm_find_all := nil;
  @_pm_list_free := nil;
  @_pm_open := nil;
  @_pm_close := nil;
  @_pm_reset := nil;
  @_pm_identify := nil;
  @_pm_set_frequency := nil;
  @_pm_set_averages := nil;
  @_pm_measure := nil;
  @_pm_blink := nil;
  @_pm_get_error_string := nil;
  @_pm_library_version := nil;
  FreeLibrary(Handle);
  Handle:=0;
  end;
Result:=True;
end;

function pm_init(var pm:pm_context):Integer;
begin
if @_pm_init<>nil
then Result:=_pm_init(pm)
else Result:=-1;
end;

function pm_find_all(var pm:pm_context; devtype:Cardinal; var list:ppm_list):Integer;
begin
if @_pm_find_all<>nil
then Result:=_pm_find_all(pm, devtype, list)
else Result:=-1;
end;

procedure pm_list_free(var list:ppm_list);
begin
if @_pm_list_free<>nil
   then _pm_list_free(list)
end;

function pm_open(var pm:pm_context; serial:LongWord):Integer;
begin
if @_pm_open<>nil
then Result:=_pm_open(pm, serial)
else Result:=-1;
end;

function pm_close(var pm:pm_context):Integer;
begin
if @_pm_close<>nil
then Result:=_pm_close(pm)
else Result:=-1;
end;

function pm_reset(var pm:pm_context):Integer;
begin
if @_pm_reset<>nil
then Result:=_pm_reset(pm)
else Result:=-1;
end;

function pm_identify(var pm:pm_context; buffer:PChar):Integer;
begin
if @_pm_identify<>nil
then Result:=_pm_identify(pm, buffer)
else Result:=-1;
end;

function pm_set_frequency(var pm:pm_context; freq:Int64):Integer;
begin
if @_pm_set_frequency<>nil
then Result:=_pm_set_frequency(pm, freq)
else Result:=-1;
end;

function pm_set_averages(var pm:pm_context; avg:Word):Integer;
begin
if @_pm_set_averages<>nil
then Result:=_pm_set_averages(pm, avg)
else Result:=-1;
end;

function pm_measure(var pm:pm_context; var power:single):Integer;
begin
if @_pm_measure<>nil
then Result:=_pm_measure(pm, power)
else Result:=-1;
end;

function pm_blink (var pm:pm_context):Integer;
begin
if @_pm_blink<>nil
then Result:=_pm_blink(pm)
else Result:=-1;
end;

function pm_get_error_string(var pm:pm_context):PChar;
begin
if @_pm_get_error_string<>nil
then Result:=_pm_get_error_string(pm)
else Result:='';
end;

function pm_library_version():PChar;
begin
if @_pm_library_version<>nil
then Result:=_pm_library_version()
else Result:='';
end;


begin
Handle  := 0;

end.
