#include "NasEngine.h"
#include <ESP8266httpUpdate.h>

DeviceManager deviceManager;
auto wifiConnection = WifiConnection::Instance();
auto webServer = NasWebServer::Instance();
auto oledExpansionModule = OledExpansionModule::Instance();

int firmwareCheck = 0;
int registerUpdate = 0;

void setup()
{
  delay(1000);
  Serial.begin(9600);

  Configuration::Instance()->loadConfig();
  Configuration::Instance()->print();

  if(wifiConnection->setup() && 
     deviceManager.setup() && 
     webServer->setup())
  {
    while(HttpClient::Instance()->registerDevice() == false)
    {
      delay(1000);
    }
    Serial.println("System startup complete.");
  }
  else
  {
    Serial.println("Error starting system.");
  }

  CommandManager::Instance()->onDeviceStart();
}

void loop()
{
  if(registerUpdate >= 1000)
  {
    HttpClient::Instance()->registerDevice();
    registerUpdate = 0;
  }
  registerUpdate++;
  
  firmwareCheck++;
  if(firmwareCheck >= 10)
  {
    HttpClient::Instance()->checkFirmwareVersion();
    firmwareCheck = 0;
  }

  webServer->handleClient();
  deviceManager.update();
  oledExpansionModule->update();
  CommandManager::Instance()->onDeviceLoop();
  
  delay(500);
  
  //if(wifiConnection->IsConnected() == false)
  //{
  //  ESP.reset();
  //}
}

