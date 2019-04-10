#ifndef analog_h
#define analog_h

#include "IDevice.h"

class Analog : public IDevice
{
public:
	Analog();
	DeviceErrorCode read(String& data) override;
	DeviceErrorCode readAll(String& data) override;
	void createEmailMessage(String& subject, String& message, DeviceMessageType deviceMessageType) override;
	String getName() override;
	String indexComponent() override;
	bool switchDevice(const String& value) override { };

	void setPin(uint8_t pin);

private:
	uint8_t _pin = A0;

};

#endif // analog_h