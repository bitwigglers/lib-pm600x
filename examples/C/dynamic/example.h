#ifndef _EXAMPLE_H_
#define _EXAMPLE_H_

typedef __stdcall int (*pm_init_func)(struct pm_context*);
typedef __stdcall int (*pm_find_all_func)(struct pm_context*, unsigned long, struct pm_list**);
typedef __stdcall void (*pm_list_free_func)(struct pm_list**);
typedef __stdcall int (*pm_reset_func)(struct pm_context*);
typedef __stdcall int (*pm_identify_func)(struct pm_context*, char*);
typedef __stdcall int (*pm_type_func)(struct pm_context*, unsigned long*);
typedef __stdcall int (*pm_open_func)(struct pm_context*, unsigned long);
typedef __stdcall int (*pm_close_func)(struct pm_context*);
typedef __stdcall int (*pm_measure_func)(struct pm_context*, float*);
typedef __stdcall int (*pm_set_frequency_func)(struct pm_context*, unsigned long long);
typedef __stdcall int (*pm_set_averages_func)(struct pm_context*, unsigned char);
typedef __stdcall int (*pm_blink_func)(struct pm_context*);
typedef __stdcall char* (*pm_get_error_string_func)(struct pm_context*);
typedef __stdcall char* (*pm_library_version_func)(void);

#endif //_EXAMPLE_H_
