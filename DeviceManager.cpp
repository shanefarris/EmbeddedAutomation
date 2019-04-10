#include "DeviceManager.h"
#include "Configuration.h"
#include "SimpleTimer.h"
#include "Gsender.h"
#include "NasWebServer.h"
#include "WifiConnection.h"
#include "HttpClient.h"
#include "CommandManager.h"

#include "Dummy.h"
#include "Dht11.h"
#include "Analog.h"
#include "DS18B20.h"
#include "HCSR04.h"
#include "RelaySingle.h"
#include "Bool.h"
#include "Motion501.h"
#include "Flood.h"

DeviceType      DeviceManager::_deviceType;
IDevice*		DeviceManager::_device = nullptr;
String          DeviceManager::_lastReading = "";
SimpleTimer*    DeviceManager::_timer = nullptr;
int             DeviceManager::_timerRead = 0;
int             DeviceManager::_timerEmailNoticeCountdown = 0;
int             DeviceManager::_timerSmsNoticeCountdown = 0;
int				DeviceManager::_timerDeviceUpdate = 0;

bool DeviceManager::setup()
{
	if (Configuration::Instance()->deviceType == "temp")
	{
		_deviceType = DeviceType::temp;
	}
	else if (Configuration::Instance()->deviceType == "temp_humidity")
	{
		_deviceType = DeviceType::temp_humidity;
		_device = new Dht11();
	}
	else if (Configuration::Instance()->deviceType == "water_level")
	{
		_deviceType = DeviceType::water_level;
		_device = new Analog();
	}
	else if (Configuration::Instance()->deviceType == "water_flow")
	{
		_deviceType = DeviceType::water_flow;
	}
	else if (Configuration::Instance()->deviceType == "flood")
	{
		_deviceType = DeviceType::flood;
		_device = new Flood();
	}
	else if (Configuration::Instance()->deviceType == "water_temp")
	{
		_deviceType = DeviceType::water_temp;
		_device = new DS18B20();
	}
	else if (Configuration::Instance()->deviceType == "HCSR04")
	{
		_deviceType = DeviceType::HCSR04;
		_device = new Hcsr04();
	}
	else if (Configuration::Instance()->deviceType == "relay_single")
	{
		_deviceType = DeviceType::relay_single;
		_device = new RelaySingle();
	}
	else if (Configuration::Instance()->deviceType == "motion")
	{
		_deviceType = DeviceType::motion;
		_device = new Motion501();
	}
	else if (Configuration::Instance()->deviceType == "water_float")
	{
		_deviceType = DeviceType::water_float;
		_device = new Bool();
	}
	else
	{
		_device = new Dummy();
	}

	DEBUG_PRINTLN(String("Device created: ") + _device->getName());

	// Setup timers
	_timer = new SimpleTimer();

#if 0
	if (Configuration::Instance()->saveIntervalMinute != "")
	{
		auto logDevice = Configuration::Instance()->saveIntervalMinute.toInt();
		DEBUG_PRINTLN("logDevice: " + String(logDevice));
		_timerLogDevice = _timer->setInterval(MIN * logDevice, DeviceManager::deviceLogHandle);
	}
#endif

	if (Configuration::Instance()->deviceInterval != "")
	{
		auto readDevice = Configuration::Instance()->deviceInterval.toInt();
		DEBUG_PRINTLN("readDevice: " + String(readDevice));
		_timerRead = _timer->setInterval(readDevice, DeviceManager::deviceReadHandle);
	}
	else
	{
		DEBUG_PRINTLN("readDevice: " + String(SEC * 3));
		_timerRead = _timer->setInterval(SEC * 3, DeviceManager::deviceReadHandle);
	}

	_timerEmailNoticeCountdown = _timer->setInterval(HOUR, DeviceManager::turnOffEmailNotice);
	_timerSmsNoticeCountdown = _timer->setInterval(HOUR * 3, DeviceManager::turnOffSmsNotice);
	_timerDeviceUpdate = _timer->setInterval(HOUR, DeviceManager::deviceUpdateHandle);
	_timer->disable(_timerEmailNoticeCountdown);
	_timer->disable(_timerSmsNoticeCountdown);

	return true;
}

String DeviceManager::read()
{
	DEBUG_PRINTLN("DeviceManager::read");

	if (_device)
	{
		_device->read(_lastReading);
	}
	return _lastReading;
}

String DeviceManager::readAll()
{
	DEBUG_PRINTLN("DeviceManager::readAll");

	String data;
	if (_device)
	{
		_device->readAll(data);
	}
	return data;
}

void DeviceManager::deviceReadHandle()
{
	if (_device)
	{
		DEBUG_PRINTLN("Reading device...");
		auto result = _device->read(_lastReading);
		if (result == DEVICE_OK)
		{
			DEBUG_PRINTLN("Value read is: " + String(_lastReading));
			CommandManager::Instance()->onDeviceReadSuccess();
			checkDeviceParameters();
		}
		else
		{
			// Error reading
			CommandManager::Instance()->onDeviceReadFail();
		}
	}
}

void DeviceManager::deviceLogHandle()
{
}

void DeviceManager::update()
{
	_timer->run();
}

