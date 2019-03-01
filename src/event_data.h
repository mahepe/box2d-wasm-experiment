#pragma once

class EventData {
public:
  bool quit = false;
  std::vector<int> arrow_keys = std::vector<int>{0, 0};

  EventData(SDL_Event e) {
    if (e.type == SDL_QUIT)
      this->quit = true;
    else if (e.type == SDL_KEYDOWN)
      this->arrow_keys = resolve_arrow_keys(e);
  }

  std::vector<int> resolve_arrow_keys(SDL_Event e) {
    std::map<SDL_Keycode, std::vector<int>> k_to_v{
        {SDLK_UP, std::vector<int>{0, 1}},
        {SDLK_DOWN, std::vector<int>{0, -1}},
        {SDLK_LEFT, std::vector<int>{-1, 0}},
        {SDLK_RIGHT, std::vector<int>{1, 0}}};
    return k_to_v[e.key.keysym.sym];
  }
};
