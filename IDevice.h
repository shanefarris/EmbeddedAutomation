#ifndef idevice_h
#define idevice_h

#include "Configuration.h"

enum DeviceErrorCode
{
	DEVICE_OK = 0,
	ERROR_CHECKSUM = -1,
	ERROR_TIMEOUT = -2,
	ERROR_READ = -3,
	INVALID_VALUE = -999
};

enum DeviceMessageType
{
	MAX_RANGE,
	MIN_RANGE,
	OFF_LINE,
	ON_LINE,
	CONNECT,
	DISCONNECTED
};

class IDevice
{
public:
	IDevice() { }
	virtual DeviceErrorCode read(String& data) = 0;
	virtual DeviceErrorCode readAll(String& data) = 0;
	virtual void createEmailMessage(String& subject, String& message, DeviceMessageType deviceMessageType);
	virtual String getName() = 0;
	virtual String indexComponent() = 0;
	virtual bool switchDevice(const String& value) = 0;
};

#endif // idevicereader_h