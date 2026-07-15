#include <Adafruit_NeoPixel.h>

#define RGB_PIN     48      // ESP32-S3 DevKitC-1
#define NUMPIXELS   1

Adafruit_NeoPixel rgb(NUMPIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);


//==============================
// Setup
//==============================
void setup()
{
  rgb.begin();
  rgb.setBrightness(255);
  rgb.clear();
  rgb.show();

  randomSeed(micros());
}


//==============================
// Loop
//==============================
void loop()
{
  rainbow();

  breathingBlue();

  police();

  fadeRGB();

  randomColors();
}



//==============================
// Rainbow
//==============================
void rainbow()
{
  for (int j = 0; j < 256; j++)
  {
    rgb.setPixelColor(
      0,
      rgb.gamma32(rgb.ColorHSV(j * 256))
    );

    rgb.show();

    delay(8);
  }
}



//==============================
// Breathing Blue
//==============================
void breathingBlue()
{
  for (int b = 0; b <= 255; b++)
  {
    rgb.setBrightness(b);

    rgb.setPixelColor(
      0,
      0,
      0,
      255
    );

    rgb.show();

    delay(5);
  }

  for (int b = 255; b >= 0; b--)
  {
    rgb.setBrightness(b);

    rgb.setPixelColor(
      0,
      0,
      0,
      255
    );

    rgb.show();

    delay(5);
  }

  rgb.setBrightness(255);
}



//==============================
// Police
//==============================
void police()
{
  for (int i = 0; i < 20; i++)
  {
    rgb.setPixelColor(
      0,
      255,
      0,
      0
    );

    rgb.show();

    delay(70);

    rgb.clear();
    rgb.show();

    delay(40);

    rgb.setPixelColor(
      0,
      0,
      0,
      255
    );

    rgb.show();

    delay(70);

    rgb.clear();
    rgb.show();

    delay(40);
  }
}



//==============================
// Fade RGB
//==============================
void fadeRGB()
{
  for (int i = 0; i <= 255; i++)
  {
    rgb.setPixelColor(
      0,
      i,
      255 - i,
      0
    );

    rgb.show();

    delay(5);
  }

  for (int i = 0; i <= 255; i++)
  {
    rgb.setPixelColor(
      0,
      255 - i,
      0,
      i
    );

    rgb.show();

    delay(5);
  }

  for (int i = 0; i <= 255; i++)
  {
    rgb.setPixelColor(
      0,
      0,
      i,
      255 - i
    );

    rgb.show();

    delay(5);
  }
}



//==============================
// Random Colors
//==============================
void randomColors()
{
  for (int i = 0; i < 50; i++)
  {
    rgb.setPixelColor(
      0,
      random(256),
      random(256),
      random(256)
    );

    rgb.show();

    delay(120);
  }
}