#include "Dummy.h"

Dummy::Dummy()
{
}

DeviceErrorCode Dummy::read(String& data)
{
	data = "1025";
	return DEVICE_OK;
}

DeviceErrorCode Dummy::readAll(String& data)
{
	String reading;
	auto result = read(reading);
	data = "TEST reading: " + String(reading);
	return result;
}

void Dummy::createEmailMessage(String& subject, String& message, DeviceMessageType deviceMessageType)
{
	subject = String("Test Device ");
	if (deviceMessageType == MAX_RANGE)
	{
		subject += String("MAX Warning");
		message = String("The test device has triggered the maximum range.");
	}
	else if (deviceMessageType == MIN_RANGE)
	{
		subject += String("MIN Warning");
		message = String("The test device has triggered the minimum range.");
	}
	else if (deviceMessageType == OFF_LINE)
	{
		subject += String("Offline Warning");
		message = String("The test device is now offline.");
	}
	else if (deviceMessageType == ON_LINE)
	{
		subject += String("Online");
		message = String("The test device is now online.");
	}
	else if (deviceMessageType == DISCONNECTED)
	{
		subject += String("Disconnected Warning");
		message = String("The test device's sensor is disconnected.");
	}
}

String Dummy::getName()
{
	return "Dummy";
}

String Dummy::indexComponent()
{
	return "";
}