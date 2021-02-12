#ifndef _WIFI_MANAGER_H
#define _WIFI_MANAGER_H

#include <WiFi.h>
#include <WebServer.h>
#include <PubSubClient.h>
#include "BaseManager.h"
#include "matrix/MatrixModeIoT.h"

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

    WebServer server;
    PubSubClient client;
    CMatrixModeIoT *ioTManager;

    void connect();
    void listen();

    void handleRoot();
    void handleConnect();

    String getTempSensorResponse();
    
#ifdef LED_EXTERNAL_MATRIX
    void handleLEDMatrix();
#endif
    
public:
	CWifiManager();
#ifdef OLED
    virtual uint16_t OLED_Status(Adafruit_GFX *oled);
#endif
    virtual void loop();

    void setIoTManager(CMatrixModeIoT *ioTManager);

};

#endif