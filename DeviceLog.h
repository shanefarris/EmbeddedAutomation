#if 0
#ifndef devicelog_h
#define devicelog_h

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

#define LOG_SIZE 48

class DeviceLog
{
private:
	String Date[LOG_SIZE];
	String Data[LOG_SIZE];

public:
	bool load();
	bool logData(const String& date, const String& data);
	bool populateHtml(String& html);
	void print();
};

#endif // devicelog_h
#endif