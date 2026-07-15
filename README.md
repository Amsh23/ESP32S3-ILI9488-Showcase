# ESP32-S3 ILI9488 Showcase

A compact Arduino/LovyanGFX playground for driving a **3.5\" ILI9488 SPI TFT (480×320)** with an **ESP32-S3** — from first color tests to a live NTP clock and animated display demos.

<p align="center">
  <img src="docs/images/clock-cat.jpg" alt="ESP32-S3 ILI9488 clock with cat icon" width="420">
  <img src="docs/images/live-clock.jpg" alt="ESP32-S3 ILI9488 live clock" width="420">
</p>
<p align="center">
  <img src="docs/images/wiring-back.jpg" alt="ILI9488 wiring with ESP32-S3" width="420">
  <img src="docs/images/wiring-front.jpg" alt="ESP32-S3 and ILI9488 breadboard wiring" width="420">
</p>

## ✨ Demos

| Folder | What it shows |
| --- | --- |
| `ILI9488_Testwork/` | Basic RGBY screen test for validating the panel and SPI wiring. |
| `ILI9488_Testwork/simpleblink/ILI9488_Watch/` | Simple watch-style UI/animation experiment. |
| `LiveClockTestwork/LiveClockTest/` | Wi-Fi + NTP live clock with date display. |
| `PartyParroteditedworked/` | Animated Party Parrot rendered with LovyanGFX sprites. |
| `AnimeDashboardworkingonit/` | Work-in-progress dashboard with clock, image assets, and RSS-style scrolling. |

## 🧰 Hardware

- ESP32-S3 development board
- 3.5\" ILI9488 SPI TFT, 480×320
- XPT2046 touch controller pins are configured in the LovyanGFX headers
- Jumper wires + breadboard

## 🔌 Default SPI Pins

| Signal | ESP32-S3 GPIO |
| --- | ---: |
| SCLK | 12 |
| MOSI | 11 |
| MISO | 13 |
| DC | 9 |
| CS | 10 |
| RST | 14 |

## 🚀 Quick Start

1. Install the Arduino ESP32 core and the **LovyanGFX** library.
2. Open one of the `.ino` sketches in Arduino IDE.
3. Update Wi-Fi credentials where needed before flashing.
4. Select your ESP32-S3 board and upload.

> Keep private credentials out of public commits when turning these experiments into a shared project.

## 📸 Photos

Place the project photos in `docs/images/` using these names so the gallery renders automatically:

- `clock-cat.jpg`
- `live-clock.jpg`
- `wiring-back.jpg`
- `wiring-front.jpg`

## 📄 Notes

This repo is intentionally small and experimental: each folder is a separate sketch for learning, testing, and iterating on an ESP32-S3 + ILI9488 display setup.
