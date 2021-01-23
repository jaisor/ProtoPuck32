#include <Arduino.h>
#include "InternalLEDManager.h"
#include "Configuration.h"

CInternalLEDManager::CInternalLEDManager(uint16_t size, uint8_t brightness)
: CLEDManager(size, brightness) {
    
}

uint16_t CInternalLEDManager::LED_Status(CRGB *leds) {

    uint8_t colorIndex = startIndex;
    
    for( int i = 0; i < size; i++) {
        leds[i] = ColorFromPalette( PartyColors_p, colorIndex, brightness, LINEARBLEND);
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