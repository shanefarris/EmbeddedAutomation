/*
 *  Add/Modify configuration settings, you will need to update:
 *  Header
 *  Configuration::loadConfig
 *  Configuration::populateConfigHtml
 *  Configuration::writeConfiguration
 *  Configuration::print
 *  NasWebServer::renderPostConfigHtml
 *  config
 *  config.html
 */
#ifndef configuration_h
#define configuration_h

#include <Arduino.h>

//#define SERVICES_BUILD 1
//#define STORE_BUILD 1
#define PUBLIC_BUILD 1
//#define TEST_BUILD 1

#define DEBUG 1
#define DPRINT 0

 // Define where debug output will be printed.
#define DEBUG_PRINTER Serial

// Setup debug printing macros.
#ifdef DEBUG
#define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
#define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
#define DEBUG_PRINT(...) {}
#define DEBUG_PRINTLN(...) {}
#endif

#define SEC 1000
#define MIN 60 * SEC
#define HOUR MIN * 60

class Configuration
{
private:
	Configuration();
	static Configuration* _configuration;

public:
	String ssid;					// Wifi network to connect to
	String psk;						// Wifi password
	String deviceType;				// Type of logic defines the sensor, how to handle the sensor
	String phone;					// Contact (SMS) number
	String maxCheck;				// Do we check for max value
	String minCheck;				// Do we check for min value
	String sendEmail;				// Do we send an email if device is triggered
	String sendSms;					// Bool, do we send SMS on device triggered
	String maxRange;				// Max value before event (like notificatons) is triggered
	String minRange;				// Min value before event (like notificatons) is triggered
	String deviceName;				// Name of device, if not defined then we use the MAC address
	String deviceInterval;			// How often the device is read
	String saveIntervalMinute;		// Every time we case device read values (not used for now)
	String toEmail;					// Email for notifications
	String fromEmail;				// From address for sending emails
	String emailPassword;			// If email server uses base64 password (used for gmail)
	String emailHost;				// Email server
	String emailPort;				// Port if email server requires custom port
	String standalonePassword;		// Password if used as an AP
	String catalogHost;				// Server for registering the device
	String version;					// Current firmware version installed
	String stateHost;				// URL to return the state devices should be at
	String firmwareVersion;			// Used to check the current firmware version
	String event1;					// Minimum Range
	String event2;					// Maximum Range
	String event3;					// Device Start
	String event4;					// Device Loop
	String event5;					// Device Read Success
	String event6;					// Device Read Fail
	String event7;					// Firmware Update Success
	String event8;					// Firmware Update Fail
	String event9;					// Data Update Success
	String event10;					// Data Update Fail

	static Configuration* Instance();

	bool loadConfig();
	bool populateConfigHtml(String& html);
	bool populateTasksHtml(String& html);
	bool writeConfiguration();
	bool writeTasks();
	void print();
};

#endif // configuration_h
