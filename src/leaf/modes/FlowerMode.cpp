#include "FlowerMode.h"

#ifdef LED_EXTERNAL_LEAFS

const TProgmemRGBPalette16 FlowerStem_p FL_PROGMEM = {
    CRGB::DarkGreen,
    CRGB::DarkGreen,
    CRGB::DarkOliveGreen,
    CRGB::DarkGreen,

    CRGB::Green,
    CRGB::ForestGreen,
    CRGB::OliveDrab,
    CRGB::Green,

    CRGB::SeaGreen,
    CRGB::MediumAquamarine,
    CRGB::LimeGreen,
    CRGB::YellowGreen,

    CRGB::LightGreen,
    CRGB::LawnGreen,
    CRGB::MediumAquamarine,
    CRGB::ForestGreen
};

const TProgmemRGBPalette16 FlowerLeaf_p FL_PROGMEM = {
    CRGB::MidnightBlue,
    CRGB::DarkBlue,
    CRGB::MidnightBlue,
    CRGB::Navy,

    CRGB::DarkBlue,
    CRGB::MediumBlue,
    CRGB::SeaGreen,
    CRGB::Teal,

    CRGB::CadetBlue,
    CRGB::Blue,
    CRGB::DarkCyan,
    CRGB::CornflowerBlue,

    CRGB::Aquamarine,
    CRGB::SeaGreen,
    CRGB::Aqua,
    CRGB::LightSkyBlue
};

const TProgmemRGBPalette16 FlowerPetals_p FL_PROGMEM = {
    0x5500AB, 0x84007C, 0xB5004B, 0xE5001B,
    0xE81700, 0xB84700, 0xAB7700, 0xABAB00,
    0xAB5500, 0xDD2200, 0xF2000E, 0xC2003E,
    0x8F0071, 0x5F00A1, 0x2F00D0, 0x0007F9
};

const uint16_t nodes_Stem[][2] = {
    { 0, 210 },
    { 280, 420 }
};

const uint16_t nodes_Leaf[][2] = {
    { 210, 280 }
};

const uint16_t nodes_Petals[][2] = {
    { 420, 700 }
};

CFlowerMode::CFlowerMode(const uint16_t numLeds, const uint16_t nodeLength)
: CBaseMode(numLeds), nodeLength(nodeLength), delay(15) {
}


void CFlowerMode::loop() {
    if (millis() - tMillis > delay) {
        tMillis = millis();
        startIndex = startIndex + 1;
    }
}

void CFlowerMode::draw(CRGB *leds) {
    memset(leds, 0, numLeds * sizeof(CRGB));
    
    drawNode(leds, nodes_Leaf, FlowerLeaf_p);
    //drawNode(leds, nodes_Stem, FlowerStem_p);
    //drawNode(leds, nodes_Petals, FlowerPetals_p);
}

void CFlowerMode::drawNode(CRGB *leds, const uint16_t (*n)[2], const TProgmemRGBPalette16& palette) {
    uint8_t ci = startIndex;
    //log_d("Node size %i , div %i", sizeof(n), sizeof(n) / (sizeof(uint16_t)*2));
    for (uint16_t l = 0; l < sizeof(n) / (sizeof(uint16_t)*2); l++) {
        //log_d("Node %i : %i - %i", l, n[l][0], n[l][1]);
        for( int i = n[l][0]; i < n[l][1]; i++) {
            leds[i] = ColorFromPalette( palette, ci, 255 * configuration.ledBrightness, LINEARBLEND);
            ci++;
        }
    }
}

#endif