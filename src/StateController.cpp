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
  device->addKeyListener(std::bind(&CBaseManager::keyEvent, this, _1));
#endif

    tMillis = millis();
    tMillisConfig = millis();

    state = S_HOME_SCREEN;

    log_i("Initialized");
}

void CStateController::loop() {

    // Presentation
    if (millis() - tMillis > 100) {
        
        tMillis = millis();

        device->display()->clearDisplay();
        Adafruit_GFX *display = device->display();

        //
        switch (state) {
            case S_HOME_SCREEN: {

            for(CBaseManager *manager : managers) {
            #ifdef OLED
                manager->OLED_Status(display);
            #endif
            #ifdef LED
                manager->LED_Status(device->ledsInternal());
            #endif
            }

            #ifdef OLED

                // DEBUG: show key resistence value and bitmap
                display->setTextSize(1);
                display->setCursor(70, 24);
                display->print("K:");
                display->print(String(analogRead(KEYPAD_PIN)));
                display->drawBitmap(116, 24, _key_bitmaps[device->keyStatus()], 8, 8, 1);

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
            #endif // TEMP_SENSOR

            #endif // OLED

            } break;

            case S_INVOKING_CONFIG: {
            #ifdef OLED
                uint16_t dt = millis() - tMillisConfig;
                if (dt > 2500) {
                    uint16_t dta = (dt-2500)*32/2500;
                    display->fillCircle(128/2, 64/2, dta, WHITE);
                }
            #endif
            } break;

            case S_CONFIG_MAIN: {
            #ifdef OLED
                display->drawRect(0, 0, 128, 64, WHITE);
                display->fillRect(0, 0, 128, 11, WHITE);
                display->setTextColor(INVERSE);
                display->setTextSize(1);
                display->setCursor(2,2);
                display->print("Configuration");
                display->fillRect(2, 21, 124, 11, WHITE);
                for (int i=0; i<5; i++) {
                    display->setCursor(3,13 + i * 9);
                    display->printf("item %i", i);
                }
                display->setTextColor(WHITE);
            #endif
            } break;

            default: ;
        }

        
        
    #ifdef LED
        FastLED.show();
    #endif
    
        device->display()->display();
    }  

    for(CBaseManager *manager : managers) {
        manager->loop();
    }

}

#ifdef KEYPAD
void CStateController::keyEvent(key_status_t key) {

    switch (key) {
        case KEY_MIDDLE: {
            if (state == S_HOME_SCREEN) {
                state = S_INVOKING_CONFIG;
                tMillisConfig = millis();
            } else if (state == S_INVOKING_CONFIG && millis() - tMillisConfig > 5000) {
                state = S_CONFIG_MAIN;
            }
        } break;
        default: {
            if (state == S_INVOKING_CONFIG) {
                state = S_HOME_SCREEN; // Keep last state or stack
            }
        };
    }

}
#endif
