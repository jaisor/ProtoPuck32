#include <Arduino.h>
#include "MatrixModePixel.h"

#ifdef LED_EXTERNAL_MATRIX

int x = 10;
int y = 10;

CMatrixModePixel::CMatrixModePixel(const uint8_t width, const uint8_t height)
: CBaseMatrixMode(width, height) {
}

void CMatrixModePixel::draw(CRGB *leds) {

    if (millis() - tMillis > 50) {

        tMillis = millis();

        // clear
        memset(leds, 0, width * height * sizeof(CRGB));

        leds[x + y * width] = CRGB(50,200,100); // rand() % width + (rand() % height) * width

    }
    
}

#ifdef KEYPAD
void CMatrixModePixel::keyEvent(key_status_t key) {

    switch (key) { 
        case KEY_LEFT: x=x-1; break;
        case KEY_RIGHT: x=x+1; break;
        case KEY_UP: ; break;
        case KEY_DOWN: ; break;
        case KEY_MIDDLE: ; break;
        default: ;
    }
    
}
#endif

#endif