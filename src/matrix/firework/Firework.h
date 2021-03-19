#ifndef _FIREWORK_H
#define _FIREWORK_H

#include <Arduino.h>
#include "Configuration.h"
#ifdef LED
    #include <FastLED.h>
#endif

class CFirework {
    
public:
	CFirework();

    float_t x, y, yxpl, yvel, xplvel;

    CRGB color;
    uint8_t state;
};

#endif