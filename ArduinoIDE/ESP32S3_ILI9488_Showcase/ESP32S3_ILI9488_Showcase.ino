/*
  ESP32-S3 + ILI9488 + XPT2046 LovyanGFX Showcase for Arduino IDE

  Required Arduino libraries/boards:
  - ESP32 board package by Espressif Systems
  - LovyanGFX

  Put the unchanged LGFX_ILI9488.h file next to this .ino file.
  Optional wallpaper: upload /background.png to SPIFFS with the ESP32 Sketch Data Upload tool.
*/

#include "LGFX_ILI9488.h"
#include <Arduino.h>
#include <LovyanGFX.hpp>
#include <SPI.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <time.h>

// ===== User settings =====
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";
const char* NTP_SERVER_1 = "pool.ntp.org";
const char* NTP_SERVER_2 = "time.nist.gov";
const long GMT_OFFSET_SECONDS = 0;
const int DAYLIGHT_OFFSET_SECONDS = 0;

// ===== Display/UI constants =====
constexpr int SCREEN_WIDTH = 480;
constexpr int SCREEN_HEIGHT = 320;
constexpr int FRAME_MS = 33;
constexpr uint8_t DEFAULT_BRIGHTNESS = 220;
constexpr uint16_t TRANSPARENT_COLOR = 0x0120;

LGFX tft;
LGFX_Sprite topPanel(&tft);
LGFX_Sprite catSprite(&tft);
LGFX_Sprite bubbleSprite(&tft);
LGFX_Sprite rippleSprite(&tft);

const char* CAT_STATUSES[] = {"Sleeping...", "Watching stars...", "Happy!", "Thinking...", "Listening..."};
const char* CAT_MESSAGES[] = {"Meow!", "Purrr...", "Hi!", "(^_^)", "Nyaa~"};

uint32_t lastClockDraw = 0;
uint32_t lastFrame = 0;
uint32_t lastStatusChange = 0;
uint32_t bubbleUntil = 0;
uint32_t rippleStarted = 0;
int16_t rippleX = 0;
int16_t rippleY = 0;
uint8_t statusIndex = 0;
bool wasTouched = false;
bool lanternFade = false;
uint32_t lanternStarted = 0;

// Draws the wallpaper once at boot. The UI never redraws the full screen after this.
void drawWallpaperOnce() {
  if (SPIFFS.begin(true) && SPIFFS.exists("/background.png")) {
    tft.drawPngFile(SPIFFS, "/background.png", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    return;
  }

  for (int y = 0; y < SCREEN_HEIGHT; ++y) {
    const uint8_t blue = 32 + (y / 5);
    tft.drawFastHLine(0, y, SCREEN_WIDTH, tft.color565(5, 10, blue));
  }
  tft.fillCircle(395, 56, 28, tft.color565(235, 222, 178));
  tft.fillRoundRect(382, 210, 36, 58, 10, tft.color565(220, 120, 32));
  tft.setTextColor(TFT_WHITE);
  tft.drawString("Upload /background.png to SPIFFS", 112, 154);
}

// Connects WiFi once so NTP can synchronize time.
void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  const uint32_t started = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - started < 15000) {
    delay(250);
  }
}

// Formats local time safely for the top panel.
String formattedTime(const char* format, const char* fallback) {
  tm timeInfo{};
  if (!getLocalTime(&timeInfo, 5) || (timeInfo.tm_year + 1900) < 2024) return fallback;
  char buffer[32];
  strftime(buffer, sizeof(buffer), format, &timeInfo);
  return buffer;
}

// Redraws the top dirty rectangle only: clock, date, day, WiFi and weather placeholder.
void drawTopPanel() {
  topPanel.fillSprite(TRANSPARENT_COLOR);
  topPanel.fillRoundRect(6, 6, 468, 60, 12, topPanel.color565(12, 18, 34));
  topPanel.drawRoundRect(6, 6, 468, 60, 12, topPanel.color565(80, 100, 140));

  topPanel.setTextColor(TFT_WHITE);
  topPanel.setTextDatum(middle_left);
  topPanel.setFont(&fonts::Font4);
  topPanel.drawString(formattedTime("%H:%M:%S", "--:--:--"), 22, 30);

  topPanel.setFont(&fonts::Font2);
  topPanel.drawString(formattedTime("%A  %Y-%m-%d", "Syncing NTP"), 170, 23);
  topPanel.drawString("Weather API: Soon", 170, 47);

  const int rssi = WiFi.status() == WL_CONNECTED ? WiFi.RSSI() : 0;
  const uint16_t color = rssi > -65 ? TFT_GREEN : (rssi > -80 ? TFT_YELLOW : TFT_RED);
  for (int i = 0; i < 4; ++i) {
    const bool active = rssi != 0 && rssi >= (-90 + i * 10);
    topPanel.fillRect(410 + i * 9, 48 - i * 7, 6, 7 + i * 7, active ? color : TFT_DARKGREY);
  }
  topPanel.pushSprite(0, 0, TRANSPARENT_COLOR);
}

// Updates the clock panel once per second without touching the wallpaper.
void drawClockIfDue() {
  if (millis() - lastClockDraw < 1000) return;
  lastClockDraw = millis();
  drawTopPanel();
}

// Draws the fading bottom-right cat status text.
void drawCatStatus(const char* text, uint8_t fade) {
  catSprite.fillSprite(TRANSPARENT_COLOR);
  catSprite.setTextDatum(middle_right);
  catSprite.setTextColor(catSprite.color565(fade, fade, fade));
  catSprite.setFont(&fonts::Font2);
  catSprite.drawString(text, 172, 17);
  catSprite.pushSprite(292, 280, TRANSPARENT_COLOR);
}

