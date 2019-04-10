#ifndef ds18b20_h
#define ds18b20_h

#include "IDevice.h"

class OneWire;
class DallasTemperature;

// Data wire is plugged into pin D1 on the ESP8266 12-E - GPIO 5
#define ONE_WIRE_BUS 5

class DS18B20 : public IDevice
{
public:
	DS18B20();
	DeviceErrorCode read(String& data) override;
	DeviceErrorCode readAll(String& data) override;
	void createEmailMessage(String& subject, String& message, DeviceMessageType deviceMessageType) override;
	String getName() override;
	String indexComponent() override;
	bool switchDevice(const String& value) override { };

	void setPin(uint8_t pin);

private:
	OneWire* oneWire;
	DallasTemperature* tempDevice;
	char temperatureFString[6];

};

#endif // ds18b20_h