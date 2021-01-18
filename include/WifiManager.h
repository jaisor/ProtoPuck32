#ifndef _WIFI_MANAGER_H
#define _WIFI_MANAGER_H

#include <WiFi.h>
#include "BaseManager.h"

class CWifiManager: public CBaseManager {

private:
    unsigned long tMillis;
    wl_status_t lastStatus;
    char softAP_SSID[32];

    void createAccessPoint();
    
public:
	CWifiManager();
    virtual uint16_t OLED_Status(Adafruit_GFX *oled);

};

#endif