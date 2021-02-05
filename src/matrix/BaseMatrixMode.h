#ifndef _BASE_MATRIX_MODE_H
#define _BASE_MATRIX_MODE_H

#include "LEDManager.h"

#ifdef LED_EXTERNAL_MATRIX
#include <FastLED.h>

class CBaseMatrixMode {

protected:
    unsigned long tMillis;
    uint8_t width;
    uint8_t height;

    uint16_t XY( uint8_t x, uint8_t y);
    uint16_t XYsafe( uint8_t x, uint8_t y);

public:
	CBaseMatrixMode(uint8_t width, uint8_t height);
    virtual void draw(CRGB *leds) {};
    #ifdef KEYPAD
        virtual void keyEvent(key_status_t key) {};
    #endif
};

#endif
#endif