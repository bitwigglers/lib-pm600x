/**
* \mainpage PMU 600x Application Programmers Interface
*
* The Teseq PMU 600x power meter probes can be used by either interfacing to the virtual COM port
* directly using the documented remote commands, or by using a library that abstracts many of
* the nasty details and reduces complexity. For the latter, Teseq provides the libpm600x
* library.
*
* The libpm600x library makes it easy for you to interface to the PMU 600x using your favourite
* programming language. It abstracts the hardware dependent part for you. You do not need to care
* for the virtual COM ports or the devices remote commands. All you have to do is calling a few
* library functions for setup and then call pm_measure() to get the results.
*
* With libpm600x it is easy to handle multiple power meter probes simultaneously. Most of the
* library functions take a power meter context pm_context as their first parameter. The
* pm_context is used to distinguish the different probes. It is used to store probe dependent parts
* like the current frequency and average settings. You have to create a pm_context for each
* power meter you want to handle.
*
* You can scan the USB bus for PMU 600x power meter probes with the function pm_find_all(). This
* function returns a linked list of probes that were found on the USB bus. Each list entry contains
* the serial number and the device type of the probe. Additionally it contains a pointer to
* the next entry. The entries are represented by struct pm_list. You can traverse the list by
* following the <em>next</em>-pointer of struct pm_list. The end of the list is reached if the
* <em>next</em>-pointer becomes NULL.
*
* After scanning the USB bus with pm_find_all() you have to create a pm_context for each probe
* you want to use and initialise it with pm_init(). Once this is done you can open the device
* using pm_open(). This function takes a pm_context and a serial number as arguments.
* It opens and initialises the power meter probe with the specified serial number.
*
* After opening the probe with pm_open() you can set the probes frequency and averaging count
* and you can retrive results using pm_measure(). Once you are done with your measurements
* you call pm_close(). If you used pm_find_all() to create a list of devices, you have to
* call pm_list_free() to free the menmory that was allocated by the list.
*
* The following image shows the neccessary steps in an overview.
* \image html overview.png
*
* Here is a simple example in C that searches the bus and opens the first device it finds.
*
* \include example.c
*/

#include <windows.h>
#include "libpm600x.h"
#include "ftd2xx.h"
#include <stdio.h>

#define pm_error_return(code, str) do {  \
	pm->error_str = str;             \
	return code;                     \
	} while(0);

#define LIBRARY_VERSION "V1.0"

static int pm_write(struct pm_context *pm, char *buffer, unsigned int count)
{
	FT_STATUS ftstat;
	DWORD bytes_written;

	ftstat = FT_Write(pm->handle, buffer, count, &bytes_written);
	return ftstat == FT_OK ? bytes_written : -1;
}

static int pm_read(struct pm_context *pm, char *buffer, unsigned int count)
{
	FT_STATUS ftstat;
	DWORD bytes_read;

	ftstat = FT_Read(pm->handle, buffer, count, &bytes_read);

	if (ftstat == FT_OK && count == bytes_read)
		return bytes_read;

	return -1;
}

static int pm_read_line(struct pm_context *pm, char* buffer, int max_length)
{
	char c = 0;
	int nread = 0, ret;
	*buffer = 0;

	while ((c != '\n') && (nread < max_length - 1)) {
		ret = pm_read(pm, &c, 1);
		if (ret != 1)
			return ret;

		nread++;
		*buffer = c;
		buffer++;
	}
	*buffer = '\0';
	return nread;
}

/**
    Initializes a pm_context.

    After initialising you can pass the pm_context to
    pm_open() to open a power meter device.

    \param pm pointer to pm_context

    \retval  0 - all fine

    \remark This should be called before all functions
*/
PM600X_EXPORT int pm_init(struct pm_context *pm)
{
	pm->handle = NULL;
	pm->type = -1;
	pm->error_str = NULL;
	pm->frequency = 100000000;
	pm->averages = 1;
	pm_error_return(0, "all fine");
}

