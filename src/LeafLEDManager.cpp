#include <Arduino.h>
#include "LeafLEDManager.h"

#include "leaf/modes/HoneyOrangeMode.h"
#include "leaf/modes/PaletteMode.h"

CLeafLEDManager::CLeafLEDManager(CRGB *leds, uint16_t size, float brightness)
: CLEDManager(size, brightness) {

    this->leds = leds;

    tMillis = tMillsChangePalette = millis();
    currentMode = 0;

    modes.push_back(new CPaletteMode(LED_EXTERNAL_STRIP_SIZE, PartyColors_p, 1));
    modes.push_back(new CHoneyOrangeMode(LED_EXTERNAL_STRIP_SIZE));
    modes.push_back(new CPaletteMode(LED_EXTERNAL_STRIP_SIZE, RainbowStripeColors_p, 1, NOBLEND, 25));
    modes.push_back(new CPaletteMode(LED_EXTERNAL_STRIP_SIZE, HeatColors_p, 255 / LED_EXTERNAL_STRIP_SIZE));
    modes.push_back(new CPaletteMode(LED_EXTERNAL_STRIP_SIZE, RainbowColors_p, 255 * 3 / LED_EXTERNAL_STRIP_SIZE ));
    modes.push_back(new CPaletteMode(LED_EXTERNAL_STRIP_SIZE, CloudColors_p, 1, NOBLEND, 50));
    modes.push_back(new CPaletteMode(LED_EXTERNAL_STRIP_SIZE, ForestColors_p, 255 / LED_EXTERNAL_STRIP_SIZE));
    modes.push_back(new CPaletteMode(LED_EXTERNAL_STRIP_SIZE, OceanColors_p, 255 * 2 / LED_EXTERNAL_STRIP_SIZE, NOBLEND, 100));
    modes.push_back(new CPaletteMode(LED_EXTERNAL_STRIP_SIZE, HeatColors_p, 255 / LED_EXTERNAL_STRIP_SIZE));
}

void CLeafLEDManager::loop() {    
    modes[currentMode]->loop();
}

uint16_t CLeafLEDManager::LED_Status(CRGB *leds) {
    modes[currentMode]->draw(this->leds);
    return 10;
}

void CLeafLEDManager::keyEvent(key_status_t key) {

    switch (key) {
        case KEY_LEFT: {
            currentMode++; 
            if (currentMode > modes.size()-1) {
                currentMode = 0;
            }; 
        } break;
        case KEY_RIGHT: {
            currentMode--; 
            if (currentMode < 0) {
                currentMode = modes.size()-1;
            }; 
        } break;
        default: ;
    }

}