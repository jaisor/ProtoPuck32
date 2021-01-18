#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <EEPROM.h>
#include <FastLED.h>

#include "Configuration.h"
#include "WifiManager.h"

#if !( defined(ESP32) )
  #error This code is intended to run on ESP32 platform! Please check your Tools->Board setting.
#endif

CRGB leds[LED_STRIP_SIZE];
uint8_t r, g, b;

Adafruit_SSD1306 display(OLED_SCREEN_WIDTH, OLED_SCREEN_HEIGHT, &Wire, -1);
Adafruit_BME280 bme;

CWifiManager *wifiManager;

int tempInC = EEPROM.read(0);
float p = 0;

void setup() {
  Serial.begin(115200); while (!Serial); delay(200);

  EEPROM_loadConfig();

  //strcpy(configuration.wifi_ssid, "<REDACTED>");
  //strcpy(configuration.wifi_password, "<REDACTED>");
  //EEPROM_saveConfig();

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

  FastLED.addLeds<LED_TYPE, LED_PIN, LED_COLOR_ORDER>(leds, LED_STRIP_SIZE).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( LED_BRIGHTNESS );

  wifiManager = new CWifiManager();
    
  //currentPalette = RainbowColors_p;
  //currentBlending = LINEARBLEND;
}

void loop() {
  display.clearDisplay();

  wifiManager->OLED_Status(&display);

  //

  // display temperature
  
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

  for( int i = 0; i < LED_STRIP_SIZE; i++) {
      //leds[i] = CRGB(245, 5, 37);
      leds[i] = CRGB(r, g, b);

      r += random(-2,2);
      g += random(-2,2);
      b += random(-2,2);

      if (r>=255) r=0;
      if (g>=255) g=0;
      if (b>=255) b=0;

      if (r<=0) r=255;
      if (g<=0) g=255;
      if (b<=0) b=255;
  }
  FastLED.show();

  //

  display.display();
  FastLED.show();

  delay(300);
}