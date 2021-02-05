#ifndef _MATRIX_MODE_RGB_DEMO_H
#define _MATRIX_MODE_RGB_DEMO_H

#include "BaseMatrixMode.h"

#ifdef LED_EXTERNAL_MATRIX
#include <FastLED.h>

class CMatrixModeRGBDemo : public CBaseMatrixMode {

private:

public:
	CMatrixModeRGBDemo(uint8_t width, uint8_t height);
    virtual void draw(CRGB *leds);
};

#endif
#endif