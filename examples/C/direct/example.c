#include <libpm600x.h>
#include <stdio.h>

int main(void)
{
	struct pm_context pm;
	float power;
	int ret;

	printf("Example program for libpm600x %s\n", pm_library_version());
	printf("-----------------------------------\n\n");

	// initialise the pm_context we want to use
	ret = pm_init(&pm);

	// find all power meter probes on the bus
	struct pm_list *list, *curdev;
	ret = pm_find_all(&pm, TYPE_PM_ALL, &list);
	if (!list) {
		printf("No powermeter probes found\n");
		return -1;
	}

	// list all probes that we found
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
		printf("Error %i, \"%s\"\n", ret, pm_get_error_string(&pm));
		return -1;
	}

	// set frequency to 1 GHz
	ret = pm_set_frequency(&pm, 1000000000ULL);
	if (ret < 0) {
		printf("Error %i, \"%s\"\n", ret, pm_get_error_string(&pm));
		return -1;
	}

	// set amount of averaging measurements to 255
	ret = pm_set_averages(&pm, 255);
	if (ret < 0) {
		printf("Error %i, \"%s\"\n", ret, pm_get_error_string(&pm));
		return -1;
	}

	// measure the power
	ret = pm_measure(&pm, &power);
	if (ret < 0) {
		printf("Error %i, \"%s\"\n", ret, pm_get_error_string(&pm));
		return -1;
	}

	printf("Power: %.2f dBm\n", power);

	// close the device
	pm_close(&pm);

	// we do not need the device list anymore
	pm_list_free(&list);

	return 0;
}
