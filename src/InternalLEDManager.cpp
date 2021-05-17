#include <Arduino.h>
#include "InternalLEDManager.h"
#include "Configuration.h"

CInternalLEDManager::CInternalLEDManager(uint16_t size, float brightness)
: CLEDManager(size, brightness) {
    brightnessChange = 0;
    changePalette = 0;

    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;

    palettes.push_back(PartyColors_p);
    
    palettes.push_back(
        CRGBPalette16(
            green,  green,  black,  black,
            purple, purple, black,  black,
            green,  green,  black,  black,
            purple, purple, black,  black )
    );

    palettes.push_back(RainbowColors_p);
    palettes.push_back(RainbowStripeColors_p);
    palettes.push_back(RainbowStripeColors_p);
    palettes.push_back(CloudColors_p);
    palettes.push_back(LavaColors_p);
    palettes.push_back(OceanColors_p);
    palettes.push_back(ForestColors_p);
    palettes.push_back(HeatColors_p);
}

#ifdef LED
uint16_t CInternalLEDManager::LED_Status(CRGB *leds) {

    uint8_t colorIndex = startIndex;
    uint8_t b = brightness * brightness * 255;
    
    for( int i = 0; i < size; i++) {
        leds[i] = ColorFromPalette( palettes[currentPaletteIndex], colorIndex, b, LINEARBLEND);
        colorIndex += 1;
    }

    return 10;
}
#endif

#ifdef OLED
uint16_t CInternalLEDManager::OLED_Status(Adafruit_GFX *oled) {
    
    if (brightnessChange) {
        oled->drawRect(78, 46, 44, 11, 1);
        uint8_t w = brightness * 40;
        oled->fillRect(80, 48, w, 7, 1);

        oled->setTextSize(1);
        oled->setTextColor(2);
        oled->setCursor(80,48);
        oled->print(String((uint8_t)(brightness * 100)));
        oled->print("%");
        oled->setTextColor(1);

        oled->setCursor(70, 37);
        oled->print("Brightness:");
    }
    
    return 100;
}
#endif

void CInternalLEDManager::loop() {
    
    if (millis() - tMillis > 10) {
        tMillis = millis();
        startIndex = startIndex + 1;

        brightness += brightnessChange;
        if (brightness <= 0) {
            brightness = 0;
        } else if (brightness >= 1) {
            brightness = 1;
        }

        if (brightnessChange) {
            brightnessChange = 0;
            configuration.ledBrightness = brightness;
            EEPROM_saveConfig(); // Should not save so frequently to preserve the flash
        }

    }

    if (millis() - tMillsChangePalette > 500) {
        tMillsChangePalette = millis();
        currentPaletteIndex += changePalette;
        if (currentPaletteIndex >= palettes.size()) {
            currentPaletteIndex = 0;
        }
    } 
    changePalette = 0;
}

#ifdef KEYPAD
void CInternalLEDManager::keyEvent(key_status_t key) {

    switch (key) {
        case KEY_UP: brightnessChange = 0.005; break;
        case KEY_DOWN: brightnessChange = -0.005; break;
        case KEY_LEFT: changePalette = 1; break;
        case KEY_RIGHT: changePalette = -1; break;
        default: brightnessChange = 0;
    }

}
#endif