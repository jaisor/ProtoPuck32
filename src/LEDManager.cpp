#include <Arduino.h>
#include "LEDManager.h"
#include "Configuration.h"

CLEDManager::CLEDManager(const uint16_t size, float brightness)
: size(size) {
    this->brightness = brightness;
}
