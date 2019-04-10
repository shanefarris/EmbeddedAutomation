#include "RelaySingle.h"

RelaySingle::RelaySingle()
{
	pinMode(inputPin, OUTPUT);
	digitalWrite(inputPin, LOW);
	isOpen = false;
}

DeviceErrorCode RelaySingle::read(String& data)
{
	if (isOpen)
	{
		data = "Open";
	}
	else
	{
		data = "Close";
	}

	return DEVICE_OK;
}

DeviceErrorCode RelaySingle::readAll(String& data)
{
	return read(data);
}

void RelaySingle::createEmailMessage(String& subject, String& message, DeviceMessageType deviceMessageType)
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

String RelaySingle::getName()
{
	return "relay_single";
}

String RelaySingle::indexComponent()
{
	String html = "<h3>Input pin is now: ";
	if (isOpen)
	{
		html += String("OPEN");
	}
	else
	{
		html += String("CLOSE");
	}
	
	html += "<br><br><br>";
	html += "Click <a href=\"/switch?value=ON\">here</a> Turn relay ON<br>";
	html += "Click <a href=\"/switch?value=OFF\">here</a> Turn relay OFF<br>";
	html += "</h3>";

	return html;
}

bool RelaySingle::switchDevice(const String& value)
{
	if (value == "ON")
	{
		isOpen = true;
		digitalWrite(inputPin, HIGH);
		DEBUG_PRINTLN("setting to: high");
	}
	else if (value == "OFF")
	{
		isOpen = false;
		digitalWrite(inputPin, LOW);
		DEBUG_PRINTLN("setting to: low");
	}
}