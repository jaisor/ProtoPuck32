#ifndef _MATRIX_MODE_SOUND_LEVEL_H
#define _MATRIX_MODE_SOUND_LEVEL_H

#include "BaseCanvasedMatrixMode.h"

#ifdef LED_EXTERNAL_MATRIX
#ifdef MIC_I2S
#include <vector>
#include <FastLED.h>
#include <Adafruit_GFX.h>

// Sound level code heavily based on https://github.com/ikostoski/esp32-i2s-slm/blob/master/esp32-i2s-slm.ino

class CMatrixModeSoundLevel : public CBaseCanvasedMatrixMode {

private:

    uint32_t Leq_samples = 0;
    double Leq_sum_sqr = 0;
    double Leq_dB = 0;
    float ledWStep;
    float ledHStep;

    float dbRunningSum, dbRunningAvg, dbRunningMax;
    uint16_t dbRunningCount;
    unsigned long tMillisHistory;
    std::vector<float> dbHistory;

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