#ifndef dht11_h
#define dht11_h

#include "IDevice.h"

#define PIN 5
#define MIN_INTERVAL 2000

class Dht11 : public IDevice
{
public:
	Dht11();
	DeviceErrorCode read(String& data) override;
	DeviceErrorCode readAll(String& data) override;
	void createEmailMessage(String& subject, String& message, DeviceMessageType deviceMessageType);
	String getName() override;
	String indexComponent() override;
	bool switchDevice(const String& value) override { };

	float convertCtoF(const float& c);
	float convertFtoC(const float& f);
	float computeHeatIndex(const float& temperature, const float& percentHumidity);

	float readHumidity();
	float readTemperature();

private:
	void begin();
	bool _read();
	
	uint8_t data[5];
	uint32_t _lastreadtime, _maxcycles;
	bool _lastresult;

	uint32_t expectPulse(bool level);


};

#endif // dht11_h