#ifndef gsender_h
#define gsender_h

#include <WiFiClientSecure.h>

class Gsender
{
private:
	const char* _error = nullptr;
	String _serverResponce;

	bool AwaitSMTPResponse(WiFiClientSecure &client, const String &resp = "", uint16_t timeOut = 10000);

public:
	Gsender();
	~Gsender();

	bool Send(const String &subject, const String& message);
	String getLastResponce();
	const char* getError();

	bool sendSms(const String& phone, const String& message);
};
#endif // gsender_h
