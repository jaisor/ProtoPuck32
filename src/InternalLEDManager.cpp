#include <Arduino.h>
#include "InternalLEDManager.h"
#include "Configuration.h"

CInternalLEDManager::CInternalLEDManager(uint16_t size)
: CLEDManager(size) {
    
}

uint16_t CInternalLEDManager::LED_Status(CRGB *leds) {

    uint8_t brightness = 255;
    uint8_t colorIndex = startIndex;
    
    for( int i = 0; i < size; i++) {
        leds[i] = ColorFromPalette( OceanColors_p, colorIndex, brightness, LINEARBLEND);
        colorIndex += 3;
    }

    return 10;
}

void CInternalLEDManager::loop() {
    
    if (millis() - tMillis > 10) {
        tMillis = millis();
        startIndex = startIndex + 1;
    }
}