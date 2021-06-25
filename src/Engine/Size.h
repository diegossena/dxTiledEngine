#pragma once
// (unsigned short width, unsigned short height)
class USSize
{
public:
  // Props
  unsigned short width, height;
  // Methods
  USSize(unsigned short width_, unsigned short height_) : width(width_), height(height_)
  {
  }
};
// (unsigned int width, unsigned int height)
class USize
{
public:
  // Props
  unsigned int width, height;
  // Methods
  USize(unsigned int width_, unsigned int height_) : width(width_), height(height_)
  {
  }
};
// (int width, int height)
class ISize
{
public:
  // Props
  int width, height;
  // Methods
  ISize(int width_, int height_) : width(width_), height(height_)
  {
  }
};