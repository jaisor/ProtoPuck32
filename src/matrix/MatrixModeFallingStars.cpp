#include <Arduino.h>
#include "MatrixModeFallingStars.h"

CMatrixModeFallingStars::CMatrixModeFallingStars(const uint8_t width, const uint8_t height)
: CBaseMatrixMode(width, height) {

    randomize();

}

void CMatrixModeFallingStars::draw(CRGB *leds) {

    if (millis() - tMillis > 10) {

        unsigned int time = millis() - tMillis;

        tMillis = millis();

        memset(leds, 0, sizeof(CRGB) * width * height);

        for(uint8_t i=0; i<NUM_STARS; i++) {
            leds[XY(stars[i].x, stars[i].y)] = CRGB(
                stars[i].color.r * configuration.ledBrightness * stars[i].v/5,
                stars[i].color.g * configuration.ledBrightness * stars[i].v/5,
                stars[i].color.b * configuration.ledBrightness * stars[i].v/5
            );

            stars[i].y += (float)(time * stars[i].v) / 100.0;
            if (stars[i].y > height-1) {
                stars[i].y = 0;
                stars[i].x = rand() % width;
                stars[i].v = rand() % 5 + 1;
            }
        }
    }
    
}

#ifdef KEYPAD
void CMatrixModeFallingStars::keyEvent(key_status_t key) {
    switch (key) {
        case KEY_MIDDLE: randomize(); break;
        default: ;
    }
}
#endif

void CMatrixModeFallingStars::randomize() {

    for(uint8_t i=0; i<NUM_STARS; i++) {
        stars[i].x = rand() % width - 1;
        stars[i].y = rand() % height - 1;
        stars[i].v = rand() % 5 + 1;
        stars[i].color = rand()% CRGB(255, 255, 0);
    }

}