/**
	Create a list of all power meter devices on the bus.

	This function finds all power meter probes connected to your
	computer and creates a linked list of the devices. Note, that
	you have to deallocate the list with pm_list_free() after use.

	\param pm pointer to pm_context
	\param type the device type, either TYPE_PM_6006, TYPE_PM_6003 or TYPE_PM_ALL
	\param list a pointer to struct pm_list*, used to create the list

	\retval  0 - all fine
	\retval -1 - device list creation failed
	\retval -2 - out of memory
	\retval -3 - can not retrieve device info
*/
PM600X_EXPORT int pm_find_all(struct pm_context *pm, unsigned long type, struct pm_list **list)
{
	int i;
	FT_STATUS ftstat;
	FT_DEVICE_LIST_INFO_NODE *devInfo = NULL;

	struct pm_list **curdev;
	curdev = list;
	*curdev = NULL;

	/* create device info list and get the number of connected devices */
	unsigned long numDevs = 0;
	ftstat = FT_CreateDeviceInfoList(&numDevs);
	if (ftstat != FT_OK)
		pm_error_return(-1, "creation of device list failed");

	/* get device info */
	if (numDevs > 0) {
		devInfo = (FT_DEVICE_LIST_INFO_NODE*)malloc(numDevs * sizeof(FT_DEVICE_LIST_INFO_NODE));

		if (!devInfo)
			pm_error_return(-2, "out of memory");

		ftstat = FT_GetDeviceInfoList(devInfo,&numDevs);
		if (ftstat != FT_OK)
			pm_error_return(-3, "retrieving device info failed");

		for (i = 0; i < numDevs; i++) {
			/* filter out all devices, that are not a PM 6006 or PM 6003 */
			if ((type == TYPE_PM_ALL) && (devInfo[i].ID != TYPE_PM_6006 && devInfo[i].ID != TYPE_PM_6003))
				continue;
			else if ((type == TYPE_PM_6006) && (devInfo[i].ID != TYPE_PM_6006))
				continue;
			else if ((type == TYPE_PM_6003) && (devInfo[i].ID != TYPE_PM_6003))
				continue;

			*curdev = (struct pm_list*) malloc(sizeof(struct pm_list));
			if (!*curdev)
				pm_error_return(-2, "out of memory");

			(*curdev)->next = NULL;
			(*curdev)->serial = atoi(devInfo[i].SerialNumber);
			(*curdev)->type = devInfo[i].ID;
			curdev = &(*curdev)->next;
		}
	}

	pm_error_return(0, "all fine");
}

/**
	Free a powermeter device list created with pm_find_all.
*/
PM600X_EXPORT void pm_list_free(struct pm_list **list)
{
	struct pm_list *curdev, *next;

	for (curdev = *list; curdev != NULL ;) {
		next = curdev->next;
		free(curdev);
		curdev = next;
	}
	*list = NULL;
}

/**
    Get string representation for last error code

    \param pm a pointer to a pm_context

    \retval Pointer to error string
*/
PM600X_EXPORT char *pm_get_error_string (struct pm_context *pm)
{
	return pm->error_str;
}

/**
    Open a power meter probe by a given serial number

    \param pm a pointer to a pm_context
    \param serial the serial number

    \retval  0 - all fine
    \retval -1 - open failed (wrong serial number?)
    \retval -2 - setting baudrate failed
    \retval -3 - setting data characteristics failed
    \retval -4 - setting flow control failed
    \retval -5 - setting timeouts failed
    \retval -6 - purging buffers failed
    \retval -7 - resetting device failed
*/
PM600X_EXPORT int pm_open(struct pm_context *pm, unsigned long serial)
{
	FT_STATUS ftstat;

	// convert the serial into a 8 digit string with leading zeroes
	char serial_string[12];
	snprintf(serial_string, 12, "%06lu", serial);

	// open the device by a given serial number
	ftstat = FT_OpenEx((void *)serial_string, FT_OPEN_BY_SERIAL_NUMBER, &pm->handle);
	if (ftstat != FT_OK)
		pm_error_return(-1, "open failed (wrong serial number?)");

	// get the device info to obtain the power meter type
	unsigned long id;
	ftstat = FT_GetDeviceInfo(pm->handle, NULL, &id, NULL, NULL, NULL);
	if (ftstat != FT_OK)
		pm_error_return(-8, "can not retrieve device info!");

	pm->type = id;

	// set baud rate to 115200
	ftstat = FT_SetBaudRate(pm->handle, FT_BAUD_115200);
	if (ftstat != FT_OK)
		pm_error_return(-2, "setting baudrate failed");

	// set data characteristics to 8n1
	ftstat = FT_SetDataCharacteristics(pm->handle, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE);
	if (ftstat != FT_OK)
		pm_error_return(-3, "setting data characteristics failed");

	// set flow control to NONE
	ftstat = FT_SetFlowControl(pm->handle, FT_FLOW_NONE, 0, 0);
	if (ftstat != FT_OK)
		pm_error_return(-4, "setting flow control failed");

	// set timeouts to 1 second for writes and 3.5 seconds for reads. This should be enough when measuring with averaging == 10000
	ftstat = FT_SetTimeouts(pm->handle, 3500, 1000);
	if (ftstat != FT_OK)
		pm_error_return(-5, "setting timeouts failed");

	// purge bufffers just in case
	ftstat = FT_Purge(pm->handle, FT_PURGE_RX | FT_PURGE_TX);
	if (ftstat != FT_OK)
		pm_error_return(-6, "purging buffers failed");

	// reset the device via *RST
	return pm_reset(pm);
}

