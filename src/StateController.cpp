#include <Arduino.h>

#include "StateController.h"
#include "Device.h"

#include "WifiManager.h"
#include "InternalLEDManager.h"
#include "config/ConfigManager.h"

#ifdef LED_EXTERNAL_MATRIX
  #include "MatrixLEDManager.h"
#else
  #include "LeafLEDManager.h"
#endif

#define CONFIG_INVOKE_TIME_MS 2000
#define CONFIG_INVOKE_DELAY_TIME_MS 500

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

    configManager = new CConfigManager();

#ifdef WIFI
    managers.push_back(new CWifiManager());
#endif

#ifdef LED
    managers.push_back(new CInternalLEDManager(LED_STRIP_SIZE, configuration.ledBrightnessTime));
    #ifdef LED_EXTERNAL_MATRIX
        managers.push_back(new CMatrixLEDManager(device->ledsExternal(), LED_EXTERNAL_MATRIX_WIDTH, LED_EXTERNAL_MATRIX_HEIGHT, LED_EXTERNAL_BRIGHTNESS)); 
    #else
        managers.push_back(new CLeafLEDManager(device->ledsExternal(), LED_EXTERNAL_STRIP_SIZE, LED_EXTERNAL_BRIGHTNESS)); 
    #endif
#endif

#ifdef KEYPAD
  using std::placeholders::_1;
  /*
  for(CBaseManager *manager : managers) {
    device->addKeyListener(std::bind(&CBaseManager::keyEvent, manager, _1));
  }
  */
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

        switch (state) {
            case S_HOME_SCREEN: drawStateHome(); break;
            case S_INVOKING_CONFIG: drawStateInvokeConfig(); break;
            case S_CONFIG: drawStateConfig(); break;
            default: ;
        }
        
    #ifdef LED
        FastLED.show();
    #endif
    
        device->display()->display();
    }  

    switch (state) {
        case S_CONFIG: configManager->loop(); break;
        default: {
            for(CBaseManager *manager : managers) {
                manager->loop();
            }
        };
    }

}

void CStateController::drawStateHome() {

    for(CBaseManager *manager : managers) {
    #ifdef OLED
        manager->OLED_Status(device->display());
    #endif
    #ifdef LED
        manager->LED_Status(device->ledsInternal());
    #endif
    }

#ifdef OLED
    Adafruit_GFX *display = device->display();
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
}

void CStateController::drawStateInvokeConfig() {
    uint16_t dt = millis() - tMillisConfig;
    if (dt > CONFIG_INVOKE_DELAY_TIME_MS) {
    #ifdef OLED
        Adafruit_GFX *display = device->display();

        float dta = sqrt(((float)dt-CONFIG_INVOKE_DELAY_TIME_MS)/((float)CONFIG_INVOKE_TIME_MS-CONFIG_INVOKE_DELAY_TIME_MS));
        uint8_t w = dta * 64;
        display->fillRoundRect(64-w, 4, w*2, 58, 4, WHITE);
        display->setTextColor(INVERSE);
        display->setTextSize(3);
        display->setCursor(10,25);
        display->print("CONFIG");        
    #endif
    } else {
        drawStateHome();
    }

#ifdef LED
    CRGB *leds = device->ledsInternal();
    memset(leds, 0, sizeof(CRGB)*LED_STRIP_SIZE);
    for (uint8_t i=0; i<(dt * LED_STRIP_SIZE/2) / CONFIG_INVOKE_TIME_MS; i++) {
        leds[i] = CRGB(50, 50, 50);
        leds[LED_STRIP_SIZE - i - 1] = CRGB(50, 50, 50);
    }
#endif
}

void CStateController::drawStateConfig() {
#ifdef OLED
    configManager->OLED_Status(device->display());
#endif
#ifdef LED
    configManager->LED_Status(device->ledsInternal());
#endif
}

#ifdef KEYPAD
void CStateController::keyEvent(key_status_t key) {

    switch (key) {
        case KEY_MIDDLE: {
            if (state == S_HOME_SCREEN) {
                state = S_INVOKING_CONFIG;
                tMillisConfig = millis();
            } else if (state == S_INVOKING_CONFIG && millis() - tMillisConfig > CONFIG_INVOKE_TIME_MS) {
                state = S_CONFIG;
                tMillisConfig = millis();
            }

            if (state == S_CONFIG && millis() - tMillisConfig > 2000) {
                state = S_HOME_SCREEN;
            }
        } break;
        default: {
            if (state == S_INVOKING_CONFIG) {
                state = S_HOME_SCREEN; // Keep last state or stack
            }
        };
    }

    switch (state) {
        case S_CONFIG: configManager->keyEvent(key); break;
        default: {
            for(CBaseManager *manager : managers) {
                manager->keyEvent(key);
            }
        };
    }

}
#endif
