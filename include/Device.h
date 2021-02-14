#ifndef _DEVICE_MANAGER_H
#define _DEVICE_MANAGER_H

#include <functional>
#include "Configuration.h"

#ifdef LED
    #include <FastLED.h>
#endif

#ifdef OLED
    #include <Adafruit_SSD1306.h>
    #include <Adafruit_GFX.h>
#endif

#ifdef TEMP_SENSOR
    #include <Adafruit_Sensor.h>
    #include <Adafruit_BME280.h>
#endif

#ifdef KEYPAD
    typedef enum {
        KEY_NONE = 0,
        KEY_UP,
        KEY_DOWN,
        KEY_LEFT,
        KEY_RIGHT,
        KEY_MIDDLE,
        NUM_KEYS
    } key_status_t;
#endif

class CDevice {

public:
    typedef std::function<void(key_status_t)> TKeyListener;
    struct listener_t {
        TKeyListener listener;
        listener_t *next;
    };

	CDevice();
    ~CDevice();
    void loop();

#ifdef TEMP_SENSOR
    float temperature() const { return _bme->readTemperature(); /*_temperature;*/ };
    float humidity() const { return _bme->readHumidity(); /*_humidity;*/ };
    float altitude() const { return _altitude; };
#endif

#ifdef LED
    CRGB* ledsInternal() { return _ledsInternal; };
    CRGB* ledsExternal() { return _ledsExternal; };
#endif

#ifdef OLED
    Adafruit_SSD1306* display() const { return _display; };
#endif

    key_status_t keyStatus() const { return _keyStatus; };

    void addKeyListener(TKeyListener listener);

private:
    
    unsigned long tMillisTemp;
    unsigned long tMillisKey;

#ifdef LED
    CRGB _ledsInternal[LED_STRIP_SIZE];
    CRGB _ledsExternal[LED_EXTERNAL_STRIP_SIZE];
#endif

#ifdef OLED
    Adafruit_SSD1306 *_display;
#endif

#ifdef TEMP_SENSOR
    float _temperature, _humidity, _altitude;
    Adafruit_BME280 *_bme;
#endif

    key_status_t _keyStatus; 
    uint16_t keyEventCounter[NUM_KEYS];

    listener_t *listener; // Linked list of TKeyListener nodes
};

#endif