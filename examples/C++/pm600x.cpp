#include "pm600x.h"

PM600x::PM600x(unsigned long serial)
{
	isOpen = false;
	pm_init(&pm);

	if (pm_open(&pm, serial) == 0)
		isOpen = true;
}

PM600x::~PM600x()
{
	if (isOpen)
		pm_close(&pm);
}

PM600x* PM600x::getDevice(unsigned long serial)
{
	PM600x *dev =  new PM600x(serial);

	if (!dev)
		return NULL;

	return dev->isOpen ? dev : NULL;
}

std::list<struct PM600x::device> PM600x::findDevices(Type type)
{
	struct pm_list *list;
	struct pm_context pm;
	std::list<device> devList;

	unsigned long pm_type;
	switch (type) {
	case PM_6003: pm_type = TYPE_PM_6003; break;
	case PM_6006: pm_type = TYPE_PM_6006; break;
	default:      pm_type = TYPE_PM_ALL; break;
	}

	pm_find_all(&pm, pm_type, &list);
	while (list != NULL) {
		struct device dev;
		dev.serial = list->serial;
		dev.type = list->type;
		devList.push_back(dev);
		list = list->next;
	}
	pm_list_free(&list);
	return devList;
}

float PM600x::measure()
{
	float meas;
	pm_measure(&pm, &meas);
	return meas;
}

void PM600x::blink()
{
	pm_blink(&pm);
}

bool PM600x::setFrequency(uint64_t freq)
{
	return pm_set_frequency(&pm, freq) == 0;
}

bool PM600x::setAverage(uint16_t avg)
{
	return pm_set_averages(&pm, avg) == 0;
}

PM600x::Type PM600x::type()
{
	unsigned long t;
	pm_type(&pm, &t);
	switch (t) {
	case TYPE_PM_6003:
		return PM_6003;
	case TYPE_PM_6006:
		return PM_6006;
	default:
		return PM_UNKNOWN;
	}
}

const char* PM600x::idString()
{
	pm_identify(&pm, id);
	return id;
}

void PM600x::reset()
{
	pm_reset(&pm);
}



