#ifndef _DEMO_LED_MANAGER_H
#define _DEMO_LED_MANAGER_H

#include <FastLED.h>
#include "LEDManager.h"

class CDemoLEDManager: public CLEDManager {

private:
    unsigned long tMillis, tMillsChangePalette;
    uint8_t startIndex = 0;

    CRGB *leds;

    std::vector<CRGBPalette16> palettes;
    uint8_t currentPaletteIndex;
    uint8_t changePalette;

    void ChangePalettePeriodically();
    void FillLEDsFromPaletteColors(CRGB *leds, uint8_t colorIndex);

public:
	CDemoLEDManager(CRGB *leds, uint16_t size, uint8_t brightness);

    virtual uint16_t LED_Status(CRGB *leds);
    virtual void loop();

    virtual void keyEvent(key_status_t key);
};

#endif