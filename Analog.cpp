#include "Analog.h"
#include "Configuration.h"

Analog::Analog()
{
}

DeviceErrorCode Analog::read(String& data)
{
	auto sensorValue = analogRead(_pin);
	data = String(sensorValue);
	return DEVICE_OK;
}

DeviceErrorCode Analog::readAll(String& data)
{
	auto errorCode = read(data);
	data += String(" Max: ") + Configuration::Instance()->maxRange;
	data += String(" Min: ") + Configuration::Instance()->minRange;
	return errorCode;
}

void Analog::setPin(uint8_t pin)
{
	_pin = pin;
}

void Analog::createEmailMessage(String& subject, String& message, DeviceMessageType deviceMessageType)
{
	subject = String(Configuration::Instance()->deviceName);
	if (deviceMessageType == MAX_RANGE)
	{
		subject += String(" MAX Warning");
		message = String(Configuration::Instance()->deviceName) + String("  has triggered the maximum range.");
	}
	else if (deviceMessageType == MIN_RANGE)
	{
		subject += String(" MIN Warning");
		message = String(Configuration::Instance()->deviceName) + String("  has triggered the minimum range.");
	}
	else if (deviceMessageType == OFF_LINE)
	{
		subject += String(" Offline Warning");
		message = String(Configuration::Instance()->deviceName) + String(" is now offline.");
	}
	else if (deviceMessageType == ON_LINE)
	{
		subject += String(" Online");
		message = String(Configuration::Instance()->deviceName) + String(" is now online.");
	}
	else if (deviceMessageType == DISCONNECTED)
	{
		subject += String(" Disconnected Warning");
		message = String(Configuration::Instance()->deviceName) + String(" sensor is disconnected.");
	}
}

String Analog::getName()
{
	return "Analog";
}

String Analog::indexComponent()
{
	return "";
}