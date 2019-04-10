#ifndef hcsr04_h
#define hcsr04_h

#include "IDevice.h"

class Hcsr04 : public IDevice
{
public:
	Hcsr04();
	DeviceErrorCode read(String& data) override;
	DeviceErrorCode readAll(String& data) override;
	void createEmailMessage(String& subject, String& message, DeviceMessageType deviceMessageType);
	String getName() override;
	String indexComponent() override;
	bool switchDevice(const String& value) override { };

private:
	void begin();
	long _read();
	
	long _lastresult;

};

#endif // hcsr04_h