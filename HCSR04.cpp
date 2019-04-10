#include "HCSR04.h"

#define TRIGGER D1
#define ECHO    D2

Hcsr04::Hcsr04()
{
	pinMode(TRIGGER, OUTPUT);
	pinMode(ECHO, INPUT);

	_lastresult = _read();
}

DeviceErrorCode Hcsr04::read(String& data)
{
	// Print a friendly reading direct to the user.
	data = "Error reading";
	
	long result1, result2, result3;
	int count = 0;
	do
	{
		result1 = _read();
		result2 = _read();
		result3 = _read();

		count++;

		if (count >= 10)
		{
			return ERROR_READ;
		}

	} while ((result1 + 10 < result2 || result1 + 10 < result3) ||
			 (result1 - 10 > result2 || result1 - 10 > result3));

	_lastresult = result1;
	data = String(_lastresult);
	DEBUG_PRINT("Final HCSR04: ");
	DEBUG_PRINTLN(_lastresult);

	return DEVICE_OK;
}

DeviceErrorCode Hcsr04::readAll(String& data)
{
	auto errorCode = read(data);
	data += String(" Max: ") + Configuration::Instance()->maxRange;
	data += String(" Min: ") + Configuration::Instance()->minRange;
	return errorCode;
}

long Hcsr04::_read()
{
	long duration, distance;
	digitalWrite(TRIGGER, LOW);
	delayMicroseconds(2);

	digitalWrite(TRIGGER, HIGH);
	delayMicroseconds(10);

	digitalWrite(TRIGGER, LOW);
	duration = pulseIn(ECHO, HIGH);
	distance = (duration / 2) / 29.1;

	DEBUG_PRINT("Hcsr04::_read:");
	DEBUG_PRINTLN(distance);

	return distance;
}

void Hcsr04::createEmailMessage(String& subject, String& message, DeviceMessageType deviceMessageType)
{
	subject = String(Configuration::Instance()->deviceName);
	if (deviceMessageType == MAX_RANGE)
	{
		subject += String(" MAX water level Warning");
		message = String(Configuration::Instance()->deviceName) + String("  has triggered the maximum water level range.");
	}
	else if (deviceMessageType == MIN_RANGE)
	{
		subject += String(" MIN water level Warning");
		message = String(Configuration::Instance()->deviceName) + String("  has triggered the minimum water level range.");
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

String Hcsr04::getName()
{
	return "HCSR04";
}

String Hcsr04::indexComponent()
{
	return "";
}