#ifndef bool_h
#define bool_h

#include "IDevice.h"

class Bool : public IDevice
{
public:
	Bool();
	DeviceErrorCode read(String& data) override;
	DeviceErrorCode readAll(String& data) override;
	void createEmailMessage(String& subject, String& message, DeviceMessageType deviceMessageType);
	String getName() override;
	String indexComponent() override;
	bool switchDevice(const String& value) override { };

private:
	void begin();
};

#endif // bool_h