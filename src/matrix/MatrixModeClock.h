#ifndef _MATRIX_MODE_CLOCK_H
#define _MATRIX_MODE_CLOCK_H

#include "BaseMatrixMode.h"

#ifdef LED_EXTERNAL_MATRIX
#include <FastLED.h>
#include <Adafruit_GFX.h>

class CMatrixModeClock : public CBaseMatrixMode {

private:
    GFXcanvas16 *canvas;
    uint8_t h, m;

    const uint16_t colMin, colHour, colText;

    static CRGB RGB565_to_CRGB(uint16_t a); // 5-6-5 16bit pixel conversion to 24bit CRGB
    static uint16_t CRGB_to_RGB565(CRGB c); // CRGB to 5-6-5 16bit pixel

public:
	CMatrixModeClock(const uint8_t width, const uint8_t height);
    ~CMatrixModeClock();
    virtual void draw(CRGB *leds);
    #ifdef KEYPAD
        virtual void keyEvent(key_status_t key);
    #endif
};

#endif
#endif