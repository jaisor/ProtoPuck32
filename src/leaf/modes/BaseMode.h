#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "Configuration.h"

class CBaseMode {

protected:
    unsigned long tMillis;
    const uint16_t numLeds;

public:
	CBaseMode(const uint16_t numLeds);
    virtual void loop() {};
    virtual void draw(CRGB *leds) {};
};
