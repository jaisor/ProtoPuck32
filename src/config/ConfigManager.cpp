#include <Arduino.h>
#include "ConfigManager.h"

CConfigManager::CConfigManager() {
    tMillis = millis();

    configItems.push_back(new CBaseConfigItem("LED Brightness"));
    configItems.push_back(new CBaseConfigItem("Keypad calibration"));
    configItems.push_back(new CBaseConfigItem("LED Mode"));
    configItems.push_back(new CBaseConfigItem("WiFi"));
    configItems.push_back(new CBaseConfigItem("Exit"));

    itemIndex = 0;
}

uint16_t CConfigManager::LED_Status(CRGB *leds) {

  CRGB even, odd;

  if (((millis() - tMillis) / 500) % 2) {
    even = CRGB(50, 50, 50);
    odd = CRGB(0, 0, 0);
  } else {
    even = CRGB(0, 0, 0);
    odd = CRGB(50, 50, 50);
  }

  for (uint8_t i=0; i<LED_STRIP_SIZE; i++) {
    leds[i] = i % 2 == 0 ? even : odd;
  }
};

uint16_t CConfigManager::OLED_Status(Adafruit_GFX *oled) {
  oled->drawRect(0, 0, 128, 64, WHITE);
  oled->fillRect(0, 0, 128, 11, WHITE);
  oled->setTextColor(INVERSE);
  oled->setTextSize(1);
  oled->setCursor(2,2);
  oled->print("Configuration");

  oled->fillRect(2, 12 + itemIndex * 9, 124, 10, WHITE);

  for (int i=0; i<configItems.size(); i++) {
    oled->setCursor(3,13 + i * 9);
    oled->print(configItems[i]->name());
  }

  oled->setTextColor(WHITE);
}

void CConfigManager::loop() {

}

void CConfigManager::keyEvent(key_status_t key) {

    switch (key) {
        case KEY_UP: if (itemIndex>0) itemIndex--; break;
        case KEY_DOWN: if (itemIndex<configItems.size()-1) itemIndex++; break;
        case KEY_LEFT: ; break;
        case KEY_RIGHT: ; break;
        default: ;
    }
}