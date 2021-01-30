#if !( defined(ESP32) )
  #error This code is intended to run on ESP32 platform! Please check your Tools->Board setting.
#endif

#include <Arduino.h>
#include <WiFiClient.h>
#include <esp_wifi.h>

#include "WifiManager.h"
#include "Configuration.h"

#define MAX_CONNECT_TIMEOUT_MS 10000 // 10 seconds to connect before creating its own AP
#define BOARD_LED_PIN 2

const int RSSI_MAX =-50;// define maximum straighten of signal in dBm
const int RSSI_MIN =-100;// define minimum strength of signal in dBm

WiFiClient espClient;

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

void handle_mqtt_message(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  char message[50];

  /*
  ByteToChar(payload, message, length);
  Serial.println(message);

  if (!strcmp(topic, "redacted")) {
    handle_redled(message, length);
  }

  if (!strcmp(topic, "redacted")) {
    handle_relay(message, length);
  }
  */
}

CWifiManager::CWifiManager() {    
  pinMode(BOARD_LED_PIN,OUTPUT);
  this->client.setClient(espClient);
  strcpy(SSID, configuration.wifiSsid);

  connect();
}

#ifdef OLED
uint16_t CWifiManager::OLED_Status(Adafruit_GFX *oled) {

  wl_status_t s = WiFi.status();
  
  oled->setTextSize(1);
  oled->drawBitmap(0, 0, icon_wifi, 16, 16, 1);

  wifi_mode_t espm;
  esp_wifi_get_mode(&espm);

  if (s == WL_CONNECTED || espm == WIFI_MODE_AP || espm == WIFI_MODE_APSTA) {

    digitalWrite(BOARD_LED_PIN, status == WF_LISTENING ? HIGH : LOW);

    char buf[100];
    int32_t signalPercentage = dBmtoPercentage(WiFi.RSSI());

    if (s == WL_CONNECTED) {
      sprintf(buf, "%s %i%%", WiFi.SSID().c_str(), signalPercentage);
    } else if (espm == WIFI_MODE_AP || espm == WIFI_MODE_APSTA) {
      sprintf(buf, "%s (%i)", softAP_SSID, WiFi.softAPgetStationNum());
    }
    
    oled->setCursor(18,0);
    oled->print(buf);

    oled->drawBitmap(18, 8, icon_ip, 8, 8, 1);
    oled->setCursor(28,8);
    oled->print((s == WL_CONNECTED ? WiFi.localIP() : WiFi.softAPIP()).toString().c_str());

  } else if (status == WF_CONNECTING) {

    unsigned long dt = millis() - tMillis;
    
    oled->setCursor(18,0);
    oled->print(SSID);
    
    oled->drawRect(18, 9, OLED_SCREEN_WIDTH-19, 7, 1);

    uint8_t w = dt * (OLED_SCREEN_WIDTH-21) / MAX_CONNECT_TIMEOUT_MS;
    if (dt >= MAX_CONNECT_TIMEOUT_MS) {
      w = OLED_SCREEN_WIDTH-22;
    }
    oled->fillRect(20, 11, w, 3, 1);

    digitalWrite(BOARD_LED_PIN, dt % 100 ? LOW : HIGH);
  }  else {
    digitalWrite(BOARD_LED_PIN, LOW);
  }
  
  //

  return 100;
}
#endif

void CWifiManager::connect() {

  status = WF_CONNECTING;
  strcpy(softAP_SSID, "");
  tMillis = millis();

  if (strlen(SSID)) {

    // Join AP from Config
    log_d("Connecting to WiFi: '%s'", SSID);
    WiFi.begin(SSID, configuration.wifiPassword);
    
  } else {

    // Create AP using fallback and chip ID
    uint32_t chipId = 0;
    for(int i=0; i<17; i=i+8) {
      chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
    }

    log_d("Chip ID: '%i'", chipId);

    sprintf_P(softAP_SSID, "%s_%i", WIFI_FALLBACK_SSID, chipId);
    log_d("Creating WiFi: '%s'", softAP_SSID);
    
    WiFi.softAP(softAP_SSID, WIFI_FALLBACK_PASS);
  }
  
}

void CWifiManager::listen() {

  status = WF_LISTENING;

  // Web
  server.on("/", std::bind(&CWifiManager::handleRoot, this));
  server.on("/connect", HTTP_POST, std::bind(&CWifiManager::handleConnect, this));
  server.begin(WEB_SERVER_PORT);
  log_d("Web server listening on port %i", WEB_SERVER_PORT);
  
  // MQTT
  client.setServer("192.168.10.10", 1883);
  client.setCallback(handle_mqtt_message);
  
}

void CWifiManager::loop() {
  if (WiFi.status() == WL_CONNECTED || WiFi.status() == WL_NO_SHIELD) {
    // WiFi is connected

    if (status == WF_LISTENING) {  
      // Handle requests
      server.handleClient();
    } else {
      // Start listening for requests
      listen();
    }

  } else {
    // WiFi is down

    switch (status) {
      case WF_LISTENING: {
        log_d("Disconnecting");
        server.close();
        client.disconnect();
        status = WF_CONNECTING;
        connect();
      } break;
      case WF_CONNECTING: {
        if (millis() - tMillis > MAX_CONNECT_TIMEOUT_MS) {
          log_d("Connecting failed, create an AP instead");
          esp_wifi_stop();

          tMillis = millis();
          strcpy(SSID, "");

          connect();
        }
      } break;

    }

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
    <form method='POST' action='/connect' enctype='application/x-www-form-urlencoded'>\
      <label for='ssid'>SSID (AP Name):</label><br>\
      <input type='text' id='ssid' name='ssid'><br><br>\
      <label for='pass'>Password (WPA2):</label><br>\
      <input type='password' id='pass' name='password' minlength='8' autocomplete='off' required><br><br>\
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

  //String postBody = server.arg("plain");
  String ssid = server.arg("ssid");
  String password = server.arg("password");
  
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
    <p>Connecting to '%s' ... see you on the other side!</p>\
    <p>Uptime: %02d:%02d:%02d</p>\
  </body>\
</html>",
    ssid,
    hr, min % 60, sec % 60
  );

  server.send(200, "text/html", temp);

  ssid.toCharArray(configuration.wifiSsid, sizeof(configuration.wifiSsid));
  password.toCharArray(configuration.wifiPassword, sizeof(configuration.wifiPassword));

  log_i("Saved config SSID: '%s'", configuration.wifiSsid);

  EEPROM_saveConfig();

  strcpy(SSID, configuration.wifiSsid);
  connect();

  digitalWrite(BOARD_LED_PIN, HIGH);
  
}