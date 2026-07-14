#include "ui.h"
#include "config.h"
#include <SPIFFS.h>
#include <WiFi.h>

namespace {
constexpr uint16_t kTransparentColor = 0x0120;
}

UiRenderer::UiRenderer(lgfx::LGFX_Device& gfx) : gfx_(gfx), panel_(&gfx), cat_(&gfx), bubble_(&gfx), ripple_(&gfx) {}

// Allocates reusable sprites so animated regions can be redrawn without refreshing the wallpaper.
void UiRenderer::begin() {
  panel_.setColorDepth(16); panel_.createSprite(config::kScreenWidth, 72);
  cat_.setColorDepth(16); cat_.createSprite(178, 34);
  bubble_.setColorDepth(16); bubble_.createSprite(220, 58);
  ripple_.setColorDepth(16); ripple_.createSprite(96, 96);
}

// Draws the boot wallpaper exactly once, with a simple fallback if /background.png is not uploaded.
void UiRenderer::drawWallpaperOnce() {
  if (SPIFFS.begin(true) && SPIFFS.exists("/background.png")) {
    gfx_.drawPngFile(SPIFFS, "/background.png", 0, 0, config::kScreenWidth, config::kScreenHeight);
    return;
  }
  for (int y = 0; y < config::kScreenHeight; ++y) {
    uint8_t b = 32 + (y / 5); gfx_.drawFastHLine(0, y, config::kScreenWidth, gfx_.color565(5, 10, b));
  }
  gfx_.fillCircle(395, 56, 28, gfx_.color565(235, 222, 178));
  gfx_.fillRoundRect(382, 210, 36, 58, 10, gfx_.color565(220, 120, 32));
  gfx_.drawString("Add data/background.png", 120, 154);
}

// Redraws only the top-panel dirty rectangle for clock, WiFi, and weather updates.
void UiRenderer::drawTopPanel(const ClockService& clock, const WeatherService& weather, int rssi) {
  panel_.fillSprite(kTransparentColor);
  panel_.fillRoundRect(6, 6, 468, 60, 12, panel_.color565(12, 18, 34));
  panel_.drawRoundRect(6, 6, 468, 60, 12, panel_.color565(80, 100, 140));
  panel_.setTextColor(TFT_WHITE); panel_.setTextDatum(middle_left); panel_.setFont(&fonts::Font4);
  panel_.drawString(clock.timeText().c_str(), 22, 30);
  panel_.setFont(&fonts::Font2); panel_.drawString((clock.dayText() + "  " + clock.dateText()).c_str(), 170, 23);
  const auto w = weather.snapshot(); panel_.drawString((w.label + ": " + w.temperature).c_str(), 170, 47);
  uint16_t c = rssi > -65 ? TFT_GREEN : (rssi > -80 ? TFT_YELLOW : TFT_RED);
  for (int i = 0; i < 4; ++i) panel_.fillRect(410 + i * 9, 48 - i * 7, 6, 7 + i * 7, (rssi == 0 || rssi < -90 + i * 10) ? TFT_DARKGREY : c);
  panel_.pushSprite(0, 0, kTransparentColor);
}

// Redraws the bottom-right cat status sprite with a software fade color.
void UiRenderer::drawCatStatus(const char* text, uint8_t alpha) {
  cat_.fillSprite(kTransparentColor); cat_.setTextDatum(middle_right); cat_.setTextColor(cat_.color565(alpha, alpha, alpha)); cat_.setFont(&fonts::Font2);
  cat_.drawString(text, 172, 17); cat_.pushSprite(292, 280, kTransparentColor);
}

// Draws a touch-triggered speech bubble over the lower-right scene area.
void UiRenderer::drawSpeechBubble(const char* text) {
  bubble_.fillSprite(kTransparentColor); bubble_.fillRoundRect(0, 0, 218, 46, 12, TFT_WHITE); bubble_.setTextColor(TFT_BLACK); bubble_.setTextDatum(middle_center); bubble_.setFont(&fonts::Font2); bubble_.drawString(text, 109, 23); bubble_.fillTriangle(165, 45, 178, 57, 190, 45, TFT_WHITE); bubble_.pushSprite(245, 210, kTransparentColor);
}
// Clears the speech-bubble dirty rectangle; production artwork can restore this from a wallpaper tile cache.
void UiRenderer::clearSpeechBubble() { gfx_.fillRect(245, 210, 220, 58, TFT_BLACK); }
// Draws one frame of a small ripple sprite centered on the touch point.
void UiRenderer::drawRipple(int16_t x, int16_t y, uint8_t radius, uint8_t alpha) { ripple_.fillSprite(kTransparentColor); ripple_.drawCircle(48, 48, radius, ripple_.color565(alpha, alpha, 255)); ripple_.pushSprite(x - 48, y - 48, kTransparentColor); }
// Applies display brightness changes used by the lantern animation.
void UiRenderer::setBrightness(uint8_t value) { gfx_.setBrightness(value); }
