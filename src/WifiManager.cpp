#if !( defined(ESP32) )
  #error This code is intended to run on ESP32 platform! Please check your Tools->Board setting.
#endif

#include <Arduino.h>
#include "WifiManager.h"
#include "Configuration.h"

#define MAX_CONNECT_TIMEOUT_MS 10000 // 10 seconds to connect before creating its own AP
#define BOARD_LED_PIN 2

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

const unsigned char icon_wifi [] PROGMEM = {
	0x00, 0x00, 0x70, 0x00, 0x7e, 0x00, 0x07, 0x80, 0x01, 0xc0, 0x70, 0xe0, 0x7c, 0x30, 0x0e, 0x38, 
	0x03, 0x18, 0x61, 0x8c, 0x78, 0xcc, 0x1c, 0xc4, 0x0c, 0x66, 0x46, 0x66, 0x66, 0x66, 0x00, 0x00
};

const unsigned char icon_ip [] PROGMEM = {
	0x0, 0xee, 0x49, 0x49, 0x4e, 0x48, 0xe8, 0x0
};

CWifiManager::CWifiManager() {

  pinMode(BOARD_LED_PIN,OUTPUT);
  tMillis = millis();
  connect();
}

uint16_t CWifiManager::OLED_Status(Adafruit_GFX *oled) {

  wl_status_t s = WiFi.status();
  
  oled->setTextSize(1);
  oled->drawBitmap(0, 0, icon_wifi, 16, 16, 1);

  //Serial.print("Status:"); Serial.println(String(s));
  if (s == WL_CONNECTED || s == WL_NO_SHIELD) {

    tMillis = millis();
    char buf[100];
    int32_t signalPercentage = dBmtoPercentage(WiFi.RSSI());

    if (s == WL_CONNECTED) {
      sprintf(buf, "%s %i%%", WiFi.SSID().c_str(), signalPercentage);
    } else if (s == WL_NO_SHIELD) {
      sprintf(buf, "%s (%i)", softAP_SSID, WiFi.softAPgetStationNum());
    }
    
    oled->setCursor(18,0);
    oled->print(buf);

    oled->drawBitmap(18, 8, icon_ip, 8, 8, 1);
    oled->setCursor(28,8);
    oled->print((WL_CONNECTED ? WiFi.localIP() : WiFi.softAPIP()).toString().c_str());
  } else  {

    if (status == WF_CONNECTING) {
      unsigned long dt = millis() - tMillis;
      
      oled->setCursor(18,0);
      oled->print(configuration.wifi_ssid);
      
      oled->drawRect(18, 9, OLED_SCREEN_WIDTH-19, 7, 1);

      uint8_t w = dt * (OLED_SCREEN_WIDTH-21) / MAX_CONNECT_TIMEOUT_MS;
      if (dt > MAX_CONNECT_TIMEOUT_MS) {
        w = OLED_SCREEN_WIDTH-21;
        strcpy(configuration.wifi_ssid, "");
        tMillis = millis();
        connect();
      }
      oled->fillRect(20, 11, w, 3, 1);
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

  digitalWrite(BOARD_LED_PIN, LOW);
  
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

    Serial.print("Creating WiFi "); Serial.println(softAP_SSID);
    WiFi.softAP(softAP_SSID, WIFI_FALLBACK_PASS);

  }
  
}

void CWifiManager::listen() {

  digitalWrite(BOARD_LED_PIN, HIGH);
  
  status = WF_LISTENING;
  server.on("/", std::bind(&CWifiManager::handleRoot, this));
  server.on("/connect", HTTP_POST, std::bind(&CWifiManager::handleConnect, this));
  server.begin(WEB_SERVER_PORT);
  Serial.print("Web server listening on port "); Serial.println(WEB_SERVER_PORT);
  
}

void CWifiManager::loop() {
  if (WiFi.status() == WL_CONNECTED || WiFi.status() == WL_NO_SHIELD) {
    if (status != WF_LISTENING) {
      listen();
    } else {
      server.handleClient();
    }
  } else {
    server.close();
    status = WF_CONNECTING;
  }
  
}

void CWifiManager::handleRoot() {
  digitalWrite(BOARD_LED_PIN, LOW);
  
  char temp[1000];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf(temp, 1000,

           "<html>\
  <head>\
    <title>ESP32 Demo</title>\
    <style>\
      body { background-color: #303030; font-family: 'Anaheim',sans-serif; Color: #d8d8d8; }\
    </style>\
  </head>\
  <body>\
    <h1>ProtoPuck32</h1>\
    <p>Connect to WiFi Access Point (AP)</p>\
    <form method='POST' action='/connect'>\
      <label for='ssid'>SSID (AP Name):</label><br>\
      <input type='text' id='ssid' name='ssid'><br><br>\
      <label for='pass'>Password (WPA2):</label><br>\
      <input type='password' id='pass' name='pass' minlength='8' required><br><br>\
      <input type='submit' value='Connect...'>\
    </form>\
    <br><br><hr>\
    <p>Uptime: %02d:%02d:%02d</p>\
  </body>\
</html>",

           hr, min % 60, sec % 60
          );
  server.send(200, "text/html", temp);

  digitalWrite(BOARD_LED_PIN, HIGH);
  
}

void CWifiManager::handleConnect() {
  digitalWrite(BOARD_LED_PIN, LOW);
  
  char temp[1000];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf(temp, 1000,

           "<html>\
  <head>\
    <title>ESP32 Demo</title>\
    <style>\
      body { background-color: #303030; font-family: 'Anaheim',sans-serif; Color: #d8d8d8; }\
    </style>\
  </head>\
  <body>\
    <h1>ProtoPuck32</h1>\
    <p>Connecting...</p>\
    <p>Uptime: %02d:%02d:%02d</p>\
  </body>\
</html>",

           hr, min % 60, sec % 60
          );
  server.send(200, "text/html", temp);

  digitalWrite(BOARD_LED_PIN, HIGH);
  
}