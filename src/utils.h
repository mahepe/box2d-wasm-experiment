#pragma once
#include <Box2D/Box2D.h>
#include <memory>
#include <vector>

#include "deleter.h"

struct SDL_Interface {
  std::unique_ptr<SDL_Window, Deleter> window;
  std::unique_ptr<SDL_Renderer, Deleter> renderer;
};

b2Body *create_body(b2World *world, b2BodyType type = b2_dynamicBody,
                    b2Vec2 x = b2Vec2(0, 0), bool createFixture = true) {
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
  return body;
}
