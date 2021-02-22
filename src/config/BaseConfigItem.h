#ifndef _BASE_CONFIG_ITEM_H
#define _BASE_CONFIG_ITEM_H

#include <Arduino.h>
#include "Configuration.h"
#include "Device.h"
#ifdef LED
    #include <FastLED.h>
#endif
#ifdef OLED
    #include <Adafruit_GFX.h>
#endif

class CBaseConfigItem {

private:
    String _name;
    
public:
	CBaseConfigItem(String name): _name(name) {};
#ifdef LED
    virtual void LED_Status(CRGB *leds) { };
#endif
#ifdef OLED
    virtual void OLED_Status(Adafruit_GFX *oled) { };
#endif
#ifdef KEYPAD
    virtual void keyEvent(key_status_t key) { };
#endif
    virtual void loop() { };

    String name() { return _name; };
};

#endif