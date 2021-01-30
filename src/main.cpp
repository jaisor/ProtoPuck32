#include <Arduino.h>

#include <functional>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <EEPROM.h>
#include <FastLED.h>

#include "Configuration.h"
#include "WifiManager.h"
#include "KeypadManager.h"
#include "InternalLEDManager.h"

#ifdef LED_EXTERNAL_MATRIX
  #include "MatrixLEDManager.h"
#else
  #include "DemoLEDManager.h"
#endif

#if !( defined(ESP32) )
  #error This code is intended to run on ESP32 platform! Please check your Tools->Board setting.
#endif

#ifdef LED
  CRGB ledsInternal[LED_STRIP_SIZE];
  CRGB ledsExternal[LED_EXTERNAL_STRIP_SIZE];
  uint8_t r, g, b;
#endif

#ifdef OLED
  Adafruit_SSD1306 display(OLED_SCREEN_WIDTH, OLED_SCREEN_HEIGHT, &Wire, -1);
#endif

#ifdef TEMP_SENSOR
  Adafruit_BME280 bme;
#endif

CBaseManager *managers[4];

unsigned long tMillis;

int tempInC = 0; //EEPROM.read(0);
float p = 0;

void setup() {
  Serial.begin(115200); while (!Serial); delay(200);

  EEPROM_loadConfig();

  //strcpy(configuration.wifiSsid, "<REDACTED>");
  //strcpy(configuration.wifiPassword, "<REDACTED>");
  //EEPROM_saveConfig();

#ifdef OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    log_e("SSD1306 OLED initialiation failed");
    for(;;);
  }
#endif

#ifdef TEMP_SENSOR
  bool status = bme.begin(0x76);  
  if (!status) {
    log_e("BME280 sensor initialiation failed");
    while (1);
  }
#endif
  
  delay(100);

#ifdef OLED
  display.clearDisplay();
  display.setTextColor(WHITE);
#endif

#ifdef LED
  FastLED.addLeds<LED_TYPE, LED_PIN, LED_COLOR_ORDER>(ledsInternal, LED_STRIP_SIZE).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<LED_EXTERNAL_TYPE, LED_EXTERNAL_PIN, LED_COLOR_ORDER>(ledsExternal, LED_EXTERNAL_STRIP_SIZE).setCorrection( TypicalLEDStrip );
#endif

  uint8_t mgrIndex = 0;

#ifdef WIFI
  managers[mgrIndex++] = new CWifiManager();
#endif

#ifdef LED
  managers[mgrIndex++] = new CInternalLEDManager(LED_STRIP_SIZE, configuration.ledBrightness);
  #ifdef LED_EXTERNAL_MATRIX
    managers[mgrIndex++] = new CMatrixLEDManager(ledsExternal, LED_EXTERNAL_STRIP_SIZE, LED_EXTERNAL_MATRIX_WIDTH, LED_EXTERNAL_MATRIX_HEIGHT, LED_EXTERNAL_BRIGHTNESS); 
  #else
    managers[mgrIndex++] = new CDemoLEDManager(ledsExternal, LED_EXTERNAL_STRIP_SIZE, LED_EXTERNAL_BRIGHTNESS); 
  #endif
#endif

#ifdef KEYPAD
  CKeypadManager *keypadManager = new CKeypadManager();
  managers[mgrIndex++] = keypadManager;

  using std::placeholders::_1;
  for(auto & manager : managers) {
    if (manager) {
      keypadManager->addKeyListener(std::bind(&CBaseManager::keyEvent, manager, _1));
    }
  }
#endif
  
  tMillis = millis();
}

void loop() {
  #ifdef OLED
    display.clearDisplay();
  #endif

  // Presentation
  if (millis() - tMillis > 100) {
    tMillis = millis();
    
    for(auto & manager : managers) {
      #ifdef OLED
        manager->OLED_Status(&display);
      #endif
      #ifdef LED
        manager->LED_Status(ledsInternal);
      #endif
    }

    //

    // display temperature
#ifdef OLED
    
    display.setCursor(0,16);
    display.print("Temperature: ");
    display.setTextSize(2);
    display.setCursor(0,24);


    if (digitalRead(BOOT_BUTTON) == LOW) {
      tempInC = !tempInC;
      //EEPROM.write(0, tempInC);
    }

#ifdef TEMP_SENSOR
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
    display.print(String(bme.readHumidity(), 1));
    display.print("%"); 
#endif

    display.display();
#endif

#ifdef LED
    FastLED.show();
#endif
  }  

  // Post presentation
  
  for(auto & manager : managers) {
    manager->loop();
  }
}