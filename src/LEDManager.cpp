#include <Arduino.h>
#include "LEDManager.h"
#include "Configuration.h"

CLEDManager::CLEDManager(uint16_t size, uint8_t brigthness) {
    this->size = size;
    this->brightness = brigthness;
}