/**
    Reset a power meter probes settings

    \param pm a pointer to a pm_context

    \retval  0 - all fine
    \retval -1 - invalid pm_context
    \retval -2 - resetting device failed
*/
PM600X_EXPORT int pm_reset(struct pm_context *pm)
{
	if (!pm->handle)
		pm_error_return(-1, "invalid pm_context");

	// reset the device via *RST
	int ret = pm_write(pm, "\r\n*RST\r\n", strlen("\r\n*RST\r\n"));
	if (ret < 0)
		pm_error_return(-2, "resetting device failed");

	pm_error_return(0, "all fine");
}

/**
    Close a power meter probe

    \param pm a pointer to a pm_context

    \retval  0 - all fine
    \retval -1 - invalid pm_context
*/
PM600X_EXPORT int pm_close(struct pm_context *pm)
{
	if (!pm->handle)
		pm_error_return(-1, "invalid pm_context");

	FT_Close(pm->handle);

	return pm_init(pm);
}

/**
    Measure the power on a probe

    \param pm a pointer to a pm_context
    \param power a pointer to a float to store the result

    \retval  0 - all fine
    \retval -1 - invalid pm_context
    \retval -2 - sending command failed
    \retval -3 - reading device failed
    \retval -4 - internal conversion error
*/
PM600X_EXPORT int pm_measure(struct pm_context *pm, float *power)
{
	if (!pm->handle)
		pm_error_return(-1, "invalid pm_context");

	// send command
	char cmd[30];
	int count = 0;

	// for PM6006 we only accept int frequencies in MHz
	if (pm->type == TYPE_PM_6006)
		count = sprintf(cmd, "POW? %lu\r\n", (unsigned long)(pm->frequency / 1000000));

	// for PM6003 float values in MHz are allowed
	else if (pm->type == TYPE_PM_6003)
		count = sprintf(cmd, "POW? %f\r\n", (float)pm->frequency / 1000000.0f);

	if (!count)
		pm_error_return(-4, "internal conversion error");

	int ret = pm_write(pm, cmd, count);
	if (ret < 0)
		pm_error_return(-2, "sending command failed");

	// read answer
	char buf[20];
	ret = pm_read_line(pm, buf, 20);
	if (ret < 0)
		pm_error_return(-4, "reading device failed");

	*power = atof(buf);

	pm_error_return(0, "all fine");
}


/**
    Blink the probes green LED for about 6 seconds.

    This helps to identify a specific probe if several of them
    are used. You can easily identify them in your measurement
    setup by blinking the LED.

    \param pm a pointer to a pm_context

    \retval  0 - all fine
    \retval -1 - invalid pm_context
*/
PM600X_EXPORT int pm_blink(struct pm_context *pm)
{
	if (!pm->handle)
		pm_error_return(-1, "invalid pm_context");

	// send blink command
	char *cmd = "BLINK\r\n";
	int ret = pm_write(pm, cmd, strlen(cmd));
	if (ret < 0)
		pm_error_return(-2, "sending command failed");

	pm_error_return(0, "all fine");
}

