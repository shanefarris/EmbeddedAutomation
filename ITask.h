#ifndef itask_h
#define itask_h

#include <Arduino.h>

// Turn This Device On
// Turn This Device Off
// Reboot This Device
// Turn a Remote Device On
// Turn a Remote Device Off
// Send a Custom Value to a Remote Device
// Log Value
// Send Email Alert
// Send SMS Alert
// Send GET Request

// EventType = 3, Command Type = 4, arg = 192.168.2.121
// event3:4;192.168.2.121

enum EventType
{
	MIN_RANGE = 1,
    MAX_RANGE = 2,
    DEVICE_START = 3,
    DEVICE_LOOP = 4,
    READ_SUCCESS = 5,
    READ_FAIL = 6
};

class ITask
{
public:
	ITask() { }
	virtual bool run(const String& arg) = 0;
};

#endif // itask_h
