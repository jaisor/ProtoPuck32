#include <Arduino.h>
#include "MatrixModeClock.h"

CMatrixModeClock::CMatrixModeClock(const uint8_t width, const uint8_t height)
: CBaseMatrixMode(width, height) {
    canvas = new GFXcanvas16(width, height);
}

CMatrixModeClock::~CMatrixModeClock() {
    delete canvas;
}

void CMatrixModeClock::draw(CRGB *leds) {

    if (millis() - tMillis > 10) {
        tMillis = millis();
        // clear
        memset(leds, 0, width * height * sizeof(CRGB));

        canvas->fillScreen(0);
        canvas->drawCircle(11, 11, 10, 0xffff);

        m += 1;
        if (m>59) {
            m = 0; 
            if (++h>11) {
                h = 0;
            }
        }

        uint16_t x, y;
        double rad = m * PI / 30;
        x = 11 + 7 * cos(rad);
        y = 11 + 7 * sin(rad);
        canvas->drawLine(11, 11, x, y, 0xf104);
        //log_d("Min: %i, rad: %f, x: %i, y: %i", m, rad, x, y);

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
    
}

CRGB CMatrixModeClock::RGB565_to_CRGB(uint16_t color) {

    uint8_t r = ((color >> 11) & 0x1F);
    uint8_t g = ((color >> 5) & 0x3F);
    uint8_t b = (color & 0x1F);

    r = ((((color >> 11) & 0x1F) * 527) + 23) >> 6;
    g = ((((color >> 5) & 0x3F) * 259) + 33) >> 6;
    b = (((color & 0x1F) * 527) + 23) >> 6;

    return CRGB(r * configuration.ledBrightness, g * configuration.ledBrightness, b * configuration.ledBrightness);
}

#ifdef KEYPAD
void CMatrixModeClock::keyEvent(key_status_t key) {

    switch (key) { 
        case KEY_LEFT: break;
        case KEY_RIGHT: break;
        case KEY_UP: ; break;
        case KEY_DOWN: ; break;
        case KEY_MIDDLE: ; break;
        default: ;
    }
    
}
#endif


