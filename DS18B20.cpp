#include "DS18B20.h"
#include "Configuration.h"
#include "DallasTemperature.h"

DS18B20::DS18B20()
{
	oneWire = new OneWire(ONE_WIRE_BUS);
	tempDevice = new DallasTemperature(oneWire);
	tempDevice->begin();

	DEBUG_PRINTLN("DS18B20 initialized.");
}

DeviceErrorCode DS18B20::read(String& data)
{
	DEBUG_PRINTLN("Reading DS18B20");

	float temp = DEVICE_DISCONNECTED_F;
	int counter = 0;
	while (counter < 20)
	{
		tempDevice->requestTemperatures();
		temp = tempDevice->getTempFByIndex(0);

		if (temp < -100.0)
		{
			DEBUG_PRINTLN("Temp: " + String(temp));
			delay(100);
		}
		else
		{
			DEBUG_PRINTLN("Temp found.");
			break;
		}

		counter++;
	}

	data = String(temp);
	DEBUG_PRINTLN("Data: " + String(data));
	if (temp == DEVICE_DISCONNECTED_F)
	{
		return ERROR_READ;
	}

	return DEVICE_OK;
}

DeviceErrorCode DS18B20::readAll(String& data)
{
	String reading;
	auto result = read(reading);
	data = "Temp reading: " + String(reading);
	return result;
}

void DS18B20::createEmailMessage(String& subject, String& message, DeviceMessageType deviceMessageType)
{
	subject = String(Configuration::Instance()->deviceName);
	if (deviceMessageType == MAX_RANGE)
	{
		subject += String(" MAX Temp Warning");
		message = String(Configuration::Instance()->deviceName) + String("  has triggered the maximum temperature range.");
	}
	else if (deviceMessageType == MIN_RANGE)
	{
		subject += String(" MIN Temp Warning");
		message = String(Configuration::Instance()->deviceName) + String("  has triggered the minimum temperature range.");
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

String DS18B20::getName()
{
	return "DS18B20";
}

String DS18B20::indexComponent()
{
	return "";
}