#ifndef _BASE_CANVASED_MATRIX_MODE_CLOCK_H
#define _BASE_CANVASED_MATRIX_MODE_CLOCK_H

#include "BaseMatrixMode.h"

#ifdef LED_EXTERNAL_MATRIX
#include <FastLED.h>
#include <Adafruit_GFX.h>

class CBaseCanvasedMatrixMode : public CBaseMatrixMode {

protected:
    GFXcanvas16 *canvas;
    
    static CRGB RGB565_to_CRGB(uint16_t a); // 5-6-5 16bit pixel conversion to 24bit CRGB
    static uint16_t CRGB_to_RGB565(CRGB c); // CRGB to 5-6-5 16bit pixel

    void drawThickLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color, uint8_t thickness);

public:
	CBaseCanvasedMatrixMode(const uint8_t width, const uint8_t height);
    ~CBaseCanvasedMatrixMode();
    void drawCanvas(CRGB *leds);
};

#endif
#endif