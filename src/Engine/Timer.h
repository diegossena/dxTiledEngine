#pragma once
#include "Engine.h"

using namespace std::chrono;

class Timer
{
  std::chrono::steady_clock::time_point oTime = steady_clock::now();
  steady_clock::time_point fTime;
  float pDeltaTime;
  float pUnscaledDeltaTime;

public:
  // Getters
  const float &deltaTime = pDeltaTime;
  // Methods
  void update()
  {
    fTime = steady_clock::now();
    duration<float> elapsedTime = fTime - oTime;
    pDeltaTime = elapsedTime.count();
  }
  void reset()
  {
    oTime = steady_clock::now();
  }
} timer;