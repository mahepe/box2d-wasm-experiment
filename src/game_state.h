#pragma once
#include <Box2D/Box2D.h>
#include <map>
#include <memory>
#include <string>

#include "deleter.h"

struct GameObject {
  std::unique_ptr<b2Body, Deleter> body;
  std::unique_ptr<SDL_Texture, Deleter> texture;
  std::unique_ptr<SDL_Rect> texture_rect;
};

struct GameState {
  std::map<std::string, std::unique_ptr<GameObject>> objects;
  std::unique_ptr<b2Body, Deleter> camera;
};
