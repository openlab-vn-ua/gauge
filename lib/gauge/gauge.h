#ifndef __GAUGE_HEADER_FILE__
#define __GAUGE_HEADER_FILE__

#include <OLED_I2C.h>
#include <canvas.h>

class Gauge
{
 public:
  virtual int  getMinimum ();
  virtual int  getMaximum ();
  virtual int  getValue   ();
  virtual void setValue   (int value); // should redraw
  virtual bool isVisible  ();
  virtual void setVisible (bool visible);

  void attach     (Canvas *);
  void detach     ();
  bool isAttached ();
 protected:
  Canvas *canvas;
  bool    visible;
  int     value;
  int     min, max, step;
  
  virtual void draw(int color) = 0;
};

class GaugeRound : public Gauge
{
 public:
  GaugeRound();
  GaugeRound(int centerX,    int centerY,     int radius,
             int min,        int max,         int step,
             int startValue,
             float minAngle, float maxAngle);
  
 protected:
  int   centerX, centerY;
  int   radius;
  float minAngle, maxAngle;

  void draw          (int color);
  void drawScale     (int color);
  void drawArrowhead (int color);
};

#endif
