#include <windows.h>
#include <stdio.h>
#include <libpm600x.h>
#include <example.h>

int main (void)
{
	struct pm_context pm;
	struct pm_list *list, *curdev;
	int ret;

	/* A pointer to a function */
	pm_init_func pm_init;
	pm_find_all_func pm_find_all;
	pm_list_free_func pm_list_free;
	pm_reset_func pm_reset;
	pm_identify_func pm_identify;
	pm_type_func pm_type;
	pm_open_func pm_open;
	pm_close_func pm_close;
	pm_measure_func pm_measure;
	pm_set_frequency_func pm_set_frequency;
	pm_set_averages_func pm_set_averages;
	pm_get_error_string_func pm_get_error_string;
	pm_library_version_func pm_library_version;
	pm_blink_func pm_blink;

	/* Windows handle */
	HANDLE handle;

	/* Load the library */
	handle = LoadLibrary("libpm600x.dll");
	if (!handle) {
		printf("Error: can not load library libpm600x.dll\n");
		return -1;
	}

	/* Get the proc addresses of the library */
	pm_init = (pm_init_func)GetProcAddress(handle, "pm_init");
	pm_find_all = (pm_find_all_func)GetProcAddress(handle, "pm_find_all");
	pm_list_free = (pm_list_free_func)GetProcAddress(handle, "pm_list_free");
	pm_reset = (pm_reset_func)GetProcAddress(handle, "pm_reset");
	pm_identify = (pm_identify_func)GetProcAddress(handle, "pm_identify");
	pm_type = (pm_type_func)GetProcAddress(handle, "pm_type");
	pm_open = (pm_open_func)GetProcAddress(handle, "pm_open");
	pm_close = (pm_close_func)GetProcAddress(handle, "pm_close");
	pm_measure = (pm_measure_func)GetProcAddress(handle, "pm_measure");
	pm_set_frequency = (pm_set_frequency_func)GetProcAddress(handle, "pm_set_frequency");
	pm_set_averages = (pm_set_averages_func)GetProcAddress(handle, "pm_set_averages");
	pm_get_error_string = (pm_get_error_string_func)GetProcAddress(handle, "pm_get_error_string");
	pm_library_version = (pm_library_version_func)GetProcAddress(handle, "pm_library_version");
	pm_blink = (pm_blink_func)GetProcAddress(handle, "pm_blink");

	if (!pm_init || !pm_find_all || !pm_list_free || !pm_reset || !pm_identify || !pm_type ||
	    !pm_open || !pm_close || !pm_measure || !pm_set_frequency || !pm_set_averages ||
	    !pm_get_error_string || !pm_library_version || !pm_blink)
	{
		FreeLibrary(handle);
		printf ("Error: Can not get Proc Addesses!");
		return -1;
	}

	printf("Example program for libpm600x %s\n", pm_library_version());
	printf("-----------------------------------\n\n");

	// initialise our pm_context
	pm_init(&pm);

	// find all power meter probes on the bus
	ret = pm_find_all(&pm, TYPE_PM_ALL, &list);
	if (!list) {
		printf("Error: No powermeter probes found\n");
		FreeLibrary(handle);
		return -1;
	}

	// list all probes that we have found
	for (curdev = list; curdev != NULL ;) {
		if (curdev->type == TYPE_PM_6006)
			printf("found PM 6006 with serial number %lu\n", curdev->serial);
		else if (curdev->type == TYPE_PM_6003)
			printf("found PM 6003 with serial number %lu\n", curdev->serial);
		curdev = curdev->next;
	}

	// open the first power meter that was found
	ret = pm_open(&pm, list->serial);
	if (ret < 0) {
		printf("Error: %i \"%s\"\n", ret, pm_get_error_string(&pm));
		pm_list_free(&list);
		FreeLibrary(handle);
		return -1;
	}

	// blink the probes green led
	ret = pm_blink(&pm);
	if (ret < 0) {
		printf("Error %i, \"%s\"\n", ret, pm_get_error_string(&pm));
		pm_list_free(&list);
		FreeLibrary(handle);
		return -1;
	}

	// set frequency
	ret = pm_set_frequency(&pm, 1000000000ULL);
	if (ret < 0) {
		printf("Error %i, \"%s\"\n", ret, pm_get_error_string(&pm));
		pm_list_free(&list);
		FreeLibrary(handle);
		return -1;
	}

	// set amount of averaging measurements to 255
	ret = pm_set_averages(&pm, 255);
	if (ret < 0) {
		printf("Error %i, \"%s\"\n", ret, pm_get_error_string(&pm));
		pm_list_free(&list);
		FreeLibrary(handle);
		return -1;
	}

	// measure
	float power = 0;
	ret = pm_measure(&pm, &power);
	if (ret < 0) {
		printf("Error %i, \"%s\"\n", ret, pm_get_error_string(&pm));
		pm_list_free(&list);
		FreeLibrary(handle);
		return -1;
	}

	printf("Power: %.2f dBm\n", power);

	// clean up
	pm_close(&pm);
	pm_list_free(&list);
	FreeLibrary(handle);

	return 0;
}
