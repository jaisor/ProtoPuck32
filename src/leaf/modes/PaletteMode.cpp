#include "PaletteMode.h"

CPaletteMode::CPaletteMode(const uint16_t numLeds, const TProgmemRGBPalette16& palette, const uint16_t increment)
: CBaseMode(numLeds), increment(increment), palette(palette), blendType(LINEARBLEND), delay(15) {
}

CPaletteMode::CPaletteMode(const uint16_t numLeds, const TProgmemRGBPalette16& palette, const uint16_t increment, const TBlendType blendType)
: CBaseMode(numLeds), increment(increment), palette(palette), blendType(blendType), delay(15) {
}

CPaletteMode::CPaletteMode(const uint16_t numLeds, const TProgmemRGBPalette16& palette, const uint16_t increment, const TBlendType blendType, const unsigned long delay)
: CBaseMode(numLeds), increment(increment), palette(palette), blendType(blendType), delay(delay) {
}

void CPaletteMode::loop() {
    if (millis() - tMillis > delay) {
        tMillis = millis();
        startIndex = startIndex + 1;
    }
}

void CPaletteMode::draw(CRGB *leds) {
    uint8_t ci = startIndex;
    for( int i = 0; i < numLeds; i++) {
        leds[i] = ColorFromPalette( palette, ci, 255 * configuration.ledBrightness, blendType);
        ci+=increment;
    }
}