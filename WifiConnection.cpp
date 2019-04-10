#include <ESP8266WiFi.h>

#include "WifiConnection.h"
#include "Configuration.h"
#include "SimpleTimer.h"

WifiConnection* WifiConnection::_instance = nullptr;

WifiConnection* WifiConnection::Instance()
{
	if (_instance == nullptr)
	{
		_instance = new WifiConnection();
	}
	return _instance;
}

WifiConnection::WifiConnection()
{
	_wifi = new ESP8266WiFiClass();
}

bool WifiConnection::setup()
{
	DEBUG_PRINTLN("===== WifiConnection::setup =======");

	if (Configuration::Instance()->ssid != "")
	{
		auto result = setupClient();
		if (result)
		{
			// Turn off timer if it is set
			if (_timer)
			{
				_timer->disable(_connectTimer);
				delete _timer;
			}

			_isStandAlone = false;
			return true;
		}

		// Setup timer so we can try to connect to network later.
		if (_timer == nullptr)
		{
			_timer = new SimpleTimer();
		}

		_connectTimer = _timer->setInterval(MIN * 3, WifiConnection::tryNetworkConnection);
	}

	DEBUG_PRINTLN("Running as standalone.");

	_isStandAlone = true;

	return setupAp();
}

bool WifiConnection::setupAp()
{
	Serial.println("Configuring access point...");

	_wifi->mode(WIFI_AP);
	IPAddress ip(10, 1, 1, 1);
	IPAddress subnet(255, 255, 255, 0);
	_wifi->softAPConfig(ip, ip, subnet);

	// Convert AP password string to char[]
	auto length = Configuration::Instance()->standalonePassword.length() + 1;
	char AP_Pass[length];
	Configuration::Instance()->standalonePassword.toCharArray(AP_Pass, length);

	if (Configuration::Instance()->deviceName == "")
	{
		String AP_NameString = "DEVICE_" + getMac();

		char AP_NameChar[AP_NameString.length() + 1];
		memset(AP_NameChar, 0, AP_NameString.length() + 1);

		for (int i = 0; i < AP_NameString.length(); i++)
		{
			AP_NameChar[i] = AP_NameString.charAt(i);
		}
		_wifi->softAP(AP_NameChar, AP_Pass);
		DEBUG_PRINTLN("AP Name: " + String(AP_NameChar));
	}
	else
	{
		length = Configuration::Instance()->deviceName.length() + 1;
		char AP_NameChar[length];
		Configuration::Instance()->deviceName.toCharArray(AP_NameChar, length);
		_wifi->softAP(AP_NameChar, AP_Pass);
		DEBUG_PRINTLN("AP Name: " + String(AP_NameChar));
	}
	DEBUG_PRINTLN("AP Pass: " + String(AP_Pass));

	IPAddress myIP = _wifi->softAPIP();
	DEBUG_PRINT("AP IP address: ");
	DEBUG_PRINTLN(myIP);

	return true;
}

bool WifiConnection::setupClient()
{
	if (attempt >= 5)
	{
		Serial.println("Could not connect to AP.");
		return false;
	}

	Serial.print("Connecting to ");

	char ssid[Configuration::Instance()->ssid.length() + 1];
	Configuration::Instance()->ssid.toCharArray(ssid, Configuration::Instance()->ssid.length() + 1);

	char psk[Configuration::Instance()->psk.length() + 1];
	Configuration::Instance()->psk.toCharArray(psk, Configuration::Instance()->psk.length() + 1);

	_wifi->mode(WIFI_STA);
	_wifi->begin(ssid, psk);
	Serial.println(ssid);

	uint8_t i = 0;
	while (_wifi->status() != WL_CONNECTED && i++ < 50)
	{
		delay(250);
		Serial.print(".");
		if (_wifi->status() == WL_CONNECTED)
		{
			break;
		}
	}

	++attempt;
	Serial.println("");
	if (_wifi->status() != WL_CONNECTED)
	{
		Serial.print("Connection: TIMEOUT on attempt: ");
		Serial.println(attempt);

		return setupClient();
	}
	else
	{
		DEBUG_PRINTLN("Connection: ESTABLISHED");
		DEBUG_PRINT("Got IP address: ");
		DEBUG_PRINTLN(_wifi->localIP());
	}

	return true;
}

bool WifiConnection::isStandAlone()
{
	return _isStandAlone;
}

void WifiConnection::tryNetworkConnection()
{
	DEBUG_PRINTLN("WifiConnection::tryNetworkConnection");
	ESP.reset();
}

bool WifiConnection::isConnected()
{
	if (_isStandAlone)
		return true;

	return _wifi->status() == WL_CONNECTED;
}

String WifiConnection::getIp()
{
	String s = "";
	for (int i = 0; i < 4; i++)
	{
		s += i ? "." + String(_wifi->localIP()[i]) : String(_wifi->localIP()[i]);
	}
	return s;
}

String WifiConnection::getMac()
{
	uint8_t mac[WL_MAC_ADDR_LENGTH];
	_wifi->softAPmacAddress(mac);
	String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) + String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
	macID.toUpperCase();
	return macID;
}

void WifiConnection::disconnect()
{
	if (_wifi)
	{
		_wifi->disconnect(true);
		delete _wifi;
		_wifi = nullptr;
	}
}
