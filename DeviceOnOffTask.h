#ifndef deviceonofftask_h
#define deviceonofftask_h

#include "ITask.h"

class DeviceOnOffTask : ITask
{
public:
	DeviceOnOffTask();

	// No arguments required
	bool run(const String& arg) override;
};

#endif // deviceonofftask_h
