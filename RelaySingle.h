#ifndef relay_single_h
#define relay_single_h

#include "IDevice.h"

class RelaySingle : public IDevice
{
private:
	bool isOpen;
	int inputPin = D1;

public:
	RelaySingle();
	DeviceErrorCode read(String& data) override;
	DeviceErrorCode readAll(String& data) override;
	void createEmailMessage(String& subject, String& message, DeviceMessageType deviceMessageType) override;
	String getName() override;
	String indexComponent() override;
	bool switchDevice(const String& value) override;
};

#endif // relay_single_h