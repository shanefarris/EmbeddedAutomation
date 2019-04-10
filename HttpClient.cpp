#include "HttpClient.h"

#include <esp8266httpclient.h>
#include <ESP8266httpUpdate.h>

#include "Crypto.h"
#include "WifiConnection.h"
#include "Configuration.h"

HttpClient* HttpClient::_instance = nullptr;

HttpClient* HttpClient::Instance()
{
	if (_instance == nullptr)
	{
		_instance = new HttpClient();
	}
	return _instance;
}

HttpClient::HttpClient()
{
}

bool HttpClient::registerDevice()
{
#if defined TEST_BUILD || defined SERVICES_BUILD || defined PUBLIC_BUILD
	return true;
#endif

	HTTPClient http;

	String url = String("http://") + String(Configuration::Instance()->catalogHost) + "?name=";
	String name = String(Configuration::Instance()->deviceType) + WifiConnection::Instance()->getMac();
	url += name;

	url += String("&type=") + String(Configuration::Instance()->deviceType);
	
	// If it's standalone, then don't try and connect
	if (WifiConnection::Instance()->isStandAlone())
	{
		return true;
	}

	url += String("&ip=") + WifiConnection::Instance()->getIp();
	url += String("&key=");

	SHA256 hasher;
	auto stringToHash = name;
	char key[stringToHash.length() + 1];
	stringToHash.toCharArray(key, stringToHash.length() + 1);
	hasher.doUpdate(key, strlen(key));

	byte hash[SHA256_SIZE];
	hasher.doFinal(hash);

	char str[SHA256_SIZE] = "";
	array_to_string(hash, SHA256_SIZE, str);
	url += String(str);

	http.addHeader("Content-Type", "text/plain");
	http.addHeader("Authorization", "Basic YOUR_PASSWORD");

	DEBUG_PRINTLN(url);
	http.begin(url);

	int httpCode = http.GET();
	DEBUG_PRINTLN(httpCode);
	http.end();

	return httpCode == 200 ? true : false;
}

String HttpClient::checkFirmwareVersion()
{
	String msg = "Up to date";

#if defined TEST_BUILD || defined SERVICES_BUILD || defined PUBLIC_BUILD
	return msg;
#endif

	// If it's standalone, then don't try and connect
	if (WifiConnection::Instance()->isStandAlone())
	{
		return msg;
	}

	HTTPClient http;
#if defined TEST_BUILD

#else
	String url = Configuration::Instance()->firmwareVersion; 
#endif

	http.begin(url);
	int httpCode = http.GET();

	// httpCode will be negative on error
	if (httpCode > 0)
	{
		// HTTP header has been send and Server response header has been handled
		DEBUG_PRINTLN("[HTTP] GET... code: " + httpCode);

		// file found at server
		if (httpCode == HTTP_CODE_OK)
		{
			String payload = http.getString();
			DEBUG_PRINTLN(payload);

			auto serverVersion = payload.toFloat();
			auto clientVersion = Configuration::Instance()->version.toFloat();

			if (serverVersion > clientVersion)
			{
				if (UpdateFirmware())
				{
					msg = "Firmware updated!";
				}
				msg = "Failed to update firmware :(";
			}
		}
	}
	else
	{
		DEBUG_PRINTLN("[HTTP] GET... failed, error: " + http.errorToString(httpCode));
		msg = "[HTTP] GET... failed, error: " + http.errorToString(httpCode);
	}

	http.end();

	return msg;
}

bool HttpClient::UpdateFirmware()
{
	DEBUG_PRINTLN("Update SPIFFS...");

#if DEBUG
	String url = "http://localhost:52426/Catalog/Firmware/";
#else
	String url = "http://192.168.1.32/Catalog/Firmware/";
#endif

	DEBUG_PRINTLN("Update sketch...");
	auto ret = ESPhttpUpdate.updateSpiffs(url + "spiffs.bin");
	if (ret == HTTP_UPDATE_OK) 
	{
		DEBUG_PRINTLN("Update firmware...");
		ret = ESPhttpUpdate.update(url + "/file.bin");
		
		switch (ret) 
		{
		case HTTP_UPDATE_FAILED:
			DEBUG_PRINTLN("HTTP_UPDATE_FAILED Error: " + ESPhttpUpdate.getLastError() + ESPhttpUpdate.getLastErrorString());
			break;

		case HTTP_UPDATE_NO_UPDATES:
			DEBUG_PRINTLN("HTTP_UPDATE_NO_UPDATES");
			break;

		case HTTP_UPDATE_OK:
			DEBUG_PRINTLN("HTTP_UPDATE_OK");
			break;
		}
	}

	return true;
}

void HttpClient::array_to_string(byte array[], unsigned int len, char buffer[])
{
	for (unsigned int i = 0; i < len; i++)
	{
		byte nib1 = (array[i] >> 4) & 0x0F;
		byte nib2 = (array[i] >> 0) & 0x0F;
		buffer[i * 2 + 0] = nib1 < 0xA ? '0' + nib1 : 'A' + nib1 - 0xA;
		buffer[i * 2 + 1] = nib2 < 0xA ? '0' + nib2 : 'A' + nib2 - 0xA;
	}
	buffer[len * 2] = '\0';
}