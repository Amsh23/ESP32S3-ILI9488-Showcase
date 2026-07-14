#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device
{
  lgfx::Panel_ILI9488 _panel;
  lgfx::Bus_SPI _bus;

public:

  LGFX()
  {
    {
      auto cfg = _bus.config();

      cfg.spi_host = SPI2_HOST;
      cfg.spi_mode = 0;

      cfg.freq_write = 5000000;   // خیلی پایین برای تست
      cfg.freq_read  = 1000000;

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

      cfg.memory_width  = 320;
      cfg.memory_height = 480;

      cfg.panel_width  = 320;
      cfg.panel_height = 480;

      cfg.offset_x = 0;
      cfg.offset_y = 0;

      cfg.invert = false;
      cfg.rgb_order = false;

      _panel.config(cfg);
    }

    setPanel(&_panel);
  }
};


LGFX lcd;


void setup()
{
  Serial.begin(115200);

  lcd.init();

  lcd.setRotation(1);

  lcd.fillScreen(TFT_BLACK);

}


void loop()
{
  lcd.fillScreen(TFT_RED);
  lcd.setTextColor(TFT_WHITE);
  lcd.setTextSize(3);
  lcd.drawString("RED", 100, 100);
  delay(2000);


  lcd.fillScreen(TFT_GREEN);
  lcd.setTextColor(TFT_BLACK);
  lcd.drawString("GREEN", 100, 100);
  delay(2000);


  lcd.fillScreen(TFT_BLUE);
  lcd.setTextColor(TFT_WHITE);
  lcd.drawString("BLUE", 100, 100);
  delay(2000);


  lcd.fillScreen(TFT_YELLOW);
  lcd.setTextColor(TFT_BLACK);
  lcd.drawString("YELLOW", 100, 100);
  delay(2000);
}