#include <Arduino.h>
#include "Device.h"

const unsigned char _key_bitmaps [6][8] PROGMEM = {
    { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }, // NONE
	{ 0x0, 0x10, 0x38, 0x7c, 0x10, 0x10, 0x10, 0x0 }, // UP
    { 0x0, 0x10, 0x10, 0x10, 0x7c, 0x38, 0x10, 0x0 }, // DOWN
    { 0x0, 0x0, 0x10, 0x30, 0x7e, 0x30, 0x10, 0x0 }, // LEFT
    { 0x0, 0x0, 0x8, 0xc, 0x7e, 0xc, 0x8, 0x0 }, // RIGHT
    { 0x0, 0x0, 0x54, 0x38, 0x7c, 0x38, 0x54, 0x0 } // MIDDLE
};

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
  keyStatus = KEY_NONE;
  listener = nullptr;
  tMillisKey = millis();
  memset(keyEventCounter, 0, sizeof(keyEventCounter));
#endif

}

CDevice::~CDevice() { 
#ifdef OLED
  delete _display;
#endif

#ifdef TEMP_SENSOR
    delete _bme;
#endif
}

void CDevice::loop() {

  // DEBUG: show key resistence value and bitmap
  display()->setTextSize(1);
  display()->setCursor(70, 24);
  display()->print("K:");
  display()->print(String(analogRead(KEYPAD_PIN)));

  display()->drawBitmap(116, 24, _key_bitmaps[keyStatus], 8, 8, 1);

  
#ifdef OLED

  // FIXME
  // display temperature
  display()->setCursor(0,16);
  display()->print("Temperature: ");
  display()->setTextSize(2);
  display()->setCursor(0,24);

#ifdef TEMP_SENSOR
    int temp = _bme->readTemperature();

    bool tempInC = false;
    display()->print(String(tempInC ? temp : (temp*9/5)+32 ));
    display()->print(" ");
    display()->setTextSize(1);
    display()->cp437(true);
    display()->write(167);
    display()->setTextSize(2);
    display()->print(tempInC ? "C" : "F");
    
    // display humidity
    display()->setTextSize(1);
    display()->setCursor(0, 40);
    display()->print("Humidity: ");
    display()->setTextSize(2);
    display()->setCursor(0, 48);
    display()->print(String(_bme->readHumidity(), 1));
    display()->print("%"); 
#endif

#endif

  // Scan keypad
  uint16_t k = analogRead(KEYPAD_PIN);

  #ifdef ALEX_GIFT
    if (k > 3900) {
        keyStatus = KEY_NONE;
    } else if (k > 2300 && k < 2500) {
        keyStatus = KEY_RIGHT;
    } else if (k < 300) {
        keyStatus = KEY_LEFT;
    } else if (k > 2600 && k < 3000) {
        keyStatus = KEY_UP;
    } else if (k > 1200 && k < 1700) {
        keyStatus = KEY_DOWN;
    } else if (k > 3200 && k < 3400) {
        keyStatus = KEY_MIDDLE;
    }
  #else
    if (k > 4000) {
        keyStatus = KEY_NONE;
    } else if (k > 2300 && k < 2500) {
        keyStatus = KEY_LEFT;
    } else if (k < 300) {
        keyStatus = KEY_RIGHT;
    } else if (k > 2600 && k < 3000) {
        keyStatus = KEY_DOWN;
    } else if (k > 1000 && k < 1600) {
        keyStatus = KEY_UP;
    } else if (k > 3200 && k < 3400) {
        keyStatus = KEY_MIDDLE;
    }
  #endif 

  keyEventCounter[keyStatus]++;

  // Filter the noise and focus only on the most prominent event during the time period 
  if (millis() - tMillisKey > 10) {
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
      
      if (maxKey != KEY_NONE) {
          listener_t *cur = this->listener;
          while(cur) {
              cur->listener(maxKey);
              cur = cur->next;
          }
      }
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