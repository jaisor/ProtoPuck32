#ifndef _KEYPAD_MANAGER_H
#define _KEYPAD_MANAGER_H

#include <functional>
#include "BaseManager.h"

#define MAX_LISTENERS 10

class CKeypadManager: public CBaseManager {

public:
    typedef std::function<void(key_status_t)> TKeyListener;
    struct listener_t {
        TKeyListener listener;
        listener_t *next;
    };

	CKeypadManager();

#ifdef OLED
    virtual uint16_t OLED_Status(Adafruit_GFX *oled);
#endif

    virtual void loop();
    void addKeyListener(TKeyListener listener);

private:
    unsigned long tMillis;
    key_status_t keyStatus; 

    listener_t *listener; // Linked list of TKeyListener nodes
};

#endif