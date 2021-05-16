#include <Arduino.h>
#include "MatrixModeMovingSquare.h"

#ifdef LED_EXTERNAL_MATRIX

static float xf = (rand() % 1000) / 1000.0f;
static float yf = (rand() % 1000) / 1000.0f;

static float vxf = 0;
static float vyf = 0;


CMatrixModeMovingSquare::CMatrixModeMovingSquare(uint8_t width, uint8_t height)
: CBaseMatrixMode(width, height) {
    /*
    log_d("Position (%0.1f, %0.1f)", xf, yf);
    log_d("Velocity (%0.1f, %0.1f)", vxf, vyf);
    */
}

void CMatrixModeMovingSquare::draw(CRGB *leds) {

    if (millis() - tMillis > 10) {
        tMillis = millis();

        memset(leds, 0, width * height * sizeof(CRGB));
         
        if (vxf == 0 ) { vxf = (rand() % 1000 - 500) / 100000.0f; }
        if (vyf == 0 ) { vyf = (rand() % 1000 - 500) / 100000.0f; }
        
        xf += vxf;
        yf += vyf;

        if (xf<0) { xf = 0; vxf *= -1 + (rand() % 1000 - 500) / 10000000.0f; }
        if (xf>1) { xf = 1; vxf *= -1 + (rand() % 1000 - 500) / 10000000.0f; }
        if (yf<0) { yf = 0; vyf *= -1 + (rand() % 1000 - 500) / 10000000.0f; }
        if (yf>1) { yf = 1; vyf *= -1 + (rand() % 1000 - 500) / 10000000.0f; }

        uint8_t xi = xf * width;
        uint8_t yi = yf * height;
        uint8_t b = configuration.ledBrightness * 255;

        //leds[ XYsafe(xi, yi) ]  = CHSV( 255, 0, b);
        leds[ XYsafe(xi-1, yi) ]  = CHSV( 255, 0, b);
        leds[ XYsafe(xi+1, yi) ]  = CHSV( 255, 0, b);
        leds[ XYsafe(xi, yi-1) ]  = CHSV( 255, 0, b);
        leds[ XYsafe(xi, yi+1) ]  = CHSV( 255, 0, b);
        leds[ XYsafe(xi-1, yi-1) ]  = CHSV( 255, 0, b);
        leds[ XYsafe(xi+1, yi-1) ]  = CHSV( 255, 0, b);
        leds[ XYsafe(xi-1, yi+1) ]  = CHSV( 255, 0, b);
        leds[ XYsafe(xi+1, yi+1) ]  = CHSV( 255, 0, b);
    }
    
}

#ifdef KEYPAD
void CMatrixModeMovingSquare::keyEvent(key_status_t key) {
    switch (key) {
        case KEY_LEFT: vxf -= 0.0000001; break;
        case KEY_RIGHT: vxf += 0.0000001; break;
        case KEY_UP: vyf -= 0.0000001; break;
        case KEY_DOWN: vyf += 0.0000001; break;
        case KEY_MIDDLE: vyf = vxf = 0; break;
        default: ;
    }
}
#endif

#endif