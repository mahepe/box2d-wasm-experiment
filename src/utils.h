#pragma once
#include <Box2D/Box2D.h>
#include <memory>
#include <vector>

#include "deleter.h"

std::unique_ptr<b2Body, Deleter> create_body(b2World * world,
                                             b2BodyType type = b2_dynamicBody,
                                             b2Vec2 x = b2Vec2(0,0),
                                             bool createFixture = true){
  b2BodyDef bodyDef;
  bodyDef.type = type;
  bodyDef.position.Set(x.x, x.y);

  b2PolygonShape dynamicBox;
  dynamicBox.SetAsBox(1.0f, 1.0f);

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &dynamicBox;
  fixtureDef.density = 1.0f;
  fixtureDef.friction = 0.3f;

  b2Body * body = world->CreateBody(&bodyDef);
  if(createFixture)
    body->CreateFixture(&fixtureDef);
  return std::unique_ptr<b2Body, Deleter>(body);
}

// Map world coordinates to screen coordinates.
std::vector<int> world_to_screen(b2Vec2 x,
                                 b2Vec2 camera_x,
                                 int px_per_unit,
                                 std::vector<int> screen_size){
  return std::vector<int> {(int) (((float) screen_size.at(0))/2+px_per_unit*(x.x-camera_x.x)),
                             (int) (((float) screen_size.at(1))/2-px_per_unit*(x.y-camera_x.y))};
}
