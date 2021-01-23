#ifndef _DEMO_LED_MANAGER_H
#define _DEMO_LED_MANAGER_H

#include <FastLED.h>
#include "LEDManager.h"

class CDemoLEDManager: public CLEDManager {

private:
    unsigned long tMillis;
    uint8_t startIndex = 0;

    void ChangePalettePeriodically();
    void FillLEDsFromPaletteColors(CRGB *leds, uint8_t colorIndex);

public:
	CDemoLEDManager(uint16_t size);

    virtual uint16_t LED_Status(CRGB *leds);
    virtual void loop();
};

#endif