#ifndef PM600X_H
#define PM600X_H

#include "libpm600x.h"
#include <list>
#include <stdint.h>

class PM600x
{
public:
	struct device {
		unsigned long serial;
		unsigned long type;
	};

	enum Type{PM_6003 = TYPE_PM_6003,
			  PM_6006 = TYPE_PM_6006,
			  PM_ALL = TYPE_PM_ALL,
			  PM_UNKNOWN};

	static std::list<struct device> findDevices(Type);
	static PM600x* getDevice(unsigned long serial);

	float measure();
	void blink();
	bool setFrequency(uint64_t freq);
	bool setAverage(uint16_t avg);
	void reset();
	const char *idString();
	Type type();

private:
	explicit PM600x(unsigned long serial);
	virtual ~PM600x();
	PM600x(const PM600x&);
	PM600x& operator=(const PM600x&);

	struct pm_context pm;
	bool isOpen;
	char id[50];
};

#endif // PM600X_H
