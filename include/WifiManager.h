#ifndef _WIFI_MANAGER_H
#define _WIFI_MANAGER_H

#include <WiFi.h>
#include <WebServer.h>
#include "BaseManager.h"

typedef enum {
    WF_CONNECTING = 0,
    WF_LISTENING = 1
} wifi_status;

class CWifiManager: public CBaseManager {

private:
    unsigned long tMillis;
    wifi_status status;
    char softAP_SSID[32];
    WebServer server;

    void connect();
    void listen();

    void handleRoot();
    void handleConnect();
    
public:
	CWifiManager();
    virtual uint16_t OLED_Status(Adafruit_GFX *oled);
    virtual void loop();

};

#endif