#include "Gsender.h"
#include "Configuration.h"
#include "Base64.h"

Gsender::Gsender()
{
}

bool Gsender::AwaitSMTPResponse(WiFiClientSecure &client, const String &resp, uint16_t timeOut)
{
	uint32_t ts = millis();
	while (!client.available())
	{
		if (millis() > (ts + timeOut))
		{
			_error = "SMTP Response TIMEOUT!";
			return false;
		}
	}
	_serverResponce = client.readStringUntil('\n');

	DEBUG_PRINTLN(_serverResponce);

	if (resp && _serverResponce.indexOf(resp) == -1)
		return false;
	return true;
}

String Gsender::getLastResponce()
{
	return _serverResponce;
}

const char *Gsender::getError()
{
	return _error;
}

bool Gsender::Send(const String &subject, const String &message)
{
	WiFiClientSecure client;

	DEBUG_PRINT("Connecting to :");
	char emailHost[Configuration::Instance()->emailHost.length() + 1];
	Configuration::Instance()->emailHost.toCharArray(emailHost, Configuration::Instance()->emailHost.length() + 1);
	DEBUG_PRINTLN(emailHost);

	if (!client.connect(emailHost, Configuration::Instance()->emailPort.toInt()))
	{
		_error = "Could not connect to mail server";
		return false;
	}

	DEBUG_PRINTLN("Connected to mail server.");
	if (!AwaitSMTPResponse(client, "220"))
	{
		_error = "Connection Error";
		return false;
	}

	DEBUG_PRINTLN("HELO friend:");
	client.println("HELO friend");
	if (!AwaitSMTPResponse(client, "250"))
	{
		_error = "identification error";
		return false;
	}

	DEBUG_PRINTLN("AUTH LOGIN:");
	client.println("AUTH LOGIN");
	AwaitSMTPResponse(client);

	DEBUG_PRINTLN("EMAILBASE64_LOGIN:");
	char b64data[256];
	base64_encode(b64data, (char *)Configuration::Instance()->fromEmail.c_str(), Configuration::Instance()->fromEmail.length());
	client.println(b64data);
	AwaitSMTPResponse(client);

	DEBUG_PRINTLN("EMAILBASE64_PASSWORD:");
	client.println(Configuration::Instance()->emailPassword);
	if (!AwaitSMTPResponse(client, "235"))
	{
		_error = "SMTP AUTH error";
		return false;
	}

	String mailFrom = "MAIL FROM: <" + String(Configuration::Instance()->fromEmail) + '>';
	DEBUG_PRINTLN(mailFrom);
	client.println(mailFrom);
	AwaitSMTPResponse(client);

	String rcpt = "RCPT TO: <" + String(Configuration::Instance()->toEmail) + '>';
	DEBUG_PRINTLN(rcpt);
	client.println(rcpt);
	AwaitSMTPResponse(client);

	DEBUG_PRINTLN("DATA:");
	client.println("DATA");
	if (!AwaitSMTPResponse(client, "354"))
	{
		_error = "SMTP DATA error";
		return false;
	}

	client.println("From: <" + String(Configuration::Instance()->fromEmail) + '>');
	client.println("To: <" + String(Configuration::Instance()->toEmail) + '>');

	client.print("Subject: ");
	client.println(subject);

	client.println("Mime-Version: 1.0");
	client.println("Content-Type: text/html; charset=\"UTF-8\"");
	client.println("Content-Transfer-Encoding: 7bit");
	client.println();
	String body = "<!DOCTYPE html><html lang=\"en\">" + message + "</html>";
	client.println(body);
	client.println(".");
	if (!AwaitSMTPResponse(client, "250"))
	{
		_error = "Sending message error";
		return false;
	}

	client.println("QUIT");
	if (!AwaitSMTPResponse(client, "221"))
	{
		_error = "SMTP QUIT error";
		return false;
	}
	return true;
}

bool Gsender::sendSms(const String &phone, const String &message)
{
	return false;
}

Gsender::~Gsender()
{
	if(_error)
		delete _error;
}