// Draws a speech bubble for cat and moon interactions.
void drawSpeechBubble(const char* text) {
  bubbleSprite.fillSprite(TRANSPARENT_COLOR);
  bubbleSprite.fillRoundRect(0, 0, 218, 46, 12, TFT_WHITE);
  bubbleSprite.setTextColor(TFT_BLACK);
  bubbleSprite.setTextDatum(middle_center);
  bubbleSprite.setFont(&fonts::Font2);
  bubbleSprite.drawString(text, 109, 23);
  bubbleSprite.fillTriangle(165, 45, 178, 57, 190, 45, TFT_WHITE);
  bubbleSprite.pushSprite(245, 210, TRANSPARENT_COLOR);
}

// Clears the speech-bubble dirty rectangle. A final product can restore this from a wallpaper tile cache.
void clearSpeechBubble() {
  tft.fillRect(245, 210, 220, 58, TFT_BLACK);
}

// Draws one ripple animation frame around an empty-space touch.
void drawRipple(uint32_t age) {
  rippleSprite.fillSprite(TRANSPARENT_COLOR);
  rippleSprite.drawCircle(48, 48, age / 12, rippleSprite.color565(120, 120, 255));
  rippleSprite.pushSprite(rippleX - 48, rippleY - 48, TRANSPARENT_COLOR);
}

// Hit-tests landscape coordinates against the interactive artwork regions.
int touchTarget(int16_t x, int16_t y) {
  if (x > 300 && y > 235) return 1;                         // Cat
  if (x > 350 && x < 440 && y < 105) return 2;               // Moon
  if (x > 360 && x < 440 && y > 190 && y < 285) return 3;    // Lantern
  return 4;                                                  // Empty space
}

// Handles touch actions: cat speech, moon quote, lantern brightness fade or empty-space ripple.
void handleTouch() {
  uint16_t x = 0;
  uint16_t y = 0;
  const bool touched = tft.getTouch(&x, &y);
  if (!touched || wasTouched) {
    wasTouched = touched;
    return;
  }

  switch (touchTarget(x, y)) {
    case 1:
      drawSpeechBubble(CAT_MESSAGES[random(sizeof(CAT_MESSAGES) / sizeof(CAT_MESSAGES[0]))]);
      bubbleUntil = millis() + 2200;
      break;
    case 2:
      drawSpeechBubble("The moon is beautiful tonight.");
      bubbleUntil = millis() + 3000;
      break;
    case 3:
      lanternFade = true;
      lanternStarted = millis();
      break;
    default:
      rippleX = x;
      rippleY = y;
      rippleStarted = millis();
      break;
  }
  wasTouched = touched;
}

// Advances all sprite animations at about 30 FPS.
void updateAnimations() {
  const uint32_t now = millis();
  if (now - lastFrame < FRAME_MS) return;
  lastFrame = now;

  if (now - lastStatusChange > 3500) {
    statusIndex = (statusIndex + 1) % (sizeof(CAT_STATUSES) / sizeof(CAT_STATUSES[0]));
    lastStatusChange = now;
  }
  const uint8_t fade = 120 + abs(static_cast<int>((now / 12) % 270) - 135);
  drawCatStatus(CAT_STATUSES[statusIndex], fade);

  if (bubbleUntil && now > bubbleUntil) {
    clearSpeechBubble();
    bubbleUntil = 0;
  }

  if (rippleStarted) {
    const uint32_t age = now - rippleStarted;
    if (age < 700) drawRipple(age);
    else rippleStarted = 0;
  }

  if (lanternFade) {
    const uint8_t brightness = 120 + abs(static_cast<int>((now / 8) % 200) - 100);
    tft.setBrightness(brightness);
    if (now - lanternStarted > 1200) {
      tft.setBrightness(DEFAULT_BRIGHTNESS);
      lanternFade = false;
    }
  }
}

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(1);
  tft.setBrightness(DEFAULT_BRIGHTNESS);
  tft.startWrite();

  topPanel.setColorDepth(16);
  topPanel.createSprite(SCREEN_WIDTH, 72);
  catSprite.setColorDepth(16);
  catSprite.createSprite(178, 34);
  bubbleSprite.setColorDepth(16);
  bubbleSprite.createSprite(220, 58);
  rippleSprite.setColorDepth(16);
  rippleSprite.createSprite(96, 96);

  drawWallpaperOnce();
  connectWiFi();
  configTime(GMT_OFFSET_SECONDS, DAYLIGHT_OFFSET_SECONDS, NTP_SERVER_1, NTP_SERVER_2);
  drawTopPanel();
  drawCatStatus(CAT_STATUSES[0], 255);
  tft.endWrite();
}

void loop() {
  tft.startWrite();
  drawClockIfDue();
  handleTouch();
  updateAnimations();
  tft.endWrite();
  delay(1);
}

// TODO: Weather API - replace the placeholder text with a real HTTPS weather client.
// TODO: RTC support - save synchronized time to RTC hardware or ESP32 RTC memory.
// TODO: Brightness sensor - map ambient-light readings to display brightness.
// TODO: SD card wallpaper support - load background.png from SD before SPIFFS fallback.
// TODO: GIF support - add animated GIF decoding into a sprite layer.
// TODO: LVGL integration - optional future experiment; this sketch intentionally uses LovyanGFX only.
