#ifndef _INTERNAL_LED_MANAGER_H
#define _INTERNAL_LED_MANAGER_H

#include "LEDManager.h"

class CInternalLEDManager: public CLEDManager {

private:
    uint8_t startIndex = 0;
    unsigned long tMillis;

public:
	CInternalLEDManager(uint16_t size, uint8_t brightness);

    virtual uint16_t OLED_Status(Adafruit_GFX *oled);
    virtual uint16_t LED_Status(CRGB *leds);
    virtual void loop();
    virtual void keyEvent(key_status_t key);
};

#endif