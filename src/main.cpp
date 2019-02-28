#include <stdio.h>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <Box2D/Box2D.h>
#include <vector>

#include "clock.h"
#include "deleter.h"
#include "game_state.h"
#include "utils.h"

// If the build target is web, include emscripten
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define IMG_PATH "assets/sprite.png"

std::vector<int> screen_size {800, 600};

// Smart pointers for our SDL things.
std::unique_ptr<SDL_Window, Deleter> window;
std::unique_ptr<SDL_Renderer, Deleter> renderer;

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
    else if (e.type == SDL_KEYDOWN)
     game_state->player->body->SetLinearVelocity(b2Vec2(1.0f, 0.0f));
    else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
      run = false;
  }

  std::vector<int> tmp = world_to_screen(game_state->player->body->GetPosition(),
                                         game_state->camera->GetPosition(),
                                         24,
                                         screen_size);
  game_state->player->texture_rect->x = tmp.at(0);
  game_state->player->texture_rect->y = tmp.at(1);
  // Clear, render sprite and swap buffers.
  SDL_RenderClear(renderer.get());
  SDL_RenderCopy(renderer.get(),
                 game_state->player->texture.get(),
                 NULL,
                 game_state->player->texture_rect.get());
  SDL_RenderPresent(renderer.get());
}

int main (int argc, char *argv[]) {

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    return 1;

  // Create the window, renderer, and load the sprite.
  window = std::unique_ptr<SDL_Window, Deleter>(SDL_CreateWindow("Image Loading", 100, 100, screen_size.at(0), screen_size.at(1), 0));
  renderer = std::unique_ptr<SDL_Renderer, Deleter>(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED));

  game_state = new GameState;
  b2Vec2 gravity(0.0f, 0.0f);
  world = std::unique_ptr<b2World>(new b2World(gravity));
  game_state->player = std::unique_ptr<GameObject>(new GameObject);
  game_state->player->texture = std::unique_ptr<SDL_Texture, Deleter>(IMG_LoadTexture(renderer.get(), IMG_PATH));
  game_state->player->body = create_body(world.get());
  game_state->player->texture_rect = std::unique_ptr<SDL_Rect>(new SDL_Rect());

  int w, h; // These will hold the sprite dimensions.
  SDL_QueryTexture(game_state->player->texture.get(), NULL, NULL, &w, &h);
  game_state->player->texture_rect->x = 0;
  game_state->player->texture_rect->y = 0;
  game_state->player->texture_rect->w = w;
  game_state->player->texture_rect->h = h;

  game_state->camera = create_body(world.get(), b2_kinematicBody, b2Vec2(-4, 0), false);
  game_state->camera->SetLinearVelocity(b2Vec2(2.0f, 0.0f));

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
