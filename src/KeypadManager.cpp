#include <Arduino.h>
#include "KeypadManager.h"

const unsigned char key_bitmaps [6][8] PROGMEM = {
    { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }, // NONE
	{ 0x0, 0x10, 0x38, 0x7c, 0x10, 0x10, 0x10, 0x0 }, // UP
    { 0x0, 0x10, 0x10, 0x10, 0x7c, 0x38, 0x10, 0x0 }, // DOWN
    { 0x0, 0x0, 0x10, 0x30, 0x7e, 0x30, 0x10, 0x0 }, // LEFT
    { 0x0, 0x0, 0x8, 0xc, 0x7e, 0xc, 0x8, 0x0 }, // RIGHT
    { 0x0, 0x0, 0x54, 0x38, 0x7c, 0x38, 0x54, 0x0 } // MIDDLE
};

key_status_t operator ++( key_status_t &id, int ) {
    key_status_t currentID = id;

    if ( NUM_KEYS < id + 1 ) id = KEY_NONE;
    else id = static_cast<key_status_t>( id + 1 );

    return ( currentID );
}

CKeypadManager::CKeypadManager() {
    keyStatus = KEY_NONE;
    listener = nullptr;
    tMillis = millis();
    memset(keyEventCounter, 0, sizeof(keyEventCounter));
}


#ifdef OLED
uint16_t CKeypadManager::OLED_Status(Adafruit_GFX *oled) {

    oled->setTextSize(1);
    oled->setCursor(70, 24);
    oled->print("K:");
    oled->print(String(analogRead(KEYPAD_PIN)));

    oled->drawBitmap(116, 24, key_bitmaps[keyStatus], 8, 8, 1);

    return 100;
}
#endif

void CKeypadManager::loop() {

    uint16_t k = analogRead(KEYPAD_PIN);

    #ifdef ALEX_GIFT
        if (k > 3900) {
            keyStatus = KEY_NONE;
        } else if (k > 2300 && k < 2500) {
            keyStatus = KEY_RIGHT;
        } else if (k < 300) {
            keyStatus = KEY_LEFT;
        } else if (k > 2600 && k < 3000) {
            keyStatus = KEY_UP;
        } else if (k > 1200 && k < 1700) {
            keyStatus = KEY_DOWN;
        } else if (k > 3200 && k < 3400) {
            keyStatus = KEY_MIDDLE;
        }
    #else
        if (k > 4000) {
            keyStatus = KEY_NONE;
        } else if (k > 2300 && k < 2500) {
            keyStatus = KEY_LEFT;
        } else if (k < 300) {
            keyStatus = KEY_RIGHT;
        } else if (k > 2600 && k < 3000) {
            keyStatus = KEY_DOWN;
        } else if (k > 1000 && k < 1600) {
            keyStatus = KEY_UP;
        } else if (k > 3200 && k < 3400) {
            keyStatus = KEY_MIDDLE;
        }
    #endif 

    keyEventCounter[keyStatus]++;

    // Filter the noise and focus only on the most prominent event during the time period 
    if (millis() - tMillis > 10) {
        tMillis = millis();
        uint16_t max = 0;
        key_status_t maxKey = KEY_NONE;
        for(key_status_t i=KEY_NONE; i<NUM_KEYS; i++) {
            if (keyEventCounter[i] > max) {
                max = keyEventCounter[i];
                maxKey = i;
            }
        }
        memset(keyEventCounter, 0, sizeof(keyEventCounter));
        
        if (maxKey != KEY_NONE) {
            listener_t *cur = this->listener;
            while(cur) {
                cur->listener(maxKey);
                cur = cur->next;
            }
        }
    }

}

void CKeypadManager::addKeyListener(TKeyListener listener) {

    listener_t *l = new listener_t { listener, nullptr }; 

    if (!this->listener) {
        
        this->listener = l;
        log_d("Added first keypad listener");
    } else {
        uint8_t i = 0;
        listener_t *cur = this->listener;
        while(cur) {
            if (!cur->next) {
                log_d("Added keypad listener number %i", i);
                cur->next = l;
                return;
            }
            cur = cur->next;
            i++;
        }
    }
   
}