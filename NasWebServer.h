#ifndef NasWebServer_h
#define NasWebServer_h

#include <Arduino.h>

class ESP8266WebServer;

class NasWebServer
{
public:
	NasWebServer();
	static NasWebServer* Instance();

	bool setup();
	void handleClient();
	void shutdown();
	void start();

private:
	static ESP8266WebServer*	_server;
	static NasWebServer*		_nasWebServer;

	static void renderPostConfigHtml();
	static void renderConfigHtml();
	static void renderReadHtml();
	static void renderReadAllHtml();
	static void renderIndexHtml();
	static void renderTestEmailHtml();
	static void renderRestart();
	static void renderFirmwareVersion();
	static void renderPostSwitch();

	static String openHtmlDoc(const String& fileName);
	static String escapeParameter(String& param);
};
#endif // NasWebServer_h