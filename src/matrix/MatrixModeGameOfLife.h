#ifndef _MATRIX_MODE_GAME_OF_LIFE_H
#define _MATRIX_MODE_GAME_OF_LIFE_H

#include "BaseMatrixMode.h"

#ifdef LED_EXTERNAL_MATRIX
#include <FastLED.h>

class CMatrixModeGameOfLife : public CBaseMatrixMode {

private:
    uint8_t *previous;
    uint8_t *current;
    uint8_t *next;

    const uint16_t size;

    CRGB live;
    CRGB dead;

    uint8_t populationDensity;

    void randomize();
    uint8_t howManyNeighbors(uint8_t x, uint8_t y);
    bool isAlive(const uint8_t x, const uint8_t y);
    CRGB getAlive(const uint8_t x, const uint8_t y);
    CRGB getDead(const uint8_t x, const uint8_t y);

public:
	CMatrixModeGameOfLife(const uint8_t width, const uint8_t height);
    virtual void draw(CRGB *leds);
    #ifdef KEYPAD
        virtual void keyEvent(key_status_t key);
    #endif
};

#endif
#endif