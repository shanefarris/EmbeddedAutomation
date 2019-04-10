#include "OledExpansionModule.h"
#include "DeviceManager.h"
#include "Dht11.h"
#include "IDevice.h"
#include "WifiConnection.h"
#include "Configuration.h"

OledExpansionModule* OledExpansionModule::_oledExpansionModule = nullptr;

OledExpansionModule* OledExpansionModule::Instance()
{
	if (_oledExpansionModule == nullptr)
	{
		_oledExpansionModule = new OledExpansionModule();
	}
	return _oledExpansionModule;
}

OledExpansionModule::OledExpansionModule()
{
	pinMode(CHECKPIN, OUTPUT);
}

void OledExpansionModule::createDisplay()
{
	// Initialising the UI will init the display too.
	display = new SSD1306(0x3c, sda, scl);
	display->init();
	display->flipScreenVertically();
	display->setFont(ArialMT_Plain_16);
	display->setTextAlignment(TEXT_ALIGN_LEFT);
}

void OledExpansionModule::update()
{
	if (digitalRead(CHECKPIN) > 0)
	{
		//DEBUG_PRINTLN("CHECKPIN = true");
		if (display == nullptr)
		{
			createDisplay();
		}

		display->clear();
		display->drawString(0, 0, "IP: " + WifiConnection::Instance()->getIp());
		display->drawString(0, 16, "MAC: " + WifiConnection::Instance()->getMac());
		display->drawString(0, 32, Configuration::Instance()->deviceName);

		if (DeviceManager::getDeviceType() == DeviceType::temp_humidity)
		{
			auto dht11 = static_cast<Dht11*>(DeviceManager::getDeviceReader());
			display->drawString(0, 48, "H:" + String(dht11->readHumidity()));
			display->drawString(64, 48, "T:" + String(dht11->readTemperature()));
		}
		else
		{
			display->drawString(0, 48, "Value: " + String(DeviceManager::getLastRead()));
		}
		display->display();
	}
	else
	{
		//DEBUG_PRINTLN("CHECKPIN = false");
		if (display != nullptr)
		{
			delete display;
			display = nullptr;
		}
	}
}
// TODO
void OledExpansionModule::message(const String& text)
{
	if (display == nullptr)
	{
		createDisplay();
	}
}