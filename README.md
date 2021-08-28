# ProtoPuck32

Multi-purpose prototype box based on ESP32

## Development

USB driver for Windows required: [Silicon Labs CP210X](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)

Recommended: [VSCode](https://code.visualstudio.com/) + [PlatformIO](https://platformio.org/)

## Issues

LittleFS_esp32 library installed by PlatformIO seems broken, complaining about 

```
error: unknown field 'utime_p' specified in initializer
```
Replacing `libdeps/LittleFS_esp32` with downliad from [LittleFS_esp32](https://www.arduinolibraries.info/libraries/little-fs_esp32) fixes it

## Bill of materials (BOM)

* [ESP32 Wroom](https://www.amazon.com/gp/product/B08D5ZD528)
* [OLED 128x64 i2c](https://www.amazon.com/gp/product/B072Q2X2LL)
* [BME280 Temperature/Humidity sendor i2c](https://www.amazon.com/gp/product/B07KYJNFMD)
* [FPV Camera keypad](https://www.amazon.com/RunCam-Key-Board-FPV-Camera/dp/B0874GPT4W) - I used a joystick style from Caddx, but any would work, case will need adjustment
* [DC plug connector](https://www.amazon.com/gp/product/B01N8VV78D)
* [5V 15A power adapter](https://www.amazon.com/gp/product/B08764XJ2M)
* [White PLA for case and LED diffusion](https://www.amazon.com/gp/product/B01EKEMIIS)

---

## Builds

### LED Node Leaf

![LED displaying heart](/pics/leafnodes.jpg)

Solder the DI/Signal wire (usually middle of the strip) to pin 13, otherwise change the `LED_PIN` in `Configuration.h`

## Bill of materials (BOM)

In addition to the base driver above:
* [WS2812B RGB LED Strip Light](https://www.amazon.com/gp/product/B08B59CBK9/)
* [RGBZONE 20Pairs JST SM 3PIN Plugs](https://www.amazon.com/gp/product/B075K4HLTQ/)

Case - 3D printed, STL files:

* [LED diffuser](case/NodeLeaf/LEDNodeLeafDiffuser.stl)
* [LED frame](case/NodeLeaf/LEDNodeLeafBase.stl)


### LED Matrix

![LED displaying heart](/pics/heart.jpg)
![Back-bottom open, shows parts](/pics/back_open.jpg)

Add to BOM: [22x22 WS2812B pannel](https://www.amazon.com/gp/product/B075T9RRPM)

Schematic:

![Schematic](./schematics/protopuck32-led-matrix.svg)

Case - 3D printed, STL files:

* [LED diffuser](case/LED_Matrix/LEDMatrix-Diffuser.stl)
* [LED frame](case/LED_Matrix/LEDMatrix-FrameSide.stl)
* [LED frame back brace](case/LED_Matrix/LEDMatrix-FrameBackBrace.stl)
* [Base](case/LED_Matrix/LEDMatrix-FrameBase.stl)
* [Base lid](case/LED_Matrix/LEDMatrix-FrameBaseLid.stl)

---

# Bugs / TODO

1. Connecting to Wifi AP after creating one fails - probably something needs to be cleaned up in the WiFi instances
1. Configuration menu
1. Remote controlled pixels via WiFi (HTTP Post or MQTT)
1. Report sensors data and config out to HTTP or MQTT
1. Remote control of config like brightness, mode, etc.
1. Yellow top lines for Config - looks odd
1. Web page headers and titles - remove Demo
1. Get device location automatically if possible
1. Expose current clock broadly, including periodical updates