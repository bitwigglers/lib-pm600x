unit pm600x;
interface

const
TYPE_PM_6006 = $1f180400;
TYPE_PM_6003 = $1f180401;
TYPE_PM_ALL  = $0;

type

pm_context = record
    handle : Pointer;
    devtype : cardinal;
    frequency : Int64;
    averages : Word;
    error_str : Pointer;
end;

ppm_list = ^pm_list;

pm_list = record
    next : ppm_list;
    serial : cardinal;
    devtype : cardinal;
end;

function pm_init             (var pm:pm_context):Integer; stdcall; external 'libpm600x.dll';
function pm_find_all         (var pm:pm_context; device_type:Cardinal; var list:ppm_list):Integer; stdcall; external 'libpm600x.dll';
procedure pm_list_free       (var list:ppm_list); stdcall; external 'libpm600x.dll';
function pm_open             (var pm:pm_context; serial:LongWord):Integer; stdcall; external 'libpm600x.dll';
function pm_close            (var pm:pm_context):Integer; stdcall; external 'libpm600x.dll';
function pm_reset            (var pm:pm_context):Integer; stdcall; external 'libpm600x.dll';
function pm_identify         (var pm:pm_context; buffer:PChar):Integer; stdcall; external 'libpm600x.dll';
function pm_type             (var pm:pm_context; var devtype:Cardinal):Integer; stdcall; external 'libpm600x.dll';
function pm_set_frequency    (var pm:pm_context; freq:Int64):Integer; stdcall; external 'libpm600x.dll';
function pm_set_averages     (var pm:pm_context; avg:Word):Integer; stdcall; external 'libpm600x.dll';
function pm_measure          (var pm:pm_context; var power:single):Integer; stdcall; external 'libpm600x.dll';
function pm_get_error_string (var pm:pm_context):PChar; stdcall; external 'libpm600x.dll';
function pm_blink            (var pm:pm_context):Integer; stdcall; external 'libpm600x.dll';
function pm_library_version ():PChar; stdcall; external 'libpm600x.dll';

implementation

end.
