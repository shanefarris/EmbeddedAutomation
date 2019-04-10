#include <ESP8266WebServer.h>
#include "FS.h"
#include "Configuration.h"
#include "DeviceManager.h"
#include "NasWebServer.h"
#include "HttpClient.h"

ESP8266WebServer* NasWebServer::_server = nullptr;
NasWebServer* NasWebServer::_nasWebServer = nullptr;

NasWebServer* NasWebServer::Instance()
{
	if (!_nasWebServer)
	{
		_nasWebServer = new NasWebServer();
		DEBUG_PRINTLN("Web server initialized.");
	}
	return _nasWebServer;
}

NasWebServer::NasWebServer()
{

}

bool NasWebServer::setup()
{
	start();
	DEBUG_PRINTLN("HTTP server started");
	return true;
}

void NasWebServer::shutdown()
{
	DEBUG_PRINTLN("Webserver shutdown");

	if (_server)
	{
		_server->close();
		_server->stop();
		delete _server;
		_server = nullptr;
	}
}

void NasWebServer::start()
{
	shutdown();

	DEBUG_PRINTLN("Webserver start");

	_server = new ESP8266WebServer(80);
	_server->begin();

	_server->on("/config_save", NasWebServer::renderPostConfigHtml);
	_server->on("/config", NasWebServer::renderConfigHtml);
	_server->on("/", NasWebServer::renderIndexHtml);
	_server->on("/index", NasWebServer::renderIndexHtml);
	_server->on("/test_email", NasWebServer::renderTestEmailHtml);
	_server->on("/read", NasWebServer::renderReadHtml);
	_server->on("/readall", NasWebServer::renderReadAllHtml);
	_server->on("/reset", NasWebServer::renderRestart);
	_server->on("/update", NasWebServer::renderFirmwareVersion);
	_server->on("/switch", NasWebServer::renderPostSwitch);
	_server->onNotFound(NasWebServer::renderIndexHtml);

	_server->begin();
}

void NasWebServer::handleClient()
{
	if (_server)
	{
		_server->handleClient();
	}
}

void NasWebServer::renderPostConfigHtml()
{
	DEBUG_PRINTLN("NasWebServer::renderPostConfigHtml()");

	// Network settings
	Configuration::Instance()->ssid = _server->arg("ssid");
	if (_server->arg("psk") != "")
	{
		Configuration::Instance()->psk = _server->arg("psk");
	}

	Configuration::Instance()->deviceType = _server->arg("deviceType");
	Configuration::Instance()->phone = _server->arg("phone");
	Configuration::Instance()->maxCheck = _server->arg("maxCheck");
	Configuration::Instance()->minCheck = _server->arg("minCheck");
	Configuration::Instance()->sendEmail = _server->arg("sendEmail");
	Configuration::Instance()->sendSms = _server->arg("sendSms");
	Configuration::Instance()->maxRange = _server->arg("maxRange");
	Configuration::Instance()->minRange = _server->arg("minRange");
	Configuration::Instance()->deviceName = _server->arg("deviceName");
	Configuration::Instance()->deviceInterval = _server->arg("deviceInterval");
	Configuration::Instance()->saveIntervalMinute = _server->arg("saveIntervalMinute");
	Configuration::Instance()->catalogHost = _server->arg("catalogHost");
	Configuration::Instance()->version = _server->arg("version");
	Configuration::Instance()->stateHost = _server->arg("stateHost");

	// Email settings
	Configuration::Instance()->toEmail = _server->arg("toEmail");
	Configuration::Instance()->fromEmail = _server->arg("fromEmail");
	Configuration::Instance()->emailHost = _server->arg("emailHost");
	Configuration::Instance()->emailPort = _server->arg("emailPort");

	if (_server->arg("emailPassword") != "")
	{
		Configuration::Instance()->emailPassword = _server->arg("emailPassword");
	}

	if (_server->arg("standalonePassword") != "")
	{
		Configuration::Instance()->standalonePassword = _server->arg("standalonePassword");
	}

	Configuration::Instance()->writeConfiguration();
	Configuration::Instance()->print();
	renderIndexHtml();
}

