#include <Arduino.h>
#include <EEPROM.h>
#include "Configuration.h"

configuration_t configuration;

void EEPROM_saveConfig() {
  log_i("Saving configuration to EEPROM");
  EEPROM.put(EEPROM_CONFIGURATION_START, configuration);
  EEPROM.commit();
}

void EEPROM_loadConfig() {

  memset(&configuration, 0, sizeof(configuration_t));
  EEPROM.begin(sizeof(configuration_t));
  EEPROM.get(EEPROM_CONFIGURATION_START, configuration);

  if (!configuration._loaded) {
    // blank
    log_i("Blank configuration, loading defaluts");
    configuration._loaded = true;
    #ifdef LED
      configuration.ledBrightness = LED_BRIGHTNESS;
    #endif
  }
}

void EEPROM_wipe() {
  log_w("Wiping configuration!");
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
}