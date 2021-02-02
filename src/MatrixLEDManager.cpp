#include <Arduino.h>
#include "MatrixLEDManager.h"
#include "matrix/MatrixModeRGBDemo.h"
#include "matrix/MatrixModeMovingSquare.h"
#include "matrix/MatrixModePicture.h"

CMatrixLEDManager::CMatrixLEDManager(CRGB *leds, uint8_t width, uint8_t height, float brightness)
: CLEDManager(width * height, brightness) {

    this->leds = leds;
    this->width = width;
    this->height = height;

    tMillis = tMillsChangeMode = millis();

    modes.push_back(new CMatrixModePicture(width, height));
    modes.push_back(new CMatrixModeRGBDemo(width, height));
    modes.push_back(new CMatrixModeMovingSquare(width, height));

    currentModeIndex = 0;
}

void CMatrixLEDManager::loop() {
    if (changeMode && millis() - tMillsChangeMode > 1000) {
        tMillsChangeMode = millis();
        currentModeIndex += changeMode;
        if (currentModeIndex >= modes.size()) {
            currentModeIndex = 0;
        }
        if (currentModeIndex < 0 ) {
            currentModeIndex = modes.size() - 1;
        }
        changeMode = 0;
        log_d("Change mode %i changed index to %i", changeMode, currentModeIndex);
    }

    modes[currentModeIndex]->draw(leds);
}

#ifdef KEYPAD
void CMatrixLEDManager::keyEvent(key_status_t key) {
    switch (key) {
        case KEY_LEFT: changeMode = 1; break;
        case KEY_RIGHT: changeMode = -1; break;
        default: ;
    }

    modes[currentModeIndex]->keyEvent(key);
}
#endif

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