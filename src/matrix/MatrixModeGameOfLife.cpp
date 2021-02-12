#include <Arduino.h>
#include "MatrixModeGameOfLife.h"

CMatrixModeGameOfLife::CMatrixModeGameOfLife(uint8_t width, uint8_t height)
: CBaseMatrixMode(width, height) {

    current = new CRGB[width*height];
    next = new CRGB[width*height];
    previous = new CRGB[width*height];

    live = CRGB(255, 255, 255);
    dead = CRGB(0, 0, 0);
    populationDensity = 10; // percent of initially alive cells

    randomize();

}

void CMatrixModeGameOfLife::draw(CRGB *leds) {

    if (millis() - tMillis >100) {
        tMillis = millis();

        for (uint8_t x=0; x<width; x++) {
            for (uint8_t y=0; y<height; y++) {
                
                uint16_t i = y * width + x;
                uint8_t n = howManyNeighbors(x, y);

                if (isAlive(x, y)) {
                    if (n<2 || n>3) {
                        next[i] = dead;
                    } else {
                        next[i] = live;
                    }
                } else {
                    if (n == 3) {
                        next[i] = live;
                    } else {
                        next[i] = dead;
                    }
                }

            }
        }
        
        // stuck?
        if (memcmp(current, next, width * height * sizeof(CRGB)) == 0 
            || memcmp(previous, next, width * height * sizeof(CRGB)) == 0) {
            randomize();
        } else {
            // copy
            memcpy(previous, current, width * height * sizeof(CRGB));
            memcpy(current, next, width * height * sizeof(CRGB));
        }

        // display the leds
        for (uint8_t x=0; x<width; x++) {
            for (uint8_t y=0; y<height; y++) {
                leds[XY(x,y)] = current[y*width + x];
            }
        }
        //memcpy(leds, current, width * height * sizeof(CRGB));

        for (uint16_t i=0; i<width * height; i++) {
            leds[i].r = leds[i].r * configuration.ledBrightness; 
            leds[i].g = leds[i].g * configuration.ledBrightness; 
            leds[i].b = leds[i].b * configuration.ledBrightness; 
        }
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
        if (rand() % 100 <= populationDensity)  {
            current[i] = live;
        } else {
            current[i] = dead;
        }
    }
}

uint8_t CMatrixModeGameOfLife::howManyNeighbors(uint8_t x, uint8_t y) {

    uint8_t n = 0;

    if (isAlive(x-1, y-1)) n++;
    if (isAlive(x, y-1)) n++;
    if (isAlive(x+1, y-1)) n++;
    if (isAlive(x-1, y)) n++;
    if (isAlive(x+1, y)) n++;
    if (isAlive(x-1, y+1)) n++;
    if (isAlive(x, y+1)) n++;
    if (isAlive(x+1, y+1)) n++;

    return n;
}

bool CMatrixModeGameOfLife::isAlive(uint8_t x, uint8_t y) {
    if (x<0) return false;
    if (x>=width) return false;
    if (y<0) return false;
    if (y>=height) return false;

    if (current[y*width + x] == live) return true;
    else return false;
}
