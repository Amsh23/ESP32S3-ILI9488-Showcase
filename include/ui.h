#pragma once
#include "clock.h"
#include "weather.h"
#include <LovyanGFX.hpp>

class UiRenderer {
public:
  explicit UiRenderer(lgfx::LGFX_Device& gfx);
  void begin();
  void drawWallpaperOnce();
  void drawTopPanel(const ClockService& clock, const WeatherService& weather, int rssi);
  void drawCatStatus(const char* text, uint8_t alpha);
  void drawSpeechBubble(const char* text);
  void clearSpeechBubble();
  void drawRipple(int16_t x, int16_t y, uint8_t radius, uint8_t alpha);
  void setBrightness(uint8_t value);
private:
  void pushPanelSprite();
  lgfx::LGFX_Device& gfx_;
  LGFX_Sprite panel_;
  LGFX_Sprite cat_;
  LGFX_Sprite bubble_;
  LGFX_Sprite ripple_;
};
