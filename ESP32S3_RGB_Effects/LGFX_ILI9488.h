#pragma once

#define LGFX_USE_V1
#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device
{
  lgfx::Panel_ILI9488 _panel;
  lgfx::Bus_SPI       _bus;
  lgfx::Touch_XPT2046 _touch;

public:

  LGFX(void)
  {
    //====================================================
    // SPI BUS
    //====================================================
    {
      auto cfg = _bus.config();

      cfg.spi_host = SPI2_HOST;
      cfg.spi_mode = 0;

      cfg.freq_write = 20000000;
      cfg.freq_read  = 8000000;

      cfg.spi_3wire = false;
      cfg.use_lock  = true;

      cfg.pin_sclk = 12;
      cfg.pin_mosi = 11;
      cfg.pin_miso = 13;
      cfg.pin_dc   = 9;

      _bus.config(cfg);
      _panel.setBus(&_bus);
    }

    //====================================================
    // LCD
    //====================================================
    {
      auto cfg = _panel.config();

      cfg.pin_cs   = 10;
      cfg.pin_rst  = 14;
      cfg.pin_busy = -1;

      cfg.panel_width  = 320;
      cfg.panel_height = 480;

      cfg.memory_width  = 320;
      cfg.memory_height = 480;

      cfg.offset_x = 0;
      cfg.offset_y = 0;
      cfg.offset_rotation = 0;

      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits  = 1;

      cfg.readable   = false;
      cfg.invert     = false;
      cfg.rgb_order  = false;
      cfg.dlen_16bit = false;

      cfg.bus_shared = true;

      _panel.config(cfg);
    }

    //====================================================
    // TOUCH
    //====================================================
    {
      auto cfg = _touch.config();

      cfg.x_min = 200;
      cfg.x_max = 3800;

      cfg.y_min = 200;
      cfg.y_max = 3800;

      cfg.pin_cs  = 21;
      cfg.pin_int = -1;

      cfg.spi_host = SPI2_HOST;
      cfg.freq     = 2500000;

      cfg.pin_sclk = 12;
      cfg.pin_mosi = 11;
      cfg.pin_miso = 13;

      cfg.bus_shared = true;

      cfg.offset_rotation = 0;

      _touch.config(cfg);

      _panel.setTouch(&_touch);
    }

    setPanel(&_panel);
  }
};