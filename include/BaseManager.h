#ifndef _BASE_MANAGER_H
#define _BASE_MANAGER_H

#include "Configuration.h"
#include "Device.h"

#ifdef LED
    #include <FastLED.h>
#endif
#ifdef OLED
    #include <Adafruit_GFX.h>
#endif

class CBaseManager {

public:

#ifdef LED
    virtual uint16_t LED_Status(CRGB *leds) { return 0; };
#endif
#ifdef OLED
    virtual uint16_t OLED_Status(Adafruit_GFX *oled) { return 0; };
#endif
#ifdef KEYPAD
    virtual void keyEvent(key_status_t key) { };
#endif

    virtual void loop() {};
};

#endif