#pragma once

#include "BaseMode.h"

class CPaletteMode : public CBaseMode {

private:
    uint8_t startIndex = 0;
    const uint8_t increment;
    const TProgmemRGBPalette16& palette;
    const TBlendType blendType;
    const unsigned long delay;

public:
	CPaletteMode(const uint16_t numLeds, const TProgmemRGBPalette16& palette, const uint16_t increment);
    CPaletteMode(const uint16_t numLeds, const TProgmemRGBPalette16& palette, const uint16_t increment, const TBlendType blendType);
    CPaletteMode(const uint16_t numLeds, const TProgmemRGBPalette16& palette, const uint16_t increment, const TBlendType blendType, const unsigned long delay);
    virtual void loop();
    virtual void draw(CRGB *leds);
};