void NasWebServer::renderConfigHtml()
{
	DEBUG_PRINTLN("NasWebServer::renderConfigHtml()");
	String returnHtml = openHtmlDoc("/config.html");

	Configuration::Instance()->populateConfigHtml(returnHtml);
	Configuration::Instance()->print();
	_server->send(200, "text/html", returnHtml);
	delay(100);
}

void NasWebServer::renderReadHtml()
{
	_server->send(200, "text/html", DeviceManager::getLastRead());
	delay(100);
}

void NasWebServer::renderReadAllHtml()
{
	DEBUG_PRINTLN("read all");
	_server->send(200, "text/html", DeviceManager::readAll());
	delay(100);
}

void NasWebServer::renderIndexHtml()
{
	DEBUG_PRINTLN("index");
	String returnHtml = openHtmlDoc("/index.html");

	returnHtml.replace("{0}", Configuration::Instance()->deviceName);
	returnHtml.replace("{1}", Configuration::Instance()->deviceType);
	returnHtml.replace("{2}", DeviceManager::readAll());
	returnHtml.replace("{3}", Configuration::Instance()->maxRange);
	returnHtml.replace("{4}", Configuration::Instance()->minRange);
	returnHtml.replace("{5}", Configuration::Instance()->version);
	returnHtml.replace("{6}", DeviceManager::indexComponent());

#ifdef SERVICES_BUILD
	returnHtml.replace("{7}", String("Services"));
#elif STORE_BUILD
	returnHtml.replace("{7}", String("Store"));
#elif PUBLIC_BUILD
	returnHtml.replace("{7}", String("Public"));
#elif TEST_BUILD
	returnHtml.replace("{7}", String("Test"));
#else
	returnHtml.replace("{7}", String("Not Set"));
#endif // SERVICES_BUILD


	_server->send(200, "text/html", returnHtml);
	delay(100);
}

void NasWebServer::renderTestEmailHtml()
{
	DEBUG_PRINTLN("test email");
	_server->send(200, "text/html", "<!DOCTYPE HTML>\r\n<html>\r\nTEST EMAIL</html>");
	delay(100);
}

void NasWebServer::renderRestart()
{
	DEBUG_PRINTLN("restart");
	ESP.reset();
}

void NasWebServer::renderFirmwareVersion()
{
	DEBUG_PRINTLN("renderFirmwareVersion");

	_server->send(200, "text/html", "<!DOCTYPE HTML>\r\n<html>\r\n" + HttpClient::Instance()->checkFirmwareVersion() + "</html>");
	delay(100);
}

void NasWebServer::renderPostSwitch()
{
	DEBUG_PRINTLN("NasWebServer::renderPostSwitch()");

	auto value = _server->arg("value");
	DeviceManager::switchDevice(value);

	NasWebServer::renderIndexHtml();
}

String NasWebServer::openHtmlDoc(const String& fileName)
{
	DEBUG_PRINTLN("NasWebServer::openHtmlDoc");

	String returnHtml;
	if (!SPIFFS.exists(fileName))
	{
		returnHtml += "<!DOCTYPE HTML>\r\n<html>\r\n" + String(fileName) + " does not exist.";
	}
	else
	{
		File file = SPIFFS.open(fileName, "r");
		if (file)
		{
			while (file.available())
			{
				returnHtml += char(file.read());
			}
			file.close();
		}
		else
		{
			returnHtml += "<!DOCTYPE HTML>\r\n<html>\r\nFailed to open " + String(fileName) + " file";
		}
	}

	return returnHtml;
}

String NasWebServer::escapeParameter(String& param)
{
	param.replace("+", " ");
	param.replace("%21", "!");
	param.replace("%23", "#");
	param.replace("%24", "$");
	param.replace("%26", "&");
	param.replace("%27", "'");
	param.replace("%28", "(");
	param.replace("%29", ")");
	param.replace("%2A", "*");
	param.replace("%2B", "+");
	param.replace("%2C", ",");
	param.replace("%2F", "/");
	param.replace("%3A", ":");
	param.replace("%3B", ";");
	param.replace("%3D", "=");
	param.replace("%3F", "?");
	param.replace("%40", "@");
	param.replace("%5B", "[");
	param.replace("%5D", "]");
	return param;
}