#include <OLED_I2C.h>
#include <gauge.h>

OLED myOLED(SDA, SCL, 8);

#define FONT SmallFont
extern uint8_t FONT[];

Canvas canvas(&myOLED);

GaugeRound gauge1(31, 31, 32,
                  -20, 20, 3,
                  0,
                  -45, 225);

GaugeRound gauge2(95, 31, 32,
                  -20, 20, 3,
                  0,
                  135, 405);
                  
void setup()
{
   Serial.begin(9600);
  
   myOLED.begin();
   myOLED.setFont(FONT);

   gauge1.attach(&canvas);
   gauge2.attach(&canvas);

   gauge1.setValue(0);
   gauge2.setValue(0);

   gauge1.setNumbFontSize(FONT[0], FONT[1]);
   gauge2.setNumbFontSize(FONT[0], FONT[1]);

   
}

void loop()
{
  for(int i = -20; i <= 20; ++i)
  {
    gauge1.setValue(i);
    gauge2.setValue(-i);
  }
  for(int i = 20; i >= -20; --i)
  {
    gauge1.setValue(i);
    gauge2.setValue(-i);
  }
}
