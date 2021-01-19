#ifndef _LED_MANAGER_H
#define _LED_MANAGER_H

#include <FastLED.h>
#include "BaseManager.h"

class CLEDManager: public CBaseManager {

private:
    unsigned long tMillis;

public:
	CLEDManager();

    void ChangePalettePeriodically();
    void FillLEDsFromPaletteColors(CRGB *leds, uint8_t colorIndex);

};

#endif