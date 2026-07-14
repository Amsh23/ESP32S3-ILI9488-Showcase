#pragma once
#define LGFX_USE_V1
#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device
{
  lgfx::Panel_ILI9488 _panel;
  lgfx::Bus_SPI _bus;
  lgfx::Light_PWM _light;

public:

  LGFX(void)
  {
    {
      auto cfg = _bus.config();

      cfg.spi_host = SPI3_HOST;
      cfg.spi_mode = 0;
      cfg.freq_write = 5000000;
      cfg.freq_read = 2000000;

      cfg.pin_sclk = 12;
      cfg.pin_mosi = 11;
      cfg.pin_miso = 13;
      cfg.pin_dc = 9;

      _bus.config(cfg);
      _panel.setBus(&_bus);
    }

    {
      auto cfg = _panel.config();

      cfg.pin_cs = 10;
      cfg.pin_rst = 14;
      cfg.pin_busy = -1;

      cfg.memory_width = 480;
      cfg.memory_height = 320;

      cfg.panel_width = 480;
      cfg.panel_height = 320;

      cfg.offset_x = 0;
      cfg.offset_y = 0;
      cfg.offset_rotation = 0;

      cfg.readable = true;
      cfg.invert = true;
      cfg.rgb_order = true;
      cfg.dlen_16bit = true;
      cfg.bus_shared = true;

      _panel.config(cfg);
    }

    setPanel(&_panel);
  }
};