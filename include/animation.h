#pragma once
#include "touch.h"
#include "ui.h"
#include <Arduino.h>

class AnimationController {
public:
  explicit AnimationController(UiRenderer& ui);
  void begin();
  void update(uint32_t nowMs);
  void reactTo(TouchEvent event);
private:
  const char* randomCatMessage();
  UiRenderer& ui_;
  uint32_t lastFrame_{0};
  uint32_t lastStatusChange_{0};
  uint32_t bubbleUntil_{0};
  uint32_t rippleStarted_{0};
  int16_t rippleX_{0};
  int16_t rippleY_{0};
  bool lanternFade_{false};
  uint8_t statusIndex_{0};
};
