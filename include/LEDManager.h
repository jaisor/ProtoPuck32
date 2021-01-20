#ifndef _LED_MANAGER_H
#define _LED_MANAGER_H

#include <FastLED.h>
#include "BaseManager.h"

class CLEDManager: public CBaseManager {

private:
    unsigned long tMillis;
    uint8_t startIndex = 0;

    void ChangePalettePeriodically();
    void FillLEDsFromPaletteColors(CRGB *leds, uint8_t colorIndex);

public:
	CLEDManager();

    virtual uint16_t LED_Status(CRGB *leds);
    virtual void loop();
};

#endif