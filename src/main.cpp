#include <Box2D/Box2D.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <memory>
#include <stdio.h>
#include <vector>

#include "clock.h"
#include "const.h"
#include "deleter.h"
#include "game_state.h"
#include "utils.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define IMG_PATH "assets/sprite.png"

std::vector<int> screen_size{SCREEN_WIDTH, SCREEN_HEIGHT};

static bool run = true;
std::unique_ptr<b2World> world;

Clock timer;
std::unique_ptr<SDL_Interface> interface;
GameState *game_state;

void update() {
  // Tick the timer and simulate physics.
  timer.tick();
  world->Step(timer.delta, 1, 1);

  // Loop through SDL_Events
  SDL_Event e;
  if (SDL_PollEvent(&e)) {

    // Parse the SDL_Event to our custom event object.
    std::unique_ptr<EventData> event_data =
        std::unique_ptr<EventData>(new EventData(e));

    // Loop through objects
    for (auto const &o : game_state->objects) {
      o->handle_event(event_data.get());
    }

    // Break from the main loop if quit received.
    run = !event_data->quit;
  }
}

void draw() {

  // Clear the screen.
  SDL_RenderClear(interface->renderer.get());

  // Invoke render() of each GameObject.
  for (auto const &o : game_state->objects) {
    o->render(interface.get(), game_state->camera->GetPosition(), screen_size);
  }

  // Flip the buffers.
  SDL_RenderPresent(interface->renderer.get());
}

// The main loop.
void main_loop() {

  update();
  draw();
}

// The initialization code.
int main(int argc, char *argv[]) {

  // Init SDL.
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    return 1;

  // Create the SDL window and renderer.
  interface = std::unique_ptr<SDL_Interface>(new SDL_Interface);
  interface->window = std::unique_ptr<SDL_Window, Deleter>(SDL_CreateWindow(
      "Image Loading", 100, 100, screen_size.at(0), screen_size.at(1), 0));
  interface->renderer =
      std::unique_ptr<SDL_Renderer, Deleter>(SDL_CreateRenderer(
          interface->window.get(), -1, SDL_RENDERER_ACCELERATED));

  // Init a new game state.
  game_state = new GameState;
  b2Vec2 gravity(0.0f, 0.0f);
  world = std::unique_ptr<b2World>(new b2World(gravity));

  // Create the player.
  game_state->objects.push_back(std::unique_ptr<GameObject>(
      new Player(IMG_LoadTexture(interface->renderer.get(), IMG_PATH),
                 create_body(world.get()))));

  // Create the obstacles.
  for (int i = 0; i < 3; i++) {
    game_state->objects.push_back(std::unique_ptr<GameObject>(new GameObject(
        IMG_LoadTexture(interface->renderer.get(), IMG_PATH),
        create_body(world.get(), b2_kinematicBody, b2Vec2(0, 10 * (i - 1))))));
  }

  // Create the camera.
  game_state->camera = std::unique_ptr<b2Body, Deleter>(
      create_body(world.get(), b2_kinematicBody, b2Vec2(0, 0), false));

  // Tick the timer.
  timer.tick();

  // Start the main loop.
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
