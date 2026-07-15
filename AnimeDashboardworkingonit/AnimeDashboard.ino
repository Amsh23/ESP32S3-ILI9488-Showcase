#include "LGFX_ILI9488.h"

#include <WiFi.h>

#include "config.h"
#include "Clock.h"
#include "esp_heap_caps.h"


#include "AnimeRSS.h"
#include "cat.h"

LGFX display;



// RSS حرکت
int rssX = 480;

uint32_t rssTimer = 0;



void setup()
{

  Serial.begin(115200);



  // =====================
  // PSRAM
  // =====================

  if(psramFound())
  {
    Serial.println("PSRAM OK");
    Serial.println(ESP.getPsramSize());
  }
  else
  {
    Serial.println("NO PSRAM");
  }



  // =====================
  // LCD
  // =====================

  display.init();

  display.setRotation(1);

  display.fillScreen(TFT_BLACK);



  // =====================
  // CAT
  // =====================



  display.pushImage(
    150,
    150,
    CAT_WIDTH,
    CAT_HEIGHT,
    cat
  );


  Serial.println("CAT OK");



  // =====================
  // WIFI
  // =====================

  WiFi.begin(
    WIFI_SSID,
    WIFI_PASS
  );


  while(WiFi.status() != WL_CONNECTED)
  {
    delay(300);
    Serial.print(".");
  }


  Serial.println();
  Serial.println("WiFi Connected");



  // =====================
  // RSS
  // =====================

  getAnimeRSS();


  rssX = 480;



  // =====================
  // CLOCK
  // =====================

  initClock();


}





void loop()
{

  static uint32_t clockTimer = 0;



  // =====================
  // CLOCK UPDATE
  // =====================

  if(millis() - clockTimer > 1000)
  {

    clockTimer = millis();



    // پاک کردن ساعت

    display.fillRect(
      0,
      180,
      480,
      110,
      TFT_BLACK
    );



    display.setTextDatum(
      middle_center
    );



    // TIME

    display.setTextColor(
      TFT_CYAN
    );


    display.drawString(
      getTimeNow(),
      240,
      215,
      7
    );



    // DATE

    display.setTextColor(
      TFT_WHITE
    );


    display.drawString(
      getDateNow(),
      240,
      270,
      3
    );

  }




  // =====================
  // RSS MARQUEE
  // =====================


  if(millis() - rssTimer > 40)
  {

    rssTimer = millis();



    display.fillRect(
      0,
      300,
      480,
      20,
      TFT_BLACK
    );



    display.setTextDatum(
      middle_left
    );


    display.setTextColor(
      TFT_YELLOW
    );



    display.drawString(
      animeNews,
      rssX,
      310,
      2
    );



    rssX -= 2;



    // وقتی از صفحه خارج شد

    int width = animeNews.length() * 12;


    if(rssX < -width)
    {
      rssX = 480;
    }


  }



}