#pragma once
#include <Box2D/Box2D.h>
#include <map>
#include <memory>
#include <string>

#include "const.h"
#include "deleter.h"
#include "event_data.h"
#include "utils.h"

/*
 * This base class represents any physical object in
 * the game world.
 */
class GameObject {
public:
  std::unique_ptr<b2Body, Deleter> body;
  std::unique_ptr<SDL_Texture, Deleter> texture;
  std::unique_ptr<SDL_Rect> texture_rect;

  GameObject(SDL_Texture *texture, b2Body *body) {
    this->texture = std::unique_ptr<SDL_Texture, Deleter>(texture);
    this->body = std::unique_ptr<b2Body, Deleter>(body);
    this->texture_rect = std::unique_ptr<SDL_Rect>(rect_for_texture(texture));
  }

  void render(SDL_Interface *interface, b2Vec2 camera_position,
              std::vector<int> screen_size) {
    std::vector<int> tmp = world_to_screen(
        this->body->GetPosition(), camera_position, PX_PER_UNIT, screen_size);
    this->texture_rect->x = tmp.at(0);
    this->texture_rect->y = tmp.at(1);
    SDL_RenderCopy(interface->renderer.get(), this->texture.get(), NULL,
                   this->texture_rect.get());
  }

  SDL_Rect *rect_for_texture(SDL_Texture *texture) {
    b2AABB aabb;
    aabb.lowerBound = b2Vec2(FLT_MAX, FLT_MAX);
    aabb.upperBound = b2Vec2(-FLT_MAX, -FLT_MAX);
    b2Fixture *fixture = this->body->GetFixtureList();
    aabb.Combine(aabb, fixture->GetAABB(0));
    auto l = aabb.lowerBound;
    auto u = aabb.upperBound;
    int w = (int)(PX_PER_UNIT * (u.x - l.x));
    int h = (int)(PX_PER_UNIT * (u.y - l.y));
    return new SDL_Rect{0, 0, w, h};
  }

  // Overload this to provide a subclass-specific event handler.
  virtual void handle_event(EventData *event_data) {}

  static std::vector<int> world_to_screen(b2Vec2 x, b2Vec2 camera_x,
                                          int px_per_unit,
                                          std::vector<int> screen_size) {
    return std::vector<int>{(int)(((float)screen_size.at(0)) / 2 +
                                  px_per_unit * (x.x - camera_x.x)),
                            (int)(((float)screen_size.at(1)) / 2 -
                                  px_per_unit * (x.y - camera_x.y))};
  }
};

/*
 * This class represents the player.
 */
class Player : public GameObject {
public:
  Player(SDL_Texture *texture, b2Body *body) : GameObject(texture, body) {}

  void handle_event(EventData *event_data) override {
    std::vector<int> keys = event_data->arrow_keys;
    b2Vec2 vel2 = b2Vec2(keys.at(0), keys.at(1));
    b2Vec2 vel1 = this->body->GetLinearVelocity();
    auto v = vel2 + vel1;
    this->body->SetLinearVelocity({v.x, v.y});
  }
};
