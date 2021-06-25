#pragma once
/* Declarations
----------------*/
class Rect
{
public:
  float xo, xf, yo, yf;
  Rect(float x1,
             float x2,
             float y1,
             float y2) : xo(x1), xf(x2), yo(y1), yf(y2) {}
};