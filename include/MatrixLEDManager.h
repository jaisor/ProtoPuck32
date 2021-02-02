#ifndef _DEMO_LED_MANAGER_H
#define _DEMO_LED_MANAGER_H

#include "LEDManager.h"

#ifdef LED_EXTERNAL
#include <FastLED.h>

struct matrix_pixel_t {
    uint8_t x,y;
    uint32_t color;
};

class CMatrixLEDManager: public CLEDManager {

private:
    unsigned long tMillis, tMillsChangePalette;
    uint8_t startIndex = 0;

LED_EXTERNAL
    CRGB *leds;

    uint8_t width, height;

    std::vector<CRGBPalette16> palettes;
    uint8_t currentPaletteIndex;
    uint8_t changePalette;

    void FillLEDsFromPaletteColors(CRGB *leds, uint8_t colorIndex);
    uint16_t XY( uint8_t x, uint8_t y);
    uint16_t XYsafe( uint8_t x, uint8_t y);

public:
	CMatrixLEDManager(CRGB *leds, uint16_t size, uint8_t width, uint8_t height, float brightness);

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