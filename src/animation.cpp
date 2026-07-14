#include "animation.h"
#include "config.h"
#include <cstdlib>

static constexpr const char* kStatuses[] = {"Sleeping...", "Watching stars...", "Happy!", "Thinking...", "Listening..."};
static constexpr const char* kMessages[] = {"Meow!", "Purrr...", "Hi!", "(^_^)", "Nyaa~"};

AnimationController::AnimationController(UiRenderer& ui) : ui_(ui) {}
// Draws the first status frame before the main animation loop starts.
void AnimationController::begin() { ui_.drawCatStatus(kStatuses[0], 255); }

// Advances sprite animations at an approximately 30 FPS cadence.
void AnimationController::update(uint32_t nowMs) {
  if (nowMs - lastFrame_ < config::kFrameMs) return;
  lastFrame_ = nowMs;
  if (nowMs - lastStatusChange_ > 3500) { statusIndex_ = (statusIndex_ + 1) % (sizeof(kStatuses) / sizeof(kStatuses[0])); lastStatusChange_ = nowMs; }
  uint8_t fade = 120 + std::abs((int)((nowMs / 12) % 270) - 135);
  ui_.drawCatStatus(kStatuses[statusIndex_], fade);
  if (bubbleUntil_ && nowMs > bubbleUntil_) { ui_.clearSpeechBubble(); bubbleUntil_ = 0; }
  if (rippleStarted_) { uint32_t age = nowMs - rippleStarted_; if (age < 700) ui_.drawRipple(rippleX_, rippleY_, age / 12, 255 - age / 3); else rippleStarted_ = 0; }
  if (lanternFade_) { uint8_t v = 120 + std::abs((int)((nowMs / 8) % 200) - 100); ui_.setBrightness(v); if ((nowMs - lastStatusChange_) > 1200) { ui_.setBrightness(config::kDefaultBrightness); lanternFade_ = false; } }
}

// Converts touch events into cat, moon, lantern, or ripple reactions.
void AnimationController::reactTo(TouchEvent event) {
  if (!event.pressed) return;
  if (event.target == TouchTarget::Cat) { ui_.drawSpeechBubble(randomCatMessage()); bubbleUntil_ = millis() + 2200; }
  else if (event.target == TouchTarget::Moon) { ui_.drawSpeechBubble("The moon is beautiful tonight."); bubbleUntil_ = millis() + 3000; }
  else if (event.target == TouchTarget::Lantern) { lanternFade_ = true; lastStatusChange_ = millis(); }
  else { rippleX_ = event.x; rippleY_ = event.y; rippleStarted_ = millis(); }
}
const char* AnimationController::randomCatMessage() { return kMessages[random(sizeof(kMessages) / sizeof(kMessages[0]))]; }