/**
    Retrieve the power meters device type.

    This functions returns the device type of the probe, e.g. TYPE_PM_6006.

    \param pm a pointer to a pm_context
    \param type a pointer to a unsigned long to store the device type

    \retval  0 - all fine
    \retval -1 - invalid pm_context
*/
PM600X_EXPORT int pm_type(struct pm_context *pm, unsigned long *type)
{
	if (!pm->handle)
		pm_error_return(-1, "invalid pm_context");

	*type = pm->type;
	pm_error_return(0, "all fine");
}

/**
    Returns identification string of a probe

    \param pm a pointer to a pm_context
    \param buf a paointer to a buffer where to store the id string

    \retval  0 - all fine
    \retval -1 - invalid pm_context
    \retval -2 - sending command failed
    \retval -3 - reading device failed
*/
PM600X_EXPORT int pm_identify(struct pm_context *pm, char *buf)
{
	if (!pm->handle)
		pm_error_return(-1, "invalid pm_context");

	// send command
	int ret = pm_write(pm, "\r\n*IDN?\r\n", strlen("\r\n*IDN?\r\n"));
	if (ret < 0)
		pm_error_return(-2, "sending command failed");

	// read answer
	ret = pm_read_line(pm, buf, 40);
	if (ret <= 0)
		pm_error_return(-3, "reading device failed");

	pm_error_return(0, "all fine");
}

/**
    Set the probes frequency

    \param pm a pointer to a pm_context
    \param freq a 64 bit value representing the frequency in Hz

    \retval  0 - all fine
    \retval -1 - invalid pm_context
    \retval -2 - frequency out of range
*/
PM600X_EXPORT int pm_set_frequency(struct pm_context *pm, unsigned long long freq)
{
	if (!pm->handle)
		pm_error_return(-1, "invalid pm_context");

	switch (pm->type) {
	case TYPE_PM_6003:
		if (freq < 9000ULL || freq > 3000000000ULL)
			pm_error_return(-2, "frequency out of range");
		break;
	case TYPE_PM_6006:
		if (freq < 1000000ULL || freq > 6000000000ULL)
			pm_error_return(-2, "frequency out of range");
		break;
	default:
		pm_error_return(-3, "Unknown device type");
		break;
	}


	pm->frequency = freq;

	pm_error_return(0, "all fine");
}


/**
    Set measurement average count

    Set the count of measurements the device averages before returning a result

    \param pm a pointer to a pm_context
    \param avg the averaging count in the range 1-10000

    \retval  0 - all fine
    \retval -1 - average out of range
    \retval -2 - invalid pm_context
    \retval -3 - setting average failed
    \retval -4 - internal conversion error
*/
PM600X_EXPORT int pm_set_averages(struct pm_context *pm, unsigned short avg)
{
	if (avg < 1)
		pm_error_return(-1, "average out of range");

	if (!pm->handle)
		pm_error_return(-2, "invalid pm_context");

	char cmd[20];
	int count = sprintf(cmd, "AVG %i\r\n", avg);

	if (!count)
		pm_error_return(-4, "internal conversion error");

	int ret = pm_write(pm, cmd, count);
	if (ret < 0)
		pm_error_return(-3, "setting average failed");

	pm->averages = avg;

	pm_error_return(0, "all fine");
}

/**
    Get string representation for the library version

    \retval Pointer to version string
*/
PM600X_EXPORT char *pm_library_version(void)
{
	return LIBRARY_VERSION;
}

/// @cond INTERN
/**
    Measure the power on a probes channel

    \param pm a pointer to a pm_context
    \param power a pointer to a float to store the result

    \retval  0 - all fine
    \retval -1 - invalid pm_context
    \retval -2 - invalid channel selected
    \retval -3 - sending command failed
    \retval -3 - reading device failed
    \retval -4 - internal conversion error
*/
PM600X_EXPORT int pm_measure_power_channel(struct pm_context *pm, float *power, int channel)
{
	if (!pm->handle)
		pm_error_return(-1, "invalid pm_context");

	if (channel < 1 || channel > 2)
		pm_error_return(-2, "invalid channel selected");

	// send command
	char cmd[30];
	int count = 0;

	// for PM6006 we only accept int frequencies in MHz
	if (pm->type == TYPE_PM_6006)
		count = sprintf(cmd, "POW? %lu\r\n", (unsigned long)(pm->frequency / 1000000));

	// for PM6003 float values in MHz are allowed
	else if (pm->type == TYPE_PM_6003)
		count = sprintf(cmd, "POW? %f\r\n", (float)pm->frequency / 1000000.0f);

	if (!count)
		pm_error_return(-4, "internal conversion error");

	int ret = pm_write(pm, cmd, count);
	if (ret < 0)
		pm_error_return(-3, "sending command failed");

	// read answer
	char buf[20];
	ret = pm_read_line(pm, buf, 20);
	if (ret < 0)
		pm_error_return(-4, "reading device failed");

	*power = atof(buf);

	pm_error_return(0, "all fine");
}
/// @endcond

