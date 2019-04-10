#ifndef dummy_h
#define dummy_h

#include "IDevice.h"

class Dummy : public IDevice
{
public:
	Dummy();
	DeviceErrorCode read(String& data) override;
	DeviceErrorCode readAll(String& data) override;
	void createEmailMessage(String& subject, String& message, DeviceMessageType deviceMessageType) override;
	String getName() override;
	String indexComponent() override;
	bool switchDevice(const String& value) override { };
};

#endif // dummy_h