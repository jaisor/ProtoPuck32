#include <Arduino.h>
#include "Firework.h"

CFirework::CFirework(const uint8_t width, const uint8_t height)
: CBaseMatrixMode(width, height) {
    reset();
}

void CFirework::reset() {

    tMillis = millis();

    y = height;
    x = rand()%width;
    yxpl = rand()%(height-10);

    yvel = 10 + rand()%90;
    xplvel = rand()%10;

    color = CRGB(rand()%155 + 100, rand()%155 + 100, rand()%155 + 100);

    state = 0;
}

void CFirework::draw(CRGB *leds) {

    if (state == 0) {
        
        // Shooting
        if (millis() - tMillis > 10) {
            tMillis = millis();
            y = y - yvel / 200.0;
            if (y <= yxpl) {
                state = 1;
            }
        }
        leds[XYsafe(x, y)] = color;

    } else if (state == 1) {
        // Exploding
        reset();
    }



    

}
