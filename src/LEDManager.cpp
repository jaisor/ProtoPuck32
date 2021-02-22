#include <Arduino.h>
#include "LEDManager.h"
#include "Configuration.h"

CLEDManager::CLEDManager(const uint16_t size, float brigthness)
: size(size) {
    this->brightness = brigthness;
}
