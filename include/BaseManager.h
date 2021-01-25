#ifndef _BASE_MANAGER_H
#define _BASE_MANAGER_H

#include <Adafruit_GFX.h>
#include <FastLED.h>

typedef enum {
    KEY_NONE = 0,
    KEY_UP = 1,
    KEY_DOWN = 2,
    KEY_LEFT = 3,
    KEY_RIGHT = 4,
    KEY_MIDDLE = 5
} key_status_t;


class CBaseManager {
public:
    // Return value is the recommended refresh delay in ms
	virtual uint16_t LED_Status(CRGB *leds) { return 0; };
    virtual uint16_t OLED_Status(Adafruit_GFX *oled) { return 0; };
    virtual void keyEvent(key_status_t key) { };
    virtual void loop() {};
};

#endif