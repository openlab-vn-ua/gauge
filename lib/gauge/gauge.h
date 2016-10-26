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
  int     min_value, max_value;
  
  virtual void draw(int color) = 0;
};

class GaugeRound : public Gauge
{
 public:
  GaugeRound();
  GaugeRound(int   centerX,    int centerY,     int radius,
             int   min_value,  int max_value,   int step,
             int   start_value,
             float str_angle,  float max_angle);

  enum Text_Orient{
    horizontal, vertical
  };
  
 protected:
  int   centerX, centerY;
  int   radius;
  int   arrowhead_begin_at_center, gauge_division_length, gauge_scale_step,
        space_btwn_arrw_divisn;
  float str_angle, end_angle;
  char  suffix;
  Text_Orient txt_or_abt_gauge;

  void draw          (int color);
  void drawScale     (int color);
  void drawArrowhead (int color);
  void drawTextData  (int color);
};

#endif
