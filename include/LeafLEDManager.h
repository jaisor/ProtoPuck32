#ifndef _DEMO_LED_MANAGER_H
#define _DEMO_LED_MANAGER_H

#include <vector>
#include "LEDManager.h"

#ifdef LED
    #include <FastLED.h>
#endif

#include "leaf/modes/BaseMode.h"

class CLeafLEDManager: public CLEDManager {

private:
    unsigned long tMillis, tMillsChangePalette;
    uint8_t startIndex = 0;

    CRGB *leds;

    std::vector<CBaseMode*> modes;
    uint8_t currentMode;

public:
	CLeafLEDManager(CRGB *leds, uint16_t size, float brightness);

#ifdef LED
    virtual uint16_t LED_Status(CRGB *leds);
#endif
#ifdef KEYPAD
    virtual void keyEvent(key_status_t key);
#endif

    virtual void loop();
};

#endif