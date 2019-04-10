#ifndef motion501_h
#define motion501_h

#include "IDevice.h"

class Motion501 : public IDevice
{
public:
	Motion501();
	DeviceErrorCode read(String& data) override;
	DeviceErrorCode readAll(String& data) override;
	void createEmailMessage(String& subject, String& message, DeviceMessageType deviceMessageType);
	String getName() override;
	String indexComponent() override;
	bool switchDevice(const String& value) override { };

private:
	void begin();
};

#endif // motion501_h