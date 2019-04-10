#ifndef OledExpansionModule_h
#define OledExpansionModule_h

#include <Arduino.h>
#include "SSD1306.h"

class OledExpansionModule
{
private:
	OledExpansionModule();
	static OledExpansionModule* _oledExpansionModule;

	const uint8_t CHECKPIN = D7;     // is the oled connected, then this should be high
	const uint8_t sda = D3;
	const uint8_t scl = D5;
	SSD1306* display = nullptr;

	void createDisplay();

public:
	void update();
	void message(const String& text);

	static OledExpansionModule* Instance();
};

#endif // OledExpansionModule_h
