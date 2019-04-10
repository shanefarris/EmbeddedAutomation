#ifndef commandmanager_h
#define commandmanager_h

#include <Arduino.h>

class CommandManager
{
public:
	static CommandManager* Instance();

	bool onMaxReached();		// Event1
	bool onMinReached();		// Event2
	bool onDeviceStart();		// Event3
	bool onDeviceLoop();		// Event4
	bool onDeviceReadSuccess();	// Event5
	bool onDeviceReadFail();	// Event6
	bool onFirmwareSuccess();	// Event7
	bool onFirmwareFail();		// Event8
	bool onDataSuccess();		// Event9
	bool onDataFail();			// Event10

private:
	static CommandManager* _commandManager;

	CommandManager();
};

#endif // commandmanager_h
