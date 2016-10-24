#include <OLED_I2C.h>
#include <gauge.h>

OLED myOLED(SDA, SCL, 8);
extern uint8_t SmallFont[];

Canvas canvas(&myOLED);
/*
GaugeRound gauge1(64, 32, 32,
                  -10, 10, 2,
                  0,
                  -60, 240);
*/

GaugeRound gauge1(64, 31, 32,
                  -10, 10, 2,
                  0,
                  -60, 240);
                  
void setup()
{
   Serial.begin(9600);
  
   myOLED.begin();
   myOLED.setFont(SmallFont);

   gauge1.attach(&canvas);

   gauge1.setValue(0);
   delay(1000);
}

void loop()
{
  for(int i = -10; i <= 10; ++i)
  {
     gauge1.setValue(i);
     delay(100);
  }
}
