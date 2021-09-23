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
      strcpy(configuration.ntpServer, NTP_SERVER);
      configuration.gmtOffset_sec = NTP_GMT_OFFSET_SEC;
      configuration.daylightOffset_sec = NTP_DAYLIGHT_OFFSET_SEC;
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
      log_v("Bad SSID, loading default", wifiStr);
      strcpy(configuration.wifiSsid, "");
      break;
    }
  }
#endif

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