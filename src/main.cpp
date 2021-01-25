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
#include "DemoLEDManager.h"

#if !( defined(ESP32) )
  #error This code is intended to run on ESP32 platform! Please check your Tools->Board setting.
#endif

CRGB ledsInternal[LED_STRIP_SIZE];
CRGB ledsExternal[LED_EXTERNAL_STRIP_SIZE];
uint8_t r, g, b;

Adafruit_SSD1306 display(OLED_SCREEN_WIDTH, OLED_SCREEN_HEIGHT, &Wire, -1);
Adafruit_BME280 bme;

CBaseManager *managers[4];

unsigned long tMillis;

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

  FastLED.addLeds<LED_TYPE, LED_PIN, LED_COLOR_ORDER>(ledsInternal, LED_STRIP_SIZE).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<LED_EXTERNAL_TYPE, LED_EXTERNAL_PIN, LED_COLOR_ORDER>(ledsExternal, LED_EXTERNAL_STRIP_SIZE).setCorrection( TypicalLEDStrip );
  //FastLED.setBrightness( LED_BRIGHTNESS );

  uint8_t mgrIndex = 0;
  managers[mgrIndex++] = new CWifiManager();
  managers[mgrIndex++] = new CDemoLEDManager(ledsExternal, LED_EXTERNAL_STRIP_SIZE, LED_EXTERNAL_BRIGHTNESS);
  managers[mgrIndex++] = new CInternalLEDManager(LED_STRIP_SIZE, LED_BRIGHTNESS);
  CKeypadManager *keypadManager = new CKeypadManager();
  managers[mgrIndex++] = keypadManager;

  using std::placeholders::_1;
  for(auto & manager : managers) {
    keypadManager->addKeyListener(std::bind(&CBaseManager::keyEvent, manager, _1));
  }
  //auto p = std::bind(&CInternalLEDManager::keyEvent, ledManager);
  //keypadManager->addKeyListener(p);
  


  tMillis = millis();
}

void loop() {
  display.clearDisplay();

  // Presentation
  if (millis() - tMillis > 100) {
    tMillis = millis();
    
    for(auto & manager : managers) {
      manager->OLED_Status(&display);
      manager->LED_Status(ledsInternal);
    }

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
    display.print(String(bme.readHumidity(), 1));
    display.print("%"); 

    display.display();
    FastLED.show();
  }  

  // Post presentation
  
  for(auto & manager : managers) {
    manager->loop();
  }
}