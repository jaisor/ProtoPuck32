#include <Arduino.h>
#include <EEPROM.h>
#include "Configuration.h"

configuration_t configuration;

int EEPROM_saveConfig() {
  EEPROM.put(EEPROM_CONFIGURATION_START, configuration);
}

int EEPROM_loadConfig() {
  EEPROM.get(EEPROM_CONFIGURATION_START, configuration);
}

int EEPROM_wipe() {
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
}