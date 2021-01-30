#include <Arduino.h>
#include "MatrixLEDManager.h"

static float xf = (rand() % 1000) / 1000.0f;
static float yf = (rand() % 1000) / 1000.0f;

static float vxf = 0;
static float vyf = 0;

CMatrixLEDManager::CMatrixLEDManager(CRGB *leds, uint16_t size, uint8_t width, uint8_t height, float brightness)
: CLEDManager(size, brightness) {

    log_d("Position (%0.1f, %0.1f)", xf, yf);
    log_d("Velocity (%0.1f, %0.1f)", vxf, vyf);

    this->leds = leds;
    this->width = width;
    this->height = height;

    tMillis = tMillsChangePalette = millis();
    changePalette = 0;

    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;

    palettes.push_back(
        CRGBPalette16(
            green,  green,  black,  black,
            purple, purple, black,  black,
            green,  green,  black,  black,
            purple, purple, black,  black )
    );

    palettes.push_back(RainbowColors_p);
    palettes.push_back(RainbowStripeColors_p);
    palettes.push_back(RainbowStripeColors_p);
    palettes.push_back(CloudColors_p);
    palettes.push_back(PartyColors_p);
    palettes.push_back(LavaColors_p);
    palettes.push_back(OceanColors_p);
    palettes.push_back(ForestColors_p);
    palettes.push_back(HeatColors_p);

    currentPaletteIndex = 0;
}

void CMatrixLEDManager::loop() {

    if (millis() - tMillis > 10) {
        tMillis = millis();
        //startIndex = startIndex + 1;

        FastLED.clear();
        
        int32_t yHueDelta32 = ((int32_t)cos16( tMillis * (27/1) ) * (350 / width));
        int32_t xHueDelta32 = ((int32_t)cos16( tMillis * (39/1) ) * (310 / height));

        byte startHue8 = tMillis / 65536;
        int8_t yHueDelta8 = yHueDelta32 / 32768;
        int8_t xHueDelta8 = xHueDelta32 / 32768;

        byte lineStartHue = startHue8;
        for( byte y = 0; y < height; y++) {
            lineStartHue += yHueDelta8;
            byte pixelHue = lineStartHue;      
            for( byte x = 0; x < width; x++) {
                pixelHue += xHueDelta8;
                leds[ XY(x, y) ]  = CHSV( pixelHue, 255, configuration.ledBrightness * 255);
            }
        }
        

        if (vxf == 0 ) { vxf = (rand() % 1000 - 500) / 100000.0f; }
        if (vyf == 0 ) { vyf = (rand() % 1000 - 500) / 100000.0f; }

        
        
        xf += vxf;
        yf += vyf;

        if (xf<0) { xf = 0; vxf *= -1 + (rand() % 1000 - 500) / 10000000.0f; }
        if (xf>1) { xf = 1; vxf *= -1 + (rand() % 1000 - 500) / 10000000.0f; }
        if (yf<0) { yf = 0; vyf *= -1 + (rand() % 1000 - 500) / 10000000.0f; }
        if (yf>1) { yf = 1; vyf *= -1 + (rand() % 1000 - 500) / 10000000.0f; }

        uint8_t xi = xf * width;
        uint8_t yi = yf * height;
        uint8_t b = brightness * 255;
        //leds[ XYsafe(xi, yi) ]  = CHSV( 255, 0, b);
        leds[ XYsafe(xi-1, yi) ]  = CHSV( 255, 0, b);
        leds[ XYsafe(xi+1, yi) ]  = CHSV( 255, 0, b);
        leds[ XYsafe(xi, yi-1) ]  = CHSV( 255, 0, b);
        leds[ XYsafe(xi, yi+1) ]  = CHSV( 255, 0, b);
        leds[ XYsafe(xi-1, yi-1) ]  = CHSV( 255, 0, b);
        leds[ XYsafe(xi+1, yi-1) ]  = CHSV( 255, 0, b);
        leds[ XYsafe(xi-1, yi+1) ]  = CHSV( 255, 0, b);
        leds[ XYsafe(xi+1, yi+1) ]  = CHSV( 255, 0, b);
    }
    
}

#ifdef KEYPAD
void CMatrixLEDManager::keyEvent(key_status_t key) {
    switch (key) {
        case KEY_LEFT: vxf -= 0.0000001; break;
        case KEY_RIGHT: vxf += 0.0000001; break;
        case KEY_UP: vyf -= 0.0000001; break;
        case KEY_DOWN: vyf += 0.0000001; break;
        case KEY_MIDDLE: vyf = vxf = 0; break;
        default: ;
    }
}
#endif

uint16_t CMatrixLEDManager::XY( uint8_t x, uint8_t y) {
  uint16_t i;
  
  if( LED_EXTERNAL_MATRIX_SERPENTINE == false) {
    if (LED_EXTERNAL_MATRIX_VERTICAL == false) {
      i = (y * width) + x;
    } else {
      i = height * (width - (x+1))+y;
    }
  }

  if( LED_EXTERNAL_MATRIX_SERPENTINE == true) {
    if (LED_EXTERNAL_MATRIX_VERTICAL == false) {
      if( y & 0x01) {
        // Odd rows run backwards
        uint8_t reverseX = (width - 1) - x;
        i = (y * width) + reverseX;
      } else {
        // Even rows run forwards
        i = (y * width) + x;
      }
    } else { // vertical positioning
      if ( x & 0x01) {
        i = height * (width - (x+1))+y;
      } else {
        i = height * (width - x) - (y+1);
      }
    }
  }
  
  return i;
}

uint16_t CMatrixLEDManager::XYsafe( uint8_t x, uint8_t y) {
  if( x >= width) return -1;
  if( y >= height) return -1;
  return XY(x,y);
}

#ifdef OLED
uint16_t CMatrixLEDManager::OLED_Status(Adafruit_GFX *oled) {
    /*
    oled->setTextSize(1);
    oled->setCursor(00, 16);
    char buf[50];
    sprintf(buf, "%0.2f:%0.2f", xf, yf);
    oled->print(buf);
    */
    return 0;
}
#endif