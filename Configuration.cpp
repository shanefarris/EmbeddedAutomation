#include "Configuration.h"
#include "FS.h"

Configuration* Configuration::_configuration = nullptr;

Configuration* Configuration::Instance()
{
	if (_configuration == nullptr)
	{
		_configuration = new Configuration();
	}
	return _configuration;
}

Configuration::Configuration()
{
}

bool Configuration::loadConfig()
{
	DEBUG_PRINTLN("Configuration::loadConfig()");

	SPIFFS.begin();
	if (!SPIFFS.exists("/config"))
	{
		//writeConfiguration();
		return false;
	}

	File configFile = SPIFFS.open("/config", "r");
	if (!configFile)
	{
		DEBUG_PRINTLN("Failed to open config file");
		return false;
	}

	while (configFile.available())
	{
		//Lets read line by line from the file
		String line = configFile.readStringUntil('\n');

		int separator = line.indexOf(':');
		if (separator > -1)
		{
			String title = line.substring(0, separator);

			line.replace(title, "");
			line.replace(":", "");
			line.replace("\n", "");
			line.trim();

			if (title == "ssid")
			{
				ssid = line;
			}
			else if (title == "psk")
			{
				psk = line;
			}
			else if (title == "deviceType")
			{
				deviceType = line;
			}
			else if (title == "phone")
			{
				phone = line;
			}
			else if (title == "maxCheck")
			{
				maxCheck = line;
			}
			else if (title == "minCheck")
			{
				minCheck = line;
			}
			else if (title == "sendEmail")
			{
				sendEmail = line;
			}
			else if (title == "sendSms")
			{
				sendSms = line;
			}
			else if (title == "maxRange")
			{
				maxRange = line;
			}
			else if (title == "minRange")
			{
				minRange = line;
			}
			else if (title == "minRange")
			{
				minRange = line;
			}
			else if (title == "deviceName")
			{
				deviceName = line;
			}
			else if (title == "deviceInterval")
			{
				deviceInterval = line;
			}
			else if (title == "saveIntervalMinute")
			{
				saveIntervalMinute = line;
			}
			else if (title == "toEmail")
			{
				toEmail = line;
			}
			else if (title == "fromEmail")
			{
				fromEmail = line;
			}
			else if (title == "emailPassword")
			{
				emailPassword = line;
			}
			else if (title == "emailHost")
			{
				emailHost = line;
			}
			else if (title == "emailPort")
			{
				emailPort = line;
			}
			else if (title == "standalonePassword")
			{
				standalonePassword = line == "" ? "password" : line;
			}
			else if (title == "catalogHost")
			{
				catalogHost = line;
			}
			else if (title == "firmwareVersion")
			{
				firmwareVersion = line;
			}
			else if (title == "event1")
			{
				event1 = line;
			}
			else if (title == "event2")
			{
				event2 = line;
			}
			else if (title == "event3")
			{
				event3 = line;
			}
			else if (title == "event4")
			{
				event4 = line;
			}
			else if (title == "event5")
			{
				event5 = line;
			}
			else if (title == "event6")
			{
				event6 = line;
			}

			else if (title == "event7")
			{
				event7 = line;
			}
			else if (title == "event8")
			{
				event8 = line;
			}
			else if (title == "event9")
			{
				event9 = line;
			}
			else if (title == "event10")
			{
				event10 = line;
			}
		}
	}

	configFile.close();

	return true;
}

bool Configuration::populateConfigHtml(String& html)
{
	DEBUG_PRINTLN("Configuration::populateConfigHtml()");
	
	html.replace("{0}", deviceType);
	html.replace("{1}", ssid);
	html.replace("{3}", toEmail);
	html.replace("{4}", phone);
	html.replace("{5}", maxCheck);
	html.replace("{6}", minCheck);
	html.replace("{7}", sendEmail);
	html.replace("{8}", sendSms);
	html.replace("{9}", maxRange);
	html.replace("{10}", minRange);
	html.replace("{11}", deviceName);
	html.replace("{12}", deviceInterval);
	html.replace("{13}", saveIntervalMinute);
	html.replace("{14}", fromEmail);
	html.replace("{15}", emailHost);
	html.replace("{16}", emailPort);
	html.replace("{17}", catalogHost);
	html.replace("{18}", version);
	html.replace("{19}", stateHost);
	// firmwareVersion not used here
}

