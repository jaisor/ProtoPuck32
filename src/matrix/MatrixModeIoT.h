#ifndef _MATRIX_MODE_IOT_H
#define _MATRIX_MODE_IOT_H

#include "BaseMatrixMode.h"

#ifdef LED_EXTERNAL_MATRIX
#include <FastLED.h>

struct matrix_pixel_t {
    uint8_t x,y;
    uint32_t color;
};

class CMatrixModeIoT : public CBaseMatrixMode {

private:
    CRGB *leds;

public:
	CMatrixModeIoT(uint8_t width, uint8_t height);
    virtual void draw(CRGB *leds);
    #ifdef KEYPAD
        virtual void keyEvent(key_status_t key);
    #endif
    void setLeds(matrix_pixel_t *pixels);
};

#endif
#endif