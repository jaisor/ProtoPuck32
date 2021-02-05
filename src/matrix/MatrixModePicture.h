#ifndef _MATRIX_MODE_PICTURE_H
#define _MATRIX_MODE_PICTURE_H

#include "BaseMatrixMode.h"

#ifdef LED_EXTERNAL_MATRIX
#include <FastLED.h>

class CMatrixModePicture : public CBaseMatrixMode {

private:
    uint8_t index;
    bool indexChange;

    uint8_t *bitmap;
    uint8_t numBitmaps;

    void copyPicture(uint8_t i);

public:
	CMatrixModePicture(uint8_t width, uint8_t height, uint8_t *bitmap, uint8_t numBitmaps);
    virtual void draw(CRGB *leds);
    #ifdef KEYPAD
        virtual void keyEvent(key_status_t key);
    #endif
};

#endif
#endif