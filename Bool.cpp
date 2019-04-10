#include "Bool.h"

#define TRIGGER D1

Bool::Bool()
{
	pinMode(TRIGGER, INPUT_PULLUP);   // declare sensor as input
}

DeviceErrorCode Bool::read(String& data)
{
	long state = digitalRead(TRIGGER);
	if (state == HIGH) 
	{
		data = "1";
		DEBUG_PRINTLN("State: HIGH");
	}
	else 
	{
		data = "0";
		DEBUG_PRINTLN("State: LOW");
	}

	delay(500);
	return DEVICE_OK;
}

DeviceErrorCode Bool::readAll(String& data)
{
	return read(data);
}

void Bool::createEmailMessage(String& subject, String& message, DeviceMessageType deviceMessageType)
{
	subject = String(Configuration::Instance()->deviceName);
	subject += String(" Warning");
	message = String(Configuration::Instance()->deviceName) + String("  has triggered the state to HIGH. ");
}

String Bool::getName()
{
	return "Bool";
}

String Bool::indexComponent()
{
	return "";
}