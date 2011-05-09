using System;

namespace PM600x
{
    unsafe sealed public class PM600x_Declarations 
    { 
		public struct pm_context
		{
			void *handle;
			UInt32 type;
			UInt64 frequency;
			UInt16 averages;
			sbyte *error_str;
		}
		
		[System.Runtime.InteropServices.DllImport("libpm600x.dll")]
        public static extern int pm_init(out pm_context pm); 

        [System.Runtime.InteropServices.DllImport("libpm600x.dll")]
        public static extern int pm_open(out pm_context pm, UInt32 serial); 

        [System.Runtime.InteropServices.DllImport("libpm600x.dll")]
        public static extern int pm_close(out pm_context pm); 

        [System.Runtime.InteropServices.DllImport("libpm600x.dll")]
        public static extern int pm_reset(out pm_context pm); 

        [System.Runtime.InteropServices.DllImport("libpm600x.dll")]
        public static extern int pm_blink(out pm_context pm); 

        [System.Runtime.InteropServices.DllImport("libpm600x.dll")]
        public static extern int pm_type(out pm_context pm, out UInt32 type); 

        [System.Runtime.InteropServices.DllImport("libpm600x.dll")]
        public static extern int pm_set_frequency(out pm_context pm, UInt64 freq);
        
        [System.Runtime.InteropServices.DllImport("libpm600x.dll")]
        public static extern int pm_set_averages(out pm_context pm, UInt16 avg);
        
        [System.Runtime.InteropServices.DllImport("libpm600x.dll")]
        public static extern int pm_measure(out pm_context pm, out float power);
        
        [System.Runtime.InteropServices.DllImport("libpm600x.dll")]
        unsafe public static extern sbyte* pm_get_error_string(out pm_context pm);  
        
        [System.Runtime.InteropServices.DllImport("libpm600x.dll")]
        unsafe public static extern sbyte* pm_library_version();   
    }
} 
