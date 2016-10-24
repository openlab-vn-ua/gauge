#include <gauge.h>

//----------------------------------------------

void drawCircle(int centerX, int centerY, int radius,
                float at_angle_deg, float to_angle_deg,
                OLED *screen, int color)
{
  double at_angle_rad = at_angle_deg / 180.0 * PI;
  double to_angle_rad = to_angle_deg / 180.0 * PI;

  void (OLED::*drawFunc)(uint16_t, uint16_t);

  if(color == 0)
    drawFunc = &OLED::clrPixel;
  else if(color == 1)
    drawFunc = &OLED::setPixel;
  
  for(float curr_angle = at_angle_deg; curr_angle <= to_angle_deg; curr_angle += 1)
  {
    int x = radius * cos(curr_angle / 180.0 * PI);
    int y = radius * sin(curr_angle / 180.0 * PI);
    (screen->*drawFunc)(centerX + x, centerY - y);
  }
}

//----------------------------------------------

int Gauge::getMinimum()
{
  return min;
}

int  Gauge::getMaximum()
{
  return max;
}

int  Gauge::getValue()
{
  return value;
}

void Gauge::setValue(int value)
{
  draw(0);
  this->value = value;
  this->value = constrain(this->value, min, max);
  draw(1);
}

bool Gauge::isVisible()
{
  return visible;
}

void Gauge::setVisible(bool visible)
{
  this->visible = visible;
}


void Gauge::attach(Canvas *canvas)
{
  this->canvas = canvas;
  canvas->addCanvasUser();
}

void Gauge::detach()
{
  canvas->removeCanvasUser();
  canvas = NULL;
}

bool Gauge::isAttached ()
{
  return (canvas == NULL) ? false : true;
}

//----------------------------------------------

GaugeRound::GaugeRound(): Gauge()
{
  centerX = 0;
  centerY = 0;
  radius = 0;
  min = 0;
  max = 0;
  step = 0;
  value = 0;
  minAngle = 0;
  maxAngle = 0;

  visible = true;
}

GaugeRound::GaugeRound(int   centerX,    int centerY,    int radius,
                       int   min,        int max,        int step,
                       int   startValue,
                       float minAngle,   float maxAngle)
{
  this->centerX = centerX;
  this->centerY = centerY;
  
  this->radius = radius;
  
  this->min = min;
  this->max = max;
  
  this->step = step;
  
  this->value = startValue;
  
  this->minAngle = minAngle;
  this->maxAngle = maxAngle;

  visible = true;
}

void GaugeRound::draw(int color)
{
  canvas->beginPaint();
  
  OLED *screen = canvas->getScreen();
  drawCircle(centerX, centerY, radius, minAngle, maxAngle, screen, color);
  drawScale(color);
  drawArrowhead(color);

  canvas->endPaint();
}

void GaugeRound::drawScale(int color)
{
  OLED *screen = canvas->getScreen();
  
  int l_radius = radius - 5;
  
  int   scale_stick_number = abs(abs(min) + abs(max)) / step;
  float scale_stick_angle  = abs(abs(minAngle) + abs(maxAngle)) / scale_stick_number;

  void (OLED::*drawFunc)(int, int, int, int);

  if(color == 0)
  {
    drawFunc = &OLED::clrLine;
  }
  else if(color == 1)
  {
    drawFunc = &OLED::drawLine;
  }

  for(int i = 0; i <= scale_stick_number; ++i)
  {
    float angle = (minAngle + scale_stick_angle * i) / 180 * PI;
    (screen->*drawFunc)(centerX + radius   * cos(angle),
                        centerY - radius   * sin(angle),
                        centerX + l_radius * cos(angle),
                        centerY - l_radius * sin(angle));
  }
}

void GaugeRound::drawArrowhead(int color)
{
  OLED *screen = canvas->getScreen();

  void (OLED::*drawFunc)(int, int, int, int);

  if(color == 0)
  {
    drawFunc = &OLED::clrLine;
  }
  else if(color == 1)
  {
    drawFunc = &OLED::drawLine;
  }

  int   b_radius = radius - 10;
  int   l_radius = 10;
  float angle    = map(value, min, max, maxAngle, minAngle);
  
  //Serial.print("angle="); Serial.println(angle);
  //Serial.print("value="); Serial.println(value);

  angle = angle / 180 * PI;

  (screen->*drawFunc)(centerX + b_radius * cos(angle),
                      centerY - b_radius * sin(angle),
                      centerX + l_radius * cos(angle),
                      centerY - l_radius * sin(angle));
}

