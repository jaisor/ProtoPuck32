#ifndef _CONFIG_MANAGER_H
#define _CONFIG_MANAGER_H

#include <vector>
#include "BaseManager.h"
#include "BaseConfigItem.h"
#include "Configuration.h"

class CConfigManager: public CBaseManager {

private:
    unsigned long tMillis;
    uint8_t itemIndex;

    std::vector<CBaseConfigItem*> configItems;
    
public:
	CConfigManager();
#ifdef LED
    virtual uint16_t LED_Status(CRGB *leds);
#endif
#ifdef OLED
    virtual uint16_t OLED_Status(Adafruit_GFX *oled);
#endif
#ifdef KEYPAD
    virtual void keyEvent(key_status_t key);
#endif
    virtual void loop();
};

#endif