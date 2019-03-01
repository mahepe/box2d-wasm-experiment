#pragma once
#include "game_object.h"

struct GameState {
public:
  std::vector<std::unique_ptr<GameObject>> objects;
  std::unique_ptr<b2Body, Deleter> camera;
};
