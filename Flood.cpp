#include "Flood.h"
#include "Configuration.h"

Flood::Flood()
{
}

DeviceErrorCode Flood::read(String& data)
{
	auto sensorValue = analogRead(_pin);
	data = String(sensorValue);
	return DEVICE_OK;
}

DeviceErrorCode Flood::readAll(String& data)
{
	auto errorCode = read(data);
	data += String(" Max: ") + Configuration::Instance()->maxRange;
	data += String(" Min: ") + Configuration::Instance()->minRange;
	return errorCode;
}

void Flood::setPin(uint8_t pin)
{
	_pin = pin;
}

void Flood::createEmailMessage(String& subject, String& message, DeviceMessageType deviceMessageType)
{
	auto name = Configuration::Instance()->deviceName;
	if (name == "")
	{
		name = "Flood Sensor";
	}

	subject = name;
	if (deviceMessageType == MAX_RANGE)
	{
		subject += String(" Warning");
		message = String(name) + String(" has triggered a warning on the flood sensor.");
	}
	else if (deviceMessageType == MIN_RANGE)
	{
		subject += String(" Warning");
		message = String(name) + String(" has triggered a flood warning.");
	}
	else if (deviceMessageType == OFF_LINE)
	{
		subject += String(" Offline Warning");
		message = String(name) + String(" is now offline.");
	}
	else if (deviceMessageType == ON_LINE)
	{
		subject += String(" Online");
		message = String(name) + String(" is now online.");
	}
	else if (deviceMessageType == DISCONNECTED)
	{
		subject += String(" Disconnected Warning");
		message = String(name) + String(" sensor is disconnected.");
	}
}

String Flood::getName()
{
	return "Flood";
}

String Flood::indexComponent()
{
	return "";
}