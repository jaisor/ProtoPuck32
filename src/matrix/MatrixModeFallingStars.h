#ifndef _MATRIX_MODE_FALLING_STARS_H
#define _MATRIX_MODE_FALLING_STARS_H

#include "BaseMatrixMode.h"

#ifdef LED_EXTERNAL_MATRIX
#include <FastLED.h>

#define NUM_STARS 10

class CStar {

    float x, y, v;
    CRGB color; 

    friend class CMatrixModeFallingStars;
};

class CMatrixModeFallingStars : public CBaseMatrixMode {

private:
    void randomize();

    CStar stars[NUM_STARS+1];
    
public:
	CMatrixModeFallingStars(const uint8_t width, const uint8_t height);
    virtual void draw(CRGB *leds);
    #ifdef KEYPAD
        virtual void keyEvent(key_status_t key);
    #endif
};

#endif
#endif