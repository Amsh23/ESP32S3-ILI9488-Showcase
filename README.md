# ESP32-S3 ILI9488 LovyanGFX Showcase

Simple PlatformIO project for an ESP32-S3 with an SPI ILI9488 LCD and XPT2046 touch controller.

The project intentionally keeps the code beginner-friendly: one main source file plus the known-good LovyanGFX display configuration.

## Hardware

- ESP32-S3 development board
- ILI9488 SPI LCD
- XPT2046 resistive touch controller
- LovyanGFX
- Arduino framework

## Project structure

```text
.
├── data/
│   └── README.md              # Optional SPIFFS wallpaper note
├── include/
│   └── LGFX_ILI9488.h         # Working LCD and touch configuration
├── src/
│   └── main.cpp               # Simple showcase sketch
├── .gitignore
├── LICENSE
├── platformio.ini
└── README.md
```

## Configuration

Edit the WiFi and timezone constants near the top of `src/main.cpp`:

```cpp
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";
const long GMT_OFFSET_SECONDS = 0;
const int DAYLIGHT_OFFSET_SECONDS = 0;
```

Do not change `include/LGFX_ILI9488.h` unless you intentionally need to change the working display or touch wiring.

## Optional wallpaper

Place a 480x320 landscape PNG at:

```text
data/background.png
```

Then upload the SPIFFS filesystem image.

If no wallpaper is uploaded, the firmware draws a simple fallback background.

## Build and upload

```bash
pio run
pio run --target upload
pio run --target uploadfs
pio device monitor
```

## Cleanup notes

The repository was reduced to one active PlatformIO project. Removed files were old experiments, duplicate Arduino sketches, or unused C++ modules that made the project more complex than necessary.

Deleted files and folders:

- `ArduinoIDE/` - duplicate Arduino IDE copy of the showcase code. Removed to keep one source of truth.
- `ILI9488_Test/` - old display test sketches and alternate display configurations. Removed because they were not part of the cleaned app and some settings conflicted with the working configuration.
- `LiveClockTest/` - old clock-only experiment with hard-coded credentials. Removed because the cleaned app already includes NTP clock display.
- `include/animation.h`, `include/clock.h`, `include/config.h`, `include/touch.h`, `include/ui.h`, `include/weather.h` - unused module headers after simplifying the app into `src/main.cpp`.
- `src/animation.cpp`, `src/clock.cpp`, `src/touch.cpp`, `src/ui.cpp`, `src/weather.cpp` - unused module implementations after removing unnecessary classes and abstractions.

## Notes

- The working LCD/touch configuration remains in `include/LGFX_ILI9488.h`.
- SPI pins, display rotation, and touch settings were not changed.
- The code avoids extra classes and keeps display behavior in plain functions.
