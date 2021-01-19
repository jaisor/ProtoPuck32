#include <Arduino.h>
#include <EEPROM.h>
#include "Configuration.h"

configuration_t configuration;

void EEPROM_saveConfig() {
  EEPROM.put(EEPROM_CONFIGURATION_START, configuration);
}

void EEPROM_loadConfig() {
  EEPROM.get(EEPROM_CONFIGURATION_START, configuration);
}

void EEPROM_wipe() {
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
}