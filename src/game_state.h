#pragma once
#include <memory>
#include <Box2D/Box2D.h>

#include "deleter.h"

struct GameObject{
  std::unique_ptr<b2Body, Deleter> body;
  std::unique_ptr<SDL_Texture, Deleter> texture;
  std::unique_ptr<SDL_Rect> texture_rect;
};

struct GameState{
  std::unique_ptr<GameObject> player;
  std::unique_ptr<b2Body, Deleter> camera;
};
