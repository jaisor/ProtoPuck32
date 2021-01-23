#ifndef _LED_MANAGER_H
#define _LED_MANAGER_H

#include <FastLED.h>
#include "BaseManager.h"

class CLEDManager: public CBaseManager {

protected:
    uint16_t size;
    uint8_t brightness;

public:
	CLEDManager(uint16_t size, uint8_t brightness);

    virtual uint16_t LED_Status(CRGB *leds) { return 0; };
    virtual void loop() {};
};

#endif