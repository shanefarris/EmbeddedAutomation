#include "CommandManager.h"
#include "Configuration.h"

CommandManager* CommandManager::_commandManager = nullptr;

CommandManager* CommandManager::Instance()
{
	if (_commandManager == nullptr)
		_commandManager = new CommandManager();
	return _commandManager;
}

CommandManager::CommandManager()
{
	// Create tasks
	if (Configuration::Instance()->event1 != "")
	{

	}
}

bool CommandManager::onMaxReached()
{
	DEBUG_PRINTLN("CommandManager::onMaxReached");
	return true;
}

bool CommandManager::onMinReached()
{
	DEBUG_PRINTLN("CommandManager::onMinReached");
	return true;
}

bool CommandManager::onDeviceStart()
{
	DEBUG_PRINTLN("CommandManager::onDeviceStart");
	return true;
}

bool CommandManager::onDeviceLoop()
{
	DEBUG_PRINTLN("CommandManager::onDeviceLoop");
	return true;
}

bool CommandManager::onDeviceReadSuccess()
{
	DEBUG_PRINTLN("CommandManager::onDeviceReadSuccess");
	return true;
}

bool CommandManager::onDeviceReadFail()
{
	DEBUG_PRINTLN("CommandManager::onDeviceReadFail");
	return true;
}

bool CommandManager::onFirmwareSuccess()
{
	DEBUG_PRINTLN("CommandManager::onFirmwareSuccess");
	return true;
}

bool CommandManager::onFirmwareFail()
{
	DEBUG_PRINTLN("CommandManager::onFirmwareFail");
	return true;
}

bool CommandManager::onDataSuccess()
{
	DEBUG_PRINTLN("CommandManager::onDataSuccess");
	return true;
}

bool CommandManager::onDataFail()
{
	DEBUG_PRINTLN("CommandManager::onDataFail");
	return true;
}