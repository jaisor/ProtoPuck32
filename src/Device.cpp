#include <Arduino.h>
#include "Device.h"

#include <Wire.h>
#include <EEPROM.h>
#include <FastLED.h>

key_status_t operator ++( key_status_t &id, int ) {
  key_status_t currentID = id;

  if ( NUM_KEYS < id + 1 ) id = KEY_NONE;
  else id = static_cast<key_status_t>( id + 1 );

  return ( currentID );
}

CDevice::CDevice() {
    
#ifdef OLED
  _display = new Adafruit_SSD1306(OLED_SCREEN_WIDTH, OLED_SCREEN_HEIGHT, &Wire, -1);
  if(!_display->begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ID)) {
      log_e("SSD1306 OLED initialiation failed with ID %x", OLED_I2C_ID);
      while (1);
  }
  _display->clearDisplay();
  _display->setTextColor(WHITE);
#endif

  delay(100);

#ifdef TEMP_SENSOR
  _bme = new Adafruit_BME280();
  if (!_bme->begin(BME_I2C_ID)) {
      log_e("BME280 sensor initialiation failed with ID %x", BME_I2C_ID);
      while (1);
  }
#endif
  
  delay(100);

#ifdef LED
  FastLED.addLeds<LED_TYPE, LED_PIN, LED_COLOR_ORDER>(_ledsInternal, LED_STRIP_SIZE).setCorrection( TypicalLEDStrip );
  #ifdef LED_EXTERNAL
    FastLED.addLeds<LED_EXTERNAL_TYPE, LED_EXTERNAL_PIN, LED_EXTERNAL_COLOR_ORDER>(_ledsExternal, LED_EXTERNAL_STRIP_SIZE).setCorrection( TypicalLEDStrip );
  #endif
#endif

#ifdef KEYPAD
  _keyStatus = KEY_NONE;
  listener = nullptr;
  tMillisKey = millis();
  memset(keyEventCounter, 0, sizeof(keyEventCounter));
#endif

  log_i("Initialized");
}

CDevice::~CDevice() { 
#ifdef OLED
  delete _display;
#endif

#ifdef TEMP_SENSOR
  delete _bme;
#endif
  log_i("Destroyed");
}

void CDevice::loop() {

  // Scan keypad
  uint16_t k = analogRead(KEYPAD_PIN);

  #ifdef ALEX_GIFT
    if (k > 3900) {
        _keyStatus = KEY_NONE;
    } else if (k > 2300 && k < 2500) {
        _keyStatus = KEY_RIGHT;
    } else if (k < 300) {
        _keyStatus = KEY_LEFT;
    } else if (k > 2600 && k < 3000) {
        _keyStatus = KEY_UP;
    } else if (k > 1200 && k < 1700) {
        _keyStatus = KEY_DOWN;
    } else if (k > 3200 && k < 3400) {
        _keyStatus = KEY_MIDDLE;
    }
  #else
    if (k > 4000) {
        _keyStatus = KEY_NONE;
    } else if (k > 2000 && k < 2500) {
        _keyStatus = KEY_LEFT;
    } else if (k < 300) {
        _keyStatus = KEY_RIGHT;
    } else if (k > 2600 && k < 3000) {
        _keyStatus = KEY_DOWN;
    } else if (k > 1000 && k < 1600) {
        _keyStatus = KEY_UP;
    } else if (k > 3200 && k < 3400) {
        _keyStatus = KEY_MIDDLE;
    }
  #endif 

  keyEventCounter[_keyStatus]++;

  // Filter the noise and focus only on the most prominent event during the time period 
  if (millis() - tMillisKey > 50) {
      tMillisKey = millis();
      uint16_t max = 0;
      key_status_t maxKey = KEY_NONE;
      for(key_status_t i=KEY_NONE; i<NUM_KEYS; i++) {
          if (keyEventCounter[i] > max) {
              max = keyEventCounter[i];
              maxKey = i;
          }
      }
      memset(keyEventCounter, 0, sizeof(keyEventCounter));
      
      if (maxKey != KEY_NONE || filteredKeyStatus != maxKey) {
          log_v("Notifying listeners about pressed key %i", maxKey);
          listener_t *cur = this->listener;
          while(cur) {
              cur->listener(maxKey);
              cur = cur->next;
          }
      }

      filteredKeyStatus = maxKey;
  }

  if (millis() - tMillisMin > 60000) {
    tMillisMin = millis();
    #ifdef LED
      CONFIG_updateLedBrightnessTime();
    #endif
  }

}


void CDevice::addKeyListener(TKeyListener listener) {

    listener_t *l = new listener_t { listener, nullptr }; 

    if (!this->listener) {
        
        this->listener = l;
        log_d("Added first keypad listener");
    } else {
        uint8_t i = 0;
        listener_t *cur = this->listener;
        while(cur) {
            if (!cur->next) {
                log_d("Added keypad listener number %i", i);
                cur->next = l;
                return;
            }
            cur = cur->next;
            i++;
        }
    }
   
}