#ifndef _MATRIX_MODE_SOUND_LEVEL_H
#define _MATRIX_MODE_SOUND_LEVEL_H

#include "BaseCanvasedMatrixMode.h"

#ifdef LED_EXTERNAL_MATRIX
#ifdef MIC_I2S
#include <FastLED.h>
#include <Adafruit_GFX.h>

class CMatrixModeSoundLevel : public CBaseCanvasedMatrixMode {

private:

    uint32_t Leq_samples = 0;
    double Leq_sum_sqr = 0;
    double Leq_dB = 0;
    uint8_t led_level_step;

public:
	CMatrixModeSoundLevel(uint8_t width, uint8_t height);
    virtual void draw(CRGB *leds);
    #ifdef KEYPAD
        virtual void keyEvent(key_status_t key);
    #endif
};

#endif
#endif
#endif