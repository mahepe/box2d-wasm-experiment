#pragma once
#include <Box2D/Box2D.h>
#include <memory>

#include "deleter.h"

std::unique_ptr<b2Body, Deleter> create_body(b2World * world){
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(0.0f, 4.0f);

  b2PolygonShape dynamicBox;
  dynamicBox.SetAsBox(1.0f, 1.0f);

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &dynamicBox;
  fixtureDef.density = 1.0f;
  fixtureDef.friction = 0.3f;

  b2Body * body = world->CreateBody(&bodyDef);
  body->CreateFixture(&fixtureDef);
  return std::unique_ptr<b2Body, Deleter>(body);
}
