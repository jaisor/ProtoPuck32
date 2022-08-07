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

unsigned long tsSmoothBoot;
bool smoothBoot;

float p = 0;

void setup() {
  delay( 1000 ); // power-up safety delay

  Serial.begin(115200);
  while(!Serial && !Serial.available()){}
  randomSeed(analogRead(0));

  if (EEPROM_initAndCheckFactoryReset() >= 3) {
    log_w("Factory reset conditions met!");
    EEPROM_wipe();    
  }

  tsSmoothBoot = millis();
  smoothBoot = false;

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