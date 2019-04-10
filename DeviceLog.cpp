#if 0
#include "DeviceLog.h"
#include "FS.h"
#include "Configuration.h"

bool DeviceLog::load()
{
	// Open the device log file.
	if (SPIFFS.exists("/device.log"))
	{
		File deviceLog = SPIFFS.open("/device.log", "r");
		if (!deviceLog)
		{
			DEBUG_PRINTLN("Failed to open device.log file");
			return false;
		}

		// Clear the arrays out.
		for (int i = 0; i < LOG_SIZE; i++)
		{
			Date[i] = "";
			Data[i] = "";
		}

		int numOfLines = 0;
		while (deviceLog.available())
		{
			//Lets read line by line from the file
			deviceLog.readStringUntil('\n');
			numOfLines++;
		}

		int i = 0;
		deviceLog.seek(0, SeekSet);
		while (deviceLog.available() && i <= numOfLines)
		{
			String line = deviceLog.readStringUntil('\n');
			DEBUG_PRINTLN(line);

			int separator = line.lastIndexOf(':');
			if (separator > -1)
			{
				Date[i] = line.substring(0, separator);

				line.replace(Date[i], "");
				line.replace(":", "");
				line.trim();
				Data[i] = line;
			}
			i++;
		}
		return true;
	}

	return false;
}

bool DeviceLog::logData(const String& date, const String& data)
{
	DEBUG_PRINTLN("logData: " + String(date) + ", " + String(data));
	int i = 0;
	while (Date[i] != "" && i < LOG_SIZE)
	{
		i++;
	}

	DEBUG_PRINTLN("logData array size: " + String(i));
	if (i == (LOG_SIZE - 1))
	{
		for (int i = 0; i < (LOG_SIZE - 1); i++)
		{
			Date[LOG_SIZE - 1] = date;
			Data[LOG_SIZE - 1] = data;
		}
		Date[i] = Date[i + 1];
		Data[i] = Data[i + 1];
	}
	else
	{
		Date[i] = date;
		Data[i] = data;
	}

	return true;
}

bool DeviceLog::populateHtml(String& html)
{
	String dict = "";
	int i = 0;
	while (i < LOG_SIZE && Date[i] != "")
	{
		dict += "dict.push({key:\"" + String(Date[i]) + "\", value: \"" + String(Data[i]) + "\"});";
		i++;
	}

	html.replace("{0}", dict);
}

void DeviceLog::print()
{
	Serial.println("=========== DeviceLog::print ===========");
	int i = 0;
	while (i < LOG_SIZE && Date[i] != "")
	{
		DEBUG_PRINTLN(String(Date[i]) + String(": ") + String(Data[i]));
		i++;
	}
}
#endif