/// @cond INTERN
/**
    Measure the voltage on a probes channel

    \param pm a pointer to a pm_context
    \param power a pointer to a float to store the result

    \retval  0 - all fine
    \retval -1 - invalid pm_context
    \retval -2 - invalid channel selected
    \retval -3 - sending command failed
    \retval -3 - reading device failed
    \retval -4 - internal conversion error
*/
PM600X_EXPORT int pm_measure_voltage_channel(struct pm_context *pm, float *voltage, int channel)
{
	if (!pm->handle)
		pm_error_return(-1, "invalid pm_context");

	if (channel < 1 || channel > 2)
		pm_error_return(-2, "invalid channel selected");

	// send command
	char cmd[30];
	int count = sprintf(cmd, "VOLTCH%i?\r\n", channel);

	if (!count)
		pm_error_return(-4, "internal conversion error");

	int ret = pm_write(pm, cmd, count);
	if (ret < 0)
		pm_error_return(-3, "sending command failed");

	// read answer
	char buf[20];
	ret = pm_read_line(pm, buf, 20);
	if (ret < 0)
		pm_error_return(-4, "reading device failed");

	*voltage = atof(buf);

	pm_error_return(0, "all fine");
}
/// @endcond

/// @cond INTERN
/**
    Measure the raw data on a probes channel

    \param pm a pointer to a pm_context
    \param power a pointer to a float to store the result

    \retval  0 - all fine
    \retval -1 - invalid pm_context
    \retval -2 - invalid channel selected
    \retval -3 - sending command failed
    \retval -3 - reading device failed
    \retval -4 - internal conversion error
*/
PM600X_EXPORT int pm_measure_raw_channel(struct pm_context *pm, unsigned long *raw_value, int channel)
{
	if (!pm->handle)
		pm_error_return(-1, "invalid pm_context");

	if (channel < 1 || channel > 2)
		pm_error_return(-2, "invalid channel selected");

	// send command
	char cmd[30];
	int count = sprintf(cmd, "RAWCH%i?\r\n", channel);

	if (!count)
		pm_error_return(-4, "internal conversion error");

	int ret = pm_write(pm, cmd, count);
	if (ret < 0)
		pm_error_return(-3, "sending command failed");

	// read answer
	char buf[20];
	ret = pm_read_line(pm, buf, 20);
	if (ret < 0)
		pm_error_return(-4, "reading device failed");

	*raw_value = atol(buf) & 0xfff;	// only the lower 12 bits are relevant

	pm_error_return(0, "all fine");
}
/// @endcond

/// @cond INTERN
/**
    Measure the power supply voltage of the probe

    \param pm a pointer to a pm_context
    \param power a pointer to a float to store the result

    \retval  0 - all fine
    \retval -1 - invalid pm_context
    \retval -2 - sending command failed
    \retval -3 - reading device failed
*/
PM600X_EXPORT int pm_measure_vsupply(struct pm_context *pm, float *value)
{
	if (!pm->handle)
		pm_error_return(-1, "invalid pm_context");

	// send command
	char *cmd = "TEMP?\r\n";

	int ret = pm_write(pm, cmd, strlen(cmd));
	if (ret < 0)
		pm_error_return(-2, "sending command failed");

	// read answer
	char buf[20];
	ret = pm_read_line(pm, buf, 20);
	if (ret < 0)
		pm_error_return(-3, "reading device failed");

	*value = atof(buf);

	pm_error_return(0, "all fine");
}
/// @endcond

