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

void drawFillRect(OLED *screen, int x1, int y1, int x2, int y2, int color)
{
  if(x1 > x2)
  {
    int temp = x1;
    x1 = x2; x2 = temp;
  }
  
  void (OLED::*drawFunc)(int, int, int, int);

  if(color == 0)
  {
    drawFunc = &OLED::clrLine;
  }
  else if(color == 1)
  {
    drawFunc = &OLED::drawLine;
  }

  int l = abs(y1 - y2);
  for(int i = 0; i < (x2 - x1); ++i)
  {
    (screen->*drawFunc)(x1 + i, y1, x1 + i, y2);
  }
}

//----------------------------------------------

int Gauge::getMinimum()
{
  return min_value;
}

int  Gauge::getMaximum()
{
  return max_value;
}

int  Gauge::getValue()
{
  return value;
}

void Gauge::setValue(int value)
{
  if(visible)
  {
    this->last_value = this->value;
    this->value      = value;
    this->value      = constrain(this->value, min_value, max_value);
    redraw();
  }
  else
  {
    this->value = value;
    this->value = constrain(this->value, min_value, max_value);
  }
}

bool Gauge::isVisible()
{
  return visible;
}

void Gauge::setVisible(bool visible)
{
  if(visible)
  {
    this->visible = visible;
  }
  else
  {
    canvas->beginPaint();
    draw(0);
    canvas->endPaint();
    this->visible = visible;
  }
    
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

void Gauge::redraw()
{
  canvas->beginPaint();
  
  draw(0);
  draw(1);
  
  canvas->endPaint();
}

//----------------------------------------------

GaugeRound::GaugeRound(): Gauge()
{
  centerX = 0;
  centerY = 0;
  radius = 0;
  min_value = 0;
  max_value = 0;
  gauge_scale_step = 0;
  value = 0;
  str_angle = 0;
  end_angle = 0;

  visible = true;
  arrowhead_r_begin_at_center = 0;
  gauge_division_length = 0;
  space_btwn_arrw_divisn = 0;
  suffix = '0';
  txt_or_abt_gauge = 0;
}

GaugeRound::GaugeRound(int   centerX,    int centerY,    int radius,
                       int   min_value,  int max_value,  int gauge_scale_step,
                       int   start_value,
                       float str_angle,  float end_angle)
{
  this->centerX = centerX;
  this->centerY = centerY;
  
  this->radius = radius;
  
  this->min_value = min_value;
  this->max_value = max_value;
  
  this->gauge_scale_step = gauge_scale_step;
  
  this->value = start_value;
  
  this->str_angle = str_angle;
  this->end_angle = end_angle;

  visible = true;
  arrowhead_r_begin_at_center = radius / 2.5;
  gauge_division_length = radius / 8;
  space_btwn_arrw_divisn = gauge_division_length;
  suffix = 'A';
  txt_or_abt_gauge = horizontal;
}

void GaugeRound::setNumbFontSize(int size_x, int size_y)
{
  numb_font_size_x = size_x;
  numb_font_size_y = size_y;

  char_font_size_x = size_x;
  char_font_size_y = size_y;
  
  calculateTextPositions();
}

void GaugeRound::getNumbFontSizeX(void)
{
  return numb_font_size_x;
}

void GaugeRound::getNumbFontSizeY(void)
{
  return numb_font_size_y;
}

void GaugeRound::setCharFontSize(int size_x, int size_y)
{
  char_font_size_x = size_x;
  char_font_size_y = size_y;
}

void GaugeRound::getCharFontSizeX(void)
{
  return char_font_size_x;
}

void GaugeRound::getCharFontSizeY(void)
{
  return char_font_size_y;
}

void GaugeRound::calculateTextPositions()
{
  uint8_t seats_in_center = 0;

  float angle_rad = PI - asin( numb_font_size_y / arrowhead_r_begin_at_center / 2);

  numb_x = arrowhead_r_begin_at_center * cos(angle_rad);
  numb_y = -arrowhead_r_begin_at_center * sin(angle_rad);
}

void GaugeRound::draw(int color)
{ 
  OLED *screen = canvas->getScreen();
  //drawCircle(centerX, centerY, radius, str_angle, end_angle, screen, color);
  drawScale(color);
  drawArrowhead(color);
  drawTextData(color);
}

void GaugeRound::drawScale(int color)
{
  OLED *screen = canvas->getScreen();
  
  int l_radius = radius - gauge_division_length;

  int   scale_stick_number = scale_stick_number = abs(abs(min_value) + abs(max_value)) / gauge_scale_step;
  float scale_stick_angle = 0;
  if(str_angle < 0)
  {
    scale_stick_angle  = abs(abs(str_angle) + abs(end_angle)) / scale_stick_number;
  }
  else
  {
    scale_stick_angle  = abs(abs(str_angle) - abs(end_angle)) / scale_stick_number;
  }

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
    float angle = (str_angle + scale_stick_angle * i) / 180 * PI;
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

  float angle;
  if(color == 0)
  {
    drawFunc = &OLED::clrLine;
    angle = float(map(last_value, min_value, max_value, end_angle, str_angle)) / 180 * PI;
  }
  else if(color == 1)
  {
    drawFunc = &OLED::drawLine;
    angle = float(map(value, min_value, max_value, end_angle, str_angle)) / 180 * PI;
  }

  int   b_radius = radius - gauge_division_length - space_btwn_arrw_divisn;
  int   l_radius = arrowhead_r_begin_at_center;
  
  (screen->*drawFunc)(centerX + b_radius * cos(angle),
                      centerY - b_radius * sin(angle),
                      centerX + l_radius * cos(angle),
                      centerY - l_radius * sin(angle));
}

void GaugeRound::drawTextData  (int color)
{
  OLED *screen = canvas->getScreen();

  if(color == 0)
  {
    screen->clrCircle(centerX, centerY, arrowhead_r_begin_at_center);
    screen->clrRect(numb_x + centerX, numb_y + centerY,
                    numb_x + numb_font_size_x + centerX, numb_y + numb_font_size_y + centerY);
    //screen->clrRect(char_x + centerX, char_y + centerY,
    //              char_x + char_font_size_x + centerX, char_y + char_font_size_y + centerY);
  }
  else if(color == 1)
  {
    screen->drawCircle(centerX, centerY, arrowhead_r_begin_at_center);
    screen->drawRect(numb_x + centerX, numb_y + centerY,
                     numb_x + numb_font_size_x + centerX, numb_y + numb_font_size_y + centerY);
    //screen->drawRect(char_x + centerX, char_y + centerY,
    //               char_x + char_font_size_x + centerX, char_y + char_font_size_y + centerY);
  }
  /*
  if(color == 0)
  {
    drawFillRect(screen,
                 numb_x + centerX, numb_y + centerY,
                 numb_x + numb_font_size_x * 3 + centerX, numb_y + numb_font_size_y + centerY,
                 0);
  }
  else if(color == 1)
  {
    if(value < 0)
    {
      screen->printNumI(value, numb_x + centerX, numb_y + centerY);
    }
    else
    {
      screen->printNumI(value, numb_x + centerX  + numb_font_size_x, numb_y + centerY);
    }
  }
  */
  //screen->print(&suffix, char_x, char_y);
}
