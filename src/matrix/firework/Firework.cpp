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

    yvel = 40 + rand()%90;
    xplvel = rand()%10;

    //color = CRGB(rand()%155 + 100, rand()%155 + 100, rand()%155 + 100);
    color = ColorFromPalette( PartyColors_p, rand()%255, 255, LINEARBLEND);

    state = 0;
}

void CFirework::draw(CRGB *leds) {

    CRGB c = CRGB(
        color.r * configuration.ledBrightness, 
        color.g * configuration.ledBrightness, 
        color.b * configuration.ledBrightness
    );

    if (state == 0) {
        
        // Shooting
        if (millis() - tMillis > 10) {
            tMillis = millis();
            y = y - yvel / 200.0;
            if (y <= yxpl) {
                state = 1;
            }
        }
        leds[XYsafe(x, y)] = c;

    } else if (state == 1) {
        // Exploding

        float t = sqrtf((float)(millis() - tMillis) / 1000.0);
        float d = t * (float)xplvel * 1.2;
        float b = 1.0 - t;
        CRGB cb = CRGB(c.r * b, c.g * b, c.b * b);

        leds[XYsafe(x, y-d)] = cb;
        leds[XYsafe(x, y+d)] = cb;
        leds[XYsafe(x-d, y)] = cb;
        leds[XYsafe(x+d, y)] = cb;
        leds[XYsafe(x-d, y-d)] = cb;
        leds[XYsafe(x-d, y+d)] = cb;
        leds[XYsafe(x-d, y+d)] = cb;
        leds[XYsafe(x+d, y+d)] = cb;

        if (millis() - tMillis > 1000) {
            reset();
        }
    }



    

}
