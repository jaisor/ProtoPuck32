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

  log_i("SSID: '%s'", configuration.wifiSsid);
  log_i("loaded: '%i'", configuration._loaded);

  if (!configuration._loaded) {
    configuration._loaded = true;
    log_i("loaded: '%i'", configuration._loaded);
    // blank
    log_i("Blank configuration, loading defaluts");
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