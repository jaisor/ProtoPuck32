#include <Arduino.h>
#include "InternalLEDManager.h"
#include "Configuration.h"

CInternalLEDManager::CInternalLEDManager(uint16_t size, uint8_t brightness)
: CLEDManager(size, brightness) {
    
}

uint16_t CInternalLEDManager::LED_Status(CRGB *leds) {

    uint8_t colorIndex = startIndex;
    
    for( int i = 0; i < size; i++) {
        leds[i] = ColorFromPalette( LavaColors_p, colorIndex, brightness, LINEARBLEND);
        colorIndex += 1;
    }

    return 10;
}

uint16_t CInternalLEDManager::OLED_Status(Adafruit_GFX *oled) {
    

    if (brightnessChange) {
        oled->drawRect(78, 46, 44, 11, 1);
        uint8_t w = brightness * 40 / 255;
        oled->fillRect(80, 48, w, 7, 1);

        oled->setTextSize(1);
        oled->setTextColor(2);
        oled->setCursor(80,48);
        oled->print(String((uint8_t)(brightness * 100 / 255)));
        oled->print("%");
        oled->setTextColor(1);
    }
    
    return 100;
}

void CInternalLEDManager::loop() {
    
    if (millis() - tMillis > 10) {
        tMillis = millis();
        startIndex = startIndex + 1;

        brightness += brightnessChange;
        if (brightness <= 0) {
            brightness = 0;
        } else if (brightness >= 255) {
            brightness = 255;
        }

        brightnessChange = 0;
    }
}

void CInternalLEDManager::keyEvent(key_status_t key) {

    switch (key) {
        case KEY_UP: brightnessChange = 0.5; break;
        case KEY_DOWN: brightnessChange = -0.5; break;
        default: brightnessChange = 0;;
    }

}