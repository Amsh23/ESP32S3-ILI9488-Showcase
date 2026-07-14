#include "touch.h"
TouchController::TouchController(lgfx::LGFX_Device& gfx) : gfx_(gfx) {}
// Emits a single event on the leading edge of each touch press.
TouchEvent TouchController::poll() { uint16_t x, y; bool p = gfx_.getTouch(&x, &y); TouchEvent e; if (p && !wasPressed_) { e = {hitTest(x, y), (int16_t)x, (int16_t)y, true}; } wasPressed_ = p; return e; }
// Maps landscape screen coordinates to named interactive scene regions.
TouchTarget TouchController::hitTest(int16_t x, int16_t y) const { if (x > 300 && y > 235) return TouchTarget::Cat; if (x > 350 && x < 440 && y < 105) return TouchTarget::Moon; if (x > 360 && x < 440 && y > 190 && y < 285) return TouchTarget::Lantern; return TouchTarget::Empty; }
