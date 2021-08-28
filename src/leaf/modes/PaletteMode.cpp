#include "PaletteMode.h"

CPaletteMode::CPaletteMode(const uint16_t numLeds, const TProgmemRGBPalette16& palette, const float increment)
: CBaseMode(numLeds), increment(increment), palette(palette), blendType(LINEARBLEND), delay(15) {
}

CPaletteMode::CPaletteMode(const uint16_t numLeds, const TProgmemRGBPalette16& palette, const float increment, const TBlendType blendType)
: CBaseMode(numLeds), increment(increment), palette(palette), blendType(blendType), delay(15) {
}

CPaletteMode::CPaletteMode(const uint16_t numLeds, const TProgmemRGBPalette16& palette, const float increment, const TBlendType blendType, const unsigned long delay)
: CBaseMode(numLeds), increment(increment), palette(palette), blendType(blendType), delay(delay) {
}

void CPaletteMode::loop() {
    if (millis() - tMillis > delay) {
        tMillis = millis();
        startIndex = startIndex + 1;
    }
}

void CPaletteMode::draw(CRGB *leds) {
    float ci = startIndex;
    for( int i = 0; i < numLeds; i++) {
        leds[i] = ColorFromPalette( palette, (uint8_t)ci, 255 * configuration.ledBrightness, blendType);
        ci+=increment;
    }
}