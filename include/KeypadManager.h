#ifndef _KEYPAD_MANAGER_H
#define _KEYPAD_MANAGER_H

#include "BaseManager.h"

class CKeypadManager: public CBaseManager {

private:
    unsigned long tMillis;

public:
	CKeypadManager();

    virtual uint16_t OLED_Status(Adafruit_GFX *oled);
    virtual void loop();
};

#endif