#ifndef _MATRIX_MODE_GAME_OF_LIFE_H
#define _MATRIX_MODE_GAME_OF_LIFE_H

#include "BaseMatrixMode.h"

#ifdef LED_EXTERNAL_MATRIX
#include <FastLED.h>

class CMatrixModeGameOfLife : public CBaseMatrixMode {

private:
    CRGB *previous;
    CRGB *current;
    CRGB *next;
    CRGB live;
    CRGB dead;

    uint8_t populationDensity;

    void randomize();
    uint8_t howManyNeighbors(uint8_t x, uint8_t y);
    bool isAlive(uint8_t x, uint8_t y);

public:
	CMatrixModeGameOfLife(uint8_t width, uint8_t height);
    virtual void draw(CRGB *leds);
    #ifdef KEYPAD
        virtual void keyEvent(key_status_t key);
    #endif
};

#endif
#endif