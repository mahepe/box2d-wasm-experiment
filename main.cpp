#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WIDTH 800
#define HEIGHT 600
#define IMG_PATH "assets/sprite.png"

int main (int argc, char *argv[]) {

// variable declarations
SDL_Window *win = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *img = NULL;
int w, h; // texture width & height

// Initialize SDL.
if (SDL_Init(SDL_INIT_VIDEO) < 0)
      return 1;

// create the window and renderer
// note that the renderer is accelerated
win = SDL_CreateWindow("Image Loading", 100, 100, WIDTH, HEIGHT, 0);
renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

// load our image
img = IMG_LoadTexture(renderer, IMG_PATH);
SDL_QueryTexture(img, NULL, NULL, &w, &h); // get the width and height of the texture
// put the location where we want the texture to be drawn into a rectangle
// I'm also scaling the texture 2x simply by setting the width and height
SDL_Rect texr; texr.x = WIDTH/2; texr.y = HEIGHT/2; texr.w = w*2; texr.h = h*2;

// main loop
while (1) {

// event handling
SDL_Event e;
if ( SDL_PollEvent(&e) ) {
      if (e.type == SDL_QUIT)
              break;
      else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
              break;
}

// clear the screen
SDL_RenderClear(renderer);
// copy the texture to the rendering context
SDL_RenderCopy(renderer, img, NULL, &texr);
// flip the backbuffer
// this means that everything that we prepared behind the screens is actually shown
SDL_RenderPresent(renderer);

}

SDL_DestroyTexture(img);
SDL_DestroyRenderer(renderer);
SDL_DestroyWindow(win);

return 0;
}
