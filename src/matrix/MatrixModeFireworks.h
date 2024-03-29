#ifndef _MATRIX_MODE_FIREWORKS_H
#define _MATRIX_MODE_FIREWORKS_H

#include "BaseCanvasedMatrixMode.h"
#include "firework/Firework.h"

#ifdef LED_EXTERNAL_MATRIX
#include <FastLED.h>
#include <Adafruit_GFX.h>

class CMatrixModeFireworks : public CBaseCanvasedMatrixMode {

private:
    CFirework** fireworks;

public:
	CMatrixModeFireworks(const uint8_t width, const uint8_t height);
    ~CMatrixModeFireworks();
    virtual void draw(CRGB *leds);
    #ifdef KEYPAD
        virtual void keyEvent(key_status_t key);
    #endif
};

#endif
#endif