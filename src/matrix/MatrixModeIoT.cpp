#include <Arduino.h>
#include "MatrixModeIoT.h"


CMatrixModeIoT::CMatrixModeIoT(uint8_t width, uint8_t height)
: CBaseMatrixMode(width, height) {
    this->leds = new CRGB[width * height];
}

void CMatrixModeIoT::draw(CRGB *leds) {

    if (millis() - tMillis > 10) {
        tMillis = millis();

        //memset(leds, 0, width * height * sizeof(CRGB));
    }
    
}

void CMatrixModeIoT::setLeds(matrix_pixel_t *pixels) {
    // TODO
}

#ifdef KEYPAD
void CMatrixModeIoT::keyEvent(key_status_t key) {
    switch (key) {
        case KEY_MIDDLE: ; break;
        default: ;
    }
}
#endif


