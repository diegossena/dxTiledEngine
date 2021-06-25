#pragma once
/* Local Dependencies
----------------------*/
#include "Engine.h"
/* Declarations
----------------*/
class Vertex
{
public:
  Vector3 position;
  Vector2 Texture2D;
  Vertex() {}
  Vertex(Vector3 pos, Vector2 tex) : position(pos), Texture2D(tex) {}
};