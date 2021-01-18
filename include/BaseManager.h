#ifndef _BASE_MANAGER_H
#define _BASE_MANAGER_H

#include <Adafruit_GFX.h>

class CBaseManager {
public:
    // Return value is the recommended refresh delay in ms
	virtual uint16_t LED_Status() { return 0; };
    virtual uint16_t OLED_Status(Adafruit_GFX *oled) { return 0; };
};

#endif