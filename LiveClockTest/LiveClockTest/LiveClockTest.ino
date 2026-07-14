#include <WiFi.h>
#include <time.h>
#include <LovyanGFX.hpp>
#include "LGFX_ILI9488.h"   // فایل تنظیمات نمایشگر خودت

LGFX lcd;

const char* ssid     = "Arian";
const char* password = "Amir2003Sh@Q%";

// ایران
const long gmtOffset_sec = 3 * 3600 + 30 * 60;
const int daylightOffset_sec = 0;

void setup() {
  Serial.begin(115200);

  lcd.init();
  lcd.setRotation(1);
  lcd.fillScreen(TFT_BLACK);

  lcd.setTextColor(TFT_GREEN, TFT_BLACK);
  lcd.setTextSize(4);

  lcd.setCursor(20,20);
  lcd.println("Connecting WiFi...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  lcd.fillScreen(TFT_BLACK);
  lcd.setCursor(20,20);
  lcd.println("WiFi Connected!");

  // گرفتن زمان از اینترنت
  configTime(gmtOffset_sec, daylightOffset_sec,
             "pool.ntp.org",
             "time.nist.gov");

  struct tm timeinfo;

  lcd.setCursor(20,70);
  lcd.print("Syncing Time...");

  while (!getLocalTime(&timeinfo)) {
    delay(500);
    Serial.println("Waiting for NTP...");
  }

  lcd.fillScreen(TFT_BLACK);
}

void loop()
{
    static int lastSecond = -1;

    struct tm timeinfo;

    // نمایش ساعت
    if (getLocalTime(&timeinfo))
    {
        if (timeinfo.tm_sec != lastSecond)
        {
            lastSecond = timeinfo.tm_sec;

            lcd.fillRect(0, 80, 480, 80, TFT_BLACK);

            lcd.setTextColor(TFT_GREEN, TFT_BLACK);
            lcd.setTextSize(5);
            lcd.setCursor(30, 100);

            lcd.printf("%02d:%02d:%02d",
                       timeinfo.tm_hour,
                       timeinfo.tm_min,
                       timeinfo.tm_sec);
        }
    }

    // تست تاچ
    uint16_t x, y;

    if (lcd.getTouch(&x, &y))
    {
        Serial.printf("Touch: X=%d  Y=%d\n", x, y);

        lcd.fillCircle(x, y, 5, TFT_RED);

        lcd.fillRect(0, 0, 240, 30, TFT_BLACK);
        lcd.setCursor(0, 0);
        lcd.setTextSize(2);
        lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
        lcd.printf("X:%3d  Y:%3d", x, y);

        delay(15);
    }
}