bool Configuration::populateTasksHtml(String& html)
{
	DEBUG_PRINTLN("Configuration::populateTasksHtml()");
}

bool Configuration::writeConfiguration()
{
	DEBUG_PRINTLN("Configuration::writeConfiguration()");
	SPIFFS.begin();
	File configFile = SPIFFS.open("/config", "w");
	if (!configFile)
	{
		DEBUG_PRINTLN("Failed to open config file");
		return false;
	}

	configFile.println("ssid:" + String(ssid));
	configFile.println("psk:" + String(psk));
	configFile.println("deviceType:" + String(deviceType));
	configFile.println("phone:" + String(phone));
	configFile.println("maxCheck:" + String(maxCheck));
	configFile.println("minCheck:" + String(minCheck));
	configFile.println("sendEmail:" + String(sendEmail));
	configFile.println("sendSms:" + String(sendSms));
	configFile.println("maxRange:" + String(maxRange));
	configFile.println("minRange:" + String(minRange));
	configFile.println("deviceName:" + String(deviceName));
	configFile.println("deviceInterval:" + String(deviceInterval));
	configFile.println("saveIntervalMinute:" + String(saveIntervalMinute));
	configFile.println("toEmail:" + String(toEmail));
	configFile.println("fromEmail:" + String(fromEmail));
	configFile.println("emailPassword:" + String(emailPassword));
	configFile.println("emailHost:" + String(emailHost));
	configFile.println("emailPort:" + String(emailPort));
	configFile.println("standalonePassword:" + String(standalonePassword));
	configFile.println("catalogHost:" + String(catalogHost));
	configFile.println("version:" + String(version));
	configFile.println("stateHost:" + String(stateHost));
	configFile.println("firmwareVersion:" + String(firmwareVersion));
	configFile.println("event1:" + String(event1));
	configFile.println("event2:" + String(event2));
	configFile.println("event3:" + String(event3));
	configFile.println("event4:" + String(event4));
	configFile.println("event5:" + String(event5));
	configFile.println("event6:" + String(event6));
	configFile.println("event7:" + String(event7));
	configFile.println("event8:" + String(event8));
	configFile.println("event9:" + String(event9));
	configFile.println("event10:" + String(event10));

	configFile.close();
	return true;
}

void Configuration::print()
{
	Serial.println("===== Configuration::print =====");
	Serial.println("ssid: " + String(ssid));
	Serial.println("psk: " + String(psk));
	Serial.println("deviceType: " + String(deviceType));
	Serial.println("phone: " + String(phone));
	Serial.println("maxCheck: " + String(maxCheck));
	Serial.println("minCheck: " + String(minCheck));
	Serial.println("sendEmail: " + String(sendEmail));
	Serial.println("sendSms: " + String(sendSms));
	Serial.println("maxRange: " + String(maxRange));
	Serial.println("minRange: " + String(minRange));
	Serial.println("deviceName: " + String(deviceName));
	Serial.println("deviceInterval: " + String(deviceInterval));
	Serial.println("saveIntervalMinute: " + String(saveIntervalMinute));
	Serial.println("toEmail: " + String(toEmail));
	Serial.println("fromEmail: " + String(fromEmail));
	Serial.println("emailPassword: " + String(emailPassword));
	Serial.println("emailHost: " + String(emailHost));
	Serial.println("emailPort: " + String(emailPort));
	Serial.println("standalonePassword: " + String(standalonePassword));
	Serial.println("catalogHost: " + String(catalogHost));
	Serial.println("version: " + String(version));
	Serial.println("stateHost: " + String(stateHost));
	Serial.println("firmwareVersion: " + String(firmwareVersion));
	Serial.println("event1: " + String(event1));
	Serial.println("event2: " + String(event2));
	Serial.println("event3: " + String(event3));
	Serial.println("event4: " + String(event4));
	Serial.println("event5: " + String(event5));
	Serial.println("event6: " + String(event6));
	Serial.println("event7: " + String(event7));
	Serial.println("event8: " + String(event8));
	Serial.println("event9: " + String(event9));
	Serial.println("event10: " + String(event10));
}
