#include "Motion501.h"

#define TRIGGER D1
#define ECHO    D2

Motion501::Motion501()
{
	pinMode(TRIGGER, INPUT);   // declare sensor as input
}

DeviceErrorCode Motion501::read(String& data)
{
	long state = digitalRead(TRIGGER);
	if (state == HIGH) 
	{
		data = "1";
		DEBUG_PRINTLN("Motion detected.");
	}
	else 
	{
		data = "0";
		DEBUG_PRINTLN("Motion absent");
	}

	delay(500);
	return DEVICE_OK;
}

DeviceErrorCode Motion501::readAll(String& data)
{
	return read(data);
}

void Motion501::createEmailMessage(String& subject, String& message, DeviceMessageType deviceMessageType)
{
	subject = String(Configuration::Instance()->deviceName);
	subject += String(" Warning");
	message = String(Configuration::Instance()->deviceName) + String("  has triggered the maximum water level range. ");
}

String Motion501::getName()
{
	return "Motion501";
}

String Motion501::indexComponent()
{
	return "";
}