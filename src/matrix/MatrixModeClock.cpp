#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Fonts/Picopixel.h>
#include "MatrixModeClock.h"

#define BITMAP_BYTES_PER_PIXEL 4
#define HAND_COLOR_MIN 0x5fff
#define HAND_COLOR_HOUR 0xfd00

const uint8_t BITMAP_CLOCK_FACE[] PROGMEM = {
    0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x10, 0x03, 0x0a, 0xff, 0x61, 0x11, 0x40, 0xff, 0xa4, 0x1d, 0x6b, 0xff, 0xce, 0x24, 0x87, 0xff, 0xe4, 0x28, 0x95, 0xff, 0xdf, 0x27, 0x92, 0xff, 0xc5, 0x23, 0x81, 0xff, 0xa4, 0x1d, 0x6b, 0xff, 0x61, 0x11, 0x40, 0xff, 0x10, 0x03, 0x0a, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 
    0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x07, 0x01, 0x05, 0xff, 0x6f, 0x13, 0x49, 0xff, 0xde, 0x34, 0x8d, 0xff, 0xec, 0x38, 0x96, 0xff, 0xea, 0x29, 0x99, 0xff, 0xea, 0x29, 0x99, 0xff, 0xfe, 0xdf, 0x77, 0xff, 0xfe, 0xdf, 0x77, 0xff, 0xeb, 0x27, 0x96, 0xff, 0xea, 0x29, 0x99, 0xff, 0xea, 0x29, 0x99, 0xff, 0xde, 0x34, 0x8d, 0xff, 0x70, 0x14, 0x49, 0xff, 0x08, 0x01, 0x05, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 
    0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x1d, 0x05, 0x13, 0xff, 0xc2, 0x22, 0x7f, 0xff, 0xf0, 0x5f, 0x8f, 0xff, 0xfe, 0xde, 0x77, 0xff, 0xec, 0x37, 0x96, 0xff, 0xea, 0x29, 0x99, 0xff, 0xea, 0x29, 0x99, 0xff, 0xea, 0x28, 0x98, 0xff, 0xeb, 0x27, 0x94, 0xff, 0xec, 0x25, 0x91, 0xff, 0xeb, 0x28, 0x96, 0xff, 0xea, 0x29, 0x99, 0xff, 0xfe, 0xde, 0x77, 0xff, 0xf2, 0x70, 0x8c, 0xff, 0xc2, 0x22, 0x7f, 0xff, 0x1d, 0x05, 0x13, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 
    0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x1a, 0x05, 0x11, 0xff, 0xd0, 0x24, 0x88, 0xff, 0xea, 0x29, 0x99, 0xff, 0xf3, 0x7c, 0x8a, 0xff, 0xee, 0x49, 0x93, 0xff, 0xea, 0x29, 0x99, 0xff, 0xea, 0x29, 0x99, 0xff, 0xea, 0x28, 0x97, 0xff, 0xec, 0x26, 0x93, 0xff, 0xed, 0x24, 0x8e, 0xff, 0xed, 0x22, 0x8c, 0xff, 0xec, 0x26, 0x92, 0xff, 0xea, 0x28, 0x97, 0xff, 0xee, 0x49, 0x93, 0xff, 0xf6, 0x96, 0x85, 0xff, 0xea, 0x29, 0x99, 0xff, 0xd0, 0x24, 0x88, 0xff, 0x1a, 0x05, 0x11, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 
    0x00, 0x00, 0x00, 0xff, 0x07, 0x01, 0x05, 0xff, 0xbc, 0x27, 0x79, 0xff, 0xea, 0x29, 0x99, 0xff, 0xea, 0x29, 0x99, 0xff, 0xea, 0x29, 0x99, 0xff, 0xea, 0x29, 0x99, 0xff, 0xea, 0x29, 0x99, 0xff, 0xeb, 0x28, 0x96, 0xff, 0xec, 0x26, 0x92, 0xff, 0xed, 0x23, 0x8d, 0xff, 0xef, 0x20, 0x87, 0xff, 0xef, 0x1f, 0x86, 0xff, 0xed, 0x23, 0x8d, 0xff, 0xec, 0x26, 0x93, 0xff, 0xea, 0x28, 0x98, 0xff, 0xea, 0x29, 0x99, 0xff, 0xea, 0x29, 0x99, 0xff, 0xea, 0x29, 0x99, 0xff, 0xbe, 0x2f, 0x79, 0xff, 0x08, 0x01, 0x05, 0xff, 0x00, 0x00, 0x00, 0xff, 
    0x00, 0x00, 0x00, 0xff, 0x6e, 0x13, 0x48, 0xff, 0xf2, 0x6e, 0x8c, 0xff, 0xf2, 0x76, 0x8b, 0xff, 0xea, 0x29, 0x99, 0xff, 0xea, 0x29, 0x99, 0xff, 0xea, 0x29, 0x99, 0xff, 0xeb, 0x27, 0x96, 0xff, 0xec, 0x25, 0x91, 0xff, 0xed, 0x22, 0x8c, 0xff, 0xef, 0x1f, 0x86, 0xff, 0xf1, 0x1c, 0x80, 0xff, 0xf1, 0x1c, 0x80, 0xff, 0xef, 0x20, 0x87, 0xff, 0xed, 0x24, 0x8e, 0xff, 0xeb, 0x27, 0x94, 0xff, 0xea, 0x29, 0x98, 0xff, 0xea, 0x29, 0x99, 0xff, 0xef, 0x5b, 0x90, 0xff, 0xf6, 0x95, 0x85, 0xff, 0x6f, 0x13, 0x49, 0xff, 0x00, 0x00, 0x00, 0xff, 
    0x11, 0x03, 0x0b, 0xff, 0xdc, 0x27, 0x90, 0xff, 0xfe, 0xdd, 0x77, 0xff, 0xf3, 0x7a, 0x8a, 0xff, 0xea, 0x29, 0x99, 0xff, 0xea, 0x29, 0x98, 0xff, 0xeb, 0x27, 0x95, 0xff, 0xec, 0x25, 0x90, 0xff, 0xee, 0x22, 0x8b, 0xff, 0xef, 0x1f, 0x85, 0xff, 0xf1, 0x1c, 0x7e, 0xff, 0xf3, 0x18, 0x78, 0xff, 0xf2, 0x19, 0x79, 0xff, 0xf0, 0x1d, 0x81, 0xff, 0xee, 0x21, 0x89, 0xff, 0xed, 0x24, 0x8f, 0xff, 0xeb, 0x27, 0x95, 0xff, 0xea, 0x29, 0x99, 0xff, 0xf0, 0x5f, 0x8f, 0xff, 0xfe, 0xdf, 0x77, 0xff, 0xdc, 0x27, 0x90, 0xff, 0x11, 0x03, 0x0b, 0xff, 
    0x62, 0x11, 0x40, 0xff, 0xea, 0x29, 0x99, 0xff, 0xee, 0x4f, 0x92, 0xff, 0xea, 0x29, 0x99, 0xff, 0xea, 0x28, 0x98, 0xff, 0xeb, 0x27, 0x94, 0xff, 0xed, 0x24, 0x8f, 0xff, 0xee, 0x21, 0x8a, 0xff, 0xf0, 0x1e, 0x84, 0xff, 0xf1, 0x1b, 0x7d, 0xff, 0xf3, 0x17, 0x76, 0xff, 0xf5, 0x14, 0x70, 0xff, 0xf4, 0x15, 0x73, 0xff, 0xf2, 0x1a, 0x7a, 0xff, 0xf0, 0x1e, 0x82, 0xff, 0xee, 0x21, 0x8a, 0xff, 0xec, 0x25, 0x90, 0xff, 0xeb, 0x27, 0x96, 0xff, 0xea, 0x29, 0x99, 0xff, 0xee, 0x4e, 0x92, 0xff, 0xea, 0x29, 0x99, 0xff, 0x62, 0x11, 0x40, 0xff, 
    0xa3, 0x1d, 0x6b, 0xff, 0xea, 0x29, 0x99, 0xff, 0xea, 0x29, 0x99, 0xff, 0xea, 0x28, 0x97, 0xff, 0xec, 0x26, 0x93, 0xff, 0xed, 0x24, 0x8e, 0xff, 0xee, 0x21, 0x89, 0xff, 0xf0, 0x1e, 0x82, 0xff, 0xf2, 0x1a, 0x7c, 0xff, 0xf3, 0x17, 0x75, 0xff, 0xf5, 0x13, 0x6f, 0xff, 0xf7, 0x10, 0x68, 0xff, 0xf6, 0x12, 0x6c, 0xff, 0xf4, 0x16, 0x74, 0xff, 0xf2, 0x1a, 0x7c, 0xff, 0xf0, 0x1e, 0x84, 0xff, 0xee, 0x22, 0x8b, 0xff, 0xec, 0x25, 0x91, 0xff, 0xeb, 0x28, 0x96, 0xff, 0xea, 0x29, 0x99, 0xff, 0xea, 0x29, 0x99, 0xff, 0xa4, 0x1d, 0x6b, 0xff, 
    0xcf, 0x24, 0x88, 0xff, 0xea, 0x29, 0x99, 0xff, 0xeb, 0x28, 0x96, 0xff, 0xec, 0x26, 0x92, 0xff, 0xed, 0x23, 0x8d, 0xff, 0xef, 0x20, 0x87, 0xff, 0xf0, 0x1d, 0x81, 0xff, 0xf2, 0x1a, 0x7a, 0xff, 0xf4, 0x16, 0x74, 0xff, 0xf5, 0x13, 0x6d, 0xff, 0xf7, 0x10, 0x67, 0xff, 0xf8, 0x0d, 0x62, 0xff, 0xf7, 0x0f, 0x66, 0xff, 0xf5, 0x13, 0x6d, 0xff, 0xf3, 0x17, 0x75, 0xff, 0xf1, 0x1b, 0x7d, 0xff, 0xef, 0x1f, 0x85, 0xff, 0xed, 0x22, 0x8c, 0xff, 0xec, 0x26, 0x92, 0xff, 0xea, 0x28, 0x97, 0xff, 0xea, 0x29, 0x99, 0xff, 0xcf, 0x24, 0x88, 0xff, 
    0xe4, 0x28, 0x95, 0xff, 0xfe, 0xdf, 0x77, 0xff, 0xec, 0x25, 0x91, 0xff, 0xed, 0x22, 0x8c, 0xff, 0xef, 0x1f, 0x86, 0xff, 0xf1, 0x1c, 0x80, 0xff, 0xf2, 0x19, 0x79, 0xff, 0xf4, 0x15, 0x73, 0xff, 0xf6, 0x12, 0x6c, 0xff, 0xf7, 0x0f, 0x66, 0xff, 0xf9, 0x0c, 0x61, 0xff, 0xfa, 0x0a, 0x5c, 0xff, 0xf9, 0x0c, 0x61, 0xff, 0xf7, 0x10, 0x67, 0xff, 0xf5, 0x13, 0x6f, 0xff, 0xf3, 0x17, 0x76, 0xff, 0xf1, 0x1c, 0x7e, 0xff, 0xef, 0x1f, 0x86, 0xff, 0xed, 0x23, 0x8d, 0xff, 0xec, 0x26, 0x93, 0xff, 0xfe, 0xdf, 0x77, 0xff, 0xe4, 0x28, 0x95, 0xff, 
    0xde, 0x27, 0x91, 0xff, 0xfe, 0xdf, 0x77, 0xff, 0xeb, 0x27, 0x94, 0xff, 0xed, 0x24, 0x8e, 0xff, 0xef, 0x20, 0x87, 0xff, 0xf1, 0x1c, 0x80, 0xff, 0xf3, 0x18, 0x78, 0xff, 0xf5, 0x14, 0x70, 0xff, 0xf7, 0x10, 0x68, 0xff, 0xf8, 0x0d, 0x62, 0xff, 0xfa, 0x0a, 0x5c, 0xff, 0xfb, 0x08, 0x58, 0xff, 0xfa, 0x0a, 0x5c, 0xff, 0xf8, 0x0d, 0x62, 0xff, 0xf7, 0x10, 0x68, 0xff, 0xf5, 0x14, 0x70, 0xff, 0xf3, 0x18, 0x78, 0xff, 0xf1, 0x1c, 0x80, 0xff, 0xef, 0x20, 0x87, 0xff, 0xed, 0x24, 0x8e, 0xff, 0xfe, 0xdf, 0x77, 0xff, 0xe4, 0x28, 0x94, 0xff, 
    0xc7, 0x23, 0x82, 0xff, 0xea, 0x29, 0x99, 0xff, 0xea, 0x28, 0x98, 0xff, 0xec, 0x26, 0x93, 0xff, 0xed, 0x23, 0x8d, 0xff, 0xef, 0x1f, 0x86, 0xff, 0xf1, 0x1c, 0x7e, 0xff, 0xf3, 0x17, 0x76, 0xff, 0xf5, 0x13, 0x6f, 0xff, 0xf7, 0x10, 0x67, 0xff, 0xf9, 0x0c, 0x61, 0xff, 0xfa, 0x0a, 0x5c, 0xff, 0xf9, 0x0c, 0x61, 0xff, 0xf7, 0x0f, 0x66, 0xff, 0xf6, 0x12, 0x6c, 0xff, 0xf4, 0x15, 0x73, 0xff, 0xf2, 0x19, 0x79, 0xff, 0xf1, 0x1c, 0x80, 0xff, 0xef, 0x1f, 0x86, 0xff, 0xed, 0x22, 0x8c, 0xff, 0xec, 0x25, 0x91, 0xff, 0xd0, 0x23, 0x85, 0xff, 
    0xa3, 0x1d, 0x6b, 0xff, 0xea, 0x29, 0x99, 0xff, 0xea, 0x29, 0x99, 0xff, 0xea, 0x28, 0x97, 0xff, 0xec, 0x26, 0x92, 0xff, 0xed, 0x22, 0x8c, 0xff, 0xef, 0x1f, 0x85, 0xff, 0xf1, 0x1b, 0x7d, 0xff, 0xf3, 0x17, 0x75, 0xff, 0xf5, 0x13, 0x6d, 0xff, 0xf7, 0x0f, 0x66, 0xff, 0xf8, 0x0d, 0x62, 0xff, 0xf7, 0x10, 0x67, 0xff, 0xf5, 0x13, 0x6d, 0xff, 0xf4, 0x16, 0x74, 0xff, 0xf2, 0x1a, 0x7a, 0xff, 0xf0, 0x1d, 0x81, 0xff, 0xef, 0x20, 0x87, 0xff, 0xed, 0x23, 0x8d, 0xff, 0xec, 0x26, 0x92, 0xff, 0xeb, 0x28, 0x96, 0xff, 0xa4, 0x1d, 0x6b, 0xff, 
    0x62, 0x11, 0x40, 0xff, 0xea, 0x29, 0x99, 0xff, 0xf1, 0x6a, 0x8d, 0xff, 0xea, 0x29, 0x99, 0xff, 0xeb, 0x28, 0x96, 0xff, 0xec, 0x25, 0x91, 0xff, 0xee, 0x22, 0x8b, 0xff, 0xf0, 0x1e, 0x84, 0xff, 0xf2, 0x1a, 0x7c, 0xff, 0xf4, 0x16, 0x74, 0xff, 0xf6, 0x12, 0x6c, 0xff, 0xf7, 0x10, 0x68, 0xff, 0xf5, 0x13, 0x6f, 0xff, 0xf3, 0x17, 0x75, 0xff, 0xf2, 0x1a, 0x7c, 0xff, 0xf0, 0x1e, 0x82, 0xff, 0xee, 0x21, 0x89, 0xff, 0xed, 0x24, 0x8e, 0xff, 0xec, 0x26, 0x93, 0xff, 0xf1, 0x68, 0x8c, 0xff, 0xea, 0x29, 0x99, 0xff, 0x62, 0x11, 0x40, 0xff, 
    0x11, 0x03, 0x0b, 0xff, 0xdc, 0x27, 0x90, 0xff, 0xfd, 0xd9, 0x78, 0xff, 0xf5, 0x8a, 0x87, 0xff, 0xea, 0x29, 0x99, 0xff, 0xeb, 0x27, 0x96, 0xff, 0xec, 0x25, 0x90, 0xff, 0xee, 0x21, 0x8a, 0xff, 0xf0, 0x1e, 0x82, 0xff, 0xf2, 0x1a, 0x7a, 0xff, 0xf4, 0x15, 0x73, 0xff, 0xf5, 0x14, 0x70, 0xff, 0xf3, 0x17, 0x76, 0xff, 0xf1, 0x1b, 0x7d, 0xff, 0xf0, 0x1e, 0x84, 0xff, 0xee, 0x21, 0x8a, 0xff, 0xed, 0x24, 0x8f, 0xff, 0xeb, 0x27, 0x94, 0xff, 0xf1, 0x68, 0x8c, 0xff, 0xfe, 0xdf, 0x77, 0xff, 0xdc, 0x27, 0x90, 0xff, 0x11, 0x03, 0x0b, 0xff, 
    0x00, 0x00, 0x00, 0xff, 0x6f, 0x13, 0x49, 0xff, 0xf0, 0x5d, 0x8f, 0xff, 0xf0, 0x61, 0x8e, 0xff, 0xea, 0x29, 0x99, 0xff, 0xea, 0x29, 0x99, 0xff, 0xeb, 0x27, 0x95, 0xff, 0xed, 0x24, 0x8f, 0xff, 0xee, 0x21, 0x89, 0xff, 0xf0, 0x1d, 0x81, 0xff, 0xf2, 0x19, 0x79, 0xff, 0xf3, 0x18, 0x78, 0xff, 0xf1, 0x1c, 0x7e, 0xff, 0xef, 0x1f, 0x85, 0xff, 0xee, 0x22, 0x8b, 0xff, 0xec, 0x25, 0x90, 0xff, 0xeb, 0x27, 0x95, 0xff, 0xea, 0x29, 0x98, 0xff, 0xee, 0x4e, 0x92, 0xff, 0xf4, 0x85, 0x88, 0xff, 0x6f, 0x13, 0x49, 0xff, 0x00, 0x00, 0x00, 0xff, 
    0x00, 0x00, 0x00, 0xff, 0x08, 0x01, 0x05, 0xff, 0xc2, 0x24, 0x7e, 0xff, 0xea, 0x29, 0x99, 0xff, 0xea, 0x29, 0x99, 0xff, 0xea, 0x29, 0x99, 0xff, 0xea, 0x29, 0x98, 0xff, 0xeb, 0x27, 0x94, 0xff, 0xed, 0x24, 0x8e, 0xff, 0xef, 0x20, 0x87, 0xff, 0xf1, 0x1c, 0x80, 0xff, 0xf1, 0x1c, 0x80, 0xff, 0xef, 0x1f, 0x86, 0xff, 0xed, 0x22, 0x8c, 0xff, 0xec, 0x25, 0x91, 0xff, 0xeb, 0x27, 0x96, 0xff, 0xea, 0x29, 0x99, 0xff, 0xea, 0x29, 0x99, 0xff, 0xea, 0x29, 0x99, 0xff, 0xc3, 0x26, 0x7f, 0xff, 0x08, 0x01, 0x05, 0xff, 0x00, 0x00, 0x00, 0xff, 
    0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x21, 0x06, 0x16, 0xff, 0xd2, 0x25, 0x89, 0xff, 0xea, 0x29, 0x99, 0xff, 0xf4, 0x88, 0x87, 0xff, 0xf1, 0x66, 0x8e, 0xff, 0xea, 0x28, 0x98, 0xff, 0xec, 0x26, 0x93, 0xff, 0xed, 0x23, 0x8d, 0xff, 0xef, 0x1f, 0x86, 0xff, 0xef, 0x20, 0x87, 0xff, 0xed, 0x23, 0x8d, 0xff, 0xec, 0x26, 0x92, 0xff, 0xeb, 0x28, 0x96, 0xff, 0xf1, 0x66, 0x8e, 0xff, 0xf8, 0xa7, 0x82, 0xff, 0xea, 0x29, 0x99, 0xff, 0xd1, 0x25, 0x89, 0xff, 0x21, 0x06, 0x16, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 
    0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x1e, 0x05, 0x14, 0xff, 0xc2, 0x22, 0x7f, 0xff, 0xee, 0x4e, 0x92, 0xff, 0xfd, 0xd4, 0x79, 0xff, 0xec, 0x3a, 0x96, 0xff, 0xea, 0x28, 0x97, 0xff, 0xec, 0x26, 0x92, 0xff, 0xed, 0x22, 0x8c, 0xff, 0xed, 0x24, 0x8e, 0xff, 0xec, 0x26, 0x93, 0xff, 0xea, 0x28, 0x97, 0xff, 0xea, 0x29, 0x99, 0xff, 0xfd, 0xd4, 0x79, 0xff, 0xef, 0x5a, 0x90, 0xff, 0xc2, 0x22, 0x7f, 0xff, 0x1d, 0x05, 0x13, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 
    0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x08, 0x01, 0x05, 0xff, 0x70, 0x14, 0x49, 0xff, 0xdb, 0x26, 0x8f, 0xff, 0xeb, 0x34, 0x97, 0xff, 0xea, 0x29, 0x99, 0xff, 0xeb, 0x28, 0x96, 0xff, 0xfe, 0xdf, 0x77, 0xff, 0xfe, 0xdf, 0x77, 0xff, 0xea, 0x28, 0x98, 0xff, 0xea, 0x29, 0x99, 0xff, 0xea, 0x29, 0x99, 0xff, 0xdb, 0x26, 0x8f, 0xff, 0x70, 0x14, 0x49, 0xff, 0x08, 0x01, 0x05, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 
    0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x10, 0x03, 0x0a, 0xff, 0x62, 0x11, 0x40, 0xff, 0xa4, 0x1d, 0x6b, 0xff, 0xce, 0x24, 0x87, 0xff, 0xe6, 0x26, 0x93, 0xff, 0xe5, 0x28, 0x95, 0xff, 0xce, 0x24, 0x87, 0xff, 0xa4, 0x1d, 0x6b, 0xff, 0x62, 0x11, 0x40, 0xff, 0x10, 0x03, 0x0a, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 

};

