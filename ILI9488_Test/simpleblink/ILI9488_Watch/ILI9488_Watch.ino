#include "LGFX_ILI9488.h"


LGFX display;



unsigned long lastAnim = 0;

int frame = 0;



void setup()
{

Serial.begin(115200);


Serial.println("START");


display.init();


display.setRotation(1);


display.fillScreen(TFT_BLACK);



display.setTextColor(TFT_WHITE);

display.setTextSize(3);


display.drawString(
"ILI9488 WATCH",
60,
30
);



Serial.println("LCD OK");

}



void loop()
{


// ----------------
// ANIMATION
// ----------------

if(millis()-lastAnim > 500)
{

lastAnim = millis();


frame++;


if(frame>1)
frame=0;



// پاک کردن بخش ساعت

display.fillRect(
0,
80,
480,
240,
TFT_BLACK
);



// چشم باز

if(frame==0)
{


display.fillCircle(
160,
160,
40,
TFT_WHITE
);


display.fillCircle(
320,
160,
40,
TFT_WHITE
);


}


// چشم بسته

else
{


display.fillRect(
120,
160,
80,
10,
TFT_WHITE
);


display.fillRect(
280,
160,
80,
10,
TFT_WHITE
);


}



}


// ----------------
// TOUCH
// ----------------


uint16_t x,y;


if(display.getTouch(&x,&y))
{


Serial.print("X:");
Serial.print(x);

Serial.print(" Y:");
Serial.println(y);



display.fillCircle(
x,
y,
8,
TFT_RED
);


}



}