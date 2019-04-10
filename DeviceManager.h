#ifndef device_manager_h
#define device_manager_h

#include "IDevice.h"

class SimpleTimer;

// There are different sensors for these measurments, but this will not support multiple sensors
// for the same measurement.  A sensor will be picked and stuck with unless there are issues with it.
enum DeviceType
{
	temp,
	temp_humidity,
	water_level,
	water_flow,
	flood,
	water_temp,
	HCSR04,
	relay_single,
	motion,
	water_float,
	dummy
};

class DeviceManager
{
private:
	static DeviceType	  _deviceType;
	static String         _lastReading;
	static IDevice*		  _device;
	static SimpleTimer*   _timer;

	static int     _timerRead;
	static int     _timerLogDevice;
	static int     _timerEmailNoticeCountdown;
	static int     _timerSmsNoticeCountdown;
	static int	   _timerDeviceUpdate;

	static void deviceReadHandle();
	static void deviceLogHandle();
	static void deviceUpdateHandle();
	static void checkDeviceParameters();
	static void sendEmailNotice(DeviceMessageType deviceMessageType);
	static void sendSmsNotice(DeviceMessageType deviceMessageType);
	static void turnOffEmailNotice();
	static void turnOffSmsNotice();

public:
	bool setup();
	String read();                  // Tied to the max and min range check.
	
	static String readAll();        // Used when device has mutiple readings, but is not checked against range.
	static String getLastRead() { return _lastReading; }
	static String indexComponent();
	static bool switchDevice(const String& value);
	static DeviceType	getDeviceType() { return _deviceType; }
	static IDevice*		getDeviceReader() { return _device; }

	void update();
};

#endif // device_manager_h
