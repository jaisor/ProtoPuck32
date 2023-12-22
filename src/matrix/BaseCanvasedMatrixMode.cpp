#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Fonts/Picopixel.h>
#include "BaseCanvasedMatrixMode.h"

#ifdef LED_EXTERNAL_MATRIX

CBaseCanvasedMatrixMode::CBaseCanvasedMatrixMode(const uint8_t width, const uint8_t height)
: CBaseMatrixMode(width, height) {
    canvas = new GFXcanvas16(width, height);

    canvas->setTextColor(0xffff);
    canvas->setTextSize(1);
    canvas->setFont(&Picopixel);
}

CBaseCanvasedMatrixMode::~CBaseCanvasedMatrixMode() {
    delete canvas;
}

CRGB CBaseCanvasedMatrixMode::RGB565_to_CRGB(uint16_t color) {

    uint8_t r = ((color >> 11) & 0x1F);
    uint8_t g = ((color >> 5) & 0x3F);
    uint8_t b = (color & 0x1F);

    r = ((((color >> 11) & 0x1F) * 527) + 23) >> 6;
    g = ((((color >> 5) & 0x3F) * 259) + 33) >> 6;
    b = (((color & 0x1F) * 527) + 23) >> 6;

    return CRGB(r * configuration.ledBrightnessTime, g * configuration.ledBrightnessTime, b * configuration.ledBrightnessTime);
}

uint16_t CBaseCanvasedMatrixMode::CRGB_to_RGB565(CRGB c) {
    return (uint16_t) (((c.r >> 3) & 0x1f) << 11 | ((c.g >> 2) & 0x3f) << 5 | (c.b >> 3) & 0x1f);
}

void CBaseCanvasedMatrixMode::drawCanvas(CRGB *leds) {
    
    // TODO: performance optimize
    uint16_t *b = canvas->getBuffer();
    for(uint8_t x=0; x<width; x++) {
        for(uint8_t y=0; y<height; y++) {
            uint16_t c565 = b[y * height + x];
            if (c565) {
                leds[XY(x, y)] = RGB565_to_CRGB(b[y * height + x]);
            }
        }
    }
    
}

#ifndef _swap_int16_t
#define _swap_int16_t(a, b)                                                    \
  {                                                                            \
    int16_t t = a;                                                             \
    a = b;                                                                     \
    b = t;                                                                     \
  }
#endif

/**************************************************************************/
/*!
   @brief    Draw line where each pixel is filled rect.  Bresenham's algorithm - thx wikpedia
    @param    x0  Start point x coordinate
    @param    y0  Start point y coordinate
    @param    x1  End point x coordinate
    @param    y1  End point y coordinate
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void CBaseCanvasedMatrixMode::drawThickLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color, uint8_t thickness) {
    int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    uint8_t thickDiv2 = thickness / 2;
    if (steep) {
        _swap_int16_t(x0, y0);
        _swap_int16_t(x1, y1);
    }

    if (x0 > x1) {
        _swap_int16_t(x0, x1);
        _swap_int16_t(y0, y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for (; x0 <= x1; x0++) {
        if (steep) {
            canvas->fillRect(y0-thickDiv2, x0-thickDiv2, thickness, thickness, color);
        } else {
            canvas->fillRect(x0-thickDiv2, y0-thickDiv2, thickness, thickness, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

#endif