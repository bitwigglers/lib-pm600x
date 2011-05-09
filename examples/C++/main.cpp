#include <iostream>
#include "pm600x.h"

using namespace std;

int main(int argc, char *argv[])
{
	/* Find all probes on the USB bus and store them in a device list.
	 * We use the static function PM600x::findDevices() for this purpose */
	list<PM600x::device> deviceList;
	deviceList = PM600x::findDevices(PM600x::PM_ALL);

	if (deviceList.empty()) {
		cerr << "No power meter probes found!" << endl;
		return -1;
	}

	/* print out all devices with serial number and device type */
	list<PM600x::device>::iterator it;
	for (it = deviceList.begin() ; it != deviceList.end(); it++)
		cout << (*it).serial << "\t0x" << hex << (*it).type << dec << endl;

	/* Create a PM600x object from the first device in the list.
	 * The class PM600x does not offer a public constructor.
	 * Instead, we use the static function PM600x::getDevice()
	 * to create an PM600x object from a specific serial number */
	PM600x *probe = PM600x::getDevice(deviceList.front().serial);
	if (!probe) {
		cerr << "Failed to create probe object!" << endl;
		return -1;
	}

	/* print the probes id string */
	cout << probe->idString();

	/* blink the probes green led */
	probe->blink();

	/* set the frequency of the probe to 1 GHz */
	probe->setFrequency(1e9);

	/* set avergaes to 10 measurements */
	probe->setAverage(10);

	/* measure the power */
	float power = probe->measure();
	cout << "Power:\t" << power << " dBm" << endl;

	return 0;
}
