#pragma once
#include <LovyanGFX.hpp>

enum class TouchTarget { None, Cat, Moon, Lantern, Empty };
struct TouchEvent { TouchTarget target{TouchTarget::None}; int16_t x{0}; int16_t y{0}; bool pressed{false}; };
class TouchController {
public:
  explicit TouchController(lgfx::LGFX_Device& gfx);
  TouchEvent poll();
private:
  TouchTarget hitTest(int16_t x, int16_t y) const;
  lgfx::LGFX_Device& gfx_;
  bool wasPressed_{false};
};
