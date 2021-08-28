#include <Arduino.h>
#include "BaseMatrixMode.h"

#ifdef LED_EXTERNAL_MATRIX

CBaseMatrixMode::CBaseMatrixMode(uint8_t width, uint8_t height):
width(width), height(height) {
    tMillis = millis();
}


uint16_t CBaseMatrixMode::XY( uint8_t x, uint8_t y) {
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

uint16_t CBaseMatrixMode::XYsafe( uint8_t x, uint8_t y) {
  if( x >= width) return -1;
  if( y >= height) return -1;
  return XY(x,y);
}

#endif