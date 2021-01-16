#if !( defined(ESP32) )
  #error This code is intended to run on ESP32 platform! Please check your Tools->Board setting.
#endif


#include "WifiManager.h"
#include <WiFi.h>
#include <Arduino.h>

void setupWifi() {

    uint32_t chipId = 0;
    for(int i=0; i<17; i=i+8) {
	  chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
	}

    Serial.print("Chip ID: "); Serial.println(chipId);

    char uniqueSsid[100] = "";
    sprintf_P(uniqueSsid, "%s_%i", DEFAULT_SSID, chipId);

    Serial.print("Setting AP (Access Point): "); Serial.print(uniqueSsid);
    WiFi.softAP(uniqueSsid, DEFAULT_PASS);


}