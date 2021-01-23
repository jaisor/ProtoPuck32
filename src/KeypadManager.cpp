#include <Arduino.h>
#include "KeypadManager.h"
#include "Configuration.h"

CKeypadManager::CKeypadManager() {
}

uint16_t CKeypadManager::OLED_Status(Adafruit_GFX *oled) {

    oled->setTextSize(1);
    oled->setCursor(80, 24);
    oled->print("K: ");
    oled->print(String(analogRead(KEYPAD_PIN)));

    return 100;
}

void CKeypadManager::loop() {

}