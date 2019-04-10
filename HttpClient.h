
#ifndef http_client_h
#define http_client_h

#include <Arduino.h>

class HttpClient
{
private:

	static HttpClient* _instance;

	HttpClient();

	bool UpdateFirmware();
	void array_to_string(byte array[], unsigned int len, char buffer[]);

public:
	static HttpClient* Instance();
	
	bool registerDevice();
	String checkFirmwareVersion();

};

#endif // http_client_h