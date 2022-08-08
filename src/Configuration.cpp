#include <Arduino.h>
#include <EEPROM.h>
#include "Configuration.h"

configuration_t configuration;

uint8_t EEPROM_initAndCheckFactoryReset() {
  log_i("Configuration size: %i", sizeof(configuration_t));
  
  EEPROM.begin(sizeof(configuration_t) + EEPROM_FACTORY_RESET + 1);
  uint8_t resetCounter = EEPROM.read(EEPROM_FACTORY_RESET);

  log_i("Factory reset counter: %i", resetCounter);
  log_v("EEPROM length: %i", EEPROM.length());

  // Bump reset counter
  EEPROM.write(EEPROM_FACTORY_RESET, resetCounter + 1);
  EEPROM.commit();

  return resetCounter;
}

void EEPROM_clearFactoryReset() {
  EEPROM.write(EEPROM_FACTORY_RESET, 0);
  EEPROM.commit();
}

void EEPROM_saveConfig() {
  log_i("Saving configuration to EEPROM");
  EEPROM.put(EEPROM_CONFIGURATION_START, configuration);
  EEPROM.commit();
}

void EEPROM_loadConfig() {

  configuration = {};
  EEPROM.get(EEPROM_CONFIGURATION_START, configuration);

  log_i("Configuration loaded: [%s]", configuration._loaded);

  if (strcmp(configuration._loaded, "jaisor")) {
    // blank
    log_i("Blank configuration, loading defaults");
    strcpy(configuration._loaded, "jaisor");
    strcpy(configuration.name, DEVICE_NAME);
    #ifdef LED
      configuration.ledBrightness = LED_BRIGHTNESS;
    #endif
    #ifdef WIFI
      strcpy(configuration.ntpServer, NTP_SERVER);
      configuration.gmtOffset_sec = NTP_GMT_OFFSET_SEC;
      configuration.daylightOffset_sec = NTP_DAYLIGHT_OFFSET_SEC;
      strcpy(configuration.mqttServer, "");
      configuration.mqttPort = 1883;
      strcpy(configuration.mqttTopic, "");
    #endif
  }

#ifdef LED
  if (isnan(configuration.ledBrightness)) {
    log_v("NaN brightness");
    configuration.ledBrightness = LED_BRIGHTNESS;
  }
#endif

#ifdef WIFI
  String wifiStr = String(configuration.wifiSsid);
  for (auto i : wifiStr) {
    if (!isAscii(i)) {
      log_v("Bad SSID, loading default: %s", wifiStr.c_str());
      strcpy(configuration.wifiSsid, "");
      break;
    }
  }
#endif

  log_i("Device name: %s", configuration.name);

  // FIXME: Always default NTP values
  strcpy(configuration.ntpServer, NTP_SERVER);
  configuration.gmtOffset_sec = NTP_GMT_OFFSET_SEC;
  configuration.daylightOffset_sec = NTP_DAYLIGHT_OFFSET_SEC;
  configuration.ledBrightnessTime = configuration.ledBrightness;

}

void EEPROM_wipe() {
  log_w("Wiping configuration!");
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
}

#ifdef LED
void CONFIG_updateLedBrightnessTime() {
  struct tm timeinfo;
  bool timeUpdated = getLocalTime(&timeinfo);
  if (timeUpdated) {
    
    float dimmer = 1;
    if (timeinfo.tm_hour >= 20 && timeinfo.tm_hour < 21) {
      dimmer = 0.2;
    } else if (timeinfo.tm_hour >= 21 || timeinfo.tm_hour < 7) {
      dimmer = 0;
    }

    configuration.ledBrightnessTime = configuration.ledBrightness * dimmer;
  }
}
#endif