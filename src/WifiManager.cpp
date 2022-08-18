#if !( defined(ESP32) )
  #error This code is intended to run on ESP32 platform! Please check your Tools->Board setting.
#endif

#include <Arduino.h>
#include <WiFiClient.h>
#include <esp_wifi.h>
#include <ezTime.h>

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

const String htmlTop FL_PROGMEM = "<html>\
  <head>\
    <title>%s</title>\
    <style>\
      body { background-color: #303030; font-family: 'Anaheim',sans-serif; Color: #d8d8d8; }\
    </style>\
  </head>\
  <body>\
    <h1>%s ProtoPuck32</h1>";

const String htmlBottom FL_PROGMEM = "<br><br><hr>\
  <p>Uptime: %02d:%02d:%02d | Device: %s</p>\
  %s\
  </body>\
</html>";

const String htmlWifiApConnectForm FL_PROGMEM = "<h2>Connect to WiFi Access Point (AP)</h2>\
    <form method='POST' action='/connect' enctype='application/x-www-form-urlencoded'>\
      <label for='ssid'>SSID (AP Name):</label><br>\
      <input type='text' id='ssid' name='ssid'><br><br>\
      <label for='pass'>Password (WPA2):</label><br>\
      <input type='password' id='pass' name='password' minlength='8' autocomplete='off' required><br><br>\
      <input type='submit' value='Connect...'>\
    </form>";

const String htmlDeviceConfigs FL_PROGMEM = "<hr><h2>Configs</h2>\
    <form method='POST' action='/config' enctype='application/x-www-form-urlencoded'>\
      <label for='deviceName'>Device name:</label><br>\
      <input type='text' id='deviceName' name='deviceName' value='%s'><br>\
      <br>\
      <label for='mqttServer'>MQTT Server:</label><br>\
      <input type='text' id='mqttServer' name='mqttServer' value='%s'><br>\
      <label for='mqttPort'>MQTT Port:</label><br>\
      <input type='text' id='mqttPort' name='mqttPort' value='%i'><br>\
      <label for='mqttTopic'>MQTT Topic:</label><br>\
      <input type='text' id='mqttTopic' name='mqttTopic' value='%s'><br>\
      <br>\
      <input type='submit' value='Set...'>\
    </form>";

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

char _deviceId[32];
char *getDeviceId() {
  
  uint32_t chipId = 0;
    for(int i=0; i<17; i=i+8) {
      chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
    }

  log_d("Chip ID: '%i'", chipId);

  sprintf_P(_deviceId, "%s_%i", WIFI_FALLBACK_SSID, chipId);

  return _deviceId;
}

CWifiManager::CWifiManager(CDevice * const device)
:apMode(false), rebootNeeded(false), device(device) {    
  pinMode(BOARD_LED_PIN,OUTPUT);
  this->client.setClient(espClient);
  strcpy(SSID, configuration.wifiSsid);
  this->server = new AsyncWebServer(WEB_SERVER_PORT);
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
    apMode = false;
    
  } else {

    strcpy(softAP_SSID, getDeviceId());
    log_i("Creating WiFi: '%s' / '%s'", softAP_SSID, WIFI_FALLBACK_PASS);
    
    if (WiFi.softAP(softAP_SSID, WIFI_FALLBACK_PASS)) {
      apMode = true;
      log_i("Wifi AP '%s' created, listening on '%s'", softAP_SSID, WiFi.softAPIP().toString().c_str());
    } else {
      log_e("Wifi AP faliled");
    };
    
    WiFi.softAP(softAP_SSID, WIFI_FALLBACK_PASS);
  }
  
}

