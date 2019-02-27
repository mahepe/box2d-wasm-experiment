#pragma once
#include <memory>
#include <Box2D/Box2D.h>

#include "deleter.h"

struct GameState{
  std::unique_ptr<b2Body, Deleter> player_body;
};
