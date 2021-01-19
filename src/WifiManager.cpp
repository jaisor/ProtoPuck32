#if !( defined(ESP32) )
  #error This code is intended to run on ESP32 platform! Please check your Tools->Board setting.
#endif

#include <Arduino.h>
#include "WifiManager.h"
#include "Configuration.h"

#define MAX_CONNECT_TIMEOUT_MS 20000

const int RSSI_MAX =-50;// define maximum straighten of signal in dBm
const int RSSI_MIN =-100;// define minimum strength of signal in dBm

int dBmtoPercentage(int dBm) {
  int quality;
  if(dBm <= RSSI_MIN) {
    quality = 0;
  } else if(dBm >= RSSI_MAX) {  
    quality = 100;
  } else {
    quality = 2 * (dBm + 100);
  }
  return quality;
}

static const uint8_t icon_wifi[8] = {
    0, 0x3c, 0x42, 0x81, 0x3c, 0x42, 0x18, 0x0
};

CWifiManager::CWifiManager() {

  tMillis = millis();
  status = WF_CONNECTING;
  connect();
}

uint16_t CWifiManager::OLED_Status(Adafruit_GFX *oled) {

  wl_status_t s = WiFi.status();
  
  oled->setTextSize(1);

  if (s == WL_CONNECTED) {

    listen();

    tMillis = millis();
    oled->drawBitmap(0, 0, icon_wifi, 8, 8, 1);

    char buf[100];
    int32_t signalPercentage = dBmtoPercentage(WiFi.RSSI());

    sprintf(buf, "%s %i%%", WiFi.SSID().c_str(), signalPercentage);
    oled->setCursor(10,0);
    oled->print(buf);

    IPAddress ip = WiFi.localIP();
    sprintf(buf, "IP: %s", ip.toString().c_str());
    oled->setCursor(0,8);
    oled->print(buf);
  } else if(s == WL_NO_SHIELD) {

    listen();
    
    tMillis = millis();
    oled->drawBitmap(0, 0, icon_wifi, 8, 8, 1);

    char buf[100];
    
    oled->setCursor(10,0);
    sprintf(buf, "%s (%i)", softAP_SSID, WiFi.softAPgetStationNum());
    oled->print(buf);

    IPAddress ip = WiFi.softAPIP();
    sprintf(buf, "IP: %s", ip.toString().c_str());
    oled->setCursor(0,8);
    oled->print(buf);

  } else  {

    if (status == WF_CONNECTING) {
      unsigned long dt = millis() - tMillis;
      oled->drawBitmap(0, 0, icon_wifi, 8, 8, 1);

      oled->setCursor(10,0);
      oled->print(configuration.wifi_ssid);
      
      oled->drawRect(1, 9, OLED_SCREEN_WIDTH-1, 7, 1);

      uint8_t w = dt * (OLED_SCREEN_WIDTH-5) / MAX_CONNECT_TIMEOUT_MS;
      if (dt > MAX_CONNECT_TIMEOUT_MS) {
        w = OLED_SCREEN_WIDTH-5;
      }
      oled->fillRect(3, 11, w, 3, 1);
    } else {
      connect();
    }

  }  
  
  //

  return 100;
}

void CWifiManager::connect() {

  status = WF_CONNECTING;
  strcpy(softAP_SSID, "");

   if (strlen(configuration.wifi_ssid)) {

    // Join AP from Config
    Serial.print("Connecting to WiFi ");
    Serial.println(configuration.wifi_ssid);
    WiFi.begin(configuration.wifi_ssid, configuration.wifi_password);
    
  } else {

    // Create AP using fallback and chip ID
    uint32_t chipId = 0;
    for(int i=0; i<17; i=i+8) {
      chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
    }

    Serial.print("Chip ID: "); Serial.println(chipId);

    sprintf_P(softAP_SSID, "%s_%i", WIFI_FALLBACK_SSID, chipId);

    Serial.print("Creating WiFi "); Serial.print(softAP_SSID);
    WiFi.softAP(softAP_SSID, WIFI_FALLBACK_PASS);

  }
  
}

void CWifiManager::listen() {

  if (status != WF_LISTENING) {
    status = WF_LISTENING;
    server.begin(WEB_SERVER_PORT);
    Serial.print("Web server listening on port "); Serial.println(WEB_SERVER_PORT);
    return;
  }

  if (server.hasClient()) {
    WiFiClient client = server.available();
    client.setTimeout(2);
    Serial.print("New client from: "); Serial.println(client.remoteIP());
    while (client.connected()) {
      char c = client.read();
      Serial.write(c);   
    }
    client.stop();
  }  else {
    Serial.print(".");
  }
  
  
}