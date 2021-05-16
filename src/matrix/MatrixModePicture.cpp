#include <Arduino.h>
#include "MatrixModePicture.h"

#ifdef LED_EXTERNAL_MATRIX

#define BITMAP_BYTES_PER_PIXEL 4

CMatrixModePicture::CMatrixModePicture(uint8_t width, uint8_t height, const uint8_t *bitmap, const uint8_t numBitmaps)
: CBaseMatrixMode(width, height), bitmap(bitmap), numBitmaps(numBitmaps) {
    index = 0;
    indexChange = false;
}

void CMatrixModePicture::draw(CRGB *leds) {

    uint16_t pixels = width * height;

    if (indexChange && millis() - tMillis > 500) {
        tMillis = millis();
        index++;

        if (index >= numBitmaps) {
            index = 0;
        }
        log_d("Changed picture index to %i / %i", index, numBitmaps);
    }
    indexChange = false;

    for(uint8_t y=0; y<height; y++) {
        for(uint8_t x=0; x<width; x++) {
            uint8_t r, g, b;
            uint16_t bi = index * pixels * BITMAP_BYTES_PER_PIXEL + (y * width + x) * BITMAP_BYTES_PER_PIXEL;

            b = bitmap[bi] * configuration.ledBrightness;
            g = bitmap[bi+1] * configuration.ledBrightness;
            r = bitmap[bi+2] * configuration.ledBrightness;

            leds[ XY(x, y) ] = CRGB(r, g, b);           
        }
    }
}

void CMatrixModePicture::copyPicture(uint8_t i) {

/*
    for(uint8_t y=0; y<height; y++) {
        for(uint8_t x=0; x<width; x++) {
            uint8_t r, g, b;
            uint16_t bi = (y * width + x) * 4;

            b = bitmap[index][bi] * configuration.ledBrightness;
            g = bitmap[index][bi+1] * configuration.ledBrightness;
            r = bitmap[index][bi+2] * configuration.ledBrightness;

            leds[ XY(x, y) ] = CRGB(r, g, b);           
        }
    }
*/
    /*
    uint16_t size = width * height;
    for (uint16_t i = 0; i<size; i++) {
        uint8_t r, g, b;
        uint16_t bi = i*4;

        b = bitmap[index][bi] * configuration.ledBrightness;
        g = bitmap[index][bi+1] * configuration.ledBrightness;
        r = bitmap[index][bi+2] * configuration.ledBrightness;

        this->leds[i] = CRGB(r, g, b);
    }
    */
}

#ifdef KEYPAD
void CMatrixModePicture::keyEvent(key_status_t key) {
    switch (key) {
        case KEY_MIDDLE: indexChange = true; break;
        default: indexChange = false;
    }
}
#endif

#endif