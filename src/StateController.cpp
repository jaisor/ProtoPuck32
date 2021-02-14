#include <Arduino.h>

#include "StateController.h"
#include "Device.h"

#include "WifiManager.h"
#include "InternalLEDManager.h"

#ifdef LED_EXTERNAL_MATRIX
  #include "MatrixLEDManager.h"
#else
  #include "DemoLEDManager.h"
#endif

const unsigned char _key_bitmaps [6][8] PROGMEM = {
  { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }, // NONE
  { 0x0, 0x10, 0x38, 0x7c, 0x10, 0x10, 0x10, 0x0 }, // UP
  { 0x0, 0x10, 0x10, 0x10, 0x7c, 0x38, 0x10, 0x0 }, // DOWN
  { 0x0, 0x0, 0x10, 0x30, 0x7e, 0x30, 0x10, 0x0 }, // LEFT
  { 0x0, 0x0, 0x8, 0xc, 0x7e, 0xc, 0x8, 0x0 }, // RIGHT
  { 0x0, 0x0, 0x54, 0x38, 0x7c, 0x38, 0x54, 0x0 } // MIDDLE
};

CStateController::CStateController(CDevice *device)
:device(device) {

#ifdef WIFI
    managers.push_back(new CWifiManager());
#endif

#ifdef LED
    managers.push_back(new CInternalLEDManager(LED_STRIP_SIZE, configuration.ledBrightness));
    #ifdef LED_EXTERNAL_MATRIX
        managers.push_back(new CMatrixLEDManager(device->ledsExternal(), LED_EXTERNAL_MATRIX_WIDTH, LED_EXTERNAL_MATRIX_HEIGHT, LED_EXTERNAL_BRIGHTNESS)); 
    #else
        managers.push_back(new CDemoLEDManager(device->ledsExternal(), LED_EXTERNAL_STRIP_SIZE, LED_EXTERNAL_BRIGHTNESS)); 
    #endif
#endif

#ifdef KEYPAD
  using std::placeholders::_1;
  for(CBaseManager *manager : managers) {
    device->addKeyListener(std::bind(&CBaseManager::keyEvent, manager, _1));
  }
#endif

    tMillis = millis();
    log_i("Initialized");
}

void CStateController::loop() {

    // Presentation
    if (millis() - tMillis > 100) {
        
        tMillis = millis();

        device->display()->clearDisplay();
        Adafruit_GFX *display = device->display();
    
        for(CBaseManager *manager : managers) {
        #ifdef OLED
            manager->OLED_Status(display);
        #endif
        #ifdef LED
            manager->LED_Status(device->ledsInternal());
        #endif
        }

        //

        // DEBUG: show key resistence value and bitmap
        display->setTextSize(1);
        display->setCursor(70, 24);
        display->print("K:");
        display->print(String(analogRead(KEYPAD_PIN)));
        display->drawBitmap(116, 24, _key_bitmaps[device->keyStatus()], 8, 8, 1);

#ifdef OLED

        // FIXME
        // display temperature
        display->setCursor(0,16);
        display->print("Temperature: ");
        display->setTextSize(2);
        display->setCursor(0,24);

    #ifdef TEMP_SENSOR
        int temp = device->temperature();

        bool tempInC = false;
        display->print(String(tempInC ? temp : (temp*9/5)+32 ));
        display->print(" ");
        display->setTextSize(1);
        display->cp437(true);
        display->write(167);
        display->setTextSize(2);
        display->print(tempInC ? "C" : "F");

        // display humidity
        display->setTextSize(1);
        display->setCursor(0, 40);
        display->print("Humidity: ");
        display->setTextSize(2);
        display->setCursor(0, 48);
        display->print(String(device->humidity(), 1));
        display->print("%"); 
    #endif

    #endif
        
    #ifdef LED
        FastLED.show();
    #endif
    
        device->display()->display();
    }  

    for(CBaseManager *manager : managers) {
        manager->loop();
    }

}