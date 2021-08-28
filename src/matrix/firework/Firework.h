#ifndef _FIREWORK_H
#define _FIREWORK_H

#include <Arduino.h>
#include "Configuration.h"

#ifdef LED_EXTERNAL_MATRIX

#include "matrix/BaseMatrixMode.h"
#ifdef LED
    #include <FastLED.h>
#endif

class CFirework : public CBaseMatrixMode {

private:
    float_t x, y, yxpl, yvel, xplvel;

    CRGB color;
    uint8_t state;

    
    
public:
	CFirework(const uint8_t width, const uint8_t height);
    void reset();
    virtual void draw(CRGB *leds);
};

#endif

#endif