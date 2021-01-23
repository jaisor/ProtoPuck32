#ifndef _CONFIGURATION_H
#define _CONFIGURATION_H

#include <Arduino.h>

#define WIFI        // 2.4Ghz wifi access point
#define LED         // Individually addressible LED strip
#define OLED        // Display
#define WEATHER     // Temperature, humidity, pressure
#define KEYPAD      // Buttons

#define BOOT_BUTTON 0
#define EEPROM_CONFIGURATION_START 10    // First EEPROM byte to be used for storing the configuration

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
    #define LED_STRIP_SIZE 484 // 28
    #define LED_BRIGHTNESS 20
    #define LED_TYPE WS2811
    #define LED_COLOR_ORDER GRB

    #define LED_EXTERNAL    
    #ifdef LED
        #define LED_EXTERNAL_PIN 12
        #define LED_EXTERNAL_STRIP_SIZE 484 // 28
        #define LED_EXTERNAL_TYPE WS2812B
        #define LED_EXTERNAL_COLOR_ORDER GRB
    #endif

#endif

#ifdef OLED
    #define OLED_SCREEN_WIDTH 128 // OLED display width, in pixels
    #define OLED_SCREEN_HEIGHT 64 // OLED display height, in pixels
#endif

#ifdef KEYPAD
    #define KEYPAD_PIN 34
#endif

struct configuration_t
{
    #ifdef WIFI
        char wifi_ssid[32];
        char wifi_password[63];
    #endif
};

extern configuration_t configuration;

void EEPROM_saveConfig();
void EEPROM_loadConfig();
void EEPROM_wipe();

#endif