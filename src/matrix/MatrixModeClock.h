#ifndef _MATRIX_MODE_CLOCK_H
#define _MATRIX_MODE_CLOCK_H

#include "BaseCanvasedMatrixMode.h"

#ifdef LED_EXTERNAL_MATRIX
#include <FastLED.h>
#include <Adafruit_GFX.h>

class CMatrixModeClock : public CBaseCanvasedMatrixMode {

private:
    uint8_t h, m;

    const uint16_t colMin, colHour, colText;

    struct tm timeinfo;
    unsigned long tMillisClockUpdate;
    bool timeUpdated;

public:
	CMatrixModeClock(const uint8_t width, const uint8_t height);
    ~CMatrixModeClock();
    virtual void draw(CRGB *leds);
    #ifdef KEYPAD
        virtual void keyEvent(key_status_t key);
    #endif
};

#endif
#endif