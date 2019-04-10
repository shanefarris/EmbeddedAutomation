#ifndef icontroller_h
#define icontroller_h

#include <Arduino.h>

#include "Configuration.h"

enum ControllerErrorCode
{
	CONTROLLER_OK = 0,
	ERROR_CONNECTION = -1,
	ERROR_TIMEOUT = -2,
	INVALID_VALUE = -999
};

class IController
{
public:
	IController() { }
	virtual ControllerErrorCode execute(String[] params) = 0;
	virtual String getName() = 0;
};

#endif // icontroller_h