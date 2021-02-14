#ifndef _MATRIX_LED_MANAGER_H
#define _MATRIX_LED_MANAGER_H

#include "LEDManager.h"

#ifdef LED_EXTERNAL_MATRIX
#include "matrix/BaseMatrixMode.h"
#include <FastLED.h>

class CMatrixLEDManager: public CLEDManager {

private:
    unsigned long tMillis, tMillsChangeMode;
    uint8_t startIndex = 0;

    CRGB *leds;

    const uint8_t width, height;

    std::vector<CBaseMatrixMode*> modes;
    int8_t currentModeIndex;
    int8_t changeMode;

public:
	CMatrixLEDManager(CRGB *leds, const uint8_t width, const uint8_t height, float brightness);

#ifdef OLED
    virtual uint16_t OLED_Status(Adafruit_GFX *oled);
#endif

#ifdef KEYPAD
    virtual void keyEvent(key_status_t key);
#endif

    virtual void loop();
};

#endif
#endif