CMatrixModeClock::CMatrixModeClock(const uint8_t width, const uint8_t height)
: CBaseCanvasedMatrixMode(width, height), colMin(HAND_COLOR_MIN), colHour(HAND_COLOR_HOUR), colText(0x0001) {
    
    canvas->setTextColor(colText);
    canvas->setTextSize(1);
    canvas->setFont(&Picopixel);

    tMillisClockUpdate = 0;
}

CMatrixModeClock::~CMatrixModeClock() {
}

void CMatrixModeClock::draw(CRGB *leds) {

    if (millis() - tMillisClockUpdate > 10000) {
        tMillisClockUpdate = millis();
        timeUpdated = getLocalTime(&timeinfo);
    }

    if (millis() - tMillis > 100) {
        tMillis = millis();
        // clear
        memset(leds, 0, width * height * sizeof(CRGB));

        for(uint8_t y=0; y<height; y++) {
            for(uint8_t x=0; x<width; x++) {
                uint8_t r, g, b;
                uint16_t bi = (y * width + x) * BITMAP_BYTES_PER_PIXEL;

                b = BITMAP_CLOCK_FACE[bi] * configuration.ledBrightness;
                g = BITMAP_CLOCK_FACE[bi+1] * configuration.ledBrightness;
                r = BITMAP_CLOCK_FACE[bi+2] * configuration.ledBrightness;

                leds[ XY(x, y) ] = CRGB(r, g, b);           
            }
        }

        canvas->fillScreen(0);
        //canvas->drawCircle(11, 11, 10, 0xffff);

        /*
        m++;
        if (m>59) {
            m = 0;
            h++; 
            if (h>11) {
                h = 0;
            }
        }
        */

        if (!timeUpdated) {
            return;
        }

        h = timeinfo.tm_hour;
        m = timeinfo.tm_min;

        uint16_t x, y;
        double rad;

        // Hour
        rad = (h - 3) * PI / 6;
        x = 11 + 5 * cos(rad);
        y = 11 + 5 * sin(rad);
        drawThickLine(11, 11, x, y, colHour,2);
        
        // Minutes
        rad = (m-15) * PI / 30;
        x = 11 + 9 * cos(rad);
        y = 11 + 9 * sin(rad);
        canvas->drawLine(11, 11, x, y, colMin);

        canvas->setCursor(3, 18);
        canvas->printf("%02i:%02i", h, m);
        
        //log_d("Min: %i, rad: %f, x: %i, y: %i", m, rad, x, y);

        drawCanvas(leds);
    }
    
}

#ifdef KEYPAD
void CMatrixModeClock::keyEvent(key_status_t key) {

    switch (key) { 
        case KEY_LEFT: break;
        case KEY_RIGHT: break;
        case KEY_UP: ; break;
        case KEY_DOWN: ; break;
        case KEY_MIDDLE: ; break;
        default: ;
    }
    
}
#endif


