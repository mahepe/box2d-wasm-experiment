#pragma once
#include <SDL2/SDL.h>

// This interfaces the smart pointers with SDL's destruction methods.
struct Deleter {
  inline void operator()(SDL_Window* window) {
    SDL_DestroyWindow(window);
  }

  inline void operator()(SDL_Texture* texture) {
    SDL_DestroyTexture(texture);
  }

  inline void operator()(SDL_Renderer* renderer) {
    SDL_DestroyRenderer(renderer);
  }

  inline void operator()(b2Body* body){
    body -> GetWorld() -> DestroyBody(body);
  }
};