void CWifiManager::listen() {

  status = WF_LISTENING;

  // Web
  server->on("/", std::bind(&CWifiManager::handleRoot, this, std::placeholders::_1));
  server->on("/connect", HTTP_POST, std::bind(&CWifiManager::handleConnect, this, std::placeholders::_1));
  server->on("/config", HTTP_POST, std::bind(&CWifiManager::handleConfig, this, std::placeholders::_1));
  server->begin();
  log_d("Web server listening on port %i", WEB_SERVER_PORT);
  
  // MQTT
  client.setServer(configuration.mqttServer, configuration.mqttPort);
  client.setCallback(handle_mqtt_message);

  if (!client.connected()) {
    log_d("Attempting MQTT connection to '%s:%i' ...", configuration.mqttServer, configuration.mqttPort);
    if (client.connect("arduinoClient")) {
      log_i("MQTT connected");

      if (strlen(configuration.mqttTopic)) {
        #ifdef TEMP_SENSOR
          char topic[255];
          sprintf_P(topic, "%s/sensor/temperature", configuration.mqttTopic);
          client.publish(topic,String(device->temperature(), 2).c_str());
          sprintf_P(topic, "%s/sensor/humidity", configuration.mqttTopic);
          client.publish(topic,String(device->humidity(), 2).c_str());
        #endif
      } else {
        log_i("Blank MQTT topic");
      }
      
      // ... and resubscribe
      //client.subscribe("inTopic");
    } else {
      log_d("MQTT connect failed, rc=%i", client.state());
    }
  }

  // NTP
  log_i("Configuring time from %s at %i (%i)", configuration.ntpServer, configuration.gmtOffset_sec, configuration.daylightOffset_sec);

  configTime(configuration.gmtOffset_sec, configuration.daylightOffset_sec, configuration.ntpServer);
  struct tm timeinfo;
  //time()
  if(getLocalTime(&timeinfo)){
    log_i("The time is %i:%i", timeinfo.tm_hour,timeinfo.tm_min);
    CONFIG_updateLedBrightnessTime();
  }
  
}

void CWifiManager::loop() {

  if (rebootNeeded && millis() - tMillis > 200) {
    log_i("Rebooting...");
#ifdef ESP32
    ESP.restart();
#elif ESP8266
    ESP.reset();
#endif
    return;
  }

  if (WiFi.status() == WL_CONNECTED || apMode ) {
    // WiFi is connected

    if (status != WF_LISTENING) {  
      // Start listening for requests
      listen();
    }

    // Send sensor updates periodically
    if (millis() - tMillis > 60000) {
      tMillis = millis();
      // FIXME redundant code
      if (client.connect("arduinoClient")) {
        log_v("Sending MQTT sensor updates");

        if (strlen(configuration.mqttTopic)) {
          #ifdef TEMP_SENSOR
            char topic[255];
            sprintf_P(topic, "%s/sensor/temperature", configuration.mqttTopic);
            client.publish(topic,String(device->temperature(), 2).c_str());
            sprintf_P(topic, "%s/sensor/humidity", configuration.mqttTopic);
            client.publish(topic,String(device->humidity(), 2).c_str());
          #endif
        }
        
        // ... and resubscribe
        //client.subscribe("inTopic");
      }
    }

  } else {
    // WiFi is down

    switch (status) {
      case WF_LISTENING: {
        log_i("Disconnecting %i", status);
        server->end();
        status = WF_CONNECTING;
        connect();
      } break;
      case WF_CONNECTING: {
        if (millis() - tMillis > MAX_CONNECT_TIMEOUT_MS) {
          log_w("Connecting failed (wifi status %i) after %l ms, create an AP instead", (millis() - tMillis), WiFi.status());
          tMillis = millis();
          strcpy(SSID, "");
          connect();
        }
      } break;

    }

  }
  
}

void CWifiManager::handleRoot(AsyncWebServerRequest *request) {

  log_i("handleRoot");
  
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  AsyncResponseStream *response = request->beginResponseStream("text/html");
  response->printf(htmlTop.c_str(), configuration.name, configuration.name);

  if (apMode) {
    response->printf(htmlWifiApConnectForm.c_str());
  } else {
    response->printf("<p>Connected to '%s'</p>", SSID);
  }
  
  String modeOptions = "";
  
  response->printf(htmlDeviceConfigs.c_str(), configuration.name, configuration.mqttServer, 
    configuration.mqttPort, configuration.mqttTopic);

  response->printf(htmlBottom.c_str(), hr, min % 60, sec % 60, String(DEVICE_NAME), getTempSensorResponse().c_str());
  request->send(response);
}

