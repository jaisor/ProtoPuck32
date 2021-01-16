#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <EEPROM.h>
#include <FastLED.h>

#include "WifiManager.h"

#if !( defined(ESP32) )
  #error This code is intended to run on ESP32 platform! Please check your Tools->Board setting.
#endif

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define BOOT_BUTTON 0

#define LED_PIN     13
#define NUM_LEDS    12
#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

Adafruit_BME280 bme;

int tempInC = EEPROM.read(0);
float p = 0;

void setup() {
  Serial.begin(115200); while (!Serial); delay(200);

  setupWifi();

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  bool status = bme.begin(0x76);  
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
    
  //currentPalette = RainbowColors_p;
  //currentBlending = LINEARBLEND;
}

void loop() {
  display.clearDisplay();
  // display temperature
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Wazzaaa!!!");
  
  //
  display.fillRect(0, 8, p, 8, WHITE);
  p += 5;
  if (p>SCREEN_WIDTH) {
    p=0;
  }
  //
  
  display.setCursor(0,16);
  display.print("Temperature: ");
  display.setTextSize(2);
  display.setCursor(0,24);


  if (digitalRead(BOOT_BUTTON) == LOW) {
    tempInC = !tempInC;
    EEPROM.write(0, tempInC);
  }

  int temp = bme.readTemperature();

  display.print(String(tempInC ? temp : (temp*9/5)+32 ));
  display.print(" ");
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(2);
  display.print(tempInC ? "C" : "F");
  
  // display humidity
  display.setTextSize(1);
  display.setCursor(0, 40);
  display.print("Humidity: ");
  display.setTextSize(2);
  display.setCursor(0, 48);
  display.print(String(bme.readHumidity()));
  display.print(" %"); 
  
  display.display();

  //

  for( int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(245, 5, 37);
  }
  FastLED.show();

  delay(100);
}