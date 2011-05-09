namespace test

import System
import System.Runtime.InteropServices

struct pm_context:
    handle as int
    type as UInt32
    frequency as UInt64
    averages as UInt16
    error_str as IntPtr	


[DllImport("libpm600x.dll")]
def pm_init(ref pm as pm_context) as int:
	pass	

[DllImport("libpm600x.dll")]
def pm_open(ref pm as pm_context, serial as UInt32) as int:
	pass	
	
[DllImport("libpm600x.dll")]
def pm_close(ref pm as pm_context) as int:
	pass	

[DllImport("libpm600x.dll")]
def pm_set_frequency(ref pm as pm_context, freq as UInt64) as int:
	pass	

[DllImport("libpm600x.dll")]
def pm_set_averages(ref pm as pm_context, avg as UInt16) as int:
	pass	

[DllImport("libpm600x.dll")]
def pm_measure(ref pm as pm_context, ref power as single) as int:
	pass	


