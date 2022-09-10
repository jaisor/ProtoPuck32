#ifndef _CONFIGURATION_H
#define _CONFIGURATION_H

#include <Arduino.h>
#include <ArduinoLog.h>

#define WIFI    // 2.4Ghz wifi access point
#define LED     // Individually addressible LED strip
#define OLED    // Display
#define TEMP_SENSOR // Temperature, humidity, pressure
#define KEYPAD    // Buttons

//#define ALEX_GIFT   // Temp, since the keyboard is flipped. TODO: implement key mapping sequence.

#define BOOT_BUTTON 0

#ifdef WIFI
  #define WIFI_SSID "ProtoPuck32"
  #define WIFI_PASS "password123"

  // If unable to connect, it will create a soft accesspoint
  #define WIFI_FALLBACK_SSID "PP" // device chip id will be suffixed
  #define WIFI_FALLBACK_PASS "password123"

  #define NTP_SERVER "pool.ntp.org"
  #define NTP_GMT_OFFSET_SEC -25200
  #define NTP_DAYLIGHT_OFFSET_SEC 0

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
    #define LED_EXTERNAL_STRIP_SIZE 700 // 70 PER LEAF // 28
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

    //#define LED_EXTERNAL_LEAFS
    #ifdef LED_EXTERNAL_LEAFS
      #define LED_EXTERNAL_LEAF_SIZE 70
    #endif

    #define MIC_I2S
    #ifdef MIC_I2S
      #define MIC_I2S_WS      25 
      #define MIC_I2S_SCK       27 
      #define MIC_I2S_SD      26
      // I2S peripheral to use (0 or 1)
      #define MIC_I2S_PORT      I2S_NUM_0
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
  #define TEMP_UNIT_CELSIUS   0
  #define TEMP_UNIT_FAHRENHEIT  1
  #define BME_I2C_ID  0x76
#endif

#define EEPROM_FACTORY_RESET 0       // Byte to be used for factory reset device fails to start or is rebooted within 1 sec 3 consequitive times
#define EEPROM_CONFIGURATION_START 1   // First EEPROM byte to be used for storing the configuration

#define FACTORY_RESET_CLEAR_TIMER_MS 3000   // Clear factory reset counter when elapsed, considered smooth boot

#define DEVICE_NAME "PP32"

#define MQTT_DATA_JSON  0
#define MQTT_DATA_SCALAR  1
#define MQTT_DATA_BOTH  2

struct configuration_t {

  #ifdef WIFI
    char wifiSsid[32];
    char wifiPassword[63];

    char ntpServer[128];
    long gmtOffset_sec = 0;
    int daylightOffset_sec = 3600;

    char mqttServer[128];
    int mqttPort;
    char mqttTopic[64];
    uint16_t mqttDataType; // 0-json(one message); 1-scalar(multiple messages); 2-both; 
  #endif

  #ifdef LED
    float ledBrightness;
    float ledBrightnessTime;
  #endif

  char name[128];
  uint8_t tempUnit;

  char _loaded[7]; // used to check if EEPROM was correctly set
};

extern configuration_t configuration;

uint8_t EEPROM_initAndCheckFactoryReset();
void EEPROM_clearFactoryReset();

void EEPROM_saveConfig();
void EEPROM_loadConfig();
void EEPROM_wipe();

#ifdef LED
  void CONFIG_updateLedBrightnessTime();
#endif

#endif