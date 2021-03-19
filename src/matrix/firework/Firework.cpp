#include <Arduino.h>
#include "Firework.h"

CFirework::CFirework() {
    
    y = 22;
    x = rand()%22;
    yxpl = rand()%21;
    yvel = rand()%100;
    xplvel = rand()%10;

    color = CRGB(rand()%155 + 100, rand()%155 + 100, rand()%155 + 100);

    state = 0;
}

