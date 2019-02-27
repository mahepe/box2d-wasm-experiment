#include <stdio.h>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <Box2D/Box2D.h>

#include "clock.h"
#include "deleter.h"
#include "game_state.h"
#include "utils.h"

// If the build target is web, include emscripten
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define WIDTH 800
#define HEIGHT 600
#define IMG_PATH "assets/sprite.png"

// Smart pointers for our SDL things.
std::unique_ptr<SDL_Window, Deleter> window;
std::unique_ptr<SDL_Renderer, Deleter> renderer;
std::unique_ptr<SDL_Texture, Deleter> texture;
std::unique_ptr<SDL_Rect> texture_rect = std::unique_ptr<SDL_Rect>(new SDL_Rect());

// Flow control
static bool run = true;

// Smart pointers to Box2D things
std::unique_ptr<b2World> world;

Clock timer;
GameState * game_state;

void main_loop(){
  timer.tick();
  world -> Step(timer.delta, 1, 1);
  SDL_Event e;
  if ( SDL_PollEvent(&e) ) {
    if (e.type == SDL_QUIT)
      run = false;
    else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
      run = false;
  }

  texture_rect->x = game_state->player_body->GetPosition().x;
  // Clear, render sprite and swap buffers.
  SDL_RenderClear(renderer.get());
  SDL_RenderCopy(renderer.get(), texture.get(), NULL, texture_rect.get());
  SDL_RenderPresent(renderer.get());
}

int main (int argc, char *argv[]) {
  game_state = new GameState;
  b2Vec2 gravity(0.0f, 0.0f);
  world = std::unique_ptr<b2World>(new b2World(gravity));
  game_state->player_body = create_body(world.get());
  game_state->player_body->SetLinearVelocity(b2Vec2(10.0f, 0.0f));

  int w, h; // These will hold the sprite dimensions.

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    return 1;

  // Create the window, renderer, and load the sprite.
  window = std::unique_ptr<SDL_Window, Deleter>(SDL_CreateWindow("Image Loading", 100, 100, WIDTH, HEIGHT, 0));
  renderer = std::unique_ptr<SDL_Renderer, Deleter>(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED));
  texture = std::unique_ptr<SDL_Texture, Deleter>(IMG_LoadTexture(renderer.get(), IMG_PATH));
  SDL_QueryTexture(texture.get(), NULL, NULL, &w, &h);
  texture_rect->x = 0;
  texture_rect->y = 0;
  texture_rect->w = w;
  texture_rect->h = h;

  timer.tick();

  // If the build target is web, hand the main loop over to emscripten...
#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(main_loop, 0, 1);
  // ...otherwise just run it here
#else
  while (run) {
    main_loop();
  }
#endif

  delete game_state;
  return 0;
}
