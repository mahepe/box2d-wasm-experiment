#include <stdio.h>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// If the build target is web, include emscripten
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define WIDTH 800
#define HEIGHT 600
#define IMG_PATH "assets/sprite.png"

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
};

// Smart pointers for our SDL things.
std::unique_ptr<SDL_Window, Deleter> window;
std::unique_ptr<SDL_Renderer, Deleter> renderer;
std::unique_ptr<SDL_Texture, Deleter> texture;
std::unique_ptr<SDL_Rect> texture_rect = std::unique_ptr<SDL_Rect>(new SDL_Rect());
static bool run = true;

void main_loop(){
    SDL_Event e;
    if ( SDL_PollEvent(&e) ) {
      if (e.type == SDL_QUIT)
        run = false;
      else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
        run = false;
    }

    // Clear, render sprite and swap buffers.
    SDL_RenderClear(renderer.get());
    SDL_RenderCopy(renderer.get(), texture.get(), NULL, texture_rect.get());
    SDL_RenderPresent(renderer.get());
}

int main (int argc, char *argv[]) {

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

// If the build target is web, hand the main loop over to emscripten...
#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(main_loop, 0, 1);
// ...otherwise just run it here
#else
  while (run) {
    main_loop();
  }
#endif

  // Notice the lack of cleanup code as we use smart pointers. :)

  return 0;
}
