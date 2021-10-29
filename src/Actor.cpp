#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

void Actor::Update(float deltaTime)
{
  ;
}

Actor::Actor(Game *game)
{
  mGame = game;
  mState = EActive;
  mScale = 1.0f;
  mRotation = 0.0f;
}

void Actor::AddComponent(Component *component)
{
  mComponents.emplace_back(component);
}

Actor::~Actor()
{
  ;
}

void Actor::UpdateActor(float deltaTime)
{
  ;
}
