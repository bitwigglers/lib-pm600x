#ifndef _LIBPM600X_H_
#define _LIBPM600X_H_

#ifdef BUILD_DLL
/* DLL export */
#define PM600X_EXPORT __stdcall __declspec(dllexport)
#else
/* Exe import */
#define PM600X_EXPORT __stdcall __declspec(dllimport)
#endif

#define TYPE_PM_6006 0x1f180400ul
#define TYPE_PM_6003 0x1f180401ul
#define TYPE_PM_ALL  0x0ul

/**
    \brief Main context structure for all pm functions.

    Do not access directly if possible.
*/
struct pm_context {
    /// power meters device handle
    void *handle;

    /// the type of power meter
    unsigned long type;

    /// the frequency at which we measure in Hz
    unsigned long long frequency;

    /// the amount of measurements that are averaged before result is returned
    unsigned short averages;

    /// String representation of last error
    char *error_str;
};


/**
	\brief List of power meter probes created by pm_find_all().
*/
struct pm_list {
	/// pointer to next entry
	struct pm_list *next;
	/// serial number of the found probe
	unsigned long serial;
	/// the power meter type
	unsigned long type;
};


#ifdef __cplusplus
extern "C" {
#endif

PM600X_EXPORT int pm_init(struct pm_context *pm);
PM600X_EXPORT int pm_find_all(struct pm_context *pm, unsigned long type, struct pm_list **list);
PM600X_EXPORT void pm_list_free(struct pm_list **list);
PM600X_EXPORT int pm_open(struct pm_context *pm, unsigned long serial);
PM600X_EXPORT int pm_close(struct pm_context *pm);
PM600X_EXPORT int pm_reset(struct pm_context *pm);
PM600X_EXPORT int pm_identify(struct pm_context *pm, char *buf);
PM600X_EXPORT int pm_type(struct pm_context *pm, unsigned long *type);
PM600X_EXPORT int pm_set_frequency(struct pm_context *pm, unsigned long long freq);
PM600X_EXPORT int pm_set_averages(struct pm_context *pm, unsigned short avg);
PM600X_EXPORT int pm_measure(struct pm_context *pm, float *power);
PM600X_EXPORT int pm_blink(struct pm_context *pm);
PM600X_EXPORT char *pm_get_error_string (struct pm_context *pm);
PM600X_EXPORT char *pm_library_version(void);
/// @cond INTERN
PM600X_EXPORT int pm_measure_power_channel(struct pm_context *pm, float *power, int channel);
PM600X_EXPORT int pm_measure_voltage_channel(struct pm_context *pm, float *power, int channel);
PM600X_EXPORT int pm_measure_raw_channel(struct pm_context *pm, unsigned long *raw_value, int channel);
PM600X_EXPORT int pm_measure_vsupply(struct pm_context *pm, float *value);
/// @endcond

#ifdef __cplusplus
}
#endif

#endif //_LIBPM600X_H_

