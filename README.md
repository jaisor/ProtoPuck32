# ProtoPuck32

Multi-purpose prototype box based on ESP32

## Issues

LittleFS_esp32 library installed by PlatformIO seems broken, complaining about 

```
error: unknown field 'utime_p' specified in initializer
```
Replacing `libdeps/LittleFS_esp32` with downliad from [LittleFS_esp32](https://www.arduinolibraries.info/libraries/little-fs_esp32) fixes it