#ifndef _CONFIGURATION_H
#define _CONFIGURATION_H

#include <Arduino.h>

#define WIFI        // 2.4Ghz wifi access point
#define LED         // Individually addressible LED strip
#define OLED        // Display
#define TEMP_SENSOR // Temperature, humidity, pressure
#define KEYPAD      // Buttons

#define ALEX_GIFT   // Temp, since the keyboard is flipped. TODO: implement key mapping sequence.

#define BOOT_BUTTON 0
#define EEPROM_CONFIGURATION_START 0    // First EEPROM byte to be used for storing the configuration

#ifdef WIFI
    #define WIFI_SSID "ProtoPuck32"
    #define WIFI_PASS "password123"

    // If unable to connect, it will create a soft accesspoint
    #define WIFI_FALLBACK_SSID "PP" // device chip id will be suffixed
    #define WIFI_FALLBACK_PASS "password123"

    // Web server
    #define WEB_SERVER_PORT 80
#endif

#ifdef LED
    #define LED_PIN 13
    #define LED_STRIP_SIZE 31
    #define LED_BRIGHTNESS 0.5 // 0-1
    #define LED_TYPE WS2811
    #define LED_COLOR_ORDER GRB

    #define LED_EXTERNAL    
    #ifdef LED_EXTERNAL
        #define LED_EXTERNAL_PIN 12
        #define LED_EXTERNAL_STRIP_SIZE 484 // 28
        #define LED_EXTERNAL_BRIGHTNESS 0.2 // 0-1
        #define LED_EXTERNAL_TYPE WS2812B
        #define LED_EXTERNAL_COLOR_ORDER GRB

        #define LED_EXTERNAL_MATRIX
        #ifdef LED_EXTERNAL_MATRIX
            #define LED_EXTERNAL_MATRIX_WIDTH 22
            #define LED_EXTERNAL_MATRIX_HEIGHT 22
            #define LED_EXTERNAL_MATRIX_SERPENTINE true
            #define LED_EXTERNAL_MATRIX_VERTICAL false
        #endif
    #endif

#endif

#ifdef OLED
    #define OLED_SCREEN_WIDTH 128 // OLED display width, in pixels
    #define OLED_SCREEN_HEIGHT 64 // OLED display height, in pixel
    #define OLED_I2C_ID  0x3C
#endif

#ifdef KEYPAD
    #define KEYPAD_PIN 34
#endif

#ifdef TEMP_SENSOR
    #define BME_I2C_ID  0x76
#endif

struct configuration_t {

    bool _loaded; // used to check if EEPROM was empty, should be true

    #ifdef WIFI
        char wifiSsid[32];
        char wifiPassword[63];
    #endif

    #ifdef LED
        float ledBrightness;
    #endif
};

extern configuration_t configuration;

void EEPROM_saveConfig();
void EEPROM_loadConfig();
void EEPROM_wipe();

#endif