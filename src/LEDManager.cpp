#include <Arduino.h>
#include "LEDManager.h"
#include "Configuration.h"

CLEDManager::CLEDManager(uint16_t size, float brigthness) {
    this->size = size;
    this->brightness = brigthness;
}
