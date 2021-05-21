#pragma once

#include "BaseMode.h"

#ifdef LED_EXTERNAL_LEAFS

class CFlowerMode : public CBaseMode {

private:
    uint8_t startIndex = 0;
    const uint16_t nodeLength;
    const unsigned long delay;

    void drawNode(CRGB *leds, const uint16_t (*n)[2], const TProgmemRGBPalette16& palette);

public:
    CFlowerMode(const uint16_t numLeds, const uint16_t nodeLength);
	virtual void loop();
    virtual void draw(CRGB *leds);
};

#endif