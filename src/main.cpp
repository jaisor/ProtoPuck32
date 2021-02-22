#include <Arduino.h>

#include <functional>

#if !( defined(ESP32) )
  #error This code is intended to run on ESP32 platform! Please check your Tools->Board setting.
#endif


#include "Configuration.h"
#include "Device.h"
#include "StateController.h"

CDevice *device;
CStateController *stateController;

float p = 0;

void setup() {
  Serial.begin(115200); while (!Serial); delay(200);

  EEPROM_loadConfig();

  //strcpy(configuration.wifiSsid, "<REDACTED>");
  //strcpy(configuration.wifiPassword, "<REDACTED>");
  //EEPROM_saveConfig();

  device = new CDevice();
  stateController = new CStateController(device);
}

void loop() {
  stateController->loop();
  device->loop();
}