# ESP32-S3 ILI9488 LovyanGFX Showcase

![PlatformIO](https://img.shields.io/badge/PlatformIO-ready-orange)
![ESP32--S3](https://img.shields.io/badge/ESP32--S3-supported-blue)
![LovyanGFX](https://img.shields.io/badge/Graphics-LovyanGFX-green)
![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)

A polished PlatformIO demo for an ESP32-S3 Dev Board with a 3.5-inch 320x480 SPI ILI9488 TFT and XPT2046 touch controller. The project draws a full-screen wallpaper once, then renders clock, WiFi, weather placeholder, touch responses, and sprite-based animations on top using LovyanGFX only.

## Hardware List

- ESP32-S3 Dev Board
- 3.5-inch SPI TFT display
- ILI9488 display controller
- XPT2046 resistive touch controller
- 320x480 panel used in landscape orientation

## Features

- LovyanGFX-only rendering; no TFT_eSPI and no LVGL dependency.
- Existing `LGFX_ILI9488.h` display configuration is used unchanged.
- Arduino IDE sketch included for users who do not want to use PlatformIO.
- Wallpaper is loaded once from SPIFFS as `/background.png`.
- Top translucent-style panel with time, date, day of week, WiFi signal, and weather placeholder.
- NTP synchronization over WiFi using Arduino `time.h` support.
- Animated cat status text with touch-triggered speech bubbles.
- Moon, lantern, cat, and empty-space touch interactions.
- 30 FPS animation pacing with sprites and dirty-rectangle-oriented updates.
- TODO hooks for Weather API, RTC, brightness sensor, SD wallpaper, GIF, and future LVGL experiments.

## Folder Structure

```text
.
├── ArduinoIDE/
│   └── ESP32S3_ILI9488_Showcase/
│       ├── ESP32S3_ILI9488_Showcase.ino
│       └── LGFX_ILI9488.h
├── data/
│   └── README.md              # Put background.png here for SPIFFS upload
├── include/
│   ├── LGFX_ILI9488.h         # Provided display/touch configuration
│   ├── animation.h
│   ├── clock.h
│   ├── config.h
│   ├── touch.h
│   ├── ui.h
│   └── weather.h
├── src/
│   ├── animation.cpp
│   ├── clock.cpp
│   ├── main.cpp
│   ├── touch.cpp
│   ├── ui.cpp
│   └── weather.cpp
├── LICENSE
├── platformio.ini
└── README.md
```

## PlatformIO Setup

1. Install [PlatformIO Core](https://docs.platformio.org/) or the VS Code PlatformIO extension.
2. Copy your supplied wallpaper into `data/background.png`. Use a 480x320 PNG already prepared for landscape orientation.
3. Edit WiFi credentials and timezone values in `include/config.h`.
4. Build, upload firmware, and upload the SPIFFS filesystem image.

## Compilation

### PlatformIO

```bash
pio run
pio run --target upload
pio run --target uploadfs
pio device monitor
```

### Arduino IDE

1. Open `ArduinoIDE/ESP32S3_ILI9488_Showcase/ESP32S3_ILI9488_Showcase.ino` in Arduino IDE.
2. Install the ESP32 board package by Espressif Systems and the LovyanGFX library.
3. Select an ESP32-S3 board profile that matches your dev board.
4. Edit the WiFi credentials and timezone constants at the top of the `.ino` file.
5. Keep the unchanged `LGFX_ILI9488.h` file in the same sketch folder, then compile and upload.
6. Optional: upload `background.png` to SPIFFS as `/background.png` using an ESP32 filesystem upload tool.

## Screenshots

Add photos or captures of the running display here:

| Boot wallpaper | Clock panel | Touch interaction |
| --- | --- | --- |
| _Coming soon_ | _Coming soon_ | _Coming soon_ |

## Future Roadmap

- Real weather API integration.
- RTC persistence for offline timekeeping.
- Ambient-light-driven brightness control.
- SD card wallpaper loading.
- Animated GIF character layer.
- Optional LVGL exploration in a separate branch while keeping this demo LovyanGFX-first.

## Contributing

Contributions are welcome. Please keep the project focused on ESP32-S3, ILI9488, XPT2046, PlatformIO, and LovyanGFX. Open an issue for larger feature proposals before sending a pull request.

## License

This project is released under the MIT License. See [LICENSE](LICENSE) for details.
