#include <Arduino.h>
#include "MatrixModeGameOfLife.h"

CMatrixModeGameOfLife::CMatrixModeGameOfLife(const uint8_t width, const uint8_t height)
: CBaseMatrixMode(width, height), size(width * height) {

    current = new uint8_t[size];
    next = new uint8_t[size];
    previous = new uint8_t[size];

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
                        next[i] = 0;
                    } else {
                        next[i] = 1;
                    }
                } else {
                    if (n == 3) {
                        next[i] = 1;
                    } else {
                        next[i] = 0;
                    }
                }

            }
        }
        
        // stuck?
        if (memcmp(current, next, size) == 0 
            || memcmp(previous, next, size) == 0) {
            randomize();
        } else {
            // copy
            memcpy(previous, current, size);
            memcpy(current, next, size);
        }

        // display the leds
        for (uint8_t x=0; x<width; x++) {
            for (uint8_t y=0; y<height; y++) {
                leds[XY(x,y)] = isAlive(x, y) ? getAlive(x, y) : getDead(x, y);
            }
        }

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
            current[i] = 1;
        } else {
            current[i] = 0;
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

bool CMatrixModeGameOfLife::isAlive(const uint8_t x, const uint8_t y) {
    if (x<0) return false;
    if (x>=width) return false;
    if (y<0) return false;
    if (y>=height) return false;

    if (current[y*width + x]) return true;
    else return false;
}

CRGB CMatrixModeGameOfLife::getAlive(const uint8_t x, const uint8_t y) {

    uint8_t n = howManyNeighbors(x, y);

    if (n == 8) return CRGB(250, 250, 250);
    if (n == 7) return CRGB(200, 200, 200);
    if (n == 6) return CRGB(160, 160, 160);
    if (n == 5) return CRGB(80, 80, 80);
    if (n == 4) return CRGB(40, 40, 40);
    if (n == 3) return CRGB(20, 20, 20);
    if (n == 2) return CRGB(10, 10, 10);
    if (n == 1) return CRGB(5, 5 , 5);
    if (n ==0) return CRGB(1, 1, 1);

    //return CRGB(rand() % 256, rand() % 256, rand() % 256);
}

CRGB CMatrixModeGameOfLife::getDead(const uint8_t x, const uint8_t y) {
    return dead;
}