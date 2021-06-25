#pragma once
// Representation of 3D vectors and points
class Vector3
{
public:
  float x, y, z;
  Vector3() {}
  Vector3(float &oX, float &oY) : x(oX), y(oY) { z = 0.f; }
  Vector3(float &oX, float &oY, float &oZ) : x(oX), y(oY), z(oZ) {}
  Vector3(float oX, float oY, float oZ) : x(oX), y(oY), z(oZ) {}
};