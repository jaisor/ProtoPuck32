#include <Arduino.h>
#include "MatrixModeSoundLevel.h"

#ifdef LED_EXTERNAL_MATRIX
#ifdef MIC_I2S

#include <Adafruit_GFX.h>
#include <Fonts/TomThumb.h>
#include <Fonts/Picopixel.h>
#include <driver/i2s.h>
#include "sos-iir-filter.h"

#define LEQ_PERIOD        1           // second(s)
#define WEIGHTING         C_weighting // Also avaliable: 'C_weighting' or 'None' (Z_weighting)
#define LEQ_UNITS         "LAeq"      // customize based on above weighting used
#define DB_UNITS          "dBA"       // customize based on above weighting used
#define USE_DISPLAY       1

// NOTE: Some microphones require at least DC-Blocker filter
#define MIC_EQUALIZER     ICS43434    // See below for defined IIR filters or set to 'None' to disable
#define MIC_OFFSET_DB     3.0103      // Default offset (sine-wave RMS vs. dBFS). Modify this value for linear calibration

// Customize these values from microphone datasheet
#define MIC_SENSITIVITY   -26         // dBFS value expected at MIC_REF_DB (Sensitivity value from datasheet)
#define MIC_REF_DB        94.0        // Value at which point sensitivity is specified in datasheet (dB)
#define MIC_OVERLOAD_DB   116.0       // dB - Acoustic overload point
#define MIC_NOISE_DB      29          // dB - Noise floor
#define MIC_BITS          24          // valid number of bits in I2S data
#define MIC_CONVERT(s)    (s >> (SAMPLE_BITS - MIC_BITS))
#define MIC_TIMING_SHIFT  0           // Set to one to fix MSB timing for some microphones, i.e. SPH0645LM4H-x

// Calculate reference amplitude value at compile time
constexpr double MIC_REF_AMPL = pow(10, double(MIC_SENSITIVITY)/20) * ((1<<(MIC_BITS-1))-1);

// TODO: Move to Configuration.h
#define I2S_WS            18 
#define I2S_SCK           23 
#define I2S_SD            19
// I2S peripheral to use (0 or 1)
#define I2S_PORT          I2S_NUM

DEFINE_GRADIENT_PALETTE( GreenYellowRed_gp ) {
    0, 0x00, 0xff, 0x00,
  120, 0xff, 0xff, 0x00,
  240, 0xff, 0x00, 0x00,
  255, 0xff, 0x00, 0x00
};

CRGBPalette16 GreenYellowRed_p = GreenYellowRed_gp;

static uint8_t dba = 0;

CMatrixModeSoundLevel::CMatrixModeSoundLevel(uint8_t width, uint8_t height)
: CBaseCanvasedMatrixMode(width, height) {
    /*
    log_d("Position (%0.1f, %0.1f)", xf, yf);
    log_d("Velocity (%0.1f, %0.1f)", vxf, vyf);
    */

    canvas->setTextColor(0xffff);
    canvas->setTextSize(1);
}

void CMatrixModeSoundLevel::draw(CRGB *leds) {

    if (millis() - tMillis > 50) {
        tMillis = millis();

        memset(leds, 0, width * height * sizeof(CRGB));
        canvas->fillScreen(0);
        
        /*
        for (uint8_t y=0; y<16; y++) {
            for (uint8_t x=0; x<16; x++) {
                leds[ XYsafe(x, y) ]  = ColorFromPalette(GreenYellowRed_p, x + y * 15, 255 * configuration.ledBrightness, NOBLEND);
            }
        }
        */

        for (uint8_t i=0; i<22; i++) {
            
        }


        canvas->setCursor(3, 19);
        canvas->setTextColor(CRGB_to_RGB565(ColorFromPalette(GreenYellowRed_p, dba * 2, 255 * configuration.ledBrightness, NOBLEND)));
        canvas->setFont(&TomThumb);
        canvas->printf("%3i", dba);

        canvas->setTextColor(CRGB_to_RGB565(CRGB(255 * configuration.ledBrightness, 255 * configuration.ledBrightness, 255 * configuration.ledBrightness)));
        canvas->setCursor(14, 18);
        canvas->setFont(&Picopixel);
        canvas->printf("dB");

        dba += 1;
        if (dba >= 127) {
            dba = 0;
        }

        drawCanvas(leds);
    }
    
}

#ifdef KEYPAD
void CMatrixModeSoundLevel::keyEvent(key_status_t key) {
    switch (key) {
        default: ;
    }
}
#endif

#endif
#endif