#ifndef _MATRIX_MODE_FIREWORKS_H
#define _MATRIX_MODE_FIREWORKS_H

#include "BaseMatrixMode.h"
#include "firework/Firework.h"

#ifdef LED_EXTERNAL_MATRIX
#include <FastLED.h>
#include <Adafruit_GFX.h>

#define NUM_FIREWORKS 5

class CMatrixModeFireworks : public CBaseMatrixMode {

private:
    GFXcanvas16 *canvas;
    CFirework fireworks[NUM_FIREWORKS];

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