#ifndef _MATRIX_MODE_PIXEL_H
#define _MATRIX_MODE_PIXEL_H

#include "BaseMatrixMode.h"

#ifdef LED_EXTERNAL_MATRIX
#include <FastLED.h>

class CMatrixModePixel : public CBaseMatrixMode {

private:

public:
	CMatrixModePixel(const uint8_t width, const uint8_t height);
    virtual void draw(CRGB *leds);
    #ifdef KEYPAD
        virtual void keyEvent(key_status_t key);
    #endif
};

#endif
#endif