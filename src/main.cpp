#include "LGFX_ILI9488.h"
#include "animation.h"
#include "clock.h"
#include "config.h"
#include "touch.h"
#include "ui.h"
#include "weather.h"
#include <Arduino.h>
#include <Preferences.h>
#include <SPI.h>
#include <WiFi.h>

namespace {
LGFX display;
ClockService clockService;
WeatherService weatherService;
UiRenderer ui(display);
TouchController touch(display);
AnimationController animations(ui);
Preferences preferences;
uint32_t lastClockDraw = 0;

// Connects to WiFi once at boot so the clock can synchronize with NTP.
void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(config::kWifiSsid, config::kWifiPassword);
  const uint32_t started = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - started < 15000) delay(250);
}

// Updates the top-panel dirty rectangle once per second.
void drawClockIfDue() {
  if (millis() - lastClockDraw < 1000) return;
  lastClockDraw = millis();
  clockService.update();
  weatherService.update();
  ui.drawTopPanel(clockService, weatherService, WiFi.status() == WL_CONNECTED ? WiFi.RSSI() : 0);
}
}

void setup() {
  Serial.begin(115200);
  preferences.begin("showcase", false);
  display.init();
  display.setRotation(1);
  display.setBrightness(config::kDefaultBrightness);
  display.startWrite();
  ui.begin();
  ui.drawWallpaperOnce();
  connectWiFi();
  clockService.begin();
  weatherService.begin();
  animations.begin();
  display.endWrite();
}

void loop() {
  display.startWrite();
  drawClockIfDue();
  animations.reactTo(touch.poll());
  animations.update(millis());
  display.endWrite();
  delay(1);
}

// TODO: Weather API - replace WeatherService placeholder with a real HTTPS weather provider.
// TODO: RTC support - persist NTP time to an external or ESP32-compatible RTC source.
// TODO: Brightness sensor - map ambient light readings to display brightness.
// TODO: SD card wallpaper support - load /background.png from SD before SPIFFS fallback.
// TODO: GIF support - add a sprite-backed animated GIF layer for character effects.
// TODO: LVGL integration - optional future branch only; this demo intentionally uses LovyanGFX directly.
