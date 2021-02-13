#include <Arduino.h>

#include <functional>

#if !( defined(ESP32) )
  #error This code is intended to run on ESP32 platform! Please check your Tools->Board setting.
#endif

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <EEPROM.h>
#include <FastLED.h>

#include "Configuration.h"
#include "Device.h"

#include "WifiManager.h"
#include "InternalLEDManager.h"

#ifdef LED_EXTERNAL_MATRIX
  #include "MatrixLEDManager.h"
#else
  #include "DemoLEDManager.h"
#endif

CDevice *device;
CBaseManager *managers[3];

unsigned long tMillis;

float p = 0;

void setup() {
  Serial.begin(115200); while (!Serial); delay(200);

  EEPROM_loadConfig();

  //strcpy(configuration.wifiSsid, "<REDACTED>");
  //strcpy(configuration.wifiPassword, "<REDACTED>");
  //EEPROM_saveConfig();

  device = new CDevice();

  uint8_t mgrIndex = 0;

#ifdef WIFI
  managers[mgrIndex++] = new CWifiManager();
#endif

#ifdef LED
  managers[mgrIndex++] = new CInternalLEDManager(LED_STRIP_SIZE, configuration.ledBrightness);
  #ifdef LED_EXTERNAL_MATRIX
    managers[mgrIndex++] = new CMatrixLEDManager(device->ledsExternal(), LED_EXTERNAL_MATRIX_WIDTH, LED_EXTERNAL_MATRIX_HEIGHT, LED_EXTERNAL_BRIGHTNESS); 
  #else
    managers[mgrIndex++] = new CDemoLEDManager(ledsExternal, LED_EXTERNAL_STRIP_SIZE, LED_EXTERNAL_BRIGHTNESS); 
  #endif
#endif

#ifdef KEYPAD
  using std::placeholders::_1;
  for(auto & manager : managers) {
    if (manager) {
      device->addKeyListener(std::bind(&CBaseManager::keyEvent, manager, _1));
    }
  }
#endif
  
  tMillis = millis();
}

void loop() {
  
#ifdef OLED
#endif

  // Presentation
  if (millis() - tMillis > 100) {
    device->display()->clearDisplay();
    device->loop();
  
    tMillis = millis();
    
    for(auto & manager : managers) {
      #ifdef OLED
        manager->OLED_Status(device->display());
      #endif
      #ifdef LED
        manager->LED_Status(device->ledsInternal());
      #endif
    }

    //
    
#ifdef LED
  FastLED.show();
#endif
  
  device->display()->display();

  }  


  // Post presentation
  for(auto & manager : managers) {
    manager->loop();
  }
}