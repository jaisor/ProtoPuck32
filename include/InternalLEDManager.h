#ifndef _INTERNAL_LED_MANAGER_H
#define _INTERNAL_LED_MANAGER_H

#include "LEDManager.h"

class CInternalLEDManager: public CLEDManager {

private:
    uint8_t startIndex = 0;
    unsigned long tMillis;

public:
	CInternalLEDManager(uint16_t size);

    virtual uint16_t LED_Status(CRGB *leds);
    virtual void loop();
};

#endif