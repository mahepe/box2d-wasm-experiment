#pragma once
#include <SDL2/SDL.h>

struct Clock {
  uint32_t last_tick_time = 0;
  float delta = 0;

  void tick() {
    uint32_t tick_time = SDL_GetTicks();
    delta = ((float)(tick_time - last_tick_time)) / 1000;
    last_tick_time = tick_time;
  }
};
