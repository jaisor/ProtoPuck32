#include <Arduino.h>
#include <EEPROM.h>
#include "Configuration.h"

configuration_t configuration;

void EEPROM_saveConfig() {
  EEPROM.put(EEPROM_CONFIGURATION_START, configuration);
}

void EEPROM_loadConfig() {
  EEPROM.get(EEPROM_CONFIGURATION_START, configuration);

  if (!configuration._loaded) {
    // blank
    Serial.println("Configuration was blank, loading defaults");
    #ifdef LED
      configuration.ledBrightness = LED_BRIGHTNESS;
    #endif
  }
}

void EEPROM_wipe() {
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
}