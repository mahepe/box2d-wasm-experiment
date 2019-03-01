#pragma once
#include <Box2D/Box2D.h>
#include <memory>
#include <vector>

#include "deleter.h"

struct SDL_interface {
  std::unique_ptr<SDL_Window, Deleter> window;
  std::unique_ptr<SDL_Renderer, Deleter> renderer;
};

std::unique_ptr<b2Body, Deleter> create_body(b2World *world,
                                             b2BodyType type = b2_dynamicBody,
                                             b2Vec2 x = b2Vec2(0, 0),
                                             bool createFixture = true) {
  b2BodyDef bodyDef;
  bodyDef.type = type;
  bodyDef.position.Set(x.x, x.y);

  b2PolygonShape dynamicBox;
  dynamicBox.SetAsBox(1.0f, 1.0f);

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &dynamicBox;
  fixtureDef.density = 1.0f;
  fixtureDef.friction = 0.3f;

  b2Body *body = world->CreateBody(&bodyDef);
  if (createFixture)
    body->CreateFixture(&fixtureDef);
  return std::unique_ptr<b2Body, Deleter>(body);
}

// Map world coordinates to screen coordinates.
std::vector<int> world_to_screen(b2Vec2 x, b2Vec2 camera_x, int px_per_unit,
                                 std::vector<int> screen_size) {
  return std::vector<int>{
      (int)(((float)screen_size.at(0)) / 2 + px_per_unit * (x.x - camera_x.x)),
      (int)(((float)screen_size.at(1)) / 2 - px_per_unit * (x.y - camera_x.y))};
}

std::unique_ptr<SDL_Rect> rect_for_texture(SDL_Texture *texture) {
  SDL_Rect *rect = new SDL_Rect;

  int w, h;
  SDL_QueryTexture(texture, NULL, NULL, &w, &h);
  rect->x = 0;
  rect->y = 0;
  rect->w = w;
  rect->h = h;

  return std::unique_ptr<SDL_Rect>(rect);
}

b2Vec2 resolve_arrow_keys(SDL_Event e) {
  std::map<SDL_Keycode, b2Vec2> k_to_v{{SDLK_UP, b2Vec2(0, 1)},
                                       {SDLK_DOWN, b2Vec2(0, -1)},
                                       {SDLK_LEFT, b2Vec2(-1, 0)},
                                       {SDLK_RIGHT, b2Vec2(1, 0)}};
  return k_to_v[e.key.keysym.sym];
}

struct EventData {
  EventData(void) {
    this->quit = false;
    this->arrow_keys = b2Vec2(0, 0);
  }
  bool quit;
  b2Vec2 arrow_keys;
};

std::unique_ptr<EventData> parse_event(SDL_Event e) {
  EventData *out = new EventData;
  if (e.type == SDL_QUIT)
    out->quit = true;
  else if (e.type == SDL_KEYDOWN)
    out->arrow_keys = resolve_arrow_keys(e);
  return std::unique_ptr<EventData>(out);
}

void handle_event(GameState *game_state, EventData *event_data) {
  b2Vec2 vel2 = event_data->arrow_keys;
  b2Vec2 vel1 = game_state->objects["player"]->body->GetLinearVelocity();
  auto v = vel2 + vel1;
  game_state->objects["player"]->body->SetLinearVelocity({v.x, v.y});
}

void render_game_objects(GameState *game_state, SDL_Renderer *renderer,
                         std::vector<int> screen_size) {
  for (auto const &object : game_state->objects) {
    std::vector<int> tmp =
        world_to_screen(object.second->body->GetPosition(),
                        game_state->camera->GetPosition(), 24, screen_size);
    object.second->texture_rect->x = tmp.at(0);
    object.second->texture_rect->y = tmp.at(1);
    SDL_RenderCopy(renderer, object.second->texture.get(), NULL,
                   object.second->texture_rect.get());
  }
}
