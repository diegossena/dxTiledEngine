#include "settings.h"
/* Declarations
----------------*/
int main()
{
  const float refreshRate = .01666666754f; // 60 FPS
  Timer timer;
  Texture2D tilesTexture("resources/tiles.png", ISize(821, 347), 32);
  std::vector<std::string> level = {
      "..................................................................................................................................................................................................................",
      "..................................................................................................................................................................................................................",
      "..................................................................................................................................................................................................................",
      "..................................................................................................................................................................................................................",
      "..................................................................................................................................................................................................................",
      "......................?...........................................................................................................................................................................................",
      "..................................................................................................................................................................................................................",
      "..................................................................................................................................................................................................................",
      "..................................................................................................................................................................................................................",
      "................?...B?B?B.........................................................................................................................................................................................",
      "..................................................................................................................................................................................................................",
      "..................................................................................................................................................................................................................",
      "................................................................................................................................................................................................................R.",
      "RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR..RRRRRRRRRRRRRRR...RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR",
      "RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR..RRRRRRRRRRRRRRR...RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR"};
  /* Get Tile
  ------------*/
  float frameCounter = 0;
  unsigned short animatedTile = 0;
  auto getTile = [&](unsigned y, unsigned x) {
    char tileId = level[y][x];
    switch (tileId)
    {
    case '.':
      return Rect(.002436053706f, .002436053706f, .03746397793f, .03746397793f);
    case 'R':
      return Rect(.001218026853f, .02070645615f, .3775216043f, .4236311316f);
    case 'B':
      return Rect(.02192448266f, .04141291231f, .2795388997f, .325648427f);
    case '?':
      switch (animatedTile)
      {
      case 0:
        return Rect(.001218026853f, .02070645615f, .181556195f, .2276657075f);
      case 1:
        return Rect(.02192448266f, .04141291231f, .181556195f, .2276657075f);
      }
    }
    return Rect(0.f, 0.f, 0.f, 0.f);
  };
  //
  auto drawTiles = [&]() {
    //Vector2 tileOffset(screenOffset.x / tileSizeX, screenOffset.y / zoom);
  };
  Vector2 offset(0.f, 0.f);
  auto inputHandler = [&]() {
    if (KeyBoard::isKeyPressed(Key::Up))
    {
      offset.y -= 10.0f * timer.deltaTime;
    }
    else if (KeyBoard::isKeyPressed(Key::Down))
    {
      offset.y += 10.0f * timer.deltaTime;
    }
    if (KeyBoard::isKeyPressed(Key::Right))
    {
      offset.x += 10.0f * timer.deltaTime;
    }
    else if (KeyBoard::isKeyPressed(Key::Left))
    {
      offset.x -= 10.0f * timer.deltaTime;
    }
    offset.x = std::clamp(offset.x, 0.f, level[0].size() - 2 / window.tileSize.x);
    offset.y = std::clamp(offset.y, 0.f, level.size() - 2 / window.tileSize.y);
  };
  float tileSize = 0.2f;
  while (window.isOpen())
  {
    timer.update();
    if (timer.deltaTime < refreshRate)
      continue;
    inputHandler();
    timer.reset();
    graphics.clearRender(fColor::Black);
    /* Level Render
    ----------------*/
    frameCounter += timer.deltaTime / refreshRate;
    unsigned frameCounterAux = frameCounter;
    if (frameCounterAux > 15)
    {
      animatedTile++;
      frameCounter = 0;
      if (animatedTile > 1)
        animatedTile = 0;
    }
    {
      unsigned tileY = offset.y;
      float yI = (offset.y - tileY) * window.tileSize.y + 1.f;
      float yF;
      do
      {
        yF = yI - window.tileSize.y;
        //
        unsigned tileX = offset.x;
        float xI = (tileX - offset.x) * window.tileSize.x - 1.f;
        float xF;
        do
        {
          xF = xI + window.tileSize.x;
          tilesTexture.rectangle(Rect(xI, xF, yI, yF), getTile(tileY, tileX++));
          xI = xF;
        } while (xF <= .999f);
        tileY++;
        yI = yF;
      } while (yF >= -.999f);
    }
    tilesTexture.draw();
    graphics.render();
  }
  return 0;
}
