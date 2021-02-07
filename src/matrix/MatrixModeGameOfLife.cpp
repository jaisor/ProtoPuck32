#include <Arduino.h>
#include "MatrixModeGameOfLife.h"

CMatrixModeGameOfLife::CMatrixModeGameOfLife(uint8_t width, uint8_t height)
: CBaseMatrixMode(width, height) {

    current = new CRGB[width*height];
    randomize();

}

void CMatrixModeGameOfLife::draw(CRGB *leds) {

    if (millis() - tMillis > 10) {
        tMillis = millis();

        memcpy(leds, current, width * height * sizeof(CRGB));
    }
    
}

#ifdef KEYPAD
void CMatrixModeGameOfLife::keyEvent(key_status_t key) {
    switch (key) {
        case KEY_MIDDLE: randomize(); break;
        default: ;
    }
}
#endif

void CMatrixModeGameOfLife::randomize() {
    uint16_t size = width * height;
    for (uint16_t i=0; i<size; i++) {
        if (rand() % 2 == 0)  {
            current[i] = CRGB(0, 0, 0);
        } else {
            current[i] = CRGB(255 * configuration.ledBrightness, 255 * configuration.ledBrightness, 255 * configuration.ledBrightness);
        }
    }
}

