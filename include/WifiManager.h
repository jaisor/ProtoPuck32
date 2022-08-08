#ifndef _WIFI_MANAGER_H
#define _WIFI_MANAGER_H

#include <WiFi.h>

#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#elif ESP8266
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

#include <PubSubClient.h>
#include "BaseManager.h"
//#include "matrix/MatrixModeIoT.h"

typedef enum {
    WF_CONNECTING = 0,
    WF_LISTENING = 1
} wifi_status;

class CWifiManager: public CBaseManager {

private:
    unsigned long tMillis;
    wifi_status status;
    char softAP_SSID[32];
    char SSID[32];
    bool apMode;
    bool rebootNeeded;

    AsyncWebServer* server;
    PubSubClient client;
    //CMatrixModeIoT *ioTManager;
    CDevice * const device;

    void connect();
    void listen();

    void handleRoot(AsyncWebServerRequest *request);
    void handleConnect(AsyncWebServerRequest *request);
    void handleConfig(AsyncWebServerRequest *request);

    String getTempSensorResponse();
    
#ifdef LED_EXTERNAL_MATRIX
    void handleLEDMatrix();
#endif
    
public:
	CWifiManager(CDevice * const device);
#ifdef OLED
    virtual uint16_t OLED_Status(Adafruit_GFX *oled);
#endif
    virtual void loop();

    //void setIoTManager(CMatrixModeIoT *ioTManager);

};

#endif