#include <Arduino.h>
#include "MatrixModeRGBDemo.h"

CMatrixModeRGBDemo::CMatrixModeRGBDemo(uint8_t width, uint8_t height)
: CBaseMatrixMode(width, height) {
}

void CMatrixModeRGBDemo::draw(CRGB *leds) {

    if (millis() - tMillis > 10) {
        tMillis = millis();
                
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
                leds[ XY(x, y) ]  = CHSV( pixelHue, 255, configuration.ledBrightnessTime * 255);
            }
        }
    }
    
}

// Test LEDs remnant
        /*
        static uint8_t r, g, b = 0;
        //memset(leds, 10, 3*width*height);
        for (uint16_t i=0; i<LED_EXTERNAL_STRIP_SIZE; i++) {
            //leds[i] = CRGB(r, g, b);
            leds[i] = CRGB(10, 0, 0);
        }
        
        r++;
        if (r>=255) {
            r=0;
            g++;
            if (g>=255) {
                g=0;
                b++;
                if (b>=255) {
                    b=0;
                }
            }
        }
        */
        

