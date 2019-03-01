#include <Box2D/Box2D.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <memory>
#include <stdio.h>
#include <vector>

#include "clock.h"
#include "deleter.h"
#include "game_state.h"
#include "utils.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define IMG_PATH "assets/sprite.png"

std::vector<int> screen_size{800, 600};

static bool run = true;
std::unique_ptr<b2World> world;

Clock timer;
std::unique_ptr<SDL_interface> interface;
GameState *game_state;

void main_loop() {
  timer.tick();
  world->Step(timer.delta, 1, 1);

  SDL_Event e;
  if (SDL_PollEvent(&e)) {
    std::unique_ptr<EventData> event_data = parse_event(e);
    handle_event(game_state, event_data.get());
    run = !event_data->quit;
  }

  SDL_RenderClear(interface->renderer.get());
  render_game_objects(game_state, interface->renderer.get(), screen_size);
  SDL_RenderPresent(interface->renderer.get());
}

int main(int argc, char *argv[]) {

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    return 1;

  interface = std::unique_ptr<SDL_interface>(new SDL_interface);
  interface->window = std::unique_ptr<SDL_Window, Deleter>(SDL_CreateWindow(
      "Image Loading", 100, 100, screen_size.at(0), screen_size.at(1), 0));
  interface->renderer =
      std::unique_ptr<SDL_Renderer, Deleter>(SDL_CreateRenderer(
          interface->window.get(), -1, SDL_RENDERER_ACCELERATED));

  game_state = new GameState;
  b2Vec2 gravity(0.0f, 0.0f);
  world = std::unique_ptr<b2World>(new b2World(gravity));
  game_state->objects["player"] = std::unique_ptr<GameObject>(new GameObject);
  game_state->objects["player"]->texture =
      std::unique_ptr<SDL_Texture, Deleter>(
          IMG_LoadTexture(interface->renderer.get(), IMG_PATH));
  game_state->objects["player"]->body = create_body(world.get());
  game_state->objects["player"]->texture_rect =
      rect_for_texture(game_state->objects["player"]->texture.get());
  game_state->camera =
      create_body(world.get(), b2_kinematicBody, b2Vec2(-4, 0), false);
  timer.tick();

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(main_loop, 0, 1);
#else
  while (run) {
    main_loop();
  }
#endif

  delete game_state;
  return 0;
}