void CWifiManager::handleConnect(AsyncWebServerRequest *request) {

  log_i("handleConnect");

  String ssid = request->arg("ssid");
  String password = request->arg("password");
  
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  AsyncResponseStream *response = request->beginResponseStream("text/html");
  response->printf(htmlTop.c_str(), configuration.name, configuration.name);
  response->printf("<p>Connecting to '%s' ... see you on the other side!</p>", ssid.c_str());
  response->printf(htmlBottom.c_str(), hr, min % 60, sec % 60, String(DEVICE_NAME), getTempSensorResponse().c_str());
  request->send(response);

  ssid.toCharArray(configuration.wifiSsid, sizeof(configuration.wifiSsid));
  password.toCharArray(configuration.wifiPassword, sizeof(configuration.wifiPassword));

  log_i("Saved config SSID: '%s'", configuration.wifiSsid);

  EEPROM_saveConfig();

  strcpy(SSID, configuration.wifiSsid);
  connect();
}

void CWifiManager::handleConfig(AsyncWebServerRequest *request) {

  log_i("handleConfig");

  String deviceName = request->arg("deviceName");
  deviceName.toCharArray(configuration.name, sizeof(configuration.name));
  log_i("Device req name: %s", deviceName);
  log_i("Device size %i name: %s", sizeof(configuration.name), configuration.name);

  String mqttServer = request->arg("mqttServer");
  mqttServer.toCharArray(configuration.mqttServer, sizeof(configuration.mqttServer));
  log_i("MQTT Server: %s", mqttServer);

  uint16_t mqttPort = atoi(request->arg("mqttPort").c_str());
  configuration.mqttPort = mqttPort;
  log_i("MQTT Port: %i", mqttPort);

  String mqttTopic = request->arg("mqttTopic");
  mqttTopic.toCharArray(configuration.mqttTopic, sizeof(configuration.mqttTopic));
  log_i("MQTT Topic: %s", mqttTopic);
  
  EEPROM_saveConfig();

    
  // FIXME: Redudent code
  // MQTT
  log_d("disconnect");
  client.disconnect();
  log_d("setServer");
  client.setServer(configuration.mqttServer, configuration.mqttPort);

  log_d("is client.connected");
  if (!client.connected()) {
    log_d("Attempting MQTT connection to '%s:%i' as '%s' ...", configuration.mqttServer, configuration.mqttPort, getDeviceId());
    log_d("client.connect");
    if (client.connect(getDeviceId())) {
      if (strlen(configuration.mqttTopic)) {
        #ifdef TEMP_SENSOR
          char topic[255];
          sprintf_P(topic, "%s/sensor/temperature", configuration.mqttTopic);
          client.publish(topic,String(device->temperature(), 2).c_str());
          sprintf_P(topic, "%s/sensor/humidity", configuration.mqttTopic);
          client.publish(topic,String(device->humidity(), 2).c_str());
        #endif
      } 

    } else {
      log_d("MQTT connect failed, rc=%i", client.state());
    }
  }
  
  request->redirect("/");
}

String CWifiManager::getTempSensorResponse() {
#ifdef TEMP_SENSOR
  float temp = device->temperature();
  bool tempInC = false;
  return String("<div>\
    Temperature: " + String(tempInC ? temp : (temp*1.8)+32, 1) + (tempInC ? "C" : "F") +"<br/>\
    Hunidity: " + String(device->humidity(), 1) + "% <br/>\
  </div>");
#else
  return "";
#endif
}

#ifdef LED_EXTERNAL_MATRIX
/*
  Post body should contain comma delimited items in a list like this: X Y RRGGBB,X Y RRGGBB
  Each item is a space delimited X,Y coordinates of the LED pixel in the matrix and the expected color in Hex
  Ex: 0 0 ffbbdd,1 0 c91232
*/
void CWifiManager::handleLEDMatrix() {
    /*
  digitalWrite(BOARD_LED_PIN, LOW);

  //matrix_pixel_t pixels[LED_EXTERNAL_MATRIX_WIDTH * LED_EXTERNAL_MATRIX_HEIGHT];

  String postBody = server->arg("plain");
  log_d("LED Matrinx: %s", postBody.c_str());

  //ioTManager->setLeds(pixels);

/*
  const char pixelDelimiter[1] = ",";
  const char *raw = postBody.c_str();
  char *pixelToken = strtok(raw, pixelDelimiter);

  while( pixelToken != NULL ) {
    log_d("P: '%s'", pixelToken);
    pixelToken = strtok(NULL, pixelDelimiter);
  }
*/
  
  //postBody.indexOf()
  
/*
  
  server->send(200, "text/html", "OK");
  digitalWrite(BOARD_LED_PIN, HIGH);
  */
}
#endif