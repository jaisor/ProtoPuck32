#ifndef _LED_MANAGER_H
#define _LED_MANAGER_H

#include <FastLED.h>
#include "BaseManager.h"

class CLEDManager: public CBaseManager {

protected:
    uint16_t size;

public:
	CLEDManager(uint16_t size);

    virtual uint16_t LED_Status(CRGB *leds) { return 0; };
    virtual void loop() {};
};

#endif