void DeviceManager::checkDeviceParameters()
{
	DEBUG_PRINTLN("DeviceManager::checkDeviceParameters");

	if (Configuration::Instance())
	{
		// Check max
		auto reading = _lastReading.toFloat();
		if (Configuration::Instance()->maxCheck == "1")
		{
			auto max = Configuration::Instance()->maxRange.toFloat();
			if (reading >= max)
			{
				CommandManager::Instance()->onMaxReached();
				sendEmailNotice(MAX_RANGE);
				sendSmsNotice(MAX_RANGE);
			}
		}

		// Check min
		if (Configuration::Instance()->minCheck == "1")
		{
			auto min = Configuration::Instance()->minRange.toFloat();
			if (reading <= min)
			{
				CommandManager::Instance()->onMinReached();
				sendEmailNotice(MIN_RANGE);
				sendSmsNotice(MIN_RANGE);
			}
		}
	}
}

void DeviceManager::sendEmailNotice(DeviceMessageType deviceMessageType)
{
	DEBUG_PRINTLN("DeviceManager::sendEmailNotice");
	if (Configuration::Instance()->sendEmail != "1")
	{
		return;
	}

	DEBUG_PRINTLN("_isStandAlone: " + String(WifiConnection::Instance()->isStandAlone()));
	if (WifiConnection::Instance()->isStandAlone())
	{
		DEBUG_PRINTLN("This is standalone, will not send notice.");
		return;
	}

	if (_timer->isEnabled(_timerEmailNoticeCountdown) == true)
	{
		DEBUG_PRINTLN("_timerEmailNoticeCountdown is already enabled, exiting.");
		return;  // We are still counting down to last notification.
	}

	// Start timer here, I think it might prevent the multiple sends
	DEBUG_PRINTLN("_timerEmailNoticeCountdown started.");
	_timer->enable(_timerEmailNoticeCountdown);

	DEBUG_PRINTLN(Configuration::Instance()->fromEmail);
	DEBUG_PRINTLN(Configuration::Instance()->toEmail);

	// Use base 64 email from address if one is in our config
	if (Configuration::Instance()->emailHost != "")
	{
		NasWebServer::Instance()->shutdown();
		auto sender = new Gsender();
		String subject;
		String message;
		_device->createEmailMessage(subject, message, deviceMessageType);
		message += String("\n\nValue: ") + String(_lastReading);

		if (sender->Send(subject, message) == false)
		{
			DEBUG_PRINTLN(sender->getError());

			// Disable the timer, maybe it will be successful if its tried again.
			_timer->disable(_timerEmailNoticeCountdown);
		}

		NasWebServer::Instance()->start();
	}
	else
	{
		DEBUG_PRINTLN("Email host on configured, not sending email.");
	}

	_timer->setInterval(MIN * 60, DeviceManager::turnOffEmailNotice);
}

void DeviceManager::sendSmsNotice(DeviceMessageType deviceMessageType)
{
	DEBUG_PRINTLN("DeviceManager::sendSmsNotice");

	if (Configuration::Instance()->sendSms != "1")
	{
		return;
	}

	DEBUG_PRINTLN("_isStandAlone: " + String(WifiConnection::Instance()->isStandAlone()));
	if (WifiConnection::Instance()->isStandAlone())
	{
		DEBUG_PRINTLN("This is standalone, will not send notice.");
		return;
	}

	if (_timer->isEnabled(_timerSmsNoticeCountdown) == true)
	{
		DEBUG_PRINTLN("_timerSmsNoticeCountdown is already enabled, exiting.");
		return;  // We are still counting down to last notification.
	}

	// Start timer here, I think it might prevent the multiple sends
	DEBUG_PRINTLN("_timerSmsNoticeCountdown started.");
	_timer->enable(_timerSmsNoticeCountdown);

	DEBUG_PRINTLN(Configuration::Instance()->phone);

	// Use phone number in our config
	if (Configuration::Instance()->phone != "")
	{
		NasWebServer::Instance()->shutdown();
		auto sender = new Gsender();
		String subject;
		String message;
		_device->createEmailMessage(subject, message, deviceMessageType);
		message += String("\n\nValue: ") + String(_lastReading);

		if (sender->sendSms(Configuration::Instance()->phone, message) == false)
		{
			DEBUG_PRINTLN(sender->getError());

			// Disable the timer, maybe it will be successful if its tried again.
			_timer->disable(_timerSmsNoticeCountdown);
		}

		NasWebServer::Instance()->start();
	}

	_timer->setInterval(MIN * 60, DeviceManager::turnOffSmsNotice);
}

void DeviceManager::turnOffEmailNotice()
{
	DEBUG_PRINTLN("DeviceManager::turnOffEmailNotice");
	_timer->disable(_timerEmailNoticeCountdown);
}

void DeviceManager::turnOffSmsNotice()
{
	_timer->disable(_timerSmsNoticeCountdown);
}

void DeviceManager::deviceUpdateHandle()
{
	HttpClient::Instance()->registerDevice();
}

String DeviceManager::indexComponent()
{
	if (_device)
	{
		return _device->indexComponent();
	}
	return "";
}

bool DeviceManager::switchDevice(const String& value)
{
	if (_device)
	{
		_device->switchDevice(value);
	}
}