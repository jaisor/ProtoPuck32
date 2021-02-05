#ifndef _MATRIX_MODE_MOVING_SQUARE_H
#define _MATRIX_MODE_MOVING_SQUARE_H

#include "BaseMatrixMode.h"

#ifdef LED_EXTERNAL_MATRIX
#include <FastLED.h>

class CMatrixModeMovingSquare : public CBaseMatrixMode {

private:

public:
	CMatrixModeMovingSquare(uint8_t width, uint8_t height);
    virtual void draw(CRGB *leds);
    #ifdef KEYPAD
        virtual void keyEvent(key_status_t key);
    #endif
};

#endif
#endif