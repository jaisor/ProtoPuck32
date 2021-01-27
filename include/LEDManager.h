#ifndef _LED_MANAGER_H
#define _LED_MANAGER_H

#include "BaseManager.h"
#ifdef LED
    #include <FastLED.h>
#endif

class CLEDManager: public CBaseManager {

protected:
    uint16_t size;
    float_t brightness;
    float_t brightnessChange;

public:
	CLEDManager(uint16_t size, uint8_t brightness);

#ifdef LED
    virtual uint16_t LED_Status(CRGB *leds) { return 0; };
#endif

    virtual void loop() {};
};

#endif